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
#include "garou.h"

void do_staredown( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( ch->power[DISC_WERE_OWL] < 5 )
    {
      stc("Your power in owl is not great enough.\n\r",ch);
      return;
    }

    if ( ( victim = get_char_room( ch, arg) ) == NULL )
    {
      stc("They aren't here.\n\r",ch);
      return;
    }

    arg[0] = UPPER(arg[0]);    

    if ( ch->fighting == NULL )
    {
      stc("You must be fighting to use the gift of Staredown.\n\r",ch);
      return;
    }

    if ( victim->fighting == NULL )
    {
      sprintf(buf, "%s is not fighting anyone.", arg);
      stc(buf, ch);
      return;
    }  

    WAIT_STATE( ch, 12 );

    if ( IS_NPC(victim) )
    {
      if ( ch->power[DISC_WERE_OWL]  < 6 )
      {
	  if ( number_range( 1, 3 ) != 1 )
        {
          act("You stare deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_CHAR, FALSE);
          act("$n stares deeply into your eyes.",ch,NULL,victim,TO_VICT, FALSE);
          act("$n stares deeply into $N's eyes, but nothing happens.", ch,NULL,victim,TO_NOTVICT, FALSE);
	    return;
        }
      }

      act("You stare into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_CHAR, FALSE);
      act("$n stares deeply into your eyes, and you are filled with terror.",ch,NULL,victim,TO_VICT, FALSE);
      act("$n stares deeply into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_NOTVICT, FALSE);
      do_flee( victim, "" );
      return;
    }

    else
    {
      if ( ch->power[DISC_WERE_OWL] < 7 )
      {
        if ( number_range( 1, 4 ) != 2 )
        {
          act("You stare deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_CHAR, FALSE);
          act("$n stares deeply into your eyes.",ch,NULL,victim,TO_VICT, FALSE);
          act("$n stares deeply into $N's eyes, but nothing happens.",ch,NULL,victim,TO_NOTVICT, FALSE);
	    return;
        }
      }

      act("You stare into $N's eyes, and $E flees in terror.",ch,NULL,victim,TO_CHAR, 
FALSE);
      act("$n stares deeply into your eyes, and you are filled with terror.",
ch,NULL,victim,TO_VICT, FALSE);
      act("$n stares deeply into $N's eyes, and $E flees in terror.",
ch,NULL,victim,TO_NOTVICT, FALSE);
      do_flee( victim, "" );
      return;
    }

  return;

}

void do_disquiet( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }

    if ( ch->power[DISC_WERE_OWL] < 6 )
    {
      stc("Your power in owl is not great enough.\n\r",ch);
      return;
    }

    if ( ( victim = get_char_room(ch, arg) ) == NULL ) 
    {
      stc("They are not here.\n\r",ch);
      return;
    }

    if ( is_safe( ch, victim) ) return;

    if ( ch->gnosis[GCURRENT] < 1 )
    {
      stc("You need one point of gnosis power to use the gift of Disquiet.\n\r",ch);
      return;
    }

    af.type      = skill_lookup("reserved");
    af.duration  = ch->power[DISC_WERE_OWL];
    af.location  = APPLY_DAMROLL;
    af.modifier  = -( ch->power[DISC_WERE_OWL] * 5 );
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = skill_lookup("reserved");
    af.duration  = ch->power[DISC_WERE_OWL];
    af.location  = APPLY_HITROLL;
    af.modifier  = -( ch->power[DISC_WERE_OWL] * 5 );
    af.bitvector = 0;
    affect_to_char( victim, &af );

    ch->gnosis[CURRENT]--;
    act("Your eyes glow a sinister red as you glare at $N.",ch,NULL,victim,TO_CHAR, FALSE);
    act("$n's eyes glow a sinister red as $e glares at you.\n\rYou feel odd.",
ch,NULL,victim,TO_VICT, FALSE);
    act("$n's eyes glow a sinister red as $e glares at $N.",ch,NULL,victim,TO_NOTVICT, 
FALSE);
    WAIT_STATE(ch,4);    
    return;

}

void do_reshape(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    OBJ_DATA *obj; 

    argument=one_argument(argument,arg1);
    argument=one_argument(argument,arg2);
    strcpy(arg3,argument);

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch,CLASS_WEREWOLF))
    {
      send_to_char("Huh?\n\r",ch);
      return;
    }

    if (ch->power[DISC_WERE_OWL] < 7)
    {
      stc("Your power in owl is not great enough.\n\r",ch);
      return;
    }

    if ( ( obj = get_obj_carry( ch,arg1 ) ) == NULL )
    {
      send_to_char("You dont have that item.\n\r",ch);
      return;
    }

    if (!(!str_cmp(arg2,"short") || !str_cmp(arg2,"name")))
    {
      send_to_char("syntax : reshape <item> <short/name> <newname>.\n\r",ch);
      return;
    }

    if (strlen(arg3) > 100 || strlen(arg3) < 3)
    {
      send_to_char("The name should be between 3 and 100 characters.\n\r",ch);
      return;
    }
    if (has_bad_chars(ch, arg3))
    {
      send_to_char("Illegal chars.\n\r",ch);
      return;
    }
    if (IS_SET(obj->quest, QUEST_ARTIFACT))
    {
      send_to_char("Not on artifacts.\n\r",ch);
      return;
    }
        if (obj->pIndexData->vnum == 420000 || obj->pIndexData->vnum == 220004)
      {
        send_to_char("I don't think the Guardian would like that.\n\r",ch);
        return;
      }

    if (!str_cmp(arg2,"name"))
    {
	free_string(obj->name);
	obj->name = str_dup(arg3);
	obj->questmaker = str_dup(ch->name);
    }
    if (!str_cmp(arg2,"short"))
    {
	free_string(obj->short_descr);
	obj->short_descr=str_dup(arg3);
	obj->questmaker = str_dup(ch->name);
    }

    send_to_char("Ok.\n\r",ch);
    return;
}

void do_cocoon( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( ch->power[DISC_WERE_OWL] < 8 )
    {
      stc("Your power in owl is not great enough.\n\r",ch);
      return;
    }


    if ( IS_GAR1(ch, WOLF_COCOON) )
    {
      stc("Your cocoon breaks and falls to the ground.\n\r",ch);
      act("$n's cocoon breaks and falls to the ground.",ch,NULL,NULL,TO_ROOM, FALSE);
      REMOVE_BIT(ch->garou1, WOLF_COCOON);
      return;
    } 

    if ( ch->gnosis[GCURRENT] < 2 )
    {
      stc("You do not have enough gnosis to use Cocoon.\n\r",ch);
      return;
    }

    else
    {
      stc("Your body is surrounded by a thick, opaque epidermis.\n\r",ch);
      act("$n's body is surrounded by a thick, opaque epidermis.",ch,NULL,NULL,TO_ROOM, 
FALSE);
      SET_BIT(ch->garou1, WOLF_COCOON);
      ch->gnosis[GCURRENT]--;
      return;
    }
  return;
}

/*
 * Metis Gifts
 * Level One  - Gift of the Porcupine - Quills
 * Level Two  - Burrow          - Like Demon Travel
 * Level Three- Eyes of the Cat - Nightsight
 * Level Four - Wither Limb	- Withers a limb, easier on NPCs
 * Level Five - Totem Gift	- Random, Special Power. Ooo.
 */

void do_quills( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
   	stc("Huh?\n\r",ch);
	return;
    }

    if ( ch->power[DISC_WERE_HAWK] < 5 )
    {
      stc("Your power in hawk is not great enough.\n\r",ch);
	return;
    }

    if ( IS_SET(ch->newbits, NEW_QUILLS) )
    {
        stc("Your quills soften and twist into fur.\n\r",ch);
	act("$n's quills soften and revert back to fur.",ch,NULL,NULL,TO_ROOM, FALSE);
        REMOVE_BIT(ch->newbits, NEW_QUILLS);
	return;
    }

    if ( !IS_SET(ch->newbits, NEW_QUILLS) )
    {
        stc("Your fur becomes bristly and sharp.\n\r",ch);
	act("$n's fur becomes bristly and sharp.",ch,NULL,NULL,TO_ROOM, FALSE);
	SET_BIT(ch->newbits, NEW_QUILLS);
	return;
    }
  return;
}

