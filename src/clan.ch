/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,
 *
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
 * Local functions.
 */

void	adv_spell_damage	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_affect	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_action	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	birth_write		args( ( CHAR_DATA *ch, char *argument ) );
bool	birth_ok		args( ( CHAR_DATA *ch, char *argument ) );

/* Need to get rid of those flames somehow - KaVir */

void do_level( CHAR_DATA *ch, char *argument )
{
    char arg       [MAX_INPUT_LENGTH];
    char skill  [20];  
   /* char stance [20];*/
    one_argument( argument, arg );

    if (IS_NPC(ch)) return;  
    
    send_to_char("---------------------------=[#6Weapon Levels#n]=--------------------------------\n\r", ch);
    sprintf(skill,"Slice  : %-4d", ch->wpn[1]);
    send_to_char( skill, ch );
    sprintf(skill," Stab   : %-4d", ch->wpn[2]);
    send_to_char( skill, ch );      
    sprintf(skill," Slash  : %-4d", ch->wpn[3]);
    send_to_char( skill, ch );  
    sprintf(skill," Whip   : %-4d", ch->wpn[4]);
    send_to_char( skill, ch ); 
    sprintf(skill," Claw   : %-4d\n\r", ch->wpn[5]);
    send_to_char( skill, ch );
    sprintf(skill,"Blast  : %-4d", ch->wpn[6]);
    send_to_char( skill, ch );
    sprintf(skill," Pound  : %-4d", ch->wpn[7]);
    send_to_char( skill, ch );
    sprintf(skill," Crush  : %-4d", ch->wpn[8]);
    send_to_char( skill, ch );
    sprintf(skill," Grep   : %-4d", ch->wpn[9]);
    send_to_char( skill, ch ); 
    sprintf(skill," Bite   : %-4d\n\r", ch->wpn[10]);
    send_to_char( skill, ch );  
    sprintf(skill,"Pierce : %-4d", ch->wpn[11]);
    send_to_char( skill, ch );
    sprintf(skill," Suck   : %-4d", ch->wpn[12]);
    send_to_char( skill, ch );        
    sprintf(skill," Unarmed: %-4d\n\r\n", ch->wpn[0]);
    send_to_char( skill, ch );    
    
    send_to_char("---------------------------=[#6Stance Levels#n]=--------------------------------\n\r",ch);
    sprintf(skill,"Viper    : %-4d", ch->stance[1]);
    send_to_char( skill, ch );
    sprintf(skill, "Crane   : %-4d", ch->stance[2]);
    send_to_char( skill, ch );
    sprintf(skill,  "Crab   : %-4d", ch->stance[3]);
    send_to_char( skill, ch );
    sprintf(skill,"Mongoose : %-4d", ch->stance[4]);
    send_to_char( skill, ch );
    sprintf(skill,"Bull     : %-4d\n\r", ch->stance[5]);
    send_to_char( skill, ch );
    sprintf(skill, "Mantis   : %-4d", ch->stance[6]);
    send_to_char( skill, ch );
    sprintf(skill,"Dragon  : %-4d", ch->stance[7]);
    send_to_char( skill, ch );
    sprintf(skill,"Tiger  : %-4d", ch->stance[8]);
    send_to_char( skill, ch );
    sprintf(skill,"Monkey   : %-4d", ch->stance[9]);
    send_to_char( skill, ch );
    sprintf(skill,"Swallow  : %-4d\n\r", ch->stance[10]);
    send_to_char( skill, ch );

    send_to_char("\n\r---------------------------=[#6Super Stances#n]=--------------------------------\n\r",ch);
    sprintf(skill,"SS1      : %-4d", ch->stance[13]);
    send_to_char( skill, ch );
    sprintf(skill,"SS2     : %-4d", ch->stance[14]);
    send_to_char( skill, ch );
    sprintf(skill,"SS3    : %-4d", ch->stance[15]);
    send_to_char( skill, ch );
    sprintf(skill,"SS4      : %-4d", ch->stance[16]);
    send_to_char( skill, ch );
    sprintf(skill,"SS5      : %-4d\n\r", ch->stance[17]);
    send_to_char( skill, ch );

    send_to_char("\n\r---------------------------=[#6Spell Levels#n]=---------------------------------\n\r", ch);
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) sprintf(skill,"#5Purple#n : %-4d", ch->spl[0] + 50);
    else sprintf(skill,"#5Purple#n : %-4d", ch->spl[0]);
    send_to_char( skill, ch );
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) sprintf(skill,"#4Blue#n : %-4d", ch->spl[2] + 50);
    else sprintf(skill,"#4Blue#n : %-4d", ch->spl[2]);
    send_to_char( skill, ch );
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) sprintf(skill,"#1Red#n : %-4d", ch->spl[1] + 50);
    else sprintf(skill,"#1Red#n : %-4d", ch->spl[1]);
    send_to_char( skill, ch );
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) sprintf(skill,"#2Green#n : %-4d", ch->spl[3] + 50);
    else sprintf(skill,"#2Green#n : %-4d", ch->spl[3]);
    send_to_char( skill, ch );
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) sprintf(skill,"#3Yellow#n : %-4d\n\r\n", ch->spl[4] + 50);
    else sprintf(skill, "#3Yellow#n : %-4d\n\r\n", ch->spl[4]);
    send_to_char( skill, ch );
    
    
    send_to_char("---------------------------=[#6Misc. Things#n]=---------------------------------\n\r", ch);
    sprintf(skill, "Recall Room : %d\n\r", ch->home);
    send_to_char( skill, ch );
    sprintf(skill, "Legend Status : %d\n\r", ch->lstatus);
    send_to_char( skill, ch );
    sprintf(skill, "Legend Points : %d\n\r", ch->lexp);
    send_to_char( skill, ch );
	 send_to_char("---------------------------=[#6Class Things#n]=---------------------------------\n\r", ch);
    
    if ( IS_CLASS(ch, CLASS_DEMON) )
     send_to_char("You are a demon!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_MAGE) )
     send_to_char("You are a battlemage!\n\r", ch);    
     if ( IS_CLASS(ch ,CLASS_SHAPESHIFTER))
       send_to_char("You are a shapeshifter!\n\r",ch);
     if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
       send_to_char("You are an undead knight!\n\r",ch);
     if ( IS_CLASS(ch, CLASS_WEREWOLF) )
     send_to_char("You are a werewolf!\n\r", ch);
     if ( IS_CLASS(ch, CLASS_NINJA) )
     send_to_char("You are a ninja!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_VAMPIRE) )
     send_to_char("You are a vampire!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_HIGHLANDER) )
     send_to_char("You are a Samurai!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_PALADIN))
	stc("You are a paladin!\n\r",ch);
    if ( IS_CLASS(ch, CLASS_DROW) )
     send_to_char("You are a drow!\n\r", ch);    
    if (IS_CLASS(ch, CLASS_ANGEL))
     send_to_char("You are an angel!\n\r", ch);
    if (IS_CLASS(ch, CLASS_TANARRI))
     send_to_char("You are a warrior of The Bloodwars.\n\r",ch);
    if( IS_CLASS(ch, CLASS_MONK) )
	stc( "You are a monk!\n\r",ch);
    if( IS_CLASS(ch, CLASS_CYBORG))
	stc( "You are a Cybernetic drow!\n\r",ch);
    if( IS_CLASS(ch, CLASS_PSIONIST))
	stc( "You are a Psionist!\n\r",ch);
    if( IS_CLASS(ch, CLASS_INQUISITOR))
	stc( "You are an Inquisitor!\n\r",ch);
    sprintf(skill, "Generation : %d\n\r", ch->generation);
    send_to_char( skill, ch );

 if (IS_CLASS(ch, CLASS_TANARRI))
 {
  if (ch->pcdata->rank == TANARRI_FODDER)
    send_to_char("You are nothing but fodder for the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_FIGHTER)
    send_to_char("You are a fighter in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_ELITE)
    send_to_char("You are an elite warrior in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_CAPTAIN)
    send_to_char("You are a captain in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_WARLORD)
    send_to_char("You are a warlord in the great war.\n\r",ch);
  if (ch->pcdata->rank == TANARRI_BALOR)
    send_to_char("You are a Tanar'ri Balor.\n\r",ch);
 } 
 if (IS_CLASS(ch, CLASS_PSIONIST))
 {
  if (ch->pcdata->rank == PSIONIST_NOVICE)
	stc("You are a Novice Psionist.\n\r",ch);
  if (ch->pcdata->rank == PSIONIST_APPRENTICE)
        stc("You are a Psionist Apprentice.\n\r",ch);
  if (ch->pcdata->rank == PSIONIST_JOURNEYMAN)
        stc("You are a Psionist Journeyman.\n\r",ch);
  if (ch->pcdata->rank == PSIONIST_PSIONIST)
        stc("You are a Psionist.\n\r",ch);
  if (ch->pcdata->rank == PSIONIST_MASTER)
        stc("You are a Psionist Master.\n\r",ch);
  if (ch->pcdata->rank == PSIONIST_MINDBENDER)
        stc("You are a Psionist Mindbender.\n\r",ch);
 }
 if (IS_CLASS(ch, CLASS_VAMPIRE) )
 {

	if (ch->pcdata->rank == AGE_TRUEBLOOD)
	  send_to_char("You are a TrueBlood!\n\r",ch);
	else if (ch->pcdata->rank == AGE_LA_MAGRA)
	  send_to_char("You are a La Magra!\n\r",ch);
	else if (ch->pcdata->rank == AGE_METHUSELAH)
	  send_to_char("You are a Methuselah!\n\r",ch);
	else if (ch->pcdata->rank == AGE_ELDER)
	  send_to_char("You are an Elder!\n\r",ch);
	else if (ch->pcdata->rank == AGE_ANCILLA)
	  send_to_char("You are an Ancilla!\n\r",ch);
	else if (ch->pcdata->rank == AGE_NEONATE)
	  send_to_char("You are a Neonate!\n\r", ch);
	else
	  send_to_char("You are a Childe!\n\r", ch);
	if ( IS_SET(ch->special, SPC_PRINCE) )
	  send_to_char("You are a prince!\n\r", ch);
	if ( IS_SET(ch->special, SPC_GENERAL))
	  send_to_char("You are a general!\n\r",ch);
 }
 
 if (IS_CLASS(ch, CLASS_NINJA)&&!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_WEREWOLF) )
 {
	if ( IS_SET(ch->special, SPC_PRINCE) )
	  send_to_char("You can discipline people!\n\r", ch);
 }
 
}


void do_smother( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *inroom;
    one_argument( argument, arg );
    if (IS_NPC( ch )) return;
    inroom=ch->in_room;
    if ( arg[0] == '\0' && !IS_SET(inroom->room_flags,ROOM_FLAMING))
    {
	send_to_char( "Smother whom?\n\r", ch );
	return;
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

/*    if ( ch == victim )
    {
	send_to_char( "You cannot smother yourself.\n\r", ch );
	return;
    }
*/
    if (!IS_AFFECTED(victim, AFF_FLAMING))
    {
	send_to_char( "But they are not on fire!\n\r", ch );
	return;
    }

    if ( number_percent() > (ch->level*10) )
    {
	act( "You try to smother the flames around $N but fail!",  ch, NULL, victim, TO_CHAR, FALSE    );
	act( "$n tries to smother the flames around you but fails!", ch, NULL, victim, TO_VICT, FALSE    );
	act( "$n tries to smother the flames around $N but fails!",  ch, NULL, victim, TO_NOTVICT, FALSE );
	if ( number_percent() > 98 && !IS_AFFECTED(ch,AFF_FLAMING) )
	{
	    act( "A spark of flame from $N's body sets you on fire!",  ch, NULL, victim, TO_CHAR, FALSE    );
	    act( "A spark of flame from your body sets $n on fire!", ch, NULL, victim, TO_VICT, FALSE    );
	    act( "A spark of flame from $N's body sets $n on fire!",  ch, NULL, victim, TO_NOTVICT, FALSE );
	    SET_BIT(ch->affected_by, AFF_FLAMING);
	    do_humanity(ch,"");
	}
	return;
    }

    act( "You manage to smother the flames around $M!",  ch, NULL, victim, TO_CHAR, FALSE    );
    act( "$n manages to smother the flames around you!", ch, NULL, victim, TO_VICT, FALSE    );
    act( "$n manages to smother the flames around $N!",  ch, NULL, victim, TO_NOTVICT, FALSE );
    REMOVE_BIT(victim->affected_by, AFF_FLAMING);
    do_humanity(ch,"");
    return;
}
	
void do_introduce( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );


    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->generation > 0 && ch->generation < 14)
    {
      if ( ch->generation == 1 )
      {
	sprintf(buf,"My name is %s.  I am of the First Generation.  I am the Sire of all Kindred.\n\r", ch->name);
	do_say(ch,buf);
      }
      else if ( ch->generation == 2 )
      {
	sprintf(buf,"My name is %s.  I am of the Second Generation.  My sire is Caine.\n\r",ch->name );
	do_say(ch,buf);
      }
      else if ( ch->generation == 3 )
      {
	sprintf(buf,"My name is %s.  I am of the Third Generation.  My sire is Caine.\n\r",ch->name );
	do_say(ch, buf);
      }
      else if ( ch->generation > 3 )
      {
	sprintf(buf,"My name is %s.  I am of the %dth Generation.  My sire is Caine.\n\r",ch->name, ch->generation );
	do_say(ch,buf);
      }
      else return;
    }
    else if (IS_CLASS(ch, CLASS_WEREWOLF) && IS_HERO(ch) &&
        ch->generation >= 0 && ch->generation < 7)
    {
        if ( ch->generation == 0 )
        {
          sprintf(buf,"My name is %s.  I am Gaia's champion, sworn to vengeance.\n\r",ch->name );
          do_say(ch,buf);
        }
	else if ( ch->generation == 1 )
	{
	  sprintf(buf,"My name is %s.  I am the chosen champion of Gaia.\n\r",ch->name );
	  do_say(ch,buf);
	}
	else if ( ch->generation > 1 )
	{
	  sprintf(buf,"My name is %s.  I am the pup of Gaia.",ch->name );
	  do_say(ch,buf);
	}
        return;
    }
    else send_to_char("Huh?\n\r",ch);
    return;
}  


/* Loads of Vampire powers follow.  KaVir */
void do_coil( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_SERP] < 8)
    {
      send_to_char("You require level 8 Serpentis to use Body Coil.\n\r", ch);
      return;
    }
    if (!IS_SET(ch->newbits, NEW_COIL))
    {
     send_to_char("You prepare to coil your victims.\n\r", ch);
     SET_BIT(ch->newbits, NEW_COIL);
     return;
    }
    else if (IS_SET(ch->newbits, NEW_COIL))
    {
     send_to_char("You stand down from your coil posture.\n\r", ch);
     REMOVE_BIT(ch->newbits, NEW_COIL);
     return;
    }
 return;
}

void do_tide( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_THAU] < 5)
    {
      send_to_char("You require level 5 Thaumaturgy to use Tide of Vitae.\n\r", ch);
      return;
    }

    if (ch->practice < 10)
    {
     send_to_char("You require at least 10 primal to use Tide of Vitae.\n\r",ch);
     return;
    }

    if (IS_SET(ch->newbits, NEW_TIDE))
    {send_to_char("The tide is already with you.\n\r",ch); return;}
    SET_BIT(ch->newbits, NEW_TIDE);
    ch->practice -= 10;
    send_to_char("You feel a tide of vitae rush over you.\n\r",ch);
 
 
}

void do_flash( CHAR_DATA *ch, char *argument )
{
   char       buf[MAX_STRING_LENGTH];
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_QUIE] < 9)
    {
      send_to_char("You require level 9 Quietus to use Flashing Speed.\n\r", ch);
      return;
    }
    if (IS_EXTRA(ch, EXTRA_FLASH))
    {
     send_to_char("You are already moving swiftly.\n\r", ch);
     return;
    }
    if (ch->pcdata->condition[COND_THIRST] < 200)
    {
     send_to_char("You have insufficient blood.\n\r",ch);
     return;
    }
    xSET_BIT(ch->extra, EXTRA_FLASH);
    ch->power[DISC_VAMP_CELE] += 2;
    ch->pcdata->condition[COND_THIRST] -= 200;
    sprintf(buf, "%s starts moving with lightening speed.",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM, FALSE);
    send_to_char("You start moving with lightening speed.\n\r",ch);
    return;
}
void do_death( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *inroom;
    inroom= ch->in_room;
 
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
      send_to_char("Huh?\n\r", ch);
      return;
    }
    if (ch->power[DISC_VAMP_QUIE] < 5)
    {
      send_to_char("You require level 5 Quietus to use Silence of Death.\n\r", ch);
      return;
    }
    if (RTIMER(ch->in_room, RTIMER_SILENCE) != 0)
    {
     sprintf(buf,"The silence leaves the room.\n\r");
     act(buf,ch,NULL,NULL,TO_ROOM, FALSE);
     send_to_char("The silence leaves the room.\n\r", ch);
     SET_RTIMER(ch->in_room, RTIMER_SILENCE, 0);
     return;
    }
    if (ch->pcdata->condition[COND_THIRST] < 300)
    {
     send_to_char("You have insufficient blood.\n\r", ch);
     return;
    }
    ch->pcdata->condition[COND_THIRST] -= 300;
    SET_RTIMER(inroom, RTIMER_SILENCE, 10);
    sprintf(buf,"A look of concentration passes over %s's face.\n\r", ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM, FALSE);
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    sprintf(buf,"An eerie silence fills the room.\n\r");
    act(buf,ch,NULL,NULL,TO_ROOM, FALSE);
    send_to_char(buf,ch);
    return;
}


