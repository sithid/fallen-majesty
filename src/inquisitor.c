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
 *  Inquisitor by Rand, aka Dominic Jones
 *
 *  12.28.02
 *  Finished: 01.01.03
 *
 *  All work contained herewithin is the property of Rand, and is to be used exclusively
 *  by Fallen Majesty (janksmud.genesismuds.com 9009).
 *
 *  This original inquisitor class is not to be placed on any other mud other than Fallen
 *  Majesty, nor is it to be implemented whatsoever without the express permission of
 *  Rand.
 *
 *  Rand_BFZ@yahoo.com
 *  AIM: KingDen420
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "player.h"


void 
do_unholyritual (CHAR_DATA *ch, char *argument)
{


  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];

  argument=one_argument(argument,arg1);
  strcpy(arg2,argument);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_INQUISITOR))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0]== '\0')
  {

    if(ch->pcdata->powers[PINQUISITOR] > 0)
      send_to_char("* You possess the knowledge of stealing souls.\n\r",ch);
    if(ch->pcdata->powers[PINQUISITOR] > 1)
      send_to_char("* You know how to use pain to purify the wicked.\n\r",ch);
    if(ch->pcdata->powers[PINQUISITOR] > 2)
      send_to_char("* You are possess the unholy rage.\n\r",ch);
    if(ch->pcdata->powers[PINQUISITOR] > 3)
      send_to_char("* You may inquire on those you are curious about.\n\r",ch);
    if(ch->pcdata->powers[PINQUISITOR] > 4)
      send_to_char("* You are very skilled in the art of torture.\n\r",ch);
    if(ch->pcdata->powers[PINQUISITOR] > 5)
      send_to_char("* You delight in tormenting the infidel.\n\r",ch);
    if(ch->pcdata->powers[PINQUISITOR] > 7)
      send_to_char("* You have the strength of the Inquisition.\n\r",ch);
    if(ch->pcdata->powers[PINQUISITOR] > 8)
      send_to_char("* Your cursed chains will bind the infidel.\n\r",ch);
    if(ch->pcdata->powers[PINQUISITOR] > 9)
      send_to_char("* Your fury is unmatched.\n\r",ch);
    sprintf(buf,"Unholy rituals learned (%d).\n\r", ch->pcdata->powers[PINQUISITOR]);
    send_to_char(buf,ch);
    if(ch->pcdata->powers[PINQUISITOR] < 10)
    stc("Syntax: unholyritual learn",ch);
    return;  
  }
  if (!str_cmp(arg1,"learn"))
  {
    int cost = (ch->pcdata->powers[PINQUISITOR]+1) * 200;
	if ( ch->pcdata->powers[PINQUISITOR] >= 10 )
    {
      send_to_char("You have learned as much as you are able.\n\r",ch);
      return;
    }
    else if (cost > ch->practice)
    {
      send_to_char("You don't have enough primal.\n\r",ch);
      return;
    }
    
    if (ch->pcdata->powers[PINQUISITOR] < 9)
    {
       ch->pcdata->powers[PINQUISITOR] +=1;
       ch->practice -= cost;
       send_to_char("You feel enlightened, and eager to learn more!\n\r",ch);
       return;
    }

    if (ch->pcdata->powers[PINQUISITOR] == 9)
    {
      if (ch->pcdata->souls < 10)
      {
       stc("You do not have the 10 souls required.\n\r",ch);
       return;
      }
      else
      {
       ch->pcdata->powers[PINQUISITOR] +=1;
       ch->practice -= cost;
       if (ch->pcdata->powers[PINQUISITOR] == 10)
       ch->pcdata->souls -= 10;
       send_to_char("You feel enlightened, and eager to learn more!\n\r",ch);
       return;
      }
    }
  }
}

