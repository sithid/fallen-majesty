/***************************************************************************
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
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

#define MAX_SLAY_TYPES 3
#define MONK_AUTODROP 12


/*
 * Local functions.
 */
void  autodrop        args((CHAR_DATA *ch));
void  dropinvis       args((CHAR_DATA *ch, CHAR_DATA *victim));
bool  check_dodge     args((CHAR_DATA *ch, CHAR_DATA *victim, int dt));
void  check_killer    args((CHAR_DATA *ch, CHAR_DATA *victim));
bool  check_parry     args((CHAR_DATA *ch, CHAR_DATA *victim, int dt));
void  dam_message     args((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt));
void  death_cry       args((CHAR_DATA *ch));
void  group_gain      args((CHAR_DATA *ch, CHAR_DATA *victim));
int   xp_compute      args((CHAR_DATA *gch, CHAR_DATA *victim));
void  set_fighting    args((CHAR_DATA *ch, CHAR_DATA *victim));
bool  can_counter     args((CHAR_DATA *ch));
bool  can_bypass      args((CHAR_DATA *ch, CHAR_DATA *victim));
int   number_attacks  args((CHAR_DATA *ch, CHAR_DATA *victim));
int   dambonus        args((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance));
void  update_damcap   args((CHAR_DATA *ch, CHAR_DATA *victim));
void  check_arena     args((CHAR_DATA *ch, CHAR_DATA *victim));

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
  CHAR_DATA *ch;
  CHAR_DATA *ch_next;
  CHAR_DATA *emb;
  CHAR_DATA *victim;
  CHAR_DATA *rch;
  CHAR_DATA *rch_next;
  CHAR_DATA *mount;
  char buf[MSL];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int thecounter = 0;
  int thecounter2 =0; 
  int chance;
  int dam;
  chance = number_percent();
  if (extra_log) log_string("Combat is being updated NOW!");

  for ( ch = char_list; ch != NULL; ch = ch->next )
  {
    if (!IS_NPC(ch)) thecounter++;
    victim = ch->blinkykill;

    if (victim != NULL) {

    if (IS_SET(ch->flag3, AFF3_BLINK_1ST_RD))
    {
      REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
      SET_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
    }
    else if (IS_SET(ch->flag3, AFF3_BLINK_2ND_RD))
    {
      REMOVE_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
      REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
      /* Has the victim left? */
      if (victim->in_room != ch->in_room)
      {
        SET_BIT(ch->affected_by2,EXTRA_BLINKY);
        victim = NULL;
        act("$n pops back into existence.", ch, NULL, NULL, TO_ROOM, FALSE);
        stc("You pop back into existence.\n\r", ch);
        stc("Your victim is no longer in the room.\n\r", ch);
        continue;
      }
      if (!can_see(ch,victim))
      {
	SET_BIT(ch->affected_by2, EXTRA_BLINKY);
	victim = NULL;
        act("$n pops back into existence.", ch, NULL, NULL, TO_ROOM, FALSE);
        stc("You pop back into existence.\n\r", ch);
	stc("You cannot see your victim!\n\r", ch);
        continue;
      }
      
      SET_BIT(ch->affected_by2,EXTRA_BLINKY);
      act("$n pops back into existence next to $N.", ch,NULL,victim, TO_NOTVICT, FALSE);
      act("You pop back into existence next to $N.", ch, NULL,victim, TO_CHAR, FALSE);
      act("$n pops back into existence next to you.", ch, NULL,victim, TO_VICT, FALSE);
      if (!is_safe(ch,victim) && can_see(ch,victim)) {
        if (ch->hit > 0) {
        set_fighting(ch, victim);
        do_say(ch, "OH MAN IS THIS GONNA HURT!");
	if (ch->pcdata->souls > 10)
	{
	   if (number_range(1,2)==1)
	   {
		ch->pcdata->souls -= 1;
		stc("#0You consume #R1 soul#0 in the process, asking for strength from #RLord Satan#0!#n\n\r",ch);
	   }
	}
   
	if (get_eq_char(ch, WEAR_WIELD) != NULL && get_eq_char(ch, WEAR_HOLD) != NULL)
          multi_hit(ch, victim, gsn_blinky);
        if (IS_SET(ch->newbits, THIRD_HAND) && get_eq_char(ch, WEAR_THIRD) != NULL)
          multi_hit(ch, victim, gsn_blinky);
        if (IS_SET(ch->newbits, FOURTH_HAND) && get_eq_char(ch, WEAR_FOURTH) != NULL)
          multi_hit(ch, victim, gsn_blinky);
        update_pos(victim);
        }
      }
    }
    } /*end of blink*/
    ch_next = ch->next;

    if (ch->fury_timer > 0)
    {
	ch->fury_timer--;
	if (ch->fury_timer == 0)
	{
	REMOVE_BIT(ch->special, SPC_FURY);
	stc("#0Your #7wrath#0 returns to normal, you feel less #7furious.#n\n\r",ch);
	}
    }

    if (ch->fury_timer == 0 && IS_SET(ch->special, SPC_FURY))
        REMOVE_BIT(ch->special, SPC_FURY);

    if (ch->fight_timer > 0) 
    {
    if (ch->fight_timer > 200)
    ch->fight_timer = 200;
    ch->fight_timer--;
	if (ch->fight_timer == 0)
	{
	sprintf(buf,"%s now has NO Fight Timer.",ch->pcdata->switchname);
	log_string(buf);
	sprintf(buf,"#R%s #0now has #yNO#R Fight Timer#0.#n",ch->pcdata->switchname);
	do_update(NULL,buf);
	}
    }
    if (!IS_NPC(ch))
    if (ch->pcdata->exhaustion > 0)
    {
	ch->pcdata->exhaustion--;
	if (ch->pcdata->exhaustion == 0)
	{
	  if (IS_CLASS(ch, CLASS_DEMON))
		sprintf(buf,"You may now use the entomb ability again.\n\r");
	  if (IS_CLASS(ch, CLASS_PSIONIST))
		sprintf(buf,"You may now use the entrapment ability again.\n\r");
	  stc(buf,ch);
	}
    }
    if (ch->wish_timer > 0)
    {
    ch->wish_timer--;
        if (ch->wish_timer == 0)
        {
	sprintf(buf,"You have recovered from your last drink from your last wish.\n\r");
	stc(buf,ch);
        }
    }
    if (ch->quit_timer > 0)
    {
    ch->quit_timer--;
        if (ch->quit_timer == 0)
        {
        sprintf(buf,"You are now able to quit.\n\r");
        stc(buf,ch);
        }
    }

    if (ch->bomb_timer > 0)
    {
        ch->bomb_timer--;
        if (ch->bomb_timer == 0)
	{
	if (IS_CLASS(ch, CLASS_ANGEL))
	stc("You may now summon the Creator's Divine Wrath!\n\r",ch);
	if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
	stc("You may now blink like faerie you are.\n\r",ch);
	if (IS_CLASS(ch, CLASS_MAGE))
        stc("You can #rb#Rom#rb#n again.\n\r", ch);
	if (IS_CLASS(ch, CLASS_INQUISITOR))
	stc("#7You can summon the #0unholy fury#7 once again#n.\n\r",ch);
	if (IS_CLASS(ch, CLASS_PALADIN))
	stc("#7You may now bladeslash your enemies once again!#n\n\r",ch);
	}
    }
   if (!IS_NPC(ch))
   { 
     if (ch->fighting == NULL && !IS_HEAD(ch,LOST_HEAD) && !IS_EXTRA(ch,EXTRA_OSWITCH) && 
!IS_SET(ch->monkstuff, MONK_DEATH) && !IS_SET(ch->special, SPC_TORTURE) && 
!IS_AFFECTED(ch, AFF_ETHEREAL) && !xIS_SET(ch->act, PLR_HIDE) && !xIS_SET(ch->act, PLR_WIZINVIS)
&& !IS_SET(ch->newbits, NEW_DARKNESS) && !IS_AFFECTED(ch, AFF_PEACE) && !IS_AFFECTED(ch,AFF_SHADOWPLANE)
)
     {
      if (ch->in_room != NULL)
      {
   if (IS_SET(ch->in_room->room_flags, ROOM_HEALING))
    {
    if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move)    
    {
     if (ch->fight_timer > 0)
      {
	 if (ch->hit < 0 || ch->mana < 0 || ch->move < 0)
         {
          ch->hit += 1;
          ch->mana += 1;
          ch->move += 1;
         }
         else
	 {
          ch->hit += 2500;
          ch->mana += 2500;
          ch->move += 2500;
	 }
      }
     else
      {
         if (ch->hit < 0 || ch->mana < 0 || ch->move < 0)
         {
          ch->hit += 1;
          ch->mana += 1;
          ch->move += 1;
         }
         else
         {
          ch->hit += 5000;
          ch->mana += 5000;
          ch->move += 5000;
         }
      }
     if (ch->hit > ch->max_hit)  ch->hit = ch->max_hit;
     if (ch->mana > ch->max_mana) ch->mana = ch->max_mana;
     if (ch->move > ch->max_move) ch->move = ch->max_move;
     stc("#7The serenity of the room heals your wounds.\n\r",ch);
     update_pos(ch);
    }
    }
   if (IS_SET(ch->in_room->room_flags, ROOM_DAMAGE))
    {
     if (!IS_IMMORTAL(ch))
     {
       dam = number_range(2000,5000);
       if (ch->hit <= ch->max_hit)
       ch->hit -= dam;
       if (ch->mana <= ch->max_mana)
       ch->mana -= dam;
       if (ch->move <= ch->max_move)
       ch->move -= dam;
       if (ch->hit < -10)  ch->hit = -10;
       if (ch->mana < -10) ch->mana = -10;
       if (ch->move < -10) ch->move = -10;
       stc("#0There is an evil presense here, sapping you strength.\n\r",ch);
       update_pos(ch);
     }
    }
   }
 }
}

    if (ch->ffa_timer > 0)
    {
	ch->ffa_timer--;
	if (ch->ffa_timer == 0)
	{
	 stc("#0You may now enter the #RFREE #yFOR #RALL#0 again!\n\r",ch);
	}
    }

    if (ch->ffa_timer == 0 && xIS_SET(ch->extra, FFA_HUNG))
          xREMOVE_BIT(ch->extra, FFA_HUNG);

    if ( ch->embracing != NULL )
    {
      emb = ch->embracing;
      if ( emb == NULL ) ch->embracing = NULL;
      if ( ch->in_room != emb->in_room )
      {
        if ( emb->embraced != NULL )
        {
          emb->embraced = NULL;
          ch->embracing = NULL;
        }
        else ch->embracing = NULL;
      }
    }
    if ( ch->embraced != NULL )
    {
      emb = ch->embraced;
      if ( emb == NULL ) ch->embraced = NULL;  
      if ( ch->in_room != emb->in_room )
      {
        if ( emb->embracing != NULL )
        {
          emb->embracing = NULL;
          ch->embraced   = NULL;
        }
        else ch->embraced = NULL;
      }
    }
   if ( IS_SET(ch->special, SPC_TORTURE) )
    {  
      if ( ch->hit > (ch->max_hit / 3 ) )
      {
	if (number_range(1,4) == 1)
        {
          stc("You feel relieved as the torture ends.\n\r",ch);
          act("$sn sighs in relief.",ch,NULL,NULL,TO_ROOM, FALSE);
          REMOVE_BIT(ch->special, SPC_TORTURE);
        }
	else 
	{
        if (ch->hit < 0) return;
	ch->hit -= number_range( 2500, 7500);
        stc("You scream like a girl as you are viciously tortured!\n\r",ch);
        act("$n screams like a girl as $s body is mutilated.",ch,NULL,NULL,TO_ROOM, FALSE);
        }

      }
      else
      {
        if ( number_range(1,2) == 1 )
        {
          stc("You feel relieved as the torture ends.\n\r",ch);
          act("$sn sighs in relief.",ch,NULL,NULL,TO_ROOM, FALSE);
          REMOVE_BIT(ch->special, SPC_TORTURE);
        }
        else
        {
        if (ch->hit < 0) return;
		ch->hit -= number_range( 2500, 7500);
        stc("You scream like a girl as you are viciously tortured!\n\r",ch);
        act("$n screams like a girl as $s body is mutilated.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
      }
    }
   if ( IS_SET(ch->monkstuff, MONK_DEATH) )
    {  
      if ( ch->hit > (ch->max_hit / 3 ) )
      {
        if (number_range(1,20) != 1)
        {
          if (ch->hit < 0) return;
	  ch->hit -= number_range( 2500, 7500);
          stc("You writhe in agony as magical energies tear you asunder.\n\r",ch);
          act("$n writhes in agony as magical forces tear apart $s body.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
        else
        {
          stc("You feel the magical forces leave your body.\n\r",ch);
          act("The magical forces leave $n's body.",ch,NULL,NULL,TO_ROOM, FALSE);
          REMOVE_BIT(ch->monkstuff, MONK_DEATH);
        }
      }	
      else
      {
        if ( number_range(1,5) == 1 )
        {
          stc("You feel the magical forces leave your body.\n\r",ch);
          act("The magical forces leave $n's body.",ch,NULL,NULL,TO_ROOM, FALSE);
          REMOVE_BIT(ch->monkstuff, MONK_DEATH);
        }
        else
        {
	 if (ch->hit < 0) return;
          ch->hit -= number_range( 2500, 7500);
          stc("Your writhe in agony as magical energies tear you asunder.\n\r",ch);
          act("$n writhes in agony as magical forces tear apart $s body.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
      }
    }
    if (IS_SET(ch->monkstuff, MONK_HEAL))
    {
      int monkheal = 0;
      if ((ch->hit < (ch->max_hit * 0.75 )) && (ch->hit > 0))
      {
        stc("Your body emits glowing sparks.\n\r",ch);
        act("$n's body emits glowing sparks and fizzes.",ch,NULL,NULL,TO_ROOM, FALSE);
        monkheal +=  number_range( 2500,7500 );
      }
      else if (ch->hit < 0)
      {
        stc("Your body emits glowing sparks.\n\r",ch);
        act("$n's body emits glowing sparks and fizzes.",ch,NULL,NULL,TO_ROOM, FALSE);
        monkheal +=  number_range( 0,1 );
      }
      else if ((ch->hit) >= (ch->max_hit * 0.75))
      {  
        if ( number_range( 1,5 ) == 1 )
        {
           stc("The sparks fizzle and die.\n\r",ch);
           act("The sparks around $n's body fizzle and die.",ch,NULL,NULL,TO_ROOM, FALSE);
           REMOVE_BIT(ch->monkstuff, MONK_HEAL);
        }
        else
        {
          if ( ch->hit < ch->max_hit )
	  {
          monkheal +=  number_range( 2500,7500 );
	  stc("Your body emits glowing sparks.\n\r",ch);
          act("$n's body emits glowing sparks and fizzes.",ch,NULL,NULL,TO_ROOM, FALSE);
	  }
        }
      }
     if (!IS_AFFECTED(ch, AFF_FLAMING))
     {
      ch->hit += monkheal;
      update_pos(ch);
     }
     if (IS_AFFECTED(ch, AFF_FLAMING))
     {
      monkheal /= 2;
      ch->hit += monkheal;
      update_pos(ch);
     }
    }
    if ((victim = ch->fighting ) == NULL || ch->in_room == NULL) continue;
    if (!IS_NPC(ch) && !IS_NPC(victim) && !is_safe(ch, victim) 
&&!is_safe(victim,ch))
    {
      if (!IS_SET(ch->in_room->room_flags, ROOM_ARENA) || 
!IS_SET(victim->in_room->room_flags, ROOM_ARENA))
      {
       if (ch->fight_timer < 10) ch->fight_timer = 10;
       if (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_POWER_LICH) )
       ch->fight_timer += 1;
       else if (!IS_CLASS(ch, CLASS_MAGE) || !IS_CLASS(ch, CLASS_POWER_LICH))
       ch->fight_timer += 3;
      }
    }  
    if (IS_AWAKE(ch) && IS_AWAKE(victim) && ch->in_room == victim->in_room) 
    {
       multi_hit( ch, victim, TYPE_UNDEFINED );
       if (!IS_NPC(ch)) 
          if(ch->desc)
	     process_output(ch->desc, TRUE);
    }
    else stop_fighting(ch, FALSE);
    if ((victim = ch->fighting ) == NULL) continue;
    /*   
     * Fun for the whole family!
     */
    for ( rch = ch->in_room->people; rch != NULL; rch = rch_next )
    {
      rch_next = rch->next_in_room;
      if ( IS_AWAKE(rch) && rch->fighting == NULL )
      {
      /*
       * Mount's auto-assist their riders and vice versa.
       */
        if ( (mount = rch->mount) != NULL )
        {
          if (mount == ch) multi_hit( rch, victim, TYPE_UNDEFINED );
          continue;
        }
        /*
         * PC's auto-assist others in their group.
         */
        if ( !IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) )
        {
          if ((!IS_NPC(rch) || IS_AFFECTED(rch, AFF_CHARM) ) && is_same_group(ch, rch))
            multi_hit( rch, victim, TYPE_UNDEFINED );
          continue;
        }
        /*
         * NPC's assist NPC's of same type or 12.5% chance regardless.
         */
        if ( IS_NPC(rch) && !IS_AFFECTED(rch, AFF_CHARM) )
        {   
          if (rch->pIndexData == ch->pIndexData || number_bits(3) == 0)
          {
            CHAR_DATA *vch;
            CHAR_DATA *target;
            int number;
          
            target = NULL;
            number = 0;
            for ( vch = ch->in_room->people; vch; vch = vch->next )
            {
              if (can_see(rch,vch) && is_same_group(vch, victim) && number_range(0,number)==0)
              {
                target = vch;  
                number++;
              }
            }
            if ( target != NULL )
            multi_hit( rch, target, TYPE_UNDEFINED );
          }
        }
      }
    }
    if (!IS_NPC(ch)) thecounter2++;
  }
  sprintf(buf1,"amount of players started update %d", thecounter);
  sprintf(buf2,"amount of players ended update %d", thecounter2);
  if (extra_log) log_string(buf1); 
  if (extra_log) log_string(buf2);
  return;
}


/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
  OBJ_DATA *wield;
  OBJ_DATA *wield1;
  OBJ_DATA *wield2;
  OBJ_DATA *wield3;
  OBJ_DATA *wield4;
  char buf[MAX_STRING_LENGTH];
  int sn, level, chance, unarmed, maxcount,countup,x,wieldie,i;
  AFFECT_DATA af;
  int wieldorig = 0;
  int wieldtype = 0;
  int dam;
  if ( ch->position < POS_SLEEPING ) return;

      if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_PEACE) && (ch->fight_timer<1))
      {
        send_to_char( "You can't seem to attack them.\n\r", ch );
        return;
      }
   if (!IS_NPC(victim))
   {
      if (is_fighting_player(victim, ch))
      {
         stc("They're already fighting another player.\n\r", ch);
         return;
      }
  }

  if ( IS_CREATOR(ch) || IS_CLASS(ch, CLASS_MONK) )
  {
    if (!IS_NPC(ch) && wieldorig == 0 )
    {
      if (countup == 0) unarmed = number_range(0,3);
      else if (countup == 1) unarmed = number_range(4,7);
      else unarmed = number_range(0,7);
      if (!IS_NPC(ch) && ch->cmbt[unarmed] != 0 && wieldorig == 0)
        fightaction(ch,victim,ch->cmbt[unarmed], dt, wieldtype);
    } 
  }
  if ( !IS_NPC( ch ) && dt == gsn_headbutt )
  {
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && ch->pcdata->powers[BULL_LEVEL]>1);
    one_hit( ch, victim, dt, 0 );
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && ch->pcdata->powers[BULL_LEVEL]>4);
    {
    one_hit( ch, victim, dt, 0 );
    one_hit( ch, victim, dt, 0 );
    }
    return;
  }
  if ( dt == gsn_hooves )
  {
    one_hit( ch, victim, dt, 0 );
    one_hit( ch, victim, dt, 0 );
    return;
  }
  if ( !IS_NPC( ch ) && dt == gsn_shiroken)
  {
    for(i = number_range(2,4);i>0;i--)
      one_hit(ch, victim, dt, 1);
      if (number_range(1,10) >= 9)
      one_hit(ch, victim, dt, 1);
    if(ch->pcdata->powers[NPOWER_NINGENNO] >=4)
      spell_poison(gsn_poison,(ch->level*number_range(50,60)),ch,victim); 
    return;
  }
  if ( dt == gsn_laser )
  {
    one_hit(ch,victim,dt,2);
	if (number_range(1,2) == 1)
    one_hit(ch,victim,dt,1);
        if (number_range(1,3) == 1)
    one_hit(ch,victim,dt,0);
    return;
  }
  if ( dt == gsn_stinger )
  {
    one_hit(ch,victim,dt,0);
    one_hit(ch,victim,dt,2);
        if (number_range(1,3) == 1)
    one_hit(ch,victim,dt,1);
    return;
  }
  if (dt == gsn_claws)
  {
  int x;
  int i;
  if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && (ch->pcdata->powers[SHAPE_FORM] != TIGER_FORM))
  x = number_range(1,2);
  if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM))
  x = number_range(1,3);  
  else x = 2;
  for (i = 0;i < x;i++)
  one_hit( ch, victim, dt, 1 );
  return;
  }
  if ( dt == gsn_fangs )
  {
  int x;
  int i = 0;
  if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && (ch->pcdata->powers[SHAPE_FORM] != TIGER_FORM))
  x = number_range(1,2);
  if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM))
  x = number_range(1,3);
  if ( IS_CLASS(ch, CLASS_DEMON))
  x = number_range(1,2);
  if ( IS_CLASS(ch, CLASS_VAMPIRE))
  x = number_range(2,3);
  else x = 1;
  for (i = 0;i < x;i++)
  one_hit( ch, victim, dt, 1 );
  return;
  } 
  if ( dt== gsn_quills)
  {
    if ( (IS_CLASS(ch,CLASS_WEREWOLF)) && (ch->power[DISC_WERE_PAIN] > 9))
    {
      one_hit(ch,victim,dt,1);
      one_hit(ch,victim,dt,2);
      if (number_range(1,3)==1)
      one_hit(ch,victim,dt,0);
	  	  	}
        return;
  }
  if (dt == gsn_darktendrils && IS_CLASS(ch, CLASS_DROW))
  {
        if (number_range(1,2)==1)
    one_hit(ch,victim,dt,0);
        if (number_range(1,2)==1)
    one_hit(ch,victim,dt,0);
    one_hit(ch,victim,dt,0);
    one_hit(ch,victim,dt,0);
   return;
  }
  if ( dt==gsn_venomtong)
  {
    one_hit(ch,victim,dt,0);
    return;
  }
 
  if ( dt==gsn_spiketail)
  {
    one_hit(ch,victim,dt,0);
        if (number_range(1,2)==1)
    one_hit(ch,victim,dt,0);
    return;
  }  
  if ( dt==gsn_badbreath)
  {
  int x;
  int i;
  if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
  x = number_range(1,2);
  if ( IS_CLASS(ch, CLASS_DEMON))
  x = number_range(1,2);
  for (i = 0;i < x;i++)
  one_hit(ch,victim,dt,0);
    return;
  }  
  if ( dt==gsn_magma)
  {
    one_hit(ch,victim,dt,0);
    return;
  }  
  if ( dt==gsn_shards)
  {
    one_hit(ch,victim,dt,0);
    one_hit(ch,victim,dt,0);
    return;
  }  
  if ( dt==gsn_cheapshot)
  {
    send_to_char("You stun them with a shoulder charge!\n\r",ch);
    send_to_char("You are stunned by a shoulder charge!\n\r",victim);
    one_hit(ch,victim,dt,0);    
    one_hit(ch,victim,dt,1);
    if (number_range(1,3)==1)
    one_hit(ch,victim,dt,1);
	    return;
  } 
  if ( dt == gsn_divinewrath )
  {
    int x;
    if (IS_CLASS(ch, CLASS_ANGEL) && ch->pcdata->powers[ANGEL_JUSTICE] > 4)
      x = number_range(2,4);
    else
      x = number_range(1,3);
    for (i=0; i < x; i++)
    one_hit( ch, victim, dt, 0 );
    return;
  }

  if ( dt == gsn_buffet )
  {
    int x;
    if (IS_CLASS(ch, CLASS_ANGEL))
      x = number_range(2,3);
    if (IS_CLASS(ch, CLASS_VAMPIRE))
      x = number_range(1,2);
    else
      x = number_range(1,3);
    for (i=0; i < x; i++)
    one_hit( ch, victim, dt, 0 );
    return;
  } 
  if ( dt == gsn_sweep )
  {
    one_hit( ch, victim, dt, 0 );
    return;
  }
  if ( dt == gsn_rfangs )
  {
    one_hit( ch, victim, dt, 0);
    return;
  }
  if (dt==gsn_heavenlyaura)
  {
    if (IS_CLASS(ch, CLASS_MONK)) x = number_range(1,3);
    else
    {
       x = number_range(2,3);
    }
    for (i=0; i < x; i++)
      one_hit(ch,victim,dt,1);
    return;
  }
  if (dt==gsn_mageshield)
  {
    if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
    {
      one_hit(ch,victim,dt,1);
      one_hit(ch,victim,dt,1);
      if (IS_SET(ch->newbits, NEW_SUPREME))
	{
	one_hit(ch,victim,dt,1);
	one_hit(ch,victim,dt,1);
	one_hit(ch,victim,dt,1);
	}
    }
    return;
  }
  if (dt==gsn_shadowblast)
  {
    one_hit(ch,victim,dt,1);
    one_hit(ch,victim,dt,1);
    one_hit(ch,victim,dt,1);
    return;
  }
  if ( dt == gsn_inquisitor)
   {
      one_hit(ch,victim,gsn_purify,0);
      if (number_range(1,2)==1) one_hit(ch,victim,gsn_purify,0);
      if (number_range(1,2)==1) one_hit(ch,victim,gsn_unholytorment,0);
      if (number_range(1,2)==1) one_hit(ch,victim,gsn_unholytorment,0);
      if ((ch->wpn[0] >= 700) && (ch->wpn[3] >= 700)
      && (ch->wpn[6] >= 700) && (ch->wpn[9] >= 700)
      && (ch->wpn[1] >= 700) && (ch->wpn[4] >= 700)
      && (ch->wpn[7] >= 700) && (ch->wpn[10] >= 700) 
      && (ch->wpn[2] >= 700) && (ch->wpn[5] >= 700)
      && (ch->wpn[8] >= 700) && (ch->wpn[11] >= 700)
      && (ch->wpn[12] >= 700))
      {
        if (number_range(1,3) <= 2) one_hit(ch,victim,gsn_unholytorment,0);
        if (number_range(1,3) <= 2) one_hit(ch,victim,gsn_unholytorment,0);
        if (number_range(1,3) <= 2) one_hit(ch,victim,gsn_purify,0);
        if (number_range(1,3) <= 2) one_hit(ch,victim,gsn_purify,0);
      }
    return;
  }

  if ( dt == gsn_bladespin)
  {
    if (number_range(1,2)==1) one_hit(ch,victim,dt,2);
    if (number_range(1,3)==1) one_hit(ch,victim,gsn_lightningkick,2);
    if (number_range(1,3) >= 2) one_hit(ch,victim,gsn_knee,2);
    if (number_range(1,3) >= 2) one_hit(ch,victim,gsn_lightningslash,2);
      if ((ch->wpn[0] >= 1000)  && (ch->wpn[3] >= 1000)
      && (ch->wpn[6] >= 1000) && (ch->wpn[9] >= 1000)
      && (ch->wpn[1] >= 1000) && (ch->wpn[4] >= 1000)
      && (ch->wpn[7] >= 1000)
      && (ch->wpn[10] >= 1000) && (ch->wpn[2] >= 1000)
      && (ch->wpn[5] >= 1000) && (ch->wpn[8] >= 1000)
      && (ch->wpn[11] >= 1000) && (ch->wpn[12] >= 1000))
      {
      if (number_range(1,4) >= 2) one_hit(ch,victim,gsn_lightningslash,2);
      if (number_range(1,4) >= 2) one_hit(ch,victim,gsn_lightningkick,2);
      if (number_range(1,4) >= 2) one_hit(ch,victim,gsn_knee,2);
      if (number_range(1,4) >= 3) one_hit(ch,victim,gsn_lightningslash,2);
      if (number_range(1,4) >= 3) one_hit(ch,victim,gsn_knee,2);
      }
   return;
   }
   if (dt == gsn_riposte)
   {
	one_hit(ch,victim,gsn_riposte,0);
        if (number_range(1,2)==1)
    one_hit(ch,victim,gsn_riposte,0);
    if (number_range(1,2)==1) one_hit(ch,victim,gsn_feint,0);
    if (number_range(1,2)==1) one_hit(ch,victim,gsn_feint,0);
      if ((ch->wpn[0] >= 700)  && (ch->wpn[3] >= 700)
      && (ch->wpn[6] >= 700) && (ch->wpn[9] >= 700)
      && (ch->wpn[1] >= 700) && (ch->wpn[4] >= 700)
      && (ch->wpn[7] >= 700)
      && (ch->wpn[10] >= 700) && (ch->wpn[2] >= 700)
      && (ch->wpn[5] >= 700) && (ch->wpn[8] >= 700)
      && (ch->wpn[11] >= 700) && (ch->wpn[12] >= 700))
      {
      if (number_range(1,3) <= 2) one_hit(ch,victim,gsn_riposte,0);
      if (number_range(1,3) <= 2) one_hit(ch,victim,gsn_riposte,0);
      if (number_range(1,3) <= 2) one_hit(ch,victim,gsn_feint,0);
      if (number_range(1,3) <= 2) one_hit(ch,victim,gsn_feint,0);
      }
    return;
  }
  if(IS_NPC(ch))
  {
    switch( ch->pIndexData->vnum ){
    case 30003:
    case 20050:
    case 30704:
    case 30703:
    case 10000:
    case 10001:
    case 10002:
    case 10003:
    case 10004:
    break;
    default:
    {
      if (!IS_NPC(victim) && IS_AFFECTED(victim, AFF_PEACE))
      {
        send_to_char( "You can't seem to attack them.\n\r", ch );
        return;
      }
    }
    }
  }
  wield1 = get_eq_char( ch, WEAR_WIELD );
  wield2 = get_eq_char( ch, WEAR_HOLD );
  wield3 = get_eq_char( ch, WEAR_THIRD );
  wield4 = get_eq_char( ch, WEAR_FOURTH);
  if ( wield1 != NULL && wield1->item_type == ITEM_WEAPON ) wieldorig  = 1;
  if ( wield2 != NULL && wield2->item_type == ITEM_WEAPON ) wieldorig += 2;
  if ( wield3 != NULL && wield3->item_type == ITEM_WEAPON ) wieldorig += 4;
  if ( wield4 != NULL && wield4->item_type == ITEM_WEAPON ) wieldorig += 8;
  wieldtype = wieldorig;
  wieldie = number_range(1,4);
  if ( wieldorig == 15 ) 
  { 
    if (wieldie == 2) wieldtype = 8; 
    else wieldtype = 4;
  }
  if ( wieldorig >= 14 )
  {
    if (wieldie == 2) wieldtype = 8;
    else if (wieldie == 1) wieldtype = 4;
    else if (wieldie == 3) wieldtype = 2;
    else wieldtype = 1;
  }
  if ( wieldorig == 13 )
  {
    if ( wieldie == 1) wieldtype = 8;
    else if (wieldie == 2) wieldtype = 4;
    else wieldtype = 1;    
  }
  if ( wieldorig == 12 )
  {
    if ( wieldie == 1 ) wieldtype = 8;
    else wieldtype = 4;
  }
  if ( wieldorig == 11 )
  {
    if ( wieldie == 1 ) wieldtype = 8;
    if ( wieldie == 2 ) wieldtype = 2;
    else wieldtype = 1;
  }
  if ( wieldorig == 10 )
  {  
    if ( wieldie == 1 ) wieldtype = 8;
    else wieldtype = 2;    
  }
  if ( wieldorig == 9 )
  {
    if ( wieldie == 1 ) wieldtype = 8;
    else wieldtype = 1;
  }
  if ( wieldorig == 8 ) wieldtype = 8;
  if ( wieldorig == 7 )
  {
    if ( wieldie == 1 ) wieldtype = 4;
    else if ( wieldie == 2 ) wieldtype = 2;
    else wieldtype = 1;
  }
  if ( wieldorig == 6 )
  {
    if ( wieldie == 1 || wieldie == 2) wieldtype = 2;
    else wieldtype = 4;
  } 
  if ( wieldorig == 5 )
  {
    if ( wieldie == 1) wieldtype = 4;
    else wieldtype = 1;
  }
  if ( wieldorig == 4 ) wieldtype = 4;
  if ( wieldorig == 3 )    
  {
    if ( wieldie == 2 || wieldie == 4) wieldtype = 2;
    else wieldtype = 1;
  }
  if ( wieldorig == 2 ) wieldtype = 2;
  if ( wieldorig == 1 ) wieldtype = 1;

  if ( wieldtype == 8 ) wield = wield4;
  else if ( wieldtype == 4 ) wield = wield3;
  else if ( wieldtype == 2 ) wield = wield2;
  else wield = wield1;

  if (!IS_NPC(ch) && ch->stance[0] > 0 && number_percent() == 5)
  {
    int stance = ch->stance[0];
    if (ch->stance[stance] >= 200)
    {
      special_move(ch,victim);
      return;
    }    
  }

  unarmed = 0;
  if (IS_SET( ch->flag2, VAMP_OBJMASK) )
  {
    act("You return to your normal form.",ch,NULL,NULL,TO_CHAR, FALSE);
    sprintf(buf,"%s reforms as %s.\n\r",ch->morph, ch->name );
    act(buf,ch,NULL,NULL,TO_ROOM, FALSE);
    free_string(ch->morph);
    free_string(ch->objdesc);
    ch->long_descr = str_dup("");
    REMOVE_BIT(ch->flag2, VAMP_OBJMASK);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
  }
  one_hit( ch, victim, dt, wieldtype );

  if ( victim == NULL || victim->position != POS_FIGHTING ) return;

  /* Only want one spell per round from spell weapons...otherwise it's 
   * too powerful, and would unbalance player killing (as this is a PK mud).
   */

  if ( dt == TYPE_UNDEFINED )
  {
    dt = TYPE_HIT;
    if ( wield != NULL && wield->item_type == ITEM_WEAPON )
    {
      dt += wield->value[3];
      if (wield->value[0] >= 1)
      {
        if ( IS_ITEMAFF(ch, ITEMA_CRITICAL ) && number_range(1,4)==1 )
	{
		dam = ch->damcap[0];
	  damage( ch, victim, dam, gsn_critical_hit );
	}
        /* Look, just don't ask...   KaVir */
        if (wield->value[0] >= 1000) sn = wield->value[0] - ((wield->value[0] / 1000) * 1000);
        else sn = wield->value[0];
        if ((sn != 0 && sn !=70 && sn !=158 && sn !=61)  && (victim->position == POS_FIGHTING) && (!IS_ITEMAFF(victim, ITEMA_REFLECT) && (ch != victim))) (*skill_table[sn].spell_fun)
          (sn,wield->level,ch,victim);
        /* sn.70 is gas breath and sn.158 is desanct and sn.61 is sleep */
      }
    }
  }
  if ( victim == NULL || victim->position != POS_FIGHTING ) return;
  if ( ch->fighting != victim || dt == gsn_backstab || dt == gsn_shiroken
    || dt == gsn_garotte || dt == gsn_headbutt || dt == gsn_circle ) return;
  maxcount = number_attacks(ch, victim);
  if (!IS_NPC(ch))
  {
    chance = 0;
    if (wield != NULL && wield->item_type == ITEM_WEAPON)
    {
      int tempnum = wield->value[3];
      chance = (ch->wpn[tempnum]) * 0.5;
    }
    else chance = (ch->wpn[0]) * 0.5;
    if (number_percent() <= chance) maxcount += 1;
  }
  if (wieldorig == 3) maxcount += 1;
  for (countup = 0; countup <= maxcount; countup++)
  {
    wieldtype = wieldorig;
    if ( wieldorig == 15 ) 
    { 
      if (wieldie == 2)  wieldtype = 8; 
      else wieldtype = 4;
    }
    if ( wieldorig == 14 )
    {
      if (wieldie == 2) wieldtype = 8;
      else if (wieldie == 1) wieldtype = 4;
      else if (wieldie == 3) wieldtype = 2;
      else wieldtype = 1;
    }
    if ( wieldorig == 13 )
    {
      if ( wieldie == 1) wieldtype = 8;
      else if (wieldie == 2) wieldtype = 4;
      else wieldtype = 1;    
    }
    if ( wieldorig == 12 )
    {
      if ( wieldie == 1 ) wieldtype = 8;
      else wieldtype = 4;
    }
    if ( wieldorig == 11 )
    {
      if ( wieldie == 1 ) wieldtype = 8;
      if ( wieldie == 2 ) wieldtype = 2;
      else wieldtype = 1;
    }
    if ( wieldorig == 10 )
    {
      if ( wieldie == 1 ) wieldtype = 8;
      else wieldtype = 2;
    }
    if ( wieldorig == 9 )
    {
      if ( wieldie == 1 ) wieldtype = 8;
      else wieldtype = 1;
    }
    if ( wieldorig == 7 )
    {
      if ( wieldie == 1 ) wieldtype = 4;
      else if ( wieldie == 2 ) wieldtype = 2;
      else wieldtype = 1;
    }
    if ( wieldorig == 6 )
    {
      if ( wieldie == 1 || wieldie == 2) wieldtype = 2;
      else wieldtype = 4;
    } 
    if ( wieldorig == 5 )
    {
      if ( wieldie == 1) wieldtype = 4;
      else wieldtype = 1;
    }
    if ( wieldorig == 3 )
    {
      if ( wieldie == 2 || wieldie == 4) wieldtype = 2;
      else wieldtype = 1;
    }
    one_hit( ch, victim, -1, wieldtype );
    if ( victim == NULL || victim->position != POS_FIGHTING ) return;
    if ( ch->fighting != victim ) return;
  }
  if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FANGS))
  {
     if (!IS_NPC(victim)) {
       one_hit(ch,victim, (TYPE_HIT + 10),0);
       one_hit( ch, victim, ( TYPE_HIT + 10 ), 0 );
     }
     else {
       one_hit(ch,victim, (TYPE_HIT + 10),1);
       one_hit( ch, victim, ( TYPE_HIT + 10 ), 1 );
     }
  }
  if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_SPIKETAIL) && number_range(1,4) == 1)
  {
    if (IS_NPC(victim)) {
      one_hit( ch, victim, gsn_spiket, 0);
  }
    else one_hit( ch, victim, gsn_spiket, 0);
  }
  /* Golems speciels */
  if (IS_NPC(ch))
  {
    if (ch->pIndexData->vnum == MOB_VNUM_FIRE)
    {
      if ((sn = skill_lookup( "curse" ) ) > 0)
        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
      if ((sn = skill_lookup( "imp faerie fire" ) ) > 0)
        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
    }
    if (ch->pIndexData->vnum == MOB_VNUM_STONE)
    {
      af.type      = skill_lookup("reserved");  
      af.duration  = 20;
      af.location  = APPLY_DAMROLL;
      af.modifier  = -50;
      af.bitvector = 0;
      affect_to_char(victim, &af );
    
      af.type      = skill_lookup("reserved");
      af.duration  = 20;
      af.location  = APPLY_HITROLL;
      af.modifier  = -50;
      af.bitvector = 0;
      affect_to_char(victim, &af );
      send_to_char("You feel weak in the presence of the stone golem.\n\r",victim);
    }
    if (ch->pIndexData->vnum == MOB_VNUM_CLAY)
    {
      if ((sn = skill_lookup( "clay" ) ) > 0)
        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
    }
    if (ch->pIndexData->vnum == MOB_VNUM_IRON)
    {
      if ((sn = skill_lookup( "group heal" ) ) > 0)
        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
    }
  }
  if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_SHARDS) && number_range(1,4) == 1)
    one_hit( ch, victim, gsn_shards, 0);
  if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_MAGMA) && number_range(1,4) == 1)
  {
    if (IS_NPC(victim)) {
      one_hit( ch, victim, gsn_magma, 0);
      one_hit( ch, victim, gsn_magma, 1);
    }
    else if (IS_CLASS(victim, CLASS_VAMPIRE))
    {
      one_hit( ch, victim, gsn_magma, 1);
      one_hit( ch, victim, gsn_magma, 1);
    }
    else one_hit( ch, victim, gsn_magma, 0);
  }
  if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_VENOMTONG) && number_range(1,4) == 1)
    one_hit( ch, victim, gsn_venomt, 0);  
   if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_BADBREATH) && number_range(1,4) == 1)
    multi_hit(ch,victim, gsn_badbreath);
  if ( victim == NULL || victim->position != POS_FIGHTING ) return;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER) && ch->level > 2)
  {
    if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
    {
      if (ch->pcdata->powers[HYDRA_LEVEL]==1) one_hit(ch,victim, gsn_fangs, 1);
      if (ch->pcdata->powers[HYDRA_LEVEL]>3) multi_hit(ch,victim, gsn_fangs);
      if (ch->pcdata->powers[HYDRA_LEVEL]>4) multi_hit(ch,victim, gsn_badbreath);
      if (ch->pcdata->powers[HYDRA_LEVEL]>4) multi_hit(ch,victim, gsn_badbreath);
    }
    if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
    {
      if (ch->pcdata->powers[TIGER_LEVEL]>0) one_hit(ch, victim, gsn_fangs, 0);
      if (ch->pcdata->powers[TIGER_LEVEL]>1) one_hit(ch, victim, gsn_claws, 0);
      if (ch->pcdata->powers[TIGER_LEVEL]>3) multi_hit(ch, victim, gsn_claws);
      if (ch->pcdata->powers[TIGER_LEVEL]>4) multi_hit(ch, victim, gsn_fangs);

    }
    if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) 
    {
      if (ch->pcdata->powers[BULL_LEVEL]>0) one_hit(ch, victim, gsn_headbutt,0);
      if (ch->pcdata->powers[BULL_LEVEL]>1) multi_hit(ch, victim, gsn_hooves);
      if (ch->pcdata->powers[BULL_LEVEL]>4) multi_hit(ch, victim, gsn_headbutt);
      if (ch->pcdata->powers[BULL_LEVEL]>4) multi_hit(ch, victim, gsn_hooves);
    }
    if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
    {
      one_hit(ch, victim, gsn_fireball,0);
      one_hit(ch, victim, gsn_fireball,0);
      if (ch->pcdata->powers[FAERIE_LEVEL] > 0) multi_hit(ch, victim, gsn_fangs);
      if (ch->pcdata->powers[FAERIE_LEVEL] > 1) multi_hit(ch, victim, gsn_buffet);
      one_hit(ch, victim, gsn_fireball,0);
    }
  }
  if (!IS_NPC(ch) && ch->level > 2)
  {
    if (IS_VAMPAFF(ch,VAM_HORNS) && number_range(1,3) == 1)
      multi_hit( ch, victim, gsn_headbutt );
  }
  if (IS_CLASS(ch, CLASS_PSIONIST) && !IS_NPC(ch))
  {
    if (IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDBLAST))
    {
        one_hit(ch,victim, gsn_mindblast,0);
        one_hit(ch,victim, gsn_mindblast,0);
	if (number_range(1,3)==1)
        one_hit(ch,victim, gsn_mindblast,0);
    }
    if (IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENFEEBLE))
    {
	one_hit(ch,victim, gsn_enfeeble,0);
        one_hit(ch,victim, gsn_enfeeble,0);
        if (number_range(1,3)==1)
	one_hit(ch,victim, gsn_enfeeble,0);
    }
  }
  if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
  {
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FANGS))
    {
      one_hit(ch, victim, gsn_fangs, 1);
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) one_hit(ch, victim, gsn_fangs, 1);
    }
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FIERY))
    {
      one_hit(ch, victim, gsn_fiery, 1);
      one_hit(ch, victim, gsn_fiery, 1);
      if (IS_CLASS(victim, CLASS_ANGEL)) one_hit(ch, victim, gsn_fiery, 1);
    }
  }
  if( !IS_NPC( ch ) )
  {
     if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) && ch->pcdata->powers[WEAPONSKILL] > 9)
     {
        one_hit(ch,victim,gsn_lightningslash,1);
        one_hit(ch,victim,gsn_lightningslash,1);
     }
  }
  if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_POWER_LICH))
  {
    if (ch->pcdata->powers[CON_LORE] > 4) one_hit(ch,victim, gsn_fireball,1);
    if (ch->pcdata->powers[NECROMANTIC] > 4) one_hit(ch,victim, gsn_chillhand,1);
    if (ch->pcdata->powers[DEATH_LORE] > 4) one_hit(ch,victim, gsn_deathaura,1);
  }
  if (IS_CLASS(ch, CLASS_DEMON)  && IS_DEMAFF(ch,DEM_HORNS) && number_range(1,4) == 1)
    one_hit( ch, victim, gsn_headbutt, 0 );
  if (IS_CLASS(ch, CLASS_DEMON) && number_range(1,3) == 1 && ch->power[DISC_DAEM_HELL] > 3)
    one_hit(ch, victim, gsn_hellfire, 0);
  if (IS_CLASS(ch, CLASS_DEMON)  && IS_DEMAFF(ch, DEM_TAIL) && number_range(1,4) == 2)
    one_hit( ch, victim, gsn_sweep, 0 );
  if (!IS_NPC(ch) )
  {
     if( IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] >=5 && number_percent() > 30)
        multi_hit(ch, victim, gsn_shiroken);
  }

  if (!IS_NPC(ch))
  {
    if (IS_VAMPAFF(ch,VAM_TAIL) && number_range(1,2) == 1)
      multi_hit( ch, victim, gsn_sweep );
  }
  
  if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_HEAD))
  {
    multi_hit( ch, victim, gsn_fangs );
  }
  if (!IS_NPC(ch) && IS_SET(ch->pcdata->powers[AURAS], DEATH_AURA) && IS_CLASS(ch,CLASS_UNDEAD_KNIGHT))
  {
    one_hit(ch,victim,gsn_deathaura,0);
    one_hit(ch,victim,gsn_deathaura,0);
    one_hit(ch,victim,gsn_deathaura,0);
  }
  if (!IS_NPC(ch) && IS_SET(ch->pcdata->powers[AURAS], FEAR_AURA) && IS_CLASS(ch, 
CLASS_UNDEAD_KNIGHT))
  {
    if (number_range(1,2) == 1)
    {
    af.type      = skill_lookup("reserved");
    af.duration  = 10;
    af.location  = APPLY_DAMROLL;
    af.modifier  = -100;
    af.bitvector = 0;
    affect_to_char(victim, &af );

    af.type      = skill_lookup("reserved");
    af.duration  = 10;
    af.location  = APPLY_HITROLL;
    af.modifier  = -100;
    af.bitvector = 0;
    affect_to_char(victim, &af );
    send_to_char("You piss your pants out of fear.\n\r",victim);
    }
  }
  if (!IS_NPC(ch) && IS_SET(ch->special, SPC_TORMENT) && IS_CLASS(ch, CLASS_INQUISITOR))
  {
    if (number_range(1,2) == 1)
    {
    af.type      = skill_lookup("reserved");
    af.duration  = 10;
    af.location  = APPLY_DAMROLL;
    af.modifier  = -100;
    af.bitvector = 0;
    affect_to_char(victim, &af );

    af.type      = skill_lookup("reserved");
    af.duration  = 10;
    af.location  = APPLY_HITROLL;
    af.modifier  = -100;
    af.bitvector = 0;
    affect_to_char(victim, &af );
    send_to_char("#0Your very soul is tormented.#n\n\r",victim);
    }
  }

  if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_QUILLS) && IS_CLASS(ch, CLASS_WEREWOLF))
    multi_hit(ch,victim,gsn_quills);
  if (!IS_NPC(ch) && ch->pcdata->powers[PINQUISITOR] > 7 && IS_CLASS(ch, CLASS_INQUISITOR))
    multi_hit(ch,victim,gsn_inquisitor);
  if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_BLADESPIN) && IS_CLASS(ch, CLASS_HIGHLANDER))
    multi_hit(ch,victim,gsn_bladespin);
  if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_BLADESPIN) && IS_CLASS(ch, CLASS_PALADIN))
    multi_hit(ch,victim,gsn_riposte);
  if (!IS_NPC(ch) && IS_SET(ch->newbits, NEW_DARKTENDRILS) && IS_CLASS(ch, CLASS_DROW))
    multi_hit(ch,victim,gsn_darktendrils);
  if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_SHADOWBLAST))
    multi_hit(ch,victim,gsn_shadowblast);
  if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_MULTIARMS) && IS_CLASS(ch, CLASS_MAGE))
    multi_hit(ch,victim,gsn_mageshield);
  if (!IS_NPC(ch) && ch->monkab[BODY] > 3 && IS_CLASS(ch, CLASS_MONK))
    multi_hit(ch,victim,gsn_heavenlyaura);
  if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_ANGEL))
    if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_AURA)) multi_hit(ch,victim,gsn_heavenlyaura);
  if (!IS_NPC(ch) && IS_SET(ch->newbits, NEW_CUBEFORM) && IS_CLASS(ch, CLASS_CYBORG))
  {
    one_hit(ch,victim,gsn_stuntubes,1);
    multi_hit(ch,victim,gsn_stinger);
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_CYBORG))
  {
    if ( ch->pcdata->powers[CYBORG_FACE] > 4)
	multi_hit(ch,victim,gsn_laser);
  }
  if (!IS_NPC(ch)&& IS_CLASS(ch,CLASS_WEREWOLF))
  {
    if (IS_SET(ch->newbits,NEW_SLAM) && number_range(1,3)==1)
      multi_hit(ch,victim,gsn_cheapshot);
  }
  if (!IS_NPC(ch) && IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_WINGS) && IS_CLASS(ch, CLASS_ANGEL))
    multi_hit( ch, victim, gsn_buffet );
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && IS_VAMPAFF(ch,VAM_WINGS) && number_range(1,2) == 1 && ch->level > 2)
    multi_hit( ch, victim, gsn_buffet );
  if (IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_WINGS) && number_range(1,2) == 1)
    one_hit(ch, victim, gsn_buffet, 0 );
  if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_WEREWOLF) && ch->power[DISC_WERE_RAPT] > 0)
    multi_hit(ch,victim,gsn_rfangs);
  if ( victim == NULL || victim->position != POS_FIGHTING ) return;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && IS_VAMPAFF(ch, VAM_SERPENTIS))
    spell_poison( gsn_poison, (ch->level*number_range(5,10)),ch,victim );
  else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_SPID] > 0)
    spell_poison( gsn_poison, (ch->level*number_range(5,10)),ch,victim );
  else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON))
    spell_poison(gsn_poison,(ch->level*number_range(10,20)),ch,victim);
  else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] >=5)
    spell_poison(gsn_poison,(ch->level*number_range(100,200)),ch,victim);
  if (victim->itemaffect < 1) return;
  if (IS_NPC(victim) || victim->spl[1] < 4) level = victim->level;
  else level = (victim->spl[1] * 0.25);
  if (IS_SET(victim->newbits, NEW_MONKFLAME) && !IS_AFFECTED(ch, AFF_FLAMING))
  {
    if (number_percent() > 95)
    {
      SET_BIT(ch->affected_by, AFF_FLAMING);
      if (!IS_CLASS(ch, CLASS_WEREWOLF))
      {
        act("Your flaming hands catch $n on fire!", ch, NULL, victim, TO_VICT, FALSE);
        act("$N's flaming hands catch $n on fire!", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$N's flaming hands catch you on fire!.", ch, NULL, victim, TO_CHAR, FALSE);
      }
      else
      {
      act("Your flaming claws catch $n on fire!", ch, NULL, victim, TO_VICT, FALSE);
      act("$N's flaming claws catch $n on fire!", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$N's flaming claws catch you on fire!.", ch, NULL, victim, TO_CHAR, FALSE);
      }
    }
    else return;
  }
  if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "lightning bolt" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "fireball" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  if (IS_ITEMAFF(victim, ITEMA_ICESHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "chill touch" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "acid blast" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  if (IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) && ch->position == POS_FIGHTING)
    if ( (sn = skill_lookup( "chaos blast" ) ) > 0)
      (*skill_table[sn].spell_fun) (sn,level,victim,ch);
  return;
}

int number_attacks( CHAR_DATA *ch, CHAR_DATA *victim )
{
  int count = 1;

  if (IS_NPC(ch))
  {
    if (ch->level >= 50) count += 1;
    if (ch->level >= 100) count += 1;
    if (ch->level >= 500) count += 1;
    if (ch->level >= 1000) count += 1;
    if (ch->level >= 1500) count += 1;
    if (ch->level >= 2000) count += 2;
    if( ch->pIndexData->hitsizedice > 20) count += 20;
    else count += ch->pIndexData->hitsizedice;
    return count;
  }

  if (IS_NPC(victim))
  {
    if ( IS_STANCE(ch, STANCE_VIPER) && number_percent() < ch->stance[STANCE_VIPER] * 0.5) count += 1;
    else if ( IS_STANCE(ch, STANCE_MANTIS) && number_percent() < ch->stance[STANCE_MANTIS] * 0.5) count += 1;
    else if ( IS_STANCE(ch, STANCE_TIGER) && number_percent() < ch->stance[STANCE_TIGER] * 0.5) count += 1;
    else if ( IS_STANCE(ch, STANCE_WOLF) && number_percent() < ch->stance[STANCE_WOLF] * 0.5) count += 2;
    else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_SPEED) 
	     && number_percent() < ch->stance[(ch->stance[0])] * 0.5 ) count +=2;

    if (IS_NPC(victim) && !IS_NPC(ch)) 
    {   
	if (ch->class == 0)
	{
          if( ch->move < 50000 ) count += 2;
          else count += ch->move/50000;
	}
      if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_CELE] > 0)
      {
        if (ch->power[DISC_VAMP_CELE] > 10)
        {
          count += 5;
 	}
        else if ( ch->power[DISC_VAMP_CELE] < 7)
          {
            count += 4;
 	  }
        else if ( ch->power[DISC_VAMP_CELE] < 5)
	  {
              count += 3;
          }
      }
    
      if (IS_CLASS(ch, CLASS_MONK))
      {
     	if (ch->chi[CURRENT] > 0 && ch->chi[CURRENT] < 3 ) count += 1;
    	else if (ch->chi[CURRENT] > 2 && ch->chi[CURRENT] <= 5 ) count+=3;
    	else if (ch->chi[CURRENT] == 6) count += 6;
      }

	  if (IS_CLASS(ch, CLASS_TANARRI)) count += ch->pcdata->rank;
	  if (IS_CLASS(ch, CLASS_PSIONIST))  count += ch->pcdata->rank;

      if (IS_CLASS(ch, CLASS_ANGEL)) count += ch->pcdata->powers[ANGEL_JUSTICE];

      if (IS_CLASS(ch, CLASS_WEREWOLF))
      {
        if (ch->power[DISC_WERE_LYNX] <= 6 )count += 2;
        if (ch->power[DISC_WERE_BOAR] > 6)
        {
          if( ch->move < 150000 ) count += 2;
          else count += ch->move/150000;
        }
      }

      if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
        count += ch->pcdata->powers[WEAPONSKILL];
      if (IS_CLASS(ch, CLASS_POWER_LICH)) count += 5;

      if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
      {
        count +=4;
        if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) count += 7;
        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) count += 9;
        if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) count += 6;
        if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) count += 8;
      }
      if ( IS_CLASS(ch, CLASS_DEMON))
      {
        if (IS_DEMPOWER( ch, DEM_SPEED)) count += 3;     
        if (IS_SET(ch->warp, WARP_QUICKNESS)) count += 6;
      }
      if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_DROW))
      {
        if (IS_SET(ch->special, SPC_DROW_WAR)) count += 4;
        if (IS_SET(ch->pcdata->powers[1],DPOWER_SPEED)) count += 4;
      }
        if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_MAGE))
        {
        if (IS_CLASS(ch, CLASS_MAGE) && !IS_ITEMAFF(ch, ITEMA_BEAST))
          count +=1;
        if (IS_CLASS(ch, CLASS_MAGE) && !IS_ITEMAFF(ch, ITEMA_BEAST))
          {
          if(ch->move < 50000) count += 2;
          else count += ch->move/50000;
          }
        }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 3)
      count += 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_ONE)
      count += 1;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TWO)
      count += 1;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_THREE)
      count += 2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FOUR)
      count += 2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FIVE)
      count += 2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SIX)
      count += 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SEVEN)
      count += 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_EIGHT)
      count += 4;

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_NINE)
      count += 4;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TEN)
      count += 5;
      if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_CYBORG))
        {
        if ( ch->pcdata->powers[CYBORG_LIMBS] < 5)
          count += ch->pcdata->powers[CYBORG_LIMBS];
        else if (ch->pcdata->powers[CYBORG_LIMBS] > 4)
          {
          if(ch->move < 100000) count += 2;
          else count += ch->move/100000;
          }
        }

    if (IS_CLASS(ch, CLASS_HIGHLANDER))
        {
        if (!IS_SET(ch->newbits, NEW_BLADESPIN) )
        count += 2;
        if (IS_SET(ch->newbits, NEW_BLADESPIN))
          {
          if(ch->move < 50000) count += 2;
          else count += ch->move/50000;
          }

        }
    if (IS_CLASS(ch, CLASS_PALADIN))
        {
        if (!IS_SET(ch->newbits, NEW_BLADESPIN) )
        count += 2;
        if (IS_SET(ch->newbits, NEW_BLADESPIN))
          {
          if(ch->move < 75000) count += 2;
          else count += ch->move/75000;
          }

        }

    if (IS_CLASS(ch, CLASS_INQUISITOR)) count += 7;
    if ( IS_ITEMAFF(ch, ITEMA_SPEED) ) count += 2;
    }
  }

