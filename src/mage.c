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


/**************************
 * New mage class by Jobo * 
 **************************/

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

void do_reveal(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *ich;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?!?.\n\r",ch);
    return;
  }
    if (( ch->in_room->vnum >= 420000) && (ch->in_room->vnum <= 420099))
    {
        send_to_char( "You cannot do this in this area.\n\r",ch);
        return;
    }

  if (ch->mana < 5000)
  {
    send_to_char("You don't have the mystical energies to do this.\n\r",ch);
    return;
  } 
  act( "$n mumbles a few words, and you are suddenly blinded by a flash.", ch, NULL, NULL, 
TO_ROOM, FALSE );   
  send_to_char( "You reveal everything hidden in the room.\n\r", ch );
  for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
  {
    if (ich==ch || ich->trust > 6) continue;
    affect_strip ( ich, gsn_invis );
    affect_strip ( ich, gsn_mass_invis );
    affect_strip ( ich, gsn_sneak );
    if (IS_SET(ich->affected_by, AFF_HIDE))      REMOVE_BIT(ich->affected_by, AFF_HIDE);
    if (IS_SET(ich->affected_by, AFF_INVISIBLE)) REMOVE_BIT(ich->affected_by, AFF_INVISIBLE);
    if (IS_SET(ich->affected_by, AFF_SNEAK))     REMOVE_BIT(ich->affected_by, AFF_SNEAK);
    if (xIS_SET(ich->act, PLR_WIZINVIS))         xREMOVE_BIT(ich->act, PLR_WIZINVIS);
    if (xIS_SET(ich->act, PLR_HIDE))             xREMOVE_BIT(ich->act, PLR_HIDE);
    if (IS_SET(ich->affected_by, AFF_SHIFT))     REMOVE_BIT(ich->affected_by, AFF_SHIFT);
    if (xIS_SET(ich->extra, EXTRA_EARTHMELD))    xREMOVE_BIT(ich->extra, EXTRA_EARTHMELD);
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE)) REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    if (IS_AFFECTED(ch, AFF_PEACE)) REMOVE_BIT(ch->affected_by, AFF_PEACE);
    if (IS_AFFECTED(ch,AFF_ETHEREAL)) REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

    if (IS_CLASS(ich, CLASS_DROW) && IS_SET(ich->newbits, NEW_DARKNESS))
    {
      REMOVE_BIT(ich->newbits, NEW_DARKNESS);
//      REMOVE_BIT(ich->in_room->room_flags, ROOM_TOTAL_DARKNESS);
    }
    send_to_char("You are suddenly very visibly.\n\r",ich);
  }
  ch->mana -= 5000;
  return;
}

void do_magearmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
 	if(!IS_IMMORTAL(ch))
	{
    if( !IS_CLASS(ch, CLASS_MAGE) )
    {
      send_to_char("What?\n\r",ch);
      return;
    }
	}
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of mage armor you wish to make: Dagger Staff\n\rRing Collar Robe Cap Leggings Boots Gloves Sleeves Cape Belt Bracer Mask.\n\r",ch);
    return;
  }
  if ( ch->practice < 150 )
  {
    send_to_char("It costs 150 points of primal to create mage equipment.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"mask")) vnum = 33013;
  else if (!str_cmp(arg,"belt")) vnum = 33011;
  else if (!str_cmp(arg,"cape")) vnum = 33010;
  else if (!str_cmp(arg,"ring")) vnum = 33002;
  else if (!str_cmp(arg,"collar")) vnum = 33003;
  else if (!str_cmp(arg,"robe")) vnum = 33004;
  else if (!str_cmp(arg,"cap")) vnum = 33005;
  else if (!str_cmp(arg,"leggings")) vnum = 33006;
  else if (!str_cmp(arg,"boots")) vnum = 33007;
  else if (!str_cmp(arg,"gloves")) vnum = 33008;
  else if (!str_cmp(arg,"sleeves")) vnum = 33009;
  else if (!str_cmp(arg,"bracer")) vnum = 33012;
  else if (!str_cmp(arg,"staff")) vnum = 33000;
  else if (!str_cmp(arg,"dagger")) vnum = 33001;
  else
  {
    do_magearmor(ch,"");
    return;
  }
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform a God.\n\r",ch);
    return;
  }
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  ch->practice -= 150;
  act("$p appears in your hands.",ch,obj,NULL,TO_CHAR, FALSE);
  act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM, FALSE);
  return;
}

