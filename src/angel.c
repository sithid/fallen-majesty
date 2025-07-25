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


/*********************************************
 * Angel, another class by Jobo for Dystopia *
 *********************************************/

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

void do_divinewrath( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_JUSTICE] < 5)
  {
    send_to_char("You haven't learned the secret of the Divine Wrath\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (ch->mana < 50000)
  {
    stc("You haven't got the energy for this!\n\r",ch);
    return;
  }
  if (ch->bomb_timer > 0)
  {
    stc("You have not recovered from your previous rage.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  if (victim == ch)
  {
    send_to_char("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  if (!IS_NPC(victim))
  {
  ch->fight_timer += 10;
  WAIT_STATE(ch, 30);
  }
  else if (IS_NPC(victim))
  WAIT_STATE(ch, 14);
  ch->mana -= 50000;
  act("#yYou rise into the sky and lay the divine wrath upon, $N#n",ch,NULL,victim,TO_CHAR, FALSE);
  act("#y$n rises in the air and rains down the Creator's Divine Wrath.#n",ch,NULL,victim,TO_VICT, FALSE);
  act("#7$n rises in the air in front of $N and rains down the Creator's Divine Wrath upon $N.#n",ch,NULL,victim,TO_NOTVICT, FALSE);
  multi_hit(ch,victim, TYPE_UNDEFINED);
  multi_hit(ch,victim, gsn_heavenlyaura);
  multi_hit(ch,victim, gsn_buffet);
  if ( IS_EVIL(victim) )
	{
        multi_hit(ch,victim, gsn_divinewrath);
        if (number_range(1,2) == 1)
	multi_hit(ch,victim, gsn_divinewrath);
	if (number_range(1,2) == 1)
	multi_hit(ch,victim, gsn_divinewrath);
	}
  ch->bomb_timer += 6;
  return;
}

void do_spiritform( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_PEACE] < 2)
  {
    send_to_char("Your not peaceful enough.\n\r",ch);
    return;
  }
    if (( ch->in_room->vnum >= 420000) && (ch->in_room->vnum <= 420099))
    {
        send_to_char( "You cannot do this in this area.\n\r",ch);
        return;
    }

  if (has_timer(ch)) return;
  if (IS_AFFECTED(ch, AFF_ETHEREAL))
  {
    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
    act("You regain your form and become solid.",  ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n regains $s form and becomes solid again.",  ch, NULL, NULL, TO_ROOM, FALSE);
    return;
  }
        if (ch->mana < 1000) 
        {
        send_to_char("You don't have enough mana to spiritform.\n\r",ch );
        return;
        }

  SET_BIT(ch->affected_by, AFF_ETHEREAL);
  ch->mana -= 1000;
  act("You transform into pure spiritual matter.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n glows and fades into a form of pure energy.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

void do_gpeace( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
    if (( ch->in_room->vnum >= 420000) && (ch->in_room->vnum <= 420099))
    {
        send_to_char( "You cannot do this in this area.\n\r",ch);
        return;
    }

  if (ch->pcdata->powers[ANGEL_PEACE] < 1)
  {
    send_to_char("Your not peaceful enough.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  if (IS_AFFECTED(ch, AFF_PEACE))
  {
    REMOVE_BIT(ch->affected_by, AFF_PEACE);
    act("Time to punish the unbelivers.",  ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n looks wicked.",  ch, NULL, NULL, TO_ROOM, FALSE);
    return;
  }
  SET_BIT(ch->affected_by, AFF_PEACE);
  act("The almighty hand of God protects you from further harm.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n smiles, seems God is on $s side.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

void do_innerpeace( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_PEACE] < 3)
  {
    send_to_char("Your not peaceful enough.\n\r",ch);
    return;
  }
  if (ch->mana < 1500)
  {
    send_to_char("You don't have enough mana.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  ch->mana -= 1500;
  ch->hit += ch->pcdata->powers[ANGEL_PEACE]*500;  
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  act("You feel the cleansing love of God run through your veins, filling you with an inner peace.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n is struck by a ray of pure light, and a blissful smile crosses $s lips.", ch, NULL, NULL, TO_ROOM, FALSE);
  WAIT_STATE(ch,18);
  return;
}

void do_houseofgod( CHAR_DATA *ch, char *argument )
{
  int level;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_PEACE] < 5)
  {
    send_to_char("Your not peaceful enough.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  if (ch->pcdata->powers[ANGEL_PEACE_COUNTER] > 0)
  {
    send_to_char("It is not the time for peace yet, God want you to fight.\n\r",ch);
    return;
  }
  ch->pcdata->powers[ANGEL_PEACE_COUNTER] = 50;
  level = ch->level;
  ch->level = 12;
  do_peace(ch,"");
  ch->level = level;
  ch->trust = 0;
  act("You call for God to transfer the pain of these mortals to yourself.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n says '#yLet not these followers of God suffer, let their pain be mine instead#n'.", ch, NULL, NULL, TO_ROOM, FALSE);
  WAIT_STATE(ch, 24);
  return;
}

void do_angelicaura( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_HARMONY] < 2)
  {
    send_to_char("Your not in contact with your inner harmonies.\n\r",ch);
    return;
  }
  if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_AURA))
  {
    REMOVE_BIT(ch->pcdata->powers[ANGEL_POWERS], ANGEL_AURA);
    act("Your angelic aura fades.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n looks less powerful.", ch, NULL, NULL, TO_ROOM, FALSE);
    return;
  }
  SET_BIT(ch->pcdata->powers[ANGEL_POWERS], ANGEL_AURA);
  act("Your angelic auras flickers into existance.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n smiles, the power of God is running through $s body.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

void do_gbanish( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *mount;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  int dam;
     
  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_HARMONY] < 3)
  {
    send_to_char("Your not in contact with your inner harmonies.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  if (victim == ch)
  {
    send_to_char("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  if (victim->alignment > 500)
  {
    send_to_char("They are not evil, you cannot punish them.\n\r",ch);
    return;
  }
  if (victim->alignment > 0) dam =4000;
  else if (victim->alignment > - 500) dam = 5000;
  else dam = 3500;
  sprintf(buf,"Your call for God to banish $N and $S is hurt [%d]",dam);
  act(buf,ch,NULL,victim,TO_CHAR, FALSE);
  sprintf(buf,"$n calls for God to banish you, and it hurts BAD! [%d]",dam);
  act(buf,ch,NULL,victim,TO_VICT, FALSE);
  sprintf(buf,"$n calls for God to Banish $N.");
  act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
  hurt_person(ch,victim,dam);
  if (number_range(1,10) > 7)
  {
    act( "$n is flung into a vortex and disappears.", victim, NULL, NULL, TO_ROOM, FALSE );
    char_from_room( victim );
    char_to_room( victim, get_room_index(ROOM_VNUM_HELL));
    act( "$n is flung out of a vortex.", victim, NULL, NULL, TO_ROOM, FALSE );
    do_look(victim, "auto");
    if ((mount = victim->mount) == NULL) return;
    char_from_room( mount );
    char_to_room(mount, victim->in_room);
    do_look(mount, "auto");
    return;
  }
  if (victim->position > POS_STUNNED)
  {
    if (victim->fighting == NULL) set_fighting(victim, ch);
    if (ch->fighting == NULL) set_fighting(ch, victim);
  }  
  WAIT_STATE(ch,18);
  return;
}

void do_harmony( CHAR_DATA *ch, char *argument )
{
  int sn, level;


  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_HARMONY] < 5)
  {
    send_to_char("Your not in contact with your inner harmonies.\n\r",ch);
    return;
  }

  level = (ch->spl[1] + ch->spl[2] + ch->spl[3] + ch->spl[4] + ch->spl[0])/5;
  sn = skill_lookup("spirit kiss");
  if (sn  > 0) (*skill_table[sn].spell_fun) (sn,level,ch,ch);
  sn = skill_lookup("godbless");
  if (sn  > 0) (*skill_table[sn].spell_fun) (sn,level,ch,ch);
  sn = skill_lookup("bless");
  if (sn  > 0) (*skill_table[sn].spell_fun) (sn,level,ch,ch);
  WAIT_STATE(ch,12);
  return;
}

void do_gsenses( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_LOVE] < 1)
  {
    send_to_char("God does not love you enough to grant you sight.\n\r",ch);
    return;
  }
  if (xIS_SET(ch->act, PLR_HOLYLIGHT))
  {
    xREMOVE_BIT(ch->act, PLR_HOLYLIGHT);
    send_to_char("Your holy vision fades.\n\r", ch);
  }
  else
  {
    xSET_BIT(ch->act, PLR_HOLYLIGHT);
    send_to_char( "You see through God's own eyes.\n\r", ch );
  }
  return;
}

void do_gfavor( CHAR_DATA *ch, char *argument )
{
  char buf [MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {  
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_LOVE] < 2)
  {
    send_to_char("God does not love you enough to grant his divine favor.\n\r",ch);
    return;
  }
  if (ch->alignment != 1000) ch->alignment = 1000;
  if (IS_SET(ch->newbits, NEW_CUBEFORM))
  {
    REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    act( "$n turns into a human.",ch,NULL,NULL,TO_ROOM, FALSE);
    send_to_char("You turn into your normal form.\n\r",ch);
    ch->damroll -=1500;
    ch->hitroll -=1500;
	ch->armor +=1500;
    free_string(ch->morph);
    ch->morph=str_dup("");
    WAIT_STATE(ch, 12);
    return;
  }
  if (ch->move < 2000 || ch->mana < 2000)
  {
    send_to_char("You need 2000 mana and 2000 move to make yourself a true angel.\n\r",ch);
    return;
  }
  else
  {
    act( "$n transforms into an angel.",ch,NULL,NULL,TO_ROOM, FALSE);
    send_to_char("God is with you, your now one of his divine.\n\r",ch);
    SET_BIT(ch->newbits, NEW_CUBEFORM);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    sprintf(buf,"%s the angel",ch->name);
    free_string(ch->morph);
    ch->morph = str_dup(buf);
    ch->move -= 2000;
    ch->mana -= 2000;
    ch->damroll +=1500;
    ch->hitroll +=1500;
	ch->armor -=1500;
    WAIT_STATE(ch,12);
    return;
  }
  return;
}

void do_forgivness( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH]; 
  
  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_LOVE] < 3)
  {
    send_to_char("Your are not loving enough.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
    if ((victim->fight_timer > 0) && (ch != victim))
    {
    stc("You cannot aid those in battle.\n\r",ch);
    return;
    }

  if (victim == ch)
  {
    send_to_char("You haven't sinned, why seek forgivness ??\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_TANARRI))
  {
    send_to_char("You cannot forgive a creature of utter evil.\n\r",ch);
    return;
  }
  if (victim->alignment < 0)
  {
    send_to_char("They are not ready to seek forgivness, first they must repent.\n\r",ch);
    return;
  }
  victim->hit += number_range(1000,1500);
  if (victim->hit > victim->max_hit) victim->hit = victim->max_hit;
  act("You feel the cleansing love of God run through your veins, filling you with an inner peace.", victim, NULL, NULL, TO_CHAR, FALSE);
  act("$n is struck by a ray of pure light, and a blissful smile crosses $s lips.", victim, NULL, NULL, TO_ROOM, FALSE);
  send_to_char("You forgive them, and thus God forgive them for their sins.\n\r",ch);
  WAIT_STATE(ch, 16);
  return;
}

void do_martyr( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *ich;
  int dummy;
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_LOVE] < 5)
  {
    send_to_char("Your love for mortals are not strong enough.\n\r",ch);
    return;
  }
  if (ch->max_hit < 100000)
  {
	stc("God does not think your sacrifice is worthy.\n\r",ch);
	return;
  }
  if (ch->hit < ch->max_hit)
  {
    send_to_char("Your body cannot take the strain.\n\r",ch);
    return;
  }
  if (has_timer(ch)) return;
  dummy = ch->level;
  ch->level = 12;
  act("You call for God to transfer the pain of these mortals to yourself.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n says '#yLet not these followers of God suffer, let their pain be mine instead#n'.", ch, NULL, NULL, TO_ROOM, FALSE);
  for (ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room)
    if (!IS_NPC(ich) && (ich->fight_timer < 1) && (ich->alignment > 0)) do_restore(ch,ich->pcdata->switchname);
  ch->level = dummy;
  ch->hit = 1000;
  ch->move = 1000;
  ch->mana = 1000;
  WAIT_STATE(ch,20);
  return;
}

void do_swoop( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;
  CHAR_DATA *victim;
          
  one_argument (argument, arg);
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if (ch->pcdata->powers[ANGEL_JUSTICE] < 1)
  {
    send_to_char("You haven't learned to fly yet.\n\r",ch);  
    return;
  }
  if (!IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_WINGS))                       
  {
    send_to_char("Where are your wings?\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Swoop down on whom?\n\r", ch );
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
        stc("Rand should kill you, you wussy-assed coward.\n\r",ch);
        return;
    }
    if (IS_SET(victim->in_room->room_flags, ROOM_ARENA))
    {
        stc("Rand should rip your head off, cheater.\n\r",ch);
        return;
    }

  location = victim->in_room;
  if (ch->move < 500)
  {
    send_to_char("You don't have the move to fly that far.\n\r", ch );
    return;
  }
  act("You fly up into the sky.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n flies into the sky.", ch, NULL, NULL, TO_ROOM, FALSE);
  ch->move -= 500;
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  act("You swoop down upon $N.", ch, NULL, victim, TO_CHAR, FALSE);
  act("$n swoops down from the sky.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

void do_touchofgod(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  int dam;

  argument = one_argument( argument, arg );
  dam = number_range(1000,2000);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_JUSTICE] < 4)
  {
    send_to_char("You cannot channel the will of God yet.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  if (victim == ch)
  {
    send_to_char("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  WAIT_STATE(ch, 16);
  hurt_person(ch, victim, dam);
  act("You grab $N by the forehead and channel God's justice into $S body.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n grabs you by your forehead and sends a prayer to God, DAMN THAT HURT.",ch,NULL,victim,TO_VICT, FALSE);
  act("$n grabs $N by the forehead and sends a prayer to God.",ch,NULL,victim,TO_NOTVICT, FALSE);
  if (number_range(1,2) == 1)
  {
    if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
    act("$N screams in pain and drops to the ground as God's justice wrecks $S body.",ch,NULL,victim,TO_CHAR, FALSE);
    act("AARGH!!! The pain is to much, you drop to the ground.",ch,NULL,victim,TO_VICT, FALSE);
    act("$N screams in agony and falls to the ground.",ch,NULL,victim,TO_NOTVICT, FALSE);
    victim->position = POS_STUNNED;
  }
  return;
}

void do_awings( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_JUSTICE] < 1)
  {
    send_to_char("Your don't have wings yet.\n\r",ch);
    return;
  }
  if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_WINGS))
  {
    REMOVE_BIT(ch->pcdata->powers[ANGEL_POWERS], ANGEL_WINGS);
    act("You fold your wings back into your body.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n fold $s wings back into $s body.", ch, NULL, NULL, TO_ROOM , FALSE);
    return;
  }
  SET_BIT(ch->pcdata->powers[ANGEL_POWERS], ANGEL_WINGS);
  act("You spread your wings.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n spread $s wings.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

void do_halo( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_JUSTICE] < 2)
  {
    send_to_char("You haven't gotten your halo yet.\n\r",ch);
    return;
  }
  if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_HALO))
  {
    REMOVE_BIT(ch->pcdata->powers[ANGEL_POWERS], ANGEL_HALO);
    act("Your halo flickers and disappears.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n's halo flickers and disappears.", ch, NULL, NULL, TO_ROOM, FALSE );
    return;
  }
  SET_BIT(ch->pcdata->powers[ANGEL_POWERS], ANGEL_HALO);
  act("A halo appears over your head.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("A halo appears over $n's head.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

void do_sinsofthepast( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
 
  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_JUSTICE] < 3)
  {
    send_to_char("Your not ready to punish the sinners.\n\r",ch);
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobiles.\n\r",ch);
    return;
  }
  if (is_safe(ch,victim)) return;
  if (victim == ch)
  {
    send_to_char("That doesn't seem like a good idea.\n\r",ch);
    return;
  }
  if (IS_CLASS(victim, CLASS_ANGEL))
  {
    send_to_char("Angels are born without sin, they are perfect creatures.\n\r",ch);
    return;
  }
  send_to_char("#RYou burn them with the fires of hell!#n\n\r",ch);
  if (!IS_AFFECTED(victim, AFF_FLAMING)) SET_BIT(victim->affected_by, AFF_FLAMING);
  send_to_char("#LTheir body is wrecked by poison!#n\n\r",ch);
  if (!IS_AFFECTED(victim, AFF_POISON)) SET_BIT(victim->affected_by, AFF_POISON);
  send_to_char("#CGod's fury upon them for they have truly sinned!!!#n\n\r",ch);
  one_hit(ch,victim, gsn_wrathofgod,1);
  WAIT_STATE(ch,12);
  return;
}

void do_eyeforaneye( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ANGEL_JUSTICE] < 5)
  {
    send_to_char("Your not ready to punish the sinners.\n\r",ch);
    return;
  }   
  if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_EYE))
  {
    REMOVE_BIT(ch->pcdata->powers[ANGEL_POWERS], ANGEL_EYE);
    act("The sinners have paid for their sin, you send a prayer of thanks to God.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n's prays to $s god, thanking for the opportunity to punish the sinners.", ch, NULL, NULL, TO_ROOM, FALSE);
    return;
  }
  SET_BIT(ch->pcdata->powers[ANGEL_POWERS], ANGEL_EYE);
  act("#CTime to punish the wicked and the sinful.#n", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n grins wickedly, and a smile of pleasure crosses $s lips.", ch, NULL, NULL, TO_ROOM, FALSE);
  return;
}