else if (!IS_NPC(ch) && !IS_NPC(victim))
 {   
	
	/* Vampires */
      if ( IS_CLASS(ch, CLASS_VAMPIRE))
	 {
	  if( ch->power[DISC_VAMP_CELE] < 10)
		count += ch->power[DISC_VAMP_CELE]/2;
          else if ( ch->power[DISC_VAMP_CELE] > 9)
	   {
          if( ch->move < 150000 ) count += 2;
          else count += ch->move/150000;
           }
         }

      if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK))
      {
        if (ch->chi[CURRENT] > 0 && ch->chi[CURRENT] < 3 ) count += 1;
        if (ch->chi[CURRENT] > 2 && ch->chi[CURRENT] <= 5 ) count+=3;
        if (ch->chi[CURRENT] == 6) count += ch->move/50000;
      }

      if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_CYBORG))
	{
	if ( ch->pcdata->powers[CYBORG_LIMBS] < 5)
    	  count += ch->pcdata->powers[CYBORG_LIMBS];
	else if (ch->pcdata->powers[CYBORG_LIMBS] > 4)
	  {
	  if(ch->move < 100000) count += 2;
	  else count += ch->move/100000;
	  }
	}

	/* Tanar'ri */
	if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_TANARRI)) 
 	 {
	   if (ch->pcdata->rank < 6)
		count += ch->pcdata->rank;
	   if (ch->pcdata->rank > 5)
          {
          if(ch->move < 75000) count += 2;
          else count += ch->move/75000;
          }
	 }	
	/* Psionist */
        if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_PSIONIST))
         {
           if (ch->pcdata->rank < 6)
                count += ch->pcdata->rank;
           if (ch->pcdata->rank > 5)
          {
          if(ch->move < 75000) count += 2;
          else count += ch->move/75000;
          }
         }
	
	/* Angel */
	if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_ANGEL)) 
	{
	if (ch->pcdata->powers[ANGEL_JUSTICE] < 4)
	   count += ch->pcdata->powers[ANGEL_JUSTICE];
      	else if (ch->pcdata->powers[ANGEL_JUSTICE] > 4)
          {
          if(ch->move < 50000) count += 2;
          else count += ch->move/50000;
          }

        }
	/* Werewolf */
	if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_WEREWOLF))
      {
        if (ch->power[DISC_WERE_LYNX] <= 6)
	  count += ch->power[DISC_WERE_LYNX];
        if (ch->power[DISC_WERE_BOAR] > 6)
          {
          if(ch->move < 75000) count += 2;
          else count += ch->move/75000;
          }
      }
      
	/* Undead Knight */
	if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
        {
	  if (ch->pcdata->powers[WEAPONSKILL] < 9)
      	count += ch->pcdata->powers[WEAPONSKILL]/2;
	  if (ch->pcdata->powers[WEAPONSKILL] > 8)
          {
          if(ch->move < 50000) count += 2;
          else count += ch->move/50000;
          }
	  }

	/* Power Lich */
	if (IS_CLASS(ch, CLASS_POWER_LICH)) 
          {
	  count += 1;
          if(ch->move < 50000) count += 2;
          else count += ch->move/50000;
          }
	
	/* Shapeshifter */
	if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
      {
      if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) 
		{
		if (ch->pcdata->powers[BULL_LEVEL] < 5)
		count += ch->pcdata->powers[BULL_LEVEL];
		if (ch->pcdata->powers[BULL_LEVEL] > 4)
          {
          if(ch->move < 70000) count += 2;
          else count += ch->move/70000;
          }

		}
      else if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) 
		{
		if (ch->pcdata->powers[TIGER_LEVEL] < 5)
		count += ch->pcdata->powers[TIGER_LEVEL]*1.3;
		if (ch->pcdata->powers[TIGER_LEVEL] > 4)
          {
          if(ch->move < 50000) count += 2;
          else count += ch->move/50000;
          }

		}
      else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) 
		{
	  	if (ch->pcdata->powers[FAERIE_LEVEL] < 5)
		count += ch->pcdata->powers[FAERIE_LEVEL];
	  	if (ch->pcdata->powers[FAERIE_LEVEL] > 4)
          {
          if(ch->move < 100000) count += 2;
          else count += ch->move/100000;
          }

		}
      else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) 
		{
		if (ch->pcdata->powers[HYDRA_LEVEL] < 5)
		count += ch->pcdata->powers[HYDRA_LEVEL]*1.5;
		if (ch->pcdata->powers[HYDRA_LEVEL] > 4)
          {
          if(ch->move < 70000) count += 2;
          else count += ch->move/70000;
          }

		}
      }
	/* Demon */
	if ( IS_CLASS(ch, CLASS_DEMON))
      {
        if (IS_DEMPOWER( ch, DEM_SPEED)) count += 1;     
        if (IS_SET(ch->warp, WARP_QUICKNESS)) count += 5;
	if (ch->power[DISC_DAEM_ATTA] < 10)
	count += ch->power[DISC_DAEM_ATTA]/2;
	else if (ch->power[DISC_DAEM_ATTA] > 9)
          {
          if(ch->move < 50000) count += 2;
          else count += ch->move/50000;
          }

      }
      
	/* Drow */
	if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_DROW))
      {
        if (IS_SET(ch->special, SPC_DROW_WAR)) count += 5;
        if (IS_SET(ch->pcdata->powers[1],DPOWER_SPEED))
          {
          if(ch->move < 50000) count += 2;
          else count += ch->move/50000;
          }

	}
 	
	/* Battlemage */
	if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_MAGE))
	{
	if (IS_CLASS(ch, CLASS_MAGE) && !IS_ITEMAFF(ch, ITEMA_BEAST)) 
	  count +=1;
        if (IS_CLASS(ch, CLASS_MAGE) && !IS_ITEMAFF(ch, ITEMA_BEAST))
          {
          if(ch->move < 100000) count += 2;
          else count += ch->move/100000;
          }

	}
	
	/* Ninja */
      if( !IS_NPC( ch ) && (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >=3))
     	{
    	count += 4;
    	if (ch->pcdata->rank == BELT_ONE)
     	count += 1;
 	if(ch->pcdata->rank == BELT_TWO)
      count += 1;
    	if (ch->pcdata->rank == BELT_THREE)
      count += 1;
    	if (ch->pcdata->rank == BELT_FOUR)
      count += 2;
    	if (ch->pcdata->rank == BELT_FIVE)
      count += 2;
    	if (ch->pcdata->rank == BELT_SIX)
      count += 2;
    	if (ch->pcdata->rank == BELT_SEVEN)
      count += 3;
    	if (ch->pcdata->rank == BELT_EIGHT)
      count += 3;
    	if (ch->pcdata->rank == BELT_NINE)
      count += 3;
    	if (ch->pcdata->rank == BELT_TEN)
          {
          if(ch->move < 100000) count += 2;
          else count += ch->move/100000;
          }

    	}
	
	/* Samurai */    
  	if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_HIGHLANDER))
	{
	if (!IS_SET(ch->newbits, NEW_BLADESPIN) )
	count += 2;
    	if (IS_SET(ch->newbits, NEW_BLADESPIN))
          {
          if(ch->move < 100000) count += 2;
          else count += ch->move/100000;
          }

	}

	if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PALADIN))
        {
        if (!IS_SET(ch->newbits, NEW_BLADESPIN) )
        count += 2;
        if (IS_SET(ch->newbits, NEW_BLADESPIN))
          {
          if(ch->move < 100000) count += 2;
          else count += ch->move/100000;
          }

        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_INQUISITOR))
        {
        if (ch->pcdata->powers[PINQUISITOR] < 8)
        count += 2;
        if (ch->pcdata->powers[PINQUISITOR] > 7)
          {
          if(ch->move < 75000) count += 2;
          else count += ch->move/75000;
          }

        }


	/* Speed Eq */
	if ( IS_ITEMAFF(ch, ITEMA_SPEED) ) count += 2;
     }

 
  if (victim->power[DISC_VAMP_CELE] >= 3)
	count -= 1;
  if (victim->power[DISC_WERE_MANT] >= 3)
	count -= 1;
  else if (IS_ITEMAFF(victim, ITEMA_AFFMANTIS))
    count -= 1;
  if (victim->power[DISC_DAEM_ATTA] > 0)
	count -= 1;
  if (number_range(1,4) == 2)
    count -= 1;
  if (IS_ITEMAFF(victim, ITEMA_SPEED)) count -= 2;

  if ( count < 1)
    count = 1;

  return count;
  }


/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype)
{

  OBJ_DATA *wield;
  int victim_ac;
  int thac0;
  int thac0_00;
  int thac0_32;
  int dam, wdam;
  int diceroll;
  int level;
  int attack_modify;
  int right_hand;

  /*
   * Can't beat a dead char!
   * Guard against weird room-leavings.
   */
  if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
    return;
  if ( !IS_NPC(ch) )
  {
    if (IS_AFFECTED( ch, AFF_PEACE) && ch->fighting != NULL)
    REMOVE_BIT(ch->affected_by, AFF_PEACE);
  }
  if ( !IS_NPC(victim) )
  {
    if (IS_AFFECTED(victim, AFF_PEACE) && victim->fighting != NULL)
    REMOVE_BIT(victim->affected_by, AFF_PEACE);
  }
  /*
   * Figure out the type of damage message.
   */
  if (handtype == 8)
  {
    wield = get_eq_char( ch, WEAR_FOURTH );
    right_hand = 8;
  }
  else if (handtype == 4)
  {
    wield = get_eq_char( ch, WEAR_THIRD );
    right_hand = 4;
  }
  else if (handtype == 2)
  {
    wield = get_eq_char( ch, WEAR_HOLD );
    right_hand = 2;
  }
  else if (handtype == 0) 
  {
    wield = NULL;
    right_hand = 0;
  }
  else
  {
    wield = get_eq_char( ch, WEAR_WIELD );
    right_hand = 1;
  }
  if ( dt == TYPE_UNDEFINED )
  {
    dt = TYPE_HIT;
    if ( wield != NULL && wield->item_type == ITEM_WEAPON )
      dt += wield->value[3];
  }
  if (ch->wpn[dt-1000] > 5) level = (ch->wpn[dt-1000] / 5);
  else level = 1;
  if (level > 40) level = 40;

  /*
   * Calculate to-hit-armor-class-0 versus armor.
   */
  if ( IS_NPC(ch) )
  {
    thac0_00 = 20;
    thac0_32 =  0;
  }
  else
  {
    thac0_00 = SKILL_THAC0_00;
    thac0_32 = SKILL_THAC0_32;
  }
  thac0     = interpolate( level, thac0_00, thac0_32 ) - char_hitroll(ch);
  victim_ac = UMAX( -100, char_ac(victim) / 10 );
  if (!can_see( ch, victim ))
    victim_ac -= 4;
  if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_DAMAGE_3))
    victim_ac += 1000;
  if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_DAMAGE_2))
    victim_ac += 750;
  if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_DAMAGE_1))
    victim_ac += 500;

  
  /*
   * The moment of excitement!
   */
  while ((diceroll = number_bits( 5 ) ) >= 20)
  ;

  if ( diceroll == 0 || ( diceroll != 19 && diceroll < thac0 - victim_ac ) 
)
  { /* Miss. */
    damage( ch, victim, 0, dt );
    tail_chain( );
    if (!is_safe(ch,victim))
    {
      improve_wpn(ch,dt,right_hand);
      improve_stance(ch);
    }
    return;
  }

  /*
   * Hit.
   * Calc damage.
   */
  if ( IS_NPC(ch) )
  {
    dam = number_range( ch->level / 2, ch->level * 3 / 2 );
    if ( wield != NULL )
      dam += dam * 0.5;
  }
  else
  {
   if (dt == gsn_mageshield)
    {
      dam = (ch->spl[RED_MAGIC] + ch->spl[YELLOW_MAGIC] + ch->spl[GREEN_MAGIC]  + ch->spl[PURPLE_MAGIC] + ch->spl[BLUE_MAGIC])/4;
      dam = number_range(2*dam, 3*dam);
    }
   if (wield != NULL && wield->item_type == ITEM_WEAPON)
      dam += dice( wield->value[1], wield->value[2] );
    else if ( IS_VAMPAFF(ch, VAM_CLAWS) && IS_GAR1(ch, WOLF_RAZORCLAWS))
      dam += dice( 30, 40 ); 
    else if ( IS_CLASS(ch, CLASS_PALADIN))
      dam += dice( 30, 40 );
    else if ( IS_CLASS(ch, CLASS_INQUISITOR))
      dam += dice( 30, 40 );
    else if  ( IS_CLASS(ch, CLASS_HIGHLANDER))
      dam += dice( 30, 40 );
    else if ( IS_CLASS(ch, CLASS_PSIONIST))
      dam += dice( 30, 40 );
    else if ( IS_VAMPAFF(ch,VAM_CLAWS))
      dam += dice( 30, 40 );
    else if (IS_CLASS(ch, CLASS_MONK))
      dam += dice( 30, 40 );
    else if (IS_CLASS(ch, CLASS_MAGE))
      dam += dice( 30, 40 );
    else if (IS_SET(ch->pcdata->powers[AURAS], DEATH_AURA))
      dam += dice( 30, 40 );
    else if (IS_CLASS(ch, CLASS_SHAPESHIFTER) || IS_CLASS(ch, CLASS_ANGEL) || IS_CLASS(ch, CLASS_DROW) || IS_CLASS(ch, CLASS_CYBORG))
      dam += dice( 30, 40 );
    else if (IS_CLASS(ch, CLASS_NINJA))
    {
     if (ch->pcdata->rank == BELT_SEVEN)
	dam+=dice( 1, 5 );
     if (ch->pcdata->rank == BELT_EIGHT)
	dam+=dice( 5, 15 );
     if (ch->pcdata->rank == BELT_NINE)
	dam+=dice( 15, 25 );
     if (ch->pcdata->rank == BELT_TEN)
	dam+=dice( 30, 40 );
    }    
  }
  /*
   * Bonuses.
   */
  dam += char_damroll(ch);
  if (!IS_AWAKE(victim)) dam *= 2;
  if (!IS_NPC(ch) && dt >= TYPE_HIT) dam = dam + (dam * (UMIN(350,(ch->wpn[dt-1000]+1)) / 60));

  /* Other Resistances */
   
  if (IS_ITEMAFF(victim, ITEMA_RESISTANCE))
  {
    dam /= 2;
  }

  /* damage reduction */
/*  if (!IS_NPC(ch) && !IS_NPC(victim)) dam /= 2;*/  /* slow down pk */
  if (IS_NPC(ch) && dam > 2000) dam = 2000 + (dam-2000)/2;    /* mob damage >2000 halved */
  if ( dam <= 0 ) dam = 5;

/* up that damage */


  /* Vampire */
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
   {
 	if (ch->power[DISC_VAMP_POTE] > 7)
	 dam *= 2;
	if (ch->power[DISC_VAMP_POTE] > 9)
	 dam *= 2.5;
   }
  
  /* Tanar'ri */
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_TANARRI))
  {
  if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT)) 
  dam *= 2.6;
  if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)
  dam *= 3.0;
  }
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_PSIONIST))
  {
  if (IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_WRATH))
  dam *= 2.6;
  if (ch->pcdata->powers[PSIONIST_WRATH_ON] == 1)
  dam *= 3.0;
  }

  
  /* Angel */
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL)) 
  {
  if (ch->pcdata->powers[ANGEL_JUSTICE] < 2)
    dam *= 2.0;
  if (ch->pcdata->powers[ANGEL_JUSTICE] > 4)
    dam *= 3.5;
  }

  /* Drow */
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW))    
  {
  if (!IS_SET(ch->newbits, NEW_FIGHTDANCE))
  dam *= 2;
  if (IS_SET(ch->newbits, NEW_FIGHTDANCE))
  dam *= 3;
  }

  /* Demon */
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON))
  {
  if (IS_DEMPOWER(ch, DEM_MIGHT))
    dam *= 2.5;
  if (IS_SET(ch->warp, WARP_STRONGARMS))
    dam *= 2.0;
  }