void do_enlist(CHAR_DATA *ch, char *argument)
{
if (!xIS_SET(ch->extra, EXTRA_POLICY))
  {
        stc("You cannot enlist until you accept the policy. (help policy).\n\r",ch);
        return;
  }

    if (ch->class != 0)
	{
	  stc("You are already classed, poor fool.\n\r",ch);
	  return;
	}
    if (ch->exp < 100000000000)
	{
	  stc("100 billion exp is required as a sacrifice.\n\r",ch);
	  return;
	}
    if (ch->max_hit < 300000)
	{
	  stc("300k hp is required as a sacrifice.\n\r",ch);
	  return;
	}
    if (ch->max_mana < 300000)
        {
          stc("300k mana is required as a sacrifice.\n\r",ch);
          return;
        }
    if (ch->max_hit < 300000)
        {
          stc("300k hp is required as a sacrifice.\n\r",ch);
          return;
        }
    if (ch->max_move < 300000)
        {
          stc("300k move is required as a sacrifice.\n\r",ch);
          return;
        }
    if (ch->pcdata->quest < 20000)
        {
          stc("20k QP is required as a sacrifice.\n\r",ch);
          return;
        }
    if (ch->practice < 20000)
	{
	  stc("20k primal is required as a sacrifice.\n\r",ch);
	  return;
	}
    else
        {
          ch->class = CLASS_INQUISITOR;
          stc("You are now an inquisitor!\n\r",ch);
          ch->alignment = -1000;
/*	  ch->max_hit = 200000;
	  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
	  ch->max_mana = 200000;
          if (ch->mana > ch->max_mana) ch->mana = ch->max_mana;
	  ch->max_move = 200000;
          if (ch->move > ch->max_move) ch->move = ch->max_move;
*/
	  ch->practice -= 20000;
	  ch->pcdata->quest -= 20000;
	  ch->exp -= 100000000000;
	  do_save(ch,"");
	  return;
        }
  return;
}


void do_purify(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim = NULL;
    char buf[MAX_STRING_LENGTH];
	int dam;
	int heal;
    if (!IS_CLASS(ch, CLASS_INQUISITOR))
    {
		stc("Huh?\n\r",ch);
		return;
    }
    if (ch->pcdata->powers[PINQUISITOR] < 2)
    {
	stc("You do not know how to purify anything.\n\r",ch);
	return;
    }

    if (argument[0] == '\0')
    {
	stc("Whom do you wish to purify?\n\r", ch);
	return;
    }

    if ((victim = get_char_room(ch, argument)) == NULL)
    {
	    stc("They aren't here.\n\r", ch);
	    return;
    }
    if (victim == ch)
    {
	stc("Thats... not safe.\n\r",ch);
	return;
    }
      if (is_safe(ch, victim)) return;

	WAIT_STATE(ch, 18);

	act("$n points at $N.", ch, NULL, victim, TO_NOTVICT, FALSE );
	act("You point at $N.", ch, NULL, victim, TO_CHAR, FALSE );
	act("$n points at you.", ch, NULL, victim, TO_VICT, FALSE );
	act("$N begins to scream at $n, writhing in pain.", ch, NULL, victim, TO_NOTVICT, FALSE );
	act("$N writhes in pain and screams at you!", ch, NULL, victim, TO_CHAR, FALSE );
	act("You scream at $n and writhe in pain.", ch, NULL, victim, TO_VICT, FALSE );

	  dam = 5000 + ch->damcap[0];
	  heal = dam / 2;

	if (victim->hit > 50000)
        { 
          if (dam < 0) dam = 1000;
	  victim->hit -= dam;
	  ch->hit += heal;
	  if (ch->hit > ch->max_hit)
	     { 
		  ch->hit = ch->max_hit;
	     }
	  sprintf(buf,"#0Your purification causes #7%d#0 damage.#n\n\r",dam);
	  stc(buf,ch);
	  sprintf(buf,"#7%s#0 hurts you for #7%d#0 points of damage!.#n\n\r",ch->name,dam);
          stc(buf,victim);
	  sprintf(buf,"#0You absorb #7%d #0hitpoints.#n\n\r",heal);
	  stc(buf,ch);
	  sprintf(buf,"#7%s#0 absorbed #7%d#0 of your hitpoints!#n\n\r",ch->name,heal);
	  stc(buf,victim);
	  set_fighting(ch,victim);
	}	  
        else stc("Nothing seemed to happen.\n\r",ch);
	return;
}

