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
#include "player.h"

void do_stalk( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
    
  if ( IS_NPC(ch) )
    return;

  argument = one_argument (argument, arg);

  if (!IS_CLASS(ch, CLASS_NINJA) )
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }

  if ( arg[0] == '\0' )
  {
    send_to_char( "Stalk whom?\n\r", ch );
    return;
  }

  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "You cannot find them.\n\r", ch );
    return;
  }
    
  if( ch->move < 1000) 
  {
    stc( "You don't have enough movement points to stalk them.\n\r",ch);
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


  if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  { 
    stc( "You can't find it's room.\n\r",ch);
    return; 
  } 
  
  if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your body can't go there.\n\r",ch);
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
    send_to_char( "They are hiding from you.\n\r", ch );
    return;
  }
  if (victim->in_room == ch->in_room)
  {
    send_to_char("But you're already there!\n\r",ch);
    return;
  }

  act("You pick up the trail of $N, and quickly find them.", ch, NULL, victim, TO_CHAR, FALSE);
  act("$n walks into the shadows.", ch, NULL, victim, TO_ROOM, FALSE);
  char_from_room(ch);
  char_to_room(ch,victim->in_room);
  ch->move -= 1000;
  act("$n walks out of the shadows behind $N.", ch, NULL, victim, TO_NOTVICT, FALSE);
  act("$n walks out of the shadows behind you.", ch, NULL, victim, TO_VICT, FALSE);
  do_look(ch,"auto");
  return;
}