/* Superstances, hmmmm, this is just a test - Jobo, remove if players 
get to rigged */

  if(IS_NPC(victim)) {

  if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_DAMAGE_3)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 1.5 * (ch->stance[(ch->stance[0])] / 200);
  else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_DAMAGE_2)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 1.0 * (ch->stance[(ch->stance[0])] / 200);
  else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_DAMAGE_1)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 0.5 * (ch->stance[(ch->stance[0])] / 200);
  }

  if(!IS_NPC(victim)) {
    
  if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_DAMAGE_3)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 0.5 * (ch->stance[(ch->stance[0])] / 200);
  else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_DAMAGE_2)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 0.3 * (ch->stance[(ch->stance[0])] / 200);
  else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_DAMAGE_1)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 0.1 * (ch->stance[(ch->stance[0])] / 200);
  }

  if ( victim->stance[0] > 12 && 
IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_3)
    && victim->stance[(victim->stance[0])] >100)
      dam *= 0.5;
  else if ( victim->stance[0] > 12 && 
IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_2)
    && victim->stance[(victim->stance[0])] >100)
      dam *= 0.6;
  else if ( victim->stance[0] > 12 && 
IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_1)
    && victim->stance[(victim->stance[0])] >100)
      dam *= 0.7;

  /* The test ends here */

  /* Power Lich */
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_POWER_LICH))
  {
   if (ch->pcdata->powers[DEATH_LORE] < 5)
	dam *= 1.8;
   if (ch->pcdata->powers[DEATH_LORE] > 9)
	dam *= 2.0;
  }
  
  /* Battlemage */
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE))
  {
    if (ch->pcdata->powers[PINVOKE]<9) dam *=1.7;
    if (ch->pcdata->powers[PINVOKE]>9) dam *=1.9;
  }

  /* Ninja */
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
  {
      if (dt == gsn_assassinate) dam *= 2;

    if (ch->pcdata->rank == BELT_ONE) dam *= 2.0;
    else if (ch->pcdata->rank == BELT_TWO) dam *= 2.1;
    else if (ch->pcdata->rank == BELT_THREE) dam *= 2.2;
    else if (ch->pcdata->rank == BELT_FOUR) dam *= 2.3;
    else if (ch->pcdata->rank == BELT_FIVE) dam *= 2.4;
    else if (ch->pcdata->rank == BELT_SIX) dam *= 2.5;
    else if (ch->pcdata->rank == BELT_SEVEN) dam *= 2.6;
    else if (ch->pcdata->rank == BELT_EIGHT) dam *= 2.7;
    else if (ch->pcdata->rank == BELT_NINE) dam *= 2.8;
    else if (ch->pcdata->rank == BELT_TEN) dam *= 3.5;
  }

  /* Shapeshifer */
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
    dam *= 1.6;
    if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM && ch->pcdata->powers[TIGER_LEVEL] > 1) 
    dam *= 3.1;
    else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM && ch->pcdata->powers[FAERIE_LEVEL] > 1) 
    dam *= 3.1;
    else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM && ch->pcdata->powers[HYDRA_LEVEL] > 1) 
    dam *= 3.2;
    else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM && ch->pcdata->powers[BULL_LEVEL] > 1) 
    dam *= 3.3;
  }
 
  /* Spider Droid */
  if (IS_CLASS(ch,CLASS_CYBORG) && !IS_NPC(ch))
  {
    if (ch->pcdata->powers[CYBORG_LIMBS] < 2) dam *= 1.5;
    if (ch->pcdata->powers[CYBORG_LIMBS] > 2) dam *= 3;
  }

  /* Samurai */
  if (IS_CLASS(ch,CLASS_HIGHLANDER))
  {
   if (!IS_SET(ch->newbits, NEW_BLADESPIN))
   dam *= 1.5;
   if (IS_SET(ch->newbits, NEW_BLADESPIN))
   dam *= 2.3;
  }

  /* Paladin */
  if (IS_CLASS(ch,CLASS_PALADIN))
  {
   if (!IS_SET(ch->newbits, NEW_BLADESPIN))
   dam *= 2;
   if (IS_SET(ch->newbits, NEW_BLADESPIN))
   dam *= 5;
  }

  if (IS_CLASS(ch, CLASS_INQUISITOR))
  {
   if (ch->pcdata->powers[PINQUISITOR] > 7)
     dam *= 5;
   else
     dam *= 2;
  }


  /* Undead Knight */
  if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
  {
    if (ch->pcdata->powers[WEAPONSKILL] > 4) dam *= 1.9;
    if (ch->pcdata->powers[WEAPONSKILL] > 8) dam *= 4;
  }
  
  /* Werewolf */
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF))
  {
   if (ch->power[DISC_WERE_BEAR] > 5)
    dam *= 1.6;
   if (ch->power[DISC_WERE_BEAR] > 9)
    dam *= 3.2;
  }
  
  /* Monk */
  if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK))
  {
    if (ch->chi[CURRENT] > 0 && ch->chi[CURRENT] < 3 ) dam *= 1.6;
    else if (ch->chi[CURRENT] > 2 && ch->chi[CURRENT] <= 5 ) dam *= 2.0;
    else if (ch->chi[CURRENT] == 6) dam *= 2.4;
  }


if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
  {
    if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM && 
victim->pcdata->powers[FAERIE_LEVEL]>0)
    {
      int growl = number_range(1,50);
      if (growl == 10 || growl == 20 || growl == 30 || growl == 40 || 
growl == 50)
      {
        stc("#3You dodge your opponents attack#n.\n\r",victim);
        stc("#3Your opponent dodges your attack#n.\n\r",ch);
        dam = 0;
      }
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && 
IS_SET(victim->newbits, NEW_DARKTENDRILS))
  {
    int ddodge = number_range(1,50);
    if (ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 || 
ddodge == 50 || ddodge == 15 || ddodge == 25 || ddodge == 35 || ddodge == 45)
    {
    if (!xIS_SET(victim->act, PLR_BRIEF2))
      stc("#0You dark aura absorbs the damage#n.\n\r",victim);
    if (!xIS_SET(ch->act, PLR_BRIEF2))
      stc("#0Your opponent's dark aura absorbs your damage#n.\n\r",ch);
      dam = 0;
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_POWER_LICH))
  {
    int ddodge = number_range(1,50);
    if (ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 ||
ddodge == 50 || ddodge == 15 || ddodge == 25 || ddodge == 35)
    {
    if (!xIS_SET(victim->act, PLR_BRIEF2))
        stc( "#0Your undead body is not harmed by this attack.#n\n\r",victim);
    if (!xIS_SET(ch->act, PLR_BRIEF2))
        stc( "#0You cannot seem to harm their undead body.#n\n\r",ch);
    dam = 0;
     }
  }
  if (IS_ITEMAFF(victim, ITEMA_MANASHIELD)) 
  {
    int ddodge;
    if (IS_CLASS(victim, CLASS_MAGE) || IS_CLASS(victim, CLASS_POWER_LICH) || IS_CLASS(victim, CLASS_INQUISITOR))
     {
      ddodge = number_range(1,50);
      if ((ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 ||
	ddodge == 50 || ddodge == 15 || ddodge == 25 || ddodge == 35) && victim->mana >= 1000)
      {
    	if (IS_NPC(ch))
      	  victim->mana -= 500;
	else if (!IS_NPC(ch))
      	  victim->mana -= 1000;
    	if (!xIS_SET(victim->act, PLR_BRIEF2))
      	  stc("#CYour shield of pulsating mana prevents the strike against you!#n.\n\r",victim);
    	if (!xIS_SET(ch->act, PLR_BRIEF2))
      	  stc("#CYour weapon fails to penetrate your opponent's shield of mana!#n.\n\r",ch);
    	dam = 0;
      }
    }
    else
    {
        ddodge = number_range(1,100);
      if ((ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 || ddodge == 50) && victim->mana >= 10000)
      {
	if (IS_NPC(ch))
	  victim->mana -= 5000;
        if (!IS_NPC(ch))
          victim->mana -= 10000;
        if (!xIS_SET(victim->act, PLR_BRIEF2))
          stc("#CYour shield of pulsating mana prevents the strike against you!#n.\n\r",victim);
        if (!xIS_SET(ch->act, PLR_BRIEF2))
          stc("#CYour weapon fails to penetrate your opponent's shield of mana!#n.\n\r",ch);
        dam = 0;
      }
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_CYBORG) && 
victim->pcdata->powers[CYBORG_BODY] > 1)
  {
    int ddodge = number_range(1,50);
    if (ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 || ddodge == 50)
    {
        if (!xIS_SET(victim->act, PLR_BRIEF2))
      stc("#0Your hardened body helps abosrb the damage#n.\n\r",victim);
        if (!xIS_SET(ch->act, PLR_BRIEF2))
      stc("#0Your opponent's hard body helps abosorb your damage#n.\n\r",ch);
      dam = 0;
    }
  }

  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_ILLUSIONS))
  {
    int ddodge = number_range(1,50);
    if (ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 ||
ddodge == 50 || ddodge == 15 || ddodge == 25 || ddodge == 35)
    {
    if (!xIS_SET(victim->act, PLR_BRIEF2))
      stc("#CYour illusion takes the blow, saving you from harm#n.\n\r",victim);
    if (!xIS_SET(ch->act, PLR_BRIEF2))
      stc("#CYour weapon passes through your opponent without doing any harm#n.\n\r",ch);
      dam = 0;
    }
  }
  if ( IS_CLASS(victim, CLASS_MONK ) && victim->monkab[SPIRIT] >= 2 && IS_SET(victim->newbits, NEW_NATURAL))
  {
    int ddodge = number_range(1,50);
    if (ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 || ddodge == 50)
    {
    if (!xIS_SET(victim->act, PLR_BRIEF2))
      stc("You absorb the natural imbalances of your opponent.\n\r",victim);
    if (!xIS_SET(ch->act, PLR_BRIEF2))
      stc("Your weapon fails to do any damage with that attack.\n\r",ch);
      dam = 0;
    }
  }

  if (!IS_NPC(ch) && wield != NULL)
  {
    if (number_range(1,6) == 2)
    {
      if (ch->fighting == victim && IS_WEAP(wield,WEAPON_VAMPIRIC))
      {
        wdam = number_range(1,wield->level / 5 + 1);
        act("$p draws life from $n.",victim,wield,NULL,TO_ROOM, FALSE);
        act("You feel $p drawing your life away.",
        victim,wield,NULL,TO_CHAR, FALSE);
        hurt_person(ch,victim, wdam); 
      }
      if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_FLAMING) || 
IS_WEAP(wield, WEAPON_ELE_FLAME)))
      {
        wdam = number_range(1,wield->level / 4 + 1);
        act("$n is burned by $p.",victim,wield,NULL,TO_NOTVICT, FALSE);
        act("$p sears your flesh.",victim,wield,NULL,TO_CHAR, FALSE);
        fire_effect( (void *) victim,wield->level/2,wdam,TARGET_CHAR);
        hurt_person(ch, victim, wdam);
      }
      if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_FROST) || 
IS_WEAP(wield, WEAPON_ELE_WATER)))
      {
        int sn = skill_lookup("chill touch");
        if (!is_affected(victim,sn)) {
        wdam = number_range(1,wield->level / 6 + 2);
        act("$p freezes $n.",victim,wield,NULL,TO_ROOM, FALSE);
        act("The cold touch of $p surrounds you with ice.",
        victim,wield,NULL,TO_CHAR, FALSE);
        cold_effect(victim,wield->level/2,wdam,TARGET_CHAR);
        hurt_person(ch,victim, wdam); }
      }
      if (ch->fighting == victim && IS_WEAP(wield, WEAPON_ELE_EARTH) )
      {
        wdam = number_range(1,wield->level / 5 + 2);
        act("$p covers $n in acid.",victim,wield,NULL,TO_ROOM, FALSE);
        act("The corrosive touch of $p surrounds you with acid.",
        victim,wield,NULL,TO_CHAR, FALSE);
        acid_effect(victim,wield->level/2,wdam,TARGET_CHAR);
        hurt_person(ch,victim, wdam);
      }
      if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_SHOCKING)  || 
IS_WEAP(wield, WEAPON_ELE_AIR)))
      {
        wdam = number_range(1,wield->level/5 + 2);
        act("$n is struck by lightning from $p.",victim,wield,NULL,TO_ROOM, FALSE);
        act("You are shocked by $p.",victim,wield,NULL,TO_CHAR, FALSE);
        shock_effect(victim,wield->level/2,wdam,TARGET_CHAR);
        hurt_person(ch,victim, wdam);
      }
    }
  }

  if ( dt == gsn_garotte )
    dam *= (number_range(3,5));
   
  if ( dt == gsn_backstab ) {
    if( IS_CLASS(ch, CLASS_NINJA))
    {
      dam *= number_range(100,1000);
    }
    else if( IS_CLASS(ch, CLASS_VAMPIRE))
    {
      dam *= number_range(500,750);
    }
    else dam *= number_range(2,4);
  }

  if (dt == gsn_shiroken && IS_CLASS(ch, CLASS_NINJA)) dam *= 1.5;
  if (dt == gsn_heavenlyaura && IS_CLASS(ch, CLASS_MONK)) dam *= 2.5;
  if (dt == gsn_deathaura) dam *= 3;
  dam = cap_dam(ch,victim,dam);
  if( IS_NPC(ch) && ch->pIndexData->hitplus > 3) dam *=3;
  else if( IS_NPC(ch) && ch->pIndexData->hitplus > 0 ) dam *= ch->pIndexData->hitplus;

  if( IS_NPC(victim))
  {
    if( victim->pIndexData->hitnodice > 100 ) 
victim->pIndexData->hitnodice = 0;
    dam = dam * (100 - victim->pIndexData->hitnodice) / 100;
  }

  attack_modify = dice(1, 100);
  randomize_damage(ch,dam,attack_modify);

  if (dt == gsn_deathaura && dam >= victim->hit && IS_NPC(victim)) dam = 
victim->hit-1; /* trust me, Jobo */
  damage( ch, victim, dam, dt );

  tail_chain( );
  if (!is_safe(ch,victim))
  {
    improve_wpn(ch,dt,right_hand);
    improve_stance(ch);
  }
}


int randomize_damage( CHAR_DATA *ch, int dam, int am )
{
  dam = dam * (am + 50) / 100;
  return dam;
}

int cap_dam(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
  if (is_safe(ch, victim)) return 0;

  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW)) 
  {
    if (IS_SET(victim->pcdata->powers[1], DPOWER_TOUGHSKIN))
      dam /= 3;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_POWER_LICH))
  {
    dam /=4;
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam *= 0.25; /*liches hate fae*/
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL))
  {
    dam *= (100 - (victim->pcdata->powers[ANGEL_HARMONY] * 13));
    dam /= 100;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_TANARRI))
  {
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_EXOSKELETON)) dam *= 0.35;
    if (IS_GOOD(victim)) dam *= 0.75; 
 }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_PSIONIST))
  {
    if (IS_SET(victim->pcdata->powers[PSIONIST_POWER], PSIONIST_BODY)) dam *= 0.5;
    if (IS_SET(victim->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS))  dam *= 0.85;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_CYBORG))
  {
    if (victim->pcdata->powers[CYBORG_BODY] > 5) dam *= 0.5;
    if (IS_SET(victim->newbits, NEW_CUBEFORM))   dam *= 0.5;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))
  {
    if (victim->power[DISC_VAMP_PROT] >= 10) dam /= 2;
    if (victim->power[DISC_VAMP_FORT] < 9)
    {
      dam *= (100 - (victim->power[DISC_VAMP_FORT] * 5.5));
      dam /= 100;
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_HIGHLANDER))
  {
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam /= 2; /* damn shapies */
    dam /= 1.1;
    if (IS_CLASS(ch, CLASS_ANGEL)) dam *= 0.75;
	}
  if (!IS_NPC(victim) && !IS_CLASS(victim, CLASS_PALADIN))
  {
	dam /= 1.1;
	if (IS_SET(victim->newbits, NEW_BLADESPIN)) dam /= 1.5;
  }
  if (!IS_NPC(victim) && !IS_CLASS(victim, CLASS_INQUISITOR))
  {	dam /= 1.5;
	if (victim->pcdata->powers[PINQUISITOR] > 7) dam /= 1.5;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE))
  {
    if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD)) dam /= 1.2;
    if (IS_ITEMAFF(victim, ITEMA_BEAST))       dam /= 1.1;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
  {
    if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
    {
      dam *= (100 - victim->pcdata->powers[FAERIE_LEVEL]*9);
      dam /= 100;
    }
    else if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
    {
      dam *= (100 - victim->pcdata->powers[TIGER_LEVEL]*8);
      dam /= 100;
      if (victim->pcdata->powers[PHASE_COUNTER] > 0)
      {
        dam *= (100 - victim->pcdata->powers[PHASE_COUNTER] * 2.5);
        dam /= 100;
      }
    }
    else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
    {
      dam *= (100 - victim->pcdata->powers[HYDRA_LEVEL]*10);
      dam /= 100;
    }
    else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
    {
      dam *= (100 - victim->pcdata->powers[BULL_LEVEL]*12);
      dam /= 100;
    }
    else dam = dam;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK))
  {
    if (victim->monkab[BODY] >= 2) dam /= 2.0;
    if ( IS_SET(ch->newbits, NEW_MONKSKIN) ) dam /= 2;	
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON))
  {
    if (victim->power[DISC_DAEM_IMMU] > 0)
    {
      dam *= (100 - (victim->power[DISC_DAEM_IMMU] * 4));
      dam /= 100;
    }
    if (IS_DEMPOWER( victim, DEM_TOUGH))    dam *= 0.9;
    if (IS_SET(victim->warp, WARP_STAMINA)) dam *= 0.8;
    if (IS_SET(victim->warp, WARP_SBODY))   dam *= 0.9;
    if (IS_SET(victim->warp, WARP_CBODY))   dam *= 0.9;    
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
  {
    dam *= (100 - (victim->pcdata->powers[UNDEAD_SPIRIT] * 6));
    dam /= 100;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
  {
    if (victim->pcdata->powers[NPOWER_CHIKYU] >= 1) dam /= 2.75;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF))
  {
    if(victim->power[DISC_WERE_BOAR] > 2) dam /= 1.8;
    if(IS_GAR1(victim, WOLF_COCOON))      dam /= 1.8;
  }
 if (dam >= 2000) dam = dam - (dam / number_range(3,10) + number_range(10,30));
  if (!IS_NPC(victim))
  {
    dam = dam + (dam / number_range(2,5) + number_range(10,50));
    dam *= (number_range(2,4) * number_range(2,3) / number_range(4,6));
  }
  if (dam <= 0) dam = 1;
  if (dam > 30000) dam = 30000;                                               
  if (dam > ch->damcap[DAM_CAP]) dam = number_range( (ch->damcap[DAM_CAP] - 200), (ch->damcap[DAM_CAP] + 100) );
  if ( IS_AFFECTED(victim, AFF_SANCTUARY ) ) dam *= 0.5;
  return dam;
}

bool can_counter( CHAR_DATA *ch)
{
  if (!IS_NPC(ch) && IS_STANCE(ch, STANCE_MONKEY)) return TRUE;
  return FALSE;
}

bool can_bypass( CHAR_DATA *ch, CHAR_DATA *victim )
{
  if (IS_NPC(ch) || IS_NPC(victim)) return FALSE;
  else if (IS_STANCE(ch, STANCE_VIPER)) return TRUE;
  else if (IS_STANCE(ch, STANCE_MANTIS)) return TRUE;
  else if (IS_STANCE(ch, STANCE_TIGER)) return TRUE;
  else if (IS_STANCE(ch, STANCE_WOLF)) return TRUE;
  else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], 
STANCEPOWER_BYPASS)) return TRUE;
  return FALSE;
}

void update_damcap( CHAR_DATA *ch, CHAR_DATA *victim )
{
  int max_dam = 1000;
  int i = 0;
  OBJ_DATA  *trial;
  OBJ_DATA  *trial2;
  OBJ_DATA  *trial3;
  OBJ_DATA  *trial4;
  OBJ_DATA  *trial5;
  OBJ_DATA  *trial6;
  OBJ_DATA  *trial7;
  OBJ_DATA  *trial8;
  OBJ_DATA  *trial9;
  OBJ_DATA  *trial10;
  OBJ_DATA  *trial11;
  OBJ_DATA  *trial12;
  OBJ_DATA  *trial13;
  OBJ_DATA  *trial14;
  OBJ_DATA  *trial15;
  OBJ_DATA  *trial16;
  OBJ_DATA  *trial17;
  OBJ_DATA  *trial18;
  OBJ_DATA  *trial19;
  OBJ_DATA  *trial20;
  OBJ_DATA  *trial21;
  OBJ_DATA  *trial22;
  OBJ_DATA  *trial23;
  OBJ_DATA  *trial24;
  OBJ_DATA  *trial25;
  OBJ_DATA  *trial26;
  if (!IS_NPC(ch))
  {
    if (ch->practice >= 20000 && ch->practice < 40000)  max_dam += 100;
    else if (ch->practice >= 40000 && ch->practice < 60000)  max_dam += 200;
    else if (ch->practice >= 60000 && ch->practice < 80000)  max_dam += 300;
    else if (ch->practice >= 80000 && ch->practice < 100000)  max_dam += 400;
    else if (ch->practice >= 100000 && ch->practice < 150000) max_dam += 500;
    else if (ch->practice >= 150000 && ch->practice < 200000) max_dam += 600;
    else if (ch->practice >= 200000 && ch->practice < 300000) max_dam += 700;
    else if (ch->practice >= 300000 && ch->practice < 400000) max_dam += 800;	
    else if (ch->practice >= 400000 && ch->practice < 500000) max_dam += 900;
    else if (ch->practice >= 500000) max_dam += 1000;
       if (ch->lstatus > 0 )
       {
	 if ( ch->lstatus == 1 )
       max_dam += 100;
         if ( ch->lstatus == 2 )
       max_dam += 200;
         if ( ch->lstatus == 3 )
       max_dam += 300;
         if ( ch->lstatus == 4 )
       max_dam += 400;
         if ( ch->lstatus == 5 )
       max_dam += 500;
         if ( ch->lstatus == 6 )
       max_dam += 600;
         if ( ch->lstatus == 7)
       max_dam += 700;
         if ( ch->lstatus == 8)
       max_dam += 800;
         if ( ch->lstatus == 9)
       max_dam += 1000;
       }


    if (ch->generation == 0) max_dam += 500; /*Mobius*/
    if (ch->generation == 1) max_dam += 350;
    if (ch->generation == 2) max_dam += 200;
    if (ch->generation == 3) max_dam += 150;
    if (ch->generation == 4) max_dam += 100;
    if (ch->generation == 5) max_dam += 50;

    if (!xIS_SET(ch->extra, EXTRA_JUST_PK))
    {
    if (ch->race >= 1)
	{
	 max_dam += ch->race*10;
	}
    }
    else if (xIS_SET(ch->extra, EXTRA_JUST_PK))
    {
    if (ch->race >= 1)
        {
         max_dam += ch->race*20;
        }
    }

       if (IS_SET(ch->newbits, NEW_SUPREME) && ch->level < 6)
       max_dam += 500;

    if (ch->class == 0)
	{
  	max_dam += (ch->max_hit / 300);
	max_dam += (ch->max_move / 300);
	max_dam += (ch->max_mana / 300);
	}	
    if (IS_CLASS(ch, CLASS_MAGE))
    {
       if (IS_ITEMAFF(ch, ITEMA_BEAST)) max_dam += 2000;
       max_dam += 1500;
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_INQUISITOR))
    {
      if (ch->pcdata->powers[PINQUISITOR] > 0 && ch->pcdata->souls > 10)
	max_dam += ch->pcdata->souls * 10;
      for (i=0; i < 13 ; i++)
        {
        if (ch->wpn[i] >= 700 && ch->wpn[i] < 800) max_dam += 225;
        else if (ch->wpn[i] >= 800 && ch->wpn[i] < 900) max_dam += 230;
        else if (ch->wpn[i] >= 900 && ch->wpn[i] < 1000) max_dam += 240;
        else if (ch->wpn[i] == 1000) max_dam += 250;
        }
      if (IS_SET(ch->special, SPC_FURY))
	max_dam *= 2;
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_TANARRI))
    {
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT)) max_dam += 1000;
      max_dam += ch->pcdata->rank*500; /* max rank is 6 btw */
    }
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_PSIONIST))
    {
      if (IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_BODY)) max_dam += 350;
      if (IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_ENERGY)) max_dam += 350;
      max_dam += ch->pcdata->souls*5;
      max_dam += ch->pcdata->rank*350; /* max rank is 6 btw */
    }

    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_POWER_LICH))
    {
      if (ch->pcdata->powers[CON_LORE] > 4) max_dam += 500;
      if (ch->pcdata->powers[DEATH_LORE] > 4) max_dam += 500;
      if (ch->pcdata->powers[LIFE_LORE] > 4) max_dam += 500;
      if (ch->pcdata->powers[NECROMANTIC] > 4) max_dam += 500;
      if (ch->pcdata->powers[CHAOS_MAGIC] > 4) max_dam += 500;
      max_dam += 500;
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_ANGEL))
    {
      max_dam += ch->pcdata->powers[ANGEL_JUSTICE]*155;
      max_dam += ch->pcdata->powers[ANGEL_PEACE]*155;
      max_dam += ch->pcdata->powers[ANGEL_HARMONY]*155;
      max_dam += ch->pcdata->powers[ANGEL_LOVE]*155;
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      max_dam += 1500;
      if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
	 max_dam += ch->pcdata->powers[TIGER_LEVEL] * 400;
      else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
	 max_dam += ch->pcdata->powers[HYDRA_LEVEL] * 400;
      else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
	 max_dam += ch->pcdata->powers[BULL_LEVEL] * 400;
      else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
	 max_dam += ch->pcdata->powers[FAERIE_LEVEL] * 400;
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_DROW))
    {
      if (IS_SET(ch->newbits, NEW_DROWHATE)) max_dam += 2000;
      if (IS_SET(ch->newbits,NEW_DFORM)) max_dam += 2000;
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_DEMON) && ch->in_room != NULL)
    {
      max_dam += ch->power[DISC_DAEM_ATTA] * 200;
      max_dam += 1000;
      if (ch->pcdata->souls < 10 && ch->pcdata->souls > 1) max_dam += 500;
      if (ch->pcdata->souls >= 10) max_dam += (1000 + ((ch->pcdata->souls - 10) * 5));
      max_dam += (ch->pcdata->stats[DEMON_POWER] * 2);
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_CYBORG))
    {
      if (ch->pcdata->powers[CYBORG_LIMBS] > 0) max_dam += 800;
      if (ch->pcdata->powers[CYBORG_LIMBS] > 1) max_dam += 800;
      if (ch->pcdata->powers[CYBORG_LIMBS] > 2) max_dam += 800;
      if (ch->pcdata->powers[CYBORG_LIMBS] > 3) max_dam += 800;
      if (ch->pcdata->powers[CYBORG_LIMBS] > 4) max_dam += 800;
    }
    if (IS_CLASS(ch, CLASS_MONK)) {
      max_dam +=  (ch->monkab[COMBAT] * 250);
      if (ch->chi[CURRENT] > 0) max_dam += ch->chi[CURRENT] * 400;
      max_dam += 500;
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_VAMPIRE))
    {
      max_dam += (ch->rage * 10);

      if (ch->power[DISC_VAMP_POTE] > 0) 
        max_dam += (ch->power[DISC_VAMP_POTE] * 40);

      if( ch->pcdata->rank == AGE_TRUEBLOOD)  max_dam += 750;
      else if( ch->pcdata->rank == AGE_LA_MAGRA) max_dam += 400;
      else if (ch->pcdata->rank == AGE_METHUSELAH) max_dam += 300;
      else if (ch->pcdata->rank == AGE_ELDER) max_dam += 200;
      else if (ch->pcdata->rank == AGE_ANCILLA) max_dam += 100;
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_NINJA)) 
    {
      max_dam += (ch->rage * 7);

      if(ch->pcdata->powers[NPOWER_CHIKYU] >=6 && ch->pcdata->powers[HARA_KIRI] > 0) 
        max_dam += 1300;

      if(ch->pcdata->powers[NPOWER_CHIKYU] >=2)
        max_dam += 1300;
    }
    if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
      if (ch->rage > 99)
      {
        max_dam += (ch->rage * 6);
      }
      if (ch->power[DISC_WERE_PAIN]>0)
	max_dam += ch->power[DISC_WERE_PAIN] * 150;
    }
    if ( !IS_NPC( ch ) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
    {
      max_dam += ch->pcdata->powers[WEAPONSKILL] * 300;
	if (IS_SET(ch->pcdata->powers[AURAS], MIGHT_AURA)) max_dam += 500;
    }
    if (IS_CLASS(ch, CLASS_PALADIN))
    {
	max_dam += 500;
      for (i=0; i < 13 ; i++)
	{
	if (ch->wpn[i] >= 700 && ch->wpn[i] < 800) max_dam += 230;
	else if (ch->wpn[i] >= 800 && ch->wpn[i] < 900) max_dam += 240;
        else if (ch->wpn[i] >= 900 && ch->wpn[i] < 1000) max_dam += 250;
        else if (ch->wpn[i] == 1000) max_dam += 260;
	}
    }

    if (IS_CLASS(ch, CLASS_HIGHLANDER))
    {
      for (i=0; i < 13 ; i++)
	{
        if (ch->wpn[i] >= 1200 && ch->wpn[i] < 1300) max_dam += 250;
        else if (ch->wpn[i] >= 1300 && ch->wpn[i] < 1400) max_dam += 260;
        else if (ch->wpn[i] >= 1400 && ch->wpn[i] < 1500) max_dam += 270;
        else if (ch->wpn[i] == 1500) max_dam += 275;
	}
    }
  if (IS_ITEMAFF(ch, ITEMA_ARTIFACT)) max_dam += 250;
  trial = get_eq_char( ch, WEAR_LIGHT );
  if (trial != NULL && trial->pIndexData->vnum == 420000) max_dam += 100; 
  trial2 = get_eq_char( ch, WEAR_SHIELD );
  if (trial2 != NULL && trial2->pIndexData->vnum == 220004) max_dam += 100;
  trial3 = get_eq_char( ch, WEAR_FACE );
  if (trial3 != NULL && trial3->pIndexData->vnum == 100100) max_dam += 100;
  trial4 = get_eq_char( ch, WEAR_FACE );
  if (trial4 != NULL && trial4->pIndexData->vnum == 100101) max_dam += 100;
  trial5 = get_eq_char( ch, WEAR_FACE );
  if (trial5 != NULL && trial5->pIndexData->vnum == 100102) max_dam += 100;
  trial6 = get_eq_char( ch, WEAR_WIELD );
  if (trial6 != NULL && (trial6->pIndexData->vnum == 100103 || trial6->pIndexData->vnum ==101503)) max_dam += 100;
  trial7 = get_eq_char( ch, WEAR_HEAD );
  if (trial7 != NULL && trial7->pIndexData->vnum == 420004) max_dam += 100;
  trial8 = get_eq_char( ch, WEAR_HOLD );
  if (trial8 != NULL && (trial8->pIndexData->vnum == 100103 || trial8->pIndexData->vnum == 101503)) max_dam += 100;
  trial9 = get_eq_char( ch, WEAR_THIRD );
  if (trial9 != NULL && (trial9->pIndexData->vnum == 100103 || trial9->pIndexData->vnum ==101503)) max_dam += 100;
  trial10 = get_eq_char( ch, WEAR_FOURTH );
  if (trial10 != NULL && (trial10->pIndexData->vnum == 100103 || trial10->pIndexData->vnum == 101503)) max_dam += 100;
  trial11 = get_eq_char( ch, WEAR_LEGS );
  if (trial11 != NULL && (trial11->pIndexData->vnum == 101500 || trial11->pIndexData->vnum == 101501 || trial11->pIndexData->vnum == 101502)) max_dam += 100;
  trial12 = get_eq_char( ch, WEAR_ABOUT );
  if (trial12 != NULL && trial12->pIndexData->vnum == 101400) max_dam += 100;
  trial13 = get_eq_char( ch, WEAR_FINGER_L );
  if (trial13 != NULL && trial13->pIndexData->vnum == 100928) max_dam += 100;
  trial14 = get_eq_char( ch, WEAR_FINGER_R );
  if (trial14 != NULL && trial14->pIndexData->vnum == 100928) max_dam += 100;
  trial15 = get_eq_char( ch, WEAR_NECK_1 );
  if (trial15 != NULL && trial15->pIndexData->vnum == 100404) max_dam += 100;
  trial16 = get_eq_char( ch, WEAR_NECK_2 );
  if (trial16 != NULL && trial16->pIndexData->vnum == 100404) max_dam += 100;
  trial17 = get_eq_char( ch, WEAR_LEGS );
  if (trial17 != NULL && trial17->pIndexData->vnum == 101500) max_dam += 100;
  trial18 = get_eq_char( ch, WEAR_LEGS );
  if (trial18 != NULL && trial18->pIndexData->vnum == 101501) max_dam += 100;
  trial19 = get_eq_char( ch, WEAR_LEGS );
  if (trial19 != NULL && trial19->pIndexData->vnum == 101501) max_dam += 100;
  trial20 = get_eq_char( ch, WEAR_HANDS );
  if (trial20 != NULL && trial20->pIndexData->vnum == 101401) max_dam += 100;
  trial21 = get_eq_char( ch, WEAR_BODY );
  if (trial21 != NULL && trial21->pIndexData->vnum == 101403) max_dam += 500;
  trial22 = get_eq_char( ch, WEAR_NECK_1 );
  if (trial22 != NULL && trial22->pIndexData->vnum == 101404) max_dam += 100;
  trial23 = get_eq_char( ch, WEAR_NECK_2 );
  if (trial23 != NULL && trial23->pIndexData->vnum == 101404) max_dam += 100;
  trial24 = get_eq_char( ch, WEAR_TATTOO );
  if (trial24 != NULL && trial24->pIndexData->vnum == 41046) max_dam += 100;
  trial25 = get_eq_char( ch, WEAR_TATTOO );
  if (trial25 != NULL && trial25->value[3] == 33) max_dam += 500;
  trial26 = get_eq_char( ch, WEAR_TATTOO );
  if (trial26 != NULL && trial26->value[3] == 34) max_dam += 1000;

  if (IS_NPC(victim) || victim->stance[0] != STANCE_MONKEY )
  {
    if      (ch->stance[0] == STANCE_BULL)   max_dam += 200;
    else if (ch->stance[0] == STANCE_DRAGON) max_dam += 250;
    else if (ch->stance[0] == STANCE_WOLF)   max_dam += 250;
    else if (ch->stance[0] == STANCE_TIGER)  max_dam += 200;
    else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMCAP_3)) max_dam += 500;
    else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMCAP_2)) max_dam += 250;
    else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMCAP_1)) max_dam += 100;
  }

  if (!IS_NPC(victim) && ch->stance[0] != STANCE_MONKEY )
  {  
    if (victim->stance[0] > 12 && 
IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_REV_DAMCAP_3)) 
max_dam -= 500;
    else if (victim->stance[0] > 12 && 
IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_REV_DAMCAP_2)) 
max_dam -= 250;
    else if (victim->stance[0] > 12 && 
IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_REV_DAMCAP_1)) 
max_dam -= 100;
  }


  /* Max Dam Reductions */
  /* Werewolves */

  if (!IS_NPC(victim))
  {
    int silver_tol = (victim->siltol * 2.5);
    if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(ch, 
ITEMA_RIGHT_SILVER))
      max_dam += (150 - silver_tol);
    if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(ch, 
ITEMA_LEFT_SILVER))
      max_dam += (150 - silver_tol);
    if (IS_NPC(ch) || ch->stance[0] != STANCE_MONKEY)
    {
      if      (victim->stance[0] == STANCE_CRAB)    max_dam -= 250;
      else if (victim->stance[0] == STANCE_DRAGON)  max_dam -= 250;
      else if (victim->stance[0] == STANCE_SWALLOW) max_dam -= 250;
    }
  }
  if (xIS_SET(ch->extra, IS_A_BITCH))
  {
   if (max_dam > 1000) max_dam /= 2;
  }

  if (ch->level >= LEVEL_BUILDER) max_dam = 30000;
  ch->damcap[DAM_CAP] = max_dam;
  ch->damcap[DAM_CHANGE] = 0;
  return;
  }
}

int check_immune(CHAR_DATA *ch, int dam_type)
{
  int immune, def;
  int bit;

  immune = -1;
  def = IS_NORMAL;

  if (dam_type == DAM_NONE) return immune;

  if (ch->class == CLASS_VAMPIRE)
  {
    if (dam_type == DAM_COLD) immune = IS_RESISTANT;
    if (dam_type == DAM_DROWNING) immune = IS_VULNERABLE;
    if (dam_type == DAM_FIRE) immune = IS_VULNERABLE;
  }

  if (ch->power[DISC_VAMP_PROT] >= 7 && dam_type == DAM_FIRE) immune = 
IS_RESISTANT;
  if (dam_type == DAM_POISON && ch->power[DISC_DAEM_CORR] >= 3) immune = 
IS_DIMMUNE;
  if (dam_type == DAM_POISON && ch->power[DISC_DAEM_CORR] >= 1) immune = 
IS_RESISTANT;

  if (dam_type == DAM_FIRE && ch->power[DISC_DAEM_HELL] >= 2) immune = 
IS_DIMMUNE;
  if (dam_type == DAM_FIRE && ch->power[DISC_DAEM_HELL] >= 1) immune = 
IS_RESISTANT;

  if (dam_type == DAM_COLD && ch->power[DISC_DAEM_GELU] >= 4) immune = 
IS_DIMMUNE;
  if (dam_type == DAM_COLD && ch->power[DISC_DAEM_GELU] >= 1) immune = 
IS_RESISTANT;

  if (dam_type == DAM_POISON && ch->power[DISC_VAMP_FORT] >= 1) immune = 
IS_RESISTANT;

  if (immune != -1) return immune;

    
 /* set bits to check -- VULN etc. must ALL be the same or this will fail 
*/
  switch (dam_type)
  {
  case(DAM_BASH):         bit = DIMM_BASH;         break;
  case(DAM_PIERCE):       bit = DIMM_PIERCE;       break;
  case(DAM_SLASH):        bit = DIMM_SLASH;        break;
  case(DAM_FIRE):         bit = DIMM_FIRE;         break;
  case(DAM_COLD):         bit = DIMM_COLD;         break;
  case(DAM_LIGHTNING):    bit = DIMM_LIGHTNING;    break;
  case(DAM_ACID):         bit = DIMM_ACID;         break;
  case(DAM_POISON):       bit = DIMM_POISON;       break;
  case(DAM_NEGATIVE):     bit = DIMM_NEGATIVE;     break;
  case(DAM_HOLY):         bit = DIMM_HOLY;         break;
  case(DAM_ENERGY):       bit = DIMM_ENERGY;       break;
  case(DAM_MENTAL):       bit = DIMM_MENTAL;       break;
  case(DAM_DISEASE):      bit = DIMM_DISEASE;      break;
  case(DAM_DROWNING):     bit = DIMM_DROWNING;     break;
  case(DAM_LIGHT):        bit = DIMM_LIGHT;        break;
  case(DAM_CHARM):        bit = DIMM_CHARM;        break;
  case(DAM_SOUND):        bit = DIMM_SOUND;        break;
  default:                return def;
  }

  if (immune == -1) return def;
  else return immune;
}


