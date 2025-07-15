/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/



/*
THE FOLLOWING ADDITIONAL MODIFICATIONS ARE REQUIRED TO USE THIS FILE:
1. in Makefile, add "freeze.o" to the list of files (name this
file "freeze.c").

2. in merc.h add the following at the bottom of struct char_data:
    long		tag_flags;

and then add the following lines:
#define TAG_PLAYING		(A)
#define TAG_FROZEN		(B)
#define TAG_RED			(C)
#define TAG_BLUE			(D)

3. in interp.h, add in the list of functions:
DECLARE_DO_FUN( do_ftag );
DECLARE_DO_FUN( do_tag );
DECLARE_DO_FUN( do_red );
DECLARE_DO_FUN( do_blue );

4. in interp.c, add in the list of miscellaneous commands:
{ "ftag",	do_ftag,	POS_SLEEPING,	 IM, LOG_NORMAL, 1 },
{ "tag",	do_tag,	POS_RESTING,	 0,  LOG_NORMAL, 1 },
{ "red",	do_red,	POS_RESTING,	 0,  LOG_NORMAL, 1 },
{ "blue",	do_blue,	POS_RESTING,	 0,  LOG_NORMAL, 1 },

5. in act_info.c modify the do_who and show_char_to_char_0
functions as required to display "RED" or "BLUE" tags on players.
For example, add an extra %s and the following in do_who:
	    IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {x" : 
	    (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {x" : ""),
and extra if statements in show_char_to_char_0:
    if ( IS_SET(ch->tag_flags,TAG_RED) ) strcat( buf,"{R[RED] {x"  );
    if ( IS_SET(ch->tag_flags,TAG_BLUE)) strcat( buf,"{B[BLUE] {x" );

6. add the following in act_move.c in move_char(), between other
checks for reasons not to move the character
    if ( IS_SET(ch->tag_flags, TAG_FROZEN)
    &&   IS_SET(ch->tag_flags, TAG_PLAYING))
    {
	send_to_char( "You've been frozen, you can't move!\n\r", ch );
	return;
    }

7. other small changes may be needed for your MUD.  For example,
Clandestine has "shadowform" characters which cannot be seen by normal
mortals.  The can_see() function was edited to ensure that freeze tag
players can see each other.  Also, you should edit do_switch or any
other function which puts a player in a mob's body to remove that
player's TAG_PLAYING bit to avoid problems.

8. create an area to use for your tag game.  A 7 x 7 square grid works
well for teams of about 5 each.  Use consecutive room vnums and enter
them in the #define statements below.
*/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* 
9. set these VNUMs to the min/max room number to randomly locate players
in the freeze tag zone 
*/
#define FTAG_MIN_VNUM		50800
#define FTAG_MAX_VNUM		50829

DECLARE_DO_FUN( do_transfer );

void do_red( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf [MAX_STRING_LENGTH];

#ifdef DEBUG
        Debug ("do_red");
#endif

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  red <message>\n\r", ch );
	return;
    }

    if (!IS_IMMORTAL(ch) && !IS_SET(ch->tag_flags,TAG_PLAYING))
    {
	send_to_char( "You must be a freeze tag player to use this channel.\n\r", ch );
	return;
    }

    if (IS_SET(ch->tag_flags,TAG_RED) || IS_IMMORTAL(ch) )
    {
	sprintf(buf,"#1{RED}#4 %s#n: %s\n\r",ch->name,argument);
	send_to_char(buf, ch);
    }
    else	
    {
	stc( "You aren't on the red team.\n\r",ch);
	return;
    }

    for ( d = first_descriptor; d != NULL; d = d->next )
    {
	if ( (d->connected == CON_PLAYING)
      && (IS_SET(d->character->tag_flags,TAG_RED) || IS_IMMORTAL(d->character)) && d->character != ch) 
	{
	  send_to_char(buf, d->character);
	}
    }
}

void do_blue( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf [MAX_STRING_LENGTH];

#ifdef DEBUG
        Debug ("do_blue");
#endif

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax:  blue <message>\n\r", ch );
	return;
    }

    if (!IS_IMMORTAL(ch) && !IS_SET(ch->tag_flags,TAG_PLAYING))
    {
	send_to_char( "You must be a freeze tag player to use this channel.\n\r", ch );
	return;
    }

    if (IS_SET(ch->tag_flags,TAG_BLUE) || IS_IMMORTAL(ch) )
    {
	sprintf(buf,"#4{BLUE}#1 %s#n: %s\n\r",ch->name,argument);
	send_to_char(buf, ch);
    }
    else	
    {
	stc( "You aren't on the blue team.\n\r",ch);
	return;
    }

    for ( d = first_descriptor; d != NULL; d = d->next )
    {
	if ( (d->connected == CON_PLAYING)
      && (IS_SET(d->character->tag_flags,TAG_BLUE) || IS_IMMORTAL(d->character)) && d->character != ch ) 
	{
	  send_to_char(buf, d->character);
	}
    }
}