void do_forget( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1[MAX_INPUT_LENGTH];
    char      arg2[MAX_INPUT_LENGTH];
 
 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->power[DISC_VAMP_DOMI] < 8)
    {
        send_to_char("You must obtain at least level 8 in Dominate to use Forgetful Mind.\n\r",ch);
	return;
    }
 
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Force whom to forget what disc?\n\r", ch );
	return;
    }
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("Why use Forgetful Mind on a mob?\n\r", ch);
        return;
    }
    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
        send_to_char("Only useful on vamps?\n\r", ch);
        return;
    }
 
 
    if ( IS_IMMORTAL(victim) && victim != ch )
    {
        send_to_char( "You can only use Forgetful Mind on Avatar's or lower.\n\r", ch );
	return;
    }
    if (ch->pcdata->condition[COND_THIRST] < 250)
    {
    send_to_char("You have insufficient blood.\n\r", ch);
    return;
    }
 
    if (ch->practice < 25)
    {
    send_to_char("You need 25 primal to use Forgetful Mind.\n\r", ch);
    return;
    }
 
    act("You close your eyes and concentrate on $N.", ch,NULL,victim,TO_CHAR, FALSE);
    act("$n closes $s eyes and concentrates on you.", ch,NULL,victim,TO_VICT, FALSE);
    act("$n closes $s eyes and cencentrates on $N.", ch,NULL,victim,TO_NOTVICT, FALSE);
    ch->pcdata->condition[COND_THIRST] -= 250;
    ch->practice -= 25;
    WAIT_STATE(ch, 12);
    
 
    if ( number_percent() > 10)
    {
        send_to_char("You failed.\n\r",ch);
        return;
    }
 
    if ( !str_cmp(arg2,"animalism") )
    {
    victim->power[DISC_VAMP_ANIM] -= 1;
    act("Your mind becomes hazy about Animalism.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"celerity") )
    {
    victim->power[DISC_VAMP_CELE] -= 1;
    act("Your mind becomes hazy about Celerity.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"dominate") )
    {
    victim->power[DISC_VAMP_DOMI] -= 1;
    act("Your mind becomes hazy about Dominate.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"fortitude") )
    {
    victim->power[DISC_VAMP_FORT] -= 1;
    act("Your mind becomes hazy about Fortitude.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"auspex") )
    {
    victim->power[DISC_VAMP_AUSP] -= 1;
    act("Your mind becomes hazy about Auspex.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"obfuscate") )
    {
    victim->power[DISC_VAMP_OBFU] -= 1;
    act("Your mind becomes hazy about Obfuscate.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"obtenebration") )
    {
    victim->power[DISC_VAMP_OBTE] -= 1;
    act("Your mind becomes hazy about Obtenebration.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"presence") )
    {
    victim->power[DISC_VAMP_PRES] -= 1;
    act("Your mind becomes hazy about Presence.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"potence") )
    {
    victim->power[DISC_VAMP_POTE] -= 1;
    act("Your mind becomes hazy about Potence.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"protean") )
    {
    victim->power[DISC_VAMP_PROT] -= 1;
    act("Your mind becomes hazy about Protean.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"quietus") )
    {
    victim->power[DISC_VAMP_QUIE] -= 1;
    act("Your mind becomes hazy about Quietus.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"serpentis") )
    {
    victim->power[DISC_VAMP_SERP] -= 1;
    act("Your mind becomes hazy about Serpentis.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"thaumaturgy") )
    {
    victim->power[DISC_VAMP_THAU] -= 1;
    act("Your mind becomes hazy about Thaumaturgy.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
    if ( !str_cmp(arg2,"vicissitude") )
    {
    victim->power[DISC_VAMP_VICI] -= 1;
    act("Your mind becomes hazy about Vicissitude.", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n blinks $s eyes for a moment.", victim,NULL,NULL,TO_ROOM, FALSE);
    return;
    }
 
return;
 
}
 
 

void do_rot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg );
 
    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->power[DISC_VAMP_VICI] < 2)
    {
        send_to_char("You must obtain at level 2 in Thanatosis to use Putrefaction.\n\r",ch);
	return;
    }
 
 
    if ( arg[0] == '\0' )
    {
        send_to_char( "Who do you wish to use Putrefaction on?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_NPC(victim))
    {
        send_to_char("Why use Putrefaction on a mob?\n\r", ch);
        return;
    }

    if ( IS_IMMORTAL(victim) && victim != ch )
    {
        send_to_char( "You can only use Putrefaction on Avatar's or lower.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim) == TRUE) return;

    if (victim->hit < victim->max_hit)
    {
      send_to_char("They are hurt and suspicious.\n\r", ch);
      return;
    }
    act("You close your eyes and concentrate on $N.", ch,NULL,victim,TO_CHAR, FALSE);
    act("$n closes $s eyes and concentrates on you.", ch,NULL,victim,TO_VICT, FALSE);
    act("$n closes $s eyes and cencentrates on $N.", ch,NULL,victim,TO_NOTVICT, FALSE);
    WAIT_STATE(ch, 12);
    if (number_percent( ) < 50)
    {
        send_to_char("You failed.\n\r",ch);
        return;
    }
 
    act("Your flesh begins to rot!", victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n's flesh begins to rot!", victim,NULL,NULL,TO_ROOM, FALSE);
    xSET_BIT(victim->extra, EXTRA_ROT);
    return;
}
 

void do_dragon (CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        char buf[MAX_STRING_LENGTH];
        int dam;
 
        if (IS_NPC(ch)) return;
        if (!IS_CLASS(ch, CLASS_VAMPIRE))
        {
         send_to_char("Huh?\n\r", ch);
         return;
        }
        if (ch->power[DISC_VAMP_VICI] < 8)
        {
         send_to_char("You must have at least level 8 Vicissitude to use Dragon Breath.\n\r",ch);
         return;
        }
        if ((victim = ch->fighting) == NULL)
	{
	send_to_char("You aren't fighting anyone.\n\r", ch);
	return;
	}
        if (!IS_EXTRA(ch,EXTRA_DRAGON))
        {
        send_to_char("You must be in Dragonform to use Dragonbreath.\n\r",ch);
        return;
        }
        if (ch->pcdata->condition[COND_THIRST] < 6)
        {
         send_to_char("You have insufficient blood.\n\r", ch);
         return;
        }
        WAIT_STATE(ch, 12);
        if (!IS_NPC(victim))
        {
         dam = ch->power[DISC_VAMP_VICI] * 50;
        }
        if (IS_NPC(victim))
        {
         dam = ch->power[DISC_VAMP_VICI] * 200;
        }
        ch->pcdata->condition[COND_THIRST] -= 6;
        if ((!IS_NPC(victim)) && IS_CLASS(victim, CLASS_VAMPIRE))
        {
         dam *= 1.5;
        }
        if ((!IS_NPC(victim)) && IS_CLASS(victim, CLASS_WEREWOLF))
        {
         if (ch->power[DISC_WERE_BOAR] > 2) dam *= 1.5;
        }
 
        if (dam <= 0)
          dam = 1;
sprintf(buf,"Your flame fries $N! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_CHAR, FALSE);
sprintf(buf,"$n's flame fries you! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_VICT, FALSE);
sprintf(buf,"$n's flame fries $N! [%d]\n\r",dam);
act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
 

      hurt_person(ch,victim, dam);
     if(!IS_AFFECTED(victim,AFF_FLAMING)) SET_BIT(victim->affected_by,AFF_FLAMING);
 
       return;
}         


void do_awe    ( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch)) return;
        if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_PRES] < 1)
	{
	  send_to_char("You must obtain at least level 1 in Presence to you Awe.\n\r", ch);
	  return;
	}
	if (IS_CLASS(ch, CLASS_PSIONIST) && !IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_AWE))
	{
	  stc("You should achieve this command first.\n\r",ch);
	  return;
	}
	else if (!IS_CLASS(ch, CLASS_PSIONIST) && !IS_CLASS(ch, CLASS_VAMPIRE))
	{
	  stc("Huh?\n\r",ch);
	  return;
	}
        if (IS_EXTRA(ch, EXTRA_AWE))
	{
	  xREMOVE_BIT(ch->extra, EXTRA_AWE);
	  send_to_char("You are no longer Awe Inspiring.\n\r", ch);
	  act("$n is no longer awe inspiring.", ch, NULL, NULL, TO_ROOM, FALSE);
	  return;
	}
        xSET_BIT(ch->extra, EXTRA_AWE);
	send_to_char("You are now Awe Inspiring.\n\r", ch);
	act("$n is now awe inspiring", ch, NULL, NULL, TO_ROOM, FALSE);
        return;


}

void do_plasma( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    
     if ( IS_NPC(ch) )
	return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE)) 
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }

    if (ch->power[DISC_VAMP_VICI] < 5)
    {
       send_to_char("You must obtain at least level 5 in Vicissitude to use Plasma Form.\n\r", ch);
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
  if ((obj = create_object(get_obj_index(30007),60)) == NULL)
  {
    send_to_char( "Object bug, tell Jank about it.\n\r", ch);
    return;
  }
  obj_to_room(obj,ch->in_room);
  act("$n transforms into $p and falls to the ground.",ch,obj,NULL,TO_ROOM, FALSE);
  act("You transform into $p and fall to the ground.",ch,obj,NULL,TO_CHAR, FALSE);
  ch->pcdata->obj_vnum = 30007;
  obj->chobj = ch;
  ch->pcdata->chobj = obj;
  SET_BIT(ch->affected_by, AFF_POLYMORPH);
  xSET_BIT(ch->extra, EXTRA_OSWITCH);
  free_string(ch->morph);
  ch->morph = str_dup(obj->short_descr);
  return;
}
 
void do_taste( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      buf [MAX_STRING_LENGTH];
    char      age [MAX_STRING_LENGTH];
    char      lin [MAX_STRING_LENGTH];
    char     lord [MAX_STRING_LENGTH];
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument (argument, arg );

    if (IS_NPC(ch)) return;
 
    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    if (ch->power[DISC_VAMP_THAU] < 1)
    {
        send_to_char("You must obtain at least level 1 in Thaumaturgy to use Taste of Blood.\n\r",ch);
	return;
    }
 
       if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
        send_to_char( "Only useful on vampire targets.\n\r",ch);
        return;
    }

    if (IS_NPC(victim))
    {
        send_to_char( "Only useful on vampire targets.\n\r",ch);
        return;
    }                               

sprintf(buf,"You examine $N's blood carefully.\n\r");
act(buf,ch,NULL,victim,TO_CHAR, FALSE);
sprintf(buf,"$n examines your blood carefully.\n\r");
act(buf,ch,NULL,victim,TO_VICT, FALSE);
sprintf(buf,"$n examines $N's blood carefully.\n\r");
act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
 

send_to_char("\n\r",ch);
send_to_char("\n\r",victim);


if (victim->pcdata->rank == AGE_ANCILLA)         sprintf( age, "Ancilla");
else if (victim->pcdata->rank == AGE_CHILDE)     sprintf( age, "Childe");
else if (victim->pcdata->rank == AGE_NEONATE)    sprintf( age, "Neonate");
else if (victim->pcdata->rank == AGE_ELDER)      sprintf( age, "Elder");
else if (victim->pcdata->rank == AGE_METHUSELAH) sprintf( age, "Methuselah");
else if (victim->pcdata->rank == AGE_LA_MAGRA)   sprintf( age, "La Magra");
else if (victim->pcdata->rank == AGE_TRUEBLOOD)  sprintf( age, "TrueBlood");
if (victim->lord == NULL) sprintf(lord, "None");
    else sprintf(lord, "%s",victim->lord);
    sprintf( lin,
"---------------------------------------------------------------------------\n\r");
send_to_char( lin,ch);
send_to_char(
"                              Vampire Status\n\r",ch);
send_to_char(lin,ch);
sprintf(buf,
"Generation:%d  Bloodpool:%d  Age:%s  Lord:%s\n\r",
victim->generation,victim->pcdata->condition[COND_THIRST],
age,lord);
send_to_char(buf,ch);
send_to_char(lin,ch);
send_to_char(
"                              Disciplines\n\r",ch);
send_to_char(lin,ch);
sprintf(buf,
"Animalism:    [%d]             Celerity:   [%d]             Fortitude: [%d]\n\r",
victim->power[DISC_VAMP_ANIM],
victim->power[DISC_VAMP_CELE],
victim->power[DISC_VAMP_FORT]);
send_to_char(buf,ch);
sprintf(buf,
"Obtenebration:[%d]             Presence:   [%d]             Quietus:   [%d]\n\r",
victim->power[DISC_VAMP_OBTE],
victim->power[DISC_VAMP_PRES],
victim->power[DISC_VAMP_QUIE]);
send_to_char(buf,ch);
sprintf(buf,
"Thaumaturgy:  [%d]             Auspex:     [%d]             Dominate:  [%d]\n\r",
victim->power[DISC_VAMP_THAU],
victim->power[DISC_VAMP_AUSP],
victim->power[DISC_VAMP_DOMI]);
send_to_char(buf,ch);
sprintf(buf,
"Obfuscate:    [%d]             Potence:    [%d]             Protean:   [%d]\n\r",
victim->power[DISC_VAMP_OBFU],
victim->power[DISC_VAMP_POTE],
victim->power[DISC_VAMP_PROT]);
send_to_char(buf,ch);
sprintf(buf,
"Serpentis:    [%d]             Vicissitude:[%d]             Daimoinon: [%d]\n\r",
victim->power[DISC_VAMP_SERP],
victim->power[DISC_VAMP_VICI],
victim->power[DISC_VAMP_DAIM]);
send_to_char(buf,ch);
send_to_char(lin,ch);
 
return;
}
 
 




void do_shadowstep( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if (ch->power[DISC_VAMP_OBTE] < 4)
	{
           send_to_char("You must obtain at least level 4 in Obtenebration to use this power.\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Shadowstep to who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
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
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }

 if (IS_IMMUNE(victim, IMM_TRAVEL)) 
    {
    send_to_char("He doesnt want you near him!\n\r",ch);
    return;
    }

/*
    if (IS_NPC(victim))
    {
	send_to_char("Not to mobs!\n\r",ch);
	return;
    }
*/
    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
	  return;
    }

    if (IS_NPC(victim) && (!room_is_dark(victim->in_room)))
    {
	send_to_char("You cant find a shadow in his room.\n\r",ch);
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

    if (victim->in_room == ch->in_room)
    {
	send_to_char("But you're already there!\n\r",ch);
        return;
    }

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    act("You step into the shadows and appear before $N.", ch, NULL, victim, TO_CHAR, FALSE);
    act("$n appears out of the shadows before $N.", ch, NULL, victim, TO_NOTVICT, FALSE);
    act("$n appears from the shadows in front of you.", ch, NULL, victim, TO_VICT, FALSE);
    do_look(ch,"auto");
    return;
}

void do_earthmeld( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ch->power[DISC_VAMP_PROT] < 4 )
    {
        send_to_char("You must obtain at least level 4 Protean to use Earthmeld.\n\r",ch);
	return;
    }
    

    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

	if (has_timer(ch)) return;

    if ( IS_NPC(ch) || IS_EXTRA(ch,EXTRA_EARTHMELD) )
    {
            REMOVE_BIT(ch->affected_by, AFF_SHIFT);
            xREMOVE_BIT(ch->extra, EXTRA_EARTHMELD);
            xREMOVE_BIT(ch->act, PLR_WIZINVIS);
           if (IS_HEAD(ch,LOST_HEAD)) REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
            free_string(ch->morph);
            ch->morph = str_dup("");
           send_to_char( "You rise up from the ground.\n\r", ch );
            sprintf(buf, "%s rises up from the ground",ch->name);
            act(buf,ch,NULL,NULL,TO_ROOM, FALSE);

            return;

    }

    if ( (mount = ch->mount) != NULL) do_dismount(ch,"");
    SET_BIT(ch->affected_by, AFF_SHIFT);
    xSET_BIT(ch->extra, EXTRA_EARTHMELD);
    xSET_BIT(ch->act, PLR_WIZINVIS);
    free_string(ch->morph);
    ch->morph = str_dup("Someone");
    send_to_char( "You sink into the ground.\n\r", ch );
    sprintf(buf, "%s sinks into the ground.",ch->name);
    act(buf,ch,NULL,NULL,TO_ROOM, FALSE);
    return;
}

void do_serenity( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;
    if(!IS_CLASS(ch, CLASS_VAMPIRE))
    {
        send_to_char("huh?.\n\r",ch);
        return;
    }
    if (ch->power[DISC_VAMP_ANIM] < 2)
    {
        send_to_char("You must obtain at least level 2 in Animalism to use Song of Serenity.\n\r",ch);
	return;
    }



    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->beast < 1)
    {
	if ( ch->rage < 1 )
	{
	    send_to_char("Your beast doesn't control your actions.\n\r",ch);
	    return;
	}
        send_to_char("You chant a soft tune and pacify your inner beast.\n\r",ch);
        act("$n chants a soft tune and pacifies their inner beast.",ch,NULL,NULL,TO_ROOM, FALSE);
	ch->rage = 0;
	if (IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) do_nightsight(ch,"");
	if (IS_VAMPAFF(ch, VAM_FANGS)) do_fangs(ch,"");
	if (IS_VAMPAFF(ch, VAM_CLAWS)) do_claws(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    else 
    {
     send_to_char("Only those who achieve Golconda can sing the Song of Serenity.\n\r",ch);
     return;
    }
}

void do_theft( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    int bloodpool;
	int blpr; /* Blood sucked storage variable. Shakti */

    argument = one_argument( argument, arg );

    if (ch->generation < 0)
        ch->generation = 4;

    {
    if (IS_SET(ch->newbits, NEW_TIDE))
    bloodpool = (3000 / ch->generation);
    else bloodpool = (2000 / ch->generation);
    } 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (ch->power[DISC_VAMP_THAU] < 4)
    {
        send_to_char("You must obtain at least level 4 in Thaumaturgy to use Theft of Vitae.\n\r",ch);
	return;
    }


    if ( arg[0] == '\0' )
    {
        send_to_char( "Steal blood from whom?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
	if (IS_NPC(victim) && xIS_SET(victim->act, ACT_NOAUTOKILL)) {
	send_to_char("You can't do that to them.\n\r", ch );
	return;}

/*
    if ( IS_NPC(victim) ) 
    {
        send_to_char( "Lower life forms are immune to Theft of Vitae.\n\r", ch);
        return;
    }
*/
    if (!IS_NPC(victim) && victim->pcdata->condition[COND_THIRST] <= 0)
    {
        send_to_char( "There isn't enough blood to steal.\n\r", ch);
        return;
    }
    if (!IS_NPC(victim) && IS_IMMORTAL(victim) && victim != ch )
    {
        send_to_char( "You can only steal blood from Avatar's or lower.\n\r", ch );
	return;
    }
if (is_safe(ch,victim) == TRUE) return;

if (!IS_NPC(victim)) {
        sprintf(buf,"A stream of blood shoots from %s into your body.",victim->name);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
        sprintf(buf,"A stream of blood shoots from %s into %s.",victim->name,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM, FALSE);
        sprintf(buf,"A stream of blood shoots from your body into %s.",ch->name);
        act(buf,ch,NULL,victim,TO_VICT, FALSE);
        } else {
        sprintf(buf,"A stream of blood shoots from %s into your body.",victim->short_descr);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
        sprintf(buf,"A stream of blood shoots from %s into %s.",victim->short_descr,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM, FALSE);
        sprintf(buf,"A stream of blood shoots from your body into %s.",ch->name);
        act(buf,ch,NULL,victim,TO_VICT, FALSE);


	}
        ch->pcdata->condition[COND_THIRST] += number_range(10,15);
     
		
		if (IS_NPC(victim))
        {
/* Raw-killing it from one theft is stupid. Im going to use the primal        */
/* stat on the mobs for blood its quick, effective, and straightford, AND	  */
/* no new fields have to be added to the mob.Shakti 09/07/98				  */
			(blpr = number_range (30,40) );
			(victim->practice -=blpr);
            (ch->pcdata->condition[COND_THIRST] += blpr);
		if (victim->practice < 0) {
		sprintf(buf,"%s falls over dead.",victim->short_descr);
		act(buf,ch,NULL,victim,TO_ROOM, FALSE);
		act(buf,ch,NULL,victim,TO_CHAR, FALSE);
		raw_kill(victim);}		

		 if (ch->pcdata->condition[COND_THIRST] >= bloodpool /ch->generation)
             {
              ch->pcdata->condition[COND_THIRST] = bloodpool / ch->generation;
             }
	if (ch->fighting == NULL) set_fighting(ch,victim);

         return;
        }

    if (!IS_NPC(victim)) 
{
victim->pcdata->condition[COND_THIRST] -=number_range(30,40);
}
    if (ch->pcdata->condition[COND_THIRST] >= bloodpool / ch->generation)
    {
       ch->pcdata->condition[COND_THIRST] = bloodpool /ch->generation;
    }

   if (victim->pcdata->condition[COND_THIRST] <= 0)
    {
       victim->pcdata->condition[COND_THIRST] = 0; 
    }
	if (ch->fighting == NULL) set_fighting(ch,victim);

   return;
}


void do_demonform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    /*if (ch->power[DISC_VAMP_VICI] < 4)
    {
        send_to_char("You must obtain at least level 4 in Vicissitude to
use Zuloform.\n\r",ch);
	return;
    }*/
    
    
    if (!IS_DEMPOWER( ch, DEM_FORM) && IS_CLASS(ch,CLASS_DEMON))
    {
	send_to_char("You haven't been granted the gift of demonform.\n\r",ch);
	return;
    }
    

    if (IS_POLYAFF(ch, POLY_ZULOFORM))
    {   
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR, FALSE );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM, FALSE
);
        ch->damroll -= 1000;
        ch->hitroll -= 1000;
        ch->armor   += 1000;
	free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
send_to_char("You cant demon form when changed.\n\r",ch);
return;
}
  
  

        if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
        act( "You transform into a huge demon.", ch, NULL, NULL, TO_CHAR, FALSE);
        act( "$n's body grows and distorts into a huge demon.", ch, NULL,NULL, TO_ROOM, FALSE );
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->polyaff, POLY_ZULOFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        sprintf(buf, "%s the huge hulking demon", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
	ch->hit +=  500;
	ch->damroll += 1000;
	ch->hitroll += 1000;
        ch->armor   -= 1000;
        return;
    }
  




void do_zuloform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;
   

    if (ch->power[DISC_VAMP_VICI] < 2)
    {
        send_to_char("You must obtain at least level 2 in Vicissitude to use Zuloform.\n\r",ch);
	return;
    }

    if (IS_EXTRA(ch,EXTRA_DRAGON))
    {
    send_to_char("You cannot unzulo while a dragon.\n\r",ch);
    return;
    }
    if(IS_POLYAFF(ch, POLY_DRAGON))
    {
	stc( "You can't do that.\n\r",ch);
	return;
    }

    if (IS_POLYAFF(ch, POLY_ZULOFORM))
    {   
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You transform into human form.", ch, NULL, NULL, TO_CHAR, FALSE );
        act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM, FALSE );
        if (ch->hit < 1) ch->hit = 1;
        ch->damroll = ch->damroll - 350;
        ch->hitroll = ch->hitroll - 350; 
	ch->armor = ch->armor + 200;
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
send_to_char("You cant zulo when changed.\n\r",ch);
return;
}
  
  
    if ( ch->pcdata->condition[COND_THIRST] < 200 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}

        if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(100,200);
        act( "You transform into large beast.", ch, NULL, NULL, TO_CHAR, FALSE);
        act( "$n's body grows and distorts into a large beast.", ch, NULL, NULL, TO_ROOM, FALSE );
        /*
    	if (ch->wpn[0] > 0)
    	{
	    ch->hitroll += (ch->wpn[0]);
	    ch->damroll += (ch->wpn[0]);
	    ch->armor   -= (ch->wpn[0] * 3);
    	}
        */
        ch->pcdata->mod_str = 15;
        ch->pcdata->mod_dex = 15;
        SET_BIT(ch->polyaff, POLY_ZULOFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        sprintf(buf, "A big black monster");
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        ch->damroll = ch->damroll + 350;
        ch->hitroll = ch->hitroll + 350;
	ch->armor = ch->armor - 200;
        return;
    }
  


void horn args((CHAR_DATA *ch));

/* claw and fang commands */

void declaw(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_CLAWS)) return;

    send_to_char("You retract your claws.\n\r",ch);
    act("$n retracts $s claws.",ch, NULL, NULL, TO_ROOM, FALSE);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
}

void dehorn(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_HORNS)) return;

    send_to_char("Your horns burrow back into your skull.\n\r",ch);
    act("$n's horns retract into $s skull.",ch, NULL, NULL, TO_ROOM, FALSE);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
}

void horn(CHAR_DATA *ch)
{
    OBJ_DATA *obj;
  
    if (IS_VAMPAFF(ch, VAM_HORNS)) return;

    /* Only certain demons may use their horns */
    if (ch->power[DISC_DAEM_ATTA] < 4) return;

    send_to_char("Sharp horns grow out of your skull.\n\r",ch);
    act("Sharp horns grow out of $n's skull.",ch, NULL, NULL, TO_ROOM, FALSE);

    if ( (obj = get_eq_char(ch, WEAR_HEAD)) != NULL) 
    { 
        act("Your horns shred $p.", ch, obj, NULL, TO_CHAR, FALSE); 
        act("$n's horns shred $p.", ch, obj, NULL, TO_ROOM, FALSE); 
        extract_obj(obj); 
    } 

    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_HORNS);
}

void defang(CHAR_DATA *ch)
{
    if (!IS_VAMPAFF(ch, VAM_FANGS)) return;

    send_to_char("Your fangs slide back into your gums.\n\r",ch);
    act("$n retracts $s fangs back into $s gums.",ch, NULL, NULL, TO_ROOM, FALSE);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
}

void do_claws(CHAR_DATA *ch, char *argument)
{
    if (ch->cur_form != 0 && !form_data[ch->cur_form].can_use_claws )
    {
	send_to_char("You cannot use your claws in this form.\n\r", ch);
	return;
    }
   if (IS_CLASS(ch, CLASS_WEREWOLF))
   {
	if (ch->power[DISC_WERE_WOLF] < 1)
      {
	    stc("Huh?\n\r",ch);
	    return;
	}
   }
   else if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	if (!IS_DEMPOWER( ch, DEM_CLAWS) && ch->power[DISC_DAEM_ATTA] < 1)
	{
	    send_to_char("You haven't been granted the gift of claws or attack is below level 1.\n\r",ch);
	    return;
	}
    }
   else if (IS_CLASS(ch, CLASS_VAMPIRE))
   {
	if (ch->power[DISC_VAMP_PROT] < 2)
	{
	    stc("Huh?\n\r",ch);
	    return;
	}
   }
   else if (IS_CLASS(ch, CLASS_TANARRI))
   {
   if (!IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_CLAWS))
     {
       send_to_char("Huh?\n\r",ch);
       return;
     }
  }
   else
   {
	stc("Huh?\n\r",ch);
	return;
   }

   if (IS_VAMPAFF(ch, VAM_CLAWS))
   {
     if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_DEMON))
     {
	if (ch->rage >0)
	{    
	    send_to_char("You cannot retract your claws while the beast is within you.\n\r", ch);
	    return;
	}
     }
	send_to_char("You retract your claws.\n\r",ch);
	act("$n retracts $s claws.",ch, NULL, NULL, TO_ROOM, FALSE);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   }
   else
   {
	send_to_char("Wicked claws extend from your fingers.\n\r",ch);
	act("Long sharp claws extend from $n's fingers.",ch, NULL, NULL, TO_ROOM, FALSE);
	remove_obj( ch, WEAR_WIELD, TRUE );
	remove_obj( ch, WEAR_HOLD, TRUE );
	remove_obj( ch, WEAR_THIRD, TRUE );
	remove_obj( ch, WEAR_FOURTH, TRUE );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
   }
}

void do_fangs(CHAR_DATA *ch, char *argument)
{
    if (ch->cur_form != 0 && !form_data[ch->cur_form].can_use_claws )
    {
	send_to_char("You cannot use your fangs in this form.\n\r", ch);
	return;
    }

   if (IS_CLASS(ch, CLASS_WEREWOLF))
   {
	if (ch->power[DISC_WERE_WOLF] < 2)
        {
	    stc("Huh?\n\r",ch);
	    return;
	}
   }

	else if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	if (!IS_DEMPOWER( ch, DEM_FANGS) && ch->power[DISC_DAEM_ATTA] < 2)
	{
	    send_to_char("You haven't been granted the gift of fangs.\n\r",ch);
	    return;
	}
    }

   else if (!IS_CLASS(ch, CLASS_VAMPIRE))
   {
	stc("Huh?\n\r",ch);
	return;
   }

   if (IS_VAMPAFF(ch, VAM_FANGS))
   {
     if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_DEMON))
     {
	if (ch->rage >0)
	{    
	    send_to_char("You cannot retract your fangs while the beast is within you.\n\r", ch);
	    return;
	}
     }
	send_to_char("Your fangs slide back into your gums.\n\r",ch);
	act("$n retracts $s fangs back into $s gums.",ch, NULL, NULL, TO_ROOM, FALSE);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
   }
   else
   {
	send_to_char("Sharp fangs slide out of your gums.\n\r",ch);
	act("Sharp fangs slide out of $n's gums.",ch, NULL, NULL, TO_ROOM, FALSE);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
   }
}

void do_nightsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_WEREWOLF))
    {/*supposed to be < but changedd while gifts are out */
	if ( ch->power[DISC_WERE_HAWK] < 1 )
	{
	    send_to_char("Your power in hawk is not great enough yet.\n\r",ch);
	    return;
	}
    }
    else if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {
	if (!IS_DEMPOWER( ch, DEM_EYES))
	{
	    send_to_char("You haven't been granted the gift of nightsight.\n\r",ch);
	    return;
	}
    }
    else if (IS_CLASS(ch, CLASS_MONK) )
    {
	if ( ch->monkab[AWARE] < 1 )
	{
	    stc("You must obtain level one in Awareness to use Nightsight.\n\r",ch);
	    return;
        }
    }
    else if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_PROT] < 1 &&
       ch->power[DISC_VAMP_OBTE] < 3) 
    {
        send_to_char("You must obtain at least level 1 in Protean or 3 in Obtenebration.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->rage > 0)
    {
	send_to_char("Not while your beast is in control.\n\r",ch);
	return;
    }

    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
    {
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.", ch, NULL, NULL, TO_ROOM, FALSE);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
	return;
    }
    send_to_char("Your eyes start glowing red.\n\r",ch);
    act("$n's eyes start glowing red.", ch, NULL, NULL, TO_ROOM, FALSE);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    return;
}

void do_inconnu( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }


    if (ch->pcdata->rank < AGE_ELDER)
    {
	send_to_char("You must be at least an Elder to become an Inconnu.\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_INCONNU))
    {
	send_to_char("But you are already an Inconnu!\n\r",ch);
	return;
    }

    if (ch->exp < 1000000)
    {
	send_to_char("It costs 1000000 exp to become an Inconnu.\n\r",ch);
	return;
    }

    if (IS_SET(ch->special, SPC_ANARCH)) REMOVE_BIT(ch->special, SPC_ANARCH);
    ch->exp -= 1000000;
    send_to_char("You are now an Inconnu.\n\r",ch);
    sprintf(buf,"%s is now an Inconnu!",ch->name);
    do_info(ch,buf);
    SET_BIT(ch->special, SPC_INCONNU);
    return;
}

void do_shadowsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_HAWK] < 2)
    {
	stc("Huh?\n\r",ch);
	return;
    }

    else if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[AWARE] < 2 )
    {
	stc("Huh?\n\r",ch);
	return;
    }

    else if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBTE] < 2)
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
/*
        if (ch->power[DISC_VAMP_OBTE] < 2  )
	{
            send_to_char("You must obtain at least Level 2 Obtenebration to use Shadowsight.\n\r",ch);
	    return;
	}
*/
//    }
    if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
    {
	send_to_char("You can no longer see between planes.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 10)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(5,10);
    send_to_char("You can now see between planes.\n\r",ch);
    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    return;
}

void do_class( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char      arg1 [MAX_INPUT_LENGTH];
  char      arg2 [MAX_INPUT_LENGTH];
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
        
  if ( IS_NPC(ch) ) return;
    
  if ( arg1[0] == '\0' || arg2[0] == '\0' )
  {
    send_to_char( "Syntax: class <char> <class>.\n\r", ch );
    send_to_char("#rClasses:#n\n\r", ch );
    send_to_char("None, Demon, Drow, Werewolf, Monk, Vampire, Mage, Ninja.\n\r", ch);
    send_to_char("Tanarri, Shapeshifter, Spiderdroid, Samurai, Undeadknight, Angel, Lich, Necromancer.\n\r",ch);
    return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
    send_to_char( "That player is not here.\n\r", ch);
    return;
    }
    if ( !str_cmp( arg2, "demon"      ) ) 
    {
    victim->class = CLASS_DEMON;
    set_learnable_disciplines(victim);
    send_to_char("You are now a demon!\n\r", victim);
    }
    else if (!str_cmp(arg2,"tanarri"))
    {
    victim->class = CLASS_TANARRI;
    send_to_char("You are now a Tanar'ri.\n\r",victim);
    }
    else if (!str_cmp(arg2,"undeadknight"))
    {
    victim->class = CLASS_UNDEAD_KNIGHT;
    send_to_char("You are now an Undead Knight.\n\r",victim);
    }
    else if (!str_cmp(arg2,"spiderdroid"))
    {
    victim->class = CLASS_CYBORG;
    send_to_char("You have become a mechanical drow.\n\r",victim);
    }
    else if (!str_cmp(arg2,"shapeshifter"))
    {
    victim->class = CLASS_SHAPESHIFTER;
    send_to_char("You are now a Shapeshifter.\n\r",victim);
    }
    else if (!str_cmp(arg2,"Angel"))
    {
    victim->class = CLASS_ANGEL;
    send_to_char("You are now an Angel.\n\r",victim);
    }
    else if (!str_cmp(arg2,"lich"))
    {
    victim->class = CLASS_POWER_LICH;
    send_to_char("You are now a Lich.\n\r",victim);
    }
    else if ( !str_cmp( arg2, "mage"  ) )
    {
    victim->class = 2;
    send_to_char("You are now a mage!\n\r", victim);
    }    
    else if ( !str_cmp( arg2, "werewolf"   ) ) 
    {
    victim->class = CLASS_WEREWOLF;
    set_learnable_disciplines(victim);
    send_to_char("You are now a werewolf!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "vampire"    ) ) 
    {
    victim->class = CLASS_VAMPIRE;
    victim->beast = 30;
    set_learnable_disciplines(victim);
    send_to_char("You are now a vampire!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "samurai" ) ) 
    {
    victim->pcdata->powers[HPOWER_WPNSKILL] = 1;
    victim->class = 16;
    send_to_char("You are now a Samurai!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "monk"      ) )
    {
     victim->class = CLASS_MONK;
     send_to_char("You are now a monk!\n\r", victim);
    }
     else if ( !str_cmp( arg2, "drow"       ) ) 
    {
    victim->class = 32;
    send_to_char("You are now a drow!\n\r", victim);
    }  
   else if ( !str_cmp( arg2, "ninja"       ) )
    {
    victim->class = 128;
    send_to_char("You are now a ninja!\n\r", victim);
    }
   else if ( !str_cmp( arg2, "paladin"       ) )
    {
    victim->class = CLASS_PALADIN;
    send_to_char("You are now a Knight Templar!\n\r", victim);
    }
   else if ( !str_cmp( arg2, "psionist"       ) )
    {
    victim->class = CLASS_PSIONIST;
    send_to_char("You are now a Psionist!\n\r", victim);
    }
    else if (!str_cmp( arg2, "inquisitor" ))
	{
	victim->class = CLASS_INQUISITOR;
	stc("You are now an inquisitor!\n\r",victim);
	victim->alignment = -1000;
	}
    else if (!str_cmp( arg2, "necromancer" ))
        {
	victim->class = CLASS_NECROMANCER;
	stc("You are now a necromancer!\n\r",victim);
	victim->alignment = -1000;
	}

   else if ( !str_cmp( arg2, "none") )
    {
      victim->class=0;
      send_to_char("Your class has been removed.\n\r",victim);
    }
   else
    {
      send_to_char( "Syntax: class <char> <class>.\n\r", ch );
        send_to_char("#rClasses:#n\n\r", ch );
  send_to_char("None, Demon, Drow, Werewolf, Monk, Vampire, Mage, Ninja.\n\r", ch);
  send_to_char("Tanarri, Shapeshifter, Spiderdroid, Samurai, Undeadknight, Angel, Lich, Hunter.\n\r",ch);
	return;
    }
    send_to_char("Class Set.\n\r",ch);
    return;
}

void do_generation( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int gen;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
        
    if ( IS_NPC(ch) ) return;
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: generation <char> <generation>.\n\r", ch );
    send_to_char("Generation 1 is a Master <Class> and 2 is clan leader.\n\r", ch);
	return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    
    gen = is_number( arg2 ) ? atoi( arg2 ) : -1;

    send_to_char("Generation Set.\n\r",ch);
    victim->generation = gen;
    return;
}


void do_clan( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char lord[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if( !str_cmp( ch->clan, "(null)" ) )
    {
	stc( "Huh?\n\r", ch);
	return;
    }
    if (ch->generation < 3)
      strcpy(lord,ch->clan);
    else strcpy(lord,ch->clan);
    
    sprintf( buf, "The members of the clan %s:\n\r", lord );
    send_to_char( buf, ch );
    send_to_char("#2[       Name       ] [ Hits ] [ Mana ] [ Move ] [  Exp  ]#n\n\r",ch);
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
        if ( IS_NPC(gch) ) continue;
/*        if ( !IS_CLASS(gch, CLASS_HIGHLANDER))
            continue;*/
        if ( !str_cmp(gch->clan,lord) || !str_cmp(gch->name,lord) || (str_cmp(gch->clan, "(null)") && ch->generation < 3))
        {
            sprintf( buf,
            "#2[#n%-18s#2] [#n%-6d#2] [#n%-6d#2] [#n%-6d#2] [#n%2.0f#2]#n\n\r",
                capitalize( gch->name ),
                gch->hit,gch->mana,gch->move, gch->exp);
                send_to_char( buf, ch );
        }
    }
    return;
}


void reset_weapon( CHAR_DATA *ch, int dtype ) {
	if ( ch->wpn[dtype] > 200 )
		ch->wpn[dtype] = 200;
	return;
}

void reset_spell( CHAR_DATA *ch, int dtype )
{
	if ( ch->spl[dtype] > 200 )
		ch->spl[dtype] = 200;
	return;
}


void do_stake( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *stake;
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    

	return;

    argument = one_argument( argument, arg );
    if (IS_NPC( ch )) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stake whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    stake = get_eq_char( ch, WEAR_HOLD );
    if ( stake == NULL || stake->item_type != ITEM_STAKE )
    {
	stake = get_eq_char( ch, WEAR_WIELD );
	if ( stake == NULL || stake->item_type != ITEM_STAKE )
	{
	    send_to_char( "How can you stake someone down without holding a stake?\n\r", ch );
	    return;
	}
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot stake yourself.\n\r", ch );
	return;
    }

    if (!IS_CLASS(victim, CLASS_VAMPIRE))
    {
	send_to_char( "You can only stake vampires.\n\r", ch );
	return;
    }

    if (victim->position > POS_MORTAL)
    {
	send_to_char( "You can only stake down a vampire who is mortally wounded.\n\r", ch );
	return;
    }

    act("You plunge $p into $N's heart.", ch, stake, victim, TO_CHAR, FALSE);
    act("$n plunges $p into $N's heart.", ch, stake, victim, TO_NOTVICT, FALSE);
    send_to_char( "You feel a stake plunged through your heart.\n\r", victim );
    if (IS_IMMUNE(victim,IMM_STAKE)) return;

    /* Have to make sure they have enough blood to change back */
    blood = victim->pcdata->condition[COND_THIRST];
    victim->pcdata->condition[COND_THIRST] = 666;

    /* To take care of vampires who have powers in affect. */
    if (IS_VAMPAFF(victim,VAM_DISGUISED) ) do_mask(victim,"self");
    if (IS_IMMUNE(victim,IMM_SHIELDED) ) do_shield(victim,"");
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) ) do_shadowplane(victim,"");
    if (IS_VAMPAFF(victim,VAM_FANGS) ) do_fangs(victim,"");
    if (IS_VAMPAFF(victim,VAM_CLAWS) ) do_claws(victim,"");
    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) ) do_nightsight(victim,"");
    if (IS_AFFECTED(victim,AFF_SHADOWSIGHT) ) do_shadowsight(victim,"");
    if (xIS_SET(victim->act,PLR_HOLYLIGHT) ) do_truesight(victim,"");
    if (IS_VAMPAFF(victim,VAM_CHANGED) ) do_change(victim,"human");
    if (IS_POLYAFF(victim,POLY_SERPENT) ) do_serpent(victim,"");
    victim->rage = 0;
    victim->pcdata->condition[COND_THIRST] = blood;

    REMOVE_BIT(victim->class, CLASS_VAMPIRE);
    obj_from_char(stake);
    obj_to_char(stake,victim);
    ch->exp = ch->exp + 1000;
    victim->home = 3001;
    return;
}

void do_mask( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    if (ch->power[DISC_VAMP_OBFU] < 2 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
        send_to_char("You must obtain at least level 2 in Obfuscate to use Mask.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && ch->pcdata->powers[SHAPE_POWERS] < 4)
    {
      send_to_char("You need level 4 in general shapeshifting powers.\n\r",ch);
      return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Change to look like whom?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
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

    if ( IS_IMMORTAL(victim) && victim != ch )
    {
	send_to_char( "You can only mask avatars or lower.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 40 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE)) ch->pcdata->condition[COND_THIRST] -= number_range(30,40);

    if ( ch == victim )
    {
	if (!IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_VAMPAFF(ch,VAM_DISGUISED))
	{
	    send_to_char( "You already look like yourself!\n\r", ch );
	    return;
	}
	sprintf(buf,"Your form shimmers and transforms into %s.",ch->name);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
	sprintf(buf,"%s's form shimmers and transforms into %s.",ch->morph,ch->name);
	act(buf,ch,NULL,victim,TO_ROOM, FALSE);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    if (IS_VAMPAFF(ch,VAM_DISGUISED))
    {
    	sprintf(buf,"Your form shimmers and transforms into a clone of %s.",victim->name);
    	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    	sprintf(buf,"%s's form shimmers and transforms into a clone of %s.",ch->morph,victim->name);
    	act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
    	sprintf(buf,"%s's form shimmers and transforms into a clone of you!",ch->morph);
    	act(buf,ch,NULL,victim,TO_VICT, FALSE);
    	free_string( ch->morph );
    	ch->morph = str_dup( victim->name );
	return;
    }
    sprintf(buf,"Your form shimmers and transforms into a clone of %s.",victim->name);
    act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    sprintf(buf,"%s's form shimmers and transforms into a clone of %s.",ch->name,victim->name);
    act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
    sprintf(buf,"%s's form shimmers and transforms into a clone of you!",ch->name);
    act(buf,ch,NULL,victim,TO_VICT, FALSE);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string( ch->morph );
    ch->morph = str_dup( victim->name );
    return;
}

void do_change( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_PROT] < 3  )
    {
        send_to_char("You must obtain at least level 3 in Protean to use Minor Change.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "You can change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r", ch );
	return;
    }


    if ( !str_cmp(arg,"bat") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r", ch );
	    return;
	}
    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
    clear_stats(ch);
	act( "You transform into bat form.", ch, NULL, NULL, TO_CHAR, FALSE);
	act( "$n transforms into a bat.", ch, NULL, NULL, TO_ROOM, FALSE );
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_BAT);*/
	SET_BIT(ch->polyaff, POLY_BAT);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	sprintf(buf, "%s the vampire bat", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"wolf") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r", ch );
	    return;
	}
    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act( "You transform into wolf form.", ch, NULL, NULL, TO_CHAR, FALSE);
	act( "$n transforms into a dire wolf.", ch, NULL, NULL, TO_ROOM, FALSE );
    clear_stats(ch);
    	if (ch->wpn[0] > 0)
    	{
	    ch->hitroll += (ch->wpn[0]);
	    ch->damroll += (ch->wpn[0]);
	    ch->armor   -= (ch->wpn[0] * .5);
    	}
    	ch->pcdata->mod_str = 10;
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);*/
	SET_BIT(ch->polyaff, POLY_WOLF);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	sprintf(buf, "%s the dire wolf", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"mist") )
    {
	if (IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You can only polymorph from human form.\n\r",
ch );
	    return;
	}

	if (has_timer(ch)) return;

    	if ( ch->pcdata->condition[COND_THIRST] < 50 )
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}

	if (ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act( "You transform into mist form.", ch, NULL, NULL, TO_CHAR, FALSE);
	act( "$n transforms into a white mist.", ch, NULL, NULL, TO_ROOM, FALSE );
	if (IS_EXTRA(ch, TIED_UP))
	{
	    act("The ropes binding you fall through your ethereal form.",ch,NULL,NULL,TO_CHAR, FALSE);
	    act("The ropes binding $n fall through $s ethereal form.",ch,NULL,NULL,TO_ROOM, FALSE);
	    xREMOVE_BIT(ch->extra, TIED_UP);
	    xREMOVE_BIT(ch->extra, GAGGED);
	    xREMOVE_BIT(ch->extra, BLINDFOLDED);
	}
	if (is_affected(ch, gsn_web))
	{
	    act("The webbing entrapping $n falls through $s ethereal form.",ch,NULL,NULL,TO_ROOM, FALSE);
	    send_to_char("The webbing entrapping you falls through your ethereal form.\n\r",ch);
	    affect_strip(ch, gsn_web);
	}
    clear_stats(ch);
/*	SET_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_MIST);*/
	SET_BIT(ch->polyaff, POLY_MIST);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
	SET_BIT(ch->affected_by, AFF_POLYMORPH);
	SET_BIT(ch->affected_by, AFF_ETHEREAL);
	sprintf(buf, "%s the white mist", ch->name);
    	free_string( ch->morph );
    	ch->morph = str_dup( buf );
	return;
    }
    else if ( !str_cmp(arg,"human") )
    {
       if (!IS_AFFECTED(ch, AFF_POLYMORPH))
	{
	    send_to_char( "You are already in human form.\n\r", ch );
	    return;
	}
	if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_BAT))
	{
	    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
	    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
	    REMOVE_BIT(ch->polyaff, POLY_BAT);
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_BAT);*/
	}
	else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WOLF))
	{
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);*/
	    REMOVE_BIT(ch->polyaff, POLY_WOLF);
	    if (ch->hit < 1) ch->hit = 1;
	}
	else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_MIST))
	{
/*	    REMOVE_BIT(ch->pcdata->powers[WOLF_POLYAFF], POLY_MIST);*/
	    REMOVE_BIT(ch->polyaff, POLY_MIST);
	    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
	}
	else
	{
	    /* In case they try to change to human from a non-vamp form */
	    send_to_char( "You seem to be stuck in this form.\n\r", ch );
	    return;
	}
	act( "You transform into human form.", ch, NULL, NULL, TO_CHAR, FALSE);
	act( "$n transforms into human form.", ch, NULL, NULL, TO_ROOM, FALSE );
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
    clear_stats(ch);
  	free_string( ch->morph );
    	ch->morph = str_dup( "" );
	return;
    }
    else
	send_to_char( "You can change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r", ch );
    return;
}