/*
 * Inflict damage from a hit.
 */
void damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
  CHAR_DATA *emb;
  if ( victim->position == POS_DEAD ) return;
  if ( ch->embracing != NULL )
  {
    if ( ( emb = ch->embracing ) != NULL ) stop_embrace(ch,emb);
    else stop_embrace(ch,NULL);
  }
  /*
   * Stop up any residual loopholes.
   */
  if (ch->damcap[DAM_CHANGE] == 1) update_damcap(ch,victim);
  if ( dam > ch->damcap[0] ) dam = ch->damcap[0];
  
  /*
   * Certain attacks are forbidden.
   * Most other attacks are returned.
   */
  if ( victim != ch )
  {
    if (is_safe( ch, victim )) return;
    check_killer( ch, victim );
    if ( victim->position > POS_STUNNED )
    {
      if ( victim->fighting == NULL ) set_fighting( victim, ch );
      victim->position = POS_FIGHTING;
    }
    if ( victim->position > POS_STUNNED )
    {
      if ( ch->fighting == NULL )
      set_fighting( ch, victim );
      /*
       * If victim is charmed, ch might attack victim's master.
       */
      if ( IS_NPC(ch)
      &&   IS_NPC(victim)
      &&   IS_AFFECTED(victim, AFF_CHARM)
      &&   victim->master != NULL
      &&   victim->master->in_room == ch->in_room
      &&   number_bits( 3 ) == 0 )
      {
        stop_fighting( ch, FALSE );
        multi_hit( ch, victim->master, TYPE_UNDEFINED );
        return;
      }
    }
    if ( victim->master == ch ) stop_follower( victim );
  /*
   * Damage modifiers.
   */
    if (IS_NPC(ch)) 
    {
    dam = ch->level / number_range(1,4);
    if ( IS_AFFECTED(victim, AFF_SANCTUARY))
    dam /= 2;
    }
    if ( IS_AFFECTED(ch, AFF_HIDE) )
    {
      if (!can_see(victim,ch)) 
      {
        dam *= 1.5;
        send_to_char("You use your concealment to get a surprise attack!\n\r",ch);
      }
      REMOVE_BIT( ch->affected_by, AFF_HIDE );
      act( "$n leaps from $s concealment.", ch, NULL, NULL, TO_ROOM , FALSE);
    }
    if ( IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch) && dam > 1  )
      dam -= dam * 0.10;
    if ( IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) && dam > 1  )
      dam -= dam * 0.05;
    if ( dam < 0 ) dam = 0;
    /*
     * Check for disarm, trip, parry, and dodge.
     */
    if ( dt >= TYPE_HIT )
    {
      if (IS_NPC(ch) && number_percent( ) < ch->level * 0.5)
        disarm( ch, victim );
      if (IS_NPC(ch) && number_percent( ) < ch->level * 0.5)
        trip( ch, victim );
      if (check_dodge( ch, victim, dt))
        return;
      if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_MONGOOSE) && 
      victim->stance[STANCE_MONGOOSE] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
        return;
      else if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) && 
      victim->stance[STANCE_SWALLOW] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
        return;
        /* new shit stance super JOBO */
      else if (!IS_NPC(victim) && victim->stance[0] > 12
      && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_DODGE)
      && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
        return;
        /* end of new shit */
      if (check_parry( ch, victim, dt)) return;
      if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE) && 
      victim->stance[STANCE_CRANE] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
        return;
      else if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MANTIS) && 
      victim->stance[STANCE_MANTIS] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
        return;
        /* new shit stance super JOBO */
      else if (!IS_NPC(victim) && victim->stance[0] > 12
      && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_PARRY)
      && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch)
      && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
        return;
        /* end of new shit */
    }
    dam_message( ch, victim, dam, dt );
    if (!IS_NPC(ch) && xIS_SET(ch->act, PLR_BRIEF3))
    {
      ch->amount_damage_dealt += dam;
      ch->amount_attacks_dealt++;
    }
    if (!IS_NPC(victim) && xIS_SET(victim->act, PLR_BRIEF3))
    {
      victim->amount_damage_recieved += dam;
      victim->amount_attacks_recieved++;
    }

  }
  hurt_person(ch,victim,dam);
  dropinvis(ch, victim);
  return;
}

void hurt_person( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int vnum;
  int new_dam;

  victim->hit -= dam;

  if(IS_NPC(victim))
  {
    switch( victim->pIndexData->vnum )
    {
      case 30003:
      case 20050:
      case 30704:
      case 30703:
      case 10000:
      case 10001:
      case 10002:
      case 10003:
      case 10004:
      case 102000:
      {
       if( victim->hit <= 0 ) victim->hit = victim->max_hit;
      }  
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL) && !IS_CLASS(ch, CLASS_ANGEL))
  {
    if (IS_SET(victim->pcdata->powers[ANGEL_POWERS], ANGEL_EYE) && dam > 100)
    {
      new_dam = number_range(dam/3,dam/4);
	if (!IS_CLASS(ch, CLASS_DEMON))
	{
        if (new_dam > 1000) new_dam = 1000;
	}
	if (IS_CLASS(ch, CLASS_DEMON))
	{
	if (new_dam > 1500) new_dam = 1500;
	}
      hurt_person(victim, ch, new_dam);
      sprintf(buf, "God's justice strikes you HARD [#C%d#n]",new_dam);
      sprintf(buf2, "The sinner dares to lay hands on you, God's wrath on the sinner [#C%d#n]", new_dam);
      act(buf, victim, NULL, ch, TO_VICT, FALSE);
      act(buf2, victim, NULL, ch, TO_CHAR, FALSE);
    }
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_PALADIN) && !IS_CLASS(ch, CLASS_PALADIN))
  {
    if (IS_SET(victim->special, SPC_PALADIN_EYE) && dam > 100)
    {
      new_dam = number_range(dam/4,dam/5);
      if (!IS_CLASS(ch, CLASS_ANGEL) && (number_range(1,2)==1))
	{
        if (new_dam > 500) new_dam = 500;
      hurt_person(victim, ch, new_dam);
      sprintf(buf, "#7The Creator's divine fury lashes back at you! #n[#C%d#n]",new_dam);
      sprintf(buf2,"#7Woe to those who do harm to the Creator's Child! #n[#C%d#n]", new_dam);
      act(buf, victim, NULL, ch, TO_VICT, FALSE);
      act(buf2, victim, NULL, ch, TO_CHAR, FALSE);
	}
      if (IS_CLASS(ch, CLASS_ANGEL) && (number_range(1,2)==1))
        {
        if (new_dam > 250) new_dam = 250;
      hurt_person(victim, ch, new_dam);
      sprintf(buf, "#7The Creator's divine fury lashes back at you! #n[#C%d#n]",new_dam);
      sprintf(buf2,"#7Woe to those who do harm to the Creator's Child! #n[#C%d#n]", new_dam);
      act(buf, victim, NULL, ch, TO_VICT, FALSE);
      act(buf2, victim, NULL, ch, TO_CHAR, FALSE);
        }
    }
  }

  if (!IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL && victim->hit < 1) 
    victim->hit = 1;
  if (!IS_NPC(victim) && IS_SET(victim->newbits, NEW_CLOAK) &&victim->hit < 1 )
  {
    victim->hit += UMIN((victim->max_hit * .1), 4000);
    if (IS_CLASS(victim, CLASS_MONK)) send_to_char("your cloak of life saves your from certain death.\n\r",victim);
    else send_to_char("your cloak of death prevents the destruction of your body.\n\r",victim);
    send_to_char("They escape your final blow.\n\r",ch);
    REMOVE_BIT(victim->newbits, NEW_CLOAK);
  }
  update_pos( victim );

  switch( victim->position )
  {
    case POS_MORTAL:
    act( "$n is mortally wounded, and spraying blood everywhere.", victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char("You are mortally wounded, and spraying blood everywhere.\n\r", victim );
    break;
    case POS_INCAP:
    act( "$n is incapacitated, and bleeding badly.",victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char("You are incapacitated, and bleeding badly.\n\r",victim );
    break;
    case POS_STUNNED:
    act( "$n is stunned, but will soon recover.",victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char("You are stunned, but will soon recover.\n\r", victim );
    break;
    case POS_DEAD:
    act( "$n is DEAD!!", victim, 0, 0, TO_ROOM, FALSE );
    send_to_char( "You have been KILLED!!\n\r\n\r", victim );
    break;
    default:
    if ( dam > victim->max_hit / 4 )
       if (!xIS_SET(victim->act, PLR_BRIEF3))
          send_to_char( "That really did HURT!\n\r", victim );
    if ( victim->hit < victim->max_hit / 4 && dam > 0 )
    {
      if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && number_percent() < victim->beast)
        vamp_rage(victim);
      else
         if (!xIS_SET(victim->act, PLR_BRIEF3))
            send_to_char( "You sure are BLEEDING!\n\r", victim );
    }
    break;
  }

  /*
   * Sleep spells and extremely wounded folks.
   */
  if (!IS_AWAKE(victim)) stop_fighting( victim, FALSE );

  /*
   * Payoff for killing things.
   */
  if (victim->hit <= 0 && IS_NPC(victim))
  {
    group_gain( ch, victim );
    victim->position = POS_DEAD;
  }
  
  if ( victim->position == POS_DEAD )
  {
    if (!IS_NPC(victim))
    {
      sprintf( log_buf, "%s killed by %s at %d", victim->name, (IS_NPC(ch) ? ch->short_descr : ch->name), victim->in_room->vnum);
      log_string( log_buf );
      if ( victim->exp > 0 )
        victim->exp = victim->exp / 2;
    }
    if (IS_NPC(victim) && !IS_NPC(ch))
    {
      ch->mkill += 1;
      if ( !IS_NPC( ch ) && ( IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION)
      || IS_CLASS(ch, CLASS_DROW) || IS_CLASS(ch,CLASS_CYBORG) || IS_CLASS(ch, CLASS_TANARRI)
      || IS_CLASS(ch, CLASS_PSIONIST)))
      {
        if ((vnum = victim->pIndexData->vnum) > 29000)
        {
          switch (vnum)
          {
          default:
          if (IS_CLASS(ch, CLASS_DEMON)) 
          {
            ch->pcdata->stats[DEMON_CURRENT] +=victim->level*5;
            ch->pcdata->stats[DEMON_TOTAL] +=victim->level*5;
          }
	  if (IS_CLASS(ch, CLASS_PSIONIST))
	    ch->pcdata->stats[PPOINTS] += victim->level*5;
          else
            ch->pcdata->stats[DROW_POWER] += victim->level*5;
          break;
          case 29600:
          case 30001:
          case 30006:
          case 30007:
          case 30008:
          case 30009:
          case 30000:
          case 90001:
          case 90101:
          case 90201:
          case 90301:
          case 90401:
          case 90501:
          case 90601:
          case 90701:
          case 90801:
          case 90901:
          break;
          }
        }
        else if (IS_NPC(victim) && !xIS_SET(victim->act, ACT_NOEXP))
        {
          if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special,SPC_CHAMPION)) 
          {
            ch->pcdata->stats[DEMON_CURRENT] += victim->level*4;
            ch->pcdata->stats[DEMON_TOTAL] += victim->level*4;
          }
          else
            ch->pcdata->stats[DROW_POWER] += victim->level*4;
        }
      }
      if (ch->level == 1 && ch->mkill > 4)
      {
        ch->level = 2;
        do_save(ch,"");
      }
    }
    if (!IS_NPC(victim) && IS_NPC(ch)) 
    {
      victim->mdeath = victim->mdeath + 1;
    }
    if (IS_NPC(victim))
    {
	if (victim->level >= 10000)
	{
	   if (number_range(1,20) == 1)
	   {
	     ch->max_hit += 5;
	     sprintf(buf, "You are awared 5 hit points for an excellent victory.\n\r");
	     stc(buf,ch);
	   }
	}
        else if (victim->level < 10000 && victim->level >= 8000)
        {
           if (number_range(1,20) == 1)
           {
             ch->max_hit += 4;
             sprintf(buf, "You are awared 4 hit points for a nice victory.\n\r");
             stc(buf,ch);
           }
        }
        else if (victim->level < 8000 && victim->level >= 6000)
        {
           if (number_range(1,20) == 1)
           {
             ch->max_hit += 3;
             sprintf(buf, "You are awared 3 hit points for a good victory.\n\r");
             stc(buf,ch);
           }
        }
        else if (victim->level < 6000 && victim->level >= 4000)
        {
           if (number_range(1,20) == 1)
           {
             ch->max_hit += 2;
             sprintf(buf, "You are awared 2 hit points for a victory.\n\r");
             stc(buf,ch);
           }
        }
        else if (victim->level < 4000 && victim->level >= 2000)
        {
           if (number_range(1,20) == 1)
           {
             ch->max_hit += 1;
             sprintf(buf, "You are awared 1 hit points for a victory.\n\r");
             stc(buf,ch);
           }
        }
    }
    raw_kill( victim );
    if (!IS_NPC(ch) && !IS_NPC(victim) && victim->pcdata->bounty > 0)
    {
      sprintf(buf,"You recive a %d QP bounty, for killing %s.\n\r",
      victim->pcdata->bounty, victim->name);
      send_to_char(buf, ch);
      ch->pcdata->score[SCORE_QUEST] += victim->pcdata->bounty;
      ch->pcdata->quest += victim->pcdata->bounty;
      victim->pcdata->bounty =0;  
    }
    if (xIS_SET(ch->act, PLR_AUTOGOLD))
	do_get(ch, "coins corpse" );
    if (xIS_SET(ch->act, PLR_AUTOSLAB))
	do_get(ch, "adamantite corpse" );
    if (xIS_SET(ch->act, PLR_AUTOELEC))
        do_get(ch, "electrum corpse" );
    if (xIS_SET(ch->act, PLR_AUTOLOOT) )
      do_get( ch, "all corpse" );
    else
      do_look( ch, "in corpse" );
    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
      if ( xIS_SET(ch->act, PLR_AUTOSAC) )
        do_sacrifice( ch, "corpse" );
    }
    return;
  }
  if ( victim == ch )
    return;
  if ( !IS_NPC(victim) && victim->desc == NULL )
  {
    if ( number_range( 0, victim->wait ) == 0 )
    {
      do_recall( victim, "" );
      return;
    }
  }

/*  if ( !IS_NPC(victim) && victim->hit > 0 && victim->hit <= victim->wimpy && 
victim->wait == 0)
    do_flee( victim, "" );
*/
  tail_chain( );
  return;
}

bool is_range( CHAR_DATA *ch, CHAR_DATA *victim)
{
int range;
if (IS_NPC(victim)) return FALSE;
if (IS_NPC(ch)) return FALSE;
if (victim == ch) return TRUE;
if (!IS_SET(ch->in_room->room_flags, ROOM_ARENA) || 
!IS_SET(victim->in_room->room_flags, ROOM_ARENA))
{
 if (!IS_NPC(ch) && IS_NPC(victim)) return FALSE;
  if (IS_SET(ch->special, SPC_NEWBIE) && !IS_NPC(ch) && !IS_NPC(victim))
    return TRUE;
  if (IS_SET(victim->special, SPC_NEWBIE) && !IS_NPC(victim) && !IS_NPC(ch))
    return TRUE;

  if (ch->race > victim->race)
    range = ch->race - victim->race;
  else if (victim->race > ch->race)
    range = victim->race - ch->race;
  else if (ch->race == victim->race)
    range = 0;
  if (ch->race > 0 && victim->race == 0)  return TRUE;
  if (ch->race == 0 && victim->race > 0)  return TRUE;
  if (ch->race == 0)
  {
	if (range == 0) return FALSE;
	 else return TRUE;
  }
  if (ch->race > 0)
  {
	if (ch->race <= 10)
	{
	  if (victim->race <= 10)
  	    if (range > 5) return TRUE;
	  if (victim->race > 10)
	    if (range > 5) return TRUE;
	}
	if (ch->race > 10)
	{
          if (victim->race <= 10)
            if (range > 5) return TRUE;
          if (victim->race > 10)
  	    if (range > 10) return TRUE;
	}
  }

if (ch->level > 6) return FALSE;
}
else if (IS_SET(ch->in_room->room_flags, ROOM_ARENA) && 
!IS_SET(victim->in_room->room_flags, ROOM_ARENA))
 {
  if (ch->level > 6) return TRUE;
  if (is_fighting_player(victim,ch)) return FALSE;
 }
 return FALSE;
}

bool is_safe( CHAR_DATA *ch, CHAR_DATA *victim )
{
  int range;
  if (!IS_NPC(victim) && !IS_NPC(ch))
  {
    if ( !CAN_PK(ch) || !CAN_PK(victim) )
    {
      send_to_char( "Both players must be avatars to fight.\n\r", ch );
      return TRUE;
    }  
  }
    if (!IS_NPC(ch) && !IS_NPC(victim))
    {
    if (IS_SET(ch->in_room->room_flags, ROOM_SAFE))
    {
	stc("You are in safe room, NO VIOLENCE!\n\r",ch);
        return TRUE;
    }
    if (IS_SET(victim->in_room->room_flags, ROOM_SAFE))
    {
        stc("They are in safe room, NO VIOLENCE!\n\r",ch);
        return TRUE;
    }

    if (!xIS_SET(ch->extra, EXTRA_POLICY) && !xIS_SET(victim->extra, EXTRA_POLICY))
    {
	stc("Neither of you have accepted the policy! (help policy)\n\r",ch);
	return TRUE;
    }
    if (!xIS_SET(ch->extra, EXTRA_POLICY) && xIS_SET(victim->extra, EXTRA_POLICY))
    {
        stc("They have accepted the policy but you have not! (help policy)\n\r",ch);
        return TRUE;
    }
    if (xIS_SET(ch->extra, EXTRA_POLICY) && !xIS_SET(victim->extra, EXTRA_POLICY))
    {
        stc("You have accepted the policy but they have not! (help policy)\n\r",ch);
        return TRUE;
    }
    }

    if (IS_SET(ch->special, SPC_NEWBIE) && !IS_NPC(ch) && !IS_NPC(victim) && (victim != ch))
    {
      send_to_char("You cannot attack other chars while you are a newbie\n\r",ch);
      return TRUE;
    }
    if (IS_SET(victim->special, SPC_NEWBIE) && !IS_NPC(victim) && !IS_NPC(ch) && (victim != ch))
    {
    stc("You cannot attack newbies!\n\r",ch);
    return TRUE;
    }

    if (is_fighting_player(victim, ch) && !IS_SET(ch->in_room->room_flags, ROOM_ARENA))
    {
	stc("They are fighting somebody else.\n\r",ch);
	return TRUE;
    } 
  
  if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
  {
    send_to_char( "Objects cannot fight!\n\r", ch );
    return TRUE;
  }
  if ( IS_HEAD(victim,LOST_HEAD) || IS_EXTRA(victim,EXTRA_OSWITCH))
  {
    send_to_char( "You cannot attack an object.\n\r", ch );
    return TRUE;
  }
  if ( !IS_NPC(ch ) )
    if (IS_AFFECTED(ch, AFF_PEACE) && ch->fighting != NULL) REMOVE_BIT(ch->affected_by, AFF_PEACE);
  if ( !IS_NPC(victim) )
    if (IS_AFFECTED(victim, AFF_PEACE) && victim->fighting != NULL) REMOVE_BIT(victim->affected_by, AFF_PEACE);

  if (victim->fighting == ch) return FALSE;
  if (IS_AFFECTED(ch, AFF_PEACE))
  {
    send_to_char( "You are unable to attack them.\n\r", ch );
    return TRUE;
  }
  if (IS_AFFECTED(victim, AFF_PEACE))
  {
    send_to_char( "You can't seem to attack them.\n\r", ch );
    return TRUE;
  }
   if (IS_SET(ch->affected_by, AFF_ETHEREAL))
   {
    send_to_char( "You cannot while ethereal.\n\r", ch );
    return TRUE;
    }
   if (IS_SET(victim->affected_by, AFF_ETHEREAL))
  {
    send_to_char( "You cannot fight an ethereal person.\n\r", ch );
    return TRUE;
  }

   if (IS_SET(ch->affected_by, AFF_SHADOWPLANE))
  {
    act( "You are too insubstantial!", ch, NULL, victim, TO_CHAR, FALSE );
    return TRUE;
  }
   if (IS_SET(victim->affected_by, AFF_SHADOWPLANE))
  {
    act( "$E is too insubstantial!", ch, NULL, victim, TO_CHAR , FALSE);
    return TRUE;
  }
  if (!IS_NPC(ch) && !IS_NPC(victim))
  {
  if (!IS_SET(ch->in_room->room_flags, ROOM_ARENA) || !IS_SET(victim->in_room->room_flags, ROOM_ARENA))
  {

  if (ch->race > victim->race)
    range = ch->race - victim->race;
  else if (victim->race > ch->race)
    range = victim->race - ch->race;
  else if (ch->race == victim->race)
    range = 0;

  if (ch->race > 0 && victim->race == 0)
  {
    stc("They are not within your range! (help range.)\n\r",ch);
    return TRUE;
  }

  if (ch->race == 0 && victim->race > 0)
  {
    stc("They are not within your range! (help range.)\n\r",ch);
    return TRUE;
  }


  if (ch->race == 0)
  {
        if (range == 0) 
	  return FALSE;
        else if (range > 0)
        {
          stc("They are not within your range! (help range.)\n\r",ch);
	  return TRUE;
	}
  }
  if (ch->race > 0)
  {
        if (ch->race <= 10)
        {
          if (victim->race <= 10)
            if (range > 5)
            {
              stc("They are not within your range! (help range.)\n\r",ch);
              return TRUE;
            }
          if (victim->race > 10)
            if (range > 5)
            {
              stc("They are not within your range! (help range.)\n\r",ch);
              return TRUE;
            }
        }
        if (ch->race > 10)
        {
          if (victim->race <= 10)
            if (range > 5)
            {
              stc("They are not within your range! (help range.)\n\r",ch);
              return TRUE;
            }
          if (victim->race > 10)
            if (range > 10)
            {
              stc("They are not within your range! (help range.)\n\r",ch);
              return TRUE;
            }
        }
  }
 }
if (IS_SET(ch->in_room->room_flags, ROOM_ARENA) && IS_SET(victim->in_room->room_flags,
ROOM_ARENA))
 {
 if (is_fighting_player(victim, ch)) return FALSE;
 }


  }
  if (IS_NPC(victim)) return FALSE;
  if (IS_NPC(ch)) return FALSE;

return FALSE;
}

/*
 * See if an attack justifies a KILLER flag.  ** This function does NOTHING - JOBO **
 */
void check_killer( CHAR_DATA *ch, CHAR_DATA *victim )
{
  /*
   * Follow charm thread to responsible character.

   * Attacking someone's charmed char is hostile!
   */
  while (IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL )
    victim = victim->master;
  /*
   * NPC's are fair game.
   * So are killers and thieves.
   */
  if (IS_NPC(victim))
    return;

  return;
}

/*
 * Check for parry.
 */
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
  OBJ_DATA *obj = NULL;
  int chance = 0;
  bool claws = FALSE;

  if (!IS_AWAKE(victim)) return FALSE;
  if (IS_NPC(victim))  obj = NULL;
  else if (IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_BEAR] > 2 
  && IS_VAMPAFF(victim, VAM_CLAWS) && get_eq_char(victim, WEAR_WIELD) == NULL 
  && get_eq_char(victim, WEAR_HOLD) == NULL)
  {
    obj = NULL;
    claws = TRUE;
  }
  else if ( IS_CLASS(victim, CLASS_MONK) && IS_SET(victim->newbits, NEW_MONKADAM) 
  && get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL)
  {
    obj = NULL;
    claws = TRUE;
  }
  else if( IS_CLASS(victim, CLASS_SHAPESHIFTER) && get_eq_char(victim, WEAR_WIELD) == NULL 
  && get_eq_char(victim, WEAR_HOLD) == NULL)
  {
    obj = NULL;
    claws = TRUE;
  }
  else
  {
    if ((obj = get_eq_char(victim, WEAR_WIELD)) == NULL || obj->item_type != ITEM_WEAPON)
    {
      if ((obj = get_eq_char(victim, WEAR_HOLD)) == NULL || obj->item_type != ITEM_WEAPON) 
        return FALSE;
    }
  }
  if ( ( dt < 1000 || dt > 1012) && !IS_CLASS(ch, CLASS_MONK) ) return FALSE;
  if (  dt >= 1000 && dt <= 1012 )
  {
    if (!IS_NPC(ch)) chance -= (ch->wpn[dt-1000] * 0.1);
    else chance -= (ch->level * 0.2);
    if (!IS_NPC(victim)) chance += (victim->wpn[dt-1000] * 0.5);
    else chance += victim->level;
  }
  else
  {
    if (!IS_NPC(ch)) chance -= (ch->wpn[0] * 0.1);
    else chance -= (ch->level * 0.2);
    if (!IS_NPC(victim)) chance += (victim->wpn[0] * 0.5);
    else chance += victim->level;
  }
  if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE) && victim->stance[STANCE_CRANE] > 0 
  && !can_counter(ch) && !can_bypass(ch,victim))
    chance += (victim->stance[STANCE_CRANE] * 0.25);
  else if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MANTIS) && victim->stance[STANCE_MANTIS] > 0 
  && !can_counter(ch) && !can_bypass(ch,victim))  
    chance += (victim->stance[STANCE_MANTIS] * 0.25);
  /* new shit stance super JOBO */ /* DETTE VIRKER, lev med det */
  if (!IS_NPC(victim) && victim->stance[0] > 12 
  && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_PARRY)
  && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch) && !can_bypass(ch,victim))
    chance += victim->stance[(victim->stance[0])] * 0.25;
  /* new shit stops here */
  
  chance -= (char_hitroll(ch) * 0.1);
  if (claws) 
  {
     if (victim->power[DISC_WERE_LYNX] > 3) chance += (char_hitroll(victim) * 0.1);
     else chance += (char_hitroll(victim) * 0.075);
  }
  if (!IS_NPC(ch))
  {
    if (IS_CLASS(ch, CLASS_POWER_LICH)) chance -= 40;
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
	 chance -= ch->pcdata->powers[TIGER_LEVEL]*8;
      else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
	 chance -= ch->pcdata->powers[BULL_LEVEL]*6;
      else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
	 chance -= ch->pcdata->powers[HYDRA_LEVEL]*6;
      else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
	 chance -= ch->pcdata->powers[FAERIE_LEVEL]*7;
      else chance -= 10;
    }
    if (IS_CLASS(ch, CLASS_TANARRI))
    {
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 20;
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 20;
      if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 10;
    }
    if (IS_CLASS(ch, CLASS_PSIONIST))
    {
      if (IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS)) chance -= 40;
      if (ch->pcdata->powers[PSIONIST_WRATH_ON] == 1) chance -= 10;
    }

    if (IS_CLASS(ch, CLASS_ANGEL)) chance -= ch->pcdata->powers[ANGEL_JUSTICE]*7;
    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_CELE] > 0) chance -= (ch->power[DISC_VAMP_CELE]*4);
    if (IS_CLASS(ch, CLASS_NINJA)  && ch->pcdata->powers[NPOWER_CHIKYU] >= 6 && ch->pcdata->powers[HARA_KIRI] > 0) chance -= 45;
    if (ch->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(ch, CLASS_NINJA)) chance -= 20;
    if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMPOWER(ch, DEM_SPEED)) chance -= 45;
    if (IS_CLASS(ch, CLASS_MONK) && ch->chi[CURRENT] > 0) chance -= (ch->chi[CURRENT] * 9);
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL]*5;
    if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 30;
    if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_ARMS)) chance -= 45;
    if (IS_SET(ch->newbits, NEW_MONKFLAME)) chance -= 10;
    if (IS_CLASS(ch, CLASS_WEREWOLF)) chance -= (ch->power[DISC_WERE_MANT] * 4);
    if (IS_CLASS(ch, CLASS_HIGHLANDER)) chance -= 45;
    if (IS_CLASS(ch, CLASS_PALADIN)) chance -= 45;
    if (IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 7) chance -= 40;
    if (IS_ITEMAFF(ch, ITEMA_AFFMANTIS)) chance -= 12;
  }
  if (!IS_NPC(victim))
  {
    if (IS_CLASS(victim, CLASS_POWER_LICH)) chance += 40;
    if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
    {
      if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
	 chance += victim->pcdata->powers[TIGER_LEVEL]*8;
      else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
	 chance += victim->pcdata->powers[BULL_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
	 chance += victim->pcdata->powers[HYDRA_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
	 chance += victim->pcdata->powers[FAERIE_LEVEL]*7;
      else chance += 10;
    }
    if (IS_CLASS(victim, CLASS_TANARRI))
    {
      if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 20;
      if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 20;
      if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance +=10;
    }
    if (IS_CLASS(victim, CLASS_PSIONIST) && !IS_NPC(victim))
    {
      if (IS_SET(victim->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS)) chance += 40;
      if (victim->pcdata->powers[PSIONIST_WRATH_ON] == 1) chance += 10;
    }

    if (IS_CLASS(victim, CLASS_ANGEL)) chance += victim->pcdata->powers[ANGEL_PEACE]*7;
    if (victim->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(victim, CLASS_NINJA)) chance 
+= 45;
    if (victim->power[DISC_VAMP_CELE] > 0 &&  IS_CLASS(victim, CLASS_VAMPIRE)) chance += (victim->power[DISC_VAMP_CELE]*4);
    if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_ARMS)) chance += 45;
    if (IS_CLASS(victim, CLASS_MONK) && ch->chi[CURRENT] > 0 ) chance += (ch->chi[CURRENT] * 9);
    if (IS_SET(victim->newbits, NEW_MONKFLAME)) chance += 10;
    if (IS_CLASS(victim, CLASS_HIGHLANDER)) chance +=45;
    if (IS_CLASS(victim, CLASS_PALADIN)) chance += 45;
    if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*5;
    if (IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) chance += 40;
    if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER(victim, DEM_SPEED)) chance += 45;
    if (IS_CLASS(victim, CLASS_WEREWOLF)) chance += (victim->power[DISC_WERE_MANT] * 4);
    if (IS_CLASS(victim, CLASS_INQUISITOR) && victim->pcdata->powers[PINQUISITOR] > 7) chance -=40;
 
 }
  if (chance > 80) chance = 80;
  else if (chance < 20) chance = 20;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_POWER_LICH)) chance -= 17;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_POWER_LICH)) chance += 17;
  if (!IS_NPC(ch) && ch->power[DISC_VAMP_CELE] > 0 && IS_CLASS(ch, CLASS_VAMPIRE))
  {
    if( ch->pcdata->rank == AGE_TRUEBLOOD)  chance -= 30;
    else if( ch->pcdata->rank == AGE_LA_MAGRA) chance -= 17;
    else if (ch->pcdata->rank == AGE_METHUSELAH) chance -= 15;
    else if (ch->pcdata->rank == AGE_ELDER) chance -= 10;
    else if (ch->pcdata->rank == AGE_ANCILLA) chance -= 5;    
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF)) chance -= (ch->power[DISC_WERE_MANT] *4);
    if (IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 7) chance -= 40;

    if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
         chance -= ch->pcdata->powers[TIGER_LEVEL]*8;
      else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
         chance -= ch->pcdata->powers[BULL_LEVEL]*6;
      else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
         chance -= ch->pcdata->powers[HYDRA_LEVEL]*6;
      else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
         chance -= ch->pcdata->powers[FAERIE_LEVEL]*7;
      else chance -= 10;
    }
    if (IS_CLASS(ch, CLASS_PSIONIST))
    {
      if (IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS)) chance -= 40;
      if (ch->pcdata->powers[PSIONIST_WRATH_ON] == 1) chance -= 10;
    }
    if (IS_CLASS(victim, CLASS_PSIONIST) && !IS_NPC(victim))
    {
      if (IS_SET(victim->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS)) chance -= 40;
      if (victim->pcdata->powers[PSIONIST_WRATH_ON] == 1) chance -= 10;
    }

  if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
  {
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 15;
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 15;
    if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 10;
  }

  if (IS_CLASS(victim, CLASS_TANARRI) && !IS_NPC(victim))
  {
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 15;
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 15;
    if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 10;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL)) chance -= 
ch->pcdata->powers[ANGEL_JUSTICE]*7;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL)) chance += 
victim->pcdata->powers[ANGEL_PEACE]*7;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
    {
      if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
         chance += victim->pcdata->powers[TIGER_LEVEL]*8;
      else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
         chance += victim->pcdata->powers[BULL_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
         chance += victim->pcdata->powers[HYDRA_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
         chance += victim->pcdata->powers[FAERIE_LEVEL]*7;
      else chance += 10;
    }

  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 30;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL]*1.5;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*1.5;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))                                   
  {
    if( ch->pcdata->rank == BELT_TEN)  chance -= 40;     
    else if( ch->pcdata->rank == BELT_NINE) chance -= 26;   
    else if (ch->pcdata->rank == BELT_EIGHT) chance -= 22;    
    else if (ch->pcdata->rank == BELT_SEVEN) chance -= 18;
    else if (ch->pcdata->rank == BELT_SIX) chance -= 14;  
    else if (ch->pcdata->rank == BELT_FIVE) chance -= 10;
    else if (ch->pcdata->rank == BELT_FOUR) chance -= 6;
    else if (ch->pcdata->rank == BELT_THREE) chance -= 2;
  }
  if(!IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON)) chance -= (ch->power[DISC_DAEM_ATTA] * 4);
  if(!IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON)) chance += (victim->power[DISC_DAEM_ATTA] *4);
  if (!IS_NPC(victim) && victim->power[DISC_VAMP_CELE] > 0 && IS_CLASS(victim, CLASS_VAMPIRE))
  {
    if( victim->pcdata->rank == AGE_TRUEBLOOD)  chance += 30;
    else if( victim->pcdata->rank == AGE_LA_MAGRA) chance += 17;
    else if (victim->pcdata->rank == AGE_METHUSELAH) chance += 15;
    else if (victim->pcdata->rank == AGE_ELDER) chance += 10;
    else if (victim->pcdata->rank == AGE_ANCILLA) chance += 5;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF)) chance += (victim->power[DISC_WERE_MANT] * 4);
    if (IS_CLASS(victim, CLASS_INQUISITOR) && victim->pcdata->powers[PINQUISITOR] > 7) chance -=40;

  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
  {
    if( victim->pcdata->rank == BELT_TEN)  chance += 40;
    else if( victim->pcdata->rank == BELT_NINE) chance += 30;
    else if (victim->pcdata->rank == BELT_EIGHT) chance += 27;
    else if (victim->pcdata->rank == BELT_SEVEN) chance += 24;
    else if (victim->pcdata->rank == BELT_SIX) chance += 21;
    else if (victim->pcdata->rank == BELT_FIVE) chance += 18;
    else if (victim->pcdata->rank == BELT_FOUR) chance += 15;
    else if (victim->pcdata->rank == BELT_THREE) chance += 12;
    else if (victim->pcdata->rank == BELT_TWO) chance += 9;
    else if (victim->pcdata->rank == BELT_ONE) chance += 6;
  }

  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_HIGHLANDER)) chance += 35;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_PALADIN)) chance += 35;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_HIGHLANDER)) chance -=35;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PALADIN)) chance -= 35;
  if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_CYBORG))
  if ((ch->pcdata->powers[CYBORG_LIMBS] > 1)) chance -= 45;
  if (victim->monkblock / 5 > 0 ) chance += victim->monkblock / 4;
  if (number_percent( ) < 100 && number_percent( ) >= chance)
    return FALSE;
  if (!IS_NPC(ch) && IS_NPC(victim)
  && IS_SET(ch->newbits, NEW_FIGHTDANCE)
  && IS_CLASS(ch, CLASS_DROW) 
  && number_percent() >=50
  && ((( obj = get_eq_char(ch, WEAR_WIELD ))  != NULL && ( obj->value[3] == 4 || obj->value[3] == 11))
      || ((obj = get_eq_char (ch, WEAR_HOLD))!= NULL &&( obj->value[3] == 4 || obj->value[3] == 11))))
  {
    act( "$n's dark weapon dances around your defences, making a strange sound.",  ch, NULL, victim, TO_VICT, FALSE );
    act( "Your sacrificial weapon hums in the air as it dances $N's defences.", ch, obj, victim, TO_CHAR, FALSE );
    return FALSE;
  }
  if (claws)
  {
    if (IS_NPC(victim) )
    {
      if (IS_CLASS(victim, CLASS_WEREWOLF))
        act( "You parry $n's blow with your claws.",  ch, NULL, victim, TO_VICT, TRUE );
      else
        act( "You parry $n's blow with your Adamantium hands.", ch,NULL,victim, TO_VICT, TRUE);
    }
    if (IS_CLASS(victim, CLASS_MONK) && victim->monkblock < 150)
      victim->monkblock += 2;
    if (IS_NPC(ch) || !xIS_SET(ch->act, PLR_BRIEF2))
    {
    if (IS_CLASS(victim, CLASS_WEREWOLF))
      act( "$N parries your blow with $S claws.", ch, NULL, victim, TO_CHAR, TRUE );
    else
      act( "$N parries your blow with $S Adamantium claws.", ch,NULL,victim, TO_CHAR, TRUE);
    }
    return TRUE;
  }
  if (!IS_NPC(victim) && obj != NULL && obj->item_type == ITEM_WEAPON && obj->value[3] >= 0 && obj->value[3] <= 12)
  {
    if (IS_NPC(victim))
      act( "You parry $n's blow with $p.",  ch, obj, victim, TO_VICT, TRUE );
    if (IS_NPC(ch))
      act( "$N parries your blow with $p.", ch, obj, victim, TO_CHAR, TRUE );
    return TRUE;
  }
  if (IS_NPC(victim))
    act( "You parry $n's attack.",  ch, NULL, victim, TO_VICT, TRUE    );
  if (IS_NPC(ch))
    act( "$N parries your attack.", ch, NULL, victim, TO_CHAR, TRUE    );
  return TRUE;
}
   