void check_team_frozen ( CHAR_DATA *ch )
{
    DESCRIPTOR_DATA *d;

#ifdef DEBUG
        Debug ("check_team_frozen");
#endif

    if (IS_SET(ch->tag_flags,TAG_BLUE))
    {
	for ( d = first_descriptor; d != NULL; d = d->next )
	{
	  if ( (d->connected == CON_PLAYING)
        &&   IS_SET(d->character->tag_flags,TAG_PLAYING)
        &&   IS_SET(d->character->tag_flags,TAG_BLUE)
	  &&   !IS_NPC(d->character)
        &&   !IS_SET(d->character->tag_flags,TAG_FROZEN)) 
	  {
	    return;
	  }
	}
	do_info(ch,"The #1RED#n team has won FREEZE TAG!!!");
    return;
    }

    if (IS_SET(ch->tag_flags,TAG_RED))
    {
	for ( d = first_descriptor; d != NULL; d = d->next )
	{
	  if ( (d->connected == CON_PLAYING)
        &&   IS_SET(d->character->tag_flags,TAG_PLAYING)
        &&   IS_SET(d->character->tag_flags,TAG_RED)
	  &&   !IS_NPC(d->character)
        &&   !IS_SET(d->character->tag_flags,TAG_FROZEN)) 
	  {
            return;

	  }
	}
	do_info(ch,"The #4BLUE#n team has won FREEZE TAG!!!");

    return;
    }
}

