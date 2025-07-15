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
 *  Paladin by Rand, aka Dominic Jones
 *   
 *  8.21.02
 *  Finished: 12.01.02
 *
 *  All work contained herewithin is the property of Rand, and is to be used exclusively 
 *  by Fallen Majesty (janksmud.genesismuds.com 9009).  
 *
 *  This original paladin class is not to be placed on any other mud other than Fallen
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

void do_blessing( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MSL];
    char arg1 [MAX_INPUT_LENGTH];
	int heal;
    argument = one_argument( argument, arg1 );


    if ( IS_NPC(ch) )
        return;


    if (!IS_CLASS(ch, CLASS_PALADIN))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if ( IS_CLASS(ch, CLASS_PALADIN) && !IS_SET(ch->newbits, NEW_BLADESPIN))
    {
      send_to_char("You need to be a True Paladin.\n\r",ch);
      return;
    }   
    if (ch->mana < 5000)
    {
      send_to_char("You do not have enough energy to provide a blessing.\n\r",ch);
      return;
    }

    if ( arg1[0] == '\0' )
    {
      send_to_char( "Who is in need of your blessing?\n\r", ch );
      return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    WAIT_STATE(ch,14);
    if (IS_NPC(victim))
    {
        stc("Not on mobbies.\n\r",ch);
        return;
     }
    if (is_safe(ch,victim)==TRUE) return;

    if ((victim->fight_timer > 0) && (ch != victim))
    {
      stc("You cannot aid those in battle.\n\r",ch);
      return;
    }
    if (ch->mana >= 5000)
    {
	if (victim->fighting != NULL && (ch != victim))
	{
  	  stc("You may only bless yourself in battle!\n\r",ch);
	  return;
	}
	else if (victim->fighting == NULL && (ch != victim))
	{
	  heal = number_range(3000,5000);
	}
	else if (victim->fighting == NULL && (ch == victim))
	{
	  heal = number_range(5000,7500);
	}
	else if (victim->fighting != NULL && (ch == victim))
	{
	  heal = number_range(3000,5000);
	}
        if (victim->hit >= victim->max_hit)
        {
          sprintf(buf,"%s are healed already.\n\r",victim->pcdata->switchname);
	  stc(buf,ch);
          return;
        }
        else if (victim->hit < victim->max_hit)
        {
          sprintf(buf, "You call forth the blessings of the creator on behalf of %s.\n\r",victim->pcdata->switchname);
          stc(buf,ch);
          sprintf(buf, "The Creator has blessed you with life and longevity.\n\r");
          stc(buf,victim);
          sprintf(buf, "Your wounds magicially seal up, and you feel better.\n\r");
          stc(buf,victim);
          victim->hit += heal;
          if (victim->hit >= victim->max_hit)
          {
             victim->hit = victim->max_hit;
          }
        return;
        }
    }
return;
}

void do_bladeslash(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  OBJ_DATA *obj;

  int x,i;    
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_PALADIN) && !IS_SET(ch->newbits, NEW_BLADESPIN))
  {
    send_to_char("You need to be a True Paladin.\n\r",ch);
    return;
  }
  if ((victim = ch->fighting) == NULL)
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if (((obj = get_eq_char(ch, WEAR_WIELD)) == NULL || obj->value[3] != 1)
    && ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL|| obj->value[3] != 1))
  {
    send_to_char( "You need to wield a slashing weapon.\n\r", ch );
    return;
  }

  if (ch->bomb_timer > 0)
  {
    stc("You have not recovered from your previous bladeslash!\n\r",ch);
	return;
  }
    x = number_range(7,10);
    for(i =x;i>0;i--)
    one_hit(ch,victim,gsn_slash,1);
    if (!IS_AFFECTED(victim, AFF_FLAMING))
    SET_BIT(victim->affected_by, AFF_FLAMING);
    WAIT_STATE(ch,20);
    ch->bomb_timer += 3;
    return;
}