/*
 * Check for dodge.
 */
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
  int chance = 0;

  if (!IS_AWAKE(victim)) return FALSE;
  if (!IS_NPC(ch)) chance -= (ch->wpn[dt-1000] * 0.1);
  else chance -= (ch->level * 0.2);
  if (!IS_NPC(victim)) chance += (victim->wpn[0] * 0.5);
  else chance += victim->level;
  if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MONGOOSE) && victim->stance[STANCE_MONGOOSE] > 0 
  && !can_counter(ch) &&!can_bypass(ch,victim))
    chance += victim->stance[STANCE_MONGOOSE] * 0.25;
  if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) && victim->stance[STANCE_SWALLOW] > 0 
  && !can_counter(ch) && !can_bypass(ch,victim))
    chance += victim->stance[STANCE_SWALLOW] * 0.25;
  /* new shit stance super JOBO */ /* DETTE VIRKER, lev med det */
  if (!IS_NPC(victim) && victim->stance[0] > 12 
  && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_DODGE)
  && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch) && !can_bypass(ch,victim))
    chance += victim->stance[(victim->stance[0])] * 0.25;
  /* new shit stops here */
  if (!IS_NPC(ch))
  {
    if (IS_CLASS(ch, CLASS_POWER_LICH)) chance -= 40;
    if (IS_CLASS(ch, CLASS_MONK)) chance -= ch->chi[CURRENT]*9;
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
         chance -= ch->pcdata->powers[TIGER_LEVEL]*8;
      else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
         chance -= ch->pcdata->powers[BULL_LEVEL]*6;
      else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
         chance -= ch->pcdata->powers[HYDRA_LEVEL]*6;
      else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
         chance -= ch->pcdata->powers[FAERIE_LEVEL]*7;
      else chance -= 10;
    }
    if (IS_CLASS(ch, CLASS_TANARRI))
    {
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 20;
      if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 20;
      if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 10;
    }
    if (IS_CLASS(ch, CLASS_PSIONIST))
    {
      if (IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS)) chance -= 40;
      if (ch->pcdata->powers[PSIONIST_WRATH_ON] == 1) chance -= 10;
    }

    if (IS_CLASS(ch, CLASS_ANGEL)) chance -= ch->pcdata->powers[ANGEL_JUSTICE]*7;
    if (IS_CLASS(ch, CLASS_NINJA)  && ch->pcdata->powers[NPOWER_CHIKYU] >= 6 && ch->pcdata->powers[HARA_KIRI] > 0)
      chance -= 45;
    if (ch->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(ch, CLASS_NINJA)) chance -= 25;
    if (ch->power[DISC_VAMP_CELE] > 0 && IS_CLASS(ch, CLASS_VAMPIRE)) chance -= (ch->power[DISC_VAMP_CELE]*4);
    if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMPOWER( ch, DEM_SPEED)) chance -= 45;
    if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 30;
    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_MANT] > 6) chance -= (ch->power[DISC_WERE_MANT] * 4);
    if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_SPEED)) chance -= 50;
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL]*5;
    if (IS_CLASS(ch, CLASS_HIGHLANDER)) chance -= 40;
    if (IS_CLASS(ch, CLASS_PALADIN)) chance -= 40;
    if (IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 7) chance -= 40;

  }
  if (!IS_NPC(victim))
  {
    if (IS_CLASS(victim, CLASS_POWER_LICH)) chance += 40;
    if (IS_CLASS(victim, CLASS_MONK)) chance += victim->chi[CURRENT]*9;
    if (victim->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(victim, CLASS_NINJA)) chance += 40;
    if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
    {
      if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
         chance += victim->pcdata->powers[TIGER_LEVEL]*8;
      else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
         chance += victim->pcdata->powers[BULL_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
         chance += victim->pcdata->powers[HYDRA_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
         chance += victim->pcdata->powers[FAERIE_LEVEL]*7;
      else chance += 10;
    }

    if (IS_CLASS(victim, CLASS_TANARRI))
    {
      if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 20;
      if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 20;
      if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 10;
    }
    if (IS_CLASS(victim, CLASS_PSIONIST) && !IS_NPC(victim))
    {
      if (IS_SET(victim->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS)) chance -= 40;
      if (victim->pcdata->powers[PSIONIST_WRATH_ON] == 1) chance -= 10;
    }

    if (IS_CLASS(victim, CLASS_ANGEL)) chance += victim->pcdata->powers[ANGEL_PEACE]*7;
    if (victim->power[DISC_VAMP_CELE] > 0 && IS_CLASS(victim, CLASS_VAMPIRE)) chance += (victim->power[DISC_VAMP_CELE]*4);
    if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER( victim, DEM_SPEED)) chance += 45;
    if (IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_MANT] > 6) chance +=(victim->power[DISC_WERE_MANT] * 4);
    if (IS_CLASS(victim, CLASS_DROW) &&  IS_SET(victim->pcdata->powers[1], DPOWER_SPEED)) chance += 50;
    if (IS_CLASS(victim, CLASS_HIGHLANDER)) chance +=45;
    if (IS_CLASS(victim, CLASS_PALADIN)) chance += 45;
    if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*5;
    if (!IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(victim,ITEMA_AFFMANTIS)) chance += 15;
    if (IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) chance += 40;
    if (IS_CLASS(victim, CLASS_INQUISITOR) && victim->pcdata->powers[PINQUISITOR] > 7) chance += 40;

  }
  if (chance > 80) chance = 80;
  else if (chance < 20) chance = 20;
  if (IS_CLASS(ch, CLASS_POWER_LICH) && !IS_NPC(ch)) chance -= 17;
  if (IS_CLASS(victim, CLASS_POWER_LICH) && !IS_NPC(victim)) chance += 17;
  if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && !IS_NPC(ch))
    {
      if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
         chance -= ch->pcdata->powers[TIGER_LEVEL]*5;
      else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
         chance -= ch->pcdata->powers[BULL_LEVEL]*4;
      else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
         chance -= ch->pcdata->powers[HYDRA_LEVEL]*4;
      else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
         chance -= ch->pcdata->powers[FAERIE_LEVEL]*5;
      else chance -= 10;
    }
  if (IS_CLASS(victim, CLASS_SHAPESHIFTER) && !IS_NPC(victim))
    {
      if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
         chance += victim->pcdata->powers[TIGER_LEVEL]*8;
      else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
         chance += victim->pcdata->powers[BULL_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
         chance += victim->pcdata->powers[HYDRA_LEVEL]*6;
      else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
         chance += victim->pcdata->powers[FAERIE_LEVEL]*7;
      else chance += 10;
    }
    if (IS_CLASS(ch, CLASS_PSIONIST))
    {
      if (IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS)) chance -= 40;
      if (ch->pcdata->powers[PSIONIST_WRATH_ON] == 1) chance -= 10;
    }
    if (IS_CLASS(victim, CLASS_PSIONIST) && !IS_NPC(victim))
    {
      if (IS_SET(victim->pcdata->powers[PSIONIST_POWER], PSIONIST_QUICKNESS)) chance += 40;
      if (victim->pcdata->powers[PSIONIST_WRATH_ON] == 1) chance += 10;
    }

  if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
  {  
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 15;
    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 15;
    if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 10;
  }
  if (IS_CLASS(victim, CLASS_TANARRI) && !IS_NPC(victim))
  {  
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 15;
    if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 10;
    if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 10;
  }
    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_MANT] > 6)
chance -= (ch->power[DISC_WERE_MANT] * 4);
    if (!IS_CLASS(ch, CLASS_WEREWOLF) && (IS_ITEMAFF(ch,ITEMA_AFFMANTIS))) chance -= 12;
    if (IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_MANT] > 6)
chance += (victim->power[DISC_WERE_MANT] * 4);
    if (!IS_CLASS(victim, CLASS_WEREWOLF) && (IS_ITEMAFF(victim,ITEMA_AFFMANTIS))) chance += 12;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL)) chance -= 
ch->pcdata->powers[ANGEL_JUSTICE]*7;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL)) chance += 
victim->pcdata->powers[ANGEL_PEACE]*7;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_SPEED)) chance += 40;
  if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER( victim, DEM_SPEED)) chance += 45;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL]*1.5;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*1.5;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_HIGHLANDER)) chance -= 35;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PALADIN)) chance -= 35;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_HIGHLANDER)) chance +=35;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_PALADIN)) chance += 35;
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 30;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_BEAST)) chance += 30;
    if (IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 7) chance -=40;
    if (IS_CLASS(victim, CLASS_INQUISITOR) && victim->pcdata->powers[PINQUISITOR] > 7) chance +=40;
    if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_ARMS)) chance -= 45;

  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
  {
    if( ch->pcdata->rank == AGE_TRUEBLOOD)  chance -= 35;
    else if( ch->pcdata->rank == AGE_LA_MAGRA) chance -= 17;
    else if (ch->pcdata->rank == AGE_METHUSELAH) chance -= 10;
    else if (ch->pcdata->rank == AGE_ELDER) chance -= 7;
    else if (ch->pcdata->rank == AGE_ANCILLA) chance -= 3;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
  {
    if( ch->pcdata->rank == BELT_TEN)  chance -= 40;
    else if( ch->pcdata->rank == BELT_NINE) chance -= 28;
    else if (ch->pcdata->rank == BELT_EIGHT) chance -= 27;
    else if (ch->pcdata->rank == BELT_SEVEN) chance -= 24;
    else if (ch->pcdata->rank == BELT_SIX) chance -= 21;
    else if (ch->pcdata->rank == BELT_FIVE) chance -= 18;
    else if (ch->pcdata->rank == BELT_FOUR) chance -= 15;
    else if (ch->pcdata->rank == BELT_THREE) chance -= 12;
    else if (ch->pcdata->rank == BELT_TWO) chance -= 9;
    else if (ch->pcdata->rank == BELT_ONE) chance -= 6;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))
  {
    if(victim->pcdata->rank == AGE_TRUEBLOOD)  chance += 35;
    else if(victim->pcdata->rank == AGE_LA_MAGRA) chance += 17;
    else if (victim->pcdata->rank == AGE_METHUSELAH) chance += 10;
    else if (victim->pcdata->rank == AGE_ELDER) chance += 7;
    else if (victim->pcdata->rank == AGE_ANCILLA) chance += 3;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
  {
    if( victim->pcdata->rank == BELT_TEN)  chance += 40;
    else if( victim->pcdata->rank == BELT_NINE) chance += 28;
    else if (victim->pcdata->rank == BELT_EIGHT) chance += 27;
    else if (victim->pcdata->rank == BELT_SEVEN) chance += 24;
    else if (victim->pcdata->rank == BELT_SIX) chance += 21;
    else if (victim->pcdata->rank == BELT_FIVE) chance += 18;
    else if (victim->pcdata->rank == BELT_FOUR) chance += 15;
    else if (victim->pcdata->rank == BELT_THREE) chance += 12;
    else if (victim->pcdata->rank == BELT_TWO) chance += 9;
    else if (victim->pcdata->rank == BELT_ONE) chance += 6;
  }
    if (!IS_NPC(victim) && IS_CLASS(victim,CLASS_CYBORG))
    if ((victim->pcdata->powers[CYBORG_LIMBS] > 1)) chance += 45;
  if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_CYBORG))
    if ((ch->pcdata->powers[CYBORG_LIMBS] > 1)) chance -= 40;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK) && ( victim->monkblock / 3 > 0 )) 
chance += (victim->monkblock / 3);
  if ( chance > 90) chance = 90;
  if ( number_percent( ) >= chance ) 
    return FALSE;
  if (IS_NPC(victim))
    act( "You dodge $n's attack.", ch, NULL, victim, TO_VICT, TRUE );
  if (IS_NPC(ch))
    act( "$N dodges your attack.", ch, NULL, victim, TO_CHAR, TRUE );
  return TRUE;
  }
    
  /*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
  CHAR_DATA *mount;
  if (victim->hit > 0)
  {
    if (victim->position <= POS_STUNNED)
    {
      bool gm_stance = FALSE;
      victim->position = POS_STANDING;
      if (!IS_NPC(victim) && victim->stance[0] > 0)
      {
        int stance = victim->stance[0];
        if (victim->stance[stance] >= 200) gm_stance = TRUE;
      }
      if (IS_NPC(victim) || victim->max_hit * 0.25 > victim->hit)
      {
        act( "$n clambers back to $s feet.", victim, NULL, NULL, TO_ROOM, FALSE);
        act( "You clamber back to your feet.", victim, NULL, NULL, TO_CHAR, FALSE);
      }
      else
      {
        act( "$n flips back up to $s feet.", victim, NULL, NULL, TO_ROOM, FALSE);
        act( "You flip back up to your feet.", victim, NULL, NULL, TO_CHAR, FALSE);
      }
    }
    return;
  }
  else if ((mount = victim->mount) != NULL)
  {
    if (victim->mounted == IS_MOUNT)
    {
      act("$n rolls off $N.",mount,NULL,victim,TO_ROOM, FALSE);
      act("You roll off $N.",mount,NULL,victim,TO_CHAR, FALSE);
    }
    else if (victim->mounted == IS_RIDING)
    {
      act("$n falls off $N.",victim,NULL,mount,TO_ROOM, FALSE);
      act("You fall off $N.",victim,NULL,mount,TO_CHAR, FALSE);
    }
    mount->mount    = NULL;
    victim->mount   = NULL;
    mount->mounted  = IS_ON_FOOT;
    victim->mounted = IS_ON_FOOT;
  }
  if (!IS_NPC(victim) && victim->hit <=-11 && IS_HERO(victim))
  {
    victim->hit = -10;
    if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
    return;
  }
  if (!IS_NPC(victim) && victim->hit <-9 && !IS_HERO(victim))
  {
    victim->hit = 1;
    do_killperson(victim,victim->name);
    victim->mdeath=victim->mdeath+1;
  }
  if(IS_NPC(victim) && victim->hit < -6 )
  {
    victim->position = POS_DEAD;
    return;
  }
  if (victim->hit <= -6 ) victim->position = POS_MORTAL;
  else if ( victim->hit <= -3 ) victim->position = POS_INCAP;
  else victim->position = POS_STUNNED;
  return;
}    

/*
 * Start fights. -----
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
  char buf[MSL];
  ROOM_INDEX_DATA *location;
  if (ch->fighting != NULL) return;
  location = get_room_index(ROOM_VNUM_ALTAR);
 
  if (IS_SET(ch->in_room->room_flags, ROOM_ARENA) && !IS_NPC(ch) && !IS_NPC(victim))
  {
     if ( IS_AFFECTED(ch, AFF_SLEEP) )
     affect_strip( ch, gsn_sleep );
     ch->fighting = victim;
     ch->position = POS_FIGHTING;
     ch->damcap[DAM_CHANGE] = 1;
     autodrop(ch);
     autodrop(victim);
     dropinvis(ch, victim);
  }
  if (!IS_SET(ch->in_room->room_flags, ROOM_ARENA) && !IS_NPC(ch) && !IS_NPC(victim)) 
  {
 //    char_from_room(ch);
 //    char_from_room(victim);
 //    char_to_room(ch, location);
 //    char_to_room(victim, location);
//     sprintf(buf, "You shall cry as I take your head, %s!",
//  	     ch->pcdata->switchname);
//     do_shout(ch,buf);
//  }  
//  if (IS_SET(ch->in_room->room_flags, ROOM_DUEL) && !IS_NPC(ch) && !IS_NPC(victim))
//  {
     if (!IS_NPC(ch) && !IS_NPC(victim) && !IS_SET(ch->in_room->room_flags, ROOM_ARENA)
        && victim->position != POS_FIGHTING)
     {
        sprintf(buf, "#yYou shall die by my hands, #R%s#y!.",
        ch->pcdata->switchname);
        do_shout(victim, buf);
     }
  }
         if ( IS_AFFECTED(ch, AFF_SLEEP) )
         affect_strip( ch, gsn_sleep );
         ch->fighting = victim;
         ch->position = POS_FIGHTING;
         ch->damcap[DAM_CHANGE] = 1;
         autodrop(ch);
         autodrop(victim);
         dropinvis(ch, victim);
	 return;
}


/*
 * Stop fights.
 */
void stop_embrace( CHAR_DATA *ch, CHAR_DATA *victim )
{
  if ( victim == NULL)
  {
    ch->embracing=NULL;
    return;
  }
  victim->embraced=NULL;
  ch->embracing=NULL;
  return;
}

void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
  CHAR_DATA *fch;
  for ( fch = char_list; fch != NULL; fch = fch->next )
  {
    if ( fch == ch || ( fBoth && fch->fighting == ch ) )
    {
      fch->fighting = NULL;
      fch->position = POS_STANDING;
      update_pos( fch );
    }
  }
  return;
}

/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *ch )
{
  char buf[MAX_STRING_LENGTH];
  OBJ_DATA *corpse;
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  char *name;
   
  if ( IS_NPC(ch) )
  {
    name = ch->name;
    corpse = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
    corpse->timer = number_range( 4, 8 );
    corpse->value[2]= ch->pIndexData->vnum;
    if ( ch->gold > 0 )
    {
      obj_to_obj( create_money( ch->gold ), corpse );
      ch->gold = 0;
    }
  }
  else
  {
    name = ch->name;

    corpse  = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
    corpse->timer = number_range( 25, 40 );
    if ( ch->gold > 0 )
    {
      obj = create_money( ch->gold );
      if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
        SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
      obj_to_obj( obj, corpse );
      ch->gold = 0;
    }
  }
  if (xIS_SET(ch->extra, EXTRA_ZOMBIE))
    SET_BIT(corpse->quest, QUEST_ZOMBIE);
  
  sprintf( buf, corpse->short_descr, name);

  free_string( corpse->short_descr );
  corpse->short_descr = str_dup( buf );
  sprintf( buf, corpse->description, name );
  free_string( corpse->description );
  corpse->description = str_dup( buf );
  if(!IS_NPC(ch))
  {
  if(ch->pcdata->killedby)
  corpse->killedby = ch->pcdata->killedby;
  else
  {
  corpse->killedby = "god";
  ch->pcdata->killedby = "god";
  }
  corpse->player   = ch->name;
  }
  
  for ( obj = ch->carrying; obj != NULL; obj = obj_next )
  {
    obj_next = obj->next_content;
    obj_from_char( obj );
    if (!IS_NPC(ch) && IS_SET( obj->extra_flags, ITEM_VANISH )) extract_obj( obj );
    else if (!IS_NPC(ch) && IS_SET(obj->quest, QUEST_ARTIFACT))
	extract_obj(obj);
    else
    {
      if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
        SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
      obj_to_obj( obj, corpse );
    }
  }
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE)) SET_BIT(corpse->extra_flags, ITEM_SHADOWPLANE);
  obj_to_room( corpse, ch->in_room );
  return;
}

void death_cry( CHAR_DATA *ch )
{
/*  Sometimes the mud crashes in this deathcode section.... I got no idea as to why - JOBO

  ROOM_INDEX_DATA *was_in_room;
  EXIT_DATA *pexit;
  char *msg;
  int door;

  if ( IS_NPC(ch) )
    msg = "You hear something's death cry.";
  else
    msg = "You hear someone's death cry.";

  was_in_room = ch->in_room;
  for ( door = 0; door <= 5; door++ )
  {
    pexit = was_in_room->exit[door];
    if (pexit != NULL &&   pexit->to_room != NULL && pexit->to_room != was_in_room )
    {
      ch->in_room = pexit->to_room;
      act( msg, ch, NULL, NULL, TO_ROOM , FALSE);
    }
  }
  ch->in_room = was_in_room;
*/
  return;
}

void make_part( CHAR_DATA *ch, char *argument )
{
  char  arg [MAX_INPUT_LENGTH];
  int vnum;

  argument = one_argument(argument,arg);
  vnum = 0;

  if (arg[0] == '\0') return;
  if (!str_cmp(arg,"head")) vnum = OBJ_VNUM_SEVERED_HEAD;
  else if (!str_cmp(arg,"arm")) vnum = OBJ_VNUM_SLICED_ARM;
  else if (!str_cmp(arg,"leg")) vnum = OBJ_VNUM_SLICED_LEG;
  else if (!str_cmp(arg,"heart")) vnum = OBJ_VNUM_TORN_HEART;
  else if (!str_cmp(arg,"turd")) vnum = OBJ_VNUM_TORN_HEART;
  else if (!str_cmp(arg,"entrails")) vnum = OBJ_VNUM_SPILLED_ENTRAILS;
  else if (!str_cmp(arg,"brain")) vnum = OBJ_VNUM_QUIVERING_BRAIN;
  else if (!str_cmp(arg,"eyeball")) vnum = OBJ_VNUM_SQUIDGY_EYEBALL;
  else if (!str_cmp(arg,"blood")) vnum = OBJ_VNUM_SPILT_BLOOD;
  else if (!str_cmp(arg,"face")) vnum = OBJ_VNUM_RIPPED_FACE;
  else if (!str_cmp(arg,"windpipe")) vnum = OBJ_VNUM_TORN_WINDPIPE;
  else if (!str_cmp(arg,"cracked_head")) vnum = OBJ_VNUM_CRACKED_HEAD;
  else if (!str_cmp(arg,"ear")) vnum = OBJ_VNUM_SLICED_EAR;
  else if (!str_cmp(arg,"nose")) vnum = OBJ_VNUM_SLICED_NOSE;
  else if (!str_cmp(arg,"tooth")) vnum = OBJ_VNUM_KNOCKED_TOOTH;
  else if (!str_cmp(arg,"tongue")) vnum = OBJ_VNUM_TORN_TONGUE;
  else if (!str_cmp(arg,"hand")) vnum = OBJ_VNUM_SEVERED_HAND;
  else if (!str_cmp(arg,"foot")) vnum = OBJ_VNUM_SEVERED_FOOT;
  else if (!str_cmp(arg,"thumb")) vnum = OBJ_VNUM_SEVERED_THUMB;
  else if (!str_cmp(arg,"index")) vnum = OBJ_VNUM_SEVERED_INDEX;
  else if (!str_cmp(arg,"middle")) vnum = OBJ_VNUM_SEVERED_MIDDLE;
  else if (!str_cmp(arg,"ring")) vnum = OBJ_VNUM_SEVERED_RING;
  else if (!str_cmp(arg,"little")) vnum = OBJ_VNUM_SEVERED_LITTLE;
  else if (!str_cmp(arg,"toe")) vnum = OBJ_VNUM_SEVERED_TOE;

  if ( vnum != 0 )
  {
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    char *name;

    name = IS_NPC(ch) ? ch->short_descr : ch->name;
    obj = create_object( get_obj_index( vnum ), 0 );
    if (IS_NPC(ch)) obj->timer = number_range(2,5);
    else obj->timer = -1;
    if (!str_cmp(arg,"head") && IS_NPC(ch))
      obj->value[1] = ch->pIndexData->vnum;
    else if (!str_cmp(arg,"head") && !IS_NPC(ch))
      {ch->pcdata->chobj = obj; obj->chobj = ch;obj->timer = number_range(1,2); obj->item_type = ITEM_HEAD;}
    else if (!str_cmp(arg,"arm"))
      SET_BIT(obj->extra_flags2, ITEM_ARM);
    else if (!str_cmp(arg,"brain") && !IS_NPC(ch) &&  IS_AFFECTED(ch,AFF_POLYMORPH) && IS_HEAD(ch, LOST_HEAD))
    {
      if (ch->pcdata->chobj != NULL) ch->pcdata->chobj->chobj = NULL;
      ch->pcdata->chobj = obj;
      obj->chobj = ch;
      obj->timer = number_range(1,2);
      obj->item_type = ITEM_HEAD;
    }
    if (vnum == OBJ_VNUM_SPILT_BLOOD) obj->timer = 2;
    if (!IS_NPC(ch))
    {
      sprintf( buf, obj->name, name );
      free_string( obj->name );
      obj->name = str_dup( buf );
    }
    else
    {
      sprintf( buf, obj->name, "mob" );
      free_string( obj->name );
      obj->name = str_dup( buf );
    }
    sprintf( buf, obj->short_descr, name );
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf( buf, obj->description, name );
    free_string( obj->description );
    obj->description = str_dup( buf );
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
      SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
    if( ch->in_room != NULL )
      obj_to_room( obj, ch->in_room );
  }
  return;
}

void raw_kill( CHAR_DATA *victim )
{
  CHAR_DATA *mount;
  stop_fighting( victim, TRUE );
/*  death_cry( victim ); */ /*Death_cry doesnt work*/
  make_corpse( victim);
 if ( victim->in_room == NULL ) //Rand
	return;
  if ( (mount = victim->mount) != NULL)
  {
    if (victim->mounted == IS_MOUNT)
    {
      act("$n rolls off the corpse of $N.",mount,NULL,victim,TO_ROOM, FALSE);
      act("You roll off the corpse of $N.",mount,NULL,victim,TO_CHAR, FALSE);
    }
    else if (victim->mounted == IS_RIDING)
    {
      act("$n falls off $N.",victim,NULL,mount,TO_ROOM, FALSE);
      act("You fall off $N.",victim,NULL,mount,TO_CHAR, FALSE);
    }
    mount->mount    = NULL;
    victim->mount   = NULL;
    mount->mounted  = IS_ON_FOOT;
    victim->mounted = IS_ON_FOOT;
  }
  if ( IS_NPC(victim) )
  {
    victim->pIndexData->killed++;
    kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
    extract_char( victim, TRUE );
    return;
  }
  extract_char( victim, FALSE );
  while ( victim->last_affect )
    affect_remove( victim, victim->last_affect );
  if (IS_AFFECTED(victim,AFF_POLYMORPH) && IS_AFFECTED(victim,AFF_ETHEREAL))
  {
    victim->affected_by = AFF_POLYMORPH + AFF_ETHEREAL;
  }
  else if (IS_AFFECTED(victim,AFF_POLYMORPH)) victim->affected_by = AFF_POLYMORPH;
  else if (IS_AFFECTED(victim,AFF_ETHEREAL)) victim->affected_by = AFF_ETHEREAL;
  else victim->affected_by = 0;
  REMOVE_BIT(victim->immune, IMM_STAKE);
  xREMOVE_BIT(victim->extra, TIED_UP);
  xREMOVE_BIT(victim->extra, GAGGED);
  xREMOVE_BIT(victim->extra, BLINDFOLDED);
  victim->pcdata->stats[DEMON_POWER] = 0;
  victim->itemaffect   = 0;
  victim->loc_hp[0]    = 0;
  victim->loc_hp[1]    = 0;
  victim->loc_hp[2]    = 0;
  victim->loc_hp[3]    = 0;
  victim->loc_hp[4]    = 0;
  victim->loc_hp[5]    = 0;
  victim->loc_hp[6]    = 0;
  victim->armor  = 100;
  victim->position   = POS_RESTING;
  victim->hit    = UMAX( 1, victim->hit  );
  victim->mana   = UMAX( 1, victim->mana );
  victim->move   = UMAX( 1, victim->move );
  victim->hitroll  = 0;
  victim->damroll  = 0;
  victim->saving_throw = 0;
  victim->carry_weight = 0;
  victim->carry_number = 0;
  do_call(victim,"all");
  save_char_obj( victim );
  return;
}

void behead( CHAR_DATA *victim )
{
  char buf [MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *location;

  if (IS_NPC(victim)) return;
  if ( victim->in_room == NULL ) //Rand
   return;
  location = victim->in_room;
  stop_fighting( victim, TRUE );
  make_corpse( victim );
  extract_char( victim, FALSE );
  char_from_room(victim);
  char_to_room(victim,location);
  if (victim == NULL)
  {bug( "Behead: Victim no longer exists.", 0 );return;}
  make_part( victim, "head" );
  while ( victim->last_affect )
    affect_remove( victim, victim->last_affect );
  if (IS_AFFECTED(victim,AFF_POLYMORPH) && IS_AFFECTED(victim,AFF_ETHEREAL))
    victim->affected_by = AFF_POLYMORPH + AFF_ETHEREAL;
  else if (IS_AFFECTED(victim,AFF_POLYMORPH))
    victim->affected_by = AFF_POLYMORPH;
  else if (IS_AFFECTED(victim,AFF_ETHEREAL))
    victim->affected_by = AFF_ETHEREAL;
  else
    victim->affected_by = 0;
  REMOVE_BIT(victim->immune, IMM_STAKE);
  xREMOVE_BIT(victim->extra, TIED_UP);
  xREMOVE_BIT(victim->extra, GAGGED);
  xREMOVE_BIT(victim->extra, BLINDFOLDED);
  xREMOVE_BIT(victim->extra, EXTRA_PREGNANT);
  xREMOVE_BIT(victim->extra, EXTRA_LABOUR);
  victim->pcdata->stats[DEMON_POWER] = 0;
  victim->itemaffect   = 0;
  victim->loc_hp[0]    = 0;
  victim->loc_hp[1]    = 0;
  victim->loc_hp[2]    = 0;
  victim->loc_hp[3]    = 0;
  victim->loc_hp[4]    = 0;
  victim->loc_hp[5]    = 0;
  victim->loc_hp[6]    = 0;
  victim->affected_by  = 0;
  victim->armor  = 100;
  victim->position   = POS_STANDING;
  victim->hit    = 1;
  victim->mana   = UMAX( 1, victim->mana );
  victim->move   = UMAX( 1, victim->move );
  victim->hitroll  = 0;
  victim->damroll  = 0;
  victim->saving_throw = 0;
  victim->carry_weight = 0;
  victim->carry_number = 0;
  victim->pcdata->sit_safe  = 0;
  victim->fight_timer  = 0;
  if (victim->cur_form != get_normal_form(victim))
  {
    set_form(victim, get_normal_form(victim));
    stc("Your body reverts to it's normal form.\n\r",victim);
  }
  if (IS_SET(victim->polyaff, POLY_ZULOFORM)) REMOVE_BIT(victim->polyaff, POLY_ZULOFORM);
  if (IS_SET(victim->affected_by, AFF_POLYMORPH)) REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
  if (IS_SET(victim->newbits, NEW_DFORM)) REMOVE_BIT(victim->newbits, NEW_DFORM);
  if (IS_SET(victim->newbits, NEW_CUBEFORM)) REMOVE_BIT(victim->newbits, NEW_CUBEFORM);
  if ( IS_SET(victim->newbits, NEW_POWER) )    REMOVE_BIT(victim->newbits, NEW_POWER);
  SET_BIT(victim->loc_hp[0],LOST_HEAD);
  SET_BIT(victim->affected_by,AFF_POLYMORPH);
  sprintf(buf,"the severed head of %s",victim->name);
  free_string(victim->morph);
  victim->morph = str_dup(buf);
  do_call(victim,"all");
  save_char_obj( victim );
  return;
}

void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
  char exp[MSL];
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *gch;
  CHAR_DATA *lch;
  CHAR_DATA *mount;
  int xp;
  int members;

  /*
   * Monsters don't get kill xp's or alignment changes.
   * P-killing doesn't help either.
   * Dying of mortal wounds or poison doesn't give xp to anyone!
   */
  if ((IS_NPC(ch) && (mount = ch->mount) == NULL) || victim == ch) return;
  members = 0;
  for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
  {
    if ( is_same_group( gch, ch ) && !IS_NPC(gch) && !IS_NPC(ch))
      members++;
  }
  if ( members == 0 )
  {
    bug( "Group_gain: members.", members );
    members = 1;
  }
  lch = (ch->leader != NULL) ? ch->leader : ch;
  for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
  {
    int xp_modifier = 100;
    if (!is_same_group( gch, ch )) continue;
    if (IS_NPC(gch)) continue;
    if (IS_NPC(ch)) continue;
    sprintf(exp,"#0=-=-= #7Exp Modifiers:#n ");

/*    if (members > 5)
      {
	xp_modifier += 100;
	strcat(exp,"#G+Big Group, #n");
      }	
    else
*/
      xp = xp_compute( gch, victim )*2.6;
      xp *= 5;
    if ( IS_AFFECTED(victim, AFF_SANCTUARY))
    {
	xp_modifier += 50;
	strcat(exp,"#C+Sanctuary#n, ");
    }
    if (victim->spec_fun != 0)
    {
	xp_modifier += 50;
	strcat(exp,"#C+Mob-Spec#n, ");
    }
    if (IS_EVIL(gch) && IS_GOOD(victim))
    {
        xp_modifier += 50;
	strcat(exp,"#C+Align#n, ");
    }
    if (IS_GOOD(gch) && IS_EVIL(victim))
    {
        xp_modifier += 50;
        strcat(exp,"#C+Align#n, ");
    }
    if (IS_EVIL(gch) && IS_EVIL(victim))
    {
        xp_modifier -= 50;
        strcat(exp,"#P-Align#n, ");
    }
    if (IS_GOOD(gch) && IS_GOOD(victim))
    {
        xp_modifier -= 50;
        strcat(exp,"#P-Align#n, ");
    }
    if (IS_SET(gch->special, SPC_NEWBIE))
    {
        xp_modifier += 200;
        strcat(exp,"#C+Newbie#n, ");
    }
    if (sysdata.exp_timer >= 0)
    {
        xp_modifier += 200;
        strcat(exp,"#C+Double Exp#n, ");
    }

    if (!IS_NPC(gch))
    {
    if (gch->class == 0)
        {
           xp_modifier += 300;
           strcat(exp, "#C+Classless#n, ");
        }
    if (xIS_SET(gch->extra, EXTRA_FIXRESET) && IS_SET(gch->special, SPC_NEWBIE))
        {
           xp_modifier += 200;
           strcat(exp, "#C+Reset#n, ");
        }

    if (IS_ITEMAFF(gch, ITEMA_DOUBLEEXP))
    {
	strcat(exp,"#C+Object Exp#n, ");
        xp_modifier += 200;
    }
    if (gch->lstatus > 0)
	{
	if (gch->lstatus == 1)
	{
        xp_modifier += 10;
        strcat(exp,"#C+Legend 1#n, ");
	}
	else if (gch->lstatus == 2)
        {
        strcat(exp,"#C+Legend 2#n, ");
        xp_modifier += 20;
        }
        else if (gch->lstatus == 3)
        {
        strcat(exp,"#C+Legend 3#n, ");
        xp_modifier += 30;
        }

        else if (gch->lstatus == 4)
        {
        strcat(exp,"#C+Legend 4#n, ");
        xp_modifier += 40;
        }

        else if (gch->lstatus == 5)
        {
        strcat(exp,"#C+Legend 5#n, ");
        xp_modifier += 50;
        }

        else if (gch->lstatus == 6)
        {
        strcat(exp,"#C+Legend 6#n, ");
        xp_modifier += 60;
        }

        else if (gch->lstatus == 7)
        {
        strcat(exp,"#C+Legend 7#n, ");
        xp_modifier += 70;
        }

        else if (gch->lstatus == 8)
        {
        strcat(exp,"#C+Legend 8#n, ");
        xp_modifier += 80;
        }

        else if (gch->lstatus == 9)
        {
        strcat(exp,"#C+Legend 9#n, ");
        xp_modifier += 90;
        }

	}
   }
	
	strcat(exp,"#n\n\r");
	stc(exp, gch);
    if (!xIS_SET(gch->extra, EXTRA_POLICY))
    {
	stc("#RYou have #yNOT#R accepted the #yPOLICY#R, type: '#yhelp policy#R'\n\r",gch);
        stc("#RYou have #yNOT#R accepted the #yPOLICY#R, you cannot #yGAIN EXP#R!\n\r",gch);
        stc("#RYou have #yNOT#R accepted the #yPOLICY#R, type: '#yhelp policy#R'\n\r",gch);
	xp = 0;
    }
    xp *= 1.5;
    xp = xp * (xp_modifier / 100);
    xp_modifier -= 100; //Account for minimum of 100 bonus.
    sprintf( buf, "#0=-=-= #7Total Exp Bonus: %d#n\n\r", xp_modifier);
    send_to_char( buf, gch );
    sprintf( buf, "#0=-=-= #CYou receive #7%d #Cexperience points.#n\n\r", xp );
    send_to_char( buf, gch );
    if ((mount = gch->mount) != NULL) send_to_char( buf, mount );
    gain_exp( gch, xp );
  }
  return;
}

