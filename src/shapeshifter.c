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


/*****************************************************
 * Shapeshifters, another class by Jobo for Dystopia *
 *****************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> /* unlink() */
#include <stdarg.h>
#include <assert.h>
#include "player.h"
#include "merc.h"

void do_stomp( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  int limb;

  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_SHAPESHIFTER) )
  {
    send_to_char("What?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[BULL_LEVEL] < 5)
  {
    send_to_char("Your bull form is not strong enough.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SHAPE_FORM] != BULL_FORM)
  {
    send_to_char("You must be in bull form to STOMP!!!#n\n\r",ch);
    return;
  }
  if (( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  act("You throw $N to the ground and trambles $S to a bloody pulp.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n throws you to the ground and #RSTOMPS#n on you.",ch,NULL,victim,TO_VICT, FALSE);
  act("$n throws $N to the ground and trambles $S to a bloody pulp.",ch,NULL,victim,TO_NOTVICT, FALSE);
  hurt_person(ch,victim,500);
  limb = number_range(1,4);
  if (number_percent() > 70)  {
  if (limb == 1)
  {
    if (!IS_ARM_R(victim,LOST_ARM))            SET_BIT(victim->loc_hp[3],LOST_ARM);
    if (!IS_BLEEDING(victim,BLEEDING_ARM_R))   SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
    if (IS_BLEEDING(victim,BLEEDING_HAND_R))   REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
    if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)     take_item(victim,obj);
    if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)    take_item(victim,obj);
    if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)    take_item(victim,obj);
    if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)  take_item(victim,obj);
    if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL) take_item(victim,obj);
  }
  else if (limb > 1)
  {
    if (!IS_ARM_L(victim,LOST_ARM))            SET_BIT(victim->loc_hp[2],LOST_ARM);
    if (!IS_BLEEDING(victim,BLEEDING_ARM_L))   SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
    if (IS_BLEEDING(victim,BLEEDING_HAND_L))   REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
    if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)     take_item(victim,obj);
    if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)     take_item(victim,obj);
    if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)    take_item(victim,obj);
    if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)  take_item(victim,obj);
    if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL) take_item(victim,obj);
  }
  }
  WAIT_STATE(ch,16);
  return;
}

