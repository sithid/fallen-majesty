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



/*
 * The following special functions are available for mobiles.
 */
DECLARE_SPEC_FUN( 	spec_rand_ft		);
DECLARE_SPEC_FUN(	spec_rand_quit		);
DECLARE_SPEC_FUN(	spec_rand_heals		);
DECLARE_SPEC_FUN(	spec_rand_new3		);
DECLARE_SPEC_FUN(	spec_rand_new2		);
DECLARE_SPEC_FUN(       spec_rand_new		);
DECLARE_SPEC_FUN(	spec_mobius_hunter	);
DECLARE_SPEC_FUN(	spec_mobius_original    );
DECLARE_SPEC_FUN(	spec_breath_any		);
DECLARE_SPEC_FUN(	spec_breath_acid	);
DECLARE_SPEC_FUN(	spec_breath_fire	);
DECLARE_SPEC_FUN(	spec_breath_frost	);
DECLARE_SPEC_FUN(	spec_breath_gas		);
DECLARE_SPEC_FUN(	spec_breath_lightning	);
DECLARE_SPEC_FUN(	spec_cast_adept		);
DECLARE_SPEC_FUN(	spec_cast_cleric	);
DECLARE_SPEC_FUN(	spec_cast_judge		);
DECLARE_SPEC_FUN(	spec_cast_mage		);
DECLARE_SPEC_FUN(	spec_cast_undead	);
DECLARE_SPEC_FUN(	spec_fido		);
DECLARE_SPEC_FUN(	spec_guard		);
DECLARE_SPEC_FUN(	spec_janitor		);
DECLARE_SPEC_FUN(	spec_mayor		);
DECLARE_SPEC_FUN(	spec_poison		);
DECLARE_SPEC_FUN(	spec_thief		);
DECLARE_SPEC_FUN(	spec_eater		);
DECLARE_SPEC_FUN(	spec_gremlin_original	);
DECLARE_SPEC_FUN(	spec_gremlin_born	);
DECLARE_SPEC_FUN(	spec_rogue		);
DECLARE_SPEC_FUN(	spec_zombie_lord	);
DECLARE_SPEC_FUN(	spec_dog		);
DECLARE_SPEC_FUN(	spec_guard_guardone	);
DECLARE_SPEC_FUN(	spec_guard_guardtwo	);
DECLARE_SPEC_FUN(	spec_guard_guardthree	);
DECLARE_SPEC_FUN(	spec_guard_guardfour	);
DECLARE_SPEC_FUN(	spec_guard_guardfive	);
DECLARE_SPEC_FUN(	spec_guard_guardsix	);
DECLARE_SPEC_FUN(	spec_guard_guardseven	);
DECLARE_SPEC_FUN(	spec_guard_guardeight	);
DECLARE_SPEC_FUN(	spec_guard_guardnine	);
DECLARE_SPEC_FUN(	spec_guard_guardten	);
DECLARE_SPEC_FUN(       spec_guard_guardfive    );

bool spec_guard_guardtwo(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
	   || victim->pcdata->kingdom == 2)
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[2].who_name);

        do_info( ch , buf);
      
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}
bool spec_guard_guardone(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;


   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
	   || victim->pcdata->kingdom == 1)
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[1].who_name); 
        do_info( ch , buf);
      
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}
bool spec_guard_guardthree(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   if (ch->in_room == NULL) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
	   || victim->pcdata->kingdom == 3)
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;
        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[3].who_name);
        do_info( ch , buf);

        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}
bool spec_guard_guardfive(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {   
       if (!IS_PLAYING(d)
           || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
           || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
           || victim->pcdata->kingdom == 5)
        {        
           continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
          ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[5].who_name);

        do_info( ch , buf);
                 
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}
bool spec_guard_guardfour(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
	   || victim->pcdata->kingdom == 4)
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[4].who_name);

        do_info( ch , buf);
      
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}
bool spec_guard_guardsix(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {   
       if (!IS_PLAYING(d)
           || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
           || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
           || victim->pcdata->kingdom == 6)
        {        
           continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
          ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[6].who_name);

        do_info( ch , buf);
                 
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}
bool spec_guard_guardseven(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
	   || victim->pcdata->kingdom == 7)
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[7].who_name);

        do_info( ch , buf);
      
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}
bool spec_guard_guardeight(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
	   || victim->pcdata->kingdom == 8)
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[8].who_name);

        do_info( ch , buf);
      
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}
bool spec_guard_guardnine(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
	   || victim->pcdata->kingdom == 9)
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[9].who_name);

        do_info( ch , buf);
      
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}
bool spec_guard_guardten(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
/*           || victim->in_room == ch->in_room*/
	   || victim->pcdata->kingdom == 10)
	{
            continue;
        }

        if (ch->in_room!=victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }

        spell_curse( 24 , 100 , ch , victim ); /* No Recall */

        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"#7%s is invading the Hall of %s#n.",victim->name,king_table[10].who_name);

        do_info( ch , buf);
      
        multi_hit( ch, victim, gsn_punch);

        return TRUE;

   }
   return FALSE;
}



/*
 * Core procedure for dragons.
 */