void do_clandisc( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        do_stat(ch,"");
        return;
    }
    if (arg2[0] == '\0')
    {
        if (!str_cmp(arg1,"animalism"))
	{
            send_to_char("Animalism: The discipline of nature control.\n\r",ch);
            if (ch->power[DISC_VAMP_ANIM] < 1) 
                send_to_char("You have none of the Animalism powers.\n\r",ch);
            if (ch->power[DISC_VAMP_ANIM] > 0) 
                send_to_char("Serenity: You can sooth the most aggressive soul (Golconda Required).\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"auspex"))
	{
            send_to_char("Auspex: The discipline of extrasensory awareness.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] < 1) 
                send_to_char("You have none of the Auspex discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] > 0) 
                send_to_char("Truesight: The user can see everything, including invis.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] > 1) 
                send_to_char("Scry: Allows you to view the room the specified target is in.\n\r",ch);
            if (ch->power[DISC_VAMP_AUSP] > 2) 
                send_to_char("Readaura: Gives you detained information about a creature or object.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"celerity"))
	{
            send_to_char("Celerity: The discipline of superior speed.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"chimerstry"))
        {
            send_to_char("chimerstry: The discipline of illusion.\n\r",ch);
            return;
        }
	else if (!str_cmp(arg1,"daimoinon"))
	{
            send_to_char("Daimoinon: The discipline of dark corruption.\n\r",ch);
            if (ch->power[DISC_VAMP_DAIM] < 1) 
                send_to_char("You have none of the Daimoinon discipline powers.\n\r",ch);
	    return;
        }

        else if (!str_cmp(arg1,"dominate"))
	{
            send_to_char("Dominate: The discipline of control.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] < 1) 
                send_to_char("You have none of the Dominate discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] > 0) 
                send_to_char("Command: You can plant suggestions in the mind of others.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] > 1) 
                send_to_char("Shield: You are able to shield your mind from scrying and aura-reading.\n\r",ch);
            if (ch->power[DISC_VAMP_DOMI] > 3) 
                send_to_char("Possession: You can take control of mobs by feeding them blood.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"fortitude"))
	{
            send_to_char("Fortitude: The discipline of supernatural toughness and vigor.\n\r",ch);
            if (ch->power[DISC_VAMP_FORT] < 1) 
		send_to_char("You have no fortitude.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"obfuscate"))
	{
            send_to_char("Obfuscate: The discipline of stealth.\n\r",ch);
            if (ch->power[DISC_VAMP_OBFU] < 1) 
                send_to_char("You have none of the Obfuscate discipline powers.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"obtenebration"))
	{
            send_to_char("Obtenebration: The discipline of darkness manipulation.\n\r",ch);
            if (ch->power[DISC_VAMP_OBTE] < 1) 
                send_to_char("You have none of the Obtenebration discipline powers.\n\r",ch);
	    return;
	}
        else if (!str_cmp(arg1,"potence"))
	{
            send_to_char("Potence: The discipline of superior strength.\n\r",ch);
            return;
	}
        else if (!str_cmp(arg1,"presence"))
	{
            send_to_char("Presence: The discipline of appeal and attractiveness.\n\r",ch);
            if (ch->power[DISC_VAMP_PRES] < 1) 
                send_to_char("You have none of the Presence discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"protean"))
	{
            send_to_char("Protean: The discipline of neutral transformation.\n\r",ch);
            if (ch->power[DISC_VAMP_PROT] < 1) 
                send_to_char("You have none of the Protean discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"quietus"))
	{
            send_to_char("Quietus: The discipline of death.\n\r",ch);
            if (ch->power[DISC_VAMP_QUIE] < 1) 
                send_to_char("You have none of the Quietus discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_QUIE] > 3)
                send_to_char("Spit: You can project your deadly spit at a foe\n\r",ch); 
           return;
        }
        else if (!str_cmp(arg1,"serpentis"))
	{
            send_to_char("Serpentis: The discipline of temptation and corruption.\n\r",ch);
            if (ch->power[DISC_VAMP_SERP] < 1) 
                send_to_char("You have none of the Serpentis discipline powers.\n\r",ch);
	    return;
        }
        else if (!str_cmp(arg1,"thaumaturgy"))
	{
            send_to_char("Thaumaturgy: The discipline of blood magic.\n\r",ch);
            if (ch->power[DISC_VAMP_THAU] < 1) 
                send_to_char("You have none of the Thaumaturgy discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_THAU] > 0) 
                send_to_char("Theft: You can take from the blood pool of other players.\n\r",ch);

            return;
        }
        else if (!str_cmp(arg1,"vicissitude"))
	{
            send_to_char("Vicissitude: The discipline of evil body control.\n\r",ch);
            if (ch->power[DISC_VAMP_VICI] < 1) 
                send_to_char("You have none of the Vicissitude discipline powers.\n\r",ch);
            if (ch->power[DISC_VAMP_VICI] > 3) 
                send_to_char("Zuloform: Transformation into a powerful being.\n\r",ch);

            return;
        }
        do_stat(ch,"");
        return;

    }
    if (!str_cmp(arg2,"improve"))
    {
	int improve;
	int cost;
	int max;
        int maxlevel;

	stc("Please use the new commands, Disciplines and research.\n\r",ch);
	return;

          maxlevel=10;
             if (!str_cmp(arg1,"animalism"       )) {improve = DISC_VAMP_ANIM;          max=maxlevel;}
        else if (!str_cmp(arg1,"auspex"          )) {improve = DISC_VAMP_AUSP;          max=maxlevel;}
        else if (!str_cmp(arg1,"celerity"        )) {improve = DISC_VAMP_CELE;        max=maxlevel;}
        else if (!str_cmp(arg1,"chimerstry"      )) {improve = DISC_VAMP_CHIM; max=maxlevel;}
	else if (!str_cmp(arg1,"dominate"        )) {improve = DISC_VAMP_DOMI;        max=maxlevel;}
        else if (!str_cmp(arg1,"daimoinon"       )) {improve = DISC_VAMP_DAIM;max=maxlevel;}
        else if (!str_cmp(arg1,"fortitude"       )) {improve = DISC_VAMP_FORT;       max=12;}
        else if (!str_cmp(arg1,"obfuscate"       )) {improve = DISC_VAMP_OBFU;       max=maxlevel;}
        else if (!str_cmp(arg1,"obtenebration"   )) {improve = DISC_VAMP_OBTE;   max=maxlevel;}
        else if (!str_cmp(arg1,"potence"         )) {improve = DISC_VAMP_POTE;         max=maxlevel;}
        else if (!str_cmp(arg1,"presence"        )) {improve = DISC_VAMP_PRES;        max=maxlevel;}
        else if (!str_cmp(arg1,"protean"         )) {improve = DISC_VAMP_PROT;         max=maxlevel;}
        else if (!str_cmp(arg1,"quietus"         )) {improve = DISC_VAMP_QUIE;         max=maxlevel;}
        else if (!str_cmp(arg1,"serpentis"       )) {improve = DISC_VAMP_SERP;       max=maxlevel;}
        else if (!str_cmp(arg1,"thaumaturgy"     )) {improve = DISC_VAMP_THAU;            max=maxlevel;}
        else if (!str_cmp(arg1,"vicissitude"     )) {improve = DISC_VAMP_VICI;     max=maxlevel;}
	else if (!str_cmp(arg1,"obeah"		 )) {improve =
DISC_VAMP_OBEA; max = maxlevel; }
	else if (!str_cmp(arg1,"melpominee"	 )) {improve =
DISC_VAMP_MELP; max = maxlevel; }
	else if (!str_cmp(arg1,"necromancy"	 )) {improve =
DISC_VAMP_NECR; max = maxlevel; }
	else if (!str_cmp(arg1,"thanatosis"	 )) {improve =
DISC_VAMP_THAN; max = maxlevel; }
        else
	{
            send_to_char("You can improve: Animalism, Auspex, Celerity, Dominate, Fortitude, Obfuscate,\n\r Obtenebration, Potence, Presence, Protean, Quietus, Serpentis,\n\r Thaumaturgy or Vicissitude.\n\r",ch);
	    return;
	}
        cost = (ch->power[improve]+1) * 10;
	arg1[0] = UPPER(arg1[0]);
	if ( ch->power[improve] >= max && ch->pcdata->rank ==AGE_METHUSELAH)
	{
            sprintf(buf,"You have already gained all the powers of the %s discipline.\n\r", arg1);
	    send_to_char(buf,ch);
	    return;
	}
        if (ch->power[improve] >= max)
        {
           sprintf(buf,"You have already maxed %s.\n\r",arg1);
           send_to_char(buf,ch);
           return;
        }
	if ( cost > ch->practice )
	{
            sprintf(buf,"It costs you %d primal to improve your %s discipline.\n\r", cost, arg1);
	    send_to_char(buf,ch);
	    return;
	}
	ch->power[improve] += 1;
	ch->practice -= cost;
        sprintf(buf,"You improve your ability in the %s discipline.\n\r", arg1);
	send_to_char(buf,ch);
    }
    else send_to_char("To improve a discipline, type: Clandisc <discipline type> improve.\n\r",ch);
    return;
}


void do_vampire( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_IMMUNE(ch, IMM_VAMPIRE))
    {
	send_to_char("You will now allow vampires to bite you.\n\r",ch);
	SET_BIT(ch->immune, IMM_VAMPIRE);
	return;
    }
    send_to_char("You will no longer allow vampires to bite you.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_VAMPIRE);
    return;
}

void do_shadowplane( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (( ch->in_room->vnum >= 420000) && (ch->in_room->vnum <= 420099))
    {
        send_to_char( "You cannot do this in this area.\n\r",ch);
        return;
    }
     
   if (ch->mana < 1000) 
        {
        send_to_char("You don't have enough mana to shadowplane.\n\r",ch );
        return;
        }

    if (!IS_CLASS(ch, CLASS_WEREWOLF) || ch->power[DISC_WERE_OWL] < 3)
    {
	if (!IS_CLASS(ch, CLASS_VAMPIRE))
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
        if (ch->power[DISC_VAMP_OBTE] < 3)
	{
            send_to_char("You must obtain at least level 3 in Obtenebration to use Shadowplane .\n\r",ch);
	    return;
	}
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 75)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    
    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(65,75);

    if ( arg[0] == '\0' )
    {
    	if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	{
            ch->mana -= 1000;
            if (has_timer(ch)) return;
	    send_to_char("You fade into the plane of shadows.\n\r",ch);
	    act("The shadows flicker and swallow up $n.",ch,NULL,NULL,TO_ROOM, FALSE);
	    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    do_look(ch,"auto");
	    return;
    	}
    	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    	send_to_char("You fade back into the real world.\n\r",ch);
	act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM, FALSE);
	do_look(ch,"auto");
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char( "What do you wish to toss into the shadow plane?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
    	send_to_char( "You toss it to the ground and it vanishes.\n\r", ch );
    else
    	send_to_char( "You toss it into a shadow and it vanishes.\n\r", ch );
    return;
}


void do_darkheart( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_SERP] < 1  )
    {
        send_to_char("You must obtain at least level 1 in Serpentis to use Darkheart.\n\r",ch);
	return;
    }
    if (IS_IMMUNE(ch,IMM_STAKE) )
    {
	send_to_char("But you've already torn your heart out!\n\r",ch);
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 1000 / (10 - ch->generation) )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= 100;
    send_to_char("You rip your heart from your body and toss it to the ground.\n\r",ch);
    act("$n rips $s heart out and tosses it to the ground.", ch, NULL, NULL, TO_ROOM, FALSE);
    make_part( ch, "heart" );
    ch->hit = ch->hit - number_range(10,20);
    update_pos(ch);
    if (ch->position == POS_DEAD && !IS_HERO(ch))
    {
	send_to_char( "You have been KILLED!!\n\r\n\r", ch );
	raw_kill(ch);
	return;
    }
    SET_BIT(ch->immune, IMM_STAKE);
    return;
}

void do_truesight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

         if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_PSIONIST) &&
	!IS_CLASS(ch, CLASS_HIGHLANDER) && !IS_CLASS(ch, CLASS_POWER_LICH) &&
	!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_TANARRI) &&
	!IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_SHAPESHIFTER) &&
	!IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) &&
	!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_SHAPESHIFTER) && 
	!IS_CLASS(ch, CLASS_PALADIN) && !IS_CLASS(ch, CLASS_INQUISITOR) )
	 {
	     send_to_char("Huh?\n\r",ch);
	     return;
         }
  if (IS_CLASS(ch, CLASS_TANARRI) && !IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_TRUESIGHT))
  {
    send_to_char("You don't have that power yet\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_PSIONIST) && !IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_TRUESIGHT))
  {
    send_to_char("You don't have that power yet\n\r",ch);
    return;
  }

 if ((ch->power[DISC_VAMP_AUSP] < 1) && IS_CLASS(ch, CLASS_VAMPIRE))
	{
	send_to_char("You must obtain at least level 1 in Auspex to use Truesight.\n\r", ch);
        return;
        }

  if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && ch->pcdata->powers[SHAPE_POWERS] < 1)
       {
        send_to_char("You must obtain at least level 1 in shiftpowers to use Truesight.\n\r", ch);
        return;
       }


    if ((ch->power[DISC_WERE_HAWK] < 3) && IS_CLASS(ch,CLASS_WEREWOLF))
        {
	send_to_char("You must obtain level 3 in the Hawk totem to use Truesight.\n\r",ch);
	return;
	}
    
	if (!IS_DEMPOWER( ch, DEM_TRUESIGHT) && IS_CLASS(ch,CLASS_DEMON))
    {
        stc("You must inpart truesight first.\n\r",ch);
        return;
    }

   if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[AWARE] < 3 )
    {
	stc("You must obtain level 4 in Awareness first.\n\r",ch);
	return;
    }
   
    if ( xIS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	xREMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses return to normal.\n\r", ch );
    }
    else
    {
	xSET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses increase to incredible proportions.\n\r", ch );
    }

    return;
}

void do_majesty( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_PRES] < 5  )
    {
        send_to_char("You must obtain at least level 5 in Presence to use Majesty.\n\r",ch);
	return;
    }
    if (( ch->in_room->vnum >= 420000) && (ch->in_room->vnum <= 420099))
    {
        send_to_char( "You cannot do this in this area.\n\r",ch);
        return;
    }

    if (has_timer(ch)) return;

    if (IS_AFFECTED(ch, AFF_PEACE))
    {
        REMOVE_BIT(ch->affected_by, AFF_PEACE);
        act( "You lower your majesty.",  ch, NULL, NULL, TO_CHAR, FALSE    );
        act( "$n looks less imposing.",  ch, NULL, NULL, TO_NOTVICT, FALSE );
        return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 45 )
    {
	send_to_char( "You have insufficient blood.\n\r", ch );
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= number_range(35,45);
    SET_BIT(ch->affected_by, AFF_PEACE);
    act( "You raise your majesty.",  ch, NULL, NULL, TO_CHAR, FALSE    );
    act( "$n suddenly looks very imposing.",  ch, NULL, NULL, TO_NOTVICT, FALSE);
    return;
}

void do_scry( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];
    char buf[MSL];
    int rand_room;
    bool chaosvalue = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_ITEMAFF(ch, ITEMA_VISION))
    {
	if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch, CLASS_PSIONIST)
	  && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_ANGEL) && !IS_CLASS(ch, CLASS_INQUISITOR) 
     && !IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch,CLASS_CYBORG) && !IS_CLASS(ch,CLASS_POWER_LICH) )
	{
	    stc("Huh?\n\r",ch);
	    return;
	}
    }

  if ( !IS_ITEMAFF(ch, ITEMA_VISION) )
  {
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_AUSP] < 2 )
    {
        send_to_char("You must obtain at least level 2 Auspex to use Scry.\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 3)
    {
	send_to_char("You need the trio mantra to use scry.\n\r",ch);
	return;
    }
  if (IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] < 4)
    {
	stc("You are not skilled enough to inquire.\n\r",ch);
	return;
    }
  if( IS_CLASS(ch, CLASS_MAGE) && ch->pcdata->powers[PINVOKE] < 3 )
	{
	stc( "You don't have the power to scry yet.\n\r", ch);
	return;
	}
  if( IS_CLASS(ch, CLASS_POWER_LICH) && ch->pcdata->powers[DEATH_LORE] < 2)
	{
	stc( "You don't have the power to scry yet.\n\r",ch);
	return;
	}	
	if (IS_CLASS(ch,CLASS_CYBORG) &&
	ch->pcdata->powers[CYBORG_FACE] < 5)
	{
	stc("You must have full facial implants to scry.\n\r",ch);
	return;
	}

    if (IS_CLASS(ch, CLASS_DEMON) && !IS_DEMPOWER(ch, DEM_SCRY) )
    {
	stc("You have not been granted the power of Scry.\n\r",ch);
	return;
    }
  }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (victim->trust > 6)
    {
      send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
      send_to_char("Someone is scrying you.\n\r",victim);
      return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim,IMM_SHIELDED))
    {
       send_to_char( "You cannot find them.\n\r",ch);
       return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_SHIELD2)) chaosvalue = TRUE;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 25 
	&& !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    if (!IS_ITEMAFF(ch, ITEMA_VISION) && IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(15,25);

    chroom = ch->in_room;
    victimroom = victim->in_room;
    if (chaosvalue)
      for ( ; ; ) 
      {
        rand_room = number_range( 0, 65535 );
        victimroom = get_room_index( rand_room );
        if (victimroom != NULL) break;   
      }

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
	do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    if (!IS_NPC(victim)) ch->fight_timer += 5;
    
    if ( IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->flag2, AFF_SPIRITGUARD))
    {
      act("Your spirit guardian sends you a message:",ch,NULL,victim,TO_VICT, FALSE);
      act("\"$n is scrying you.\"",ch,NULL,victim,TO_VICT, FALSE);
    }
    if (IS_SET(victim->immune, IMM_SCRY))
    {
    sprintf(buf, "%s is looking in on you.\n\r",ch->name);
    stc(buf, victim);
    }

    return;
}

void do_readaura( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    bool chaosvalue = FALSE;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_ITEMAFF(ch, ITEMA_VISION)
	&& !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch,CLASS_CYBORG) && !IS_CLASS(ch, CLASS_POWER_LICH))
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_AUSP] < 3 && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
        send_to_char("You must obtain at least level 3 in Auspex to use Readaura.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_POWER_LICH) && ch->pcdata->powers[DEATH_LORE] < 1)
    {
    send_to_char("You don't have that power yet.\n\r",ch);
    return;
    }

    if (IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 2)
    {
	send_to_char("You must obtain the level 2 Mantra first!\n\r",ch);
	return;
    }

    if (IS_CLASS(ch,CLASS_CYBORG) && ch->pcdata->powers[CYBORG_FACE] < 5)
    {
	stc("You must have full face implants to readaura.\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Read the aura on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "Read the aura on what?\n\r", ch );
	    return;
	}
    	if ( ch->pcdata->condition[COND_THIRST] < 50 && !IS_ITEMAFF(ch, ITEMA_VISION)
	     && !IS_CLASS(ch, CLASS_MONK) && !IS_CLASS(ch,CLASS_CYBORG) && !IS_CLASS(ch, CLASS_POWER_LICH))
    	{
	    send_to_char("You have insufficient blood.\n\r",ch);
	    return;
    	}
	if (!IS_ITEMAFF(ch, ITEMA_VISION) && !IS_CLASS(ch, CLASS_MONK))
	    ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
	act("$n examines $p intently.",ch,obj,NULL,TO_ROOM, FALSE);
	spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	return;
    }

    if (ch->pcdata->condition[COND_THIRST] < 50 &&
     !IS_ITEMAFF(ch,ITEMA_VISION) && !IS_CLASS(ch, CLASS_MONK) &&
     !IS_CLASS(ch, CLASS_CYBORG) && !IS_CLASS(ch, CLASS_POWER_LICH))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (!IS_ITEMAFF(ch, ITEMA_VISION))
	ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
(IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("You are unable to read their aura.\n\r",ch);
	return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_SHIELD2))
      chaosvalue = TRUE;

    act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT, FALSE);
    act("$n examines you intently.",ch,NULL,victim,TO_VICT, FALSE);
    if (IS_NPC(victim)) sprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
    else 
    {
	if      (victim->level == 12) sprintf(buf, "%s is an Implementor.\n\r", victim->name);
	else if (victim->level == 11) sprintf(buf, "%s is a High Judge.\n\r", victim->name);
	else if (victim->level == 10) sprintf(buf, "%s is a Judge.\n\r", victim->name);
	else if (victim->level == 9 ) sprintf(buf, "%s is an Enforcer.\n\r", victim->name);
	else if (victim->level == 8 ) sprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
	else if (victim->level == 7 ) sprintf(buf, "%s is a Builder.\n\r", victim->name);
	else if (victim->level >= 3 ) sprintf(buf, "%s is an Avatar.\n\r", victim->name);
	else sprintf(buf, "%s is a Mortal.\n\r", victim->name);
    }
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
	send_to_char(buf,ch);
    }
    if (chaosvalue) sprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r", number_range(1000,5000), number_range(10000,20000) , number_range(1000,5000),
