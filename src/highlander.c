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
#include "monk.h"



void do_safaia( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int dam = ch->damcap[DAM_CAP];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_SET(ch->newbits, NEW_BLADESPIN) )
  {
    stc("You are not a master samurai.\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  
  WAIT_STATE(ch, 12);

  if (!IS_SET(ch->monkcrap, SAM_SAFAIA))
  {  
    stc("You eyes turn a #Lbrilliant blue#n.\n\r",ch);
    act("$n's eyes turn a #Lbrilliant blue#n.",ch,NULL,victim,TO_VICT, FALSE);
    SET_BIT(ch->monkcrap, SAM_SAFAIA);
    return;
  }
  if (IS_SET(ch->monkcrap, SAM_SAFAIA))
  {  
  act("You slide to the ground, spin and kick $N's knee.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n slides to the ground, spins around and kicks your knee!  OUCH!",ch,NULL,victim,TO_VICT, FALSE);
  act("$n slides to the ground, spinds around and kicks $N's knee, causing $s to wince.",ch,NULL,victim,TO_NOTVICT, FALSE);
  damage( ch, victim,dam, gsn_slide);
  }
  return;
}

void do_strike( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  one_argument(argument,arg);

  if ( IS_NPC(ch) ) return;
  if (!IS_CLASS(ch,CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_SET(ch->newbits, NEW_BLADESPIN) )
  {
    stc("You are not a master samurai.\n\r",ch);
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  if ( ch == victim )
  {
    send_to_char( "You cannot hilt-strike yourself!\n\r", ch );
    return;
  }
  if (( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
  {
    stc("You must be wielding a weapon to strike somebody with!\n\r", ch );
    return;
  }

  if (is_safe(ch,victim)) return;

  if ( victim->position < POS_FIGHTING )
  {
    send_to_char( "You can only hilt-strike someone who is standing.\n\r", ch );
    return;
  }

  act("You slam the hilt of your weapon into $N's chest.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n slams $s weapon' hilt into your chest.",ch,NULL,victim,TO_VICT, FALSE);
  act("$n slams $s weapon's hilt into $N's chest.",ch,NULL,victim,TO_NOTVICT, FALSE);

  WAIT_STATE(ch, 11);
  if (IS_NPC(victim) && number_range(1,4) != 1)
  {
    send_to_char( "You were unsuccessful!\n\r",ch);
    return;
  }

  one_hit(ch, victim, gsn_hiltstrike,0);
  if (victim == NULL || victim->position == POS_DEAD) return;
  if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
  act("You fall to the ground stunned!",victim,NULL,NULL,TO_CHAR, FALSE);
  act("$n falls to the ground stunned!",victim,NULL,NULL,TO_ROOM, FALSE);
  victim->position = POS_STUNNED;
  return;
}

void do_shinku( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  int dam = ch->damcap[DAM_CAP];

  if (!IS_CLASS(ch,CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_SET(ch->newbits, NEW_BLADESPIN) )
  {
    stc("You are not a master samurai.\n\r",ch);
    return;
  }

  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

  WAIT_STATE(ch, 11);
  if (!IS_SET(ch->monkcrap, SAM_SAFAIA) && !IS_SET(ch->monkcrap, SAM_SHINKU))
  {
    stc("Your eyes turn a #rdull red#n.\n\r",ch);
    act("$n's eyes turn a #rdull  red#n.",ch,NULL,victim,TO_VICT, FALSE);
    SET_BIT(ch->monkcrap, SAM_SHINKU);
    return;
  }

  if ( IS_SET(ch->monkcrap, SAM_SAFAIA) && !IS_SET(ch->monkcrap, SAM_SHINKU) )
  {
    stc("Your eyes turn a #Rfierce red#n.\n\r",ch);
    act("$n's eyes turn a #Rfierce red#n.",ch,NULL,victim,TO_VICT, FALSE);
    SET_BIT(ch->monkcrap, SAM_SHINKU);
    return;
  }
  if (IS_SET(ch->monkcrap, SAM_SAFAIA) && IS_SET(ch->monkcrap, SAM_SHINKU))
  {
  act("You smash into $N, tackling $s to the ground.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n smashes into $N, tackling $s to the ground.",ch,NULL,victim,TO_VICT, FALSE);
  act("$n smashes into $N tackling $s to the ground.",ch,NULL,victim,TO_NOTVICT, FALSE);
  damage(ch, victim,dam, gsn_shinku);
  damage(ch, victim,dam, gsn_shinku);
  damage(ch, victim,dam, gsn_shinku);
  ch->monkcrap = 0;
  WAIT_STATE(victim,30);
  }
  return;
}

void do_midori( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  int move = 0;

  if (!IS_CLASS(ch,CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_SET(ch->newbits, NEW_BLADESPIN) )
  {
    stc("You are not a master samurai.\n\r",ch);
    return;
  }

  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if ( IS_SET(ch->monkcrap, SAM_SAFAIA) )
  {
    act("You spin around and grab $N by $s neck, choking $s with no remorse.",ch,NULL,victim,TO_CHAR, FALSE);
    act("Your energies seep into $N, leeching some of $s power.",ch,NULL,victim,TO_CHAR, FALSE);
    act("You feel revitalized.",ch,NULL,victim,TO_CHAR, FALSE);
    act("$n sucks some of your life from you!",ch,NULL,victim,TO_VICT, FALSE);
    act("$n hands gripping your throat tighten, causing you to see sparkles!",ch,NULL,victim,TO_VICT, FALSE);
    move = (victim->move / 5);
    victim->move -= move;
    ch->move += number_range(10000,20000);
    if (ch->move > ch->max_move) ch->move = ch->max_move;
    ch->hit += number_range(10000,20000);
    if (ch->hit > ch->max_hit)  ch->hit = ch->max_hit;
    ch->mana += number_range(10000,20000);
    if (ch->mana > ch->max_mana) ch->mana = ch->max_mana;
    ch->monkcrap = 0;
    WAIT_STATE(ch, 18);
    return;
  }
  else
   {
     stc("Your eyes glow #GBright Green#n!\n\r",ch);
     SET_BIT(ch->monkcrap, SAM_MIDORI);
     WAIT_STATE(ch, 12);
   }
  return;
}

void do_shiroi( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  int dam = ch->damcap[DAM_CAP];
  if (!IS_CLASS(ch,CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( !IS_SET(ch->newbits, NEW_BLADESPIN) )
  {
    stc("You are not a master samurai.\n\r",ch);
    return;
  }

  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if (( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
  {
    stc("You must be wielding a weapon to strike somebody with!\n\r", ch );
    return;
  }

  if (!IS_SET(ch->monkcrap, SAM_MIDORI) && !IS_SET(ch->monkcrap, SAM_SHIROI))
  {
    stc("Your eyes turn #7white#n.\n\r",ch);
    SET_BIT(ch->monkcrap, SAM_SHIROI);
    WAIT_STATE(ch, 11);
    return;
  }

  if ( IS_SET(ch->monkcrap, SAM_MIDORI) && !IS_SET(ch->monkcrap, SAM_SHIROI) )
  {
    stc("Your eyes turn a #7billiant white#n.\n\r",ch);
    SET_BIT(ch->monkcrap, SAM_SHIROI);
    WAIT_STATE(ch, 11);
    return;
  }
  if (IS_SET(ch->monkcrap, SAM_SHIROI) && IS_SET(ch->monkcrap, SAM_MIDORI))
  {
  act("You swing your weapon down upon $N, wanting blood to fly.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n swings $s weapon down at you!",ch,NULL,victim,TO_VICT, FALSE);
  act("$n swings $s weapon down at $N, looking crazy while $s does.",ch,NULL,victim,TO_NOTVICT, FALSE);
  damage(ch,victim,dam,gsn_lightningslash);
  damage(ch,victim,dam,gsn_lightningslash);
  damage(ch,victim,dam,gsn_lightningslash);
  damage(ch,victim,dam,gsn_lightningslash);
  damage(ch,victim,dam,gsn_lightningslash);
  ch->monkcrap = 0;
  WAIT_STATE(ch,15);
  return;
  }
  else
   {
   stc("Your eyes return to their natural state.\n\r",ch);
   WAIT_STATE(ch, 11);
   return;
   }
  return;
}



void do_kisetsufuu( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  int dam = ch->damcap[DAM_CAP];
  if (!IS_CLASS(ch,CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }

  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if (( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
  {
    stc("You must be wielding a weapon to strike somebody with!\n\r", ch );
    return;
  }

  WAIT_STATE(ch, 11);

  if ( ch->monkcrap == 0 && !IS_SET(ch->monkcrap, SAM_THRUST1) )
  {
    stc("Your eyes turn #Ppink#n.\n\r",ch);
    act("$n's eyes turn #Ppink#n.",ch,NULL,victim,TO_VICT, FALSE);
    act("$n's eyes turn #Ppink#n.",ch,NULL,victim,TO_NOTVICT, FALSE);
    SET_BIT(ch->monkcrap, SAM_THRUST1);
    damage(ch,victim,dam,gsn_kisetsufuu);
    return;
  }
  if ( IS_SET(ch->monkcrap, SAM_THRUST1) && !IS_SET(ch->monkcrap,SAM_THRUST2) )
  {
    stc("Your eyes turn #yyellow#n.\n\r",ch);
    act("$n's eyes turn #yyellow#n.",ch,NULL,victim,TO_VICT, FALSE);
    act("$n's eyes turn #yyellow#n.",ch,NULL,victim,TO_NOTVICT, FALSE);
    SET_BIT(ch->monkcrap, SAM_THRUST2);
    damage(ch,victim,dam,gsn_kisetsufuu);
    return;
  }
  if (IS_SET(ch->monkcrap, SAM_THRUST1) && IS_SET(ch->monkcrap, SAM_THRUST2))
  {
  act("You scream at $N in an ancient language.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n screams at you, you cannot figure out what the hell $s said.",ch,NULL,victim,TO_VICT, FALSE);
  act("$n screams at $N.  What the hell did $s say?",ch,NULL,victim,TO_NOTVICT, FALSE);
  damage(ch,victim,dam,gsn_kisetsufuu);
  damage(ch,victim,dam,gsn_kisetsufuu);
  if (number_range(1,2)==1)
  damage(ch,victim,dam,gsn_kisetsufuu);
  if (number_range(1,3)==1)
  damage(ch,victim,dam,gsn_kisetsufuu);
  if (number_range(1,4)==1)
  damage(ch,victim,dam,gsn_kisetsufuu);
  if (number_range(1,5)==1)
  damage(ch,victim,dam,gsn_kisetsufuu);
  ch->monkcrap = 0;
  }
  return;
}

void do_kaminari( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  int i;
  int x;
  if (!IS_CLASS(ch,CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }

  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if (( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
  {
    stc("You must be wielding a weapon to strike somebody with!\n\r", ch );
    return;
  }

  WAIT_STATE(ch, 11);
  if (!IS_SET(ch->monkcrap, SAM_THRUST1) || !IS_SET(ch->monkcrap, SAM_THRUST2))
  {
    stc("Nothing happens yet, you are not ready.\n\r",ch);
    return;
  }
  if (IS_SET(ch->monkcrap, SAM_THRUST1) && IS_SET(ch->monkcrap, SAM_THRUST2))
  {
    act("Your eyes do not change as they should, yet your muscles twitch.",ch,NULL,NULL,TO_CHAR, FALSE);
    act("$n's eyes do not change, yet $s looks more deadly.",ch,NULL,victim,TO_VICT, FALSE);
    act("$n's eyes do not change, yet $s looks more deadly.",ch,NULL,victim,TO_NOTVICT, FALSE);
    ch->monkcrap = 0;
    x = ch->wpn[0] / 150;
    for(i =x;i>0;i--)  
    one_hit(ch,victim,gsn_kaminari,0);
    return;
  }
}

void do_inabikari( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  int x;
  int i;
  if (!IS_CLASS(ch,CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if (( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
  {
    stc("You must be wielding a weapon to strike somebody with!\n\r", ch );
    return;
  }

  WAIT_STATE(ch,20);
  if (IS_SET(ch->monkcrap, SAM_SHIROI) && IS_SET(ch->monkcrap, SAM_THRUST1))
  {
    x = ch->lstatus + 5;
    for(i =x;i>0;i--)  
    one_hit(ch,victim,gsn_inabikari,0);
    ch->monkcrap = 0;
    victim->position = POS_STUNNED;
  }
  if (!IS_SET(ch->monkcrap, SAM_SHIROI) || !IS_SET(ch->monkcrap, SAM_THRUST1))
  {
   one_hit(ch,victim,gsn_inabikari,0);
   ch->monkcrap = 0;
  }
  return;
}


void do_bladespin( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if ( !IS_CLASS(ch, CLASS_HIGHLANDER) )
    {
   	stc("Huh?\n\r",ch);
	return;
    } 

     if (ch->wpn[3] < 1000 || ch->wpn[0] < 1000 || ch->wpn[1] < 1000 || ch->wpn[2] < 1000 ||
ch->wpn[4] < 1000 || ch->wpn[5] < 1000 || ch->wpn[6] < 1000 || ch->wpn[7] < 1000 || 
ch->wpn[8] < 1000 || ch->wpn[9] < 1000 || ch->wpn[10] < 1000 || ch->wpn[11] < 1000 || 
ch->wpn[12] < 1000)
    {
      stc("You are not a true samurai yet, get 1000 in all your weapons!\n\r",ch);
        return;
    }          
    if ( IS_SET(ch->newbits, NEW_BLADESPIN) )
    {
        stc("You relax your combat techniques.\n\r",ch);
	act("$n relaxes.",ch,NULL,NULL,TO_ROOM, FALSE);
        REMOVE_BIT(ch->newbits, NEW_BLADESPIN);
	return;
    }
    if ( !IS_SET(ch->newbits, NEW_BLADESPIN) )
    {
        stc("Your concentrate on your combat techniques.\n\r",ch);
	act("$n's muscles flexes, looking for prey that one, better beware!",ch,NULL,NULL,TO_ROOM, FALSE);
	SET_BIT(ch->newbits, NEW_BLADESPIN);
	return;
    }
  return;
}

void do_hologramtransfer( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_HIGHLANDER) )
	{
         send_to_char("Huh?\n\r",ch);
	   return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Walk the ancestrial path to whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not of this world.\n\r", ch );
	return;
    }
    
   if( ch->move < 1000) 
    {
     stc( "The path escapes your senses as you loose control over your inner energies.\n\r",ch);
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
    act("$n steps into the air, leaving behind no trace whatsoever.", ch, NULL, victim, TO_ROOM, FALSE);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    ch->move -= 1000;
    act("$n steps out of the air in front of $N.", ch, NULL, victim, TO_NOTVICT, FALSE);
    act("$n steps out of the air in front of you.", ch, NULL, victim, TO_VICT, FALSE);
    do_look(ch,"auto");
    return;
}