int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim )
{
  double xp;
  int vnum;
  
  if (victim->level < 100)
    xp = 300 - URANGE(-5, 3 - (victim->level*5/3), 6) *50;
  else if (victim->level >= 100 && victim->level < 750)
    xp = 300 - URANGE(-10,3 - (victim->level*5/3), 6) *50;   
  else if (victim->level >= 750 && victim->level < 1000)
    xp = 300 - URANGE(-15,3 - (victim->level*5/3), 6) *50;
  else if (victim->level >= 1000 && victim->level < 2000)
    xp = 300 - URANGE(-20,3 - (victim->level*5/3), 6) *50;
  else if (victim->level >= 2000 && victim->level < 5000)
    xp = 300 - URANGE(-25,3 - (victim->level*5/3), 6) *50;
  else if (victim->level >= 5000 && victim->level < 10000)
    xp = 300 - URANGE(-30,3 - (victim->level*5/3), 6) *50;
  else if (victim->level >= 10000 && victim->level < 15000)
    xp = 300 - URANGE(-35,3 - (victim->level*5/3), 6) *50;
  else if (victim->level >= 15000 && victim->level < 20000)
    xp = 300 - URANGE(-40,3 - (victim->level*5/3), 6) *50;
  else if (victim->level >= 20000)
    xp = 300 - URANGE(-50,3 - (victim->level*5/3), 6) *50;

  /* 
   * Put in mob vnum that you don't want players to gain exp for
   * Rotain
   */
  if (IS_NPC(victim) && (vnum = victim->pIndexData->vnum) > 29000)
  {
    switch (vnum)
    {
      default:
      break;
      case 29600:
      case 30001:
      case 30006:
      case 30007:
      case 30008:
      case 30009:
      case 30000:
      case 90001:
      case 90101:
      case 90201:
      case 90301:
      case 90401:
      case 90501:
      case 90601:
      case 90701:
      case 90801:
      case 90901:
      case 90100:  /* Kingdom healers 90x00 */
      case 90200:
      case 90300:
      case 90400:
      case 90500:
      case 90600:
      case 90700:
      case 90800:
      case 90900:
      return 0;
    }
  }

  /*
   * Adjust for popularity of target:
   *   -1/8 for each target over  'par' (down to -100%)
   *   +1/8 for each target under 'par' (  up to + 25%)
   */
  xp -= xp * number_range( -2, 2 ) / 8;
  xp  = number_range( xp * 3 / 4, xp * 5 / 4 );
  xp  = UMAX( 0, xp );
  xp  = (xp * (victim->level) * 0.60);
  xp  = xp / 2; /* Put in cause players compaling to much exp :P */
  if (!IS_NPC(gch))
  {
    gch->pcdata->score[SCORE_TOTAL_LEVEL] += victim->level;
    if (victim->level > gch->pcdata->score[SCORE_HIGH_LEVEL])
    gch->pcdata->score[SCORE_HIGH_LEVEL] += victim->level;
    gch->pcdata->score[SCORE_TOTAL_XP] += xp;
    if (xp > gch->pcdata->score[SCORE_HIGH_XP])
    gch->pcdata->score[SCORE_HIGH_XP] += xp;
  }
  if (!IS_NPC(gch) && xp > 499 && gch->pcdata->disc_points != 999 && gch->pcdata->disc_research != -1)
  {
    if (victim->level <= 2000 && victim->level > 0 && gch->pcdata->disc_points != 999)
    {
    stc("#RYou gained a discipline point.#n\n\r", gch);
    gain_disc_points(gch, 1);
    }
    if (victim->level > 2000 && victim->level <= 4000 && gch->pcdata->disc_points != 999)
    {
    stc("#RYou gained #y2#R discipline point.#n\n\r", gch);
    gain_disc_points(gch, 2);
    }
    if (victim->level > 4000 && gch->pcdata->disc_points != 999)
    {
    stc("#RYou gained #y3#R discipline points.#n\n\r", gch);
    gain_disc_points(gch, 3);
    }
    xp -= 500;
  }
  if( xp < 0 ) xp = 0;
  if (xp > 0 && xp < 4000) xp = number_range(3000,5000);
  if (IS_NPC(victim) && (xIS_SET(victim->act, ACT_NOEXP))) return 0;
  return (int) xp;
}

void dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
  static char * const attack_table[] =
  {
  "hit", "slice",  "stab",  "slash", "whip", "claw",
  "blast",  "pound", "crush", "grep", "bite", "pierce", "suck"
  };
  static char * const attack_table2[] =
  {
  "hits", "slices",  "stabs",  "slashes", "whips", "claws",
  "blasts",  "pounds", "crushes", "greps", "bites", "pierces", "sucks"
  };
  char buf1[512], buf2[512], buf3[512];
  const char *vs;
  const char *vp;
  const char *attack;
  const char *attack2;
  int damp;
  int bodyloc;
  int recover;
  bool critical = FALSE;
  char punct;

  if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_REND)&& dam>0 && number_range(1,7) == 5) critical=TRUE;
  if ( dam ==   0 ) { vs = " miss";  vp = " misses";   }
  else if ( dam <=  50 ) { vs = " #Clightly#n"; vp = " #Clightly#n";  }
  else if ( dam <= 250 ) { vs = " #yhard#n";  vp = " #yhard#n";   }
  else if ( dam <= 500 ) { vs = " #Lvery hard#n"; vp = " #Lvery hard#n";  }
  else if ( dam <= 750 ) { vs = " #Gextremely hard!#n"; vp = " #Gextremely hard!#n";}
  else if ( dam <= 1250 ) { vs = " #7incredibly hard!#n";vp = "#7 incredibly hard!#n";} 
  else if ( dam <= 2000 ) { vs = " #rinsanely hard!#n";vp= " #rinsanely hard!#n";}
  else if ( dam <= 3000 ) 
{ vs = ", #0splintering #7bone #Rand cutting flesh!#n";vp= ", #0splintering #7bone #Rand cutting flesh!#n";}
  else if ( dam <= 4000 ) 
{ vs = ", #Rcausing gaping rents of flesh!#n";vp= ", #Rcausing gaping rents of flesh!#n";}
  else if ( dam <= 5000 ) 
{ vs = ", #7painting the walls with #Rblood!#n"; vp = ", #7painting the walls with #Rblood!#n";  }
  else if ( dam <= 6000 ) 
{ vs = ", #ycausing little birdies to appear!#n"; vp = ", #ywhich causes little birdies to appear!#n";}
  else if ( dam <= 10000)                  
{ vs = " #7and you cackle with insane glee!#n";vp = " #7and cackles with insane glee!#n";}
  else
{ vs = ", #0causing #0t#no#7t#na#0l#n #0d#ne#7v#na#0s#nt#7a#nt#0i#no#7n#n!#n",vp = ", #0causing #0t#no#7t#na#0l#n #0d#ne#7v#na#0s#nt#7a#nt#0i#no#7n#n!#n";}
  /* If victim's hp are less/equal to 'damp', attacker gets a death blow */
  if (IS_NPC(victim)) damp = 0;
  else damp = -10;
  if ((victim->hit - dam > damp ) || (dt >= 0 && dt < MAX_SKILL) ||
     (IS_NPC(victim) && xIS_SET(victim->act, ACT_NOPARTS)))
  {
    punct   = (dam <= 250) ? ' ' : ' ';
    if ( dt == TYPE_HIT && !IS_NPC(ch) && !IS_VAMPAFF(ch,VAM_CLAWS) )
    {
      if (dam == 0)
      {
        sprintf( buf1, "$n%s $N%c[%d]",  vp, punct, dam );
        sprintf( buf2, "You%s $N%c[%d]", vs, punct, dam );
        sprintf( buf3, "$n%s you%c", vp, punct );
      }
      else
      {
        sprintf( buf1, "$n hits $N%s%c[%d]",  vp, punct, dam );
        sprintf( buf2, "You hit $N%s%c[%d]", vs, punct, dam );
        sprintf( buf3, "$n hits you%s%c[%d]", vp, punct, dam );
        critical = TRUE;
      }
    }
    else if ( dt == TYPE_HIT && !IS_NPC(ch) && IS_SET(ch->newbits, NEW_MONKFLAME))
    {
      if (dam >= 1)
      {
        if (!IS_CLASS(ch, CLASS_WEREWOLF))
        {
          sprintf( buf1, "$n's flaming hands hit $N%s%c[%d]", vp, punct, dam );
          sprintf( buf2, "Your flaming hands hit $N%s%c[%d]", vs, punct, dam );
          sprintf( buf3, "$n's flaming hands hit you%s%c[%d]", vp, punct, dam );
        }
        else
        {
          sprintf( buf1, "$n's flaming claws hit $N%s%c[%d]", vp, punct, dam );
          sprintf( buf2, "Your flaming claws hit $N%s%c[%d]", vs, punct, dam );
          sprintf( buf3, "$n's flaming claws hit you%s%c[%d]", vp, punct, dam );
        }
      }
      else 
      {
        sprintf( buf1, "$n's hit%s $N%c[%d]",  vp, punct, dam );
        sprintf( buf2, "Your hit%s $N%c[%d]", vs, punct, dam );
        sprintf( buf3, "$n hit%s you%c[%d]", vp, punct, dam );
        critical = TRUE;
      }
    }
    else
    {
      if ( dt == TYPE_HIT && !IS_NPC(ch) && (IS_VAMPAFF(ch,VAM_CLAWS)))
      {
        attack  = attack_table[dt - TYPE_HIT + 5];
        attack2 = attack_table2[dt - TYPE_HIT + 5];
      }
      else if ( dt >= 0 && dt < MAX_SKILL )
      {
        attack = skill_table[dt].noun_damage;
        attack2 = skill_table[dt].noun_damage;
      }
      else if (dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]))
      {
        attack  = attack_table[dt - TYPE_HIT];
        attack2 = attack_table2[dt - TYPE_HIT];
      }
      else
      {
        dt  = TYPE_HIT;
        attack  = attack_table[0];
        attack2 = attack_table2[0];
      }
      if (dam == 0)
      {
        sprintf( buf1, "$n's %s%s $N%c[%d]",  attack, vp, punct, dam );
        sprintf( buf2, "Your %s%s $N%c[%d]",  attack, vs, punct, dam );
        sprintf( buf3, "$n's %s%s you%c[%d]", attack, vp, punct, dam );
      }
      else if (IS_SET(ch->newbits, NEW_MONKFLAME) && (dam == 0))
      {
        sprintf( buf1, "$n's flaming %s%s $N%c[%d]", attack2, vp, punct, dam );
        sprintf( buf2, "Your flaming %s%s $N%c[%d]",  attack, vs, punct, dam );
        sprintf( buf3, "$n's flaming %s%s you%c[%d]", attack2, vp, punct, dam );
      }
      else
      {
        if ( dt >= 0 && dt < MAX_SKILL )
        {
          sprintf( buf1, "$n's %s strikes $N%s%c[%d]",  attack2, vp, punct, dam );
          sprintf( buf2, "Your %s strikes $N%s%c[%d]",  attack, vs, punct, dam );
          sprintf( buf3, "$n's %s strikes you%s%c[%d]", attack2, vp, punct, dam );
        }
        else if (IS_SET(ch->newbits, NEW_MONKFLAME))
        {
          sprintf( buf1, "$n's flaming %s hits $N%s%c[%d]", attack2, vp, punct, dam);
          sprintf( buf2, "Your flaming %s hits $N%s%c[%d]", attack, vs, punct, dam);
          sprintf( buf3, "$n's flaming %s hits you%s%c[%d]", attack2, vp, punct, dam);
        }
        else
        {
          sprintf( buf1, "$n %s $N%s%c[%d]",  attack2, vp, punct, dam );
          sprintf( buf2, "You %s $N%s%c[%d]",  attack, vs, punct, dam );
          sprintf( buf3, "$n %s you%s%c[%d]", attack2, vp, punct, dam );
          critical = TRUE;
        }
      }
      recover = 0;
      if (!IS_NPC(victim) && IS_SET(victim->newbits,NEW_SKIN) && (attack=="slash" 
          || attack == "slice" || attack == "pierce" || attack == "stab"))
        recover=number_range(1,dam);           
      if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_SLASH) && (attack == "slash" 
          || attack == "slice") && dam > 0 )
        recover = number_range(1,dam);
      if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_STAB) && (attack == "stab" 
          || attack == "pierce") && dam > 0 )
        recover = number_range(1,dam);
      if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_SMASH) && (attack == "blast" 
          || attack == "pound" || attack == "crush") && dam > 0 )    
        recover = number_range(1,dam);
      if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_ANIMAL) && (attack == "bite" 
          || attack == "claw") && dam > 0 )    
        recover = number_range(1,dam);
      if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_MISC) && (attack == "grep" 
          || attack == "suck" || attack == "whip") && dam > 0 )
        recover = number_range(1,dam);
    }

    act( buf1, ch, NULL, victim, TO_NOTVICT, TRUE );
    if (!xIS_SET(ch->act, PLR_BRIEF3))
    {
      if (!(xIS_SET(ch->act, PLR_BRIEF2)
         && (dam == 0 || dt == skill_lookup("lightning bolt") ||
         dt == skill_lookup("acid blast") || dt == skill_lookup("chill touch") ||
         dt == skill_lookup("fireball"))))
      {
        if (!(dt == skill_lookup("chaos blast") && !IS_NPC(victim)))
          act(buf2, ch, NULL, victim, TO_CHAR, FALSE);
      }
    }

    /*
     * Message to victim
     */
    if (!xIS_SET(victim->act, PLR_BRIEF3))
    {
      if (!(xIS_SET(victim->act, PLR_BRIEF2)
         && (dam == 0 || dt == skill_lookup("lightning bolt") ||
        dt == skill_lookup("acid blast") || dt == skill_lookup("chill touch") ||
        dt == skill_lookup("fireball"))))
        act(buf3, ch, NULL, victim, TO_VICT, FALSE);
    }

    if (critical) critical_hit(ch,victim,dt,dam);
    return;
  }
  if ( dt == TYPE_HIT && !IS_NPC( ch ) && !IS_VAMPAFF(ch,VAM_CLAWS) &&!IS_VAMPAFF(ch,VAM_FANGS))
  {
    damp=number_range(1,5);
    if ( damp == 1 )
    {
      act("You ram your fingers into $N's eye sockets and rip $S face off.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n rams $s fingers into $N's eye sockets and rips $S face off.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n rams $s fingers into your eye sockets and rips your face off.", ch, NULL, victim, TO_VICT, FALSE);
      make_part(victim,"face");
    }
    else if ( damp == 2 )
    {
      act("You grab $N by the throat and tear $S windpipe out.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n grabs $N by the throat and tears $S windpipe out.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n grabs you by the throat and tears your windpipe out.", ch, NULL, victim, TO_VICT, FALSE);
      make_part(victim,"windpipe");
    }
    else if ( damp == 3 )
    {
      act("You punch your fist through $N's stomach and rip out $S entrails.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n punches $s fist through $N's stomach and rips out $S entrails.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n punches $s fist through your stomach and rips out your entrails.", ch, NULL, victim, TO_VICT, FALSE);
      make_part(victim,"entrails");
    }
    else if ( damp == 4 )
    {
      if (!IS_BODY(victim,BROKEN_SPINE))
      SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
      act("You hoist $N above your head and slam $M down upon your knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n hoists $N above $s head and slams $M down upon $s knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n hoists you above $s head and slams you down upon $s knee.\n\rThere is a loud cracking sound as your spine snaps.", ch, NULL, victim, TO_VICT, FALSE);
    }
    else if ( damp == 5 )
    {
      act("You lock your arm around $N's head, and give it a vicious twist.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n locks $s arm around $N's head, and gives it a vicious twist.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n locks $s arm around your head, and gives it a vicious twist.", ch, NULL, victim, TO_VICT, FALSE);
      if (!IS_BODY(victim,BROKEN_NECK))
      {
        act("There is a loud snapping noise as your neck breaks.", victim, NULL, NULL, TO_CHAR, FALSE);
        act("There is a loud snapping noise as $n's neck breaks.", victim, NULL, NULL, TO_ROOM, FALSE);
        SET_BIT(victim->loc_hp[1],BROKEN_NECK);
      }
    }
    return;
  }
  if (dt >= 0 && dt < MAX_SKILL) attack = skill_table[dt].noun_damage;
  else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]))
    attack = attack_table[dt - TYPE_HIT];
  else
  {
    dt  = TYPE_HIT;
    attack  = attack_table[0];
  }
  if ( attack == "slash" || attack == "slice" )
  {
    damp=number_range(1,60);
    if ( damp == 1 )
    {
      act("You swing your blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n swings $s blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n swings $s blade in a low arc, rupturing your abdominal cavity.\n\rYour entrails spray out over a wide area.", ch, NULL, victim, TO_VICT, FALSE);
      make_part(victim,"entrails");
    }
    else if ( damp == 2 )
    {
      act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT, FALSE);
    }
    else if ( damp == 3 )
    {
      if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
      if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
      act("Your blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n's blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n's blow slices open your carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_VICT, FALSE);
      make_part(victim,"blood");
    }
    else if ( damp == 4 )
    {
      if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
      if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
      act("You swing your blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n swings $s blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n swings $s blade across your throat, showering the area with blood.", ch, NULL, victim, TO_VICT, FALSE);
      make_part(victim,"blood");
    }
    else if ( damp == 5 )
    {
      if (!IS_HEAD(victim,BROKEN_SKULL))
      {
        act("You swing your blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n swings $s blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n swings $s blade down upon your head, splitting it open.\n\rYour brains pour out of your forehead.", ch, NULL, victim, TO_VICT, FALSE);
        make_part(victim,"brain");
        SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
      }
      else
      {
        act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT, FALSE);
      }
    }
    else if ( damp == 6 )
    {
      act("You swing your blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n swings $s blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n swings $s blade between your legs, nearly splitting you in half.", ch, NULL, victim, TO_VICT, FALSE);
    }
    else if ( damp == 7 )
    {
      if (!IS_ARM_L(victim,LOST_ARM))
      {
        act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT, FALSE);
        make_part(victim,"arm");
        SET_BIT(victim->loc_hp[2],LOST_ARM);
        if (!IS_BLEEDING(victim,BLEEDING_ARM_L)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
        if (IS_BLEEDING(victim,BLEEDING_HAND_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
      }
      else if (!IS_ARM_R(victim,LOST_ARM))
      {
        act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT, FALSE);
        make_part(victim,"arm");
        SET_BIT(victim->loc_hp[3],LOST_ARM);
        if (!IS_BLEEDING(victim,BLEEDING_ARM_R)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
        if (IS_BLEEDING(victim,BLEEDING_HAND_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
      }
      else
      {
        act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT, FALSE);
      }
    }
    else if ( damp == 8 )
    {
      if (!IS_LEG_L(victim,LOST_LEG))
      {
        act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT, FALSE);
        make_part(victim,"leg");
        SET_BIT(victim->loc_hp[4],LOST_LEG);
        if (!IS_BLEEDING(victim,BLEEDING_LEG_L)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
        if (IS_BLEEDING(victim,BLEEDING_FOOT_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
      }
      else if (!IS_LEG_R(victim,LOST_LEG))
      {
        act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT, FALSE);
        make_part(victim,"leg");
        SET_BIT(victim->loc_hp[5],LOST_LEG);
        if (!IS_BLEEDING(victim,BLEEDING_LEG_R)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
        if (IS_BLEEDING(victim,BLEEDING_FOOT_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
      }
      else
      {
        act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT, FALSE);
      }
    }
  }
  else if ( attack == "stab" || attack == "pierce" )
  {
    damp=number_range(1,5);
    if ( damp == 1 )
    {
      act("You defty invert your weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n defty inverts $s weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n defty inverts $s weapon and plunge it point first into your chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_VICT, FALSE);
      make_part(victim,"blood");
    }
    else if ( damp == 2 )
    {
      act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT, FALSE);
    }
    else if ( damp == 3 )
    {
      act("You thrust your weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n thrusts $s weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n thrusts $s weapon up under your jaw and through your head.", ch, NULL, victim, TO_VICT, FALSE);
    }
    else if ( damp == 4 )
    {
      act("You ram your weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n rams $s weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n rams $s weapon through your body, pinning you to the ground.", ch, NULL, victim, TO_VICT, FALSE);
    }
    else if ( damp == 5 )
    {
      act("You stab your weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n stabs $s weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n stabs $s weapon into your eye and out the back of your head.", ch, NULL, victim, TO_VICT, FALSE);
      if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50) SET_BIT(victim->loc_hp[0],LOST_EYE_L);
      else if (!IS_HEAD(victim,LOST_EYE_R)) SET_BIT(victim->loc_hp[0],LOST_EYE_R);
      else if (!IS_HEAD(victim,LOST_EYE_L)) SET_BIT(victim->loc_hp[0],LOST_EYE_L);
    }
  }
  else if ( attack == "blast" || attack == "pound" || attack == "crush" )
  {
    damp=number_range(1,3);
    bodyloc = 0;
    if ( damp == 1)
    {
      act("Your blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_CHAR, FALSE);
      act("$n's blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_NOTVICT, FALSE);
      act("$n's blow smashes through your chest, caving in half your ribcage.", ch, NULL, victim, TO_VICT, FALSE);
      if (IS_BODY(victim,BROKEN_RIBS_1 )) {bodyloc += 1; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
      if (IS_BODY(victim,BROKEN_RIBS_2 )) {bodyloc += 2; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
      if (IS_BODY(victim,BROKEN_RIBS_4 )) {bodyloc += 4; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
      if (IS_BODY(victim,BROKEN_RIBS_8 )) {bodyloc += 8; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
      if (IS_BODY(victim,BROKEN_RIBS_16)) {bodyloc += 16; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
      bodyloc += number_range(1,3);
      if (bodyloc > 24) bodyloc = 24;
      if (bodyloc >= 16) {bodyloc -= 16; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
      if (bodyloc >= 8) {bodyloc -= 8; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
      if (bodyloc >= 4) {bodyloc -= 4; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
      if (bodyloc >= 2) {bodyloc -= 2; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
      if (bodyloc >= 1) {bodyloc -= 1; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
    }
    else if ( damp == 2)
    {
      act("Your blow smashes $N's spine, shattering it in several places.", ch, NULL, 
victim, TO_CHAR, FALSE);
      act("$n's blow smashes $N's spine, shattering it in several places.", ch, NULL, 
victim, TO_NOTVICT, FALSE);
      act("$n's blow smashes your spine, shattering it in several places.", ch, NULL, 
victim, TO_VICT, FALSE);
      if (!IS_BODY(victim,BROKEN_SPINE)) SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
    }
    else if ( damp == 3)
    {
      if (!IS_HEAD(victim,BROKEN_SKULL))
      {
        act("You swing your weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, leaking out brains.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n swings $s weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, covering you with brains.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n swings $s weapon down upon your head.\n\rYour head cracks open like an overripe melon, spilling your brains everywhere.", ch, NULL, victim, TO_VICT, FALSE);
        make_part(victim,"brain");
        SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
      }
      else
      {
        act("You hammer your weapon into $N's side, crushing bone.", ch, NULL, victim, 
TO_CHAR, FALSE);
        act("$n hammers $s weapon into $N's side, crushing bone.", ch, NULL, victim, 
TO_NOTVICT, FALSE);
        act("$n hammers $s weapon into your side, crushing bone.", ch, NULL, victim, 
TO_VICT, FALSE);
      }
    }
  }
  else if ( !IS_NPC( ch ) && (attack == "bite" ||IS_VAMPAFF(ch,VAM_FANGS)))
  {
    act("You sink your teeth into $N's throat and tear out $S jugular vein.\n\rYou wipe the blood from your chin with one hand.", ch, NULL, victim, TO_CHAR, FALSE);
    act("$n sink $s teeth into $N's throat and tears out $S jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_NOTVICT, FALSE);
    act("$n sink $s teeth into your throat and tears out your jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_VICT, FALSE);
    make_part(victim,"blood");
    if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
    if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
  }
  else if ( !IS_NPC(ch) && (attack == "claw" || IS_VAMPAFF(ch,VAM_CLAWS)))
  {
    damp=number_range(1,2);
    if ( damp == 1 )
    {
      act("You tear out $N's throat, showering the area with blood.", ch, NULL, victim, 
TO_CHAR, FALSE);
      act("$n tears out $N's throat, showering the area with blood.", ch, NULL, victim, 
TO_NOTVICT, FALSE);
      act("$n tears out your throat, showering the area with blood.", ch, NULL, victim, 
TO_VICT, FALSE);
      make_part(victim,"blood");
      if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
      if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
    }
    if ( damp == 2 )
    {
      if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50)
      {
        act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT, FALSE);
        make_part(victim,"eyeball");
        SET_BIT(victim->loc_hp[0],LOST_EYE_L);
      }
      else if (!IS_HEAD(victim,LOST_EYE_R))
      {
        act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT, FALSE);
        make_part(victim,"eyeball");
        SET_BIT(victim->loc_hp[0],LOST_EYE_R);
      }
      else if (!IS_HEAD(victim,LOST_EYE_L))
      {
        act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT, FALSE);
        make_part(victim,"eyeball");
        SET_BIT(victim->loc_hp[0],LOST_EYE_L);
      }
      else
      {
        act("You claw open $N's chest.", ch, NULL, victim, TO_CHAR, FALSE);
        act("$n claws open $N's chest.", ch, NULL, victim, TO_NOTVICT, FALSE);
        act("$n claws open $N's chest.", ch, NULL, victim, TO_VICT, FALSE);
      }
    }
  }
  else if ( attack == "whip" )
  {
    act("You entangle $N around the neck, and squeeze out $S life.", ch, NULL, victim, 
TO_CHAR, FALSE);
    act("$n entangles $N around the neck, and squeezes out $S life.", ch, NULL, victim, 
TO_NOTVICT, FALSE);
    act("$n entangles you around the neck, and squeezes the life out of you.", ch, NULL, 
victim, TO_VICT, FALSE);
    if (!IS_BODY(victim,BROKEN_NECK)) SET_BIT(victim->loc_hp[1],BROKEN_NECK);
  }
  else if ( attack == "suck" || attack == "grep" )
  {
    act("You place your weapon on $N's head and suck out $S brains.", ch, NULL, victim, 
TO_CHAR, FALSE);
    act("$n places $s weapon on $N's head and suck out $S brains.", ch, NULL, victim, 
TO_NOTVICT, FALSE);
    act("$n places $s weapon on your head and suck out your brains.", ch, NULL, victim, 
TO_VICT, FALSE);
  }
  else
  {
  /*bug( "Dam_message: bad dt %d.", dt );*/
  }
  return;
}

/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
  OBJ_DATA *obj;
  char buf [MAX_STRING_LENGTH];

  if (IS_NPC(ch) && victim->level > 2 && number_percent() > 10)
    return;
  if (!IS_IMMUNE(ch, IMM_DISARM))
    return;
  if (IS_IMMUNE(victim, IMM_DISARM))
    return;
  if(((obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
  {
    if(((obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
      return;
  }
  sprintf(buf,"$n disarms you!");
  ADD_COLOUR(ch,buf,WHITE);
  act( buf, ch, NULL, victim, TO_VICT , FALSE   );
  sprintf(buf,"You disarm $N!");
  ADD_COLOUR(ch,buf,WHITE);
  act( buf,  ch, NULL, victim, TO_CHAR , FALSE   );
/*  sprintf(buf,"$n disarms $N!");
  ADD_COLOUR(ch,buf,WHITE);
  act( buf,  ch, NULL, victim, TO_NOTVICT, FALSE );*/
  obj_from_char( obj );
  if (IS_SET(obj->extra_flags, ITEM_LOYAL) && ( !IS_NPC(victim) ) )
  {
    act( "$p leaps back into your hand!", victim, obj, NULL, TO_CHAR, FALSE );
    act( "$p leaps back into $n's hand!",  victim, obj, NULL, TO_ROOM, FALSE );
    obj_to_char( obj, victim );
    do_wear( victim, obj->name);
  }
  else if (IS_NPC(victim))
    obj_to_char(obj, victim);
  else
    obj_to_char(obj, victim);
  return;
}

/*
 * Trip a creature.
 * Caller must check for successful attack.
 */
void trip( CHAR_DATA *ch, CHAR_DATA *victim )
{
  char buf [MAX_STRING_LENGTH];

  if (IS_AFFECTED(victim,AFF_FLYING)) return;
  if (IS_NPC(ch) && victim->level > 2 && number_percent() > 5) return;
  if (!IS_NPC(victim))  /* hmmm, gotta check all class fly powers - JOBO */
  {
    if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_FLYING))
      return;
    if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMAFF(victim, DEM_UNFOLDED))
      return;
    if (IS_CLASS(victim, CLASS_ANGEL) && IS_SET(victim->pcdata->powers[ANGEL_POWERS], ANGEL_WINGS))
      return;
    if (IS_SET(victim->special, SPC_CHAMPION) && IS_DEMAFF(victim,DEM_UNFOLDED))
      return;
    if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)) 
      return;
  }
  if ( victim->wait == 0)
  {
    sprintf(buf,"$n trips you and you go down!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf, ch, NULL, victim, TO_VICT, FALSE    );
    sprintf(buf,"You trip $N and $E goes down!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_CHAR, FALSE    );
/*    sprintf(buf,"$n trips $N and $E goes down!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_NOTVICT, FALSE );*/
    WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
    victim->position = POS_RESTING;
  }
  return;
}

void do_kill( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  
  WAIT_STATE(ch,1);
  one_argument( argument, arg );
  if ( IS_NPC(ch) && ch->desc == NULL) return;
  if ( arg[0] == '\0' )
  {
    send_to_char( "Kill whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "You cannot kill yourself!\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;
  if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
  {
    act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR, FALSE );
    return;
  }
    if ( ch->position == POS_FIGHTING )
  {
    send_to_char( "You do the best you can!\n\r", ch );
    return;
  }
  WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
  check_killer( ch, victim );
  if(!IS_NPC(ch) && !IS_NPC(victim))
  {
      if (!IS_SET(ch->in_room->room_flags, ROOM_ARENA) || !IS_SET(victim->in_room->room_flags, ROOM_ARENA))
      {
        ch->fight_timer +=3;
        victim->fight_timer +=3;
      }
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && number_range(1,2) == 1
  && ch->power[DISC_WERE_BOAR] > 1 && victim->position == POS_STANDING)
  {
    multi_hit( ch, victim, TYPE_UNDEFINED );
    act("You charge into $N, knocking $M from $S feet.",ch,NULL,victim,TO_CHAR, FALSE);
    act("$n charge into $N, knocking $M from $S feet.",ch,NULL,victim,TO_NOTVICT, FALSE);
    act("$n charge into you, knocking you from your feet.",ch,NULL,victim,TO_VICT, FALSE);
    victim->position = POS_STUNNED;
    if (number_range(1,3) == 1)
    return;
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_CYBORG) && ch->pcdata->powers[CYBORG_BODY] > 2)
  {
    if (number_range(1,2) == 1)
    {
    multi_hit( ch, victim, TYPE_UNDEFINED );
    act("You slam into $N, knocking $M from $S feet.",ch,NULL,victim,TO_CHAR, FALSE);
    act("$n slam into $N, knocking $M from $S feet.",ch,NULL,victim,TO_NOTVICT, FALSE);
    act("$n slam into you, knocking you from your feet.",ch,NULL,victim,TO_VICT, FALSE);
    victim->position = POS_STUNNED;
    if (number_range(1,4) == 1)
    return;
    }
  }
  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
  {
    act("You swoop down from the sky and charge into $N.",ch,NULL,victim,TO_CHAR, FALSE);
    act("$n swoops down from the sky and charges into $N.",ch,NULL,victim,TO_NOTVICT, 
FALSE);
    act("$n swoops dwon from the sky and charges into you.",ch,NULL,victim,TO_VICT, FALSE);
    if (ch->pcdata->powers[ANGEL_JUSTICE] >= 5) multi_hit( ch, victim, TYPE_UNDEFINED);
    else if (number_range(1,4) == 1) multi_hit( ch, victim, TYPE_UNDEFINED);
  }
  multi_hit( ch, victim, TYPE_UNDEFINED );
  return;
}

void do_backstab( CHAR_DATA *ch, char *argument )
{
  char buf[MSL];
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;

  one_argument( argument, arg );
  if ( arg[0] == '\0' )
  {
    send_to_char( "Backstab whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "How can you sneak up on yourself?\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;
  if ((( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11)
      && ((obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || obj->value[3] != 11))
  {
    send_to_char( "You need to wield a piercing weapon.\n\r", ch );
    return;
  }
  if ( victim->fighting != NULL )
  {
    send_to_char( "You can't backstab a fighting person.\n\r", ch );
    return;
  }
  if ( victim->hit < victim->max_hit )
  {
    act( "$N is hurt and suspicious ... you can't sneak up.",
    ch, NULL, victim, TO_CHAR , FALSE);
    return;
  }
  if (!IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_backstab])
  {
    if (!IS_NPC(victim))
    sprintf(buf,"You try to sneak up behind %s but fail!\n\r",victim->pcdata->switchname);
    else
    sprintf(buf,"You try to sneak up behind %s but fail!\n\r",victim->short_descr);
    stc(buf,ch);
    if (number_range(1,10) == 1)
    {
      sprintf(buf, "You learn from your mistakes, and your %s skill improves!\n\r",
        skill_table[gsn_backstab].name);
      stc(buf,ch);
      ch->pcdata->learned[gsn_backstab] += 1;
    }
    return;
  }

  check_killer( ch, victim );
  WAIT_STATE( ch, skill_table[gsn_backstab].beats );
  if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_BACKSTAB) && ch->power[DISC_VAMP_QUIE] < 10 &&
    !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_DROW))
    damage( ch, victim, 0, gsn_backstab );
  else if (!IS_AWAKE(victim) || IS_NPC(ch) ||( !IS_NPC( ch ) &&  number_percent( ) < ch->pcdata->learned[gsn_backstab] ))
    multi_hit( ch, victim, gsn_backstab );
  if( !IS_NPC( ch ) && (ch->pcdata->rank == BELT_SEVEN || ch->pcdata->rank == BELT_EIGHT || ch->pcdata->rank == BELT_NINE 
    || ch->pcdata->rank == BELT_TEN) && number_range(1,3) == 1)
    multi_hit( ch, victim, gsn_backstab );
  if( !IS_NPC( ch ) && (ch->pcdata->rank == BELT_NINE || ch->pcdata->rank == BELT_TEN) && number_range(1,6) == 1)
    multi_hit( ch, victim, gsn_backstab ); 
  else
    damage( ch, victim, 0, gsn_backstab );
  return;
}

void do_flee( CHAR_DATA *ch, char *argument )
{
  ROOM_INDEX_DATA *was_in;
  ROOM_INDEX_DATA *now_in;
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];    
  int attempt;

//  WAIT_STATE(ch,4); /* to prevent spam_fleeing */
  if ((victim = ch->fighting ) == NULL)
  {
    if (ch->position == POS_FIGHTING) ch->position = POS_STANDING;
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    WAIT_STATE(ch,6);
    return;
  }
  if(ch->move <= 0)
  {
    send_to_char("You can't find the energy to leave.\n\r",ch);
    WAIT_STATE(ch,6);
    return;
  }
  if (IS_AFFECTED(ch, AFF_WEBBED))
  {
    send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
    WAIT_STATE(ch,6);
    return;
  }
  if ( IS_SET(ch->flag2, AFF_TOTALBLIND) )
  {
    stc("You are unable to see anything, let alone flee!\n\r",victim);
    WAIT_STATE(ch,6);
    return;
    }
  if ( IS_NPC(victim) && victim->spec_fun != 0 && number_percent( ) > 20)
  {
    stc("You make a move to flee, but you fail!\n\r",ch);
    act("$n tries to flee, but cannot escape from $N!",ch, NULL,victim, TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_INQUISITOR) && 
 IS_SET(victim->special, SPC_CHAINS) && !IS_SET(victim->special, SPC_FURY) && 
number_percent( ) > 20)
  {
    sprintf(buf,"You are trapped, entangled within %s's unholy chains!\n\r",victim->name);
    stc(buf,ch);
    act("$n tries to flee, but $N's unholy chains bind $m!",ch, NULL, victim, TO_ROOM, 
FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], 
DPOWER_DARKTENDRILS) && number_percent( ) > 20)
  {
    sprintf(buf,"You are entangled within %s's darktendrils!",victim->name);
    stc(buf,ch);
    act("$n tries to run, but $N's darktendrils prevent any escape!",ch, NULL, victim, 
TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if ( !IS_NPC( victim ) && IS_CLASS(victim, CLASS_ANGEL) && 
IS_SET(victim->pcdata->powers[ANGEL_POWERS], ANGEL_HALO) && number_percent( ) > 15)
  {
    sprintf(buf,"You are blinded by %s's halo, you cannot escape!\n\r",victim->name);
    stc(buf,ch);
    act("$n tries to run, but $N's blinding halo prevents $n from escaping!",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if (IS_CLASS(victim, CLASS_VAMPIRE)
 && !IS_SET(ch->polyaff,POLY_WOLF)
 && IS_SET(victim->newbits,NEW_COIL) && number_percent( ) > 20)
  {
    sprintf(buf,"Not with %s coiled around you!\n\r",victim->name);
    send_to_char(buf,ch);
    act("$n cant escape with $N coiled around them.",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if ( !IS_NPC( victim ) && IS_CLASS(victim, CLASS_TANARRI) && 
IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_TENDRILS) && number_percent() > 20)
  {
    sprintf(buf,"%s spews a shower of tendrils at you, preventing your escape!\n\r",victim->name);
    send_to_char(buf,ch);
    act("$n tries to run, but $N spews forth a shower of tendrils, preventing $n from running.",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_PSIONIST) && 
IS_SET(victim->pcdata->powers[PSIONIST_POWER], PSIONIST_PRISON) && number_percent() > 20)
  {
    sprintf(buf,"%s sends forth a mental spike, imprisoning you!\n\r",victim->name);
    stc(buf,ch);
    act("$n tries to flee, but $N imprisons $n in a mental jail.",ch,NULL,victim,TO_ROOM, 
FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if (IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim,ITEMA_ILLUSIONS) && number_percent( 
) > 20)
  {
    sprintf(buf,"Theres to many of %s to escape!\n\r",victim->name);
    send_to_char(buf,ch);
    act("$n cant escape with $N blocking his way.",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if (IS_CLASS(victim, CLASS_HIGHLANDER) && IS_SET(victim->newbits,NEW_JAWLOCK) && 
number_percent( ) > 20)
  {
    sprintf(buf,"You cannot escape the fury of %s's blades!\n\r",victim->name);
    send_to_char(buf,ch);
    act("$n turns to run, but cannot escape $N's fury of attacks!",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if (IS_CLASS(victim, CLASS_PALADIN) && IS_SET(victim->newbits, NEW_JAWLOCK) && 
number_percent( ) > 20)
  {
    sprintf(buf,"You failed to run away from %s!\n\r",victim->name);
    send_to_char(buf,ch);
    act("$n turns to run, but cannot escape as $N presses the attack.",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }

  if (IS_CLASS(victim, CLASS_MONK) && IS_SET(victim->newbits,NEW_JAWLOCK) && 
number_percent( ) > 20)
  {
    sprintf(buf,"Not with god holding you!\n\r");
    send_to_char(buf,ch);
    act("$n cant escape god's mighty hold!",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if (IS_CLASS(victim, CLASS_CYBORG) && IS_SET(victim->newbits,NEW_CUBEFORM) && 
number_percent( ) > 20)
  {
    sprintf(buf,"They are everywhere, you cannot escape!\n\r");
    send_to_char(buf,ch);
    act("$n tries to flee, but fails!",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_SET(victim->newbits,NEW_JAWLOCK) && 
number_percent( ) > 20)
  {
    sprintf(buf,"Not with %s's jaws clamped on your neck!\n\r",victim->name);
    send_to_char(buf,ch);
    act("$n cant escape $N's clamped jaws!",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  if (  !IS_NPC( ch ) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT) && 
IS_SET(victim->pcdata->powers[AURAS], BOG_AURA) && number_percent() > 20)
  {
    sprintf(buf,"Your stuck in the swamp surrounding %s!\n\r",victim->name);
    send_to_char(buf,ch);
    act("$n is stuck in the swamp!",ch,NULL,victim,TO_ROOM, FALSE);
    WAIT_STATE(ch,6);
    return;
  }
  was_in = ch->in_room;
  for ( attempt = 0; attempt < 6; attempt++ )
  {
    EXIT_DATA *pexit;
    int door;

    door = number_door( );
    if ((pexit = was_in->exit[door] ) == 0 || pexit->to_room == NULL || 
       IS_SET(pexit->exit_info, EX_CLOSED) || IS_SET(pexit->exit_info, EX_ICE_WALL)
      || ( IS_NPC(ch)      &&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)))
      continue;
    move_char( ch, door );
    if (( now_in = ch->in_room ) == was_in)
      continue;
    ch->in_room = was_in;
    act( "$n has fled!", ch, NULL, NULL, TO_ROOM, FALSE );
    ch->in_room = now_in;
    if ( !IS_NPC(ch) )
      send_to_char( "You flee from combat!  Coward!\n\r", ch );
    stop_fighting( ch, TRUE );
    WAIT_STATE(ch, 10);
    return;
  }
  send_to_char( "You cannot find an exit to flee through!\n\r", ch );
  WAIT_STATE(ch,6);
  return;
}

void do_rescue( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  CHAR_DATA *fch;
  
  one_argument( argument, arg );
  if ( arg[0] == '\0' )
  {
    send_to_char( "Rescue whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "What about fleeing instead?\n\r", ch );
    return;
  }
  if (IS_NPC(ch) && !IS_NPC(victim))
  {
    return;
  }
  if (!IS_NPC(ch) && IS_NPC(victim))
  {
    send_to_char( "Doesn't need your help!\n\r", ch );
    return;
  }
  if ( ch->fighting == victim )
  {
    send_to_char( "Too late.\n\r", ch );
    return;
  }
  if ( ( fch = victim->fighting ) == NULL )
  {
    send_to_char( "That person is not fighting right now.\n\r", ch );
    return;
  }


  if ( is_safe(ch, fch) || is_safe(ch, victim) ) return;
  WAIT_STATE( ch, skill_table[gsn_rescue].beats );
  if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_rescue] )
  {
    send_to_char( "You fail the rescue.\n\r", ch );
    return;
  }
  act( "You rescue $N!",  ch, NULL, victim, TO_CHAR   , FALSE );
  act( "$n rescues you!", ch, NULL, victim, TO_VICT   , FALSE );
  act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT, FALSE );
  stop_fighting( fch, FALSE );
  stop_fighting( victim, FALSE );
  check_killer( ch, fch );
  set_fighting( ch, fch );
  set_fighting( fch, ch );
  set_fighting( victim, fch );
  do_humanity(ch,"");
  return;
}

void do_kick( CHAR_DATA *ch, char *argument )
{
  char buf [MSL];
  CHAR_DATA *victim;
  OBJ_DATA *boots;
  int dam;
  int stance;
  int hitpoints;

  if (!IS_NPC(ch) && ch->level < skill_table[gsn_kick].skill_level)
  {
    send_to_char("First you should learn to kick.\n\r", ch );
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_kick].beats );
  if (IS_NPC(ch))
  {
    dam=ch->level * 2;
    damage(ch,victim,dam,gsn_kick);
    return;
  }

  if (!IS_NPC(ch))
  {
   if (number_percent( ) > ch->pcdata->learned[gsn_kick])
   {
    if (number_range(1,10) == 1)
    {
      sprintf(buf, "You learn from your mistakes, and your %s skill improves!\n\r",
        skill_table[gsn_kick].name);
      stc(buf,ch);
      ch->pcdata->learned[gsn_kick] += 1;
    }
    else
    {
	stc("You wave your leg around foolishly!\n\r",ch);
	return;
    }
   }
   else
   {
    dam += char_damroll(ch);
    if (dam == 0) dam = 1;
    if (!IS_AWAKE(victim)) dam *= 2;
    if(IS_NPC(ch)) dam *= 100;
    if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
    {
      if (IS_SET(victim->special, SPC_WOLFMAN)) dam *= 0.8;
      if (victim->power[DISC_WERE_BOAR] > 2 ) dam *= 0.5;
      if ((boots = get_eq_char( ch, WEAR_FEET ) ) != NULL 
      && IS_SET(boots->spectype, SITEM_SILVER)) dam *= 2;
    }
    if (!IS_NPC(ch)) dam = dam + (dam * ((ch->wpn[0]+1) / 100));
    if (!IS_NPC(ch))
    {
      stance = ch->stance[0];
      if ( IS_STANCE(ch, STANCE_NORMAL) ) dam *= 1.25;
      else dam = dambonus(ch,victim,dam,stance);
    }
    if (!IS_NPC(ch) && !IS_NPC(victim) && dam > 750) dam = 750;
    if (dam <= 0) dam = 2;
    dam = cap_dam(ch, victim, dam);
    hitpoints = victim->hit;
    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_KICK) && !IS_NPC(ch)       && !(IS_CLASS(ch, CLASS_DEMON) && IS_DEMAFF(ch, DEM_HOOVES)) && !(IS_CLASS(ch, CLASS_SHAPESHIFTER)    && ch->pcdata->powers[SHAPE_FORM] == BULL_FORM))
    {
      if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMAFF(ch, DEM_HOOVES))
        dam = dam /2;
      if (IS_NPC(ch))
        dam = dam * 10;
      victim->hit += dam;
      damage( ch, victim, dam, gsn_kick );
      victim->hit = hitpoints;
    }
    else if IS_IMMUNE(victim, IMM_KICK)
    {
      dam=dam/2;
      damage( ch, victim, dam, gsn_kick );
    }
    else damage( ch, victim, dam, gsn_kick );
    return;
   }
  }
 return;
}


int dambonus( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance)
{
  if (dam < 1) return 0;
  if (stance < 1) return dam;
  if (!IS_NPC(ch) && !can_counter(victim))
  {
    if ( IS_STANCE(ch, STANCE_MONKEY) )
    {
      int mindam = dam * 0.25;
      dam *= (ch->stance[STANCE_MONKEY]+1) / 200;
      if (dam < mindam) dam = mindam;
    }
    else if ( IS_STANCE(ch, STANCE_BULL) && ch->stance[STANCE_BULL] > 100 )
      dam += dam * (ch->stance[STANCE_BULL] / 100);
    else if ( IS_STANCE(ch, STANCE_DRAGON) && ch->stance[STANCE_DRAGON] > 100 )
      dam += dam * (ch->stance[STANCE_DRAGON] / 100);
    else if ( IS_STANCE(ch, STANCE_WOLF) && ch->stance[STANCE_WOLF] > 100)
      dam += dam*(ch->stance[STANCE_WOLF] /100);
    else if ( IS_STANCE(ch, STANCE_TIGER) && ch->stance[STANCE_TIGER] > 100 )
      dam += dam * (ch->stance[STANCE_TIGER] / 100);
    /* new shit stance super JOBO */
    else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_3) 
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 3 * (ch->stance[(ch->stance[0])] / 100);
    else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_2)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * 2 * (ch->stance[(ch->stance[0])] / 100);  
    else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_1)
    && ch->stance[(ch->stance[0])] >100)
      dam += dam * (ch->stance[(ch->stance[0])] / 100);
    /* end of new shit */
    else if ( ch->stance[0] > 0 && ch->stance[stance] < 100 )
      dam *= 0.5;
  }
  if (!IS_NPC(victim) && !can_counter(ch))
  {
    if ( IS_STANCE(victim, STANCE_CRAB) && victim->stance[STANCE_CRAB] > 100 )
      dam /= victim->stance[STANCE_CRAB]/100;
    else if ( IS_STANCE(victim, STANCE_DRAGON) && victim->stance[STANCE_DRAGON] > 100)
      dam /= victim->stance[STANCE_DRAGON]/100;
    else if ( IS_STANCE(victim, STANCE_DRAGON) && victim->stance[STANCE_DRAGON] > 100 )
      dam /= victim->stance[STANCE_DRAGON]/100;
    else if ( IS_STANCE(victim, STANCE_SWALLOW) && victim->stance[STANCE_SWALLOW] > 100 )
      dam /= victim->stance[STANCE_SWALLOW]/100;
    /* new shit stance super JOBO */
    else if ( victim->stance[0] > 12 && 
    IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_3) 
    && victim->stance[(victim->stance[0])] >100)
      dam /= 3 * (victim->stance[(victim->stance[0])] / 100);
    else if ( victim->stance[0] > 12 &&
    IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_2)
    && victim->stance[(victim->stance[0])] >100)
      dam /= 2 * (victim->stance[(victim->stance[0])] / 100);
    else if ( victim->stance[0] > 12 &&
    IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_1)
    && victim->stance[(victim->stance[0])] >100)
      dam /= (victim->stance[(victim->stance[0])] / 100);
    /* end of new shit */
  }
  return dam;
}

void do_punch( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  int dam;
  int store;
  bool broke = FALSE;

  one_argument(argument,arg);
  if ( IS_NPC(ch) ) return;
  if ( ch->level < skill_table[gsn_punch].skill_level)
  {
    send_to_char( "First you should learn to punch.\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  if ( ch == victim )
  {
    send_to_char( "You cannot punch yourself!\n\r", ch );
    return;
  }
  if (is_safe(ch,victim)) return;
  if ( victim->hit < victim->max_hit )
  {
    send_to_char( "They are hurt and suspicious.\n\r", ch );
    return;
  }
  if ( victim->position < POS_FIGHTING )
  {
    send_to_char( "You can only punch someone who is standing.\n\r", ch );
    return;
  }
  act("You draw your fist back and aim a punch at $N.",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n draws $s fist back and aims a punch at you.",ch,NULL,victim,TO_VICT, FALSE);
  act("$n draws $s fist back and aims a punch at $N.",ch,NULL,victim,TO_NOTVICT, FALSE);
  WAIT_STATE( ch, skill_table[gsn_punch].beats );
  if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_punch] )
    dam = number_range(1,4);
  else
  {
    dam = 0;
    damage( ch, victim, dam, gsn_punch );
    return;
  }
  dam += char_damroll(ch);
  if (dam == 0) dam = 1;
  if (!IS_AWAKE(victim)) dam *= 2;
  if ( !IS_NPC(ch) ) dam = dam + (dam * (ch->wpn[0] / 100));
  if ( dam <= 0 ) dam = 1;
  if (dam > 1000) dam = 1000;
  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_BOAR] > 3)
  {
    store = victim->hit;
    victim->hit += dam;
    damage( ch, victim, dam, gsn_punch );
    victim->hit = store;
    if (number_percent() <= 25 && !IS_ARM_L(ch,LOST_ARM) && !IS_ARM_L(ch,LOST_HAND))
    {
      if (!IS_ARM_L(ch, LOST_FINGER_I) && !IS_ARM_L(ch, BROKEN_FINGER_I))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);broke = TRUE;}
      if (!IS_ARM_L(ch, LOST_FINGER_M) && !IS_ARM_L(ch, BROKEN_FINGER_M))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);broke = TRUE;}
      if (!IS_ARM_L(ch, LOST_FINGER_R) && !IS_ARM_L(ch, BROKEN_FINGER_R))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);broke = TRUE;}
      if (!IS_ARM_L(ch, LOST_FINGER_L) && !IS_ARM_L(ch, BROKEN_FINGER_L))
        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);broke = TRUE;}
      if (broke)
      {
        act("The fingers on your left hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR, FALSE);
        act("The fingers on $n's left hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM, FALSE);
      }
    }
    else if (number_percent() <= 25 && !IS_ARM_R(ch,LOST_ARM) && !IS_ARM_R(ch,LOST_HAND))
    {
      if (!IS_ARM_R(ch, LOST_FINGER_I) && !IS_ARM_R(ch, BROKEN_FINGER_I))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);broke = TRUE;}
      if (!IS_ARM_R(ch, LOST_FINGER_M) && !IS_ARM_R(ch, BROKEN_FINGER_M))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);broke = TRUE;}
      if (!IS_ARM_R(ch, LOST_FINGER_R) && !IS_ARM_R(ch, BROKEN_FINGER_R))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);broke = TRUE;}
      if (!IS_ARM_R(ch, LOST_FINGER_L) && !IS_ARM_R(ch, BROKEN_FINGER_L))
        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);broke = TRUE;}
      if (broke)
      {
        act("The fingers on your right hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR, FALSE);
        act("The fingers on $n's right hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM, FALSE);
      }
    }
    stop_fighting(victim,TRUE);
    return;
  }
  damage( ch, victim, dam, gsn_punch );
  if (victim == NULL || victim->position == POS_DEAD || dam < 1) return;
  if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
  if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_NOSE) && !IS_HEAD(victim,LOST_NOSE))
  {
    act("Your nose shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR, 
FALSE);
    act("$n's nose shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM, 
FALSE);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
  }
  else if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_JAW))
  {
    act("Your jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR, FALSE);
    act("$n's jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM, FALSE);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
  }
  act("You fall to the ground stunned!",victim,NULL,NULL,TO_CHAR, FALSE);
  act("$n falls to the ground stunned!",victim,NULL,NULL,TO_ROOM, FALSE);
  victim->position = POS_STUNNED;
  if( IS_NPC(ch) )
  {
    switch( ch->pIndexData->vnum ){
    case 30003:
    case 90500:
    case 90501:
    case 20050:
    case 30704:
    case 30703:
    case 10000:
    case 10001:
    case 10002:
    case 10003:
    case 10004:
    break;
    default:
    {
      victim->position = POS_STANDING;
      victim->hit = victim->max_hit -100;
    }}
  }
  if (dam > 1000) dam = 1000;
  return;
}

void do_spin( CHAR_DATA *ch, char *argument )
{
  char buf[MSL];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;
  int number_hit = 0;
  if (IS_NPC(ch)) return;
  if ( ch->level < skill_table[gsn_spin].skill_level)
  {
    send_to_char( "Spin kick? You can't do that!\n\r", ch );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_spin].beats * 0.30);
  if ( !IS_NPC( ch ) &&  number_percent( ) > ch->pcdata->learned[gsn_spin] )
  {
    act("You jump in the air, turning sideways.. and fall on your face.",ch,NULL,NULL,TO_CHAR, FALSE);
    act("$n jumps in the air and twists around before falling flat on $s face.",ch,NULL,NULL,TO_ROOM, FALSE);
    if (number_range(1,10) == 1)
    {
      sprintf(buf, "You learn from your mistakes, and your %s skill improves!\n\r",skill_table[gsn_spin].name);
      stc(buf,ch);
      ch->pcdata->learned[gsn_spin] += 1;
    }

    return;
  }
  stc("#7You execute a beautiful #CButterfly Kick#n#7!#n\n\r",ch);
  act("#7$n executes a beautiful #CButterfly Kick#n#7!#n",ch,NULL,NULL,TO_NOTVICT, FALSE);
  for ( vch = char_list; vch != NULL; vch = vch_next )
  {
    vch_next  = vch->next;
    if (number_hit > 10) continue;
    if (vch->in_room == NULL) continue;
    if (!IS_NPC(vch)) continue;
    if (xIS_SET(vch->act, ACT_NOEXP)) continue;
    if ( vch->level > 5000) continue; 
    if ( ch == vch ) continue;
    if ( vch->in_room == ch->in_room )
    {
      if ( ( mount = ch->mount ) != NULL )
      if ( mount == vch ) continue;
      if (can_see(ch,vch))
      {
        if (is_same_group(ch, vch)) continue;
        multi_hit( ch, vch, TYPE_UNDEFINED );
        number_hit++;
	if (number_range(1,2)==1)
        {
          multi_hit(ch,vch,TYPE_UNDEFINED);
          number_hit++;
        }
      }
    }
  }
  do_beastlike(ch,"");
  return;
}