void do_principles( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_NINJA))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
	sprintf(buf,"Principles: Sora (%d), Chikyu (%d), Ningenno (%d).\n\r",
	ch->pcdata->powers[NPOWER_SORA], ch->pcdata->powers[NPOWER_CHIKYU],
	ch->pcdata->powers[NPOWER_NINGENNO]);
	send_to_char(buf,ch);
	return;
    }
    if (arg2[0] == '\0')
    {
	if (!str_cmp(arg1,"sora"))
	{
	    send_to_char("Sora - The finding, observing, and locating principle.\n\r\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_SORA] < 1) 
		send_to_char("You have none of the Sora principles.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_SORA] > 0) 
		send_to_char("Mitsukeru -Locate- The scrying power to find enemies.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_SORA] > 1) 
		send_to_char("Koryou -Consider- The read aura power, learn about your enemies.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_SORA] > 2) 
		send_to_char("Kakusu -Hidden- Enhanced Hide.  Only other ninjas can see you.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_SORA] > 3) 
		send_to_char("Uro-Uro -Silent Walk- You leave no footprints behind.\n\r",ch);
		if (ch->pcdata->powers[NPOWER_SORA] > 4)
		send_to_char("Kanzuite -Aware- The truesight power.\n\r", ch);
		if (ch->pcdata->powers[NPOWER_SORA] > 5)
		send_to_char("Bomuzite -Sleep Gas- By mixing an potion, you can put everyone in a room to sleep.\n\r", ch);
	    return;
	}
	else if (!str_cmp(arg1,"chikyu"))
	{
	    send_to_char("Chikyu - Preperation for battle.\n\r\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_CHIKYU] < 1) 
		send_to_char("You have none of the Chikyu principles.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_CHIKYU] > 0) 
		send_to_char("Tsuyoku -Strength- Toughness.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_CHIKYU] > 1) 
		send_to_char("Songai -Damage- Enhanced damage.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_CHIKYU] > 2) 
		send_to_char("Isogu -Haste- Adds two extra attacks.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_CHIKYU] > 3) 
		send_to_char("Tsuiseki -Hunt- Fast hunting.\n\r",ch);
		if (ch->pcdata->powers[NPOWER_CHIKYU] > 4)
		send_to_char("Sakeru -Sonic Speed- Dodge more attacks.\n\r", ch);
	    if (ch->pcdata->powers[NPOWER_CHIKYU] > 5)
		send_to_char("HaraKiri -Blood Power- Hurt yourself to gain  power.\n\r", ch );
	    return;
	}
	else if (!str_cmp(arg1,"ningenno"))
	{
	    send_to_char("Ningenno - The battle, attacking and getting away.\n\r\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_NINGENNO] < 1) 
		send_to_char("You have none of the Ningenno principles.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_NINGENNO] > 0) 
		send_to_char("Tsume -Claw- IronClaw always worn on wrist to aid in hand-to-hand.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_NINGENNO] > 1) 
		send_to_char("Assassination -Sometimes instant kill.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_NINGENNO] > 2) 
		send_to_char("Mienaku -Vanish- Never fail flee.\n\r",ch);
	    if (ch->pcdata->powers[NPOWER_NINGENNO] > 3) 
		send_to_char("Shiroken -Throwing Star- Added attack per round, like headbutt for demons.\n\r",ch);
		if (ch->pcdata->powers[NPOWER_NINGENNO] > 4)
		send_to_char("Dokuyaku -Poison- Adds poisons to the Shiroken\n\r", ch);
	if (ch->pcdata->powers[NPOWER_NINGENNO] > 5)
	send_to_char("Circle - circle behind your opponent and hit him during battle\n\r", ch );

	    return;
	}
	sprintf(buf,"Principles: Sora (%d), Chikyu (%d), Ningenno (%d).\n\r",
	ch->pcdata->powers[NPOWER_SORA], ch->pcdata->powers[NPOWER_CHIKYU],
	ch->pcdata->powers[NPOWER_NINGENNO]);
	send_to_char(buf,ch);
	return;
    }
    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;

	     if (!str_cmp(arg1,"sora"   )) {improve = NPOWER_SORA;   max=6;}
	else if (!str_cmp(arg1,"chikyu"   )) {improve = NPOWER_CHIKYU;   max=6;}
	else if (!str_cmp(arg1,"ningenno"   )) {improve = NPOWER_NINGENNO; max=6;}
	else
	{
	    send_to_char("Principles: Sora, Chikyu, Ningenno.\n\r",ch);
	    return;
	}							
	cost = (ch->pcdata->powers[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->pcdata->powers[improve] >= max )
	{
	    sprintf(buf,"You have already gained all the powers of the %s principle.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
	if ( cost > ch->practice )
	{
	    sprintf(buf,"It costs you %d primal to improve your %s principle.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->pcdata->powers[improve] += 1;
	ch->practice -= cost;
	sprintf(buf,"You improve your ability in the %s principle.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a principle, type: Principle <principle type> improve.\n\r",ch);
	return;
}

void do_michi(  CHAR_DATA *ch, char *argument  )
{
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_NINJA) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

  if ( IS_CLASS(ch, CLASS_NINJA) )
  {
	
	if ( ch->rage >= 100 )
	{
	    send_to_char("But you are already in the state of Michi.\n\r",ch);
	    return;
	}
	if ( ch->move < 1000 )
	{
		send_to_char("But you don't have enough move to perform the michi.\n\r", ch);
		return;
	} 
	
	send_to_char("You are gifted positive energy while performing the michi.\n\r",ch);
	act("$n is gifted positives energy while performing the michi.",ch,NULL,NULL,TO_ROOM, FALSE);
	ch->rage += 100;
	ch->move -= 1000; 
	WAIT_STATE(ch,12);
	return;
  }
	else
    send_to_char("But you are already in the state of Michi.\n\r",ch);
    return;
}

void do_artteach( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_NINJA) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

    
    if ( arg[0] == '\0' )
    {
	send_to_char( "Artteach whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "Not on Immortals's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot artteach yourself.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_NINJA))
    {
	send_to_char( "They are already ArtTeached.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only teach avatars.\n\r", ch );
	return;
    }

	if (victim->class != 0) {
	send_to_char("They already have a profession.\n\r", ch );
	return;}
    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot Artteach an unwilling person.\n\r", ch );
	return;
    }

    if (ch->exp < 100000)
    {
	send_to_char("You cannot afford the 100000 exp required to Artteach them.\n\r",ch);
	return;
    }

    if (victim->exp < 100000)
    {
	send_to_char("They cannot afford the 100000 exp required to be ArtTeached from you.\n\r",ch);
	return;
    }

    ch->exp -= 100000;
    victim->exp -= 100000;

    act("You teach $N the ways of the ninja.", ch, NULL, victim, TO_CHAR, FALSE);
    act("$n teaches $N the ways of the ninja.", ch, NULL, victim, TO_NOTVICT, FALSE);
    act("$n teaches you the way of the ninja.", ch, NULL, victim, TO_VICT, FALSE);
    victim->class    = CLASS_NINJA;
    send_to_char( "You are now a ninja.\n\r", victim );
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    victim->generation = ch->generation + 1;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_kakusu( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_NINJA) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_SORA] < 3 )
	{
	  send_to_char("You have not learned the Sora principle to 3.\n\r", ch);
	  return;
	}
	
	if (ch->move < 1000)
	{
		send_to_char("You don't have 500 move to activate your power.\n\r", ch);
		return;
	} 

        if (ch->fight_timer >0) {
          send_to_char("Not until your fight timer expires.\n\r", ch );
          return;
        }
    if (( ch->in_room->vnum >= 420000) && (ch->in_room->vnum <= 420099))
    {
        send_to_char( "You cannot do this in this area.\n\r",ch);
        return;
    }

    if ( xIS_SET(ch->act, PLR_HIDE) )
    {
	xREMOVE_BIT(ch->act, PLR_HIDE);
	act( "$n appears from the shadows.", ch, NULL, NULL, TO_ROOM, FALSE );
	send_to_char( "You appear from the shadows.\n\r", ch );
    }
    else
    {
	act( "$n disappears into the shadows.", ch, NULL, NULL, TO_ROOM, FALSE );
	send_to_char( "You disappear into the shadows.\n\r", ch );
	ch->move -= 1000;
        xSET_BIT(ch->act, PLR_HIDE);
    }
    return;
}

void do_kanzuite( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_NINJA) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
    if (ch->fighting != NULL)
    	{
	  stc("Not while fighting!\n\r",ch);
	  return;
	}	
    if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_SORA] < 5 )
	{
	  send_to_char("You have not learned the Sora principle to 5.\n\r", ch);
	  return;
	}
	
	else if (ch->move < 500)
	{
		send_to_char("You don't have 500 move to increase your awareness.\n\r", ch);
		return;
	} 

    if ( xIS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	xREMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "You're senses return to normal.\n\r", ch );
    }
    else
    {
	xSET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "You're senses increase into incredible proportions.\n\r", ch );
	ch->move -= 500; 
    }

    return;
}

void do_mienaku( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    int attempt;


	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] < 3 )
	{
	send_to_char("You have not learned the Ningenno principle to 3.\n\r", ch);
	return;
	}
	else if (!IS_CLASS(ch, CLASS_NINJA) )
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}
	
	if ( IS_NPC(ch) ) return;

    if ( ( victim = ch->fighting ) == NULL )
    {
	if ( ch->position == POS_FIGHTING )
	    ch->position = POS_STANDING;
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }
	if (ch->move < 200)
	{
	send_to_char("You don't have enough movement points to flee.\n\r", ch);
	return;
	} 
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }
    if (number_range(1, 20) <= 5)
    {
	send_to_char("You failed to escape!\n\r",ch);
	return;
    }
    was_in = ch->in_room;
    
{
	EXIT_DATA *pexit;
	int door;

	send_to_char("You move to vanish from combat!\n\r", ch );

for ( attempt =  0; attempt < 6; attempt++ ) 
	{
	door = number_door( );
	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->to_room == NULL
	||   IS_SET(pexit->exit_info, EX_CLOSED)
	|| ( IS_NPC(ch)
	&&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
	      continue;


	move_char( ch, door );
	if ( ( now_in = ch->in_room ) == was_in )
	      continue;

	ch->in_room = was_in;
	act( "$n has escaped!", ch, NULL, NULL, TO_ROOM, FALSE );
	ch->in_room = now_in;
	
	if ( !IS_NPC(ch) )
	    send_to_char( "You escape from combat!\n\r", ch ); 

        ch->move -= 200;
	stop_fighting( ch, TRUE );
	return;
   }
   
}
    return;
}

void do_bomuzite( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
        
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_NINJA))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	else if ( ch->pcdata->powers[NPOWER_SORA] < 6 )
	{
	    send_to_char("You have not learned the Sora principle to 6.\n\r", ch);
	    return;
	}
	
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Bomb who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Not on yourself!\n\r", ch );
	return;
    }
	
    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "Not while fighting!\n\r", ch );
	return;
    }
    if (is_safe(victim,ch)) return;
    
	if (ch->move < 2500)
	{
	send_to_char("You don't have enough movement points.\n\r", ch);
	return;
	}     
	if (ch->bomb_timer > 0)  
        {
	stc("You must wait another moment!\n\r", ch);
	return;
	}
 if ( victim->in_room == ch->in_room )
	{
	act("You toss your bomb onto the floor and put $N to sleep.",ch,NULL,victim,TO_CHAR, FALSE);
	act("$n tosses a bomb onto the floor.  You feel sleepy.",ch,NULL,victim,TO_VICT, FALSE);
	victim->position = POS_SLEEPING;
	ch->move -= 2500;
	ch->bomb_timer += 3;
	WAIT_STATE(ch, 3);
	return;
	}
	
	return;
}


void do_tsume(CHAR_DATA *ch, char *argument) {

		if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powers[NPOWER_NINGENNO] < 1) {
	send_to_char("Huh?\n\r", ch );
	return;}

	if (IS_VAMPAFF(ch, VAM_CLAWS)) {

	send_to_char("You remove the IronClaws from your wrists.\n\r", ch );
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
	return;}

	send_to_char("You attach IronClaws to your wrists.\n\r", ch );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);

	return;
}