void do_tigercurse( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  AFFECT_DATA af;
  char arg [MAX_INPUT_LENGTH];
  int sn;
  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_SHAPESHIFTER) )
  {
    send_to_char("What?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[TIGER_LEVEL] < 4)
  {
    send_to_char("Your hydra form is not strong enough.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SHAPE_FORM] != TIGER_FORM)
  {
    send_to_char("You must be in tiger form to curse.\n\r",ch);
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );  
    return;
  }  
  if ( ch == victim )
  {
    send_to_char( "Not on yourself.\n\r", ch );
    return;
  }
    if (IS_NPC(victim))
    {
        stc("They are not affected!\n\r",ch);
        return;
    }

  act("You let the forces of the forest envelope $N.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n stares at you long and hard, you feel different!",ch,NULL,victim,TO_VICT, FALSE);
  act("$n stares at $N for a long time, what the hell?",ch,NULL,victim,TO_NOTVICT, FALSE);
    if (number_range(1,2) == 1)
    {
     WAIT_STATE(ch, 24);
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
    }

    else
    {
    WAIT_STATE(ch, 24);
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
  return;
}

void do_breath( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  int i,x;
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_SHAPESHIFTER) )
  {
    send_to_char("What?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SHAPE_FORM] != HYDRA_FORM)
  {
    send_to_char("You must be in hydra form to breath fire.\n\r",ch);
    return;
  }
  if (( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }  
  if (!IS_NPC(victim)) ch->fight_timer += 2;
    x = ch->lstatus + 5;
    for(i =x;i>0;i--)
    one_hit(ch,victim,gsn_badbreath,0);
  WAIT_STATE(ch, 18);
  return;
}

void do_phase( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_SHAPESHIFTER) )
  {
    send_to_char("What?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[TIGER_LEVEL] < 5)
  {
    send_to_char("Your tiger form is not strong enough.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SHAPE_FORM] != TIGER_FORM)
  {
    send_to_char("You must be in tiger form to phase.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PHASE_COUNTER] > 0)
  {
    send_to_char("Your to tired to phase.\n\r",ch);
    return;
  }
  ch->pcdata->powers[PHASE_COUNTER] = 10;
  act("You phase halfway out of existance.",ch,NULL,NULL,TO_CHAR, FALSE);
  act("$n phases halfway out of existance.",ch,NULL,NULL,TO_ROOM, FALSE);
  return;
}

void do_shapearmor( CHAR_DATA *ch, char *argument )
{  
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_SHAPESHIFTER) )
  {
    send_to_char("What?\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of shapeshifter armor you wish to make: Knife Kane\n\rBands Necklace Ring Jacket Helmet Pants Boots Gloves Shirt Cloak Belt Visor.\n\r",ch);
    return;
  }
  if ( ch->practice < 150 )
  {
    send_to_char("It costs 150 points of primal to create shapeshifter equipment.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"knife")) vnum = 33160;
  else if (!str_cmp(arg,"kane")) vnum = 33161;
  else if (!str_cmp(arg,"bands")) vnum = 33162;
  else if (!str_cmp(arg,"necklace")) vnum = 33163;
  else if (!str_cmp(arg,"ring")) vnum = 33164;
  else if (!str_cmp(arg,"jacket")) vnum = 33165;
  else if (!str_cmp(arg,"helmet")) vnum = 33166;
  else if (!str_cmp(arg,"pants")) vnum = 33167;
  else if (!str_cmp(arg,"boots")) vnum = 33168;
  else if (!str_cmp(arg,"gloves")) vnum = 33169;
  else if (!str_cmp(arg,"shirt")) vnum = 33170;
  else if (!str_cmp(arg,"cloak")) vnum = 33171;
  else if (!str_cmp(arg,"belt")) vnum = 33172;
  else if (!str_cmp(arg,"visor")) vnum = 33173;
  else   
  {
    do_shapearmor(ch,"");
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

void do_mistwalk(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;
  CHAR_DATA *victim;

  one_argument (argument, arg);
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if (ch->pcdata->powers[SHAPE_POWERS] < 2)
  {
    send_to_char("You need level 2 shiftpowers.\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Mistwalk to whom?\n\r", ch );
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
        stc("Rand should kill you, you wussy-assed coward.\n\r",ch);
        return;
    }
    if (IS_SET(victim->in_room->room_flags, ROOM_ARENA))
    {
        stc("Rand should rip your head off, cheater.\n\r",ch);
        return;
    }

  location = victim->in_room;
  if (ch->move < 250)
  {
    send_to_char("You are too tired to reform into mist.\n\r", ch );
    return;
  }
  act("You reform into thin mist and seep into the floor.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n reforms into thin mist and seeps into the floor.", ch, NULL, NULL, TO_ROOM, FALSE);
  ch->move -= 250;
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  act("You seep up from the floor and reforms into your body.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("A fine mist seeps up from the floor and reforms into $n.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

void do_hatform (CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SHAPE_POWERS] < 3)
  {
    send_to_char("You need level 3 shiftpowers.\n\r",ch);
    return;
  }
  if (IS_AFFECTED(ch,AFF_POLYMORPH))
  {
    send_to_char( "You must be in human form.\n\r", ch);
    return;
  }
  if (ch->fight_timer>0)
  {
    send_to_char("Not with a fighttimer.\n\r",ch);
    return;
  }
    if (ch->quit_timer > 0)
    {
        stc("Not with a quit timer!\n\r",ch);
       return;
    }

  if ((obj = create_object(get_obj_index(33175),60)) == NULL)
  {
    send_to_char( "Object bug, tell Jank about it.\n\r", ch);
    return;
  }
  obj_to_room(obj,ch->in_room);
  act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM, FALSE);
  act("You transform into $p and fall to the ground.",ch,obj,NULL,TO_CHAR, FALSE);
  ch->pcdata->obj_vnum = 33175;
  obj->chobj = ch;
  ch->pcdata->chobj = obj;
  SET_BIT(ch->affected_by, AFF_POLYMORPH);
  xSET_BIT(ch->extra, EXTRA_OSWITCH);
  free_string(ch->morph);
  ch->morph = str_dup(obj->short_descr);
  return;
}

void do_shift (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  one_argument( argument, arg);
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("What form do you want to shift to? [Tiger/Hydra/Faerie/Bull/Human]\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SHAPE_COUNTER] > 35)
  {
    send_to_char("Your to tired from all this formshifting.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"tiger"))
  {
    if (ch->pcdata->powers[SHAPE_FORM] != 0) do_shift(ch, "human");
    if (IS_SET(ch->affected_by, AFF_POLYMORPH))
    {
      send_to_char("Not while polymorphed.\n\r",ch);
      return;
    }
    act("Your shape blurs and you reform into a huge tiger.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n blurs and reforms into a huge tiger.", ch, NULL, NULL, TO_ROOM, FALSE);
    ch->pcdata->powers[SHAPE_FORM]=TIGER_FORM;
    ch->pcdata->powers[SHAPE_COUNTER]+= 14;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    if (ch->pcdata->powers[SHAPE_COUNTER] < 10)
    {
      send_to_char("#CYour wounds mend and close.\n\r#n",ch);
      ch->hit += UMIN(5000,ch->max_hit*0.1);
      if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
    }
    sprintf(buf,"%s the huge phase tiger",ch->name);
    free_string(ch->morph);
    ch->morph = str_dup(buf);
    WAIT_STATE(ch,16);
    return;
  }
  else if (!str_cmp(arg,"hydra"))  
  {
    if (ch->pcdata->powers[SHAPE_FORM] != 0) do_shift(ch, "human");
    if (IS_SET(ch->affected_by, AFF_POLYMORPH))  
    { 
      send_to_char("Not while polymorphed.\n\r",ch);
      return;
    }
    act("Your shape blurs and you reform into a horrific hydra.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n blurs and reforms into a horrific hydra.", ch, NULL, NULL, TO_ROOM, FALSE);
    ch->pcdata->powers[SHAPE_FORM]=HYDRA_FORM;
    ch->pcdata->powers[SHAPE_COUNTER]+=10;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    if (ch->pcdata->powers[SHAPE_COUNTER] < 15)
    {
      send_to_char("#CYour wounds mend and close.\n\r#n",ch);
      ch->hit += UMIN(5000,ch->max_hit*0.1);
      if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
    }
    sprintf(buf,"%s the horrific hydra",ch->name);
    free_string(ch->morph);
    ch->morph = str_dup(buf);
    WAIT_STATE(ch,16);
    return;
  }
  else if (!str_cmp(arg,"bull"))
  {
    if (ch->pcdata->powers[SHAPE_FORM] != 0) do_shift(ch, "human");
    if (IS_SET(ch->affected_by, AFF_POLYMORPH))  
    { 
      send_to_char("Not while polymorphed.\n\r",ch);
      return;
    }
    act("Your shape blurs and you reform into a black bull.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n blurs and reforms into a black bull.", ch, NULL, NULL, TO_ROOM, FALSE);
    ch->pcdata->powers[SHAPE_FORM]=BULL_FORM;
    ch->pcdata->powers[SHAPE_COUNTER]+=10;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    if (ch->pcdata->powers[SHAPE_COUNTER] < 15)
    {
      send_to_char("#CYour wounds mend and close.\n\r#n",ch);
      ch->hit += UMIN(5000,ch->max_hit*0.1);
      if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
    }
    sprintf(buf,"%s the black bull",ch->name);
    free_string(ch->morph);
    ch->morph = str_dup(buf);
    WAIT_STATE(ch,16);
    return;
  }
  else if (!str_cmp(arg,"faerie"))
  {
    if (ch->pcdata->powers[SHAPE_FORM] != 0) do_shift(ch, "human");
    if (IS_SET(ch->affected_by, AFF_POLYMORPH))  
    { 
      send_to_char("Not while polymorphed.\n\r",ch);
      return;
    }
    act("Your shape blurs and you reform into a small pixie.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n blurs and reforms into a small pixie.", ch, NULL, NULL, TO_ROOM, FALSE);
    ch->pcdata->powers[SHAPE_FORM]=FAERIE_FORM;
    ch->pcdata->powers[SHAPE_COUNTER]+=10;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    if (ch->pcdata->powers[SHAPE_COUNTER] < 15)
    {
      send_to_char("#CYour wounds mend and close.\n\r#n",ch);
      ch->hit += UMIN(5000,ch->max_hit*0.1);
      if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
    }
    sprintf(buf,"%s the small pixie",ch->name);
    free_string(ch->morph);
    ch->morph = str_dup(buf);
    WAIT_STATE(ch,16);
    return;
  }
  else if (!str_cmp(arg,"human"))
  {
    if (ch->pcdata->powers[SHAPE_FORM] == 0 && !IS_SET(ch->affected_by, AFF_POLYMORPH))
    {
      send_to_char("Your allready in human form.\n\r",ch);
      return;
    }
/*
    if (IS_SET(ch->pcdata->powers[SHAPE_FORM], TIGER_FORM))
    {
      ch->damroll -= 1000;
      ch->hitroll -= 1000;
      ch->armor   += 500;
    }
    else if (IS_SET(ch->pcdata->powers[SHAPE_FORM], FAERIE_FORM))
    { 
      ch->damroll -= 200;
      ch->hitroll -= 1000;
      ch->armor   += 3000;
    }
    else if (IS_SET(ch->pcdata->powers[SHAPE_FORM], BULL_FORM))
    {
      ch->damroll -= 1500;
      ch->hitroll -= 1500;
      ch->armor   += 1200;
    }
    else if (IS_SET(ch->pcdata->powers[SHAPE_FORM], HYDRA_FORM))
    {
      ch->damroll -= 1200;
      ch->hitroll -= 1200;
      ch->armor   += 1200;
    }*/
    ch->pcdata->powers[SHAPE_FORM]=0;
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    send_to_char("Your return to your human form.\n\r",ch);
    free_string(ch->morph);
    ch->morph=str_dup("");
    return;
  }
  else
  {
    send_to_char("That is not a valid form.\n\r",ch);
    return;
  }
}

void do_formlearn (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  
  one_argument( argument, arg);
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    sprintf(buf, "Current levels : Tiger[%d], Hydra[%d], Faerie[%d], Bull[%d], Shiftpowers[%d]\n\r",
      ch->pcdata->powers[TIGER_LEVEL], ch->pcdata->powers[HYDRA_LEVEL],
      ch->pcdata->powers[FAERIE_LEVEL], ch->pcdata->powers[BULL_LEVEL], ch->pcdata->powers[SHAPE_POWERS]);
    send_to_char(buf,ch);
    send_to_char("Improve what form? [Tiger/Hydra/Faerie/Bull/Shiftpowers]\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"hydra"))
  {
    if (ch->pcdata->powers[HYDRA_LEVEL] > 4)
    {
      send_to_char("Your Hydra form is complete.\n\r",ch);
      return;
    }
    if (ch->practice < (80*ch->pcdata->powers[HYDRA_LEVEL] + 80))
    {
      send_to_char("You don't have enough primal to improve this form.\n\r",ch);
      return;
    }
    if (ch->pcdata->powers[HYDRA_LEVEL] == 0)
      send_to_char("You now have 2 head in Hydra form.\n\r",ch);
    else if (ch->pcdata->powers[HYDRA_LEVEL] == 1)
      send_to_char("You now have 3 heads Hydra form.\n\r",ch);
    else if (ch->pcdata->powers[HYDRA_LEVEL] == 2)
      send_to_char("You have stronger limbs in Hydra form.\n\r",ch);
    else if (ch->pcdata->powers[HYDRA_LEVEL] == 3)
      send_to_char("You now have 4 heads in Hydra form.\n\r",ch);
    else if (ch->pcdata->powers[HYDRA_LEVEL] == 4)
      send_to_char("You now have 5 heads in Hydra form.\n\r",ch);
    ch->pcdata->powers[HYDRA_LEVEL]++;
    ch->practice -= ch->pcdata->powers[HYDRA_LEVEL]*80;
    return;
  }
  else if (!str_cmp(arg,"tiger"))
  {
    if (ch->pcdata->powers[TIGER_LEVEL] > 4)
    {
      send_to_char("Your Tiger form is complete.\n\r",ch);
      return;
    }
    if (ch->practice < (80*ch->pcdata->powers[TIGER_LEVEL] + 80))
    {
      send_to_char("You don't have enough primal to improve this form.\n\r",ch);
      return;
    }
    if (ch->pcdata->powers[TIGER_LEVEL] == 0)
      send_to_char("You now have razor sharp claws in Tiger form.\n\r",ch);
    else if (ch->pcdata->powers[TIGER_LEVEL] == 1)
      send_to_char("You now have long pointy teeths in Tiger form.\n\r",ch);
    else if (ch->pcdata->powers[TIGER_LEVEL] == 2)
      send_to_char("You now have stronger paws in Tiger form.\n\r",ch);
    else if (ch->pcdata->powers[TIGER_LEVEL] == 3)
      send_to_char("You now have the power to roar in Tiger form.\n\r",ch);
    else if (ch->pcdata->powers[TIGER_LEVEL] == 4)
      send_to_char("You now have the power to phase in Tiger form.\n\r",ch);
    ch->pcdata->powers[TIGER_LEVEL]++;
    ch->practice -= ch->pcdata->powers[TIGER_LEVEL]*80;
    return;
  }
  else if (!str_cmp(arg,"faerie"))
  {
    if (ch->pcdata->powers[FAERIE_LEVEL] > 4)
    {
      send_to_char("Your Faerie form is complete.\n\r",ch);
      return;
    }
    if (ch->practice < (80*ch->pcdata->powers[FAERIE_LEVEL] + 80))
    {
      send_to_char("You don't have enough primal to improve this form.\n\r",ch);
      return;
    }
    if (ch->pcdata->powers[FAERIE_LEVEL] == 0)
      send_to_char("You are increadibly fast and can dodge most attacks.\n\r",ch);
    else if (ch->pcdata->powers[FAERIE_LEVEL] == 1)
      send_to_char("You have small wings in faerie form and can cast spells in any form.\n\r",ch);
    else if (ch->pcdata->powers[FAERIE_LEVEL] == 2)
      send_to_char("Your control over the forces of magic increases.\n\r",ch);
    else if (ch->pcdata->powers[FAERIE_LEVEL] == 3)
      send_to_char("You can curse your opponents.\n\r",ch);
    else if (ch->pcdata->powers[FAERIE_LEVEL] == 4)
      send_to_char("You can blink in Faerie form.\n\r",ch);
    ch->pcdata->powers[FAERIE_LEVEL]++;
    ch->practice -= ch->pcdata->powers[FAERIE_LEVEL]*80;
    return;
  }
  else if (!str_cmp(arg,"bull"))
  {
    if (ch->pcdata->powers[BULL_LEVEL] > 4)
    {
      send_to_char("Your Bull form is complete.\n\r",ch);
      return;
    }
    if (ch->practice < (80*ch->pcdata->powers[BULL_LEVEL] + 80))
    {
      send_to_char("You don't have enough primal to improve this form.\n\r",ch);
      return;
    }
    if (ch->pcdata->powers[BULL_LEVEL] == 0)
      send_to_char("You now have pointy horns in Bull form.\n\r",ch);
    else if (ch->pcdata->powers[BULL_LEVEL] == 1)
      send_to_char("You now have strong hooves in Bull form.\n\r",ch);
    else if (ch->pcdata->powers[BULL_LEVEL] == 2)
      send_to_char("Your muscles are nothing but iron in Bull form.\n\r",ch);
    else if (ch->pcdata->powers[BULL_LEVEL] == 3)
      send_to_char("You can now charge in Bull form.\n\r",ch);
    else if (ch->pcdata->powers[BULL_LEVEL] == 4)
      send_to_char("You can now STOMP in Bull form.\n\r",ch);
    ch->pcdata->powers[BULL_LEVEL]++;
    ch->practice -= ch->pcdata->powers[BULL_LEVEL]*80;
    return;
  }
  else if (!str_cmp(arg,"shiftpowers"))
  {
    if (ch->pcdata->powers[SHAPE_POWERS] > 4)
    {
      send_to_char("Your have completed your research into shapeshifting.\n\r",ch);
      return;
    }
    if (ch->practice < (80*ch->pcdata->powers[SHAPE_POWERS] + 80))
    {
      send_to_char("You don't have enough primal to learn anything new.\n\r",ch);
      return;
    }
    if (ch->pcdata->powers[SHAPE_POWERS] == 0)
      send_to_char("You can see through almost all kinds of illusions.\n\r",ch);
    else if (ch->pcdata->powers[SHAPE_POWERS] == 1)
      send_to_char("You can change yourself into thin mist and go anywhere.\n\r",ch);
    else if (ch->pcdata->powers[SHAPE_POWERS] == 2)
      send_to_char("You can change into a tall hat.\n\r",ch);
    else if (ch->pcdata->powers[SHAPE_POWERS] == 3)
      send_to_char("You can disguise yourself as anyone.\n\r",ch);
    else if (ch->pcdata->powers[SHAPE_POWERS] == 4)
      send_to_char("You can change your form into anything.\n\r",ch);
    ch->pcdata->powers[SHAPE_POWERS]++;
    ch->practice -= ch->pcdata->powers[SHAPE_POWERS]*80;
    return;
  }
  else
  {
    send_to_char("That's not a valid form.\n\r",ch);
    return;
  }
}

void do_faerieblink(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;  
   
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[FAERIE_LEVEL] < 5)
  {
    send_to_char("Your faerie form is not strong enough.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SHAPE_FORM] != FAERIE_FORM)
  {
    send_to_char("You must be in faerie form to blink.\n\r",ch);
    return;
  }
  if (ch->mana < 50000)
  {
    send_to_char("This costs 50000 mana.\n\r",ch);
    return;
  }
  if (ch->bomb_timer > 0)
  {
    stc("You have not recovered from your previous faerie blink.\n\r",ch);
    WAIT_STATE(ch, 28);
    return;
  }

  if ( ( victim = ch->fighting ) == NULL )
  { 
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if (!IS_NPC(victim)) 
  {
  ch->fight_timer += 10;
  WAIT_STATE(ch, 28);
  ch->bomb_timer = 4;
  }
  else if (IS_NPC(victim))
  {
  WAIT_STATE(ch, 18);
  ch->bomb_timer = 2;
  ch->mana -= 50000;
  }
  act("#yYou enter limbo, steps back into reality behind $N and attack.#n",ch,NULL,victim,TO_CHAR, FALSE);
  act("#y$n vanishes and suddenly someone strikes you from behind.#n",ch,NULL,victim,TO_VICT, FALSE);
  act("#7$n vanishes and appear behind $N striking $S in the back.#n",ch,NULL,victim,TO_NOTVICT, FALSE);
  multi_hit(ch,victim, TYPE_UNDEFINED);
  multi_hit(ch,victim, gsn_fangs);
  multi_hit(ch,victim, gsn_buffet);
  multi_hit(ch,victim, gsn_fangs);
    act("#yYou shift back into reality in front of $N.#n",ch,NULL,victim,TO_CHAR, FALSE);
  act("#y$n shifts back into reality before your eyes.#n",ch,NULL,victim,TO_VICT, FALSE);
  act("#y$n shifts back into reality before $N's eyes.#n",ch,NULL,victim,TO_NOTVICT, FALSE);
  return;
}

void do_camouflage(CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char arg3[MAX_STRING_LENGTH];
  OBJ_DATA *obj;

  argument=one_argument(argument,arg1);
  argument=one_argument(argument,arg2);
  strcpy(arg3,argument);
     
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_SHAPESHIFTER))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if (ch->pcdata->powers[SHAPE_POWERS] < 1)
  {
    send_to_char("You need to have level 1 shiftpowers.\n\r",ch);
    return;
  }
  if ((obj=get_obj_carry(ch,arg1)) == NULL)
  {
    send_to_char("You dont have that item.\n\r",ch);
    return;
  }
  if (!(!str_cmp(arg2,"short") || !str_cmp(arg2,"name")))
  {
    send_to_char("syntax : camouflage <item> <short/name> <newname>.\n\r",ch);
    return;
  }
  if (strlen(arg3) > 100 || strlen(arg3) < 3)
  {
    send_to_char("From 3 to 100 characters please.\n\r",ch);
    return;
  }
  if (has_bad_chars(ch, arg3))
  {     
    send_to_char("Illegal chars, please retry.\n\r",ch);
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

void do_shapeshift(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
     
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_SHAPESHIFTER))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if (ch->pcdata->powers[SHAPE_POWERS] < 4)
  {
    send_to_char("You need to have level 5 in shiftpowers.\n\r",ch);
    return;
  }
  if (IS_SET(ch->affected_by, AFF_POLYMORPH))
  {
    send_to_char("First you must shift to human form.\n\r",ch);
    return;
  }
  if (strlen(argument) > 30 || strlen(argument) < 4)
  {
    send_to_char("From 4 to 30 characters please.\n\r",ch);
    return;
  }
  if (!str_cmp(argument,"Jank")    || !str_cmp(argument,"Charysse") ||
      !str_cmp(argument,"Tei")     || !str_cmp(argument,"Snard") ||
      !str_cmp(argument,"Omega")   || !str_cmp(argument,"Oracle") ||
      !str_cmp(argument,"Bardiel") || !str_cmp(argument,"Keale") ||   
      !str_cmp(argument,"Huw")     || !str_cmp(argument,"Rand") ||   
      !str_cmp(argument,"Charoes") || !str_cmp(argument,"Noel"))

  {
    send_to_char("You idiot! You can't shift to look like immortals!\n\r",ch);
    return;
  }
  sprintf(buf, "%s's form blurs and shifts to resemble %s.",  ch->name, argument);
  act(buf, ch, NULL, NULL, TO_ROOM, FALSE);
  send_to_char("Your form blurs and shifts to resemble ",ch);
  send_to_char(argument,ch);
  send_to_char(".\n\r",ch);
  SET_BIT(ch->affected_by, AFF_POLYMORPH);
  free_string(ch->morph);
  ch->morph = str_dup(argument);
  return;
}

void do_shaperoar (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    if (IS_CLASS(ch, CLASS_WEREWOLF)) do_roar(ch,argument);
    else send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[TIGER_LEVEL] < 3)
  {
    send_to_char("Your tiger form is not strong enough.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SHAPE_FORM] != TIGER_FORM)
  {
    send_to_char("You must be in tiger form to #RROAR!!!#n\n\r",ch);
    return;
  }
  if ( ch->fighting == NULL )
  {
    send_to_char("You must be fighting to #RROAR!!!!#n.\n\r",ch);
    return;
  }
  if ( (victim = ch->fighting) == NULL ) return;
  WAIT_STATE( ch, 16 );
  if ( number_range( 1, 2 ) != 1 )
  {
    act("You let out an insane BeeF-powered #RROAR!!!!!!!!!#n to scare the shit out of $N, but it doesn't work.",ch,NULL,victim,TO_CHAR, FALSE);
    act("$n lets out an insane BeeF-powered #RROAR!!!!!!!!!#n, but it doesn't scare you.",ch,NULL,victim,TO_VICT, FALSE);
    act("$n lets out an insane BeeF-powered #RROAR!!!!!!!!!#n",ch,NULL,victim,TO_NOTVICT, FALSE);
    return;
  }
  act("You let out an insane BeeF-powered #RROAR!!!!!!!!!#n and scares the shit out of $N.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n lets out an insane BeeF-powered #RROAR!!!!!!!!!#n, and you are filled with terror.",ch,NULL,victim,TO_VICT, FALSE);
  act("$n lets out an insane BeeF-powered #RROAR!!!!!!!!!#n",ch,NULL,victim,TO_NOTVICT, FALSE);
  do_flee( victim, "" );
  return;
}


void do_stampede(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
   
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[BULL_LEVEL] < 3)
  {
    send_to_char("Your bull form is not strong enough.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[SHAPE_FORM] != BULL_FORM)
  {
    send_to_char("You must be in bull form to charge.\n\r",ch);
    return;
  }
  if (ch->move < 5000)
  {
    send_to_char("This costs 5000 move.\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  if (!IS_NPC(victim)) ch->fight_timer += 2;
  WAIT_STATE(ch, 20);
  ch->move -= 5000;
  multi_hit(ch,victim,gsn_headbutt);  
  multi_hit(ch,victim,gsn_headbutt);
  if (number_range(1,2) == 1)
  {
  if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
  act("You lift $N up by the horns and send $M flying into the wall.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n lifts you up by the horns and send you flying into the wall.",ch,NULL,victim, TO_VICT, FALSE);
  act("$n lifts $N up by the horns and send $M flying into the wall.",ch,NULL,victim, TO_ROOM, FALSE);
  victim->position = POS_STUNNED;
  }
  return;
}