void do_chaosmagic (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  char arg1[MAX_STRING_LENGTH];
  AFFECT_DATA af;

  CHAR_DATA *victim;
  int sn, random, level;

  argument=one_argument(argument,arg);
  argument=one_argument(argument,arg1);
  level = ch->spl[RED_MAGIC];

  if( !str_cmp( arg, "self" ) )
  {
  sprintf( arg, ch->name );
  }

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_POWER_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_POWER_LICH) && ch->pcdata->powers[CHAOS_MAGIC] < 1)
  {
    send_to_char("Not yet, not yet.\n\r",ch);
    return;
  }
  if (ch->mana < 5000)
  {
    send_to_char("You need more mana.\n\r",ch);
    return;
  }
  if (arg1[0] == '\0' || arg[0] == '\0')
  {
    send_to_char("Syntax : chaosmagic <target> <hurt/help>.\n\r", ch);
    return;
  }
  if (!str_cmp(arg1, "hurt"))
  {
    if (arg[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
    else if ((victim = get_char_room(ch, arg)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (!IS_NPC(victim))
    {
    if (is_safe(ch,victim)) return;
    }
    if (IS_NPC(victim))
    {
	stc("They are not affected!\n\r",ch);
	return;
    }
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
     act("$n is coated in a sticky web!",victim,NULL,NULL,TO_ROOM, FALSE);
     }
    return;
    }
    else
    {
    if (!IS_AFFECTED(victim, AFF_SANCTUARY))
     {
        stc("They are not affected by santuary.\n\r",ch);
	WAIT_STATE(ch, 16);
        return;
     }
    if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
     {
        REMOVE_BIT(victim->affected_by, AFF_SANCTUARY);
     }
     affect_strip( victim, gsn_sanctuary );
     send_to_char( "Your white aura fades.\n\r", victim );
     send_to_char( "#sDESANCT!!!! #yMuhahahaha.\n\r#n", ch );
     WAIT_STATE(ch,16);
     if (number_range(1,2)==1) 
	WAIT_STATE(victim,16);
     return;
    }
  ch->mana -= 5000;
  return;
  }
 
 else if (!str_cmp(arg1, "help"))
  {
    if (str_cmp(arg, ch->name))
    {
	stc("Not on others.\n\r",ch);
	return;
    }
    if ((victim = get_char_room(ch, arg)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }

  if (IS_NPC(victim))
  {
  stc("Why help a mobile?\n\r",ch);
  return;
  }
  if (ch->position == POS_FIGHTING || victim->position == POS_FIGHTING) random = 5;
  else if (IS_CLASS(ch, CLASS_MAGE)) random = number_range(1,4);
  else if (IS_CLASS(ch, CLASS_POWER_LICH)) random = number_range(1,4);

  if (random == 1) sn = skill_lookup("spirit kiss");
  else if (random == 2) sn = skill_lookup("darkblessing");
  else if (random == 3) sn = skill_lookup("frenzy");
  else if (random == 4) sn = skill_lookup("bless");
  else if (random == 5) sn = skill_lookup("imp heal");
  else sn = 0;
  if (sn > 0) (*skill_table[sn].spell_fun) (sn,level,ch,victim);
  if (ch->position == POS_FIGHTING || victim->position == POS_FIGHTING) ch->mana -= 10000;
  else if (ch->position != POS_FIGHTING || victim->position != POS_FIGHTING) ch->mana -= 5000;
  if (IS_CLASS(ch, CLASS_POWER_LICH))
  {
   if  ((ch->position == POS_FIGHTING) || (victim->position == POS_FIGHTING))
   {    WAIT_STATE(ch, 18);}
   else
   WAIT_STATE(ch, 10);
  }	
  if (IS_CLASS(ch, CLASS_MAGE))
  {
  if ((ch->position == POS_FIGHTING) || (victim->position == POS_FIGHTING))
  {WAIT_STATE(ch,18);}
  else
  WAIT_STATE(ch,10);
  }
  return;
}
 else if (str_cmp(arg1, "help") || str_cmp(arg1, "hurt"))
 {
   send_to_char("Syntax : chaosmagic <target> <hurt/help>.\n\r", ch);
    return;
 }
}
/* Syntax : chant <type> <color> <target>
 * ex. chant bless red self 
 */
void do_chant (CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  AFFECT_DATA af;
  int i, dam, count, sn;
  int red_magic = ch->spl[RED_MAGIC];
  int blue_magic = ch->spl[BLUE_MAGIC];
  int green_magic = ch->spl[GREEN_MAGIC];
  int purple_magic = ch->spl[PURPLE_MAGIC];
  int yellow_magic = ch->spl[YELLOW_MAGIC];
  int magic_power = ch->spl[RED_MAGIC] + ch->spl[BLUE_MAGIC] + ch->spl[GREEN_MAGIC] + ch->spl[PURPLE_MAGIC] + ch->spl[YELLOW_MAGIC];
    
  argument=one_argument(argument,arg1);
  strcpy(arg2,argument);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_POWER_LICH))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0] == '\0')
  {  
    send_to_char("Syntax : chant <bless/curse/damage/heal> <target>.\n\r", ch);
    return;
  }  
  if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) 
  {
    magic_power +=250;
    red_magic +=50;
    yellow_magic +=50;
    purple_magic +=50;
    green_magic +=50;
    blue_magic +=50;
  }
        if ( IS_SET(ch->in_room->room_flags, ROOM_NO_CHANT))
    {
        send_to_char( "You are unable to chant, its a no chant room.\n\r", ch );
        return;
    }

  if (!str_cmp(arg1, "heal"))
  {
    if (arg2[0] == '\0') victim = ch;
    else if ((victim = get_char_room(ch, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana < 5000)
    {
      send_to_char("You need 5000 mana.\n\r",ch);
      return;
    }    
    if (is_safe(ch,victim) && (ch != victim))
    {
    stc("You cannot heal them.\n\r",ch);
    return;
    }

    act("$n channels lifeforce from the five elements into $N.",ch,NULL,victim,TO_NOTVICT, FALSE);
    act("You channel lifeforce from the five elements into $N.",ch,NULL,victim,TO_CHAR, 
FALSE);
    act("$n channels lifeforce from the five elements into you.",ch,NULL,victim,TO_VICT, 
FALSE);
    if (ch->fight_timer >= 0)
     {
      int heal = 0;
      if (ch->race == 0)
      {
      victim->hit += (blue_magic * number_range(10,15));
      WAIT_STATE(ch,16);
      ch->mana -= 5000;
      }
      else if (ch->race > 0)
      {
      heal += (blue_magic * ch->race);
      if (heal > (blue_magic * number_range(10,15)))
      heal = heal;
      else if (heal <= (blue_magic * number_range(10,15)))
      heal = (blue_magic * number_range(10,15));
      victim->hit += heal;
      if (ch->fight_timer > 0)
      WAIT_STATE(ch,18);
      if (ch->fight_timer == 0)
      WAIT_STATE(ch,16);
      ch->mana -= 5000;
      }
     }
    return;
  }
  else if (!str_cmp(arg1, "damage"))
  {
    if (arg2[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
    else if ((victim = get_char_room(ch, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (IS_NPC(victim))
    {
     if (ch->mana <5000)
     {
      send_to_char("You need 5000 mana.\n\r",ch);
      return;
     }
    }
    if (!IS_NPC(victim))
     {
     if (ch->mana <10000)
     {
      send_to_char("You need 10000 mana.\n\r",ch);
      return;
     }
    }

    if (victim == ch)
    {
      send_to_char("You really don't want to hurt yourself.\n\r", ch );
      return;
    }
    if (is_safe(ch, victim)) return;
    if (!IS_NPC(ch) && !IS_NPC(victim) && (is_fighting_player(ch,victim))) return;
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    if (IS_AFFECTED(ch, AFF_PEACE)) REMOVE_BIT(ch->affected_by, AFF_PEACE);
    if (victim->position > POS_STUNNED)
    {
      if (victim->fighting == NULL) set_fighting(victim, ch);
      if (ch->fighting == NULL) set_fighting(ch, victim);
    }
    if (IS_CLASS(ch, CLASS_POWER_LICH))
    WAIT_STATE(ch,16);
    if (IS_CLASS(ch, CLASS_MAGE))
    WAIT_STATE(ch,16);
    if (IS_NPC(ch)) ch->mana -= 5000;
    if (!IS_NPC(ch)) ch->mana -= 10000;
    dam = number_range(red_magic*5, red_magic*6);
    dam += char_damroll(ch);
      if (dam > 0) 
      {
      if (dam > ch->damcap[0])
      dam = ch->damcap[0];
      if ( IS_AFFECTED(victim, AFF_SANCTUARY)) dam /= 2;

      }
 
    sprintf(buf1,"$n calls for the forces of #Rhell#n to destroy $N [#C%d#n]" , dam);
    sprintf(buf2,"You call for the forces of #Rhell#n to destroy $N [#C%d#n]" , dam);
    sprintf(buf3,"$n calls for the forces of #Rhell#n to destroy you [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT, FALSE);
    act(buf2,ch,NULL,victim, TO_CHAR, FALSE);
    act(buf3,ch,NULL,victim, TO_VICT, FALSE);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(blue_magic*5, blue_magic*6);
    dam += char_damroll(ch);
      if (dam > 0)
      {
      if (dam > ch->damcap[0])
      dam = ch->damcap[0];
      if ( IS_AFFECTED(victim, AFF_SANCTUARY)) dam /= 2;

      }


    sprintf(buf1,"$n summons a supernatural #Lstorm#n to strike $N with lightning [#C%d#n]" , dam);
    sprintf(buf2,"You summon a supernatural #Lstorm#n to strike $N with lightning [#C%d#n]" , dam);
    sprintf(buf3,"$n summons a supernatural #Lstorm#n to strike you with lightning [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT, FALSE);
    act(buf2,ch,NULL,victim, TO_CHAR, FALSE);
    act(buf3,ch,NULL,victim, TO_VICT, FALSE);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(green_magic*5, green_magic*6);
    dam += char_damroll(ch);
      if (dam > 0)
      {
      if (dam > ch->damcap[0])
      dam = ch->damcap[0];
      if ( IS_AFFECTED(victim, AFF_SANCTUARY)) dam /= 2;

      }


    sprintf(buf1,"$n calls for #Gthorns#n to grow from the earth and pierce $N [#C%d#n]" , dam);
    sprintf(buf2,"You call for #Gthorns#n to grow from the earth and pierce $N [#C%d#n]" , dam);
    sprintf(buf3,"$n calls for #Gthorns#n to grow from the earth and pierce You [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT, FALSE);
    act(buf2,ch,NULL,victim, TO_CHAR, FALSE);
    act(buf3,ch,NULL,victim, TO_VICT, FALSE);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(purple_magic*5, purple_magic*6);
    dam += char_damroll(ch);
      if (dam > 0)
      {
      if (dam > ch->damcap[0])
      dam = ch->damcap[0];
      if ( IS_AFFECTED(victim, AFF_SANCTUARY)) dam /= 2;

      }

    sprintf(buf1,"$n calls on the powers of the #punderdark#n to shatter $N's bones [#C%d#n]" , dam);
    sprintf(buf2,"You call on the powers of the #punderdark#n to shatter $N's bones [#C%d#n]" , dam);
    sprintf(buf3,"$n calls on the powers of the #punderdark#n to shatter your bones [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT, FALSE);
    act(buf2,ch,NULL,victim, TO_CHAR, FALSE);
    act(buf3,ch,NULL,victim, TO_VICT, FALSE);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(yellow_magic*5, yellow_magic*6);
    dam += char_damroll(ch);
      if (dam > 0)
      {
      if (dam > ch->damcap[0])
      dam = ch->damcap[0];
      if ( IS_AFFECTED(victim, AFF_SANCTUARY)) dam /= 2;

      }


    sprintf(buf1,"$n spews forth a shower of #yacid#n striking $N [#C%d#n]" , dam);
    sprintf(buf2,"you spew forth a shower of #yacid#n striking $N [#C%d#n]" , dam);
    sprintf(buf3,"$n spews forth a shower of #yacid#n striking you [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT, FALSE);
    act(buf2,ch,NULL,victim, TO_CHAR, FALSE);
    act(buf3,ch,NULL,victim, TO_VICT, FALSE);
    hurt_person(ch, victim, dam);
    if(!IS_NPC(victim))
       ch->fight_timer += 5;
    return;
  }
  else if (!str_cmp(arg1, "bless"))
  {
    if ((victim = get_char_room(ch, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (!IS_NPC(victim) && (victim != ch))
    {
	stc("Your magics fail to reach them\n\r",ch);
	return;
     }
    if (ch->mana <2500)
    {
      send_to_char("You need 2500 mana.\n\r",ch);
      return;
    }
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    count = 0;
    for (i = 0; i<5 ; i++)
    {
      if (i==0) {sn = skill_lookup("purple sorcery");dam=purple_magic;}
      if (i==1) {sn = skill_lookup("yellow sorcery");dam=yellow_magic;}
      if (i==2) {sn = skill_lookup("green sorcery");dam=green_magic;}
      if (i==3) {sn = skill_lookup("red sorcery");dam=red_magic;}
      if (i==4) {sn = skill_lookup("blue sorcery");dam=blue_magic;}
      if (is_affected(victim, sn))
        continue;
      
      count++;
      
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HIT;
      af.modifier  = dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_AC;
      af.modifier  = -dam/2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MOVE;
      af.modifier  = dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_MANA;
      af.modifier  = dam*2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_HITROLL;
      af.modifier  = dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = dam/4;
      af.location  = APPLY_DAMROLL;
      af.modifier  = dam/3;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type	   = sn;
      af.duration  = dam/4;
      af.location  = APPLY_SAVING_SPELL;
      af.modifier  = 0 - 75;
      af.bitvector = 0;
      affect_to_char(victim, &af);
    }
    ch->mana -= count * 500;
    WAIT_STATE(ch,6);
    sprintf(buf1, "You bless $N with the power of the elements.");
    sprintf(buf2, "The power of the five elements fills your body.");
    act(buf1, ch, NULL, victim, TO_CHAR, FALSE);
    act(buf2, ch, NULL, victim, TO_VICT, FALSE);
    return;
  }
  else if (!str_cmp(arg1, "curse"))
  {
    if ((victim = get_char_room(ch, arg2)) == NULL)
    {
      send_to_char("They are not here.\n\r", ch );
      return;
    }
    if (ch->mana <2500)
    {
      send_to_char("You need 2500 mana.\n\r",ch);
      return;
    }
    if (victim->level < 3)
    {
      send_to_char("Not on mortal, thier bodies cant handles the strain....poor little buggers.\n\r",ch);
      return;
    }
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    count = 0;
    for (i = 0;i<5;i++)
    {
      if (i==0) {sn = skill_lookup("purple sorcery");dam=purple_magic;}
      if (i==1) {sn = skill_lookup("yellow sorcery");dam=yellow_magic;}
      if (i==2) {sn = skill_lookup("green sorcery");dam=green_magic;}
      if (i==3) {sn = skill_lookup("red sorcery");dam=red_magic;}
      if (i==4) {sn = skill_lookup("blue sorcery");dam=blue_magic;}
      if (is_affected(victim, sn))
        continue;
      
      count++;

      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_HIT;
      af.modifier  = -dam;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_AC;
      af.modifier  = dam;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_MOVE;
      af.modifier  = -dam;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_MANA;
      af.modifier  = -dam;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_HITROLL;
      af.modifier  = -dam/2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_DAMROLL;
      af.modifier  = -dam/2;
      af.bitvector = 0;
      affect_to_char(victim, &af);
      af.type      = sn;
      af.duration  = 10;
      af.location  = APPLY_SAVING_SPELL;
      af.modifier  = 50;
      af.bitvector = 0;
      affect_to_char(victim, &af);
    }
    ch->mana -= count * 500;
    WAIT_STATE(ch,6);
    sprintf(buf1, "You curse $N with the power of the elements.");
    sprintf(buf2, "The power of the five elements runs through your body.");
    act(buf1, ch, NULL, victim, TO_CHAR, FALSE);
    act(buf2, ch, NULL, victim, TO_VICT, FALSE);
    return;      
  }
  send_to_char("Syntax : chant <bless/curse/damage/heal> <target>.\n\r", ch);
  return;
}

/* Syntax : invoke <list/learn/spell> <none/power/power>
 * ex. invoke learn stoneskin
 * ex. invoke spell stoneskin
 */
void do_invoke (CHAR_DATA *ch, char *argument)
{
/* char->pcdata[PINVOKE] : [0;10]
 * merc.h, ZERG power replaced.
 */

  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];

  argument=one_argument(argument,arg1);
  strcpy(arg2,argument);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg1[0]== '\0')
  {
    if(ch->pcdata->powers[PINVOKE] > 0)
      send_to_char("* You have the power to teleport yourself anywhere.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 1)
      send_to_char("* You know how to invoke mageshields.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 2)
      send_to_char("* You have the power to scry on far away places.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 3)
      send_to_char("* You can discharge your shields in a shower of power.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 4)
      send_to_char("* A deflector shield helps you.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 5)
      send_to_char("* You have the power to turn your skin into solid steel.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 7)
      send_to_char("* You have mastered the power of illusions.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 8)
      send_to_char("* You can invoke the power of the beast.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 6 && ch->pcdata->powers[PINVOKE] < 10)
      send_to_char("* You have a deeper understanding of magic.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] > 9)
      send_to_char("* You have mastered the art of magic.\n\r",ch);
    if(ch->pcdata->powers[PINVOKE] >10)
      send_to_char("* You have the power of manashield.\n\n",ch);
    sprintf(buf,"invokes Learned (%d).\n\r", ch->pcdata->powers[PINVOKE]);
    send_to_char(buf,ch);
    return;  
  }
/*If the mage wants to try to activate them all at once-HUW*/
        if (!str_cmp(arg1,"all"))
    {
        do_invoke(ch,"deflector");
        do_invoke(ch,"manashield");
        do_invoke(ch,"mageshield");
        do_invoke(ch,"steelshield");
        do_invoke(ch,"illusions");
        do_invoke(ch,"beast");
        stc("All available invokes turned on, if sufficient primal was available.\n\r", ch);
    }

  if (!str_cmp(arg1,"learn"))
  {
    int cost = (ch->pcdata->powers[PINVOKE]+1) * 20;
    if ( ch->pcdata->powers[PINVOKE] >= 11 )
    {
      send_to_char("You have already gained all the known invokes.\n\r",ch);
      return;
    }
    else if (cost > ch->practice)
    {
      send_to_char("you don't have enough primal.\n\r",ch);
      return;
    }
    else
    {
      ch->pcdata->powers[PINVOKE] +=1;
      ch->practice -= cost;
      send_to_char("You gain a deeper understanding of the mystical powers.\n\r",ch);
    }
    return;
  }
  if (!str_cmp(arg1,"steelshield"))
  {
    int shieldcost=5;

    if (ch->pcdata->powers[PINVOKE] < 6) /* 6 is just a test */
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_STEELSHIELD) && ch->practice >= shieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
      ch->practice -= shieldcost;
      send_to_char("Your skin turns to solid steel.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_STEELSHIELD))
    {
      send_to_char("Your skin cannot become any harder.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to activate steelskin.\n\r",ch);
      return;
    }
  }
  if (!str_cmp(arg1,"beast"))
  {
    int beastcost=10;

    if (ch->pcdata->powers[PINVOKE] < 9) /* 9 is just a test */
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_BEAST) && ch->practice >= beastcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_BEAST);
      ch->practice -= beastcost;
      send_to_char("Your let your inner beast take control over your actions.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_BEAST))
    {
      send_to_char("You feel the beast inside of you roar, but nothing else happens.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 10 primal to let the beast take over.\n\r",ch);
      return;
    }
  }
  if (!str_cmp(arg1,"illusions"))
  {
    int illusioncost=5;

    if (ch->pcdata->powers[PINVOKE] < 8) /* 8 is just a test */
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_ILLUSIONS) && ch->practice >= illusioncost)
    {
      SET_BIT(ch->itemaffect, ITEMA_ILLUSIONS);
      ch->practice -= illusioncost;
      send_to_char("You conjure several images of yourself, making it impossibly to tell which is really you.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_ILLUSIONS))
    {
      send_to_char("You cannot summon any more illusions.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to invoke the illusions of yourself.\n\r",ch);
      return;
    }
    return;
  }
  if (!str_cmp(arg1,"manashield"))
  {
    int shieldcost=5;

    if (ch->pcdata->powers[PINVOKE] < 11) /* 11 is just a test */
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_MANASHIELD) && ch->practice >= shieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_MANASHIELD);
      ch->practice -= shieldcost;
      send_to_char("A shield of mana appears in the air, preventing people from hitting you.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_MANASHIELD))
    {
      send_to_char("You cannot summon any more shields.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to summon a shield.\n\r",ch);
      return;
    }
  }
  if (!str_cmp(arg1,"deflector"))
  {
    int shieldcost=5;

    if (ch->pcdata->powers[PINVOKE] < 5) /* 5 is just a test */
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_DEFLECTOR) && ch->practice >= shieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_DEFLECTOR);
      ch->practice -= shieldcost;
      send_to_char("A shield of mana appears in the air, preventing people from hitting you.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_DEFLECTOR))
    {
      send_to_char("You cannot summon any more shields.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("it costs 5 primal to summon a shield.\n\r",ch);
      return;
    }
  }

  if (!str_cmp(arg1,"mageshield") || !str_cmp(arg1,"shield"))
  {
    int shieldcost=5;

    if (ch->pcdata->powers[PINVOKE] < 2) /* 2 is just a test */
    {
      send_to_char("You don't have that invoke yet.\n\r",ch);
      return;
    }
    if (!IS_SET(ch->newbits, NEW_MULTIARMS))
      SET_BIT(ch->newbits, NEW_MULTIARMS);
    if (!IS_ITEMAFF(ch, ITEMA_MAGESHIELD) && ch->practice>=shieldcost)
    {
      SET_BIT(ch->itemaffect, ITEMA_MAGESHIELD);
      ch->practice -= shieldcost;
      send_to_char("A layer of mystic shields envelopes your body.\n\r",ch);
      return;
    }
    else if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
    {
      send_to_char("You are allready shielded.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("The cost is 5 primal.\n\r",ch);
      return;
    }
  }
  send_to_char("invoke <learn/mageshield/manashield/steelshield/illusions/beast/deflector>.\n\r",ch);
  return;
}

/* syntax : magics
 * shows all class powers in effect
 */
void do_magics (CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
    send_to_char("* Your skin is covered in a layer of mystical energies.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_BEAST))
    send_to_char("* Your inner beast is in control of your actions.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_STEELSHIELD))
    send_to_char("* Your skin is solid steel.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_MANASHIELD))
    send_to_char("* A shield of mana pulsates about you, defending you.\n\r",ch);
  if (IS_ITEMAFF(ch, ITEMA_ILLUSIONS))
    send_to_char("* Several images of you stand by your side.\n\r",ch);
  return;
}

/* syntax : discharge
 * (drops all elementalshields in one BIG blast)
 */
void do_discharge (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  int magic_power = ch->spl[RED_MAGIC] + ch->spl[BLUE_MAGIC] + ch->spl[GREEN_MAGIC] + ch->spl[PURPLE_MAGIC] + ch->spl[YELLOW_MAGIC];
  int dam;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PINVOKE] < 4) /* 4 is just a test */
  {
    send_to_char("You don't have that invoke yet.\n\r",ch);
    return;
  }
  if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) 
    magic_power +=250;
  if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
  {
    REMOVE_BIT(ch->itemaffect, ITEMA_MAGESHIELD);
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
      vch_next = vch->next_in_room;
      if (vch != ch)
      {
        if (is_safe(ch,vch)) continue;
        if ((mount = ch->mount) != NULL)
          if (mount == vch) continue;
        dam = number_range(magic_power*7/2, magic_power*9/2) + ch->damroll*2;
        if (IS_AFFECTED(vch, AFF_SANCTUARY)) dam *= 0.5;
        sprintf(buf1,"$n's mystical shields explodes in a shower of flames striking $N [#C%d#n]",dam);
        sprintf(buf2,"Your mystical shields explodes in a shower of flames striking $N [#C%d#n]",dam);
        sprintf(buf3,"$n's mystical shields explodes in a shower of flames striking you [#C%d#n]",dam);
        act(buf1,ch,NULL,vch, TO_NOTVICT, FALSE);
        act(buf2,ch,NULL,vch, TO_CHAR, FALSE);
        act(buf3,ch,NULL,vch, TO_VICT, FALSE);
        hurt_person(ch,vch,dam);
        if (vch->position > POS_STUNNED)
        {
          if (vch->fighting == NULL) set_fighting(vch, ch);
          if (ch->fighting == NULL) set_fighting(ch, vch);
        }
      }
    }
  }
  return;
}


/* syntax : teleport <target>
 * ex : teleport satan
 */
void do_teleport (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;
  CHAR_DATA *victim;

  one_argument (argument, arg);
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r", ch );
    return;
	}
  if (ch->pcdata->powers[PINVOKE] < 1) /* 1 is just a test */
  {
    send_to_char("You don't have that invoke yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL) 
  {
    send_to_char("Teleport to whom?\n\r", ch );
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

  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
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

	location = victim->in_room;
  if (ch->mana < 250) 
  {
    send_to_char("You don't have the mana to cast teleport.\n\r", ch );
    return;
  }
	act("You utter a single powerword.", ch, NULL, NULL, TO_CHAR, FALSE);
	act("$n utters a strange sounding word and disappers.", ch, NULL, NULL, TO_ROOM, 
FALSE);
	ch->mana -= 250;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");
	act("You materialize.", ch, NULL, NULL, TO_CHAR, FALSE);
	act("$n suddenly appear from out of nowhere.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

/* syntax : objectgate <object>
 * ex. objectgate 2.bells
 */
void do_objectgate (CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  char arg1[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg1);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_POWER_LICH))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if (IS_CLASS(ch, CLASS_POWER_LICH) && ch->pcdata->powers[CON_LORE] < 1)
  {
    send_to_char("But you don't know how to do that silly.\n\r",ch);
    return;
  }
  if (arg1[0] == '\0')
  {
    send_to_char("What object do you want to attempt to gate?\n\r",ch);
    return;
  }
  if ((obj = get_obj_world(ch, arg1)) == NULL)
  {
    send_to_char("No such object.\n\r",ch);
    return;
  }
  if (IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->quest, QUEST_RELIC) || 
IS_SET(obj->quest, QUEST_RUNE))
  {
    send_to_char("You cannot grab artifacts or relics.\n\r",ch);
    return;
  }
  if (obj->carried_by != NULL)
  {
    send_to_char("Someone is holding the item",ch);
    return;
  }
  if(obj->in_room == NULL)
  {
    send_to_char("It seems to be inside something.",ch);
    return;
  }
  if (!IS_SET(obj->wear_flags, ITEM_TAKE))
  {
    send_to_char("This item is too big.\n\r",ch);
    return;
  }
  if( IS_SET(obj->in_room->room_flags, ROOM_ASTRAL))
  {
    send_to_char( "You can't find it's room.\n\r",ch);
    return;
  }
  if(IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    send_to_char( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  obj_from_room( obj );
  obj_to_char( obj, ch );
  send_to_char("The item appears in your hand.\n\r",ch);
  return;
}


/*added by Adjani*/
void do_infuse(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  AFFECT_DATA paf;
  char arg[MAX_STRING_LENGTH];
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  one_argument(argument, arg);
  if ((obj = get_obj_carry(ch, arg)) == NULL)
  {
    send_to_char("You dont have that item.\n\r",ch);
    return;
  }
  if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR)
  {
    send_to_char("You can only affect weapons or armor!\n\r", ch);
    return;
  }
  if (IS_SET(obj->quest, QUEST_INFUSE))
  {
    send_to_char("This item is already infused!\n\r", ch);
    return;
  }
  if (str_cmp(ch->name, obj->questowner))
  {
    send_to_char("You can only infuse items you own.\n\r",ch);
    return;
  }

  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_HITROLL;
  paf.modifier       = 15;
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);
  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_DAMROLL;
  paf.modifier       = 15;
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);
  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_AC;
  paf.modifier       = -50;
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);
  paf.type           = 0;
  paf.duration       = -1;
  paf.location       = APPLY_MANA;
  paf.modifier       = 1000;
  paf.bitvector      = 0;
  affect_to_obj(obj, &paf);

  SET_BIT(obj->quest, QUEST_INFUSE);
  send_to_char("You infuse more power into your armor.\n\r", ch);
  return;
}
 /*added by Adjani*/
void do_bomb(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  int dam;

  one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_MAGE))
  {
     send_to_char("Huh?\n\r", ch );
     return;
  }

  if (( victim = get_char_world( ch, arg )) == NULL)
  {
     send_to_char( "They aren't here.\n\r", ch );
     return;
  }
  if (ch->mana < 5000)
  {
  send_to_char("You need 5000 mana.\n\r",ch);
  return;
  }
  if ( victim == ch )
  {
  send_to_char( "You really don't want to bomb yourself!\n\r", ch );
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
  if (victim->in_room->area != ch->in_room->area)
  {
    stc( "You cannot reach that area, only your own.\n\r",ch);
    return;
  }

  if (ch->bomb_timer > 0) 
  { 
  stc("You can't bomb again yet.\n\r", ch);
  return; 
  }
  if (is_safe( ch, victim )) return;
  dam = number_range(4000,7000) + ch->damroll*2;
  if (IS_SET(ch->newbits, NEW_SUPREME))
   dam *= 2;
  if (!IS_NPC(victim))
  {
    ch->fight_timer += 2;
    ch->bomb_timer +=3;
    if ( IS_AFFECTED(victim, AFF_SANCTUARY) ) dam /= 2;
    hurt_person(ch,victim,dam);
    if (ch->position == POS_FIGHTING)
    {
      WAIT_STATE( ch, 10 );
    }
    else if (ch->position == POS_STANDING)
    {
      WAIT_STATE( ch, 5);
    }
  }
  if (IS_NPC(victim))
  {
    ch->bomb_timer +=2;
    if ( IS_AFFECTED(victim, AFF_SANCTUARY) ) dam /= 2;
    hurt_person(ch,victim,dam);
    if (ch->position == POS_FIGHTING)
    {
      WAIT_STATE( ch, 12 );
    }
    else if (ch->position == POS_STANDING)
    {
      WAIT_STATE( ch, 5);
    }
  }

  sprintf(buf1,"#R$n #0raises their hands to the sky and emits a great flaming #Rf#rir#Reb#ral#Rl#0, which strikes #c$N#0!#n [#r%d#n]",dam);
  sprintf(buf2,"#0You raise your hands to the sky and emit a great flaming #Rf#rir#Reb#ral#Rl#0, which strikes #c$N#0!#n [#r%d#n]",dam);
  sprintf(buf3,"#R$n#0's great flaming #Rf#rir#Reb#ral#Rl#0 appears from nowhere and strikes you hard! #n[#r%d#n]",dam);
  act(buf1,ch,NULL,victim, TO_NOTVICT, FALSE);
  act(buf2,ch,NULL,victim, TO_CHAR, FALSE);
  act(buf3,ch,NULL,victim, TO_VICT, FALSE);

  return;
}

/* 
 * for affect spells
 */

void do_bewitch (CHAR_DATA *ch, char *argument)
{
  /*ged*/
  WAIT_STATE(ch, 2);
  return;
}


/*
void do_mgate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char_to_room( create_mobile( get_mob_index(MOB_VNUM_VAMPIRE) ),	ch->in_room );
    return;
}
*/