void do_berserk( CHAR_DATA *ch, char *argument )
{
  char buf[MSL];
  char arg [MAX_INPUT_LENGTH];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;
  int number_hit = 0;
  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if ( ch->level < skill_table[gsn_berserk].skill_level)
  {
    send_to_char( "You are not wild enough to go berserk.\n\r", ch );
    return;
  }
  if (IS_CLASS(ch, CLASS_PALADIN))
  {
    WAIT_STATE( ch, skill_table[gsn_berserk].beats * 0.90);
  }
  else if (IS_CLASS(ch, CLASS_WEREWOLF))
  {
    WAIT_STATE( ch, skill_table[gsn_berserk].beats * 0.85);
  }

  else WAIT_STATE( ch, skill_table[gsn_berserk].beats);
  if ( !IS_NPC( ch ) &&  number_percent( ) > ch->pcdata->learned[gsn_berserk] )
  {
    act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR, FALSE);
    act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM, FALSE);
    if (number_range(1,10) == 1)
    {
      sprintf(buf, "You learn from your mistakes, and your %s skill improves!\n\r",
	skill_table[gsn_berserk].name);
      stc(buf,ch);
      ch->pcdata->learned[gsn_berserk] += 1;
    }
    return;
  }
  act("You go BERSERK!",ch,NULL,NULL,TO_CHAR, FALSE);
  act("$n goes BERSERK!",ch,NULL,NULL,TO_ROOM, FALSE);
  for ( vch = char_list; vch != NULL; vch = vch_next )
  {
    vch_next  = vch->next;
    if (number_hit > 10) continue;
    if (vch->in_room == NULL) continue;
    if (!IS_NPC(vch) && vch->pcdata->chobj != NULL) continue;
    if (!IS_NPC(ch) && !IS_NPC(vch) && (is_fighting_player(ch,vch))) continue;
    if (!IS_NPC(ch) && !IS_NPC(vch) && (is_range(ch,vch))) continue;
    if ( ch == vch ) continue;
    if ( vch->in_room == ch->in_room ) 
    {
    if (is_same_group(ch, vch)) continue;
      if ( ( mount = ch->mount ) != NULL ) 
        if ( mount == vch ) continue;
      if (can_see(ch,vch))
      {
        multi_hit( ch, vch, TYPE_UNDEFINED );
        number_hit++;
        if (
	   ch->rage > 120 ||
	  (IS_CLASS(ch, CLASS_HIGHLANDER) && number_range(1,2)==1) ||
	   IS_CLASS(ch, CLASS_PALADIN) ||
	  (IS_CLASS(ch, CLASS_DROW) && number_range(1,2)==1) ||
	  (IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 2)
	   )
        {
          send_to_char("You go NUTZ!\n\r",ch);
          act("$n GOES FUCKING NUTZ!!",ch,NULL,NULL,TO_ROOM, FALSE);
          multi_hit(ch,vch,TYPE_UNDEFINED);
          number_hit++;
        }
      }
    }
  }
  do_beastlike(ch,"");
  return;
}
       
/* Hurl skill by KaVir */
void do_hurl( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  CHAR_DATA *mount;
  ROOM_INDEX_DATA *to_room;
  EXIT_DATA *pexit;
  EXIT_DATA *pexit_rev;
  char buf [MAX_INPUT_LENGTH];
  char direction [MAX_INPUT_LENGTH];
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  int door;
  int rev_dir;
  int dam;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  if ( !IS_NPC(ch) && ch->pcdata->learned[gsn_hurl] < 1 )
  {
    send_to_char("Maybe you should learn the skill first?\n\r",ch);
    return;
  }
  if ( arg1[0] == '\0' )
  {
    send_to_char("Who do you wish to hurl?\n\r", ch);
    return;
  }
  if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "How can you hurl yourself?\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;
  if ( (mount = victim->mount) != NULL && victim->mounted == IS_MOUNT)
  {
    send_to_char("But they have someone on their back!\n\r",ch);
    return;
  }
  else if ( (mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
  {
    send_to_char("But they are riding!\n\r",ch);
    return;
  }
  if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_HURL) )
  {
    send_to_char("You are unable to get their feet of the ground.\n\r",ch);
    return;
  }
  if ( IS_NPC(victim) && victim->level > 5000 )
  {
    send_to_char("You are unable to get their feet of the ground.\n\r",ch);
    return;
  }
  if ( (victim->hit < victim->max_hit)
     || ( victim->position == POS_FIGHTING && victim->fighting != ch) )
  {
    act( "$N is hurt and suspicious, and you are unable to approach $M.", ch, NULL, victim, 
TO_CHAR, FALSE );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_hurl].beats );
  if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_hurl] )
  {
    send_to_char("You are unable to get their feet of the ground.\n\r",ch);
    multi_hit( victim, ch, TYPE_UNDEFINED );
    return;
  }
  rev_dir = 0;
  if ( arg2[0] == '\0' ) door = number_range(0,3);
  else
  {
    if      (!str_cmp( arg2, "n" ) || !str_cmp( arg2, "north")) door = 0;
    else if (!str_cmp( arg2, "e" ) || !str_cmp( arg2, "east")) door = 1;
    else if (!str_cmp( arg2, "s" ) || !str_cmp( arg2, "south")) door = 2;
    else if (!str_cmp( arg2, "w" ) || !str_cmp( arg2, "west")) door = 3;
    else
    {
      send_to_char("You can only hurl people north, south, east or west.\n\r", ch);
      return;
    }
  }
  if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
  if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
  if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
  if (door == 3) {sprintf(direction,"west");rev_dir = 1;}
  if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
  {
    sprintf(buf,"$n hurls $N into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
    sprintf(buf,"You hurl $N into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    sprintf(buf,"$n hurls you into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_VICT, FALSE);
    dam = number_range(ch->level, (ch->level * 4));
    victim->hit = victim->hit - dam;
    update_pos(victim);
    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
    if (victim->position == POS_DEAD) {raw_kill(victim);return;}
    return;
  }
  pexit = victim->in_room->exit[door];
  if( IS_SET(pexit->exit_info, EX_PRISMATIC_WALL))
  {
    sprintf(buf,"$n hurls $N into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
    sprintf(buf,"You hurl $N into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    sprintf(buf,"$n hurls you into the %s wall.", direction);
    act(buf,ch,NULL,victim,TO_VICT, FALSE);
    dam = number_range(ch->level, (ch->level * 4));
    victim->hit = victim->hit - dam;
    update_pos(victim);
    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
    if (victim->position == POS_DEAD) {raw_kill(victim);return;}
    return;
  }
  if (IS_SET(pexit->exit_info, EX_CLOSED) && !IS_AFFECTED(victim,AFF_PASS_DOOR) && !IS_AFFECTED(victim,AFF_ETHEREAL))
  {
    if (IS_SET(pexit->exit_info, EX_LOCKED))
      REMOVE_BIT(pexit->exit_info, EX_LOCKED);
    if (IS_SET(pexit->exit_info, EX_CLOSED))
      REMOVE_BIT(pexit->exit_info, EX_CLOSED);
    sprintf(buf,"$n hoists $N in the air and hurls $M %s.", direction);
    act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
    sprintf(buf,"You hoist $N in the air and hurl $M %s.", direction);
    act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    sprintf(buf,"$n hurls you %s, smashing you through the %s.", direction,pexit->keyword);
    act(buf,ch,NULL,victim,TO_VICT, FALSE);
    sprintf(buf,"There is a loud crash as $n smashes through the $d.");
    act(buf,victim,NULL,pexit->keyword,TO_ROOM, FALSE);
    if ((to_room = pexit->to_room ) != NULL && (pexit_rev = to_room->exit[rev_dir]) != NULL
      && pexit_rev->to_room == ch->in_room && pexit_rev->keyword != NULL)
    {
      if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
        REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
      if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
        REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
      if (door == 0) sprintf(direction,"south");
      if (door == 1) sprintf(direction,"west");
      if (door == 2) sprintf(direction,"north");
      if (door == 3) sprintf(direction,"east");
      char_from_room(victim);
      char_to_room(victim,to_room);
      sprintf(buf,"$n comes smashing in through the %s $d.", direction);
      act(buf,victim,NULL,pexit->keyword,TO_ROOM, FALSE);
      dam = number_range(ch->level, (ch->level * 6));
      victim->hit = victim->hit - dam;
      update_pos(victim);
      if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
      if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
      if (victim->position == POS_DEAD) {raw_kill(victim);return;}
    }
  }
  else
  {
    sprintf(buf,"$n hurls $N %s.", direction);
    act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
    sprintf(buf,"You hurl $N %s.", direction);
    act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    sprintf(buf,"$n hurls you %s.", direction);
    act(buf,ch,NULL,victim,TO_VICT, FALSE);
    if (door == 0) sprintf(direction,"south");
    if (door == 1) sprintf(direction,"west");
    if (door == 2) sprintf(direction,"north");
    if (door == 3) sprintf(direction,"east");
    char_from_room(victim);
    char_to_room(victim,to_room);
    sprintf(buf,"$n comes flying in from the %s.", direction);
    act(buf,victim,NULL,NULL,TO_ROOM, FALSE);
    dam = number_range(ch->level, (ch->level * 2));
    victim->hit = victim->hit - dam;
    update_pos(victim);
    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
    if (victim->position == POS_DEAD) {raw_kill(victim);return;}
  }
  return;
}

void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    int percent;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_disarm].skill_level)
    {
  send_to_char( "You don't know how to disarm opponents.\n\r", ch );
  return;
    }

    if ( ( get_eq_char( ch, WEAR_WIELD ) == NULL )
    &&   ( get_eq_char( ch, WEAR_HOLD  ) == NULL ) )
    {
  send_to_char( "You must wield a weapon to disarm.\n\r", ch );
  return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
  send_to_char( "You aren't fighting anyone.\n\r", ch );
  return;
    }

    if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    &&   ( ( obj = get_eq_char( victim, WEAR_HOLD  ) ) == NULL ) )
    {
  send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
  return;
    }

    WAIT_STATE( ch, skill_table[gsn_disarm].beats );
    percent = number_percent( ) + victim->level - ch->level;
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_DISARM))
  send_to_char( "You failed.\n\r", ch );
    else if ( IS_NPC(ch) || percent < ch->pcdata->learned[gsn_disarm] * 2 / 3 )
  disarm( ch, victim );
    else
  send_to_char( "You failed.\n\r", ch );
    return;
}



void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}

/* Had problems with people not dying when POS_DEAD...KaVir */
void do_killperson( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  one_argument( argument, arg );
  if ( arg[0] == '\0' ) return;
  if ( ( victim = get_char_room( ch, arg ) ) == NULL ) return;
  send_to_char( "You have been KILLED!!\n\r\n\r", victim );
  if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
  if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
  raw_kill( victim );
  return;
}
void do_mercy( CHAR_DATA *ch, char *argument )
{
CHAR_DATA *victim;
  OBJ_DATA  *obj;
char arg[MAX_STRING_LENGTH];
char buf[MAX_STRING_LENGTH];
  one_argument( argument, arg );
if ( IS_NPC( ch ) ) return;
 if ( arg[0] == '\0' )
 {
 send_to_char( "Show mercy to whom?\n\r", ch );
 return;
 }
 if ( ( victim = get_char_room( ch, arg ) ) == NULL )
 {
 send_to_char( "They aren't here.\n\r", ch );
 return;
 }

  if( ch->challenger )
  {
   chprintf( ch, "You cant show mercy in the arena.");
   return;
  }

  if( IS_SET(victim->in_room->room_flags,ROOM_ARENA) )
  {
    send_to_char("#0You cannot show mercy in the #RFREE #yFOR #RALL#0.#n\n\r",ch);
    return;
  }

  if (str_cmp(victim->pcdata->killedby, ch->pcdata->switchname))
  {
    sprintf(buf, "You did not tie %s, you cannot tear %s head off.\n\r",
ch->sex == SEX_MALE ? "him" : ch->sex == SEX_FEMALE ? "her" : "it",
ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
    stc(buf,ch);
    return;
  }

  if (!xIS_SET(victim->extra,TIED_UP))
  {
    send_to_char( "You can only show mercy to somebody who is tied up!\n\r",ch);
    return;
  }

  if (!IS_CLASS(ch, CLASS_MONK))
if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
 {
  if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
  {
  send_to_char( "First you better get a weapon out!\n\r", ch );
  return;
  }
 }
  if (!IS_CLASS(ch, CLASS_MONK))
 if ( ( obj != NULL && obj->item_type != ITEM_WEAPON ) )
 {
 send_to_char( "But you are not wielding any weapons!\n\r", ch );
 return;
 }
 if ( ch == victim )
 {
 send_to_char( "That might be a bit tricky...\n\r", ch );
 return;
 }
 if (IS_NPC(victim))
 {
 send_to_char( "Show mercy to a mob? NEVER!\n\r",ch);
 return;
 }
 if ( victim->position > 1 )
 {
 send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
 return;
 }
// if((get_age(ch)-17)<4)
 if (IS_SET(ch->special, SPC_NEWBIE))
 {
 send_to_char("You'll have to wait till you are not a newbie.\n\r",ch);
 return;
 }
// if((get_age(victim)-17)<4)
 if (IS_SET(victim->special, SPC_NEWBIE))
 {
 send_to_char("They are a newbie, don't hurt them.\n\r",ch);
 return;
 }
 if (is_safe(ch,victim)) return;
  sprintf(buf, "#P%s #0has been left for dead by #C%s#n.",victim->pcdata->switchname,ch->pcdata->switchname);
  do_update(ch, buf);
  sprintf(log_buf, "%s shown mercy by %s at %d.",victim->pcdata->switchname,ch->pcdata->switchname,victim->in_room->vnum);
  log_string(log_buf);
  victim->fight_timer += 10;
  if (ch->fight_timer > 10)
  ch->fight_timer = 10;
  act( "You bring your weapon down upon $N's neck!",  ch, NULL, victim, TO_CHAR   , FALSE );
  sprintf(buf, "Your life is spared by the mercy of %s!\n\r", ch->pcdata->switchname);
  stc(buf, victim);
  act( "$n swings $s weapon down towards $N's neck!",  ch, NULL, victim, TO_NOTVICT, FALSE );
  act( "$n's head is spared by $N's mercy!", victim, NULL, ch, TO_ROOM, FALSE);
  act( "The gods shine upon the mecrcy you have shown $N.",  ch, NULL, victim, TO_VICT, FALSE );
  act( "The gods shine upon $n as $m shows $N mercy.",  ch, NULL, victim, TO_NOTVICT, FALSE );
  act( "You fall to the ground in agony as the gods smile on you.",  ch, NULL, NULL, 
TO_CHAR, FALSE    );
  act( "$n falls to the ground as the gods smile at $m.",  ch, NULL, NULL, TO_NOTVICT, 
FALSE );
  if (  ch->hit < ch->max_hit * 0.95) ch->hit = ch->max_hit * 0.95;
  if ( ch->mana < ch->max_mana * 0.95) ch->mana = ch->max_mana * 0.95;
  if ( ch->move < ch->max_move * 0.95) ch->move = ch->max_move * 0.95;
  char_from_room(victim);
  char_to_room(victim,get_room_index(ROOM_VNUM_NECROPOLIS));
  WAIT_STATE(ch, 30);
  WAIT_STATE(victim,50);
  return;
}

/* For decapitating players - KaVir */
void do_decapitate( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA  *obj;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
//  char buf2[MSL];
  int value;
//  int status; 
//  int next_status;
  if (IS_NPC(ch)) return;
  one_argument( argument, arg );
  if( ch->challenger )
  {
   chprintf( ch, "You cant decap in the arena.");
   return;
  }
  if (!IS_CLASS(ch, CLASS_MONK))
  if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
  {
    if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
      send_to_char( "First you better get a weapon out!\n\r", ch );
      return;
    }
  }
  if (!IS_CLASS(ch, CLASS_MONK))
  if ( ( obj != NULL && obj->item_type != ITEM_WEAPON ) )
  {
      send_to_char( "But you are not wielding any weapons!\n\r", ch );
      return;
  }
  if ( arg[0] == '\0' )
  {
    send_to_char( "Decapitate whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( ch == victim )
  {
    send_to_char( "That might be a bit tricky...\n\r", ch );
    return;
  }
  if ( IS_NPC(victim) )
  {
    send_to_char( "You can only decapitate other players.\n\r", ch );
    return;
  }
   if ( !CAN_PK(ch) )
  {
    send_to_char( "You must be an avatar to decapitate someone.\n\r", ch );
    return;
  }
  if ( !CAN_PK(victim) )
  {
    send_to_char( "You can only decapitate other avatars.\n\r", ch );
    return;
  }
  if ( victim->position > 1 )
  {
    send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
    return;
  }
//  if((get_age(ch)-17)<4)
  if (IS_SET(ch->special, SPC_NEWBIE))
  {
    send_to_char("You'll have to wait till you are not a newbie.\n\r",ch);
    return;
  }
  if( IS_SET(victim->in_room->room_flags,ROOM_ARENA) )
  {
    send_to_char("#0You cannot decapitate in the #RFREE #yFOR #RALL#0.#n\n\r",ch);
    return;
  }
//  if((get_age(victim)-17)<4)
  if (IS_SET(victim->special, SPC_NEWBIE))
  {
    send_to_char("They are a newbie, don't hurt them.\n\r",ch);
    return;
  }
  if (victim->pcdata->killedby != NULL)
  {
  if (str_cmp(victim->pcdata->killedby, ch->pcdata->switchname))
  {
    sprintf(buf, "You did not tie %s, you cannot cut %s head off.\n\r",
ch->sex == SEX_MALE ? "him" : ch->sex == SEX_FEMALE ? "her" : "it",
ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
    stc(buf,ch);
    return;
  }

  }
  if (is_safe(ch,victim)) return;
  if (IS_CLASS(ch, CLASS_VAMPIRE) && IS_SET(ch->special, SPC_INCONNU) && (IS_CLASS(victim, CLASS_VAMPIRE) || IS_VAMPAFF(victim, VAM_MORTAL)))
  {
    send_to_char( "You cannot decapitate another vampire.\n\r", ch );
    return;
  }
  if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->special, SPC_INCONNU) && (IS_CLASS(ch, CLASS_VAMPIRE) || IS_VAMPAFF(ch, VAM_MORTAL)))
  {
    send_to_char( "You cannot decapitate an Inconnu vampire.\n\r", ch );
    return;
  }
  if (!xIS_SET(victim->extra,TIED_UP))
  {
    send_to_char( "You can only decapitate somebody who is tied up!\n\r",ch);
    return;
  }
  act( "You bring your weapon down upon $N's neck!",  ch, NULL, victim, TO_CHAR , FALSE   
);
  send_to_char( "Your head is sliced from your shoulders!\n\r", victim);
  if (!IS_NPC(ch) && !IS_NPC(victim) && victim->pcdata->bounty > 0)
  {
    sprintf(buf,"You recive a %d QP bounty, for killing %s.\n\r",
    victim->pcdata->bounty, victim->name);
    send_to_char(buf, ch);
    ch->pcdata->quest += victim->pcdata->bounty;
    ch->pcdata->score[SCORE_QUEST] += victim->pcdata->bounty;
    victim->pcdata->bounty =0;
  }
  if (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION)) 
  if ((IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION)) && 
    !IS_CLASS(victim,CLASS_DEMON) && !IS_SET(victim->special,SPC_CHAMPION) && !IS_CLASS(ch, CLASS_DROW))
  {
    if (ch->race == 0 && victim->race == 0)
    {
      ch->pcdata->stats[DEMON_CURRENT] += 1000;
      ch->pcdata->stats[DEMON_TOTAL] += 1000;
    }
    else
    {
      ch->pcdata->stats[DEMON_CURRENT] += victim->race * 1000;
      ch->pcdata->stats[DEMON_TOTAL] += victim->race * 1000;
    }
  }
  if (IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(victim, CLASS_DEMON) && !IS_CLASS(victim, CLASS_DROW)) 
  {
    if (victim->race == 0 && ch->race == 0 ) ch->pcdata->stats[DROW_POWER] += 1000;
    else ch->pcdata->stats[DROW_POWER] += 1000* victim->race;
  }
  ch->exp += victim->exp / 4;
  victim->exp -= victim->exp/4;


  if (IS_CLASS(victim, CLASS_VAMPIRE))      do_mortalvamp(victim,"");
  else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
  if (IS_CLASS(victim, CLASS_MAGE) && IS_AFFECTED(victim, AFF_POLYMORPH)) do_unpolymorph(victim,"");
  ch->fight_timer += 10;
  if (IS_CLASS(ch, CLASS_DEMON) && !IS_NPC(ch) && !IS_NPC(victim) && victim->race + 15 >= ch->race)
  {
    send_to_char("You rip away a part of their soul.\n\r", ch);
    send_to_char("You feel a part of your soul has been torn away.\n\r", victim);
    ch->pcdata->souls += 1;
  }
  if (IS_CLASS(ch, CLASS_PSIONIST) && !IS_NPC(ch) && !IS_NPC(victim) &&
IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDTRAP))
  {
    send_to_char("You take their mental energies into your mindtrap.\n\r", ch);
    send_to_char("You feel your mind being ripped out of your skull!\n\r", victim);
    ch->pcdata->souls += 1;
  }
 if (IS_CLASS(ch, CLASS_INQUISITOR) && !IS_NPC(ch) && !IS_NPC(victim) &&
ch->pcdata->powers[PINQUISITOR] > 0)
  {
    send_to_char("You rip away a part of their soul.\n\r", ch);
    send_to_char("You feel a part of your soul has been torn away.\n\r", victim);
    ch->pcdata->souls += 1;
  }
  if (IS_SET(victim->pcdata->powers[AURAS], MIGHT_AURA) && IS_CLASS(victim, 
CLASS_UNDEAD_KNIGHT))
    {
      REMOVE_BIT(victim->pcdata->powers[AURAS], MIGHT_AURA);
      send_to_char("Your aura of might fades.\n\r",victim);
    }
    if(!IS_NPC(victim))
    victim->pcdata->killedby = ch->pcdata->switchname;
 if (IS_SET(victim->monkstuff, MONK_HEAL)) REMOVE_BIT(victim->monkstuff, MONK_HEAL);

  behead( victim );
  dropinvis(ch, victim);
  gainarti(ch);
  /*  Removes mage powers */
  if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD)) REMOVE_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
  if (IS_ITEMAFF(victim, ITEMA_BEAST)) REMOVE_BIT(victim->itemaffect, ITEMA_BEAST);
  if (IS_ITEMAFF(victim, ITEMA_MAGESHIELD)) REMOVE_BIT(victim->itemaffect, ITEMA_MAGESHIELD);
  if (IS_ITEMAFF(victim, ITEMA_ILLUSIONS)) REMOVE_BIT(victim->itemaffect, ITEMA_ILLUSIONS);
  if (IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) REMOVE_BIT(victim->itemaffect, ITEMA_DEFLECTOR);
  if (IS_ITEMAFF(victim, ITEMA_MANASHIELD)) REMOVE_BIT(victim->itemaffect, ITEMA_MANASHIELD);
  do_beastlike(ch,"");
  if (ch->pcdata->kingdom != 0)
  king_table[ch->pcdata->kingdom].pk += 1;
  if (victim->pcdata->kingdom != 0)
  king_table[victim->pcdata->kingdom].pd += 1;

  if ((ch->practice >= 20000) && (victim->practice >= 20000))
  {
  value = victim->practice / 20;
  ch->practice += value;
  victim->practice -= value;
  if (victim->practice < 20000) victim->practice = 20000;
  }