void do_angelicarmor( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if( !IS_CLASS(ch, CLASS_ANGEL))
  {
    send_to_char("What?\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of angel armor you wish to make: Sword Bracer\n\rNecklace Ring Plate Helmet Leggings Boots Gauntlets Sleeves Cloak Belt Visor.\n\r",ch);
    return;
  }
  if ( ch->practice < 150 )
  {
    send_to_char("It costs 150 points of primal to create angel equipment.\n\r",ch);
    return;
  }
  if (!str_cmp(arg,"sword")) vnum = 33192;
  else if (!str_cmp(arg,"bracer")) vnum = 33181;
  else if (!str_cmp(arg,"necklace")) vnum = 33182;
  else if (!str_cmp(arg,"ring")) vnum = 33180;
  else if (!str_cmp(arg,"plate")) vnum = 33187;
  else if (!str_cmp(arg,"helmet")) vnum = 33184;
  else if (!str_cmp(arg,"leggings")) vnum = 33188;
  else if (!str_cmp(arg,"boots")) vnum = 33189;
  else if (!str_cmp(arg,"gauntlets")) vnum = 33190;
  else if (!str_cmp(arg,"sleeves")) vnum = 33191;
  else if (!str_cmp(arg,"cloak")) vnum = 33185;
  else if (!str_cmp(arg,"belt")) vnum = 33183;
  else if (!str_cmp(arg,"visor")) vnum = 33186;
  else
  {
    do_angelicarmor(ch,"");
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

void do_repent( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    char buf[MAX_STRING_LENGTH];

    int dam;
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_ANGEL))
    {
    send_to_char("Huh?\n\r",ch);
    return;
    }
    if (ch->pcdata->powers[ANGEL_HARMONY] < 3)
    {
    send_to_char("You are not peaceful enough.\n\r",ch);
    return;
    }
    if ( ( victim = ch->fighting ) == NULL )
    {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
    }
    if (IS_NPC(victim))
    {
    send_to_char("Mobiles cannot repent!\n\r",ch);
    return;
    }
    if (ch->mana < 5000)
    {
    send_to_char("You don't have enough mana.\n\r",ch);
    return;
    }
    if (ch->move < 5000)
    {
    send_to_char("You don't have enough movement.\n\r",ch);
    return;
    }
    WAIT_STATE( ch, 18 );
    if (!IS_NPC(victim))
    {
    if (victim->alignment >= 750)
    	{
	dam = (ch->alignment - victim->alignment)*4;
        }
    else if (victim->alignment <= 500)
	{
	dam = (ch->alignment - victim->alignment)*4;
	}
    else if (victim->alignment <= 1)
	{
	dam = (ch->alignment - victim->alignment)*4;
	}
    else if (victim->alignment <= -500)
	{
	dam = (ch->alignment - victim->alignment)*4;
	}
    else if (victim->alignment <= -750)
	{
	dam = (ch->alignment - victim->alignment)*4;
	}
    }
    dam += number_range(1,30);
    if ( dam <= 0 )
       dam = 1;
    sprintf(buf,"Your holy touch causes $N to repent! [%d]",dam);
    act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    sprintf(buf,"$n's forced repentance sears your flesh! [%d]",dam);
    act(buf,ch,NULL,victim,TO_VICT, FALSE);
    sprintf(buf,"$n's glowing hands burn $N! [%d]",dam);
    act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);


    send_to_char("\n\r",ch);
    hurt_person(ch,victim,dam);
    if (!IS_NPC(victim))
    {
    ch->mana -= 5000;
    ch->move -= 5000;
    }
    set_fighting(ch,victim);
    ch->fight_timer += 3;
    return;
}

