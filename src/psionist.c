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
 *  Psionist by Rand, aka Dominic Jones
 *
 *  12.12.02
 *  Finished: 12.13.02
 *
 *  All work contained herewithin is the property of Rand, and is to be used exclusively
 *  by Fallen Majesty (janksmud.genesismuds.com 9009).
 *
 *  This original psionist class is not to be placed on any other mud other than Fallen
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

void do_psi_eq(CHAR_DATA *ch, char *argument)
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;
    
  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_PSIONIST))
  {
    send_to_char("What?\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of angel armor you wish to make: Rapier Bracer\n\rCollar Ring Plate Helmet Leggings Boots Gauntlets Sleeves Cloak Belt Visor.\n\r",ch);
    return;
  }
  if ( ch->practice < 150 )
  {
    send_to_char("It costs 150 points of primal to create Psionist equipment.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"rapier")) vnum = 33356;
  else if (!str_cmp(arg,"bracer")) vnum = 33346;
  else if (!str_cmp(arg,"collar")) vnum = 33345;
  else if (!str_cmp(arg,"ring")) vnum = 33344;
  else if (!str_cmp(arg,"plate")) vnum = 33347;
  else if (!str_cmp(arg,"helmet")) vnum = 33348;
  else if (!str_cmp(arg,"leggings")) vnum = 33349;
  else if (!str_cmp(arg,"boots")) vnum = 33350;
  else if (!str_cmp(arg,"gauntlets")) vnum = 33351;
  else if (!str_cmp(arg,"sleeves")) vnum = 33352;
  else if (!str_cmp(arg,"cloak")) vnum = 33353;
  else if (!str_cmp(arg,"belt")) vnum = 33354;
  else if (!str_cmp(arg,"visor")) vnum = 33355;
  else
  {
    do_psi_eq(ch,"");
    return;
  } 
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform a Rand.\n\r",ch);
    return;
  }
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  ch->practice -= 150;
  act("$p appears in your hands.",ch,obj,NULL,TO_CHAR, FALSE );
  act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM, FALSE );
  return;
}