number_range(10000,20000), number_range(1000,5000), number_range(10000,20000));
    else
      sprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
      if (chaosvalue) sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r", number_range(500,2500), number_range(500,2500), number_range(-2000,0));
      else
        sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
    }
    else sprintf(buf,"AC:%d.\n\r",char_ac(victim));
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Status:%d, ",victim->race);
	send_to_char(buf,ch);
	if (IS_CLASS(victim, CLASS_VAMPIRE))    
	{
	    sprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
	    send_to_char(buf,ch);
	}
    }
    sprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("$N is pregnant.",ch,NULL,victim,TO_CHAR, FALSE);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))    
    {
	send_to_char("Disciplines:",ch);
	if (IS_VAMPAFF(victim, VAM_PROTEAN)) send_to_char(" Protean",ch);
	if (IS_VAMPAFF(victim, VAM_CELERITY)) send_to_char(" Celerity",ch);
	if (IS_VAMPAFF(victim, VAM_FORTITUDE)) send_to_char(" Fortitude",ch);
	if (IS_VAMPAFF(victim, VAM_POTENCE)) send_to_char(" Potence",ch);
	if (IS_VAMPAFF(victim, VAM_OBFUSCATE)) send_to_char(" Obfuscate",ch);
	if (IS_VAMPAFF(victim, VAM_OBTENEBRATION)) send_to_char(" Obtenebration",ch);
	if (IS_VAMPAFF(victim, VAM_SERPENTIS)) send_to_char(" Serpentis",ch);
	if (IS_VAMPAFF(victim, VAM_AUSPEX)) send_to_char(" Auspex",ch);
	if (IS_VAMPAFF(victim, VAM_DOMINATE)) send_to_char(" Dominate",ch);
	if (IS_VAMPAFF(victim, VAM_PRESENCE)) send_to_char(" Presence",ch);
        if (IS_VAMPAFF(victim, VAM_VICISSITUDE)) send_to_char(" Vicissitude",ch);
        if (IS_VAMPAFF(victim, VAM_THAU)) send_to_char(" Thaumaturgy",ch);
        if (IS_VAMPAFF(victim, VAM_ANIMAL)) send_to_char(" Animalism",ch);
        if (IS_VAMPAFF(victim, VAM_QUIETUS)) send_to_char(" Quietus",ch);
        send_to_char(".\n\r",ch);
    }
    return;
}