void do_hara_kiri(CHAR_DATA *ch, char *argument) {

	

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powers[NPOWER_CHIKYU] < 6) {
	send_to_char("Huh?\n\r", ch );
	return;}

	if (ch->pcdata->powers[HARA_KIRI] > 0) {
	send_to_char("You are already experiencing the power of HaraKiri.\n\r", ch );
	return;}

	if (ch->hit < ch->max_hit/10) {
	send_to_char("You are hurt too badly already.\n\r", ch );
	return;}

	ch->pcdata->powers[HARA_KIRI] = ch->hit/500;
	if (ch->pcdata->powers[HARA_KIRI] < 5)
	ch->pcdata->powers[HARA_KIRI] = 5;	
	ch->hit = 1;
	ch->mana = 1;
	ch->move = 1;
	send_to_char("You cut your finger and bleed profusely.\n\r", ch );
	act("$n cuts his finger and obtains the power of HaraKiri",ch,NULL,NULL,TO_ROOM, FALSE);
	return;
}

void do_ninjaarmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

   	if(!IS_IMMORTAL(ch))
	{
    if (!IS_CLASS(ch, CLASS_NINJA))
    {
        send_to_char("What?\n\r",ch);
        return;
    }
	}

    if (arg[0] == '\0')
    {
    send_to_char("Please specify which piece of ninja eq you wish to make: Ring Collar Robe Cap\n\rLeggings Boots Gloves Sleeves Cloak Belt Bracer Mask Sword Dagger.\n\r",ch);
    return;
    }

    if ( ch->practice < 150 )
    {
        send_to_char("You don't have the 150 primal needed.\n\r",ch);
        return;
    }
    if (!str_cmp(arg,"mask"	)) vnum = 33093;
    else if (!str_cmp(arg,"dagger" )) vnum = 33080;
    else if (!str_cmp(arg,"sword"   )) vnum = 33081;
    else if (!str_cmp(arg,"ring"   )) vnum = 33082;
    else if (!str_cmp(arg,"collar"   )) vnum = 33083;
    else if (!str_cmp(arg,"robe"   )) vnum = 33085;
    else if (!str_cmp(arg,"cap"   )) vnum = 33086;
    else if (!str_cmp(arg,"leggings"   )) vnum = 33087;
    else if (!str_cmp(arg,"boots"   )) vnum = 33088;
    else if (!str_cmp(arg,"gloves"   )) vnum = 33091;
    else if (!str_cmp(arg,"sleeves"   )) vnum = 33089;
    else if (!str_cmp(arg,"cloak"   )) vnum = 33090;
    else if (!str_cmp(arg,"belt"   )) vnum = 33092;
    else if (!str_cmp(arg,"bracer"   )) vnum = 33084;
    else
    {
    do_ninjaarmor(ch,"");
    return;
    }
    
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
    send_to_char("Missing object, please inform a God .\n\r",ch);
    return;
    }

    if( !IS_IMMORTAL(ch) )
	{
		ch->practice -= 150;
	}
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("You make $p from the shadows.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$n forms $p from the shadows.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}

void do_screen(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *location;
	CHAR_DATA *victim;

	one_argument (argument, arg);

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA)) {
	send_to_char("Huh?\n\r", ch );
	return;}

	if ((victim = get_char_world(ch, arg)) == NULL) {
	send_to_char("Screen to whom?\n\r", ch );
	return;}

 if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  { 
    stc( "You can't find it's room.\n\r",ch);
    return; 
  } 
  if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  
  if(!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON))
  {
    stc( "They are hiding from you.\n\r",ch);
    return;
  }
  
	location = victim->in_room;

	if (ch->move < 250) {
	send_to_char("You are too tired to screen.\n\r", ch );
	return;}

/*	if (!IS_NPC(victim)) {
	send_to_char("You failed.\n\r", ch );
	return;}

	if (!IS_SET(victim->in_room->room_flags, ROOM_DARK)) {
	send_to_char("You failed.\n\r", ch );
	return;}*/

	act("You create a smoke screen and disappear.", ch, NULL, NULL, TO_CHAR, FALSE);
	act("$n creates a smoke screen and disappers.", ch, NULL, NULL, TO_ROOM, FALSE);
	ch->move -= 250;
	char_from_room(ch);
	char_to_room(ch, location);
	do_look(ch, "auto");	
	act("You appear from a cloud of smoke.", ch, NULL, NULL, TO_CHAR, FALSE);
	act("$n appears from a cloud of smoke.", ch, NULL, NULL, TO_CHAR, FALSE);

	return;
}