void do_ftag( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    bool fRed = FALSE;

#ifdef DEBUG
        Debug ("do_ftag");
#endif

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax:  ftag reset\n\r", ch );
	send_to_char( "         ftag start\n\r", ch );
	send_to_char( "         ftag auto (splits everyone in room onto teams)\n\r", ch );
	send_to_char( "         ftag red <player>\n\r", ch );
	send_to_char( "         ftag blue <player>\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "reset" ) )
    {
	for ( d = first_descriptor; d != NULL; d = d->next )
	{
	  if (d->connected != CON_PLAYING)
	    continue;

	  if ( IS_SET(d->character->tag_flags,TAG_PLAYING))
	  {
	    REMOVE_BIT(d->character->tag_flags,TAG_PLAYING);
	    if (!IS_NPC(d->character))
	    {
		sprintf(buf,"%s %d",d->character->name,ROOM_VNUM_TEMPLE);
		do_transfer(ch,buf);
	    }
	  }
	  if ( IS_SET(d->character->tag_flags,TAG_FROZEN))
	    REMOVE_BIT(d->character->tag_flags,TAG_FROZEN);
	  if ( IS_SET(d->character->tag_flags,TAG_RED))
	    REMOVE_BIT(d->character->tag_flags,TAG_RED);
	  if ( IS_SET(d->character->tag_flags,TAG_BLUE))
	    REMOVE_BIT(d->character->tag_flags,TAG_BLUE);
        send_to_char( "Freeze tag has been stopped!\n\r", d->character );
	}
	send_to_char( "All players reset.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "start" ) )
    {
	for ( d = first_descriptor; d != NULL; d = d->next )
	{
	  if (d->connected != CON_PLAYING)
	    continue;

	  if ( IS_SET(d->character->tag_flags,TAG_PLAYING)
		 && !IS_NPC(d->character))
	  {
	    sprintf(buf,"%s %d",d->character->name,
		number_range(FTAG_MIN_VNUM,FTAG_MAX_VNUM));
	    REMOVE_BIT(d->character->tag_flags,TAG_FROZEN);
	    do_transfer(ch,buf);
	    send_to_char( "Freeze Tag has started!\n\r", d->character );
	  }
	}
	send_to_char( "You have started a game of Freeze Tag.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "auto" ) )
    {
	for ( victim = ch->in_room->people; victim != NULL; 
			victim = victim->next_in_room )
	{
	  if ( victim == ch || IS_NPC(victim))
	    continue;

	  if ((fRed = !fRed))
	  {
	    sprintf(buf,"red %s",victim->name);
	  }
	  else
	  {
	    sprintf(buf,"blue %s",victim->name);
	  }

	  do_ftag(ch,buf);
	}
	return;
    }

    argument = one_argument( argument, arg2 );
    if ( arg2[0] == '\0' ||
	   (str_cmp( arg1, "red" ) &&
	    str_cmp( arg1, "blue" )))
    {
	send_to_char( "Syntax:  ftag reset\n\r", ch );
	send_to_char( "         ftag start\n\r", ch );
	send_to_char( "         ftag auto (splits everyone in room onto teams)\n\r", ch );
	send_to_char( "         ftag red <player>\n\r", ch );
	send_to_char( "         ftag blue <player>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "red" ) )
    {
	SET_BIT(victim->tag_flags,TAG_PLAYING);
	REMOVE_BIT(victim->tag_flags,TAG_FROZEN);
	SET_BIT(victim->tag_flags,TAG_RED);
	REMOVE_BIT(victim->tag_flags,TAG_BLUE);
	act( "You are on the #1RED#n team!", ch, NULL, victim, TO_VICT, FALSE );
	act( "$N is on the #1RED#n team!",   ch, NULL, victim, TO_NOTVICT, FALSE );
	act( "$N is on the #1RED#n team!",   ch, NULL, victim, TO_CHAR   , FALSE );
    }
    else
    if ( !str_cmp( arg1, "blue" ) )
    {
	SET_BIT(victim->tag_flags,TAG_PLAYING);
	REMOVE_BIT(victim->tag_flags,TAG_FROZEN);
	SET_BIT(victim->tag_flags,TAG_BLUE);
	REMOVE_BIT(victim->tag_flags,TAG_RED);
	act( "You are on the #4BLUE#n team!", ch, NULL, victim, TO_VICT, FALSE );
	act( "$N is on the #4BLUE#n team!",   ch, NULL, victim, TO_NOTVICT, FALSE );
	act( "$N is on the #4BLUE#n team!",   ch, NULL, victim, TO_CHAR   , FALSE );
    }

    return;
}

void do_ztag( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

#ifdef DEBUG
        Debug ("do_tag");
#endif

    argument = one_argument( argument, arg );

    if (!IS_SET(ch->tag_flags,TAG_PLAYING))
    {
	send_to_char( "You're not playing freeze tag.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Tag whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "You tag yourself.  How amusing.\n\r", ch );
	return;
    }

    if (!IS_SET(victim->tag_flags,TAG_PLAYING))
    {
	send_to_char( "They're not playing freeze tag.\n\r", ch );
	return;
    }

    if (IS_SET(ch->tag_flags,TAG_FROZEN))
    {
	send_to_char( "You can't tag, you're frozen!\n\r", ch );
	return;
    }

    act( "$n tags you.", ch, NULL, victim, TO_VICT , FALSE);
    act( "$n tags $N.",  ch, NULL, victim, TO_NOTVICT , FALSE);
    act( "You tag $N.",  ch, NULL, victim, TO_CHAR    , FALSE);

    if ((IS_SET(ch->tag_flags,TAG_RED) &&
         IS_SET(victim->tag_flags,TAG_RED))
	||(IS_SET(ch->tag_flags,TAG_BLUE) &&
         IS_SET(victim->tag_flags,TAG_BLUE)))
    {
	if (IS_SET(victim->tag_flags,TAG_FROZEN))
	{
	  REMOVE_BIT(victim->tag_flags,TAG_FROZEN);
	  act( "You are no longer frozen!", ch, NULL, victim, TO_VICT , FALSE);
	  act( "$N is no longer frozen!",   ch, NULL, victim, TO_NOTVICT , FALSE);
	  act( "$N is no longer frozen!",   ch, NULL, victim, TO_CHAR    , FALSE);
	}
	else
	{
	  act( "$N is not frozen!",   ch, NULL, victim, TO_CHAR    , FALSE);
	}
    }
    else
    {
	if (IS_SET(victim->tag_flags,TAG_FROZEN))
	{
	  act( "$N is already frozen!",   ch, NULL, victim, TO_CHAR    , FALSE);
	}
	else
	{
	  SET_BIT(victim->tag_flags,TAG_FROZEN);
	  act( "You are frozen!", ch, NULL, victim, TO_VICT, FALSE );
	  act( "$N is frozen!",   ch, NULL, victim, TO_NOTVICT , FALSE);
	  act( "$N is frozen!",   ch, NULL, victim, TO_CHAR    , FALSE);
	  check_team_frozen( victim );
	}
    }

    return;
}