void do_mortal( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_VAMPAFF(ch, VAM_MORTAL))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (ch->power[DISC_VAMP_OBFU] < 4)
    {
        send_to_char("You must obtain at least level 4 in Obfuscate to use Mortal Facade.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch,CLASS_VAMPIRE) )
    {
        if ( ch->pcdata->condition[COND_THIRST] < 1000 / (10 - ch->generation) )
        {
	    send_to_char("You must be at full blood to use this power.\n\r",ch);
	    return;
    	}
    	/* Have to make sure they have enough blood to change back */
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;

	/* Remove physical vampire attributes when you take mortal form */
    	if (IS_VAMPAFF(ch,VAM_DISGUISED) ) do_mask(ch,"self");
    	if (IS_IMMUNE(ch,IMM_SHIELDED) ) do_shield(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) ) do_shadowplane(ch,"");
    	if (IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) ) do_shadowsight(ch,"");
    	if (xIS_SET(ch->act,PLR_HOLYLIGHT) ) do_truesight(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CHANGED) ) do_change(ch,"human");
    	if (IS_POLYAFF(ch,POLY_SERPENT) ) do_serpent(ch,"");
	ch->rage = 0;
    	ch->pcdata->condition[COND_THIRST] = blood;

    	send_to_char("Colour returns to your skin and you warm up a little.\n\r",ch);
    	act("Colour returns to $n's skin.", ch, NULL, NULL, TO_ROOM, FALSE);

    	REMOVE_BIT(ch->class, CLASS_VAMPIRE);
    	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
	return;
    }
    send_to_char("You skin pales and cools.\n\r",ch);
    act("$n's skin pales slightly.", ch, NULL, NULL, TO_ROOM, FALSE);
    SET_BIT(ch->class, CLASS_VAMPIRE);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);
    return;
}

void do_mortalvamp( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    int       blood;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (IS_CLASS(ch,CLASS_VAMPIRE) )
    {
    	/* Have to make sure they have enough blood to change back */
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;

	/* Remove physical vampire attributes when you take mortal form */
    	if (IS_VAMPAFF(ch,VAM_DISGUISED) ) do_mask(ch,"self");
    	if (IS_IMMUNE(ch,IMM_SHIELDED) ) do_shield(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) ) do_shadowplane(ch,"");
    	if (IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) ) do_shadowsight(ch,"");
    	if (xIS_SET(ch->act,PLR_HOLYLIGHT) ) do_truesight(ch,"");
    	if (IS_VAMPAFF(ch,VAM_CHANGED) ) do_change(ch,"human");
    	if (IS_POLYAFF(ch,POLY_SERPENT) ) do_serpent(ch,"");
	ch->rage = 0;
    	ch->pcdata->condition[COND_THIRST] = blood;

    	send_to_char("You loose your vampire powers.\n\r",ch);

/*    	REMOVE_BIT(ch->class, CLASS_VAMPIRE);
    	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);*/
	return;
    }
    send_to_char("You regain your vampire powers.\n\r",ch);
/*    SET_BIT(ch->class, CLASS_VAMPIRE);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_MORTAL);*/
    return;
}