void do_burrow( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( ch->power[DISC_WERE_HAWK] < 6 )
    {
      stc("Your power in hawk is not great enough.\n\r",ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char("Burrow to who?\n\r", ch);
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They are not here.\n\r", ch );
        return;
    }

   /* if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    } */

    if (IS_IMMUNE(victim, IMM_TRAVEL))
    {
        send_to_char("I don't think they want you to do that.\n\r",ch);
        return;
    }        

    if ( victim == ch )
    {
        send_to_char( "You cannot burrow to yourself.\n\r", ch);
        return;
    }

   /* if ( victim->level != LEVEL_AVATAR || !IS_CLASS(victim, CLASS_WEREWOLF))
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }*/

    if (victim->in_room == NULL)
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }
    if (ch->position == POS_FIGHTING)
    {
	send_to_char( "You cannot burrow from combat.\n\r", ch);
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

    if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
    {
    stc( "Your cannot find your victim!.\n\r",ch);
    return;
    }

    if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
    {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
    }
    if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
    {
        stc("Rand should kill you, you wussy-assed coward.\n\r",ch);
        return;
    }
    if (IS_SET(victim->in_room->room_flags, ROOM_ARENA))
    {
        stc("Rand should rip your head off, cheater.\n\r",ch);
        return;
    }

    send_to_char("You burrow into the ground.\n\r",ch);
    act("$n burrows into the ground.",ch,NULL,NULL,TO_ROOM, FALSE);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    send_to_char("You burrow out of the ground.\n\r",ch);
    act("$n burrows out of the ground.",ch,NULL,NULL,TO_ROOM, FALSE);
    return;
}
  
// Eyes of the Cat - Nightsight.  clan.c

void do_wither( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    bool blah = FALSE;
    int chance = 0;

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( ch->power[DISC_WERE_HAWK] < 7 )
    {
      stc("Your power in hawk is not great enough.\n\r",ch);
	return;
    }

    if ( ch->gnosis[GCURRENT] < 1 )
    {
	stc("You do not have enough gnosis power stored to use the gift of Wither Limb.\n\r",ch);
	return;
    }
    if ( arg[0] == '\0' )
    {
	stc("Whose limb do you wish to wither?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	stc("They aren't here.\n\r",ch);
	return;
    }

    if (!IS_NPC(victim) && !IS_NPC(ch) && (ch != victim) && (is_safe(ch,victim) == TRUE) ) return;
    if ( IS_ARM_R(victim, LOST_ARM) && IS_ARM_L(victim, LOST_ARM) )
    {
	stc("They have already lost both their arms.\n\r",ch);
	return;
    }
 
    WAIT_STATE(ch, 14);
    ch->gnosis[CURRENT] -= 1;
    if ( IS_NPC( victim ) )
    {
      chance = number_range( 45, 55 );

      if ( number_percent( ) > chance )
      {
	stc("Nothing happened.\n\r",ch);
	return;
      }

       if ( !IS_ARM_R(victim, LOST_ARM) )
         blah = FALSE;
       else blah = TRUE;

	 if ( !blah )
         {
 	  if (!IS_ARM_R(victim,LOST_ARM))
	      SET_BIT(victim->loc_hp[3],LOST_ARM);
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
	      SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	      REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
	  act("You concentrate deeply and $N's arm starts to wither!",
ch,NULL,victim,TO_CHAR, FALSE);
	  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",
ch,NULL,victim,TO_NOTVICT, FALSE);
	  act("$n gazes evilly at $N, and $S arm begins withering!",
ch,NULL,victim,TO_VICT, FALSE);
	  make_part(victim,"arm");
	  if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	  {
	      if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
		  take_item(victim,obj);
	  }
	  if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
	      take_item(victim,obj);
	  return;
         }
       
	 else if ( blah )
	 {
 	  if (!IS_ARM_L(victim,LOST_ARM))
	      SET_BIT(victim->loc_hp[2],LOST_ARM);
	  else blah = TRUE;
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
	      SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	      REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
	  act("You concentrate deeply and $N's arm starts to wither!",
ch,NULL,victim,TO_CHAR, FALSE);
	  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT, FALSE);
	  act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT, FALSE);
	  make_part(victim,"arm");
	  if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	  {
	      if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
		  take_item(victim,obj);
	  }
	  if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
	      take_item(victim,obj);
	  return;
        }
    }

    else if ( !IS_NPC(victim) )
    {
	chance = 60;
      if ( number_percent( ) > chance )
      {
	act("You gaze intently at $N.\n\rNothing happens.",ch,NULL,victim,TO_CHAR, FALSE);
	act("$n gazes eerily at you.\n\rNothing happens.",ch,NULL,victim,TO_VICT, FALSE);
	act("$n gazes eerily at $N.",ch,NULL,victim,TO_NOTVICT, FALSE);
	return;
      }


       if ( !IS_ARM_R(victim, LOST_ARM) )
         blah = FALSE;
       else blah = TRUE;

	 if ( !blah )
         {
 	  if (!IS_ARM_R(victim,LOST_ARM))
	      SET_BIT(victim->loc_hp[3],LOST_ARM);
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
	      SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	      REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
	  act("You concentrate deeply and $N's arm starts to wither!",
ch,NULL,victim,TO_CHAR, FALSE);
	  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT, FALSE);
	  act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT, FALSE);
	  make_part(victim,"arm");
	  if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	  {
	      if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
		  take_item(victim,obj);
	  }
	  if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
	      take_item(victim,obj);
	  return;
	 }

	 else if ( blah )
	 {
 	  if (!IS_ARM_L(victim,LOST_ARM))
	      SET_BIT(victim->loc_hp[2],LOST_ARM);
	  else blah = TRUE;
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
	      SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	      REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
	  act("You concentrate deeply and $N's arm starts to wither!",
ch,NULL,victim,TO_CHAR, FALSE);
	  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",
ch,NULL,victim,TO_NOTVICT, FALSE);
	  act("$n gazes evilly at $N, and $S arm begins withering!",
ch,NULL,victim,TO_VICT, FALSE);
	  make_part(victim,"arm");
	  if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	  {
	      if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
		  take_item(victim,obj);
	  }
	  if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
	      take_item(victim,obj);
	  if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
	      take_item(victim,obj);
	  return;
        }
      }
  return;
}

/*
 * Ahroun Gifts
 */

void do_razorclaws(CHAR_DATA *ch, char *argument)
{

	if ( ch->power[DISC_WERE_WOLF] < 4 )
        {
	    stc("Huh?\n\r",ch);
	    return;
	}

   if ( !IS_VAMPAFF(ch, VAM_CLAWS) )
   {
	stc("Your talons aren't even extended!\n\r",ch);
	return;
   }

   if ( IS_GAR1(ch, WOLF_RAZORCLAWS ) )
   {
      stc("Your claws lose their razor-sharp edge.\n\r",ch);
      act("$n's claws lose their razor-sharp edge.",ch,NULL,NULL,TO_ROOM, FALSE);
      REMOVE_BIT(ch->garou1, WOLF_RAZORCLAWS);
   }
   else
   {
	send_to_char("You hone your claws to a razor sharpness.\n\r",ch);
	act("$n hones $s claws to a razor sharpness.",ch, NULL, NULL, TO_ROOM, FALSE);
	SET_BIT( ch->garou1, WOLF_RAZORCLAWS );
   }

  return;

}