void do_enhance(CHAR_DATA *ch, char *argument)
{
  int cost = 2;
  int i;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PSIONIST))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->alignment != -1000) ch->alignment = -1000;
  if (ch->pcdata->rank == 0)
  {
    send_to_char("You are not even a Novice Psionist!\n\r",ch);
    return;
  }
  if (ch->pcdata->rank*3 <= ch->pcdata->powers[PSIONIST_POWER_COUNTER])
  {
    send_to_char("You have gained all powers for your current rank.\n\r",ch);
    return;
  }
  for (i = 1; i < ch->pcdata->rank;i++)
    cost = cost * 1.5;
  if (ch->pcdata->stats[PPOINTS] < 10000 * cost)
  {
    send_to_char("You don't have enough Psionist Points.\n\r",ch);
    return;
  }
  if (ch->pcdata->rank == 1)
  {
    if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_TRUESIGHT))
    {
      send_to_char("You now have the power of truesight.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_TRUESIGHT);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_BODY))
    {
      send_to_char("You now have a strong body!\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_BODY);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_AWE))
    {
      send_to_char("You now have the power to become awe inspiring.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_AWE);
    }
    else
    {
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  else if (ch->pcdata->rank == 2)
  {
    if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS))
    {
      send_to_char("You start moving faster.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDBLAST))
    {
      send_to_char("You may now focus your mental energies into a mindblast.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDBLAST);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MENTAL))
    {
      send_to_char("Your mental energies allow for better spell knowledge.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MENTAL);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  if (ch->pcdata->rank == 3)
  {
    if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENFEEBLE))
    {
      send_to_char("Your mental energies may enfeeble your victim.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENFEEBLE);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MEDDLE))
    {
      send_to_char("Your cunning thoughts wish to meddle in the affairs of others.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MEDDLE);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_WRATH))
    {
      send_to_char("You have the wrath of a true psionist!\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_WRATH);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  if (ch->pcdata->rank == 4)
  {
    if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_TRAVEL))
    {
      send_to_char("Your skill in opening gateways is masterful.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_TRAVEL);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDREAD))
    {
      send_to_char("You may now read the minds of others and hunt them down.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDREAD);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDTRAP))
    {
      send_to_char("A small mindtrap dangles around your neck, awaiting victims.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDTRAP);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  if (ch->pcdata->rank == 5)
  {
    if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_CONTROL))
    {
      send_to_char("You may now control others' thoughts.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_CONTROL);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_PRISON))
    {
      send_to_char("Your mental prisons can trap your foe.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_PRISON);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENERGY))
    {
      send_to_char("You have mastered your mental energies.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENERGY);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  if (ch->pcdata->rank == 6)
  {
    if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_CONFUSE))
    {
      send_to_char("You can confuse your opponents easily.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_CONFUSE);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENTRAPMENT))
    {
      send_to_char("You may now trap people with your mind.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENTRAPMENT);
    }
    else if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ASSASSINATE))
    {
      send_to_char("You may now kill people inside their own minds.\n\r",ch);
      SET_BIT(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ASSASSINATE);
    }
    else
    { 
      send_to_char("Your char is bugged, note an immortal.\n\r",ch);
      return;
    }
  }
  ch->pcdata->powers[PSIONIST_POWER_COUNTER]++;
  ch->pcdata->stats[PPOINTS] -= 10000 * cost;
  return;
}

void do_mindblast(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PSIONIST))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDBLAST))
  {
    send_to_char("Perhaps you should learn that power first.\n\r",ch);
    return;
  }
  if ((victim = ch->fighting) == NULL)
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  if (is_safe(victim,ch) == TRUE) return;
  one_hit(ch,victim,gsn_mindblast,1);
  one_hit(ch,victim,gsn_mindblast,1);
  one_hit(ch,victim,gsn_mindblast,1);
  if (number_range(1,2) == 1)
  one_hit(ch,victim,gsn_mindblast,1);
  if (number_range(1,2) == 1)
  one_hit(ch,victim,gsn_mindblast,1);
    if (!IS_AFFECTED(victim, AFF_FLAMING))
    SET_BIT(victim->affected_by, AFF_FLAMING);
  WAIT_STATE(ch,16);
  return;
}

void do_entrapment( CHAR_DATA *ch, char *argument)
{
  char buf[MSL];
  int door;
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PSIONIST))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENTRAPMENT))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
    if (ch->pcdata->exhaustion > 0)
    {
        sprintf(buf,"You cannot entomb for another %d ticks.\n\r",ch->pcdata->exhaustion);
        stc(buf,ch);
        return;
    }
    if (IS_SET(ch->in_room->room_flags, ROOM_HEALING) || ch->in_room->vnum == 41001)
    {
        stc("You cannot entomb here.\n\r",ch);
        return;
    }
    for (door = 0 ; door < 6 ; door++)
    {
        if (ch->in_room->exit[door] != NULL)
            make_wall(ch->in_room, door, EX_ICE_WALL);
    }


    act("$n calls forth sheets of ice, blocking your path in every direction.", ch, NULL, NULL, TO_ROOM, FALSE );
    send_to_char("Your mental energies summon sheets of ice to entrap your prey.\n\r", ch);
    do_say(ch, "All those who are weak in mind must die!");
    WAIT_STATE(ch, 20);
    ch->pcdata->exhaustion = 24;
    return;
}
  

void do_mindgate(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location; 
  CHAR_DATA *victim;

  one_argument (argument, arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PSIONIST))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_TRAVEL))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Mindgate to whom?\n\r", ch );
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

  if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }
  if (IS_IMMUNE(victim, IMM_TRAVEL) && !IS_NPC(victim))
  {
    send_to_char("I don't think they want you to do that.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
    if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
    {
        stc("Rand should kill you, wussy-assed coward.\n\r",ch);
        return;
    }
    if (IS_SET(victim->in_room->room_flags, ROOM_ARENA))
    {
        stc("Rand should rip your head off, cheater.\n\r",ch);
        return;
    }

  if (ch->move < 1000)
  {
    send_to_char("You don't have the vitality to open a mind gate.\n\r", ch );
    return;
  }
  location = victim->in_room;
  act("You open a gate made from pure mental energy and steps into it.", ch, NULL, NULL, TO_CHAR, FALSE );
  act("$n opens a gate of pure mental energy and steps into it.", ch, NULL, NULL, TO_ROOM, FALSE );
  ch->move -= 1000;
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  act("You step out of the gate.", ch, NULL, NULL, TO_CHAR, FALSE );  
  act("A gate appears from out of nowhere and $n step out of it.", ch, NULL, NULL, TO_ROOM, FALSE );
  return;
}

void do_enfeeble(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA af;
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int sn;
  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PSIONIST))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENFEEBLE))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (is_safe(ch,victim) == TRUE) return;
  WAIT_STATE(ch, 12);
  if (IS_NPC(victim)) 
  {
    stc("You cannot enfeeble the feeble.\n\r",ch);
    return;
  }
  else
  {
    if (victim == ch)
    {
      send_to_char("That doesn't seem like a good idea.\n\r",ch);
      return;
    }
    one_hit(ch,victim, gsn_enfeeble, 1);
    sn = skill_lookup("reserved");
      af.type      = sn;
      af.duration  = 20;
      af.location  = APPLY_DAMROLL;
      af.modifier  = -500;
      af.bitvector = 0;
      affect_to_char(victim, &af );

      af.type      = sn;
      af.duration  = 20;
      af.location  = APPLY_HITROLL;
      af.modifier  = -500;
      af.bitvector = 0;
      affect_to_char(victim, &af );
      send_to_char("You feel very feeble.\n\r",victim);
      send_to_char("Your mental energies enfeeble your victim.\n\r",ch);
      return;
  }
  return;
}
void do_massassinate( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf[MSL];
  int dam = ((ch->spl[1] + ch->spl[2] + ch->spl[3] + ch->spl[4] + ch->spl[0]) * 2);
  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PSIONIST))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ASSASSINATE))
  {
    send_to_char("Perhaps you should learn that power first.\n\r",ch);
    return;
  }
  if (( victim = get_char_world( ch, arg )) == NULL)
  {
     send_to_char( "They aren't here.\n\r", ch );
     return;
  }

  if (IS_NPC(victim))
  {
    stc("Mobs have no brains.\n\r",ch);
    return;
  }
  if (victim->fighting != NULL)
  {
	stc("They are too alert to mentally assassinate!\n\r",ch);
	return;
  }
  if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  if (IS_SET(victim->in_room->room_flags, ROOM_ARENA))
  {
   if (!IS_SET(ch->in_room->room_flags, ROOM_ARENA))
   {
    stc( "Both players must be in the arena to fight.\n\r",ch);
    return;
   }
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
  {
   if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA))
   {
    stc( "Both players must be in the arena to fight.\n\r",ch);
    return;
   }
  }

  if (is_safe(ch,victim) == TRUE) return;
  if (dam <= 4000)
  {
	stc("You do not have the mental energies requied.\n\r",ch);
	return;
  }
  if (ch->mana < 50000)
  {
    stc("You lack the mental vitality to do this.\n\r",ch); 
    return;
  }
  if (ch->bomb_timer > 0)
  {
  stc("You can't bomb again yet.\n\r", ch);
  return;
  }
  if (is_safe( ch, victim )) return;

  ch->fight_timer += 2;
  ch->bomb_timer +=3;

  if (ch->mana >= 50000)
  {
   sprintf(buf,"#CYour mental assassination causes #P$N #Ctremendous #Ppain! #P[#C%d#P]#n",dam);
   act(buf,ch,NULL,victim,TO_CHAR, FALSE );
   sprintf(buf,"#C$n#P's mental assassination causes you #Ctremendous #Ppain! #P[#C%d#P]#n",dam);
   act(buf,ch,NULL,victim,TO_VICT, FALSE );
   sprintf(buf,"#C$n's mental assassination causes #P$N #Ctremendous #Ppain! #P[#C%d#P]#n",dam);
   act(buf,ch,NULL,victim,TO_NOTVICT, FALSE );
   stc("#PYou feel much slower.#n\n\r",victim);
   hurt_person(ch,victim,dam);
   ch->fight_timer += 5;
   ch->mana -= 50000;
   WAIT_STATE(ch, 20);
   WAIT_STATE(victim,16);
  if (ch->spl[0] > 25)
  ch->spl[0] -= 25;
  if (ch->spl[1] > 25)
  ch->spl[1] -= 25;
  if (ch->spl[2] > 25)
  ch->spl[2] -= 25;
  if (ch->spl[3] > 25)
  ch->spl[3] -= 25;
  if (ch->spl[4] > 25)
  ch->spl[4] -= 25;

   return;
  }
 return;
}
void do_meddle(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  AFFECT_DATA af;
  char arg[MAX_INPUT_LENGTH];
  int sn;
  int level = (ch->spl[1] + ch->spl[2] + ch->spl[3] + ch->spl[4] + ch->spl[0]);

  argument = one_argument( argument, arg );


  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PSIONIST))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MEDDLE))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    stc("Mobs have no brains.\n\r",ch);
    return;
  }
  if (level < 3000)
  {
    stc("Your mental energies are not strong enough.\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    stc("You don't have the mana required.\n\r",ch);
     return;
  }
  if (is_safe(ch,victim) == TRUE) return;
   if (number_range(1,2) == 1)
    {
     WAIT_STATE(ch, 14);
     if ( IS_AFFECTED(ch, AFF_WEBBED))
     {
        stc("#GYour web fails to penetrate those stuck around you!#n\n\r",ch);
        return;
     }
     if (IS_AFFECTED(victim, AFF_WEBBED))
     {
        stc("They are already webbed!\n\r",ch);
        return;
     }
     else
     {
     sn = skill_lookup("reserved");

     af.type      = sn;
     af.location  = APPLY_AC;
     af.modifier  = 200;
     af.duration  = number_range(1,2);
     af.bitvector = AFF_WEBBED;
     affect_to_char( victim, &af );
     send_to_char( "#PYou are coated in a sticky web!#n\n\r", victim );
     act("$n is coated in a sticky web!",victim,NULL,NULL,TO_ROOM, FALSE );
     }
    }   
    else
    {
    WAIT_STATE(ch, 20);
    if (!IS_AFFECTED(victim, AFF_SANCTUARY))
     {
        stc("They are not affected by santuary.\n\r",ch);
        return;
     }
    if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
     {
        REMOVE_BIT(victim->affected_by, AFF_SANCTUARY);
     }
     affect_strip( victim, gsn_sanctuary );
     send_to_char( "Your white aura fades.\n\r", victim );
     send_to_char( "#sDESANCT!!!! #yMuhahahaha.\n\r#n", ch );
    }
  ch->mana -= 5000;
  if (ch->spl[0] > 10)
  ch->spl[0] -= 10;
  if (ch->spl[1] > 10)
  ch->spl[1] -= 10;
  if (ch->spl[2] > 10)
  ch->spl[2] -= 10;
  if (ch->spl[3] > 10)
  ch->spl[3] -= 10;
  if (ch->spl[4] > 10)
  ch->spl[4] -= 10;
  return;
}

void do_wrath(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PSIONIST))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_WRATH))
  {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PSIONIST_WRATH_ON] == 1)
  {
    ch->hitroll -= 1000;
    ch->damroll -= 1000;
    send_to_char("You lose the wrath of the Psionist.\n\r",ch);
    ch->pcdata->powers[PSIONIST_WRATH_ON] = 0;
  }
  else
  {
    ch->damroll += 1000;
    ch->hitroll += 1000;
    send_to_char("You now feel the wrath of the Psionist.\n\r",ch);
    ch->pcdata->powers[PSIONIST_WRATH_ON] = 1;
  }
  return;
}