void do_torture( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if ( !IS_CLASS(ch, CLASS_INQUISITOR) )
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->pcdata->powers[PINQUISITOR] < 5 )
  {
    stc("You have not the will to torture.\n\r",ch);
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim) ) return;
  if ( victim == ch )
  {
    stc("I would not advise that.\n\r",ch);
    return;
  }
  WAIT_STATE(ch, 18);
  act("#0You place your hands on #7$N's #0head before you #7torture#0 $m.#n",ch,NULL,victim,TO_CHAR, FALSE );
  act("#7$n#n places #7$s#n hands on your head, and you scream in utter pain.#n",ch,NULL,victim,TO_VICT, FALSE );
  act("#7$n#0 places #7$s#0 hands on #7$N's#0 head and #7$N#0 screams in pain.#n",ch,NULL,victim,TO_ROOM, FALSE );
  SET_BIT(victim->special, SPC_TORTURE);
  set_fighting(ch, victim);
  return;
}
void do_torment(CHAR_DATA *ch, char *argument)
{

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_INQUISITOR) ) 
   {
    send_to_char("Huh?\n\r", ch );
    return;
   }
  if (ch->pcdata->powers[PINQUISITOR] < 6)
   {
	stc("You do not have the ability to torment the infidel.\n\r",ch);
	return;
   }
  else
   {
	  SET_BIT(ch->special, SPC_TORMENT);
	  stc("You will torment the wicked infidel.\n\r",ch);
	  return;
   }
  return;
}
void do_chains(CHAR_DATA *ch, char *argument)
{

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_INQUISITOR) ) 
   {
    send_to_char("Huh?\n\r", ch );
    return;
   }
  if (ch->pcdata->powers[PINQUISITOR] < 9)
   {
	stc("You do not have the ability to chain the infidel.\n\r",ch);
	return;
   }
  else
   {
	  SET_BIT(ch->special, SPC_CHAINS);
	  stc("You will arrest the wicked infidel's movement with unholy chains.\n\r",ch);
	  return;
   }
  return;
}
void do_unholyfury(CHAR_DATA *ch, char *argument)
{

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_INQUISITOR) ) 
   {
    send_to_char("Huh?\n\r", ch );
    return;
   }
  if (ch->pcdata->powers[PINQUISITOR] < 10)
   {
	stc("You do not possess the fury of the Inquisition.\n\r",ch);
	return;
   }
   if (ch->pcdata->souls < 1)
   {
	stc("You must sacrifice one soul to summon the unholy fury.\n\r",ch);
	return;
   }
   if (ch->bomb_timer > 0)
   {
	stc("You may not summon your unholy fury yet.\n\r",ch);
	return;
   }
   if (IS_SET(ch->special, SPC_FURY))
   {
	stc("You are already furious!\n\r",ch);
	return;
   }
  else
   {
	  SET_BIT(ch->special, SPC_FURY);
	  stc("#0You feel #7furious#0, let the infidel feel your wrath!#n\n\r",ch);
	  WAIT_STATE(ch,30);
	  ch->fury_timer = 20;
	  ch->bomb_timer = 60;
	  ch->pcdata->souls -= 1;
	  return;
   }
  return;
}
void do_staff( CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_INQUISITOR))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( 10000 > ch->practice)
  {
    send_to_char("It costs 10000 points of primal to create an Inquisitor's Staff.\n\r",ch);
    return;
  }
  if (IS_SET(ch->special, SPC_STAFF))
  {
    stc("You have already made your staff of the inquisitor.\n\r",ch);
    return;
  }
  ch->practice   -= 10000;
  obj = create_object(get_obj_index(33340) ,50 );
  SET_BIT(obj->quest, QUEST_RELIC);
  obj->condition = 100;
  obj->toughness = 100;
  obj->resistance = 1;
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj->questmaker = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  act("A staff suddenly appears in your hands.",ch,NULL,NULL,TO_CHAR, FALSE );
  act("A staff suddenly appears in $n's hands.",ch,NULL,NULL,TO_ROOM, FALSE );
  SET_BIT(ch->special, SPC_STAFF);
  return;
}

void do_walk( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_INQUISITOR) )
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Walk to whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not of this world.\n\r", ch );
	return;
    }
    

 if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  { 
    stc( "You can't find it's room.\n\r",ch);
    return; 
  } 
  if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You cannot sense any paths leading from this room.\n\r",ch);
    return;
  }

    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
	  return;
    }
    if ( room_is_private(victim->in_room ) )
    {
        send_to_char( "That room is private right now.\n\r", ch );
        return;
    }
    if ( !IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    {	
	send_to_char( "They don't want you near them.\n\r", ch );
        return;
    }
    if (victim->in_room == ch->in_room)
    {
	send_to_char("But you're already there!\n\r",ch);
        return;
    }

    act("You follow an unseen train towards $N.", ch, NULL, victim, TO_CHAR, FALSE );
    act("$n walks off somewhere, looking for something.", ch, NULL, victim, TO_ROOM, FALSE );
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    ch->move -= 1000;
    act("$n walks in from nowhwere in front of $N.", ch, NULL, victim, TO_NOTVICT, FALSE );
    act("$n steps in from nowhere in front of you.", ch, NULL, victim, TO_VICT, FALSE );
    do_look(ch,"auto");
    return;
}