void do_shield( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF) || ch->power[DISC_WERE_OWL] < 2)
    {
	if (!IS_CLASS(ch, CLASS_VAMPIRE) && !IS_CLASS(ch, CLASS_DROW)
	 && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_MONK)
	 && !IS_CLASS(ch, CLASS_CYBORG) )
	{
	    send_to_char("Huh?\n\r",ch);
	    return;
	}
    }
	if (!IS_DEMPOWER( ch, DEM_SHIELD) && IS_CLASS(ch,CLASS_DEMON))
	{
        send_to_char("You must inpart it first!\n\r",ch);
        return;
    }
	if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->pcdata->powers[1],DPOWER_DROWSHIELD))
	{  send_to_char("You don't have that power.\n\r", ch );
		return;}
    if (IS_CLASS(ch, CLASS_MONK) && !IS_NPC(ch) && ch->pcdata->powers[PMONK] < 2)
    {
	send_to_char("You must improve your Mantras to level 2 first!\n\r",ch);
	return;
    }

	if (IS_CLASS(ch,CLASS_CYBORG) && ch->pcdata->powers[CYBORG_FACE]<4)
	{
	stc("You must have 4 implants before using shield.\n\r",ch);
	return;
	}
    if (ch->power[DISC_VAMP_OBFU] < 3 && ch->power[DISC_VAMP_DOMI] < 2 && 
	!IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch,
CLASS_MONK) && !IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch,CLASS_CYBORG))
    {
        send_to_char("You must obtain at least level 3 in Obfuscate or level 2 in Dominate.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 60)
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    if (IS_CLASS(ch, CLASS_VAMPIRE))
	ch->pcdata->condition[COND_THIRST] -= number_range(50,60);
    if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
    	send_to_char("You shield your aura from those around you.\n\r",ch);
    	SET_BIT(ch->immune, IMM_SHIELDED);
	return;
    }
    send_to_char("You stop shielding your aura.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);
    return;
}

void do_serpent( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH]; 
    char buf [MAX_STRING_LENGTH]; 
    argument = one_argument( argument, arg ); 

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_SERP] < 2)
    {
        send_to_char("You must obtain at least level 2 in Serpentis to use Serpent Form.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch, AFF_POLYMORPH))
    {
	if (!IS_POLYAFF(ch,POLY_SERPENT))
	{
	    send_to_char( "You cannot polymorph from this form.\n\r", ch );
	    return;
	}
	act( "You transform back into human.", ch, NULL, NULL, TO_CHAR, FALSE);
	act( "$n transform into human form.", ch, NULL, NULL, TO_ROOM, FALSE );
    clear_stats(ch);
	REMOVE_BIT(ch->polyaff, POLY_SERPENT);
	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
	free_string( ch->morph );
	ch->morph = str_dup( "" );
	return;
    }
    if ( ch->pcdata->condition[COND_THIRST] < 50 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(40,50);
    clear_stats(ch);
    if (ch->wpn[0] > 0)
    {
    	ch->hitroll += ch->wpn[0] * 0.75;
    	ch->damroll += ch->wpn[0] * 0.75;
    	ch->armor   -= ch->wpn[0] * 3;
    }
    if (ch->stance[0] != -1) do_stance(ch,"");
    if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    ch->pcdata->mod_str = 10;
    act( "You transform into a huge serpent.", ch, NULL, NULL, TO_CHAR, FALSE);
    act( "$n transforms into a huge serpent.", ch, NULL, NULL, TO_ROOM, FALSE );
    SET_BIT(ch->polyaff, POLY_SERPENT);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    sprintf(buf, "%s the huge serpent", ch->name);
    free_string( ch->morph );
    ch->morph = str_dup( buf );
    return;
}

void do_poison( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_SERP] < 3)
    {
        send_to_char("You must obtain at least level 3 in Serpentis to use Poison.\n\r",ch);
	return;
    }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
    &&   ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL ) )
    {
	send_to_char( "You must wield the weapon you wish to poison.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 )
    {
	send_to_char( "This weapon cannot be poisoned.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 15 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }
    ch->pcdata->condition[COND_THIRST] -= number_range(5,15);
    act("You run your tongue along $p, poisoning it.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$n runs $s tongue along $p, poisoning it.",ch,obj,NULL,TO_ROOM, FALSE);
    obj->value[0] = 53;
    obj->timer = number_range(10,20);

    return;
}

void do_regenerate( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->rage > 0)
    {
	send_to_char("You cannot control your regenerative powers while the beast is so strong.\n\r",ch);
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char("You cannot regenerate while fighting.\n\r",ch);
	return;
    }

    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
	send_to_char("But you are already completely regenerated!\n\r",ch);
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 5 )
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= number_range(10,20);
    if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
    {
    	send_to_char("Your body has completely regenerated.\n\r",ch);
    	act("$n's body completely regenerates itself.",ch,NULL,NULL,TO_ROOM, FALSE);
    }
    else
    	send_to_char("Your body slowly regenerates itself.\n\r",ch);
    if (ch->hit < 1 )
    {
	ch->hit = ch->hit + 1;
	update_pos(ch);
	WAIT_STATE( ch, 24 );
    }
    else
    {
    	ch->hit = UMIN(ch->hit + 500, ch->max_hit);
    	ch->mana = UMIN(ch->mana + 500, ch->max_mana);
    	ch->move = UMIN(ch->move + 500, ch->max_move);
	WAIT_STATE(ch,8);
        update_pos(ch);
    }
    return;
}


void reg_mend( CHAR_DATA *ch )
{
    int ribs = 0;
    int teeth = 0;

    if (IS_BODY(ch,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(ch,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(ch,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(ch,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(ch,BROKEN_RIBS_16)) ribs += 16;
    if (IS_HEAD(ch,LOST_TOOTH_1  )) teeth += 1;
    if (IS_HEAD(ch,LOST_TOOTH_2  )) teeth += 2;
    if (IS_HEAD(ch,LOST_TOOTH_4  )) teeth += 4;
    if (IS_HEAD(ch,LOST_TOOTH_8  )) teeth += 8;
    if (IS_HEAD(ch,LOST_TOOTH_16 )) teeth += 16;

    if (ribs > 0)
    {
    	if (IS_BODY(ch,BROKEN_RIBS_1 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_1);
    	if (IS_BODY(ch,BROKEN_RIBS_2 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_2);
	if (IS_BODY(ch,BROKEN_RIBS_4 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_4);
	if (IS_BODY(ch,BROKEN_RIBS_8 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_8);
	if (IS_BODY(ch,BROKEN_RIBS_16))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_16);
	ribs -= 1;
	if (ribs >= 16) {ribs -= 16;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_16);}
	if (ribs >= 8 ) {ribs -= 8;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_8);}
	if (ribs >= 4 ) {ribs -= 4;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_4);}
	if (ribs >= 2 ) {ribs -= 2;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_2);}
	if (ribs >= 1 ) {ribs -= 1;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_1);}
	act("One of $n's ribs snap back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("One of your ribs snap back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
    }
    else if (IS_HEAD(ch,LOST_EYE_L))
    {
	act("An eyeball appears in $n's left eye socket.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("An eyeball appears in your left eye socket.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_L);
    }
    else if (IS_HEAD(ch,LOST_EYE_R))
    {
	act("An eyeball appears in $n's right eye socket.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("An eyeball appears in your right eye socket.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_R);
    }
    else if (IS_HEAD(ch,LOST_EAR_L))
    {
	act("An ear grows on the left side of $n's head.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("An ear grows on the left side of your head.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_L);
    }
    else if (IS_HEAD(ch,LOST_EAR_R))
    {
	act("An ear grows on the right side of $n's head.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("An ear grows on the right side of your head.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_R);
    }
    else if (IS_HEAD(ch,LOST_NOSE))
    {
	act("A nose grows on the front of $n's face.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A nose grows on the front of your face.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_NOSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (teeth > 0)
    {
    	if (IS_HEAD(ch,LOST_TOOTH_1 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);
    	if (IS_HEAD(ch,LOST_TOOTH_2 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);
    	if (IS_HEAD(ch,LOST_TOOTH_4 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);
    	if (IS_HEAD(ch,LOST_TOOTH_8 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);
    	if (IS_HEAD(ch,LOST_TOOTH_16))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);
	teeth -= 1;
	if (teeth >= 16) {teeth -= 16;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);}
	if (teeth >= 8 ) {teeth -= 8;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);}
	if (teeth >= 4 ) {teeth -= 4;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);}
	if (teeth >= 2 ) {teeth -= 2;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);}
	if (teeth >= 1 ) {teeth -= 1;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);}
	act("A missing tooth grows in your mouth.",ch,NULL,NULL,TO_CHAR, FALSE);
	act("A missing tooth grows in $n's mouth.",ch,NULL,NULL,TO_ROOM, FALSE);
    }
    else if (IS_HEAD(ch,BROKEN_NOSE))
    {
	act("$n's nose snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your nose snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (IS_HEAD(ch,BROKEN_JAW))
    {
	act("$n's jaw snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your jaw snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    else if (IS_HEAD(ch,BROKEN_SKULL))
    {
	act("$n's skull knits itself back together.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your skull knits itself back together.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_SKULL);
    }
    else if (IS_BODY(ch,BROKEN_SPINE))
    {
	act("$n's spine knits itself back together.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your spine knits itself back together.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_SPINE);
    }
    else if (IS_BODY(ch,BROKEN_NECK))
    {
	act("$n's neck snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your neck snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_NECK);
    }
    else if (IS_ARM_L(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's left shoulder.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("An arm grows from the stump of your left shoulder.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
    }
    else if (IS_ARM_R(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's right shoulder.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("An arm grows from the stump of your right shoulder.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
    }
    else if (IS_LEG_L(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's left hip.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A leg grows from the stump of your left hip.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's right hip.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A leg grows from the stump of your right hip.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,BROKEN_ARM))
    {
	act("$n's left arm snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your left arm snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
    }
    else if (IS_ARM_R(ch,BROKEN_ARM))
    {
	act("$n's right arm snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your right arm snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
    }
    else if (IS_LEG_L(ch,BROKEN_LEG))
    {
	act("$n's left leg snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your left leg snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
    }
    else if (IS_LEG_R(ch,BROKEN_LEG))
    {
	act("$n's right leg snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your right leg snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
    }
    else if (IS_ARM_L(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's left wrist.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A hand grows from the stump of your left wrist.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's right wrist.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A hand grows from the stump of your right wrist.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
    }
    else if (IS_LEG_L(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's left ankle.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A foot grows from the stump of your left ankle.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's right ankle.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A foot grows from the stump of your right ankle.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A thumb slides out of your left hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,BROKEN_THUMB))
    {
	act("$n's left thumb snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your left thumb snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("An index finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_I))
    {
	act("$n's left index finger snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your left index finger snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A middle finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_M))
    {
	act("$n's left middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your left middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A ring finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_R))
    {
	act("$n's left ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your left ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A little finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_L))
    {
	act("$n's left little finger snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your left little finger snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A thumb slides out of your right hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,BROKEN_THUMB))
    {
	act("$n's right thumb snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your right thumb snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("An index finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_I))
    {
	act("$n's right index finger snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your right index finger snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A middle finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_M))
    {
	act("$n's right middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your right middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A ring finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_R))
    {
	act("$n's right ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your right ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if ( IS_ARM_R(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("A little finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if ( IS_ARM_R(ch,BROKEN_FINGER_L))
    {
	act("$n's right little finger snaps back into place.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your right little finger snaps back into place.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if (IS_BODY(ch,CUT_THROAT))
    {
    	if (IS_SET(ch->loc_hp[6], BLEEDING_THROAT)) return;
	act("The wound in $n's throat closes up.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("The wound in your throat closes up.",ch,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],CUT_THROAT);
    }
    return;
}

void vamp_rage( CHAR_DATA *ch )
{
    if (IS_NPC(ch)) return;

    send_to_char("You scream with rage as the beast within consumes you!\n\r",ch);
    act("$n screams with rage as $s inner beast consumes $m!.", ch, NULL, NULL, TO_ROOM, FALSE);
    do_beastlike(ch,"");
    do_frenzy(ch,"");
    return;
}

void do_humanity( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC( ch )) return;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && ch->beast > 0
	&& ch->beast < 100 && number_range(1, 300) <= ch->beast)
    {
	if (ch->beast == 1)
	{
	    send_to_char("You have attained Golconda!\n\r", ch);
	    ch->exp += 1000000;
	    if (!IS_IMMUNE(ch, IMM_SUNLIGHT))
		SET_BIT(ch->immune, IMM_SUNLIGHT);
	}
	else
	    send_to_char("You feel slightly more in control of your beast.\n\r", ch);
	ch->beast -= 1;
    }
    return;
}

void do_beastlike( CHAR_DATA *ch, char *argument )
{
    char       arg [MAX_INPUT_LENGTH];
    int blood;

    argument = one_argument( argument, arg );

    if (IS_NPC( ch )) return;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && ch->beast < 100
	&& ch->beast > 0 && number_range(1, 500) <= ch->beast)
    {
	if (ch->beast < 99)
	    send_to_char("You feel your beast take more control over your actions.\n\r", ch);
	else
	    send_to_char("Your beast has fully taken over control of your actions!\n\r", ch);
	ch->beast += 1;
    	blood = ch->pcdata->condition[COND_THIRST];
    	ch->pcdata->condition[COND_THIRST] = 666;
    	if ((IS_VAMPAFF(ch,VAM_PROTEAN) || (IS_VAMPAFF(ch,VAM_OBTENEBRATION)))
		&& !IS_VAMPAFF(ch,VAM_NIGHTSIGHT) ) do_nightsight(ch,"");
    	if (!IS_VAMPAFF(ch,VAM_FANGS) ) do_fangs(ch,"");
    	if (IS_VAMPAFF(ch,VAM_PROTEAN) &&
		!IS_VAMPAFF(ch,VAM_CLAWS) ) do_claws(ch,"");
    	ch->pcdata->condition[COND_THIRST] = blood;
    }
    return;
}


void do_upkeep( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];
    char      buf2 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION))
    {

send_to_char("----------------------------------------------------------------------------\n\r",ch);
	send_to_char("                              -= Demonic powers =-\n\r",ch);

send_to_char("----------------------------------------------------------------------------\n\r",ch);

	if (ch->pcdata->powers[DPOWER_FLAGS] < 1)
	    send_to_char("You have no demonic powers.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_FANGS) )
	{
	    if (IS_VAMPAFF(ch, VAM_FANGS)) send_to_char("You have a pair of long pointed fangs extending from your gums.\n\r",ch);
	    else send_to_char("You have a pair of long pointed fangs, but they are not currently extended.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_CLAWS) )
	{
	    if (IS_VAMPAFF(ch, VAM_CLAWS)) send_to_char("You have a pair of razor sharp claws extending from your fingers.\n\r",ch);
	    else send_to_char("You have a pair of razor sharp claws, but they are not currently extended.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_HORNS) )
	{
	    if (IS_DEMAFF(ch, DEM_HORNS)) send_to_char("You have a pair of curved horns extending from your forehead.\n\r",ch);
	    else send_to_char("You have a pair of curved horns, but they are not currently extended.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_TAIL) )
	{
	    if (IS_DEMAFF(ch, DEM_TAIL)) send_to_char("You have a huge tail extended from your backside.\n\r",ch);
	    else send_to_char("You have a tail, but it is  not currently extended.\n\r",ch);
	}
            if ( IS_DEMPOWER(ch,DEM_HOOVES) )
	{
	    if (IS_DEMAFF(ch, DEM_HOOVES)) send_to_char("You have hooves instead of feet.\n\r",ch);
	    else send_to_char("You are able to transform your feet into hooves at will.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_EYES) )
	{
	    if (IS_VAMPAFF(ch, VAM_NIGHTSIGHT)) send_to_char("Your eyes are glowing bright red, allowing you to see in the dark.\n\r",ch);
	    else send_to_char("You are able to see in the dark, although that power is not currently activated.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_WINGS) )
	{
	    if (!IS_DEMAFF(ch, DEM_WINGS)) send_to_char("You have the ability to extend wings from your back.\n\r",ch);
	    else if (IS_DEMAFF(ch, DEM_UNFOLDED)) send_to_char("You have a pair of large leathery wings unfolded behind your back.\n\r",ch);
	    else send_to_char("You have a pair of large leathery wings folded behind your back.\n\r",ch);
	}
    	if ( IS_DEMPOWER(ch,DEM_MIGHT) )
	    send_to_char("Your muscles ripple with supernatural strength.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_TOUGH) )
	    send_to_char("Your skin reflects blows with supernatural toughness.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_SPEED) )
	    send_to_char("You move with supernatural speed and grace.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_TRAVEL) )
	    send_to_char("You are able to travel to other demons at will.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_SCRY) )
	    send_to_char("You are able to scry over great distances at will.\n\r",ch);
    	if ( IS_DEMPOWER(ch,DEM_SHADOWSIGHT) )
	{
	    if (IS_AFFECTED(ch, AFF_SHADOWSIGHT)) send_to_char("You are able see things in the shadowplane.\n\r",ch);
	    else send_to_char("You are able to view the shadowplane, although you are not currently doing so.\n\r",ch);
	}
    }
    else if (IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
	send_to_char("                              -= Vampire upkeep =-\n\r",ch);
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
	send_to_char("Staying alive...upkeep 1.\n\r",ch);

    	if (IS_VAMPAFF(ch,VAM_DISGUISED) )
	{
	    if      (ch->beast <= 0  ) sprintf(buf,"You are disguised as %s...no upkeep.\n\r",ch->morph);
	    else if (ch->beast == 100) sprintf(buf,"You are disguised as %s...upkeep 10-20.\n\r",ch->morph);
	    else sprintf(buf,"You are disguised as %s...upkeep 5-10.\n\r",ch->morph);
	    send_to_char(buf,ch);
	}
    	if (IS_IMMUNE(ch,IMM_SHIELDED) )
	{
	    if      (ch->beast <= 0  ) sprintf(buf,"You are shielded...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are shielded...upkeep 2-6.\n\r");
	    else sprintf(buf,"You are shielded...upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) )
	{
	    if      (ch->beast <= 0  ) sprintf(buf,"You are in the shadowplane...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are in the shadowplane...no upkeep.\n\r");
	    else sprintf(buf,"You are in the shadowplane...no upkeep.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_FANGS) )
	{
	    if      (ch->beast <= 0  ) sprintf(buf,"You have your fangs out...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have your fangs out...upkeep 2.\n\r");
	    else sprintf(buf,"You have your fangs out...upkeep 1.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_CLAWS) )
	{
	    if      (ch->beast <= 0  ) sprintf(buf,"You have your claws out...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have your claws out...upkeep 2-6.\n\r");
	    else sprintf(buf,"You have your claws out...upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
	{
	    if      (ch->beast <= 0  ) sprintf(buf,"You have nightsight...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have nightsight...upkeep 2.\n\r");
	    else sprintf(buf,"You have nightsight...upkeep 1.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
	{
	    if      (ch->beast <= 0  ) sprintf(buf,"You have shadowsight...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are shadowsight...upkeep 2-6.\n\r");
	    else sprintf(buf,"You are shadowsight...upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
	}
    	if (xIS_SET(ch->act,PLR_HOLYLIGHT) )
	{
	    if      (ch->beast <= 0  ) sprintf(buf,"You have truesight...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You have truesight...upkeep 2-10.\n\r");
	    else sprintf(buf,"You have truesight...upkeep 1-5.\n\r");
	    send_to_char(buf,ch);
	}
    	if (IS_VAMPAFF(ch,VAM_CHANGED) )
	{
	    if      (IS_POLYAFF(ch,POLY_BAT))  sprintf(buf2,"bat" );
	    else if (IS_POLYAFF(ch,POLY_WOLF)) sprintf(buf2,"wolf");
	    else                               sprintf(buf2,"mist");
	    if      (ch->beast <= 0  ) sprintf(buf,"You have changed into %s form...no upkeep.\n\r",buf2);
	    else if (ch->beast == 100) sprintf(buf,"You have changed into %s form...upkeep 10-20.\n\r",buf2);
	    else sprintf(buf,"You have changed into %s form...upkeep 5-10.\n\r",buf2);
	    send_to_char(buf,ch);
	}
    	if (IS_POLYAFF(ch,POLY_SERPENT) )
	{
	    if      (ch->beast <= 0  ) sprintf(buf,"You are in serpent form...no upkeep.\n\r");
	    else if (ch->beast == 100) sprintf(buf,"You are in serpent form...upkeep 6-8.\n\r");
	    else sprintf(buf,"You are in serpent form...upkeep 1-3.\n\r");
	    send_to_char(buf,ch);
       	}
        if (IS_POLYAFF(ch,POLY_ZULOFORM))
        {
        send_to_char("You are in Zuloform.\n\r",ch);
        if (IS_EXTRA(ch,EXTRA_DRAGON))
        {
        send_to_char("You are a huge dragon.\n\r",ch);
        return;
        }
        }
        if (IS_EXTRA(ch, EXTRA_AWE))
        {
        send_to_char("You are awe inspiring.\n\r",ch);
        }
if (IS_AFFECTED(ch, AFF_PEACE)) send_to_char("Your majesty is up.\n\r",ch);
if (IS_SET(ch->newbits, NEW_MONKFLAME)) send_to_char("Your hands and weapon are engulfed in flames.\n\r",ch);       
if (IS_EXTRA(ch, EXTRA_PLASMA)) send_to_char("You are in plasmaform.\n\r",ch);     
if (ch->power[DISC_VAMP_PROT] > 8) send_to_char("Your flesh is as hard as marble.\n\r",ch);
if (ch->rage > 0) send_to_char("You are in frenzy.\n\r",ch);
if (xIS_SET(ch->act, PLR_WIZINVIS)) send_to_char("You are blended with the Shade.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_HEAD)) send_to_char("You have the head of a lion.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_TAIL)) send_to_char("A long tail protrudes from your backside.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_EXOSKELETON)) send_to_char("Your body is covered by a bony exoskeleton.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_HORNS)) send_to_char("Large horns protrude from your head.\n\r",ch);
if (IS_VAMPAFF(ch, VAM_WINGS)) send_to_char("Large wings protrude from your back.\n\r",ch);
}
else if (IS_CLASS(ch,CLASS_WEREWOLF))
{
send_to_char("---------------------------------------------------------------------------\n\r",ch);
send_to_char(
"                          Werewolf Upkeep\n\r",ch);
send_to_char("---------------------------------------------------------------------------\n\r",ch);
if (ch->rage>50)
send_to_char("You are raged into a werewolf.\n\r",ch);
sprintf(buf,"You have %d points of tolerance against silver.\n\r",ch->siltol);
send_to_char(buf,ch);
if (IS_SET(ch->newbits,NEW_REND))
send_to_char("You are focused on rending your opponents body.\n\r",ch);
if (IS_SET(ch->newbits,NEW_SLAM))
send_to_char("You will stun and strike your victim when he is down.\n\r",ch);
if (IS_SET(ch->newbits,NEW_QUILLS))
send_to_char("Razor sharp quills are extended from your back.\n\r",ch);
if (ch->power[DISC_WERE_RAPT] > 0)
send_to_char("Your fangs are much sharper then usual.\n\r",ch);
if (IS_SET(ch->newbits,NEW_JAWLOCK))
send_to_char("Your jaw is strengthened to clamp on your foes.\n\r",ch);
if (IS_SET(ch->newbits,NEW_SKIN))
send_to_char("Your skin is very tough, making it hard to slash and pierce.\n\r",ch);
if (IS_SET(ch->newbits,NEW_PERCEPTION))
send_to_char("You can sense the slightest movement.\n\r",ch);
if (ch->power[DISC_WERE_BOAR] > 2)
send_to_char("You are extra tough, halving damage.\n\r",ch);

send_to_char("---------------------------------------------------------------------------\n\r",ch);
return;
}

    else
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    return;
}

void do_werewolf( CHAR_DATA *ch, char *argument )
{
    char       buf[MAX_INPUT_LENGTH];
    char       arg[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA  *obj;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF)) return;
    if (IS_SET(ch->special, SPC_WOLFMAN)) return;
    SET_BIT(ch->special, SPC_WOLFMAN);

    send_to_char("You throw back your head and howl with rage!\n\r",ch);
    act("$n throws back $s head and howls with rage!.", ch, NULL, NULL, TO_ROOM, FALSE);
    send_to_char("Coarse dark hair sprouts from your body.\n\r",ch);
    act("Coarse dark hair sprouts from $n's body.",ch,NULL,NULL,TO_ROOM, FALSE);
    if (!IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("Your eyes start glowing red.\n\r",ch);
	act("$n's eyes start glowing red.",ch,NULL,NULL,TO_ROOM, FALSE);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    if (!IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("A pair of long fangs extend from your mouth.\n\r",ch);
	act("A pair of long fangs extend from $n's mouth.",ch,NULL,NULL,TO_ROOM, FALSE);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (!IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Razor sharp talons extend from your fingers.\n\r",ch);
	act("Razor sharp talons extend from $n's fingers.",ch,NULL,NULL,TO_ROOM, FALSE);
	SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    if ((obj = get_eq_char(ch,WEAR_WIELD)) != NULL && (!IS_SET(obj->spectype, SITEM_WOLFWEAPON) || obj->pIndexData->vnum ==
OBJ_VNUM_PROTOPLASM))
    {
	act("$p drops from your right hand.",ch,obj,NULL,TO_CHAR, FALSE);
	act("$p drops from $n's right hand.",ch,obj,NULL,TO_ROOM, FALSE);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    if ((obj = get_eq_char(ch,WEAR_HOLD)) != NULL && (!IS_SET(obj->spectype, SITEM_WOLFWEAPON) || obj->pIndexData->vnum ==
OBJ_VNUM_PROTOPLASM))
    {
	act("$p drops from your left hand.",ch,obj,NULL,TO_CHAR, FALSE);
	act("$p drops from $n's left hand.",ch,obj,NULL,TO_ROOM, FALSE);
	obj_from_char(obj);
	obj_to_char(obj, ch);
    }
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    sprintf(buf,"%s the werewolf", ch->name);
    free_string(ch->morph);
    ch->morph = str_dup(buf);
    ch->rage += 25;
    ch->hitroll += 1500;
    ch->damroll += 1500;
    if (ch->power[DISC_WERE_WOLF] > 3) ch->rage += 100;
    if (ch->rage > 300) 
	ch->rage = 300;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( ch == vch )
	{
	    act("You throw back your head and howl with rage!", ch, NULL, NULL, TO_CHAR, FALSE);
	    continue;
	}
	if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
	    continue;
	if (!IS_NPC(vch))
	{
	    if (vch->in_room == ch->in_room)
		act("$n throws back $s head and howls with rage!", ch, NULL, vch, TO_VICT, FALSE);
	    else if (vch->in_room->area == ch->in_room->area)
		send_to_char("You hear a fearsome howl close by!\n\r", vch);
	    else
	    if (!CAN_PK(vch)) continue;
	}
	if ( vch->in_room == ch->in_room && can_see( ch, vch) )
	{
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	    if (vch == NULL || vch->position <= POS_STUNNED) continue;
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	}
    }
    return;
}

void do_unwerewolf( CHAR_DATA *ch, char *argument )
{
    char       arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WEREWOLF)) return;
    if (!IS_SET(ch->special, SPC_WOLFMAN)) return;
    REMOVE_BIT(ch->special, SPC_WOLFMAN);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    free_string(ch->morph);
    ch->morph = str_dup("");
    if (IS_VAMPAFF(ch,VAM_CLAWS))
    {
	send_to_char("Your talons slide back into your fingers.\n\r",ch);
	act("$n's talons slide back into $s fingers.",ch,NULL,NULL,TO_ROOM, FALSE);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    if (IS_VAMPAFF(ch,VAM_FANGS))
    {
	send_to_char("Your fangs slide back into your mouth.\n\r",ch);
	act("$n's fangs slide back into $s mouth.",ch,NULL,NULL,TO_ROOM, FALSE);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT))
    {
	send_to_char("The red glow in your eyes fades.\n\r",ch);
	act("The red glow in $n's eyes fades.",ch,NULL,NULL,TO_ROOM, FALSE);
	REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    send_to_char("Your coarse hair shrinks back into your body.\n\r",ch);
    act("$n's coarse hair shrinks back into $s body.",ch,NULL,NULL,TO_ROOM, FALSE);
//    ch->rage -= 25;
    ch->hitroll -= 1500;
    ch->damroll -= 1500;
    if (ch->rage < 0) ch->rage = 0;
    ch->rage = 0;
    return;
}
void do_possession( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (ch->power[DISC_VAMP_DOMI] < 3 && IS_CLASS(ch, CLASS_MAGE) )
    {
        send_to_char("You must obtain at least level 3 in Dominate to use Possession.\n\r",ch);
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL)
    {
	sprintf(buf,"You break your hold over %s.\n\r",familiar->short_descr);
	send_to_char( buf, ch );
	familiar->wizard = NULL;
	ch->pcdata->familiar = NULL;
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to Possess?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "What an intelligent idea!\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if (victim->wizard != NULL)
    {
	send_to_char( "You are unable to possess them.\n\r", ch );
	return;
    }

    if (victim->level > (ch->spl[RED_MAGIC] * 0.25))
    {
	send_to_char( "They are too powerful.\n\r", ch );
	return;
    }

    if ( ch->pcdata->condition[COND_THIRST] < 50 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
	send_to_char("You have insufficient blood.\n\r",ch);
	return;
    }

    ch->pcdata->condition[COND_THIRST] -= 50;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    act("You concentrate on $N.",ch,NULL,victim,TO_CHAR, FALSE);
    act("$n is staring at you!",ch,NULL,victim,TO_VICT, FALSE);
    act("$n starts staring at $N",ch,NULL,victim,TO_NOTVICT, FALSE);
    return;
}

void do_familiar( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Become your own familiar?\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
	familiar->wizard = NULL;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    send_to_char("Ok.\n\r",ch);

    return;
}

void do_fcommand( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;
/*
    if ((!IS_CLASS(ch, CLASS_VAMPIRE)  ch->level < LEVEL_APPRENTICE) ||
!IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
*/
    if (IS_CLASS(ch, CLASS_VAMPIRE) && (ch->power[DISC_VAMP_DOMI] < 3) )
    {
	send_to_char("You are not trained in the Dominate discipline.\n\r",ch);
	return;
    }

    if ( ( victim = ch->pcdata->familiar ) == NULL )
    {
	send_to_char( "But you don't have a familiar!\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar do?\n\r", ch );
	return;
    }
    interpret(victim,argument);
    return;
}

void do_vanish( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_VAMPIRE) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_OWL] < 1)
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBFU] < 1)
    {
	send_to_char("Huh?\n\r",ch);
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

        if (ch->mana < 1000)
        {
        send_to_char("You don't have enough mana to vanish.\n\r",ch );
        return;
        }

    if ( xIS_SET(ch->act, PLR_HIDE) )
    {
	xREMOVE_BIT(ch->act, PLR_HIDE);
	send_to_char( "You slowly fade into existance.\n\r", ch );
	act("$n slowly fades into existance.",ch,NULL,NULL,TO_ROOM, FALSE);
    }
    else
    {
	ch->mana -= 1000;
	send_to_char( "You slowly fade out of existance.\n\r", ch );
	act("$n slowly fades out of existance.",ch,NULL,NULL,TO_ROOM, FALSE);
	xSET_BIT(ch->act, PLR_HIDE);
    }
    return;
}

void do_flex( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    act("You flex your bulging muscles.",ch,NULL,NULL,TO_CHAR, FALSE);
    act("$n flexes $s bulging muscles.",ch,NULL,NULL,TO_ROOM, FALSE);

    if ( IS_EXTRA(ch, TIED_UP) )
    {
	act("The ropes restraining you snap.",ch,NULL,NULL,TO_CHAR, FALSE);
	act("The ropes restraining $n snap.",ch,NULL,NULL,TO_ROOM, FALSE);
	xREMOVE_BIT(ch->extra, TIED_UP);
        WAIT_STATE(ch,10);
    }
    if ( IS_EXTRA(ch, FFA_HUNG))
        xREMOVE_BIT(ch->extra, FFA_HUNG);
    if (is_affected(ch, gsn_web))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM, FALSE);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	affect_strip(ch, gsn_web);
        WAIT_STATE(ch,18);
    }
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM, FALSE);
	send_to_char("The webbing entrapping you breaks away.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_WEBBED);
        WAIT_STATE(ch,18);
    }

    return;
}

void do_rage( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DEMON))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

  if (IS_CLASS(ch, CLASS_WEREWOLF))
  {
/*
    if (ch->power[DISC_WERE_BEAR] < 3)
    {
	stc("You need to obtain level three in the Bear totem.\n\r",ch);
	return;
    }
*/
    if (!IS_SET(ch->special, SPC_WOLFMAN))
    {
	send_to_char("You start snarling angrilly.\n\r",ch);
	act("$n starts snarling angrilly.",ch,NULL,NULL,TO_ROOM, FALSE);
	ch->rage += number_range(40,60);
	if (ch->rage >= 100) do_werewolf(ch,"");
	WAIT_STATE(ch,12);
	return;
    }
    send_to_char("But you are already in a rage!\n\r",ch);
    }

    if (IS_CLASS(ch, CLASS_DEMON))
    {
	if (ch->power[DISC_DAEM_ATTA] < 3)
        {
	    stc("You try, but cannot summon your beast.\n\r",ch);
	    return;
	}

     if (ch->beast < 1)
	ch->beast = 1;

        if (ch->beast > ch->rage || ch->rage < 500)
	{
	    ch->rage += number_range(ch->generation,25);
	    stc("You scream as you plunge into a mad attacking frenzy.\n\r",ch);
	    act("$n screams in frenzied rage.",ch,NULL,NULL,TO_ROOM, FALSE);
	    if (number_percent() < (ch->beast + 1))
	    {
		stc("You feel the beast take more control.\n\r",ch);
		ch->beast += 1;
		if (ch->beast > 100)
		  ch->beast = 100;
	    }
	    WAIT_STATE(ch, 12);
	    return;
	}
       
        stc("You have reached the pinnacle of your frenzy.\n\r",ch);
    }

    return;
}

void do_calm( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_NINJA))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

 if (IS_CLASS(ch, CLASS_WEREWOLF))
 {

    if (ch->power[DISC_WERE_WOLF] < 3)
    {
	stc("You need level three in the Wolf totem to Calm.\n\r",ch);
	return;
    }  

    if (IS_SET(ch->special, SPC_WOLFMAN))
    {
	send_to_char("You take a deep breath and calm yourself.\n\r",ch);
	act("$n takes a deep breath and tries to calm $mself.",ch,NULL,NULL,TO_ROOM, FALSE);
	ch->rage -= number_range(60,90);
        if (ch->rage < 0) ch->rage = 0;
	if (ch->rage < 100) do_unwerewolf(ch,"");
	WAIT_STATE(ch,8);
	return;
    }
    send_to_char("But you are not in crinos form!\n\r",ch);
 }
if (IS_CLASS(ch, CLASS_NINJA))
   {
send_to_char("You take a deep breath and calm yourself.\n\r",ch);
        act("$n takes a deep breath and tries to calm $mself.",ch,NULL,NULL,TO_ROOM, FALSE);
ch->rage = 0;
WAIT_STATE(ch,12);
        return;
    }
   if (IS_CLASS(ch, CLASS_DEMON))
   {

    if (ch->power[DISC_DAEM_ATTA] < 4)
    {
	stc("You need level four in Attack to calm your frenzy.\n\r",ch);
	return;
    }

    if (ch->beast >= 100) ch->beast = 100;
    if (ch->beast <= 0) ch->beast = 0;

    ch->rage -= (40 - (ch->beast / 3) +dice (1, 10));
    if (ch->rage <= 0)
    {
	ch->rage = 0;
	send_to_char("You have calmed out of your rage completely.\n\r", ch);
    }
    else
    {
      send_to_char("You hold your breath and try to calm your frenzy.\n\r", ch);
      WAIT_STATE(ch, 8);
    }
    return;
  } 
    return;
}

void do_totems( CHAR_DATA *ch, char *argument )
{

    if (IS_NPC(ch)) return;


    send_to_char("Please use the new commands, Disciplines and research.\n\r",ch);
	return;

}

void do_web( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int sn;
    int level;
    int spelltype;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_CYBORG)
     && !IS_CLASS(ch, CLASS_TANARRI) && !IS_CLASS(ch, CLASS_HIGHLANDER) && 
!IS_CLASS(ch,CLASS_DEMON) && !IS_CLASS(ch, CLASS_PALADIN) && !IS_CLASS(ch, CLASS_NINJA))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

	if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_SPID] < 2) 
	{send_to_char("You don't have that power yet.\n\r", ch );
	return;}

	if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->pcdata->powers[1],DPOWER_WEB))
	{send_to_char("You don't have that power yet.\n\r", ch );
	return;}

    if (IS_CLASS(ch, CLASS_TANARRI) && ch->pcdata->rank < 3)
    {
      send_to_char("You don't have that power yet.\n\r",ch);
      return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot web yourself.\n\r", ch );
	return;
    }
    if (!IS_NPC(victim) && !IS_NPC(ch) && (ch != victim) && (is_safe(ch,victim) == TRUE) ) return;

    if ( IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
	send_to_char( "You cannot web an ethereal person.\n\r", ch );
	return;
    }

    if ( ( sn = skill_lookup( "web" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 0.50;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;
}

void do_birth( CHAR_DATA *ch, char *argument )
{
    char buf2[MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;

    if (!IS_EXTRA(ch, EXTRA_PREGNANT))
    {
	send_to_char("But you are not even pregnant!\n\r",ch);
	return;
    }

    if (!IS_EXTRA(ch, EXTRA_LABOUR))
    {
	send_to_char("You're not ready to give birth yet.\n\r",ch);
	return;
    }

    if ( argument[0] == '\0' )
    {
	if (ch->pcdata->genes[4] == SEX_MALE)
	    send_to_char( "What do you wish to name your little boy?\n\r", ch );
	else if (ch->pcdata->genes[4] == SEX_FEMALE)
	    send_to_char( "What do you wish to name your little girl?\n\r", ch );
	else
	    send_to_char( "What do you wish to name your child?\n\r", ch );
	return;
    }

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( char_exists(FALSE,argument) )
    {
	send_to_char( "That player already exists.\n\r", ch );
	return;
    }

    strcpy(buf2,ch->pcdata->cparents);
    strcat(buf2," ");
    strcat(buf2,argument);

    if (!birth_ok(ch, buf2))
    {
	send_to_char( "Bug - please inform KaVir.\n\r", ch );
	return;
    }
    argument[0] = UPPER(argument[0]);

/*    if(!raped) birth_write( ch, argument );*/

    ch->pcdata->genes[9] += 1;
    xREMOVE_BIT(ch->extra, EXTRA_PREGNANT);
    xREMOVE_BIT(ch->extra, EXTRA_LABOUR);
    save_char_obj(ch);
    return;
}

bool birth_ok( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH];
    char mum [MAX_INPUT_LENGTH];
    char dad [MAX_INPUT_LENGTH];
    char child [MAX_INPUT_LENGTH];

    argument = one_argument( argument, mum );
    argument = one_argument( argument, dad );
    argument = one_argument( argument, child );

    if (dad[0] == '\0')
    {
	send_to_char("You are unable to give birth - please inform KaVir.\n\r",ch);
	return FALSE;
    }
    if (child[0] == '\0')
    {
	send_to_char("You are unable to give birth - please inform KaVir.\n\r",ch);
	return FALSE;
    }
    dad[0] = UPPER(dad[0]);
    if (!str_cmp(dad,"Kavir")) strcpy(dad,"KaVir");
      child[0] = UPPER(child[0]);
    if (ch->pcdata->genes[4] == SEX_FEMALE)
    {
	send_to_char("You give birth to a little girl!\n\r",ch);
	sprintf(buf,"%s has given birth to %s's daughter, named %s!",ch->name,dad,child);
	do_info(ch,buf);
	return TRUE; 
    }
    else if (ch->pcdata->genes[4] == SEX_MALE)
    {
	send_to_char("You give birth to a little boy!\n\r",ch);
	sprintf(buf,"%s has given birth to %s's son, named %s!",ch->name,dad,child);
	do_info(ch,buf);
	return TRUE; 
    }
    return FALSE;
}


bool char_exists( bool backup, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    bool found = FALSE;

    fclose( fpReserve );
    if (backup)
    	sprintf( buf, "%sbackup/%s", PLAYER_DIR, capitalize( argument ) );
    else
    	sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen( buf, "r" ) ) != NULL )
    {
	found = TRUE;
	fclose( fp );
    }
//    else if ((fp=fopen(buf, "r")) == NULL)
//    {
//	found = FALSE;
//	fclose(fp);
//    }
    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}

void birth_write( CHAR_DATA *ch, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    char *strtime;

    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';

    sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen(buf, "w") ) != NULL) 
    {  
AFFECT_DATA *paf;
    int sn;
    int i;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYERS"                );
    fprintf( fp, "Name         %s~\n",  ch->name                );
    fprintf( fp, "Switchname   %s~\n",  ch->pcdata->switchname  );
    fprintf( fp, "ShortDescr   %s~\n",  ch->short_descr         );
    fprintf( fp, "LongDescr    %s~\n",  ch->long_descr          );
    fprintf( fp, "ObjDesc      %s~\n",  ch->objdesc		);
    fprintf( fp, "Description  %s~\n",  ch->description         );
    fprintf( fp, "Lord         %s~\n",  ch->lord                );
    fprintf( fp, "Kingdom      %d\n", ch->pcdata->kingdom	);
    fprintf( fp, "CPower %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
ch->power[0],ch->power[1],ch->power[2],ch->power[3],ch->power[4],
ch->power[5],ch->power[6],ch->power[7],ch->power[8],ch->power[9],
ch->power[10],ch->power[11],ch->power[12],ch->power[13],ch->power[14],
ch->power[15],ch->power[16],ch->power[17],ch->power[18],ch->power[19],
ch->power[20],ch->power[21],ch->power[22],ch->power[23],ch->power[24],
ch->power[25],ch->power[26],ch->power[27],ch->power[28],ch->power[29],
ch->power[30],ch->power[31],ch->power[32],ch->power[33],ch->power[34],
ch->power[35],ch->power[36],ch->power[37],ch->power[38],ch->power[39],
ch->power[40],ch->power[41],ch->power[42],ch->power[43]);
    fprintf( fp, "DiscRese     %d\n",   ch->pcdata->disc_research);
    fprintf( fp, "DiscPoin     %d\n",   ch->pcdata->disc_points );
    fprintf( fp, "Class        %d\n",   ch->class               );
    fprintf( fp, "Morph        %s~\n",  ch->morph               );
    fprintf( fp, "Createtime   %s~\n",  ch->createtime          );
    fprintf( fp, "Lasttime     %s~\n",  ch->lasttime            );
    fprintf( fp, "Lasthost     %s~\n",  ch->lasthost            );
    fprintf( fp, "Poweraction  %s~\n",  ch->poweraction         );
    fprintf( fp, "Powertype    %s~\n",  ch->powertype           );
    fprintf( fp, "Prompt       %s~\n",  ch->prompt              );
    fprintf( fp, "Cprompt      %s~\n",  ch->cprompt             );
    fprintf( fp, "Sex          %d\n",   ch->sex                 );
    fprintf( fp, "Race         %d\n",   ch->race                );
    fprintf( fp, "Immune       %d\n",   ch->immune              );
    fprintf( fp, "Polyaff      %d\n",   ch->polyaff             );
/* Dh Crap */
    fprintf( fp, "CurrentForm  %d\n",   ch->cur_form            );
    fprintf( fp, "Rage         %d\n",   ch->rage                );
    fprintf( fp, "Generation   %d\n",   ch->generation          );
    fprintf( fp, "Flag2       %d\n",   ch->flag2              );
    fprintf( fp, "Flag3       %d\n",   ch->flag3              );
    fprintf( fp, "Flag4       %d\n",   ch->flag4              ); 
    fprintf( fp, "SilTol      %d\n",   ch->siltol               );
    fprintf( fp, "Souls	      %d\n",   ch->pcdata->souls        );
/* Lala */
    fprintf( fp, "Itemaffect   %d\n",   ch->itemaffect          );
    fprintf( fp, "Form         %d\n",   ch->form                );
    fprintf( fp, "Beast        %d\n",   ch->beast               );
    fprintf( fp, "Spectype     %d\n",   ch->spectype            );
    fprintf( fp, "Specpower    %d\n",   ch->specpower           );
    fprintf( fp, "Home         %d\n",   ch->home                );
    fprintf( fp, "Level        %d\n",   ch->level               );
    fprintf( fp, "Trust        %d\n",   ch->trust               );
    fprintf( fp, "Security     %d\n",    ch->pcdata->security    );  /*
OLC */
    fprintf( fp, "Paradox      %d %d %d\n", ch->paradox[0],
ch->paradox[1],ch->paradox[2] ); 
    fprintf( fp, "Played       %d\n",ch->played + (int) (current_time -
ch->logon)           );
    fprintf( fp, "Room         %d\n",(  ch->in_room == get_room_index(
ROOM_VNUM_LIMBO )&& ch->was_in_room != NULL ) ? ch->was_in_room->vnum :
ch->in_room->vnum );
    fprintf( fp, "PkPdMkMd     %d %d %d %d\n",ch->pkill, ch->pdeath,
ch->mkill, ch->mdeath );
    fprintf( fp, "Alos         %d\n", ch->pcdata->alosses);
    fprintf( fp, "Awin         %d\n", ch->pcdata->awins); 
    fprintf( fp, "Weapons      %d %d %d %d %d %d %d %d %d %d %d %d %d\n",ch->wpn[0], ch->wpn[1], ch->wpn[2], ch->wpn[3], ch->wpn[4], ch->wpn[5], ch->wpn[6], ch->wpn[7], ch->wpn[8], ch->wpn[9], ch->wpn[10], ch->wpn[11], ch->wpn[12] );
    fprintf( fp, "Spells       %d %d %d %d %d\n",ch->spl[0], ch->spl[1],
ch->spl[2], ch->spl[3], ch->spl[4] );
    fprintf( fp, "Combat       %d %d %d %d %d %d %d %d\n",ch->cmbt[0],
ch->cmbt[1], ch->cmbt[2], ch->cmbt[3],ch->cmbt[4], ch->cmbt[5],
ch->cmbt[6], ch->cmbt[7] );
    fprintf( fp, "Stance       %d %d %d %d %d %d %d %d %d %d %d %d\n",ch->stance[0], ch->stance[1], ch->stance[2],
ch->stance[3],ch->stance[4], ch->stance[5], ch->stance[6],
ch->stance[7],ch->stance[8], ch->stance[9], ch->stance[10], ch->stance[11]
);
    fprintf( fp, "Stance2      %d %d %d %d %d %d %d %d %d %d %d %d\n",ch->stance[12], ch->stance[12], ch->stance[12],
ch->stance[12],ch->stance[12], ch->stance[12], ch->stance[12],
ch->stance[12],ch->stance[12], ch->stance[12], ch->stance[12],
ch->stance[12] );
    fprintf( fp, "Locationhp   %d %d %d %d %d %d %d\n",ch->loc_hp[0],
ch->loc_hp[1], ch->loc_hp[2], ch->loc_hp[3],ch->loc_hp[4], ch->loc_hp[5],
ch->loc_hp[6] );
    fprintf( fp, "HpManaMove   %d %d %d %d %d %d\n",ch->hit, ch->max_hit,
ch->mana, ch->max_mana, ch->move, ch->max_move );
    fprintf( fp, "Gold         %2.0f\n",   ch->gold                );
    if (IS_CLASS(ch, CLASS_DEMON))
    {
    fprintf( fp, "Warps        %d\n",   ch->warp                );
    fprintf( fp, "WarpCount    %d\n",   ch->warpcount           );
    }
    fprintf( fp, "Monkstuff    %d\n",   ch->monkstuff           );
    fprintf( fp, "Monkcrap     %d\n",   ch->monkcrap            );
    fprintf( fp, "Chi       %d %d\n",  ch->chi[0], ch->chi[1]   );
    fprintf( fp, "Focus     %d %d\n",  ch->focus[0], ch->focus[1] );
    fprintf( fp, "Monkab     %d %d %d %d\n", ch->monkab[0],
ch->monkab[1],ch->monkab[2], ch->monkab[3]);
    fprintf( fp, "Gifts    %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\r",ch->gifts[0],  ch->gifts[1],
ch->gifts[2],ch->gifts[3],  ch->gifts[4],  ch->gifts[5],ch->gifts[6],
ch->gifts[7],  ch->gifts[8],ch->gifts[9],  ch->gifts[10],
ch->gifts[11],ch->gifts[12], ch->gifts[13], ch->gifts[14],ch->gifts[15],
ch->gifts[16], ch->gifts[17],ch->gifts[18], ch->gifts[19], ch->gifts[20]
);
    fprintf( fp, "Garou1        %d\n", ch->garou1 );
    fprintf( fp, "Garou2        %d\n", ch->garou2 );
    fprintf( fp, "Explevel     %d\n",   ch->explevel            );
    fprintf( fp, "Expgained    %d\n",   ch->expgained           );
    fprintf( fp, "Exp          %2.0f\n",   ch->exp                 );
    fprintf( fp, "Levelexp     %d\n",   ch->levelexp            );
    fprintf( fp, "Act          %s\n",   print_bitvector( &ch->act                ) );
    fprintf( fp, "Special      %d\n",   ch->special             );
    fprintf( fp, "Newbits      %d\n",   ch->newbits             );
    fprintf( fp, "Extra        %s\n",   print_bitvector( &ch->extra ) );
    fprintf( fp, "AffectedBy   %d\n",   ch->affected_by         );
    fprintf( fp, "Position     %d\n",ch->position == POS_FIGHTING ?
POS_STANDING : ch->position );
    fprintf( fp, "Practice     %d\n",   ch->practice            );
    fprintf( fp, "SavingThrow  %d\n",   ch->saving_throw        );
    fprintf( fp, "Alignment    %d\n",   ch->alignment           );
    fprintf( fp, "XHitroll     %d\n",   ch->xhitroll            );
    fprintf( fp, "XDamroll     %d\n",   ch->xdamroll            );
    fprintf( fp, "Hitroll      %d\n",   ch->hitroll             );
    fprintf( fp, "Damroll      %d\n",   ch->damroll             );
    fprintf( fp, "Armor        %d\n",   ch->armor               );
    fprintf( fp, "Wimpy        %d\n",   ch->wimpy               );
    fprintf( fp, "Deaf         %s\n",   print_bitvector( &ch->deaf                ) );
    if ( IS_NPC(ch) )
    {
	fprintf( fp, "Vnum         %d\n",       ch->pIndexData->vnum    );
    }
    else
    {
	fprintf( fp, "Password     %s~\n",      ch->pcdata->pwd         );
	fprintf( fp, "Bamfin       %s~\n",      ch->pcdata->bamfin      );
	fprintf( fp, "Bamfout      %s~\n",      ch->pcdata->bamfout     );
	fprintf( fp, "Title        %s~\n",      ch->pcdata->title       );
	fprintf( fp, "Conception   %s~\n",      ch->pcdata->conception  );
	fprintf( fp, "Parents      %s~\n",      ch->pcdata->parents     );
	fprintf( fp, "Cparents     %s~\n",      ch->pcdata->cparents    );
	fprintf( fp, "Marriage     %s~\n",      ch->pcdata->marriage    );
	fprintf( fp, "Email        %s~\n",      ch->pcdata->email       );
	fprintf( fp, "AttrPerm     %d %d %d %d %d\n",ch->pcdata->perm_str,ch->pcdata->perm_int,ch->pcdata->perm_wis,ch->pcdata->perm_dex,ch->pcdata->perm_con
);
    fprintf( fp, "AttrMod      %d %d %d %d %d\n",ch->pcdata->mod_str,
ch->pcdata->mod_int, ch->pcdata->mod_wis,ch->pcdata->mod_dex,
ch->pcdata->mod_con );
	fprintf( fp, "Quest        %d\n", ch->pcdata->quest );
	fprintf( fp, "Wolf         %d\n", ch->rage      );
	fprintf( fp, "Rank         %d\n", ch->pcdata->rank );
	fprintf( fp, "Language     %d %d\n",ch->pcdata->language[0],ch->pcdata->language[1] );
	fprintf( fp, "Stage        %d %d %d\n",ch->pcdata->stage[0],ch->pcdata->stage[1],ch->pcdata->stage[2] );
	fprintf( fp, "Score        %d %d %d %d %d %d\n",ch->pcdata->score[0],ch->pcdata->score[1],ch->pcdata->score[2],ch->pcdata->score[3],ch->pcdata->score[4],ch->pcdata->score[5]);
	fprintf( fp, "Genes        %d %d %d %d %d %d %d %d %d %d\n",ch->pcdata->genes[0],ch->pcdata->genes[1],ch->pcdata->genes[2],ch->pcdata->genes[3],ch->pcdata->genes[4],ch->pcdata->genes[5],ch->pcdata->genes[6],ch->pcdata->genes[7],ch->pcdata->genes[8],ch->pcdata->genes[9]
);
	fprintf( fp, "Power        ");
	for (sn=0; sn < 20; sn++)
	{
	fprintf(fp, "%d ", ch->pcdata->powers[sn]);
	}
	fprintf(fp, "\n");
    fprintf( fp, "Stats        ");
	for (sn=0; sn < 12; sn++)
	{
	fprintf(fp, "%d ", ch->pcdata->stats[sn]);
	}
	fprintf(fp, "\n");
    fprintf( fp, "FakeCon      %d %d %d %d %d %d %d %d\n",ch->pcdata->fake_skill, ch->pcdata->fake_stance,
ch->pcdata->fake_hit,ch->pcdata->fake_dam,ch->pcdata->fake_ac,ch->pcdata->fake_hp,ch->pcdata->fake_mana,ch->pcdata->fake_move);
    if (ch->pcdata->obj_vnum != 0)
	fprintf( fp, "Objvnum      %d\n", ch->pcdata->obj_vnum );
    fprintf( fp, "Condition    %d %d %d\n",ch->pcdata->condition[0],ch->pcdata->condition[1],ch->pcdata->condition[2]
);
    fprintf( fp, "StatAbility  %d %d %d %d\n",ch->pcdata->stat_ability[0],ch->pcdata->stat_ability[1],ch->pcdata->stat_ability[2],ch->pcdata->stat_ability[3]
);
	fprintf( fp, "StatAmount   %d %d %d %d\n",ch->pcdata->stat_amount[0],ch->pcdata->stat_amount[1],ch->pcdata->stat_amount[2],ch->pcdata->stat_amount[3]
);
    fprintf( fp, "StatDuration %d %d %d %d\n",ch->pcdata->stat_duration[0],ch->pcdata->stat_duration[1],ch->pcdata->stat_duration[2],ch->pcdata->stat_duration[3]
);
	fprintf( fp, "Exhaustion   %d\n", ch->pcdata->exhaustion );
	/* Save note board status */
	/* Save number of boards in case that number changes */
	fprintf (fp, "Boards       %d ", MAX_BOARD);
	for (i = 0; i < MAX_BOARD; i++)
	fprintf (fp, "%s %ld ", boards[i].short_name,
ch->pcdata->last_note[i]);
	fprintf (fp, "\n");
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	if ( skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0 )
	{
    fprintf( fp, "Skill        %d '%s'\n",ch->pcdata->learned[sn],
skill_table[sn].name );
	}
	}
    }
    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	/* Thx Alander */
	if ( paf->type < 0 || paf->type >= MAX_SKILL )
	continue;
    fprintf( fp, "AffectData   '%s' %3d %3d %3d %10d\n",skill_table[paf->type].name,paf->duration,paf->modifier,paf->location,paf->bitvector);
    }
    fprintf( fp, "End\n\n" ); 
fclose( fp );
    }
    return;
}



void do_mitsukeru( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powers[NPOWER_SORA] <1) {
	send_to_char("Huh?\n\r", ch );
	return;}
	if (arg[0] == '\0')
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (victim->trust > 6)
    {
      send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
      send_to_char("Someone is scrying you.\n\r",victim);
      return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim,IMM_SHIELDED))
    {
       send_to_char( "You cannot find them.\n\r",ch);
       return;
    }

    chroom = ch->in_room;
    victimroom = victim->in_room;

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
	do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    ch->fight_timer += 5;
    return;
}

void do_koryou( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powers[NPOWER_SORA] <2) {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Read the aura on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "Read the aura on what?\n\r", ch );
	    return;
	}
	act("$n examines $p intently.",ch,obj,NULL,TO_ROOM, FALSE);
	spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	return;
    }

    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
(IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
	send_to_char("You are unable to read their aura.\n\r",ch);
	return;
    }

    act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT, FALSE);
    act("$n examines you intently.",ch,NULL,victim,TO_VICT, FALSE);
    if (IS_NPC(victim)) sprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
    else 
    {
	if      (victim->level == 12) sprintf(buf, "%s is an Implementor.\n\r", victim->name);
	else if (victim->level == 11) sprintf(buf, "%s is a High Judge.\n\r", victim->name);
	else if (victim->level == 10) sprintf(buf, "%s is a Judge.\n\r", victim->name);
	else if (victim->level == 9 ) sprintf(buf, "%s is an Enforcer.\n\r", victim->name);
	else if (victim->level == 8 ) sprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
	else if (victim->level == 7 ) sprintf(buf, "%s is a Builder.\n\r", victim->name);
	else if (victim->level >= 3 ) sprintf(buf, "%s is an Avatar.\n\r", victim->name);
	else sprintf(buf, "%s is a Mortal.\n\r", victim->name);
    }
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
	send_to_char(buf,ch);
    }
    sprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
    if (!IS_NPC(victim)) sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
    else sprintf(buf,"AC:%d.\n\r",char_ac(victim));
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
	sprintf(buf,"Status:%d, ",victim->race);
	send_to_char(buf,ch);
	if (IS_CLASS(victim, CLASS_VAMPIRE))    
	{
	    sprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
	    send_to_char(buf,ch);
	}
    }
    sprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
	act("$N is pregnant.",ch,NULL,victim,TO_CHAR, FALSE);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))    
    {
	send_to_char("Disciplines:",ch);
	if (IS_VAMPAFF(victim, VAM_PROTEAN)) send_to_char(" Protean",ch);
	if (IS_VAMPAFF(victim, VAM_CELERITY)) send_to_char(" Celerity",ch);
	if (IS_VAMPAFF(victim, VAM_FORTITUDE)) send_to_char(" Fortitude",ch);
	if (IS_VAMPAFF(victim, VAM_POTENCE)) send_to_char(" Potence",ch);
	if (IS_VAMPAFF(victim, VAM_OBFUSCATE)) send_to_char(" Obfuscate",ch);
	if (IS_VAMPAFF(victim, VAM_OBTENEBRATION)) send_to_char(" Obtenebration",ch);
	if (IS_VAMPAFF(victim, VAM_SERPENTIS)) send_to_char(" Serpentis",ch);
	if (IS_VAMPAFF(victim, VAM_AUSPEX)) send_to_char(" Auspex",ch);
	if (IS_VAMPAFF(victim, VAM_DOMINATE)) send_to_char(" Dominate",ch);
	if (IS_VAMPAFF(victim, VAM_PRESENCE)) send_to_char(" Presence",ch);
        if (IS_VAMPAFF(victim, VAM_VICISSITUDE)) send_to_char(" Vicissitude",ch);
        if (IS_VAMPAFF(victim, VAM_THAU)) send_to_char(" Thaumaturgy",ch);
        if (IS_VAMPAFF(victim, VAM_ANIMAL)) send_to_char(" Animalism",ch);
        if (IS_VAMPAFF(victim, VAM_QUIETUS)) send_to_char(" Quietus",ch);
        send_to_char(".\n\r",ch);
    }
    return;
}
void do_mobius_bm( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

        if (!IS_CLASS(ch, CLASS_MAGE) )
        {
                send_to_char("Huh?\n\r", ch);
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
        act( "$n appears out of thin air.", ch, NULL, NULL, TO_ROOM, FALSE );
        send_to_char( "You appear out of thin air.\n\r", ch );
    }
    else
    {
        act( "$n slowly fades into nothingness.", ch, NULL, NULL, TO_ROOM, FALSE );
        send_to_char( "You fade into nothingness.\n\r", ch );
        ch->mana -= 1000;
        xSET_BIT(ch->act, PLR_HIDE);
    }
    return;
}
void do_fixexp ( CHAR_DATA *ch, char *argument )
{
 if (IS_NPC(ch)) return;

 if (!IS_NPC(ch))
  {
    if (ch->exp > 0)
    {
     send_to_char( "You do not need to fix your exp.\n\r",ch);
     return;
    }
    if (ch->exp < 0)
    {
     ch->exp = 1;
     do_save(ch, "");
     send_to_char( "Your exp has been fixed.\n\r",ch);
     return;
    }
  }
}

void do_email( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
        smash_tilde( argument );
        free_string( ch->pcdata->email );
        ch->pcdata->email = str_dup( argument );
        send_to_char( "Ok.\n\r", ch );
    }
    return;
}