void do_reflect( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  AFFECT_DATA af;
  char arg[MAX_INPUT_LENGTH];
  argument = one_argument (argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_PALADIN) && !IS_SET(ch->newbits, NEW_BLADESPIN))
  {
    send_to_char("You need to be a True Paladin.\n\r",ch);
    return;
  }
  if (get_eq_char(ch, WEAR_SHIELD) == NULL)
  {
    stc("You do not even have a shield to reflect the light!\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    stc("Who do you wish to blind?\n\r", ch);
    return;
  }

  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    stc("They cannot be found.\n\r",ch);
    return;
  }
    if (!IS_NPC(ch) && !IS_NPC(victim) && (is_fighting_player(ch,victim))) return;;
  if (is_safe(ch, victim) == TRUE) return;
  if (xIS_SET(victim->act,PLR_HOLYLIGHT))
  {
    xREMOVE_BIT(victim->act, PLR_HOLYLIGHT);
  }
  if (IS_SET(victim->affected_by, AFF_DETECT_HIDDEN))
    REMOVE_BIT(victim->affected_by, AFF_DETECT_HIDDEN);
  if (IS_SET(victim->affected_by, AFF_DETECT_INVIS))
    REMOVE_BIT(victim->affected_by, AFF_DETECT_INVIS);
  if (!IS_NPC(victim))
  {
    af.type      = skill_lookup("blindness");
    af.location  = APPLY_HITROLL;
    af.modifier  = -1000;
    af.duration  = 60;
    af.bitvector = AFF_BLIND;
    affect_to_char( victim, &af );
    af.type      = skill_lookup("blindness");
    af.location  = APPLY_DAMROLL;
    af.modifier  = -1000;
    af.duration  = 60;
    af.bitvector = AFF_BLIND;
    affect_to_char( victim, &af );
  }
  send_to_char("You maneuver so that the sunlight will reflect off your burnished shield, blinding your enemy.\n\r",ch);
  send_to_char("Your have difficulty seeing past the glare!\n\r",victim);
  set_fighting(ch,victim);
  WAIT_STATE( ch, skill_table[gsn_berserk].beats);
  return;
}

void do_quarter( CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_PALADIN))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_PALADIN) && !IS_SET(ch->newbits, NEW_BLADESPIN))
  {
    send_to_char("You need to be a True Paladin.\n\r",ch);
    return;
  }
  if (!IS_SET(ch->newbits, NEW_JAWLOCK))
  {
    send_to_char("God has commanded no quarter be given.\n\r",ch);
    SET_BIT(ch->newbits, NEW_JAWLOCK);
    return;
  }
  else if (IS_SET(ch->newbits, NEW_JAWLOCK))
  {
    send_to_char("Your allow quarter.\n\r",ch);
    REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
    return;
  }
}

void do_paladineye( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_PALADIN) && !IS_SET(ch->newbits, NEW_BLADESPIN))
  {
    send_to_char("You need to be a True Paladin.\n\r",ch);
    return;
  }

  if (IS_SET(ch->special, SPC_PALADIN_EYE))
  {
    REMOVE_BIT(ch->special, SPC_PALADIN_EYE);
    act("Thanks be to God, they have all perished.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n's prays to $s God, and seems pacified.", ch, NULL, NULL, TO_ROOM, FALSE);
    return;
  }
  SET_BIT(ch->special, SPC_PALADIN_EYE);
  act("God grants you his divine fury.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n grins wickedly, and a smile of pleasure crosses $s lips.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

void do_paladinweapons( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_PALADIN) )
    {
        stc("Huh?\n\r",ch);
        return;
    }
     if (ch->wpn[3] < 700 || ch->wpn[0] < 700 || ch->wpn[1] < 700 || ch->wpn[2] < 700 || 
ch->wpn[4] < 700 || ch->wpn[5] < 700 || ch->wpn[6] < 700 || ch->wpn[7] < 700 || ch->wpn[8] 
< 700 || ch->wpn[9] < 700 || ch->wpn[10] < 700 || ch->wpn[11] < 700 || ch->wpn[12] < 700)
    {
      stc("You have not mastered your weapons yet, young knight.\n\r",ch);
        return;
    }
    if ( IS_SET(ch->newbits, NEW_BLADESPIN) )
    {
        stc("Your focus on your opponent's weapon is less intense.\n\r",ch);
        act("$n looks less intense.",ch,NULL,NULL,TO_ROOM, FALSE);
        REMOVE_BIT(ch->newbits, NEW_BLADESPIN);
	ch->hitroll -= 1000;
	ch->damroll -= 1000;
        return;
    }
    if ( !IS_SET(ch->newbits, NEW_BLADESPIN) )
    {
        stc("You concentrate on the weapons wielded by your foe.\n\r",ch);
        act("$n's seems to be concentrating on something.",ch,NULL,NULL,TO_ROOM, FALSE);
        SET_BIT(ch->newbits, NEW_BLADESPIN);
        ch->hitroll += 1000;
        ch->damroll += 1000;
        return;
    }
  return;
}

void do_paladintravel( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_PALADIN) )
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Charge to where?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not of this world.\n\r", ch );
	return;
    }
    
   if( ch->move < 1000) 
    {
     stc( "#7You do not have enough movement to go charging about#n.\n\r",ch);
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
  if ( room_is_noft(victim->in_room ) && ch->fight_timer >0 )
  {
    send_to_char( "That room cannot be accessed with a fight timer.\n\r", ch );
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

    act("You follow the path to $N.", ch, NULL, victim, TO_CHAR, FALSE);
    act("$n charges off, looking for something.", ch, NULL, victim, TO_ROOM, FALSE);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    ch->move -= 1000;
    act("$n charges onto the scene, right in ront of $N.", ch, NULL, victim, TO_NOTVICT, FALSE);
    act("$n charges into your face.", ch, NULL, victim, TO_VICT, FALSE);
    do_look(ch,"auto");
    return;
}