bool dragon( CHAR_DATA *ch, char *spell_name )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( spell_name ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



/*
 * Special procedures for mobiles.
 */
bool spec_breath_any( CHAR_DATA *ch )
{
    if ( ch->position != POS_FIGHTING )
	return FALSE;

    switch ( number_bits( 3 ) )
    {
    case 0: return spec_breath_fire		( ch );
    case 1:
    case 2: return spec_breath_lightning	( ch );
    case 3: return spec_breath_gas		( ch );
    case 4: return spec_breath_acid		( ch );
    case 5:
    case 6:
    case 7: return spec_breath_frost		( ch );
    }

    return FALSE;
}



bool spec_breath_acid( CHAR_DATA *ch )
{
    return dragon( ch, "acid breath" );
}



bool spec_breath_fire( CHAR_DATA *ch )
{
    return dragon( ch, "fire breath" );
}



bool spec_breath_frost( CHAR_DATA *ch )
{
    return dragon( ch, "frost breath" );
}



bool spec_breath_gas( CHAR_DATA *ch )
{
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( ( sn = skill_lookup( "gas breath" ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, NULL );
    return TRUE;
}



bool spec_breath_lightning( CHAR_DATA *ch )
{
    return dragon( ch, "lightning breath" );
}



bool spec_cast_adept( CHAR_DATA *ch )
{

    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;
    if (IS_NPC(victim))
	return FALSE;

    switch ( number_bits( 4 ) )
    {
    case 0:
	act( "", ch, NULL, NULL, TO_ROOM, FALSE );
	spell_darkblessing( skill_lookup( "darkblessing" ), ch->level, ch, victim );
	return TRUE;


    case 1:
	act( "", ch, NULL, NULL, TO_ROOM, FALSE );
	spell_bless( skill_lookup( "bless" ), ch->level, ch, victim );
	return TRUE;

    case 2:
        act( "", ch, NULL, NULL, TO_ROOM, FALSE );
        spell_frenzy( skill_lookup( "frenzy" ), ch->level, ch, victim );
        return TRUE;
    case 3:
        act( "", ch, NULL, NULL, TO_ROOM, FALSE );
        spell_godbless( skill_lookup( "godbless" ), ch->level, ch, victim );
        return TRUE;
    }
    return FALSE;
}



bool spec_cast_cleric( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";      break;
	case  1: min_level =  3; spell = "cause serious";  break;
	case  2: min_level =  7; spell = "earthquake";     break;
	case  3: min_level =  9; spell = "cause critical"; break;
	case  4: min_level = 10; spell = "dispel evil";    break;
	case  5: min_level = 12; spell = "curse";          break;
	case  6: min_level = 12; spell = "change sex";     break;
	case  7: min_level = 13; spell = "flamestrike";    break;
	case  8:
	case  9:
	case 10: min_level = 15; spell = "harm";           break;
	default: min_level = 16; spell = "dispel magic";   break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_judge( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    spell = "high explosive";
    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_mage( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 1 ) )
	{
	case  0: min_level = 20; spell = "prismatic beam"; break;
	default: min_level = 20; spell = "desanct";     break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_undead( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "curse";          break;
	case  1: min_level =  3; spell = "weaken";         break;
	case  2: min_level =  6; spell = "chill touch";    break;
	case  3: min_level =  9; spell = "blindness";      break;
	case  4: min_level = 12; spell = "poison";         break;
	case  5: min_level = 15; spell = "energy drain";   break;
	case  6: min_level = 18; spell = "harm";           break;
	case  7: min_level = 21; spell = "teleport";       break;
	default: min_level = 24; spell = "gate";           break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_fido( CHAR_DATA *ch )
{
    OBJ_DATA *corpse;
    OBJ_DATA *c_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( corpse = ch->in_room->contents; corpse != NULL; corpse = c_next )
    {
	c_next = corpse->next_content;
	if ( corpse->item_type != ITEM_CORPSE_NPC )
	    continue;

	act( "$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM, FALSE );
	for ( obj = corpse->contains; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    obj_from_obj( obj );
	    obj_to_room( obj, ch->in_room );
	}
	if (corpse != NULL) extract_obj( corpse );
	return TRUE;
    }

    return FALSE;
}



bool spec_guard( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *ech;
    int max_evil;
    int rand;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    max_evil = 300;
    ech      = NULL;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( victim->fighting != NULL
	&&   victim->fighting != ch
	&&   victim->alignment < max_evil )
	{
	    max_evil = victim->alignment;
	    ech      = victim;
	}
    }

    if ( victim != NULL )
    {
	rand=number_range(1,5);
	if (rand == 1)
	{
	    sprintf( buf, "It is an honour to meet you, %s!",
	        victim->name);
	    do_say( ch, buf );
	}
	else if (rand == 2)
	{
	    act("You bow deeply before $N.",ch,NULL,victim,TO_CHAR, FALSE);
	    act("$n bows deeply before you.",ch,NULL,victim,TO_VICT, FALSE);
	    act("$n bows deeply before $N.",ch,NULL,victim,TO_NOTVICT, FALSE);
	}
	else if (rand == 3)
	{
	    act("You shake $N's hand.",ch,NULL,victim,TO_CHAR, FALSE);
	    act("$n shakes your hand.",ch,NULL,victim,TO_VICT, FALSE);
	    act("$n shakes $N's hand.",ch,NULL,victim,TO_NOTVICT, FALSE);
	    sprintf( buf, "It's a pleasure to see you again, %s!",
	        victim->name);
	    do_say( ch, buf );
	}
	else if (rand == 4)
	{
	    act("You pat $N on the back.",ch,NULL,victim,TO_CHAR, FALSE);
	    act("$n pats you on the back.",ch,NULL,victim,TO_VICT, FALSE);
	    act("$n pats $N on the back.",ch,NULL,victim,TO_NOTVICT, FALSE);
	    sprintf( buf, "Greetings %s!  If you need anything, just say!",
	        victim->name);
	    do_say( ch, buf );
	}
	else
	{
	    act("You beam a smile at $N.",ch,NULL,victim,TO_CHAR, FALSE);
	    act("$n beams a smile at you.",ch,NULL,victim,TO_VICT, FALSE);
	    act("$n beams a smile at $N.",ch,NULL,victim,TO_NOTVICT, FALSE);
	}
	return TRUE;
    }

    if ( ech != NULL )
    {
	act( "$n screams 'PROTECT THE INNOCENT!!  BANZAI!!",
	    ch, NULL, NULL, TO_ROOM, FALSE );
	multi_hit( ch, ech, TYPE_UNDEFINED );
	return TRUE;
    }

    return FALSE;
}



bool spec_janitor( CHAR_DATA *ch )
{
    OBJ_DATA *trash;
    OBJ_DATA *trash_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( trash = ch->in_room->contents; trash != NULL; trash = trash_next )
    {
	trash_next = trash->next_content;
	if ( !IS_SET( trash->wear_flags, ITEM_TAKE ) )
	    continue;
	if ( trash->item_type == ITEM_DRINK_CON
	||   trash->item_type == ITEM_TRASH
	||   trash->cost < 10 )
	{
	    act( "$n picks up some trash.", ch, NULL, NULL, TO_ROOM, FALSE );
	    obj_from_room( trash );
	    obj_to_char( trash, ch );
	    return TRUE;
	}
    }

    return FALSE;
}



bool spec_mayor( CHAR_DATA *ch )
{
    static const char open_path[] =
	"W3a3003b33000c111d0d111Oe333333Oe22c222112212111a1S.";

    static const char close_path[] =
	"W3a3003b33000c111d0d111CE333333CE22c222112212111a1S.";

    static const char *path;
    static int pos;
    static bool move;

    if ( !move )
    {
	if ( time_info.hour ==  6 )
	{
	    path = open_path;
	    move = TRUE;
	    pos  = 0;
	}

	if ( time_info.hour == 20 )
	{
	    path = close_path;
	    move = TRUE;
	    pos  = 0;
	}
    }

    if ( ch->fighting != NULL )
	return spec_cast_cleric( ch );
    if ( !move || ch->position < POS_SLEEPING )
	return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0' );
	break;

    case 'W':
	ch->position = POS_STANDING;
	act( "$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM, FALSE );
	break;

    case 'S':
	ch->position = POS_SLEEPING;
	act( "$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM, FALSE );
	break;

    case 'a':
	act( "$n says 'Hello Honey!'", ch, NULL, NULL, TO_ROOM, FALSE );
	break;

    case 'b':
	act( "$n says 'What a view!  I must do something about that dump!'",
	    ch, NULL, NULL, TO_ROOM, FALSE );
	break;

    case 'c':
	act( "$n says 'Vandals!  Youngsters have no respect for anything!'",
	    ch, NULL, NULL, TO_ROOM, FALSE );
	break;

    case 'd':
	act( "$n says 'Good day, citizens!'", ch, NULL, NULL, TO_ROOM, FALSE );
	break;

    case 'e':
	act( "$n says 'I hereby declare the city of Midgaard open!'",
	    ch, NULL, NULL, TO_ROOM, FALSE );
	break;

    case 'E':
	act( "$n says 'I hereby declare the city of Midgaard closed!'",
	    ch, NULL, NULL, TO_ROOM, FALSE );
	break;

    case 'O':
	do_unlock( ch, "gate" );
	do_open( ch, "gate" );
	break;

    case 'C':
	do_close( ch, "gate" );
	do_lock( ch, "gate" );
	break;

    case '.' :
	move = FALSE;
	break;
    }

    pos++;
    return FALSE;
}



bool spec_poison( CHAR_DATA *ch )
{
    CHAR_DATA *victim;

    if ( ch->position != POS_FIGHTING
    || ( victim = ch->fighting ) == NULL
    ||   number_percent( ) > 2 * ch->level )
	return FALSE;

    act( "You bite $N!",  ch, NULL, victim, TO_CHAR, FALSE    );
    act( "$n bites $N!",  ch, NULL, victim, TO_NOTVICT, FALSE );
    act( "$n bites you!", ch, NULL, victim, TO_VICT, FALSE    );
    spell_poison( gsn_poison, ch->level, ch, victim );
    return TRUE;
}



bool spec_thief( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int gold;

    if ( ch->position != POS_STANDING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( IS_NPC(victim)
	||   (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_STEAL))
	||   victim->level >= LEVEL_IMMORTAL
	||   number_bits( 2 ) != 0
	||   !can_see( ch, victim ) )	/* Thx Glop */
	    continue;

	if ( IS_AWAKE(victim) && number_range( 0, ch->level ) == 0 )
	{
	    act( "You discover $n's hands in your wallet!",
		ch, NULL, victim, TO_VICT, FALSE );
	    act( "$N discovers $n's hands in $S wallet!",
		ch, NULL, victim, TO_NOTVICT, FALSE );
	    return TRUE;
	}
	else
	{
	    gold = victim->gold * number_range( 1, 20 ) / 100;
	    ch->gold     += 7 * gold / 8;
	    victim->gold -= gold;
	    return TRUE;
	}
    }

    return FALSE;
}

bool spec_eater( CHAR_DATA *ch )
{
  /* The spec_eater is a hungry bugger who eats players.  If they get 
   * eaten, they get transported to the room with the same vnum as the 
   * mob Example:  A spec_eater dragon with the vnum 31305 would send 
   * anybody eaten to room 31305.
   * KaVir.
   */
   CHAR_DATA       *victim;
   ROOM_INDEX_DATA *pRoomIndex;
   if ( ch->position != POS_FIGHTING )
   {
      return FALSE;
   }
   if (number_percent() > 50 ) return FALSE;
   victim=ch->fighting;
   act( "$n stares at $N hungrily and licks $s lips!",  ch, NULL, victim, TO_NOTVICT, FALSE    );
   act( "$n stares at you hungrily and licks $s lips!",  ch, NULL, victim, TO_VICT, FALSE    );
   if (number_percent() > 25 ) return FALSE;
   pRoomIndex = get_room_index(ch->pIndexData->vnum);
   act( "$n opens $s mouth wide and lunges at you!", ch, NULL, victim, TO_VICT, FALSE    );
   act( "$n swallows you whole!", ch, NULL, victim, TO_VICT, FALSE    );
   act( "$n opens $s mouth wide and lunges at $N!",  ch, NULL, victim, TO_NOTVICT, FALSE );
   act( "$n swallows $N whole!",  ch, NULL, victim, TO_NOTVICT, FALSE );
   char_from_room(victim);
   char_to_room(victim,pRoomIndex);
   do_emote(ch,"burps loudly.");
   do_look(victim,"auto");
   return TRUE;
}

bool spec_gremlin_original( CHAR_DATA *ch )
{
   OBJ_DATA *object;
   OBJ_DATA *object_next;
   OBJ_DATA *obj;
   char buf  [MAX_STRING_LENGTH];

   if ( !IS_AWAKE( ch ) )
      return FALSE;

   if (number_percent() > 25)
   {
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
	/* Lets make this mob DO things!  KaVir */
	int speech;
	speech = number_range(1,8);
	for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
	{
	    v_next = victim->next_in_room;
	    if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 )
	        break;
	}
	if ( victim != NULL )
	{
	    if (speech == 1) 
	    {
		strcpy(buf,"Anyone got any food? I'm famished!");
		do_say(ch,buf);
	    }
	    if (speech == 2) do_emote(ch,"rubs his tummy hungrily.");
	    if (speech == 3) do_emote(ch,"looks around for any scraps of food.");
	    if (speech == 4)
	    {
		if(!IS_NPC(victim))
		    sprintf(buf,"Excuse me %s, have you got any spare food?",victim->name);
		else
		    sprintf(buf,"I wonder if %s has got any spare food?",victim->short_descr);
		do_say(ch,buf);
	    }

	    if (speech == 5)
	    {
		strcpy(buf,"If you're not hungry, drop that pie for me!");
		do_say(ch,buf);
	    }
	    if (speech == 6)
	    {
		if(!IS_NPC(victim))
		    sprintf(buf,"Do you have any food, %s?  Pleeeaaase?",victim->name);
		else
		    sprintf(buf,"I bet %s has lots of spare food!",victim->short_descr);
		do_say(ch,buf);
	    }
	    if (speech == 7)
	    {
		if(!IS_NPC(victim))
		    sprintf(buf,"Why won't you give me any food, %s?",victim->name);
		else
		    sprintf(buf,"Why won't you give me any food, %s?",victim->short_descr);
		do_say(ch,buf);
	    }
	    if (speech == 8)
	    {
		if(!IS_NPC(victim))
		    sprintf(buf,"I think i'll follow you around for a while, ok %s?",victim->name);
		else
		    sprintf(buf,"I bet %s has lots of food.",victim->short_descr);
		do_say(ch,buf);
		do_follow(ch,victim->name);
	    }
	}
	/*
        return TRUE;
	*/
   }

   do_drop(ch,"all");
   for ( object = ch->in_room->contents; object; object = object_next )
   {
      object_next = object->next_content;
      if ( !IS_SET(object->wear_flags, ITEM_TAKE) )
         continue;

      if (object == NULL) continue;

      if ( object->item_type == ITEM_FOOD
        || object->item_type == ITEM_TRASH
        || object->item_type == ITEM_CORPSE_NPC )
      {
         act( "$n picks $p up.", ch, object, NULL, TO_ROOM, FALSE );
         obj_from_room( object );
         obj_to_char( object, ch );
         strcpy(buf,"Ah....foooood....goooood!");
         do_say(ch,buf);
         if (object->item_type == ITEM_CORPSE_NPC) do_get(ch,"all corpse");
         act( "$n eats $p.", ch, object, NULL, TO_ROOM, FALSE );
         if (object != NULL) extract_obj(object);
	 if (number_percent() > 25) return TRUE;
         act( "$n sits down and curls up into a ball.", ch,  NULL, NULL, TO_ROOM, FALSE );
         act( "You watch in amazement as a cocoon forms around $n.", ch,  NULL, NULL, TO_ROOM, FALSE );
         obj=create_object(get_obj_index(30008),0);
         obj->value[0] = 30002;
         obj->timer = 1;
         obj_to_room(obj,ch->in_room);
         extract_char(ch,TRUE);
         return TRUE;
      }
   }
   return FALSE;
}

bool spec_gremlin_born( CHAR_DATA *ch )
{
   OBJ_DATA        *object;
   OBJ_DATA        *object_next;
   OBJ_DATA        *obj;
   CHAR_DATA	   *victim;
   EXIT_DATA	   *pexit;
   ROOM_INDEX_DATA *to_room;
   int		   door;
   char            buf [MAX_STRING_LENGTH];

   /* When they first appear, they try to equip themselves as best they can */
   if ( ch->max_move == 100 )
   {
         do_get(ch,"all");
         do_wear(ch,"all");
         do_drop(ch,"all");
   }
   ch->max_move = ch->max_move -1;
   if ( ch->max_move < 5 )
   {
	 strcpy(buf,"Alas, there is not enough food to go around...");
	 do_say(ch,buf);
         act( "$n falls to the floor and crumbles into dust.", ch, NULL, NULL, TO_ROOM, FALSE );
         extract_char(ch,TRUE);
	 return TRUE;
   }
   else if (( ch->max_move < 35) && number_percent() > 95 )
         act( "$n's stomach growls with hunger.", ch, NULL, NULL, TO_ROOM, FALSE );

   if ( !IS_AWAKE( ch ) )
      return FALSE;

   for ( object = ch->in_room->contents; object; object = object_next )
   {
      object_next = object->next_content;
      if ( !IS_SET(object->wear_flags, ITEM_TAKE) )
         continue;

      if (object == NULL) continue;

      if ( object->item_type == ITEM_FOOD
        || object->item_type == ITEM_TRASH
        || object->item_type == ITEM_CORPSE_NPC )
      {
         act( "$n picks $p up.", ch, object, NULL, TO_ROOM, FALSE );
         obj_from_room( object );
         obj_to_char( object, ch );
         if (object->item_type == ITEM_CORPSE_NPC) do_get(ch,"all corpse");
         act( "$n eats $p.", ch, object, NULL, TO_ROOM, FALSE );
	 if (object != NULL) extract_obj(object);
	 if (ch->max_move < 80) ch->max_move = ch->max_move + 20;
         do_wear(ch,"all");
         do_drop(ch,"all");
	 /* Increase the following if gremlins reproduce too fast, and 
	  * reduce it if they reproduce too slowly.  KaVir.
	  */
	 if (ch->max_move > 25 && number_percent() > 2)
	 {
	     if (ch->pIndexData->vnum == 30004)
             	 obj=create_object(get_obj_index(30009),0);
	     else
	     {
             	 obj=create_object(get_obj_index(30008),0);
             	 obj->value[0] = 30003;
             	 if (ch->pIndexData->vnum == 30003)
             	     obj->value[0] = 30004;
             	 obj->timer = number_range(8,10);
	     }
             act( "$n squats down and lays $p!", ch,  obj, NULL, TO_ROOM, FALSE );
             obj_to_room(obj,ch->in_room);
	     if (number_percent() > 25)
	     	 ch->max_mana = ch->max_mana +50;
	 }
	 return TRUE;
      }
   }

   if ( ch->fighting != NULL )
	return spec_poison( ch );

   /* We'll give him 3 chances to find an open door */
   door = number_range(0,5);
   if (((pexit = ch->in_room->exit[door]) == NULL) || (to_room = pexit->to_room) == NULL)
	door = number_range(0,5);
   if (((pexit = ch->in_room->exit[door]) == NULL) || (to_room = pexit->to_room) == NULL)
	door = number_range(0,5);
   if (!(((pexit = ch->in_room->exit[door]) == NULL) || (to_room = pexit->to_room) == NULL))
   {
   	/* If the door is closed they'll have to open it */
   	if (IS_SET(pexit->exit_info, EX_CLOSED))
   	{
	    if (door == 0) do_open(ch,"north");
	    if (door == 1) do_open(ch,"east");
	    if (door == 2) do_open(ch,"south");
	    if (door == 3) do_open(ch,"west");
	    if (door == 4) do_open(ch,"up");
	    if (door == 5) do_open(ch,"down");
   	}
   	/* Now they know where they are going, they have to move */
   	move_char(ch,door);
/*	return FALSE; */
   }
   /* Now we check the room for someone to kill */
   for (victim = char_list; victim; victim = victim->next)
   {
      if ( victim->in_room != ch->in_room
	 || !victim->in_room
	 || victim == ch
	 || (!IS_NPC(victim) && IS_HERO(victim) && victim->hit < 0)
         || ((victim->name == ch->name) && (ch->max_move > 30))
         || ((victim->level > ch->level) && (ch->level < 12 ))
         || IS_IMMORTAL(victim) )
         continue;

      if (victim->in_room == ch->in_room)
      {
         if (victim->name == ch->name)
	 {
	     strcpy(buf,"Sorry brother, but I must eat you or I'll starve.");
	     do_say(ch,buf);
             do_kill(ch,"gremlin");
             do_kill(ch,"2.gremlin");
             return TRUE;
	 }
	 else
	 {
             do_kill(ch,victim->name);
             return TRUE;
	 }
         return TRUE;
      }
      continue;
   }
   return FALSE;
}

bool spec_mobius_hunter(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];
   int bleh;
   int blah;
   if (ch->fighting) victim = ch->fighting;


   if (ch->in_room == NULL) return FALSE;
   if (ch->position != POS_STANDING) return FALSE;

   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
           || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
	   || victim->level < 3          
	   || is_safe(ch,victim)
 || victim->pcdata->chobj != NULL)
/*           || victim->in_room->area != ch->in_room->area
           || victim->in_room == ch->in_room*/
	         
 {
            continue;
        }
       if (ch->position == POS_FIGHTING)
       {
	 if (ch->fighting) victim = ch->fighting;

	bleh = number_range(1,2);
       if (bleh == 1);
       {
	if (ch->fighting != NULL)
	{
	ch->hit *= 2;
	if (ch->hit > 300000) ch->hit = 300000;
	victim->hit *= 0.75;
	sprintf(buf,"%s shall feed my soul!",victim->name);
	do_chat(ch,buf);
	return TRUE;
	}
	else if (ch->fighting != NULL)	
	{
	sprintf(buf,"I will take your head with me when I go!");
	do_chat(ch,buf);
	spell_gas_breath( skill_lookup( "gas breath" ), ch->level, ch, ch );
	return TRUE;
	}
       }
       if (bleh == 2);
       {
	blah = number_range(1,2);
	if (blah == 1)
	 {
	  victim->hit = 0;
	  sprintf( buf,"%s may flee to the afterlife!",victim->name);
          do_chat(ch,buf);
          if(victim->pcdata->killedby) victim->pcdata->killedby = "mobius hunter";
	  victim->level = 2;
    	  behead( victim);
	  //victim->level = 2;
	  return TRUE;
	 }
        if (blah == 2)
	 {
	  victim->hit *= 1.1;
	  sprintf(buf,"%s has proven to be a worthy foe.",victim->name);
	  do_chat(ch,buf);
	  return TRUE;
	 }
       	}
      }
	if (ch->in_room != victim->in_room)
        {
           act("$n disappears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( ch );
           char_to_room(ch, victim->in_room);

           act("$n appears in a swirl of smoke!",ch,NULL,NULL,TO_ROOM, FALSE);
           act("You appear in a swirl of smoke!",ch,NULL,NULL,TO_CHAR, FALSE);
        }       
		if (is_safe(ch,victim)) return FALSE;

        sprintf( buf,"%s shall not get away from me!",victim->name);
        do_chat( ch , buf);

	spell_faerie_fog( skill_lookup( "faerie fog" ),	ch->level, ch, ch);
        spell_gas_breath( skill_lookup( "gas breath" ), ch->level, ch, ch );

        return TRUE;

   }
   return FALSE;
}
/*
 * Rand's new fun shit.
 */
bool spec_rand_new3( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char buf1 [MSL];
    char buf  [MAX_STRING_LENGTH];
    int speech;
	if (ch->fighting == NULL)
        {
        if (ch->position != POS_STANDING) update_pos(ch);
          if (ch->hit < ch->max_hit)
		ch->hit = ch->max_hit;
	}
        if ( ch->fighting != NULL )
        {
            victim = ch->fighting;
            speech = number_range(1,5);
            if (speech == 1)
            {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                return TRUE;
            }
            if (speech == 2) 
            {
		spell_super_gas( skill_lookup( "super gas" ), ch->level, ch, ch );
                return TRUE;
            }
                if (speech == 3)
                {
                 if (ch->hit <= (ch->max_hit * 0.50) && number_range(1,2)==1)
                 {
                  ch->hit = ch->max_hit;
                  sprintf(buf, "I will be renewed!");
                  spell_super_gas( skill_lookup( "super gas" ), ch->level, ch, ch );
                  do_say(ch, buf);
                  return TRUE;
                 }
		 else
		 {
                  spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                  spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                  spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                  spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                  spell_super_gas( skill_lookup( "super gas" ), ch->level, ch, ch );
                  return TRUE;
		 }
                }

                if (speech == 4)
                {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_super_gas( skill_lookup( "super gas" ), ch->level, ch, ch );
                return TRUE;
                }
                if (speech == 5)
                {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                sprintf(buf, "%s #0looks at you and #yblasts you with a backhand, #Rhurling#n #yyou across the room and #0stunning you.#n\n\r",ch->short_descr);
                stc(buf,victim);
                REMOVE_BIT(victim->affected_by, AFF_FLYING);
                send_to_char("You fall to the ground!\n\r",victim);
                sprintf(buf1, "%s falls to the ground!\n\r",victim->name);
                stc(buf, ch);
                return TRUE;
              }
        }
   return FALSE;
}

bool spec_rand_ft( CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
        return FALSE;
       if (ch->fighting == NULL)
        {
          if (ch->position != POS_STANDING) update_pos(ch);
          if (ch->hit < ch->max_hit)
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
        }

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
        v_next = victim->next_in_room;
        if ( victim != ch && can_see( ch, victim ) )
            break;
    }

    if ( victim == NULL )
        return FALSE;
    if (IS_NPC(victim))
        return FALSE;

    if ( victim != NULL )
    {
	if (victim->fight_timer < 1)
        {
	   spell_heal( skill_lookup( "heal" ), ch->level, ch, victim);
	   return TRUE;
	} 
	if (victim->fight_timer > 0)
	{
	   sprintf(buf, "#7You dare to violate %s #7with your vile violence?!#n",ch->in_room->name);
	   do_say(ch,buf);
           spell_prismatic_beam( skill_lookup( "prismatic beam" ), ch->level, ch, victim);
	   return TRUE;
	}
    }

    if (victim == NULL)
	return FALSE;

return FALSE;
}

bool spec_rand_new( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char buf  [MAX_STRING_LENGTH];
    int speech;
        if (ch->fighting == NULL)
        {
        if (ch->position != POS_STANDING) update_pos(ch);
        ch->hit = ch->max_hit;
        spell_faerie_fog(skill_lookup("faerie fog"), ch->level, ch, ch);
        }
    if ( ch->fighting != NULL )
        {
            victim = ch->fighting;
            speech = number_range(1,3);
            if (speech == 1)
            {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_super_gas( skill_lookup( "super gas" ), ch->level, ch, ch );
                return TRUE;
            }

/*            {
                if (victim->level <=3 && !IS_NPC(victim) && number_range(1,5) == 1)
                {
		  if ((obj = create_object(get_obj_index(420002),60)) == NULL)
  		  {
   		   sprintf(buf, "Object bug, tell Rand about it.");
		   log_string( buf );
                   return TRUE;
                  }
   		  else
		 {
                 obj_to_room(obj,victim->in_room);		 
                 victim->pcdata->obj_vnum = obj->pIndexData->vnum;
	         stop_fighting( victim, TRUE );
                 do_save(victim,"");
 		 sprintf(buf1, "%s has been frozen within a #RKin#rdr#Red #0Sp#8ir#0it #RJe#rw#Rel#n\n\r",victim->name);
		 do_info(ch,buf1);
                 act("$n screams and freezes into $p.",ch,obj,NULL,TO_ROOM, FALSE);
                 act("You scream and freeze into $p.",ch,obj,NULL,TO_CHAR, FALSE);
                 obj->chobj = victim;
                 victim->pcdata->chobj = obj;
                 SET_BIT(victim->affected_by, AFF_POLYMORPH);
                 xSET_BIT(victim->extra, EXTRA_OSWITCH);
                 free_string(victim->morph);
                 victim->morph = str_dup(obj->short_descr);
                 return TRUE;
		 }
		return FALSE;
                }
            }
*/
	    if (speech == 2)
	    {
                 spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                 spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                 spell_prismatic_beam( skill_lookup( "prismatic beam" ), ch->level, ch, victim);
		 return TRUE;
            }
                if (speech == 3)
                {
                 if (ch->hit <= (ch->max_hit * 0.25))
                 {
                 ch->hit = ch->max_hit * 0.50;
                 sprintf(buf, "I will be renewed!");
                 spell_faerie_fog(skill_lookup("faerie fog"), ch->level, ch, ch);
                 spell_gas_breath( skill_lookup( "gas breath" ), ch->level, ch, ch );
                 do_say(ch, buf);
                 return TRUE;
                 }
                 else return FALSE;
                }
             if (speech == 4)
                {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_gas_breath( skill_lookup( "gas breath" ), ch->level, ch, ch );
                spell_gas_breath( skill_lookup( "gas breath" ), ch->level, ch, ch );
                return TRUE;
                }
	return FALSE;
        }
   return FALSE;
}

bool spec_rand_new2( CHAR_DATA *ch )
{
    CHAR_DATA *victim;

    char buf  [MAX_STRING_LENGTH];
    char buf1 [MSL];
	int speech;
       if (ch->fighting == NULL)
        {
        if (ch->position != POS_STANDING) update_pos(ch);
        ch->hit = ch->max_hit;
	}
        if ( ch->fighting != NULL )
        {
         victim = ch->fighting;
         speech = number_range(1,3);
           if (speech == 1)
	   {
             if ((number_range(1,3) == 1) && victim->level <=3)
		{
		sprintf(buf, "%s #0looks at you and #yblasts you with a backhand, #Rhurling#n #yyou across the room and #0stunning you.#n\n\r",ch->short_descr); 
		stc(buf,victim);
		REMOVE_BIT(victim->affected_by, AFF_FLYING);
		send_to_char("You fall to the ground!\n\r",victim);
		sprintf(buf1, "%s falls to the ground!\n\r",victim->name);
		stc(buf, ch);
		return TRUE;
              }
             else if ((number_range(1,3) == 2) && victim->level <= 3)
                {
		spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
		spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
		spell_gas_breath( skill_lookup( "gas breath" ), ch->level, ch, ch );
                spell_gas_breath( skill_lookup( "gas breath" ), ch->level, ch, ch );
	        return TRUE;
		}
	     else if ((number_range(1,3) == 3) && victim->level <= 3)
            {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_desanct(skill_lookup("desanct"),ch->level,ch,victim);
                spell_prismatic_beam( skill_lookup( "prismatic beam" ), ch->level, ch, victim);
                return TRUE;
            }

	   }
 	 else
	  return FALSE;
	}
return FALSE;
}
bool spec_rand_quit( CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    char buf  [MAX_STRING_LENGTH];
    int speech;
       if (ch->fighting == NULL)
        {
          if (ch->position != POS_STANDING) update_pos(ch);
          ch->hit = ch->max_hit;
        }
       for ( vch = char_list; vch != NULL; vch = vch_next )
        {
          vch_next    = vch->next;
          if ( vch->in_room == NULL || IS_NPC(vch) )
             continue;
          if ( vch->in_room != ch->in_room )
             continue;
	  if ( IS_IMMORTAL(vch))
	     continue;
	  if ( vch->fight_timer == 0 && vch->fighting == NULL)
	     {
                stop_fighting(vch, TRUE);
	  	char_from_room(vch);
		char_to_room(vch,get_room_index(ROOM_VNUM_QUIT));
                sprintf(buf,"#7%s #0has been transported to %s#n.#n",vch->name,vch->in_room->name);
                do_info(ch,buf);
                do_look(vch, "auto");
                do_quit(vch,"");
                return TRUE;
	     }	
          if ( vch->fight_timer > 0 && vch->fighting == NULL)
             {
		set_fighting(ch,vch);
		return TRUE;
	     }
	  if ( vch->fight_timer > 0 && vch->fighting != NULL)
	     {
		speech = number_range(1,vch->fight_timer);
           	if (speech == 1)
            	{
		  stop_fighting(vch, TRUE);
                  char_from_room( vch );
                  char_to_room( vch,get_room_index( ROOM_VNUM_QUIT) );
                  sprintf(buf,"#7%s #0has been transported to %s#n.#n",vch->name,vch->in_room->name);
                  do_info(ch,buf);
                  do_look(vch, "auto");
   		  do_quit(vch,"");
                  return TRUE;
		}
               return TRUE;
             }
        return FALSE;
     }
   return FALSE;
}

bool spec_rand_heals( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    char buf[MSL];
    int speech;
       if (ch->fighting == NULL)
        {
          if (ch->position != POS_STANDING) update_pos(ch);
	  if (number_range(1,5)==1)
		spell_faerie_fog( skill_lookup( "faerie fog" ), ch->level, ch, ch);
	  if (ch->hit < ch->max_hit)
		spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
        }
       if ( ch->fighting != NULL )
        {
         victim = ch->fighting;
         speech = number_range(1,5);
           if (speech == 1)
            {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_desanct(skill_lookup("desanct"),ch->level,ch,victim);
                return TRUE;
            }
           if (speech == 2)
            {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                return TRUE;
            }
           if (speech == 3)
            {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_super_gas( skill_lookup( "super gas" ), ch->level, ch, ch );
                return TRUE;
            }
           if (speech == 4)
            {
             if (ch->hit <= (ch->max_hit * 0.40) && number_range(1,3)==1)
              {
                ch->hit = ch->max_hit;
                sprintf(buf, "I will be renewed!");
                do_say(ch, buf);
              }
	      else 
	      {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
	      }
             return TRUE;
            }

	return FALSE;
	}
   return FALSE;
}
/*
 Mobius' special mobby code
 */
bool spec_mobius_original( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    char buf  [MAX_STRING_LENGTH];
    int speech;
    
	if ( ch->fighting != NULL )
	{
	    victim = ch->fighting;
	    speech = number_range(1,3);
	    if (speech == 1) 
            {
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_imp_heal(skill_lookup("imp heal"), ch->level, ch, ch);
                spell_super_gas( skill_lookup( "super gas" ), ch->level, ch, ch );
                return TRUE;
            }

	    if (speech == 2) 
		{
		if (victim->hit > victim->max_hit) victim->hit = victim->max_hit;
                if (victim->mana > victim->max_mana) victim->mana = victim->max_mana;
                if (victim->move > victim->max_move) victim->move = victim->max_move;
		  
		  act("$n points his finger at $N's and screams.",ch,NULL,victim,TO_NOTVICT, FALSE);
		  act("$n points at you and screams.",ch,NULL,victim,TO_VICT, FALSE);

		  if (xIS_SET(victim->act,PLR_HOLYLIGHT)) 
			  xREMOVE_BIT(victim->act,PLR_HOLYLIGHT);
		  if (IS_SET(victim->affected_by, AFF_DETECT_HIDDEN))
			REMOVE_BIT(victim->affected_by, AFF_DETECT_HIDDEN);
		  if (IS_SET(victim->affected_by, AFF_DETECT_INVIS))
			REMOVE_BIT(victim->affected_by, AFF_DETECT_INVIS);
 			
        	  af.type      = skill_lookup("blindness");
		  af.location  = APPLY_HITROLL;
		  af.modifier  = -1000;
		  af.duration  = 1000;
		  af.bitvector = AFF_BLIND;
		  affect_to_char( victim, &af );
		  af.type      = skill_lookup("blindness");
                  af.location  = APPLY_DAMROLL;
                  af.modifier  = -1000;
                  af.duration  = 1000;
                  af.bitvector = AFF_BLIND;
                  affect_to_char( victim, &af );

		  return TRUE;
		}
		if (speech == 3)
		{
		 if (ch->hit <= (ch->max_hit * 0.25))
		  {
		   ch->hit = ch->max_hit * 0.50;
		   sprintf(buf, "I will be renewed!");
                 do_say(ch, buf);
		  }
		 spell_faerie_fog(skill_lookup("faerie fog"), ch->level, ch, ch);
		 spell_gas_breath( skill_lookup( "gas breath" ), ch->level, ch, ch );
		 spell_super_gas( skill_lookup( "super gas" ), ch->level, ch, victim);
		 return TRUE;
		
		}
		 else return FALSE;
	}
  return FALSE;
}

/* Spec_rogue, coded by Malice. */
bool spec_rogue( CHAR_DATA *ch )
{
/* To add to the life of mobs... they pickup and wear equipment */
    OBJ_DATA *object;
    OBJ_DATA *obj2;
    OBJ_DATA *object_next;
    char buf [MAX_STRING_LENGTH];

   if ( !IS_AWAKE( ch ) )
	return FALSE;
if ( ch->position == POS_FIGHTING )
do_kick(ch,"victim");


  for ( object = ch->in_room->contents; object; object = object_next )
    {
        object_next = object->next_content;
        if ( object == NULL )
	    continue;
	    
	if ( !IS_SET( object->wear_flags, ITEM_TAKE ) )
	    continue;

	if ( object->item_type == ITEM_CORPSE_NPC )
	    continue;
	    
	if ( ( object->item_type != ITEM_DRINK_CON
	    && object->item_type != ITEM_TRASH )
	    &&
	    
	   !(( IS_OBJ_STAT( object, ITEM_ANTI_EVIL   ) && IS_EVIL   ( ch ) ) ||
		 ( IS_OBJ_STAT( object, ITEM_ANTI_GOOD   ) && IS_GOOD   ( ch ) ) ||
		 ( IS_OBJ_STAT( object, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL( ch ) ) ) )
		
	{
	    act( "$n picks up $p and examines it carefully.", ch, object, NULL, TO_ROOM, FALSE );
	    obj_from_room( object );
	    obj_to_char( object, ch );
		/*Now compare it to what we already have*/
		for ( obj2 = ch->carrying; obj2; obj2 = obj2->next_content )
		{
		    if ( obj2->wear_loc != WEAR_NONE
			&& can_see_obj( ch, obj2 )
			&& object->item_type == obj2->item_type
			&& ( object->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
			break;
		}
		if (!obj2)
		{
		    switch (object->item_type)
		    {
			default:
			    strcpy(buf,"Hey, what a find!");
			    do_say(ch,buf);
			    break;
			case ITEM_FOOD:
			    strcpy(buf, "This looks like a tasty morsel!");
			    do_say(ch,buf);
			    do_eat(ch,object->name);
			    break;
			case ITEM_WAND:
			    strcpy(buf,"Wow, a magic wand!");
			    do_say(ch,buf);
			    wear_obj(ch,object,FALSE);
			    break;
			case ITEM_STAFF:
			    strcpy(buf,"Kewl, a magic staff!");
			    do_say(ch,buf);
			    wear_obj(ch,object,FALSE);
			    break;
			case ITEM_WEAPON:
			    strcpy(buf,"Hey, this looks like a nifty weapon!");
			    do_say(ch,buf);
			    wear_obj(ch,object,FALSE);
			    break;
			case ITEM_ARMOR:
			    strcpy(buf,"Oooh...a nice piece of armor!");
			    do_say(ch,buf);
			    wear_obj(ch,object,FALSE);
			    break;/*
			case ITEM_POTION:
			    strcpy(buf, "Great!  I was feeling a little thirsty!");
			    do_say(ch,buf);
			    act( "You quaff $p.", ch, object, NULL ,TO_CHAR, FALSE );
			    act( "$n quaffs $p.", ch, object, NULL, TO_ROOM, FALSE );
			    obj_cast_spell( object->value[1], object->level, ch, ch, NULL );
 			    obj_cast_spell( object->value[2], object->level, ch, ch, NULL );
			    obj_cast_spell( object->value[3], object->level, ch, ch, NULL );
			    extract_obj( object );
			    break;
			case ITEM_SCROLL:
			    strcpy(buf,"Hmmm I wonder what this says?");
			    do_say(ch,buf);
			    act( "You recite $p.", ch, object, NULL, TO_CHAR, FALSE );
			    act( "$n recites $p.", ch, object, NULL, TO_ROOM, FALSE );
			    obj_cast_spell( object->value[1], object->level, ch, NULL, object );
			    obj_cast_spell( object->value[2], object->level, ch, NULL, object );
			    obj_cast_spell( object->value[3], object->level, ch, NULL, object );
			    extract_obj( object );
			    break;*/
		    }
		    return TRUE;
		}
		
	    if ((object->level > obj2->level))
	    {
	    	strcpy(buf,"Now THIS looks like an improvement!");
		do_say(ch,buf);
	    	remove_obj(ch,obj2->wear_loc,TRUE);
	    	wear_obj(ch,object,FALSE);
	    }
	    else
	    {
	    	strcpy(buf,"I don't want this piece of junk!");
		do_say(ch,buf);
	    	act("You don't like the look of $p.",ch,object,NULL,TO_CHAR, FALSE);
			do_drop(ch,object->name);
			do_sacrifice(ch,object->name);
		}
	    return TRUE;
	}
    }

    return FALSE;
}

/*
 * Rotains Clan Guardian Spec
 * Original Code by Malice
 */
/*
bool spec_clan_guardian(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA       *d;
   CHAR_DATA             *victim;
   char                  buf[MAX_STRING_LENGTH];
   OBJ_DATA              *obj;


   if (ch->fighting) return FALSE;

   if (ch->in_room == NULL) return FALSE;
*/
   /*if (ch->in_room < 6645 && ch->in_room > 6657) break;*/
/*
   for (d = first_descriptor; d != NULL; d = d->next)
   {
       if (!IS_PLAYING(d)
	   || (victim = d->character) == NULL
           || IS_NPC(victim)
           || IS_IMMORTAL(victim)
	   || victim->in_room == NULL
           || victim->pcdata->chobj != NULL
           || victim->in_room->area != ch->in_room->area
           || victim->in_room == ch->in_room
	   || (IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->special,
SPC_INCONNU))
           || (victim->clan != NULL && !strcmp(victim->clan,"Manus Nigrum") ))
           {
             continue;
           }

      if ((obj = get_obj_carry(victim, "dlwr5"))
               && obj->pIndexData->vnum==6641)
      {
         return FALSE; 
      }
      else
      {
        if (ch->in_room != victim->in_room)
        {
           act("$n disappears in a haze of red!",victim,NULL,NULL,TO_ROOM, FALSE);
           act("You disappear into a haze of red!",victim,NULL,NULL,TO_CHAR, FALSE);

           char_from_room( victim );
           char_to_room(victim, ch->in_room);

           act("$n appears in a haze of red!",victim,NULL,NULL,TO_ROOM, FALSE);
           act("You appear into a haze of red!",victim,NULL,NULL,TO_CHAR, FALSE);
        }
     
        act("$n shouts at you \"You shall DIE!\"",ch,ch->long_descr,victim,TO_VICT, FALSE);
        act("You let out a battlecry as you defend the clan headquaters!",ch,NULL,victim,TO_CHAR, FALSE);
*//*
        spell_curse( 24 , 100 , ch , victim );
*//*
        if (victim->position <= POS_MORTAL && IS_HERO(victim) &&
	  ch->position == POS_STANDING)
        {
           char_from_room( victim );
           char_to_room(victim, get_room_index(ROOM_VNUM_ALTAR));
           continue;
        }
        sprintf( buf,"%s Is In Manus Nigrum Headquarters! Attack!\n\r",
victim->name); 
        do_shout( ch , buf);
      
        multi_hit( ch, victim, gsn_backstab); 

        return TRUE;

      }
   }
   return FALSE;
}
*/


bool spec_zombie_lord( CHAR_DATA *ch )
{
    char buf [MAX_STRING_LENGTH];
    OBJ_DATA		*obj;
    OBJ_DATA		*obj_content;
    OBJ_DATA		*obj_next;
    CHAR_DATA		*victim;
    MOB_INDEX_DATA	*pMobIndex;
    EXIT_DATA		*pexit;
    ROOM_INDEX_DATA	*to_room;
    int door;
    int consider 	= 4;
    bool north_ok 	= TRUE;
    bool east_ok 	= TRUE;
    bool south_ok 	= TRUE;
    bool west_ok 	= TRUE;
    bool up_ok 		= TRUE;
    bool down_ok 	= TRUE;
    int countup 	= 6;
    int option;

    if ( ch->position <= POS_SITTING )
    {
	do_stand(ch,"");
	return TRUE;
    }
    if ( (victim = ch->fighting) != NULL )
    {
	if (IS_AFFECTED(ch, AFF_FAERIE_FIRE))
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_POISON))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_cure_poison( skill_lookup( "cure poison" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_BLIND))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_cure_blindness( skill_lookup( "cure blindness" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_CURSE))
	{
	    act("$n's eyes glow bright purple for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_remove_curse( skill_lookup( "remove curse" ), ch->level, ch, ch );
	}
	else if (!IS_AFFECTED(ch, AFF_SANCTUARY))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_sanctuary( skill_lookup( "sanctuary" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("frenzy")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_frenzy( skill_lookup( "frenzy" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("darkblessing")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_darkblessing( skill_lookup( "darkblessing" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("bless")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_bless( skill_lookup( "bless" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("stone skin")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_stone_skin( skill_lookup( "stone skin" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("armor")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_armor( skill_lookup( "armor" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("shield")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_shield( skill_lookup( "shield" ), ch->level, ch, ch );
	}
	else if (!IS_AFFECTED(victim, AFF_FAERIE_FIRE) && number_percent() < 50)
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_faerie_fire( skill_lookup( "faerie fire" ), ch->level, ch, victim );
	}
	else if (!IS_AFFECTED(victim, AFF_BLIND) && number_percent() < 15)
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_blindness( skill_lookup( "blindness" ), ch->level, ch, victim );
	}
	else if (!IS_AFFECTED(victim, AFF_CURSE) && number_percent() < 15)
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_curse( skill_lookup( "curse" ), ch->level, ch, victim );
	}
	else if (ch->loc_hp[6] > 0)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_clot( skill_lookup( "clot" ), ch->level, ch, ch );
	}
	else if (ch->hit < (ch->max_hit * 0.5) && number_percent() < 75)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );
	}
	else if (ch->hit < (ch->max_hit * 0.25) && number_percent() < 50)
	{
	    do_flee(ch,"");
	    ch->spectype = ZOMBIE_REST;
	}
	else if (ch->hit < (ch->max_hit * 0.1) && number_percent() < 25)
	{
	    act("$n's eyes glow bright green for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_teleport( skill_lookup( "teleport" ), ch->level, ch, ch );
	    ch->spectype = ZOMBIE_REST;
	}
	else
	{
	    switch( number_range(1,10) )
	    {
		default:
		    do_kick(ch,"");
		    break;
		case 1:
		    do_disarm(ch,"");
		    break;
		case 2:
		case 3:
		case 4:
		case 5:
		    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
		    spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level, ch, victim );
		    break;
		case 6:
		case 7:
		case 8:
		    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
		    spell_harm( skill_lookup( "harm" ), ch->level, ch, victim );
		    break;
	    }
	}
	if ( (victim = ch->fighting) == NULL ) return TRUE;
	switch( number_percent() )
	{
	    default:
		break;
	    case 1:
		strcpy(buf,"Foolish mortal, you think you can kill what is already dead?");
		do_say(ch,buf);
		break;
	    case 2:
		if (IS_NPC(victim))
		    sprintf(buf,"I shall feast on your soul for this, %s",victim->short_descr);
		else
		    sprintf(buf,"I shall feast on your soul for this, %s",victim->name);
		do_say(ch,buf);
	    case 3:
		if (IS_NPC(victim))
		    sprintf(buf,"%s shall pay for his arrogance!",victim->short_descr);
		else
		    sprintf(buf,"%s shall pay for his arrogance!",victim->name);
		do_shout(ch,buf);
		break;
	    case 4:
		strcpy(buf,"This fight shall be your last!");
		do_say(ch,buf);
		break;
	}
	return TRUE;
    }

    if (ch->spectype != ZOMBIE_TRACKING && ch->spectype != ZOMBIE_REST)
    {
	if ((obj = get_obj_here(ch,"corpse")) != NULL)
	{
	    if ( ( pMobIndex = get_mob_index( obj->value[2] ) ) == NULL )
		return spec_rogue(ch);
	    victim = create_mobile( pMobIndex );
	    char_to_room(victim,ch->in_room);
	    sprintf(buf,"zombie %s",victim->name);
	    sprintf(buf,"The zombie of %s stands here.\n\r",victim->short_descr);
	    free_string(victim->long_descr);
	    victim->long_descr = str_dup(buf);
	    free_string(victim->name);
	    victim->name = str_dup(buf);
	    sprintf(buf,"the zombie of %s",victim->short_descr);
	    free_string(victim->short_descr);
	    victim->short_descr = str_dup(buf);
	    act("$n makes a few gestures over $p.",ch,obj,NULL,TO_ROOM, FALSE);
	    act("$n clambers to $s feet.",victim,NULL,NULL,TO_ROOM, FALSE);
	    free_string(victim->powertype);
	    victim->powertype = str_dup("zombie");
	    for ( obj_content = obj->contains; obj_content != NULL; obj_content = obj_next )
	    {
		obj_next = obj_content->next_content;
		obj_from_obj(obj_content);
		obj_to_char(obj_content,victim);
	    }
	    extract_obj(obj);
/*
	    victim->spec_fun = ch->spec_fun;
*/
	    do_wear(victim,"all");
	    strcpy(buf,"I shall spread the corruption!  The time of the Apocalypse is at hand!");
	    do_say(victim,buf);
	}
    	door = number_range(0,5);
	for ( door = 0; door <= 5; door++ )
	{
	    if (((pexit = ch->in_room->exit[door]) == NULL) || 
		(to_room = pexit->to_room) == NULL)
	    {
		switch (door)
		{
		    case DIR_NORTH: north_ok = FALSE; countup -= 1; break;
		    case DIR_SOUTH: south_ok = FALSE; countup -= 1; break;
		    case DIR_EAST:  east_ok  = FALSE; countup -= 1; break;
		    case DIR_WEST:  west_ok  = FALSE; countup -= 1; break;
		    case DIR_UP:    up_ok    = FALSE; countup -= 1; break;
		    case DIR_DOWN:  down_ok  = FALSE; countup -= 1; break;
		}
	    }
	}

	if (countup < 1)
	{
	    strcpy(buf,"Damn, I hate it when this happens!");
	    do_say(ch,buf);
	    char_from_room(ch);
	    char_to_room(ch,get_room_index(3001));
	    return TRUE;
	}

	for ( ; ; )
	{
	    option = number_range(0,5);
	    if (((pexit = ch->in_room->exit[option]) == NULL) || 
		(to_room = pexit->to_room) == NULL)
		continue;
	    if (countup > 1 && option == ch->specpower) continue;
	    if (IS_SET(pexit->exit_info, EX_CLOSED))
 	    {
	    	if (option == 0) do_open(ch,"north");
	    	if (option == 1) do_open(ch,"east");
	    	if (option == 2) do_open(ch,"south");
	    	if (option == 3) do_open(ch,"west");
	    	if (option == 4) do_open(ch,"up");
	    	if (option == 5) do_open(ch,"down");
   	    }
	    switch (option)
	    {
		default: break;
		case DIR_NORTH: ch->specpower = DIR_SOUTH; break;
		case DIR_SOUTH: ch->specpower = DIR_NORTH; break;
		case DIR_EAST:  ch->specpower = DIR_WEST;  break;
		case DIR_WEST:  ch->specpower = DIR_EAST;  break;
		case DIR_UP:    ch->specpower = DIR_DOWN;  break;
		case DIR_DOWN:  ch->specpower = DIR_UP;    break;
	    }
   	    move_char(ch,option);
	    break;
	}

    	for (victim = char_list; victim != NULL; victim = victim->next)
    	{
	    if (victim->in_room == NULL) continue;
    	    if ( victim->in_room != ch->in_room
	    || victim == ch
	    || (!IS_NPC(victim) && IS_HERO(victim) && victim->hit < 0)
            || IS_IMMORTAL(victim)
	    || is_safe(ch,victim)
	    || !IS_NPC(victim)
	    || !can_see(ch,victim) )
	    	continue;

	    if (IS_NPC(victim) && !str_cmp(victim->powertype,"zombie")) continue;
	    if (IS_NPC(victim) && victim->pIndexData->vnum == 30011) continue;
	    if (victim->in_room == ch->in_room)
      	    {
	    	act("$n examines $N closely, looking for $S weaknesses.",ch,NULL,victim,TO_NOTVICT, FALSE);
	    	act("$n examines you closely, looking for your weaknesses.",ch,NULL,victim,TO_VICT, FALSE);
	    	if      (victim->hit > (ch->hit*1.5)) consider -= 1;
	    	else if ((victim->hit*1.5) < ch->hit) consider += 1;
	    	if      (char_ac(victim)-50 > char_ac(ch)) consider -= 1;
	    	else if (char_ac(victim)+50 < char_ac(ch)) consider += 1;
	    	if      (char_hitroll(victim)+10 < char_hitroll(ch)) consider += 1;
	    	else if (char_hitroll(victim)-10 > char_hitroll(ch)) consider -= 1;
	    	if      (char_damroll(victim)+10 < char_damroll(ch)) consider += 1;
	    	else if (char_damroll(victim)-10 > char_damroll(ch)) consider -= 1;
	    	switch (consider)
	    	{
		    default: break;
		    case 8:
		    	strcpy(buf,"This shouldn't take more than a few seconds!");
			do_say(ch,buf);
		    	break;
		    case 7:
		    	strcpy(buf,"Ha! You are no match for me!");
			do_say(ch,buf);
		    	break;
		    case 6:
		    	strcpy(buf,"I should be able to win this one...");
			do_say(ch,buf);
		    	break;
		    case 5:
		    	strcpy(buf,"Hmmm, close match, but I think I have the edge.");
			do_say(ch,buf);
		    	break;
		    case 4:
		    	strcpy(buf,"I'd rather not, my toe hurts.");
			do_say(ch,buf);
		    	break;
		    case 3:
		    	strcpy(buf,"Hmmm, I'm not sure if I can win this one.");
			do_say(ch,buf);
		    	break;
		    case 2:
		    	strcpy(buf,"Heheh better not risk it...");
			do_say(ch,buf);
		    	break;
		    case 1:
		    	strcpy(buf,"I'd need a lot of luck...better not.");
			do_say(ch,buf);
		    	break;
		    case 0:
		    	strcpy(buf,"I think I'll give this one a miss!!!");
			do_say(ch,buf);
		    	break;
	    	}
		if (IS_IMMORTAL(victim)) continue;
	    	if (consider < 3) continue;
		if (victim->level > 1200 ) continue;
	    	do_kill(ch,victim->name);
            	return TRUE;
	    }
	}
    }

    switch ( ch->spectype )
    {
    default:
    case ZOMBIE_NOTHING:
	ch->spectype = number_range(1, 3);
	return spec_rogue(ch);
    case ZOMBIE_TRACKING:
/*
	if (strlen(ch->hunting) > 1)
	{
	    ch->spectype = number_range(1, 3);
	    return spec_rogue(ch);
	}
	if      (strlen(ch->in_room->track[0]) > 1)
	    strcpy(buf,ch->in_room->track[0]);
	else if (strlen(ch->in_room->track[1]) > 1)
	    strcpy(buf,ch->in_room->track[1]);
	else if (strlen(ch->in_room->track[2]) > 1)
	    strcpy(buf,ch->in_room->track[2]);
	else if (strlen(ch->in_room->track[3]) > 1)
	    strcpy(buf,ch->in_room->track[3]);
	else if (strlen(ch->in_room->track[4]) > 1)
	    strcpy(buf,ch->in_room->track[4]);
	else {ch->spectype = number_range(1, 3);return spec_rogue(ch);}
	do_hunt(ch,buf);
	ch->spectype = number_range(1, 3);
	return spec_rogue(ch);
*/
    case ZOMBIE_ANIMATE:
    case ZOMBIE_CAST:
	if (IS_AFFECTED(ch, AFF_FAERIE_FIRE))
	{
	    act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_POISON) && number_percent() < 75)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_cure_poison( skill_lookup( "cure poison" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_BLIND) && number_percent() < 75)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_cure_blindness( skill_lookup( "cure blindness" ), ch->level, ch, ch );
	}
	else if (IS_AFFECTED(ch, AFF_CURSE) && number_percent() < 75)
	{
	    act("$n's eyes glow bright purple for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_remove_curse( skill_lookup( "remove curse" ), ch->level, ch, ch );
	}
	else if (!IS_AFFECTED(ch, AFF_SANCTUARY) && number_percent() < 75)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_sanctuary( skill_lookup( "sanctuary" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("frenzy")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_frenzy( skill_lookup( "frenzy" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("darkblessing")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_darkblessing( skill_lookup( "darkblessing" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("bless")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_bless( skill_lookup( "bless" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("stone skin")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_stone_skin( skill_lookup( "stone skin" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("armor")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_armor( skill_lookup( "armor" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("shield")) && number_percent() < 50)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_shield( skill_lookup( "shield" ), ch->level, ch, ch );
	}
	else if (ch->loc_hp[6] > 0)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_clot( skill_lookup( "clot" ), ch->level, ch, ch );
	}
	else if (ch->hit < ch->max_hit)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("frenzy")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_frenzy( skill_lookup( "frenzy" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("darkblessing")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_darkblessing( skill_lookup( "darkblessing" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("bless")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_bless( skill_lookup( "bless" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("stone skin")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_stone_skin( skill_lookup( "stone skin" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("armor")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_armor( skill_lookup( "armor" ), ch->level, ch, ch );
	}
	else if (!is_affected(ch,skill_lookup("shield")))
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_shield( skill_lookup( "shield" ), ch->level, ch, ch );
	}
	else ch->spectype = 0;
	if (ch->hit < (ch->max_hit*0.25)) ch->spectype = ZOMBIE_REST;
	return TRUE;
    case ZOMBIE_REST:
	if (ch->hit >= ch->max_hit)
	{do_stand(ch,"");ch->spectype = 0;return TRUE;}
	if (IS_AFFECTED(ch, AFF_CURSE))
	{
	    act("$n's eyes glow bright purple for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_remove_curse( skill_lookup( "remove curse" ), ch->level, ch, ch );
	    return TRUE;
	}
	if (ch->in_room->vnum != ROOM_VNUM_ALTAR) do_recall(ch,"");
	if (ch->in_room->vnum == ROOM_VNUM_TEMPLE) do_north(ch,"");
	if (ch->position == POS_STANDING) do_rest(ch,"");
	if (ch->hit < ch->max_hit)
	{
	    act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM, FALSE);
	    spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );
	    ch->hit = ch->max_hit;
	}
	return TRUE;
    }
    return FALSE;
}

bool spec_dog( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *o_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    EXIT_DATA		*pexit;
    ROOM_INDEX_DATA	*to_room;
    int door;
    bool north_ok 	= TRUE;
    bool east_ok 	= TRUE;
    bool south_ok 	= TRUE;
    bool west_ok 	= TRUE;
    bool up_ok 		= TRUE;
    bool down_ok 	= TRUE;
    int countup 	= 6;
    int option;
    int random = number_range(1,5);

    if ( !IS_AWAKE(ch) )
    {
	if (random <= 2)
	    do_wake(ch,"");
	return TRUE;
    }
    else if (ch->position < POS_STANDING)
    {
	if (random <= 2)
	    do_stand(ch,"");
	else
	{
	    if (ch->in_room->vnum == 29107 && number_range(1,4) <= 2)
		act("$n eats some dog food from $s food bowl",ch,NULL,NULL,TO_ROOM, FALSE);
	    else if (ch->in_room->vnum == 29107)
		act("$n drinks some water from $s water bowl",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	return TRUE;
    }

    if (ch->in_room == NULL) return TRUE;

    if (ch->in_room->vnum == 29104 && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	{
	    act("$n curls up in front of the fire.",ch,NULL,NULL,TO_ROOM, FALSE);
	    do_sleep(ch,"");
	}
	else if (random == 2)
	    act("$n starts scratching up the carpet.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 3)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 4)
	{
	    act("$n jumps onto the couch.",ch,NULL,NULL,TO_ROOM, FALSE);
	    do_sit(ch,"");
	}
	else if (random == 5)
	    act("$n tries to crawl under a chair.",ch,NULL,NULL,TO_ROOM, FALSE);
	return TRUE;
    }
    else if (ch->in_room->vnum == 29104 && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	{
	    act("$n curls up under the dining table.",ch,NULL,NULL,TO_ROOM, FALSE);
	    do_sleep(ch,"");
	}
	else if (random == 2)
	    act("$n starts scratching up the carpet.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 3)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 4)
	{
	    act("$n jumps onto a chair.",ch,NULL,NULL,TO_ROOM, FALSE);
	    do_sit(ch,"");
	}
	else if (random == 5)
	    act("$n tries to crawl under a chair.",ch,NULL,NULL,TO_ROOM, FALSE);
	return TRUE;
    }
    else if (ch->in_room->vnum == 29113 && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	    act("$n starts scratching up the carpet.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 2)
	    act("$n tries to climb into the bath.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 3)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 4)
	{
	    act("$n sits behind the bathroom door.",ch,NULL,NULL,TO_ROOM, FALSE);
	    do_sit(ch,"");
	}
	else if (random == 5)
	    act("$n tries to squeeze behind the sink.",ch,NULL,NULL,TO_ROOM, FALSE);
	return TRUE;
    }
    else if (ch->in_room->vnum == 29107 && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	    act("$n sniffs around for food on the kitchen floor.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 2)
	    act("$n looks up hungrilly at the cupboard.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 3)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 4)
	{
	    act("$n walks over to $s food bowl.",ch,NULL,NULL,TO_ROOM, FALSE);
	    do_sit(ch,"");
	}
	else if (random == 5)
	    act("$n looks out the back door for cats to chase.",ch,NULL,NULL,TO_ROOM, FALSE);
	return TRUE;
    }
    else if ((ch->in_room->vnum == 29100 || ch->in_room->vnum == 29101 || 
	ch->in_room->vnum == 29117) && random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	    act("$n sniffs a flower.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 2)
	    act("$n rolls around on the grass.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 3)
	    act("$n tries to eat a blade of grass.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 4)
	    act("$n pounces on a grasshopper.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 5)
	    act("$n sniffs the air and growls playfully.",ch,NULL,NULL,TO_ROOM, FALSE);
	return TRUE;
    }
    if (random <= 2)
    {
	if ( (random = number_range(1,5)) == 1)
	    act("$n sniffs the carpet.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 2)
	    act("$n barks loudly.",ch,NULL,NULL,TO_ROOM, FALSE);
	else if (random == 3)
	    do_sit(ch,"");
	return TRUE;
    }

    if ( number_range(1,5) <= 3 )
    {
    	door = number_range(0,5);
	for ( door = 0; door <= 5; door++ )
	{
	    if (((pexit = ch->in_room->exit[door]) == NULL) || 
		(to_room = pexit->to_room) == NULL)
	    {
		switch (door)
		{
		    case DIR_NORTH: north_ok = FALSE; countup -= 1; break;
		    case DIR_SOUTH: south_ok = FALSE; countup -= 1; break;
		    case DIR_EAST:  east_ok  = FALSE; countup -= 1; break;
		    case DIR_WEST:  west_ok  = FALSE; countup -= 1; break;
		    case DIR_UP:    up_ok    = FALSE; countup -= 1; break;
		    case DIR_DOWN:  down_ok  = FALSE; countup -= 1; break;
		}
	    }
	}

	if (countup < 1)
	{
	    if (number_percent() < 80)
		act("$n barks loudly",ch,NULL,NULL,TO_ROOM, FALSE);
	    else
		do_sleep(ch,"");
	    return TRUE;
	}

	for ( ; ; )
	{
	    option = number_range(0,5);
	    if (((pexit = ch->in_room->exit[option]) == NULL) || 
		(to_room = pexit->to_room) == NULL)
		continue;
	    if (countup > 1 && option == ch->specpower) continue;
	    if (IS_SET(pexit->exit_info, EX_CLOSED))
 	    {
		if (option == 0)
		    act("$n scratches at the north door.",ch,NULL,NULL,TO_ROOM, FALSE);
		if (option == 1)
		    act("$n scratches at the east door.",ch,NULL,NULL,TO_ROOM, FALSE);
		if (option == 2)
		    act("$n scratches at the south door.",ch,NULL,NULL,TO_ROOM, FALSE);
		if (option == 3)
		    act("$n scratches at the west door.",ch,NULL,NULL,TO_ROOM, FALSE);
		if (option == 4)
		    act("$n scratches at the upward door.",ch,NULL,NULL,TO_ROOM, FALSE);
		if (option == 5)
		    act("$n scratches at the downward door.",ch,NULL,NULL,TO_ROOM, FALSE);
		act("$n barks loudly at the door.",ch,NULL,NULL,TO_ROOM, FALSE);
		for ( vch = char_list; vch != NULL; vch = vch_next )
		{
		    vch_next	= vch->next;
		    if ( vch->in_room == NULL || IS_NPC(vch) )
			continue;
		    if ( vch->in_room == ch->in_room )
			continue;
		    if ( vch->in_room->area == ch->in_room->area )
			send_to_char("You hear the sound of a dog barking nearby.\n\r",vch);
		}
		return TRUE;
   	    }
	    switch (option)
	    {
		default: break;
		case DIR_NORTH: ch->specpower = DIR_SOUTH; break;
		case DIR_SOUTH: ch->specpower = DIR_NORTH; break;
		case DIR_EAST:  ch->specpower = DIR_WEST;  break;
		case DIR_WEST:  ch->specpower = DIR_EAST;  break;
		case DIR_UP:    ch->specpower = DIR_DOWN;  break;
		case DIR_DOWN:  ch->specpower = DIR_UP;    break;
	    }
   	    move_char(ch,option);
	    break;
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = o_next )
    {
	o_next = obj->next_content;
	if (number_range(1,2) == 1) continue;
	act( "$n sniffs $p.", ch, obj, NULL, TO_ROOM, FALSE );
	if ( obj->item_type == ITEM_FOOD )
	{
	    act( "$n eats $p.", ch, obj, NULL, TO_ROOM, FALSE );
	    extract_obj(obj);
	}
	return TRUE;
    }
    return TRUE;
}


/*
 * Special Functions Table.     OLC
 */
const   struct  spec_type       spec_table      [ ] =
{
    /*
     * Special function commands.
     */
    { "spec_breath_any",        	spec_breath_any         },
    { "spec_breath_acid",       	spec_breath_acid        },
    { "spec_breath_fire",       	spec_breath_fire        },
    { "spec_breath_frost",      	spec_breath_frost       },
    { "spec_breath_gas",        	spec_breath_gas         },
    { "spec_breath_lightning",  	spec_breath_lightning   },
    { "spec_cast_adept",        	spec_cast_adept         },
    { "spec_cast_cleric",       	spec_cast_cleric        },
    { "spec_cast_judge",        	spec_cast_judge         },
    { "spec_cast_mage",         	spec_cast_mage          },
    { "spec_cast_undead",       	spec_cast_undead        },
    { "spec_fido",              	spec_fido               },
    { "spec_guard",             	spec_guard              },
    { "spec_janitor",           	spec_janitor            },
    { "spec_mayor",             	spec_mayor              },
    { "spec_poison",            	spec_poison             },
    { "spec_thief",             	spec_thief              },
	{ "spec_eater",			spec_eater },
	{ "spec_gremlin_original",	spec_gremlin_original },
	{ "spec_gremlin_born", 		spec_gremlin_born },
	{ "spec_rogue", 		spec_rogue },
	{ "spec_zombie_lord", 		spec_zombie_lord },
	{ "spec_guard_guardone",	spec_guard_guardone },
	{ "spec_guard_guardtwo",	spec_guard_guardtwo },
	{ "spec_guard_guardthree",	spec_guard_guardthree },
	{ "spec_guard_guardfour",	spec_guard_guardfour },
	{ "spec_guard_guardfive",       spec_guard_guardfive },
      { "spec_guard_guardsix",       spec_guard_guardsix },
      { "spec_guard_guardseven",       spec_guard_guardseven },
      { "spec_guard_guardeight",       spec_guard_guardeight },
      { "spec_guard_guardnine",       spec_guard_guardnine },
      { "spec_guard_guardten",       spec_guard_guardten },
	{ "spec_mobius_original",	spec_mobius_original },
	{ "spec_mobius_hunter",		spec_mobius_hunter },
	{ "spec_rand_new",	        spec_rand_new },
	{ "spec_rand_new2",		spec_rand_new2 },
	{ "spec_rand_new3",		spec_rand_new3 },
	{ "spec_rand_heals",		spec_rand_heals },
	{ "spec_rand_quit",		spec_rand_quit	},
	{ "spec_rand_ft",		spec_rand_ft	},
/*
     * End of list.
     */
    { "",                       0       }
};


/*****************************************************************************
 Name:          spec_lookup
 Purpose:       Given a name, return the appropriate spec fun.
 Called by:     do_mset(act_wiz.c) load_specials,reset_area(db.c)
 ****************************************************************************/
SPEC_FUN *spec_lookup( const char *name )       /* OLC */
{   
    int cmd;

    for ( cmd = 0; *spec_table[cmd].spec_name; cmd++ )  /* OLC 1.1b */
        if ( !str_cmp( name, spec_table[cmd].spec_name ) )
            return spec_table[cmd].spec_fun;
 
    return 0;
}

/*****************************************************************************
 Name:          spec_string
 Purpose:       Given a function, return the appropriate name.
 Called by:     <???>
 ****************************************************************************/
char *spec_string( SPEC_FUN *fun )      /* OLC */
{
    int cmd;

    for ( cmd = 0; *spec_table[cmd].spec_fun; cmd++ )   /* OLC 1.1b */
        if ( fun == spec_table[cmd].spec_fun )
            return spec_table[cmd].spec_name;
    
    return 0;
}
 