//  save_kingdoms();
  ch->hit = ch->max_hit;
  ch->mana = ch->max_mana;
  ch->move = ch->max_move;

  if (!IS_CLASS(victim, CLASS_NINJA)) victim->rage = 0;


  victim->level = 2;
  victim->trust = 2;
  sprintf(buf,"#y%s#0 has spilt the #Rblood#0 of #y%s#0 with a brutal killing!#n",ch->pcdata->switchname, victim->pcdata->switchname);
  do_update(ch,buf);

  if (ch->race >= 10 )
   { 
    if ( number_percent() > 25)
    {
	if (number_percent() < 90)
	{
          ch->race += 1;
          victim->race -= 1;
          if (victim->race < 0)
          victim->race = 0;
          sprintf(buf,"#y%s#0 has been rewarded for victory, %s gains #R1 status!#n",ch->pcdata->switchname,ch->sex == SEX_MALE ? "he" : ch->sex == SEX_FEMALE ? "she" : "it");
          do_update(NULL,buf);
          sprintf( log_buf, "%s decapitated by %s at %d for status.",
          victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
	}
        else
        {
          ch->race += 2;
          victim->race -= 1;
          if (victim->race < 0)
          victim->race = 0;
          sprintf(buf,"#y%s#0 has been rewarded for a #ygreat#0 victory, %s gains #y2 status!#n",ch->pcdata->switchname,ch->sex == SEX_MALE ? "he" : ch->sex == SEX_FEMALE ? "she" : "it");
          do_update(NULL,buf);
          sprintf( log_buf, "%s decapitated by %s at %d for status.",
          victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
        }
    }
    else
    {
        sprintf(buf,"#y%s #0has learned nothing from killing #R%s#0!#n",
        ch->pcdata->switchname,victim->pcdata->switchname);
        do_update(NULL,buf);
	sprintf( log_buf, "%s decapitated by %s at %d for no status.",
        victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
    }
   }
   else if (ch->race < 10)
    {
        ch->race += 1;
        victim->race -= 1;
        if (victim->race < 0)
        victim->race = 0;
        sprintf(buf,"#y%s#0 is still learning the ways, and gains #R1 status!#n",
        ch->pcdata->switchname);
        do_update(NULL,buf);
        sprintf( log_buf, "%s decapitated by %s at %d for status.",
        victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
    }

        ch->pkill = ch->pkill + 1;
        victim->pdeath = victim->pdeath + 1;

  if(!IS_NPC(victim)) 
  victim->pcdata->killedby = ch->pcdata->switchname;
  players_decap++;
  if (ch->pcdata->bounty <= 99999)
  ch->pcdata->bounty += ((victim->lstatus + 1) * 1000) + (victim->race * 10);
  if (ch->pcdata->bounty > 99999)
  ch->pcdata->bounty = 99999;
  if (ch->pcdata->bounty > 0)
  {
  sprintf(buf,"#y%s #0now has a bounty of #P%d #Pquest points#0 on %s head.#n",ch->pcdata->switchname,ch->pcdata->bounty,ch->sex ==SEX_MALE         ? "his" : ch->sex == SEX_FEMALE       ? "her"      : "its");
  do_update(NULL,buf);
  }
        log_string( log_buf );
  return;
}

void do_tear( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MSL];
  int value;
  int status = (victim->lstatus ) * 5;
    int next_status = (ch->lstatus + 1) * 5;

  one_argument( argument, arg );
  if ( IS_NPC(ch) ) return;
  if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_MONK) )
  {
    send_to_char( "Huh?\n\r", ch );
    return;
  }

  if(ch->challenger)
  {
    chprintf(ch, "You can not tear heads in the arena." );
    return;
  }
  if (!IS_SET(ch->newbits, NEW_MONKADAM) && IS_CLASS(ch, CLASS_MONK))
  {
    send_to_char( "You can only tear heads off with adamantium hands.\n\r", ch );
    return;
  }
  if ( !IS_SET(ch->special, SPC_WOLFMAN) && IS_CLASS(ch, CLASS_WEREWOLF) )
  {
    send_to_char( "You can only tear heads off while in Crinos form.\n\r", ch );
    return;
  }
  if ( !IS_VAMPAFF(ch, VAM_CLAWS) && IS_CLASS(ch, CLASS_WEREWOLF) )
  {
    send_to_char( "You better get your claws out first.\n\r", ch );
    return;
  }
  if ( arg[0] == '\0' )
  {
    send_to_char( "Who's head do you wish to tear off?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( ch == victim )
  {
    send_to_char( "That might be a bit tricky...\n\r", ch );
    return;
  }
  if ( IS_NPC(victim) )
  {
    send_to_char( "You can only tear the heads off other players.\n\r", ch );
    return;
  }
  if ( !CAN_PK(ch) )
  {
    send_to_char( "You must be an avatar to tear someone's head off.\n\r", ch );
    return;
  }
  if ( !CAN_PK(victim) )
  {
    send_to_char( "You can only tear the head off another avatar.\n\r", ch );
    return;
  }
 if( IS_SET(victim->in_room->room_flags,ROOM_ARENA) )
{
    send_to_char("#0You cannot tear heads in the #RFREE #yFOR #RALL#0.#n\n\r",ch);
    return;
}
  if ( victim->position > 1 )
  {
    send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
    return;
  }
  if((get_age(ch)-17)<4)
  {
    send_to_char("You'll have to wait till you have 6  hours.\n\r",ch);
    return;
  }
  if((get_age(victim)-17)<4)
  {
    send_to_char("They are a newbie, don't hurt them.\n\r",ch);    
    return;
  }
  if (!xIS_SET(victim->extra,TIED_UP))
  {
    send_to_char( "You can only tear the head of somebody who is tied up!\n\r",ch);
    return;
  }
  if (str_cmp(victim->pcdata->killedby, ch->pcdata->switchname))
  {
    sprintf(buf, "You did not tie %s, you cannot tear %s head off.\n\r",
victim->sex == SEX_MALE ? "him" : victim->sex == SEX_FEMALE ? "her" : "it",
victim->sex == SEX_MALE ? "his" : victim->sex == SEX_FEMALE ? "her" : "its");
    stc(buf,ch);
    return;
  }

  if (is_safe(ch,victim)) return;
  act( "You tear $N's head from $S shoulders!",  ch, NULL, victim, TO_CHAR, FALSE    );
  send_to_char( "Your head is torn from your shoulders!\n\r", victim);
  act( "$n tears $N's head from $S shoulders!",  ch, NULL, victim, TO_NOTVICT, FALSE );
  if (!IS_NPC(ch) && !IS_NPC(victim) && victim->pcdata->bounty > 0)
  {
    sprintf(buf,"You recive a %d QP bounty, for killing %s.\n\r",
    victim->pcdata->bounty, victim->name);
    send_to_char(buf, ch);
    ch->pcdata->score[SCORE_QUEST] += victim->pcdata->bounty;;
    ch->pcdata->quest += victim->pcdata->bounty;
    victim->pcdata->bounty =0;
  }
  if (IS_CLASS(ch, CLASS_DEMON) && !IS_NPC(ch) && !IS_NPC(victim) && victim->race + 15 >= ch->race)
  {
    send_to_char("You rip away a part of their soul.\n\r", ch);
    send_to_char("You feel a part of your soul has been torn away.\n\r", victim);
    ch->pcdata->souls += 1;
    if (victim->pcdata->souls > 0)
    victim->pcdata->souls -= 1;
  }  if (IS_CLASS(ch, CLASS_PSIONIST) && !IS_NPC(ch) && !IS_NPC(victim) &&
IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MINDTRAP))
  {
    send_to_char("You take their mental energies into your mindtrap.\n\r", ch);
    send_to_char("You feel your mind being ripped out of your skull!\n\r", victim);
    ch->pcdata->souls += 1;
    if (victim->pcdata->souls > 0)
    victim->pcdata->souls -= 1;
  }


  if ( !IS_NPC( ch ) && (IS_CLASS(ch, CLASS_DEMON) || IS_SET(ch->special, SPC_CHAMPION)) && !IS_CLASS(victim, CLASS_DEMON) 
    && !IS_CLASS(victim, CLASS_DROW) && !IS_SET(victim->special, SPC_CHAMPION)) 
  {
    if (ch->race == 0 && victim->race == 0)
    {
      ch->pcdata->stats[DEMON_CURRENT] += 1000;
      ch->pcdata->stats[DEMON_TOTAL] += 1000;
    }
    else
    {
      ch->pcdata->stats[DEMON_CURRENT] += victim->race * 1000;
      ch->pcdata->stats[DEMON_TOTAL] += victim->race * 1000;
    }
  }

  if (IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(victim, CLASS_DROW) && !IS_CLASS(victim, CLASS_DEMON) 
    && !IS_SET(victim->special, SPC_CHAMPION))
  {
    if (ch->race == 0 && victim->race == 0) ch->pcdata->stats[DROW_POWER] += 1000;
    else ch->pcdata->stats[DROW_POWER] += 1000 * victim->race;
  }
  ch->exp += victim->exp / 4;
  victim->exp -= victim->exp/4;
  if (global_status)
  {
        ch->race += 2;
      if (ch->race > next_status)
        ch->race = next_status;
        victim->race -= 1;
        if (victim->race < status)
        victim->race = status;
      stc("#RThe strength of your victim teaches you well, you recieve #ydouble status!#n\n\r",ch);
      sprintf(buf1,"#y%s#0 has earned #Rdouble status#0 for cutting #y%s's#0 head off!#n",ch->pcdata->switchname, victim->pcdata->switchname);
      do_update(ch, buf1);
      sprintf(buf1,"%s has earned double status for cutting %s's head off.",ch->pcdata->switchname, victim->pcdata->switchname);
      log_string(buf1);
  }
  if (!global_status)
  {
    if (number_percent() > 80)
     {
        ch->race += 2;
      if (ch->race > next_status)
        ch->race = next_status;
        victim->race -= 1;
        if (victim->race < status)
        victim->race = status;
      stc("#RThe strength of your victim teaches you well, you recieve #ydouble status!#n\n\r",ch);
      sprintf(buf1,"#y%s#0 has earned #Rdouble status#0 for tearing #y%s's#0 head off!#n",ch->pcdata->switchname, victim->pcdata->switchname);
      do_update(ch, buf1);
      sprintf(buf1,"%s has earned double status for tearing %s's head off.",ch->pcdata->switchname, victim->pcdata->switchname);
      log_string(buf1);
     }
    else
     {
        ch->race += 1;
      if (ch->race > next_status)
        ch->race = next_status;
        victim->race -= 1;
        if (victim->race < status)
        victim->race = status;
     }
        if (victim->race < 10 && IS_SET(victim->special, SPC_GENERAL))
        {
        REMOVE_BIT(victim->special, SPC_GENERAL);
        send_to_char("You are no longer a general.\n\r", victim);
        sprintf(buf2, "%s is too weak to remain a General!",victim->pcdata->switchname);
        do_info(ch,buf2);
        log_string(buf2);
        }
        if (victim->race < 20 && IS_SET(victim->special, SPC_PRINCE))
        {
        REMOVE_BIT(victim->special, SPC_PRINCE);
        send_to_char("You are no longer a prince.\n\r", victim);
        sprintf(buf2, "%s is too weak to remain a Prince!",victim->pcdata->switchname);
        do_info(ch,buf2);
        log_string(buf2);
        }
        if (victim->race < 20 && IS_SET(victim->immune, IMM_SCRY))
        {
        REMOVE_BIT(victim->immune, IMM_SCRY);
        stc("Your eyes stop twitching, causing your awareness to fade.\n\r",victim);
        }

        if (!IS_NPC(victim)  && (victim->race < 15)  && !str_cmp(king_table[victim->pcdata->kingdom].leader_name,victim->name))
	{
	king_table[victim->pcdata->kingdom].leader_name = "None";
        send_to_char("You are too weak to remain a king!\n\r",victim);
        sprintf(buf2,"%s is too weak to remain a King!",victim->pcdata->switchname);
        do_info(ch,buf2);
        log_string(buf2);
        }
 }
  act( "A misty white vapour pours from $N's corpse into your body.",  ch, NULL, victim, 
TO_CHAR , FALSE   );
  act( "A misty white vapour pours from $N's corpse into $n's body.",  ch, NULL, victim, 
TO_NOTVICT , FALSE);
  act( "You double over in agony as raw energy pulses through your veins.",  ch, NULL, 
NULL, TO_CHAR , FALSE   );
  act( "$n doubles over in agony as sparks of energy crackle around $m.",  ch, NULL, NULL, 
TO_NOTVICT, FALSE );
  if (IS_CLASS(victim, CLASS_VAMPIRE))      do_mortalvamp(victim,"");
  else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
  if (IS_SET(victim->pcdata->powers[AURAS], MIGHT_AURA) && IS_CLASS(victim, 
CLASS_UNDEAD_KNIGHT))
    {
      REMOVE_BIT(victim->pcdata->powers[AURAS], MIGHT_AURA);
      send_to_char("Your aura of might fades.\n\r",victim);
  //    victim->damroll -= 1000;
  //    victim->hitroll -= 1000;
    }

  if (IS_CLASS(victim, CLASS_MAGE) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
  do_unpolymorph(victim,"");
  ch->fight_timer += 10;
    if(!IS_NPC(victim))
    victim->pcdata->killedby = ch->pcdata->switchname;
   if (IS_SET(victim->monkstuff, MONK_HEAL)) REMOVE_BIT(victim->monkstuff, MONK_HEAL);

  behead( victim);
  gainarti(ch);
  dropinvis(ch, victim);
  if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD)) REMOVE_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
  if (IS_ITEMAFF(victim, ITEMA_BEAST)) REMOVE_BIT(victim->itemaffect, ITEMA_BEAST);
  if (IS_ITEMAFF(victim, ITEMA_MAGESHIELD)) REMOVE_BIT(victim->itemaffect, ITEMA_MAGESHIELD);
  if (IS_ITEMAFF(victim, ITEMA_ILLUSIONS)) REMOVE_BIT(victim->itemaffect, ITEMA_ILLUSIONS);
  if (IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) REMOVE_BIT(victim->itemaffect, ITEMA_DEFLECTOR);
  if (IS_ITEMAFF(victim, ITEMA_MANASHIELD)) REMOVE_BIT(victim->itemaffect, ITEMA_MANASHIELD);
  
do_beastlike(ch,"");
  if (ch->pcdata->kingdom != 0) king_table[ch->pcdata->kingdom].pk += 1;
  if (victim->pcdata->kingdom != 0) king_table[victim->pcdata->kingdom].pd += 1;

  save_kingdoms();
  ch->hit = ch->max_hit;
  ch->mana = ch->max_mana;
  ch->move = ch->max_move;
  if ((ch->practice >= 20000) && (victim->practice >= 20000))
  {
  value = victim->practice / 20;
  ch->practice += value;
  victim->practice -= value;
  if (victim->practice < 20000) victim->practice = 20000;
  }
  
  ch->pkill = ch->pkill + 1;
  victim->pdeath = victim->pdeath + 1;
  if (!IS_CLASS(victim, CLASS_NINJA)) victim->rage = 0;
  victim->level = 2;


  sprintf(buf,"#y%s#0 has spilt the #Rblood#0 of #y%s#0 with a brutal killing!#n",
ch->pcdata->switchname, victim->pcdata->switchname);
  do_update(NULL,buf);
  sprintf( log_buf, "%s decapitated by %s at %d.",
  victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
  if(!IS_NPC(victim))
  victim->pcdata->killedby = ch->pcdata->switchname;
  log_string( log_buf );
  players_decap++;
  if (ch->pcdata->bounty <= 99999)
  ch->pcdata->bounty += (randMight(victim));
  if (ch->pcdata->bounty > 99999)
  ch->pcdata->bounty = 99999;
  sprintf(buf,"#y%s #0now has a bounty of #P%d #Pquest points#0 on %s head.#n",
ch->pcdata->switchname,ch->pcdata->bounty,ch->sex == SEX_MALE ? "his" : ch->sex == 
SEX_FEMALE ? "her" : "its");
  do_update(NULL,buf);
  return;
}

void do_crack( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA  *obj;
  OBJ_DATA  *right;
  OBJ_DATA  *left;

  right = get_eq_char( ch, WEAR_WIELD );
  left = get_eq_char( ch, WEAR_HOLD );
  if (right != NULL && right->pIndexData->vnum == 12) obj = right;
  else if (left != NULL && left->pIndexData->vnum == 12) obj = left;
  else
  {
    send_to_char("You are not holding any heads.\n\r",ch);
    return;
  }
  act("You hurl $p at the floor.", ch, obj, NULL, TO_CHAR, FALSE);
  act("$n hurls $p at the floor.", ch, obj, NULL, TO_ROOM, FALSE);
  act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_CHAR, 
FALSE);
  if (obj->chobj != NULL)
  {
    act("$p cracks open, leaking brains out across the floor.", ch, obj, obj->chobj, 
TO_NOTVICT, FALSE);
    act("$p crack open, leaking brains out across the floor.", ch, obj, obj->chobj, 
TO_VICT, FALSE);
  }
  else
  {
    act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_ROOM, 
FALSE);
  }
  crack_head(ch,obj,obj->name);
  obj_from_char(obj);
  extract_obj(obj);
}

void crack_head( CHAR_DATA *ch, OBJ_DATA *obj, char *argument )
{
  CHAR_DATA *victim;
  MOB_INDEX_DATA *pMobIndex; 

  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  char buf [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  if ( victim->in_room == NULL ) //Rand
	return;
  if (str_cmp(arg2,"mob") && obj->chobj != NULL && !IS_NPC(obj->chobj) && IS_AFFECTED(obj->chobj,AFF_POLYMORPH))
  {
    victim = obj->chobj;
    make_part(victim,"cracked_head");
    make_part(victim,"brain");
    sprintf(buf,"the quivering brain of %s",victim->name);
    free_string(victim->morph);
    victim->morph = str_dup(buf);
    return;
  }
  else if (!str_cmp(arg2,"mob"))
  {
    if ( ( pMobIndex = get_mob_index( obj->value[1] ) ) == NULL ) return;
    victim = create_mobile( pMobIndex );
    char_to_room(victim,ch->in_room);
    make_part(victim,"cracked_head");
    make_part(victim,"brain");
    extract_char(victim,TRUE);
    return;
  }
  else
  {
    if ( ( pMobIndex = get_mob_index( 30002 ) ) == NULL ) return;
    victim = create_mobile( pMobIndex );
    sprintf( buf, capitalize(arg2) );
    free_string( victim->short_descr );
    victim->short_descr = str_dup( buf );
    char_to_room(victim,ch->in_room);
    make_part(victim,"cracked_head");
    make_part(victim,"brain");
    extract_char(victim,TRUE);
    return;
  }
  return;
}

/* Voodoo skill by KaVir */

void do_voodoo( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  OBJ_DATA  *obj;
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  char buf [MAX_INPUT_LENGTH];
  char part1 [MAX_INPUT_LENGTH];
  char part2 [MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  if ( arg1[0] == '\0')
  {
    send_to_char( "Who do you wish to use voodoo magic on?\n\r", ch );
    return;
  }
  if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
  {
    send_to_char( "You are not holding a voodoo doll.\n\r", ch );
    return;
  }
  if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
  {
    send_to_char( "They are not here.\n\r", ch );
    return;
  }
  if ( IS_NPC(victim) )
  {
    send_to_char( "Not on NPC's.\n\r", ch );
    return;
  }
  sprintf(part2,obj->name);
  sprintf(part1,"%s voodoo doll",victim->name);
  if ( str_cmp(part1,part2) )
  {
    sprintf(buf,"But you are holding %s, not %s!\n\r",obj->short_descr,victim->name);
    send_to_char( buf, ch );
    return;
  }
  if ( arg2[0] == '\0')
    send_to_char( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
  else if ( !str_cmp(arg2, "stab") )
  {
    WAIT_STATE(ch,12);
    act("You stab a pin through $p.", ch, obj, NULL, TO_CHAR, FALSE); 
    act("$n stabs a pin through $p.", ch, obj, NULL, TO_ROOM, FALSE);
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO)) return;
    act("You feel an agonising pain in your chest!", victim, NULL, NULL, TO_CHAR, FALSE);
    act("$n clutches $s chest in agony!", victim, NULL, NULL, TO_ROOM, FALSE);
  }
  else if ( !str_cmp(arg2, "burn") )
  {
    WAIT_STATE(ch,12);
    act("You set fire to $p.", ch, obj, NULL, TO_CHAR, FALSE);
    act("$n sets fire to $p.", ch, obj, NULL, TO_ROOM, FALSE);
    act("$p burns to ashes.", ch, obj, NULL, TO_CHAR, FALSE);
    act("$p burns to ashes.", ch, obj, NULL, TO_ROOM, FALSE);
    obj_from_char(obj);
    extract_obj(obj);
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO)) return;
    if (IS_AFFECTED(victim,AFF_FLAMING) )
      return;
    SET_BIT(victim->affected_by, AFF_FLAMING);
    act("You suddenly burst into flames!", victim, NULL, NULL, TO_CHAR, FALSE);
    act("$n suddenly bursts into flames!", victim, NULL, NULL, TO_ROOM, FALSE);
  }
  else if ( !str_cmp(arg2, "throw") )
  {
    WAIT_STATE(ch,12);
    act("You throw $p to the ground.", ch, obj, NULL, TO_CHAR, FALSE);
    act("$n throws $p to the ground.", ch, obj, NULL, TO_ROOM, FALSE);
    obj_from_char(obj);
    obj_to_room(obj, ch->in_room);
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO)) return;
    if (victim->position < POS_STANDING) return;
    if (victim->position == POS_FIGHTING)
      stop_fighting(victim, TRUE);
    act("A strange force picks you up and hurls you to the ground!", victim, NULL, NULL, 
TO_CHAR, FALSE);
    act("$n is hurled to the ground by a strange force.", victim, NULL, NULL, TO_ROOM, 
FALSE);
    victim->position = POS_RESTING;
    victim->hit = victim->hit - number_range(ch->level,(5*ch->level));
    update_pos(victim);
    if (victim->position == POS_DEAD && !IS_NPC(victim))
    {
      do_killperson(ch,victim->name);
      return;
    }
  }
  else
    send_to_char( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
  return;
}

void do_garotte(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  int chance;

  one_argument(argument, arg);
  chance = number_range(1,100);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE)) 
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if ((victim = get_char_room(ch, arg)) == NULL) 
  {
    send_to_char("Garotte whom?\n\r", ch );
    return;
  }
  if (victim == ch) 
  {
    send_to_char("That would be a bad idea.\n\r", ch );
    return;
  }
  if (is_safe(ch, victim)) return;
  if (((obj = get_eq_char(ch, WEAR_WIELD)) == NULL || obj->value[3] != 4) 
    && ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL|| obj->value[3] != 4))
  {
    send_to_char( "You need to wield a whip.\n\r", ch );
    return;
  }
/*  if ( victim->fighting != NULL )
  {
    send_to_char( "You can't garotte a fighting person.\n\r", ch );
    return;
  } 
  if (victim->hit < victim->max_hit) 
  {
    act("$N is too hurt for you to sneak up.", ch, NULL,victim,TO_CHAR, FALSE);
    return;
  } */
  if (ch->mana < 2500 || ch->move < 2500)
  {
    send_to_char( "You do not have enough magical power or vitality to do this.\n\r",ch);
    return;
  }
  check_killer(ch, victim);
  WAIT_STATE(ch, 16);
    if (IS_NPC(victim))
    {
   ch->move -= 2000;
    ch->mana -= 2000;

    one_hit(ch,victim,gsn_garotte,1);
    one_hit(ch,victim,gsn_garotte,1);
    one_hit(ch,victim,gsn_garotte,1);
    one_hit(ch,victim,gsn_garotte,1);
	if (number_range(1,2) == 1)
    one_hit(ch,victim,gsn_garotte,1);
        if (number_range(1,2) == 1)
    one_hit(ch,victim,gsn_garotte,1);

    if (!IS_AFFECTED(victim, AFF_POISON))
      SET_BIT(victim->affected_by, AFF_POISON);
    if (!IS_AFFECTED(victim, AFF_FLAMING))
      SET_BIT(victim->affected_by, AFF_FLAMING);

    set_fighting(ch,victim); 
    return;
    }
  else if (!IS_NPC(victim))
    {
    ch->move -= 5000;
    ch->mana -= 5000;

    one_hit(ch,victim,gsn_garotte,1);
    one_hit(ch,victim,gsn_garotte,1);
    one_hit(ch,victim,gsn_garotte,1);
    if (number_range(1,2)==1)
    one_hit(ch,victim,gsn_garotte,1);
    if (number_range(1,2)==1)
    one_hit(ch,victim,gsn_garotte,1);

    if (!IS_AFFECTED(victim, AFF_POISON))
      SET_BIT(victim->affected_by, AFF_POISON);
    if (!IS_AFFECTED(victim, AFF_FLAMING))
      SET_BIT(victim->affected_by, AFF_FLAMING);
    set_fighting(ch,victim);
    if (number_range(1,4) ==1 )
    WAIT_STATE(victim, 16);
    return;
    }

  return;
}


bool has_timer(CHAR_DATA *ch) 
{
  if (ch->fight_timer > 0 && !IS_NPC(ch)) 
  {
    send_to_char("Not until your fight timer runs out!\n\r", ch );
    return TRUE;
  }
  return FALSE;
}

void do_circle( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  int i,x;
  one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powers[NPOWER_NINGENNO] < 6) 
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    if (ch->fighting == NULL) 
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
    else victim = ch->fighting;
  }
  if ( victim == ch )
  {
    send_to_char( "How can you circle yourself?\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;
  if ((( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11)
    && ((obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || obj->value[3] != 11))
  {
    send_to_char( "You need to wield a piercing weapon.\n\r", ch );
    return;
  }
  if (ch->move < 5000 || ch->mana < 5000)
  {
    stc("You cannot, you are not capable!\n\r",ch);
    return;
  }
  check_killer( ch, victim );
    if (IS_CLASS(ch, CLASS_NINJA) && !IS_NPC(ch) && ch->pcdata->powers[NPOWER_NINGENNO] >=2)
    {
    WAIT_STATE(ch, 14);
    ch->move -= 5000;
    ch->mana -= 5000;
    x = number_range(3,5);
    for(i = x;i>0;i--)
    one_hit(ch,victim,gsn_circle,1);
    if (!IS_AFFECTED(victim, AFF_POISON))
      SET_BIT(victim->affected_by, AFF_POISON);
    return;
    }
return;
}

void do_autostance(CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];
  
  argument = one_argument(argument,arg);
  if (IS_NPC(ch)) return;
  if (!str_cmp(arg,"none"))
  {
    send_to_char("You no longer autostance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_NONE;
  }
  else if (!str_cmp(arg, "crane"))
  {
    send_to_char("You now autostance into the crane stance.\n\r",ch );
    ch->stance[MONK_AUTODROP] = STANCE_CRANE;
  }
  else if (!str_cmp(arg, "crab")) 
  {
    send_to_char("You now autostance into the crab stance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_CRAB;
  }
  else if (!str_cmp(arg, "bull"))
  {
    send_to_char("You now autostance into the bull stance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_BULL;
  }
  else if (!str_cmp(arg, "viper"))
  {
    send_to_char("You now autostance into the viper stance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_VIPER;
  }
  else if (!str_cmp(arg, "mongoose"))
  {
    send_to_char("You now autostance into the mongoose stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_MONGOOSE;
  }
  else if (!str_cmp(arg, "mantis") && ch->stance[STANCE_CRANE] >= 200 && ch->stance[STANCE_VIPER] >= 200)
  {
    send_to_char("You now autostance into the mantis stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_MANTIS;
  }
  else if (!str_cmp(arg, "monkey") && ch->stance[STANCE_CRANE] >= 200 && ch->stance[STANCE_MONGOOSE] >= 200)
  {
    send_to_char("You now autostance into the monkey stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_MONKEY;
  }
  else if (!str_cmp(arg, "swallow") && ch->stance[STANCE_CRAB] >= 200 && ch->stance[STANCE_MONGOOSE] >= 200)
  {
    send_to_char("You now autostance into the swallow stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SWALLOW;
  }
  else if (!str_cmp(arg, "tiger") && ch->stance[STANCE_BULL] >= 200 && ch->stance[STANCE_VIPER] >= 200)
  {
    send_to_char("You now autostance into the tiger stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_TIGER;
  }
  else if (!str_cmp(arg, "dragon") && ch->stance[STANCE_CRAB] >= 200 && ch->stance[STANCE_BULL] >= 200)
  {
    send_to_char("You now autostance into the dragon stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_DRAGON;
  }
  else if (!str_cmp(arg, "wolf") && (IS_CLASS(ch, CLASS_WEREWOLF)) && ch->stance[STANCE_TIGER] >= 200 && ch->stance[STANCE_SWALLOW] >= 200 && ch->stance[STANCE_MONKEY] >= 200 && ch->stance[STANCE_MANTIS] >= 200 && ch->stance[STANCE_DRAGON] >= 200)
  {
    send_to_char("You now autostance into the wolf stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_WOLF;
  }
  else if (!str_cmp(arg, "ss1") && ch->stance[19] != -1)
  {
    send_to_char("You now autostance into superstance one.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS1;
  }
  else if (!str_cmp(arg, "ss2") && ch->stance[20] != -1)
  {
    send_to_char("You now autostance into superstance two.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS2;
  }
  else if (!str_cmp(arg, "ss3") && ch->stance[21] != -1)
  {
    send_to_char("You now autostance into superstance three.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS3;
  }
  else if (!str_cmp(arg, "ss4") && ch->stance[22] != -1)
  {
    send_to_char("You now autostance into superstance four.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS4;
  }
  else if (!str_cmp(arg, "ss5") && ch->stance[23] != -1)
  {
    send_to_char("You now autostance into superstance five.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_SS5;
  }
  else send_to_char("You can't set your autostance to that!\n\r",ch);
}

void autodrop(CHAR_DATA *ch) 
{
  char buf [MAX_INPUT_LENGTH];
  char buf2 [MAX_INPUT_LENGTH];
  char stancename [10];
  
  if (IS_NPC(ch)) return;
  if (ch->stance[MONK_AUTODROP]==STANCE_NONE) return;
  if (ch->stance[MONK_AUTODROP]==STANCE_VIPER) sprintf(stancename,"viper");
  else if (ch->stance[MONK_AUTODROP]==STANCE_CRANE) sprintf(stancename,"crane");
  else if (ch->stance[MONK_AUTODROP]==STANCE_CRAB) sprintf(stancename,"crab");
  else if (ch->stance[MONK_AUTODROP]==STANCE_MONGOOSE) sprintf(stancename,"mongoose");
  else if (ch->stance[MONK_AUTODROP]==STANCE_BULL) sprintf(stancename,"bull");
  else if (ch->stance[MONK_AUTODROP]==STANCE_MANTIS) sprintf(stancename,"mantis");
  else if (ch->stance[MONK_AUTODROP]==STANCE_DRAGON) sprintf(stancename,"dragon");
  else if (ch->stance[MONK_AUTODROP]==STANCE_TIGER) sprintf(stancename,"tiger");
  else if (ch->stance[MONK_AUTODROP]==STANCE_MONKEY) sprintf(stancename,"monkey");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SWALLOW) sprintf(stancename,"swallow");
  else if (ch->stance[MONK_AUTODROP]==STANCE_XKILLA) sprintf(stancename,"leopard");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS1) sprintf(stancename,"ss1");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS2) sprintf(stancename,"ss2");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS3) sprintf(stancename,"ss3");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS4) sprintf(stancename,"ss4");
  else if (ch->stance[MONK_AUTODROP]==STANCE_SS5) sprintf(stancename,"ss5");
  else if (ch->stance[MONK_AUTODROP]==STANCE_WOLF) sprintf(stancename,"wolf");
  else return;
  if (ch->stance[0] < 1) 
  {
    ch->stance[0] = ch->stance[MONK_AUTODROP];
    sprintf(buf, "#7You autodrop into the #3%s#7 stance.", stancename);
    act(buf, ch, NULL, NULL, TO_CHAR, FALSE);
    sprintf(buf2, "#7%s autodrops into the #3%s#7 stance.",ch->pcdata->switchname,stancename);
    act(buf2, ch, NULL, NULL, TO_ROOM, FALSE);
  }       
}

void dropinvis(CHAR_DATA *ch, CHAR_DATA *victim)
{
  if (ch->level < 7 && xIS_SET(ch->act, PLR_HIDE))
xREMOVE_BIT(ch->act, PLR_HIDE);
  if (ch->level < 7 && xIS_SET(ch->act, PLR_WIZINVIS))
xREMOVE_BIT(ch->act, PLR_WIZINVIS);
  if (ch->level < 7 && IS_AFFECTED(ch, AFF_ETHEREAL))
REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);

  if (ch->level < 7 && IS_SET(ch->newbits, NEW_DARKNESS))
  {
    REMOVE_BIT(ch->newbits, NEW_DARKNESS);
//    REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
  }
  if (victim->level < 7 && xIS_SET(victim->act, PLR_HIDE))
xREMOVE_BIT(victim->act, PLR_HIDE);
  if (victim->level < 7 && xIS_SET(victim->act, PLR_WIZINVIS)) 
xREMOVE_BIT(victim->act, PLR_WIZINVIS);
  if (victim->level < 7 && IS_AFFECTED(victim, AFF_ETHEREAL))
REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);

  if (victim->level < 7 && IS_SET(victim->newbits, NEW_DARKNESS))
  {
    REMOVE_BIT(victim->newbits, NEW_DARKNESS);
  //  REMOVE_BIT(victim->in_room->room_flags, ROOM_TOTAL_DARKNESS);
  }

  if (IS_NPC(ch))
  {
  affect_strip ( ch, gsn_invis );
  affect_strip ( ch, gsn_mass_invis );
  }
  if (IS_NPC(victim))
  {
  affect_strip ( victim, gsn_mass_invis );
  affect_strip ( victim, gsn_invis );
  }
  return;
}

bool is_fighting_player(CHAR_DATA *victim, CHAR_DATA *attacker)
{
DESCRIPTOR_DATA *d;
CHAR_DATA *och;

  if (IS_NPC(victim))
     return FALSE;

  if (IS_NPC(attacker))
     return FALSE;
  if (IS_SET(victim->in_room->room_flags, ROOM_ARENA) && IS_SET(attacker->in_room->room_flags, ROOM_ARENA))
     return FALSE;
  for (d = first_descriptor; d; d = d->next)
  {
     if (d->character)
        och = d->character;
     else
        continue;

     if (!och->fighting)
        continue;

     if (och == attacker)
         continue;

     if (och->fighting == victim)
              return TRUE;
  }

  if ((victim)->fighting)
     if (!IS_NPC(victim->fighting) && victim->fighting != attacker)
        return TRUE;

  return FALSE;

}
