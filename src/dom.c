/*
 *
 *  The following code is copyrighted to Dominic Jones.
 *  3/01/02.
 *
 */


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
#include "merc.h"
#include "garou.h"
#include "olc.h"
ALIAS_DATA * first_alias;
ALIAS_DATA * last_alias;

void do_exptoken( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int value;
    double val;
    double minimum;
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( IS_NPC(ch) || IS_SET(ch->special, SPC_NEWBIE) ||(ch->exp < 1 && 
!IS_JUDGE(ch)) )
    {
    send_to_char("You are unable to make an exp token.\n\r",ch);
    return;
    }

    else if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
    send_to_char("Please specify a value for the exp token.\n\r",ch);
    return;
    }
    else
    {
        value = atoi( arg1 );
    if ( value < 1 || value > 200000000 )
        {
        send_to_char( "An exp token should have a value between 1 and 200000000 exp.\n\r", ch );
        return;
        }
	minimum = value * 2;
    if ( minimum > ch->exp )
        {
        sprintf(buf,"You need %2.0f experience to make an experience token of that value.\n\r",minimum);
        send_to_char( buf, ch );
        return;
        }
    }

    victim = get_char_room( ch, arg2 );

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
      send_to_char( "Error...missing object, please inform IMPS.\n\r",ch );
      return;
    }
	if (value > 0)
	{
	  val = value * 2;
	}

    ch->exp -= val;
    if (ch->exp < 0) ch->exp = 0;
    obj = create_object( pObjIndex, value );
    obj->value[0] = value;
    obj->level = 1;
    obj->cost = value;
    obj->item_type = ITEM_EXP_TOKEN;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
    obj->name = str_dup( "experience point token" );
    sprintf(buf,"a %d point experience token",value);
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf(buf,"A %d point experience token lies on the floor.",value);
    free_string( obj->description );
    obj->description = str_dup( buf );
    if (victim != NULL && victim != ch)
    {
        act( "You reach behind $N's ear and produce $p.", ch, obj, victim, TO_CHAR, FALSE );
        act( "$n reaches behind $N's ear and produces $p.", ch, obj, victim, TO_NOTVICT,FALSE );
        act( "$n reaches behind your ear and produces $p.", ch, obj, victim, TO_VICT,FALSE );
    }
    else
    {
        act( "You snap your fingers and reveal $p.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n snaps $s fingers and reveals $p.", ch, obj, NULL, TO_ROOM, FALSE );
    }
    return;
}
const char * exitname3 [6] =
{
        "to the north",
        "to the east",
        "to the south",
        "to the west",
        "above you",
        "below you"
};

bool check_blind             args( ( CHAR_DATA *ch ) );
void do_buy( CHAR_DATA *ch, char *argument )
{
    char arg1[MIL];
    char buf[MSL];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *tat;
    OBJ_DATA *obj;
    int qpcost;
    int vnum;
    one_argument( argument, arg1);
    if (IS_NPC(ch)) return;

    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
    {
       if ( obj->item_type == ITEM_MACHINE )
       break;
    }

    if ( obj == NULL )
    {
       send_to_char( "There is no vending machine to buy from!\n\r", ch );
       return;
    }
    if ( arg1[0] == '\0' )
    {
        stc("#7You may purchase the following:#C  2x_gold, 2x_exp, lesser_damcap, greater_damcap#n\n\r",ch);
        stc("#0The prices are as follows#7: 2x_gold #0(#C20k QP#0)#7, 2x_exp #0(#C5k QP#0)#n\n\r",ch);
        stc("#0The prices are as follows#7: lesser_damcap #0(#C5k QP#0)#7, greater_damcap #0(#C10k QP#0)#n\n\r",ch);
	return;
    }

	if (!str_cmp(arg1, "2x_exp"))
	{
	  vnum = 420006;
	  qpcost = 5000;
	}
	else if (!str_cmp(arg1, "lesser_damcap"))
	{
	  vnum = 420007;
	  qpcost = 5000;
	}
	else if (!str_cmp(arg1, "greater_damcap"))
	{
	  vnum = 420008;
	  qpcost = 10000;
	}
        else if (!str_cmp(arg1, "2x_gold"))
        {
          vnum = 420009;
          qpcost = 20000;
        }

	else
	{
	  sprintf(buf, "The vending machine is not stocked with '%s'\n\r",arg1);
	  stc(buf,ch);
          stc("#7You may purchase the following:#C  2x_gold, 2x_exp, lesser_damcap, greater_damcap#n\n\r",ch);
          stc("#0The prices are as follows#7: 2x_gold #0(#C20k QP#0)#7, 2x_exp #0(#C5k QP#0)#n\n\r",ch);
	  stc("#0The prices are as follows#7: lesser_damcap #0(#C5k QP#0)#7, greater_damcap #0(#C10k QP#0)#n\n\r",ch);
          return;
	}
	if ( ( pObjIndex = get_obj_index(  vnum  ) ) == NULL )
	{
   		 send_to_char( "No object has that vnum, inform Rand.\n\r", ch );
		 return;
	}

	if (ch->pcdata->quest < qpcost)
	{
	  sprintf(buf,"In order to purchase %s, you need a minimum of %d QP.\n\r",arg1,qpcost);
	  stc(buf,ch);
	  return;
	}
        tat = create_object( pObjIndex, 60 );
	if (tat->questowner != NULL) free_string(tat->questowner);
        tat->questowner = str_dup(ch->pcdata->switchname);
	obj_to_char( tat, ch );
	act( "$n has bought $p from vending machine!", ch, tat, NULL, TO_ROOM, FALSE );
	sprintf(buf, "%s has bought %s from the vending machine.",ch->pcdata->switchname, tat->short_descr);
	log_string(buf);
	sprintf(buf, "You buy %s from the vending machine, it will last for %d ticks.\n\r",tat->short_descr, tat->timer);
	act( buf, ch, NULL, NULL, TO_CHAR, FALSE );
	ch->pcdata->quest -= qpcost;
	return;
}
void rand_oset_affect( CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest )
{
    char buf [MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int quest;
    int range;
    int cost;
    int max;
    if (IS_NPC(ch))
    {
        send_to_char("Switch back, smart ass.\n\r", ch);
        return;
    }
    if (value == 0)
    {
        send_to_char("Please enter a positive or negative amount.\n\r", ch);
        return;
    }
    if (!IS_JUDGE(ch) && (obj->questowner == NULL))
    {
        send_to_char("First you must set the owners name on the object.\n\r", ch);
        return;
    }
    if (!IS_JUDGE(ch) && (obj->questmaker == NULL ||
        str_cmp(ch->name,obj->questmaker)) && !is_quest )
    {
        send_to_char("That item has already been oset by someone else.\n\r", ch);
        return;
    }
    if      ( affect == APPLY_STR     ) {range = 3;  cost = 200 ; quest = QUEST_STR;}
    else if ( affect == APPLY_DEX     ) {range = 3;  cost = 200 ; quest = QUEST_DEX;}
    else if ( affect == APPLY_INT     ) {range = 3;  cost = 200 ; quest = QUEST_INT;}
    else if ( affect == APPLY_WIS     ) {range = 3;  cost = 200 ; quest = QUEST_WIS;}
    else if ( affect == APPLY_CON     ) {range = 3;  cost = 200 ; quest = QUEST_CON;}
    else if ( affect == APPLY_HIT     ) {range = 500; cost = 500 ; quest = QUEST_HIT;}
    else if ( affect == APPLY_MANA    ) {range = 500; cost = 500 ; quest = QUEST_MANA;}
    else if ( affect == APPLY_MOVE    ) {range = 500; cost = 500 ; quest = QUEST_MOVE;}
    else if ( affect == APPLY_HITROLL ) {range = 50;  cost = 200 ; quest = QUEST_HITROLL;}
    else if ( affect == APPLY_DAMROLL ) {range = 50;  cost = 200 ; quest = QUEST_DAMROLL;}
    else if ( affect == APPLY_AC      ) {range = 250; cost = 100 ; quest = QUEST_AC;}
    else return;
    if (IS_SET(obj->quest, QUEST_IMPROVED))
        max = 10000 + (ch->race * 1000);
    else if ( obj->pIndexData->vnum == OBJ_VNUM_PROTOPLASM )
        {range *= 2; max = 10000 + (ch->race * 1000);}
    else
        max = 10000 + (ch->race * 1000);
    if (obj->item_type == ITEM_WEAPON) {max *= 2;range *= 2;}
    if ( !IS_JUDGE(ch) && ((value > 0 && value > range) ||
        (value < 0 && value < (range-range-range))) )
    {
        send_to_char("That is not within the acceptable range...\n\r", ch);
        send_to_char("Str, Dex, Int, Wis, Con... max =    3 each, at 200 quest points per +1 stat.\n\r", ch);
        send_to_char("Hp, Mana, Move............ max =  500 each, at 500 quest point per point.\n\r", ch);
        send_to_char("Hitroll, Damroll.......... max =   50 each, at 200 quest points per point.\n\r", ch);
        send_to_char("Ac........................ max = -250,      at 100 quest points per point.\n\r", ch);
        send_to_char("\n\rNote: Created items can have upto 2 times the above maximum.\n\r", ch);
        send_to_char("Also: Weapons may have upto 2 (4 for created) times the above maximum.\n\r", ch);
        return;
    }



    if ( quest >= QUEST_HIT && value < 0 ) cost *= (value - (value*2));
    else cost *= value;
    if ( cost < 0 ) cost = 0;
    if (!IS_JUDGE(ch) && IS_SET(obj->quest, quest) )
    {
        send_to_char("That affect has already been set on this object.\n\r", ch);
        return;
    }
    if (!IS_JUDGE(ch) && (obj->points + cost > max))
    {
        sprintf(buf,"You are limited to %d quest points per item.\n\r",max);
        send_to_char(buf, ch);
        sprintf(buf,"Base = 10000 qp.  Modifier = 1000 per status.\n\r");
        send_to_char(buf, ch);
        return;
    }
    if (is_quest && ch->pcdata->quest < cost && !IS_JUDGE(ch))
    {
        sprintf(buf,"That costs %d quest points, while you only have %d.\n\r",cost, ch->pcdata->quest);
        send_to_char(buf, ch);
        return;
    }
    if (!IS_SET(obj->quest, quest) ) SET_BIT(obj->quest,quest);
    if (is_quest) ch->pcdata->quest -= cost;
//obj->points += cost;
    if (obj->questmaker != NULL) free_string(obj->questmaker);
      obj->questmaker = str_dup(ch->name);

    CREATE( paf, AFFECT_DATA, 1 );
 
    paf->type   = 0;
    paf->duration       = -1;
    paf->location       = affect;
    paf->modifier       = value;
    paf->bitvector      = 0;
    LINK( paf, obj->first_affect, obj->last_affect, next, prev );
return;

}

char *get_exits(CHAR_DATA *ch)
{
  extern char *const dir_name[];
  static char buf[MAX_STRING_LENGTH];
  EXIT_DATA *pexit;
  bool found;
  int door;

  buf[0] = '\0';

  if (!check_blind(ch)) return "";

  sprintf(buf, "#0[#GExits#w:#C");

  found = FALSE;
  for (door = 0; door <= 5; door++)
  {
    if ((pexit = ch->in_room->exit[door]) != NULL && pexit->to_room != NULL)
    {
      found = TRUE;
      if (IS_SET(pexit->exit_info, EX_CLOSED))
      {
        strcat(buf, " #0(#C");
        strcat(buf, dir_name[door]);
        strcat(buf, "#0)#C");
      }
      else
      {
        strcat(buf, " ");
        strcat(buf, dir_name[door]);
      }
    }
  }
  if (!found)
    strcat(buf, " none");
  strcat(buf, "#0]#n\n\r");

  return buf;
}

void randhint( CHAR_DATA *ch)
{
 int randhint;
   if (IS_NPC(ch)) return;
   if (xIS_SET(ch->deaf, CHANNEL_HINT)) return;

   randhint = number_range(0, maxHint-1);

   chprintf(ch, "\n\r#0[#7HINT#0] %s#n\n\r", hint_table[randhint].text);
/*   if (randhint == 1)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7Wonder what #ygold#7 is used for? Type: #yhelp gold#7!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 2)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7How long are #Rnewbie hours#7? Type: #Rhelp pkready#7!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 3)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7How can you get free #Cquest-points#7? Type: #Chelp toss#7!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 4)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7How do you make #0equipment#7? Type: #0help equipment#7!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 5)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7What is a #Cwishing well#7 and how do I use it? Type: #Chelp wish#7!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 6)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #yHow do I see less spam#R? #yType: #Rconfig#y#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 7)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #GInvite your friends to play!! #yhelp reward#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 8)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #yWhere are the #RAREAS#y? #RType: #yhelp areas#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 9)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7What is a Zealot? What is a Legend? #GType: #7help legend#G!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 10)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7What are (#0RARE#7) equipment and what do they do? Type: #0help rare#7!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 11)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7But I really want an #0ARTIFACT#7!  How do I get one?? #RType: #7help artifact#R!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 12)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7So how do I raise my #RDAMCAP#7?!? Type: #Rhelp damcap#7!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint == 13)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #GI've found the catacombs, but when I #ylook#G at the statues, nothing happens#y! #GType: #yhelp area#G!#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint ==14)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7How do I eat a TOKEN??#R You must '#7consume#R' tokens.#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint ==15)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7How do I get to the Pit of Sacrifice? #0Go find the Dragon Highlord.#n\n\r");
     stc(hint,ch);
     return;
   }
   else if (randhint ==16)
   {
     sprintf(hint,"\n\r#0[#7HINT#0] #7What kind of equipment can I make?  #yType: #GHelp Equipment#n\n\r");
     stc(hint,ch);
     return;
   }*/

  return;
}

void do_scatter( CHAR_DATA *ch, char *argument )
{
//    char buf [MSL];
    char arg [ MIL ];
    OBJ_DATA *obj;
    OBJ_DATA *work;
    CHAR_DATA *victim;
    int number = 0;
    bool found;

    argument = one_argument( argument, arg );
   
    if( arg[0] == '\0' )
    {
	send_to_char("Syntax: scatter <item>\n\r", ch );
	send_to_char("   Will loop until you have no more in inventory.\n\r", ch );
	return;
    }
  
    found = FALSE;
    work = get_obj_carry(ch,arg);
    for( obj = get_obj_carry(ch,arg); obj; obj = get_obj_carry(ch,arg) )
    {
	if( !obj )
	{
		send_to_char("There are no more objects in your inventory.\n\r",ch);
		return;
	}
	for ( victim = char_list; victim; victim = victim->next )
        {
		if (!IS_NPC(victim)) continue;
		if( number_range(1,1000) == 1 )
		{
			found = TRUE;
			obj_from_char(obj);
			obj_to_char( obj, victim);
		}
	}
	number++;
    }
    if( found )
	{
//	   sprintf(buf, "#0%s#7 has scattered #R%d %s #7throughout the world!#n",ch->pcdata->switchname,number,obj->short_descr);
//	   do_info(NULL,buf);

	send_to_char( "Done\n\r",ch);
	}
    if (!found)
	stc("You do not have that.\n\r",ch);
    return;
}
void do_showtele( CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    bool found;

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if (obj->item_type != ITEM_TELEPORTER) continue;
	if (obj->in_room->vnum < 41000 || obj->in_room->vnum > 49999) continue;
        if (obj->in_room->vnum >= 41108 && obj->in_room->vnum <= 41129) continue;

        else if ( obj->carried_by == NULL && can_see_obj(ch, obj))
        {
            sprintf( buf, "%s in %s.\n\r",
                obj->short_descr, obj->in_room->name );
            found = TRUE;
        }
        else
        {
          found = FALSE;
        }
        if (found)
        {
          buf[0] = UPPER(buf[0]);
          send_to_char( buf, ch );
        }
    }

    if ( !found )
        send_to_char( "You cannot seem to find any teleporters.\n\r", ch );

    return;
}


void do_policy( CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char strsave[MAX_INPUT_LENGTH];
    char buf[MSL];

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg1 );
    if (IS_NPC(ch)) return;
    if (xIS_SET(ch->extra, EXTRA_POLICY))
    {
     stc("You have already accepted the Policy.\n\r",ch);
     return;
    }

    if ( arg[0] == '\0' || arg1[0] == '\0')
    {
     send_to_char("syntax:  policy <accept/decline> <your password>\n\r",ch);
     return;
    }

    if (!str_cmp(arg, "accept"))
    {
    if ( strcmp( arg1, ch->pcdata->pwd )
&&         strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ))
      {
        WAIT_STATE( ch, 40 );
        send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
        return;
      }
      else
      {
        xSET_BIT(ch->extra, EXTRA_POLICY);
        stc("OK! You are capable of gaining EXP!\n\r",ch);
	sprintf(buf, "%s has accepted the policy!",ch->pcdata->switchname);
	log_string(buf);
	return;
      }
    }
    if (!str_cmp(arg, "decline"))
    {
      if ( strcmp( arg1, ch->pcdata->pwd )
        && strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ))
      {
        WAIT_STATE( ch, 40 );
        send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
        return;
      }
      else
      {
        sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
        sprintf(buf, "%s has declined the policy and been deleted.", ch->name );
        log_string(buf);
	ch->fight_timer = 0;
        ch->quit_timer = 0;
	do_save(ch,"");
        stop_fighting(ch, TRUE);
        char_from_room( ch );
	char_to_room(ch, get_room_index(ROOM_VNUM_LIMBO));
        do_quit( ch, "" );
        unlink(strsave);
        return;
      }
   }
   else if (str_cmp(arg,"decline") || str_cmp(arg,"accept"))
    {
     send_to_char("syntax:  policy <accept/decline> <your password>\n\r",ch);
     return;
    }

}
void cleartimer( CHAR_DATA *ch)
{
   if (IS_NPC(ch)) return;
   ch->fight_timer = 0;
   ch->wish_timer = 0;
   ch->quit_timer = 0;
}
void resetchar( CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  int hp;
  int mana;
  int move;

  if (IS_NPC(ch)) return;
  if (!xIS_SET(ch->extra, EXTRA_FIXRESET))
    {
	
        while (ch->last_affect)
        affect_remove(ch,ch->last_affect);
	hp = 100000;
	mana = 100000;
	move = 100000;
        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
                obj_next = obj->next_content;
		extract_obj(obj);
	}
	if (ch->max_hit > hp) ch->max_hit = hp;
	if (ch->max_mana > mana) ch->max_mana = mana;
	if (ch->max_move > move) ch->max_move = move;
	ch->gold = 0;
	ch->pcdata->quest = 0;
	ch->pcdata->bank = 0;
	ch->lstatus = 0;
	ch->lexp = 0;
	ch->race = 0;
	ch->pkill = 0;
	ch->pdeath = 0;
	ch->explevel = 0;
	ch->pcdata->questsrun = 0;
	ch->pcdata->questtotal = 0;
	ch->pcdata->score[SCORE_QUEST] = 0;
	ch->generation = 6;
	ch->pcdata->wolf = 1;
	ch->pcdata->bounty = 0;
	ch->practice = 0;
	ch->exp = 0;
	char_from_room(ch);
	char_to_room(ch, get_room_index(ROOM_VNUM_NECROPOLIS));
	ch->hit = ch->max_hit;
	ch->mana = ch->max_mana;
	ch->move = ch->max_move;
	ch->stance[0] = 0;
	ch->stance[1] = 0;
	ch->stance[2] = 0;
	ch->stance[3] = 0;
	ch->stance[4] = 0;
	ch->stance[5] = 0;
	ch->stance[6] = 0;
	ch->stance[7] = 0;
	ch->stance[8] = 0;
	ch->stance[9] = 0;
	ch->stance[10] = 0;
	ch->stance[11] = 0;
	ch->stance[12] = 0;
	ch->stance[13] = 0;
	ch->stance[14] = 0;
	ch->stance[15] = 0;
	ch->stance[16] = 0;
	ch->stance[17] = 0;
	ch->stance[18] = 0;
	ch->stance[19] = -1;
	ch->stance[20] = -1;
	ch->stance[21] = -1;
	ch->stance[22] = -1;
	ch->stance[23] = -1;
	ch->spl[0] = 0;
	ch->spl[1] = 0;
	ch->spl[2] = 0;
	ch->spl[3] = 0;
	ch->spl[4] = 0;
	ch->wpn[0] = 0;
        ch->wpn[1] = 0;
        ch->wpn[2] = 0;
        ch->wpn[3] = 0;
        ch->wpn[4] = 0;
        ch->wpn[5] = 0;
        ch->wpn[6] = 0;
        ch->wpn[7] = 0;
        ch->wpn[8] = 0;
        ch->wpn[9] = 0;
        ch->wpn[10] = 0;
        ch->wpn[11] = 0;
        ch->wpn[12] = 0;
	ch->played = 0;
	if (IS_SET(ch->newbits, NEW_SUPREME)) REMOVE_BIT(ch->newbits, NEW_SUPREME);
	if (xIS_SET(ch->extra, EXTRA_CHAT)) xREMOVE_BIT(ch->extra, EXTRA_CHAT);
	if (xIS_SET(ch->extra, EXTRA_JUST_PK)) xREMOVE_BIT(ch->extra, EXTRA_JUST_PK);
	if (!IS_SET(ch->special, SPC_NEWBIE)) SET_BIT(ch->special, SPC_NEWBIE);
	if (xIS_SET(ch->extra, EXTRA_FIXADA)) xREMOVE_BIT(ch->extra, EXTRA_FIXADA);
	if (xIS_SET(ch->extra, EXTRA_FIXSTATS)) xREMOVE_BIT(ch->extra, EXTRA_FIXSTATS);
	if (xIS_SET(ch->extra, EXTRA_FIXSTATUS)) xREMOVE_BIT(ch->extra, EXTRA_FIXSTATUS);
	if (IS_SET(ch->special, SPC_MASTERY2)) REMOVE_BIT(ch->special, SPC_MASTERY2);
	if (IS_SET(ch->newbits, NEW_MASTERY)) REMOVE_BIT(ch->newbits, NEW_MASTERY);
	xSET_BIT(ch->extra, EXTRA_FIXRESET);
	do_newbiepack(ch,"");
	do_save(ch,"");
    }
  return;
}
void do_groups( CHAR_DATA *ch, char *argument)
{
	char buf[MSL];
	CHAR_DATA *gch;
	CHAR_DATA *leader;
	CHAR_DATA *gch_next;

        leader = (ch->leader != NULL) ? ch->leader : ch;
                sprintf(buf, "%s\n\r", leader->pcdata->switchname);
                stc(buf,ch);

        for ( gch = char_list; gch != NULL; gch = gch->next )
        {
	    gch_next = gch->next;
            if (gch == NULL) continue;
	    if (IS_NPC(gch)) continue;
	    if (gch->leader == NULL) continue;
                leader = (gch->leader != NULL) ? gch->leader : gch;
//                sprintf( buf, "%s's group:\n\r", PERS(leader, gch) );
//                send_to_char( buf, ch );
	    if (gch->leader != NULL)
	    {
//                sprintf(buf, "%s\n\r", leader->pcdata->switchname);
//                stc(buf,ch);
		if (is_same_group(leader,gch))
		{
		sprintf(buf, "%s\n\r", gch->pcdata->switchname);
		stc(buf,ch);
		}
	    }
	}
  return;
}
void do_srecall( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *location;

    if (IS_NPC(ch)) return;
    if (!IS_SET(ch->newbits, NEW_SUPREME))
    {
	stc("You are not a supreme, how can you use the supreme recall?\n\r",ch);
	return;
    }
  
    act( "$n's body flickers with supreme energy.", ch, NULL, NULL, TO_ROOM, FALSE );
    act( "Your body flickers with supreme energy.", ch, NULL, NULL, TO_CHAR, FALSE );

    if ( ch->position == POS_FIGHTING)
    {
     send_to_char( "You can't go to the supreme recall while fighting!\n\r",ch);
     return;
    }


    if ( ( location = get_room_index( ROOM_VNUM_SRECALL ) ) == NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }
    if ( ch->in_room == location )
	return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) )
    {
	send_to_char( "You cannot go to the supreme recall while cursed.\n\r", ch );
	return;
    }
	    

    act( "$n looks quite supreme and then disappears.", ch, NULL, NULL, TO_ROOM, FALSE );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM , FALSE);
    do_look( ch, "auto" );
    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    return;
}
void do_pkwhere( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    char      buf [MAX_STRING_LENGTH];
        if (IS_NPC(ch)) return;

        if (ch->race < 30)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }
        send_to_char("You overlook the world.\n\r", ch);
        for (d = first_descriptor; d != NULL; d = d->next)
    {
        if (d->connected != CON_PLAYING) continue;
        if ((victim = d->character) == NULL) continue;
        if (IS_NPC(victim)) continue;
        if (victim->in_room == NULL) continue;
        if (victim == ch) continue;
	if (IS_IMMORTAL(victim)) continue;
        if (!can_see(ch,victim)) continue;
        sprintf(buf,"%-14s - %s#n\n\r", victim->name,victim->in_room->name);
        send_to_char(buf,ch);
   }
}
void do_fixstats( CHAR_DATA *ch, char *argument)
{
  char buf[MSL];
  int value;
  int valuem;
  if (IS_NPC(ch)) return;
  if (!IS_NPC(ch))
  {
    if (xIS_SET(ch->extra, EXTRA_FIXSTATS)) return;

    if (!xIS_SET(ch->extra, EXTRA_FIXSTATS) && IS_SET(ch->newbits, NEW_SUPREME))
        {
	     value = 300000 + (ch->race * 5000) + (ch->lstatus * 20000);
	     valuem = 450000 + (ch->race * 5000) + (ch->lstatus * 20000);
	     if (ch->max_hit > value) ch->max_hit = value;
             if (ch->max_mana > valuem) ch->max_mana = valuem;
             if (ch->max_move > value) ch->max_move = value;
             if (ch->hit > value) ch->hit = value;
             if (ch->mana > valuem) ch->mana = valuem;
             if (ch->move > value) ch->move = value;

             sprintf(buf,"%s has had hp balanced.",ch->pcdata->switchname);

        }
    xSET_BIT(ch->extra, EXTRA_FIXSTATS);
    return;
   }
}

void fixstatus( CHAR_DATA *ch)
{
  char buf[MSL];
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  int legend;
  int weapon;
  int max_skl;
  int mana;
  if (IS_NPC(ch)) return;
  if (!IS_NPC(ch))
  {
    if (xIS_SET(ch->extra, EXTRA_FIXSTATUS)) return;

    if (!xIS_SET(ch->extra, EXTRA_FIXSTATUS))
	{
	  if (ch->race > 0)
	   {
	     ch->race = 0;
	     sprintf(buf,"%s is now fixed.",ch->pcdata->switchname);
	     log_string(buf);
	     sprintf(buf,"Your status and pk score has been reset to zero.\n\r");
	     stc(buf,ch);
	   }
           ch->pkill = 0;
           ch->pdeath = 0;
	   if (ch->generation != 6)
	   {
	     ch->generation = 6;
	     sprintf(buf,"%s has had %s generation reset.",ch->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
	     log_string(buf);
	     stc("Your generation has been reset to 6.\n\r",ch);
             if (IS_CLASS(ch, CLASS_DROW) && (IS_SET(ch->special, SPC_DROW_WAR)))
                max_skl = 550;
             if (IS_CLASS(ch, CLASS_DROW) && (!IS_SET(ch->special, SPC_DROW_WAR)))
                max_skl = 200;
             if (ch->class == CLASS_WEREWOLF)
                max_skl = 600;
             if (ch->class == CLASS_MONK)
                max_skl = 750;
             if (IS_CLASS(ch, CLASS_DEMON))
                max_skl = 600;
             if (IS_CLASS(ch, CLASS_ANGEL))
                max_skl = 650;
             if (IS_CLASS(ch, CLASS_POWER_LICH))
                max_skl = 500;
             if (IS_CLASS(ch, CLASS_MAGE))
                max_skl = 600;
             if (IS_CLASS(ch, CLASS_VAMPIRE))
                max_skl = 550;
             if (IS_CLASS(ch, CLASS_NINJA))
                max_skl = 750;
             if (IS_CLASS(ch, CLASS_CYBORG) && (ch->pcdata->powers[CYBORG_LIMBS] > 3))
                max_skl = 600;
             if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                max_skl = UMAX(300, ch->pcdata->powers[WEAPONSKILL] * 100);
             if (IS_CLASS(ch, CLASS_HIGHLANDER))
                max_skl = 1200;
             if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) 
		max_skl = 600;
             if (IS_CLASS(ch, CLASS_TANARRI)) 
		max_skl = 700;
             if (IS_CLASS(ch, CLASS_PALADIN)) 
		max_skl = 700;
             if (IS_CLASS(ch, CLASS_PSIONIST)) 
		max_skl = 700;
             if (IS_CLASS(ch, CLASS_INQUISITOR)) 
		max_skl = 700;
	     for (weapon = 0; weapon < 13; weapon++)
	     {
		if (ch->wpn[weapon] > max_skl)
		ch->wpn[weapon] = max_skl;
	     }
	     stc("Your weapons have been reset to generation 6 levels.\n\r",ch);
	     sprintf(buf, "%s has had %s weapons reset to generation 6 levels.",ch->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
	     log_string(buf);
	   }
           if (ch->pcdata->souls > 0)
           {
             ch->pcdata->souls = 0;
             sprintf(buf, "%s has had %s souls reset.",ch->pcdata->switchname,ch->sex ==SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
             log_string(buf);
             stc("Your souls have been reset to zero.\n\r",ch);
           }	   
	  if (ch->lstatus >= 0)
	  {
	     if (ch->lstatus == 0)  legend = 0;
	     else if (ch->lstatus == 1)  legend = 50000;
             else if (ch->lstatus == 2)  legend = 150000;
             else if (ch->lstatus == 3)  legend = 300000;
             else if (ch->lstatus == 4)  legend = 500000;
             else if (ch->lstatus == 5)  legend = 750000;
             else if (ch->lstatus == 6)  legend = 1050000;
             else if (ch->lstatus == 7)  legend = 1400000;
             else if (ch->lstatus == 8)  legend = 1800000;
             else if (ch->lstatus == 9)  legend = 2250000;
	     sprintf(buf, "Your legend has been reset to zero, and your %d QP spent reimbursed.\n\r",legend);
             stc(buf,ch);
	     sprintf(buf, "%s has had %s legend reset to zero, and %d QP reimbursed.",ch->pcdata->switchname, ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its", legend);
	     log_string(buf);
	     ch->pcdata->quest += legend;
             ch->lstatus = 0;
	  }
	  if (ch->pcdata->bounty > 0) ch->pcdata->bounty = 0;
          for ( obj = ch->carrying; obj != NULL; obj = obj_next )
          {
                obj_next = obj->next_content;

                if (obj->pIndexData->vnum >= 814 && obj->pIndexData->vnum <= 825)
                {
                extract_obj(obj);
                send_to_char("Your 15k QP from the Adamantium Eq are refunded.\n\r",ch);
                ch->pcdata->quest += 15000;
		ch->explevel = 0;
                   if (obj->points > 0)
                   {
                    sprintf( buf, "You receive a refund of %d quest points from %s.", obj->points,obj->short_descr);
                    act( buf, ch, obj, NULL, TO_CHAR, FALSE );
                    ch->pcdata->quest += obj->points;
                   }
                }
	        if (IS_SET(obj->quest, QUEST_RUNE)) 
		{
		   extract_obj(obj);
		}
                  if (IS_SET(obj->quest, QUEST_ARTIFACT))
                {
                   extract_obj(obj);
                }

        }
          stc("Your rares have been purged.\n\r",ch);
          sprintf(buf, "%s has had %s rares purged.",ch->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
	  log_string(buf);
          stc("Your artifact has been purged.\n\r",ch);
          sprintf(buf, "%s has had %s artifact purged.",ch->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" :"its");
	  log_string(buf);
	  sprintf(buf, "%s has had %s adamantium eq cleaned.",ch->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
          log_string(buf);
	  do_remove(ch,"all");
	  if (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_POWER_LICH)) mana = 450000;
	  else mana = 300000;
	  if (ch->max_hit > 300000)
	  {
	    ch->max_hit = 300000;
	    ch->hit = 300000;
	    sprintf(buf, "%s has had %s HP reset.",ch->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
	    log_string(buf);
	  }
          if (ch->max_mana > mana)
          {
            ch->max_mana = mana;
            ch->mana = mana;
            sprintf(buf, "%s has had %s MANA reset.",ch->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
            log_string(buf);
          }
          if (ch->max_move > 300000)
          {
            ch->max_move = 300000;
            ch->move = 300000;
            sprintf(buf, "%s has had %s MOVE reset.",ch->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
            log_string(buf);
          }
	}
    xSET_BIT(ch->extra, EXTRA_FIXSTATUS);
    return;
   }
 return;
}
void fixadam( CHAR_DATA *ch)
{
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  char buf[MSL];
  if (IS_NPC(ch)) return;
  if (!IS_NPC(ch))
  {
	if (xIS_SET(ch->extra, EXTRA_FIXADA))
	{
		return;
	}
        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
                obj_next = obj->next_content;
		
		if (str_cmp(obj->questowner,ch->pcdata->switchname)) continue;
		if (obj->pIndexData->vnum >= 814 && obj->pIndexData->vnum <= 825)
		{
                extract_obj(obj);
                send_to_char("Your 15k QP from the Adamantium Eq are refunded.\n\r",ch);
                ch->pcdata->quest += 15000;
		   if (obj->points > 0)
        	   {
         	    sprintf( buf, "You receive a refund of %d quest points from %s.", obj->points,obj->short_descr);
            	    act( buf, ch, obj, NULL, TO_CHAR, FALSE );
        	    ch->pcdata->quest += obj->points;
	           }

		}
	}
  }

  xSET_BIT(ch->extra, EXTRA_FIXADA);
  sprintf(buf, "%s has had their adamantium eq cleaned.",ch->name);
  log_string(buf);
  do_clearstats2(ch,"");
  return;
}

void do_fixfury( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (IS_SET(ch->special, SPC_FURY))
	REMOVE_BIT(ch->special, SPC_FURY);
}
void do_artifact( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    bool found;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
        if ( !IS_SET(obj->quest, QUEST_ARTIFACT) ) continue;

        if (obj->in_obj != NULL && can_see_obj(ch, obj))
        {
            sprintf( buf, "%s carried inside %s.\n\r",
                obj->short_descr, obj->in_obj->short_descr );
            found = TRUE;
        }

        else if ( obj->carried_by != NULL && can_see(ch,obj->carried_by))
        {
          if ( obj->wear_loc >= 0 )
            sprintf( buf, "%s equipped by %s.\n\r",
                obj->short_descr, PERS(obj->carried_by, ch) );
          else
            sprintf( buf, "%s carried by %s.\n\r",
                obj->short_descr, PERS(obj->carried_by, ch) );
            found = TRUE;
        }

        else
        {
          found = FALSE;
        }
        if (found)
        {
          buf[0] = UPPER(buf[0]);
          send_to_char( buf, ch );
        }
    }
    if ( !found )
        send_to_char( "You cannot seem to find any artifacts.\n\r", ch );

    return;
}
void do_rares( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    bool found;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !IS_SET(obj->quest, QUEST_RUNE) ) continue;

	if (obj->in_obj != NULL && can_see_obj(ch, obj))
        {
            sprintf( buf, "%s carried inside %s.\n\r",
                obj->short_descr, obj->in_obj->short_descr );
            found = TRUE;
        }

        else if ( obj->carried_by != NULL && can_see(ch,obj->carried_by))
        {
          if ( obj->wear_loc >= 0 )
            sprintf( buf, "%s equipped by %s.\n\r",
                obj->short_descr, PERS(obj->carried_by, ch) );
          else
            sprintf( buf, "%s carried by %s.\n\r",
                obj->short_descr, PERS(obj->carried_by, ch) );
            found = TRUE;
        }

	else if ( obj->carried_by == NULL && obj->in_room != NULL&& can_see_obj(ch, obj))
        {
            sprintf( buf, "%s in %s.\n\r",
                obj->short_descr, obj->in_room->name );
            found = TRUE;
        }

	else
	{
 	  found = FALSE;
	}
	if (found)
	{
  	  buf[0] = UPPER(buf[0]);
	  send_to_char( buf, ch );
	}
    }

    if ( !found )
        send_to_char( "You cannot seem to find any rare equipment.\n\r", ch );

    return;
}

int randMight(CHAR_DATA *ch)
{
  AFFECT_DATA *paf;
  OBJ_DATA *obj;
  OBJ_DATA *obj2;
  OBJ_DATA *obj2_next;
  int objhps = 0;
  int objmove = 0;
  int objmana = 0;
  int might = 0;
  int temp = 0;
  int i;
  int number_rare = 0;  
  if (IS_NPC(ch)) return might;
  if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) ) return might;
  if (ch->class == 0) return might;
  for (i = 0; i < MAX_WEAR; i++)
  {
    if ((obj = get_eq_char(ch, i)) == NULL) continue;
    for (paf = obj->pIndexData->first_affect; paf != NULL; paf = paf->next)
    {
          if (paf->location == APPLY_HIT)
        objhps += paf->modifier;
          if (paf->location == APPLY_MANA)
        objmana += paf->modifier;
	  if (paf->location == APPLY_MOVE)
        objmove += paf->modifier;
    }
    for (paf = obj->first_affect; paf; paf = paf->next) 
    {
          if (paf->location == APPLY_HIT)                       
        objhps += paf->modifier;
	  if (paf->location == APPLY_MANA)
        objmana += paf->modifier;
	  if (paf->location == APPLY_MOVE)
        objmove += paf->modifier;
    }
  }
  if ( ch->first_affect != NULL )
  {
    for ( paf = ch->first_affect; paf != NULL; paf = paf->next )
    {
          if (paf->location == APPLY_HIT)
        objhps += paf->modifier;
          if (paf->location == APPLY_MANA)
        objmana += paf->modifier;
          if (paf->location == APPLY_MOVE)
        objmove += paf->modifier;
    }
  }
  for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
  {
        if (str_cmp(obj->questowner,ch->pcdata->switchname)) continue;
        if (obj->pIndexData->vnum >= 842 && obj->pIndexData->vnum <= 867)
        {
		might += 5;
        }
        else if (obj->pIndexData->vnum >= 869 && obj->pIndexData->vnum <= 894)
        {
                might += 5;
        }
        else if (obj->pIndexData->vnum >= 780 && obj->pIndexData->vnum <= 805)
        {
                might += 5;
        }
        else if (obj->pIndexData->vnum >= 806 && obj->pIndexData->vnum <= 808)
        {
                might += 5;
        }
        else if (obj->pIndexData->vnum >= 814 && obj->pIndexData->vnum <= 825)
        {
                might += 15;
        }

        if (obj->contains != NULL)
	{
           for ( obj2 = obj->contains; obj2 != NULL; obj2 = obj2_next )
           {
	      obj2_next = obj2->next;
        if (str_cmp(obj2->questowner,ch->pcdata->switchname)) continue;
        if (obj2->pIndexData->vnum >= 842 && obj2->pIndexData->vnum <= 867)
        {
                might += 5;
        }
        else if (obj2->pIndexData->vnum >= 869 && obj2->pIndexData->vnum <= 894)
        {
                might += 5;
        }
        else if (obj2->pIndexData->vnum >= 780 && obj2->pIndexData->vnum <= 805)
        {
                might += 5;
        }
        else if (obj2->pIndexData->vnum >= 806 && obj2->pIndexData->vnum <= 808)
        {
                might += 5;
        }
        else if (obj2->pIndexData->vnum >= 814 && obj2->pIndexData->vnum <= 825)
        {
                might += 15;
        }

  	   }
        }
  }

  if (IS_ITEMAFF(ch, ITEMA_ARTIFACT))  might += 100;
  
  might += UMAX(0,(ch->max_hit - objhps) / 1000);
  might += UMAX(0,(ch->max_move - objmove) / 1000);
  might += UMAX(0,(ch->max_mana - objmana) / 1000);
  might += UMAX(0,(ch->pcdata->quest / 1000));
  might += UMAX(0,(ch->practice / 1000));
  if (IS_SET(ch->newbits, NEW_MASTERY)) might += 100;
  if (ch->played > 3600)  might += ((ch->played / 3600) * 50);
  might += UMAX(0,(ch->gold / 20000000));
  might += UMAX(0,(ch->pcdata->bank / 20000000));

  if (!IS_SET(ch->special, SPC_NEWBIE))
  {
    if (ch->lstatus > 0) might += (ch->lstatus * 100);
    if (ch->race > 0) might += (ch->race * 10);
    if ((ch->pkill - ch->pdeath) > 0)
    {
      temp = (ch->pkill - ch->pdeath);
      might += temp; 
    }
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
      if (IS_SET(obj->quest, QUEST_RUNE))
      {
        number_rare++;
      }
    }
    might += UMAX(0,(number_rare * 100));
  }

  if (IS_IMMORTAL(ch)) might = 1000;
  return might;
}

void do_purchase(CHAR_DATA *ch, char *argument) 
{
  char buf[MSL];
  int value;
  int next_status;
 
  if (IS_NPC(ch)) return;
  if (ch->lstatus < 9) next_status = (ch->lstatus + 1) * 5;
  else if (ch->lstatus == 9) next_status = 60;
  if (IS_SET(ch->special, SPC_NEWBIE))
	{
	  stc("You cannot do this, newbie.\n\r",ch);
	  return;
	}
  if (xIS_SET(ch->extra, EXTRA_JUST_PK))
       {
          stc("You cannot do this, you have sworn to just pk.\n\r",ch);
          return;
       }
  if (IS_SET(ch->newbits, NEW_SUPREME))
       {
	  stc("No more purchasing for supremes.\n\r",ch);
	  return;
       }
  if (ch->lstatus < 1)
	{
	  stc("You must be legendary to purchase status.\n\r",ch);
	  return;
	}
  else if (ch->lstatus > 0) 
	{
           value = (100000000 + (ch->lstatus * 10000000) + (ch->race * 100000));
	}
  if (ch->fight_timer > 0)
	{
	  stc("You are too occupied to do this.\n\r",ch);
	  return;
	}
  if (ch->gold < value)
	{
	  sprintf(buf,"The cost is %d gold for 1 status.\n\r",value);
	  stc(buf,ch);
	  return;
	}
  if (ch->race >= next_status)
	{
	  stc("You cannot purchase any more.\n\r",ch);
	  return;
	}

  ch->gold -= value;
  ch->race += 1;
  stc("Your gold has earned you 1 solitary status, may it serve you well.\n\r",ch);
  sprintf(buf,"%s has spend %d gold on 1 status.",ch->name,value);
  log_string(buf);
  return;
}
void do_mastery(CHAR_DATA *ch, char *argument)
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  int max_skl;
  int spl_skl;
  int vnum;
  char buf[MAX_INPUT_LENGTH];
  
  if (IS_NPC(ch)) return;
  if (xIS_SET(ch->act, PLR_MASTERY))
  {
    send_to_char("You've already gotten your mastery. If you lost it, tough luck!\n\r",ch);
    return;
  }
  if (ch->class == 0)
  {
    stc("But you don't have a class!\n\r",ch);
    return;
  }

        if (IS_CLASS(ch, CLASS_DROW) && (IS_SET(ch->special, SPC_DROW_WAR)))
                max_skl = 550;
        if (IS_CLASS(ch, CLASS_DROW) && (!IS_SET(ch->special, SPC_DROW_WAR)))
                max_skl = 200;
        if (ch->class == CLASS_WEREWOLF)
                max_skl = 600;
        if (ch->class == CLASS_MONK)
                max_skl = 750;
        if (IS_CLASS(ch, CLASS_DEMON))
                max_skl = 600;
        if (IS_CLASS(ch, CLASS_ANGEL))
                max_skl = 650;
        if (IS_CLASS(ch, CLASS_POWER_LICH))
                max_skl = 500;
        if (IS_CLASS(ch, CLASS_MAGE))
                max_skl = 600;
        if (IS_CLASS(ch, CLASS_VAMPIRE))
                max_skl = 550;
        if (IS_CLASS(ch, CLASS_NINJA))
                max_skl = 750;
        if (IS_CLASS(ch, CLASS_CYBORG) && (ch->pcdata->powers[CYBORG_LIMBS] > 3))
                max_skl = 600;
        if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                max_skl = UMAX(300, ch->pcdata->powers[WEAPONSKILL] * 100);
        if (IS_CLASS(ch, CLASS_HIGHLANDER))
                max_skl = 1200;
        if( IS_CLASS(ch, CLASS_SHAPESHIFTER)) max_skl = 600;
        if( IS_CLASS(ch, CLASS_TANARRI)) max_skl = 700;
        if (IS_CLASS(ch, CLASS_PALADIN)) max_skl = 700;
        if (IS_CLASS(ch, CLASS_PSIONIST)) max_skl = 700;
        if (IS_CLASS(ch, CLASS_INQUISITOR)) max_skl = 700;
   
  if (IS_CLASS(ch, CLASS_PSIONIST) && IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MENTAL))
     spl_skl = 1000;
  if (IS_CLASS(ch, CLASS_MAGE))
     spl_skl = 950;
  if (IS_CLASS(ch, CLASS_POWER_LICH))
     spl_skl = 450;
  if (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_POWER_LICH) && !IS_CLASS(ch, CLASS_PSIONIST))
     spl_skl = 200;

  if (ch->wpn[0] < max_skl || ch->wpn[1] < max_skl || ch->wpn[2] < max_skl || ch->wpn[3] < 
max_skl || ch->wpn[4] < max_skl || ch->wpn[5] < max_skl || ch->wpn[6] < max_skl ||
 ch->wpn[7] < max_skl || ch->wpn[8] < max_skl || ch->wpn[9] < max_skl ||
 ch->wpn[10] < max_skl || ch->wpn[11] < max_skl || ch->wpn[12] < max_skl )
  {
    send_to_char("You have not completed your weapon training, my friend.\n\r",ch);
    return;
  }
  if (ch->spl[0] < spl_skl || ch->spl[1] < spl_skl || ch->spl[2] < spl_skl || ch->spl[3] < 
spl_skl || ch->spl[4] < spl_skl )
  {
    send_to_char("You must first finish your magicial training.\n\r",ch);
    return;
  }
  if (ch->stance[1] < 200 || ch->stance[2] < 200 || ch->stance[3] < 200 || ch->stance[4] < 200
 || ch->stance[5] < 200 || ch->stance[6] < 200 || ch->stance[7] < 200 || ch->stance[8] < 200
 || ch->stance[9] < 200 || ch->stance[10] < 200 )
  {
    send_to_char("Maybe you should grandmaster your stances first.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_MAGE)) vnum = 33014;
  else if (IS_CLASS(ch, CLASS_WEREWOLF)) vnum = 33112;
  else if (IS_CLASS(ch, CLASS_NINJA)) vnum = 33094;
  else if (IS_CLASS(ch, CLASS_MONK)) vnum = 33032;
  else if (IS_CLASS(ch, CLASS_DROW)) vnum = 33074;
  else if (IS_CLASS(ch, CLASS_DEMON)) vnum = 33134;
  else if (IS_CLASS(ch, CLASS_VAMPIRE)) vnum = 33054;
  else if (IS_CLASS(ch, CLASS_CYBORG)) vnum = 33153;
  else if (IS_CLASS(ch, CLASS_HIGHLANDER)) vnum = 33177;
  else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) vnum = 29989;
  else if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) vnum = 33174;
  else if (IS_CLASS(ch, CLASS_ANGEL)) vnum = 33193;
  else if (IS_CLASS(ch, CLASS_TANARRI)) vnum = 33213;
  else if (IS_CLASS(ch, CLASS_POWER_LICH)) vnum = 33233;
  else if (IS_CLASS(ch, CLASS_PALADIN)) vnum = 33342;
  else if (IS_CLASS(ch, CLASS_PSIONIST)) vnum = 33343;
  else if (IS_CLASS(ch, CLASS_INQUISITOR)) vnum = 33341;
  else
  {
    send_to_char("Your class mastery is not done yet, please notify Rand.\n\r",ch);
    return;
  }
  if (IS_CLASS(ch, CLASS_POWER_LICH))
  {
  obj = create_object(get_obj_index(vnum), 50);
  obj_to_char(obj,ch);
  obj->questowner = str_dup(ch->pcdata->switchname);
  if ((pObjIndex = get_obj_index(33240)) != NULL)
  obj = create_object(pObjIndex, 50);
  obj_to_char(obj,ch);
  obj->questowner = str_dup(ch->pcdata->switchname);
  }

  if (!IS_CLASS(ch, CLASS_POWER_LICH))
  {
  obj = create_object(get_obj_index(vnum), 50);
  obj_to_char(obj,ch);
  obj->questowner = str_dup(ch->pcdata->switchname);
  }
  sprintf(buf,"#C%s #7has become a master of %s class.#n",ch->name,ch->sex ==SEX_MALE         ? "his" : ch->sex == SEX_FEMALE       ? "her"      : "its");
  do_info(ch,buf);
  xSET_BIT(ch->act, PLR_MASTERY);
  return;
}
void do_breakfree(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( obj->chobj == NULL || obj->chobj != ch )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if ( obj->pIndexData->vnum != 420002)
    {
        stc("You are not trapped!\n\r",ch);	
        return;
    }
    if ( obj->pIndexData->vnum == 420002)
    {
	if ( ch->pcdata->quest < 10000)
	{
	  stc("You do not have the 10,000 qp required to breakfree.\n\r",ch);
	  return;
	}
	else if (ch->pcdata->quest >= 10000)
	{
	  sprintf(buf,"You have broken free of the %s!\n\r",obj->short_descr);
	  stc(buf,ch);
	  sprintf(buf, "#P%s #0has broken free of the#n %s!",
	  ch->pcdata->switchname,obj->short_descr);
	  do_info(NULL,buf);
	  extract_obj(obj);
	  ch->pcdata->quest -= 10000;
	  return;
	}
     return;
    }
  return;
}
void do_legendexp(CHAR_DATA *ch, char *argument)
{
char arg[MSL];
char buf[MSL];
int status;
argument = one_argument(argument, arg);
status = (ch->lstatus) * 5;

if (IS_NPC(ch)) return;
if (IS_SET(ch->special, SPC_NEWBIE))
{
  stc("Newbies cannot store.\n\r",ch);
  return;
}

if ( arg[0] == '\0')
{
  stc("Options for Store: exp, status\n\r",ch);
  return;
}
if (ch->lstatus >= 9)
{
  stc("You are already the highest legend.\n\r",ch);
  return;
}
if (ch->fight_timer > 0 || ch->fighting != NULL)
{
  stc("You are too occupied to do this.\n\r",ch);
  return;
}
if (!str_cmp(arg, "exp"))
{    
  if (xIS_SET(ch->extra, EXTRA_JUST_PK))
  {
   stc("You cannot do this, you have sworn to just pk.\n\r",ch);
   return;
  }

  if (ch->lexp > (ch->lstatus))
  {
   stc("You must train your legend first.\n\r",ch);
   return;
  }

  if (ch->exp < 50000000000)
	{
	stc("One legend point requires 50 billion exp.\n\r",ch);
	return;
	}
  else if (ch->exp >= 50000000000)
	{
	stc("Your experience is converted into 1 legend point.\n\r",ch);
	ch->exp -= 50000000000;
	ch->lexp += 1;
        do_save(ch, "");
	return;
	}
}
if (!str_cmp(arg, "status"))
{
  if (IS_SET(ch->special, SPC_NEWBIE))
        {
          stc("You cannot do this, newbie.\n\r",ch);
          return;
        }

  if (ch->race == 0)
  {
   stc("No status? How can you wish to store no status?\n\r",ch);
   return;
  }
  if (ch->lexp > (ch->lstatus))
  {
   stc("You must train your legend first.\n\r",ch);
   return;
  }
  if ((ch->race - 5) < status)
  {
  sprintf(buf,"You need to retain a minimum of %d status.\n\r",status);
  stc(buf,ch);
  return;
  }
  else
  {
   stc("5 of your status is converted to 1 legend point.\n\r",ch);
   sprintf(buf,"%s spends 5 status on 1 legend point.",ch->pcdata->switchname);
   log_string(buf);
   ch->race -= 5;
   ch->lexp += 1;
   do_save(ch, "");
   return;
  }
 }
}
void do_pkguardian(CHAR_DATA *ch, char *argument)
{
 if (IS_NPC(ch)) return;
 if (!IS_NPC(ch))
 {
  if (ch->race < 20)
  {
   stc("You need 20 status.\n\r",ch);
   return;
  }
  else if (ch->race >= 20)
  { 
   if (!IS_SET(ch->immune, IMM_SCRY))
   {
    stc("You eyes begin to twitch, causing you to become very aware.\n\r",ch);
    SET_BIT(ch->immune, IMM_SCRY);
    return;
   }
   else if (IS_SET(ch->immune, IMM_SCRY))
   { 
    stc("Your eyes lose their twitch, causing your vision to slowly revert to normal.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SCRY);
    return;
   }
  }
 }
return;
}

void do_ffa(CHAR_DATA *ch, char *argument)
{
char arg[MSL];
char buf[MSL];
int char_room;
char_room = number_range(500006,500026);

argument = one_argument(argument, arg);
if (IS_NPC(ch)) return;
if (!IS_HERO(ch)) return;
if ( arg[0] == '\0')
{
  stc("#0Options for the #RFREE #yFOR #RALL #0are: #7on #Roff #7join #Rleave #n\n\r",ch);
  return;
}
  if (!str_cmp(arg, "on"))
    {
     if (ch->pcdata->quest < 5000)
     {
     	stc("#0You need at least #75000 qp #0to buy the #RFREE #yFOR #RALL#0!",ch);
 	return;
     }
     if (sysdata.ffa_timer >= 0)
        {
            send_to_char("#0The #RFREE #yFOR #RALL #0is already in effect!#0\n\r",ch);
            return;
        }
     sysdata.ffa_timer += 1200;
     ch->pcdata->quest -= 5000;
     sprintf( buf, "#0=-=-=-=-=-=-=-==-=-=-=-=-=-=-=#R(#y[ #RFREE #yFOR #RALL #y]#R)#0=-=-=-=-=-=-=-==-=-=-=-=-=-=-=#n");
     do_infof( NULL, buf );
     sprintf( buf, "#0     To join the #RFREE #yFOR #RALL:  #0type: #7ffa join  :#0To join the #RFREE #yFOR #RALL#n");
     do_infof( NULL, buf );
     sprintf( buf, "#0      Info about #RFREE #yFOR #RALL:  #0type: #7help ffa  :#0Info about #RFREE #yFOR #RALL#n");
     do_infof( NULL, buf );
     sprintf( buf, "#0=-=-=-=-=-=-=-==-=-=-=-=-=-=-=#R(#y[ #RFREE #yFOR #RALL#y ]#R)#0=-=-=-=-=-=-=-==-=-=-=-=-=-=-=#n");
     do_infof( NULL, buf );
     send_to_char("#0The #RFREE #yFOR #RALL #0has begun!#n\n\r",ch);
     return;
    }
  else if (!str_cmp(arg, "off"))
    {
     if (!IS_IMMORTAL(ch))
     {
	stc("#0Who are you to try and remove the #RFREE #yFOR #RALL#0?#n\n\r",ch);
	return;
     }
     if (sysdata.ffa_timer < 0)
        {
            send_to_char("#0The #RFREE #yFOR #RALL #0is not in effect!#n\n\r",ch);
            return;
        }
     sysdata.ffa_timer = -1;
     sprintf(buf, "#0The #RFREE #yFOR #RALL#0 has been ended!#n\n\r");
     do_info(NULL, buf);
    }
  else if (!str_cmp(arg, "join"))
    {
    if (IS_IMMORTAL(ch))
    {
	stc("#0You are too high and mighty to join the #RFREE #yFOR #RALL#0!#n\n\r",ch);
	return;
    }
    if (!CAN_PK(ch))
    {
	stc("#0You must be an avatar to join the #RFREE #yFOR #RALL#0!#n\n\r",ch);
	return;
    }
    if (IS_SET(ch->special, SPC_NEWBIE))
    {
	stc("#0Newbies are too weak to join the #RFREE #yFOR #RALL#0!#n\n\r",ch);
	return;
    }
    if (ch->fight_timer > 0)
    {
/*	sprintf(buf, "#0[#yA#Rlert#y!#0]  #7%s#0 has decided to join the #RFREE #yFOR #RALL#n!",ch->name);
	do_echo(ch,buf);
	sprintf(buf, "#0[#yA#Rlert#y!#0]  #7%s#0 was realized to have a #PFIGHT TIMER#0, what a #PWUSS#0!#n",ch->name);
	do_echo(ch,buf);
	sprintf(buf, "#0[#yA#Rlert#y!#0]  #7%s#0 begins to look around nervously, acting very #yparanoid#0!#n",ch->name);
	do_echo(ch,buf);
	sprintf(buf, "#0[#yA#Rlert#y!#0]  #7%s#0 has been #Rstruck down#0 from #7above#0 by the #RAngry #7G#yo#7ds#0.",ch->name);
	do_echo(ch,buf);
	do_paradox(ch,"self"); */
	send_to_char("You cannot join the free for all with a fight timer.\n\r",ch);
	return;
    }
    if (sysdata.ffa_timer < 0)
    {
	stc("#0Why do you want to join, why not #7START#0 the #RFREE #yFOR #RALL#0!#n\n\r",ch);
	return;
    }
    else if (sysdata.ffa_timer > 0)
    {
	send_to_char("#0You join the #RFREE #yFOR #RALL#0!#n\n\r",ch);
	sprintf(buf,"#R%s#0 has decided to join the #RFREE #yFOR #RALL#0!#n",ch->name);
	do_infof(NULL, buf);
	char_from_room(ch);
	char_to_room(ch,get_room_index(char_room));
	do_look(ch, "auto");
	return;
    }
   }
   else if (!str_cmp(arg, "leave"))
   {
	if (sysdata.ffa_timer < 0)
	{
        stc("#0Why do you want to leave, why not #7START#0 the #RFREE #yFOR #RALL#0!#n\n\r",ch);
	return;
	}
	else if (sysdata.ffa_timer > 0)
 	{
     	  if (ch->position != POS_STANDING)
	  {
	    stc("#0You must be standing to depart from the #RFREE #yFOR #RALL#0!#n\n\r",ch);
	    return;
	  }
	  else if (ch->position == POS_STANDING)
	  {
	    if (ch->pcdata->quest < 500)
	    {
	      stc("#0There is a #7500 qp#0 fee for departing the #RFREE #yFOR #RALL#0!#n\n\r",ch);
	      return;
	    }
            else if (ch->pcdata->quest >= 500)
	    {
	    char_from_room(ch);
	    char_to_room(ch,get_room_index(ROOM_VNUM_NECROPOLIS));
	    sprintf(buf, "#7%s#0 leaves the #RFREE #yFOR #RALL#0 like a coward!#n",ch->name);
	    do_infof(NULL, buf);
	    stc("#0Your time within #RFREE #yFOR #RALL #0has ended!#n\n\r",ch);
	    stc("#0You have been transported to the #7Necropolis#0!#n\n\r",ch);
	    ch->fight_timer = 0;
	    ch->pcdata->quest -= 500;
	    update_pos(ch);
	    do_look(ch, "auto");
	    return;
	    }
	  }
	}
    }
 else
 {
 stc("#0Options for the #RFREE #yFOR #RALL #0are: #7on #Roff #7join #Rleave #n\n\r",ch);
 return;
 }

 return;
}

void do_general(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;
	char arg[MAX_STRING_LENGTH];

	argument = one_argument(argument, arg);

	if (IS_NPC(ch)) return;

	if (ch->pcdata->kingdom == 0) {
	send_to_char("You are not in a clan.\n\r", ch );
	return;}

	if (str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader_name)) {
	send_to_char("You are not the leader of your clan.\n\r", ch );
	return;}

	if ((victim = get_char_room(ch, arg)) == NULL) {
	send_to_char("They are not here.\n\r", ch );
	return;}

	if (IS_NPC(victim)) {
	send_to_char("Not on NPCs.\n\r", ch );
	return;}

	if (victim->pcdata->kingdom != ch->pcdata->kingdom) {
	send_to_char("They are not in your clan.\n\r", ch );
	return;}

	if (ch == victim) {
	send_to_char("You can't do that.\n\r", ch );
	return;}
	if (victim->race < 5)
	{
	 send_to_char("To become a general, they must be stronger.\n\r",ch);
	 return;
	}
	if (IS_SET(victim->special, SPC_PRINCE))
 	{
	 send_to_char("They are already a prince!\n\r",ch);
	 return;
        }
	if (IS_SET(victim->special, SPC_GENERAL)) {
	REMOVE_BIT(victim->special, SPC_GENERAL);
	send_to_char("You are no longer a general.\n\r", victim);
	send_to_char("They are no longer a general.\n\r", ch );
	save_char_obj(victim);
	return;}

	SET_BIT(victim->special, SPC_GENERAL);
	send_to_char("You are now a general!\n\r", victim);
	send_to_char("You make them a general.\n\r", ch );
	save_char_obj(victim);
	return;
}
void do_pkdamage( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 AFFECT_DATA af;
 char arg[MSL];
 char buf[MSL];
 int dam;

 argument = one_argument( argument, arg );

 if (IS_NPC(ch)) return;
 if (ch->race < 45) return;
 if (ch->fight_timer < 1)
   {
    send_to_char("Not without a fighttimer.\n\r",ch);
    return;
   }
        if ( arg[0] == '\0' )
    {
     send_to_char( "Abuse whom?\n\r", ch );
     return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
     send_to_char( "They aren't here.\n\r", ch );
     return;
    }

    if ( victim == ch )
    {
     send_to_char( "You do not wish to hurt yourself!\n\r", ch );
     return;
    }
    if (IS_NPC(victim))
    {
     send_to_char("Not on mobbies!\n\r",ch);
     return;
    }
    else if (!IS_NPC(victim))
    {
    if (is_safe(ch,victim) == TRUE) return;
   
   dam = number_range(char_damroll(ch), char_damroll(ch)*4);
   if (dam > 25000) dam = 25000;
   sprintf(buf,"#GYour abuse causes #y$N #Rhorrible #Gdamage! #n[#y%d#n]",dam);
   act(buf,ch,NULL,victim,TO_CHAR, FALSE);
   sprintf(buf,"#y$n#G's abuse causes you #Rhorrible #Gdamage! #n[#y%d#n]",dam);
   act(buf,ch,NULL,victim,TO_VICT, FALSE);
   sprintf(buf,"#y$n#G's abuses #y$N #Gfor #Rhorrible #Gdamage! #n[#y%d#n]",dam);
   act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
   if (number_range(1,2) == 1)
   {
      af.type      = skill_lookup("web");
      af.location  = APPLY_AC;
      af.modifier  = 200;
      af.duration  = number_range(30,60);
      af.bitvector = AFF_WEBBED;
      affect_to_char( victim, &af );
    send_to_char( "You are coated in a sticky web!\n\r", victim );
    act("$n is coated in a sticky web!",victim,NULL,NULL,TO_ROOM, FALSE);
   }
   hurt_person(ch,victim,dam);
   set_fighting(ch,victim);
   ch->fight_timer += 3;
   WAIT_STATE(ch, 20);
 return;
   }
return;
}


void do_pkteleport( CHAR_DATA *ch, char *argument )
{
 if (IS_NPC(ch)) return;
 if (ch->race < 75) return;
 if (ch->fight_timer < 1)
   {
    send_to_char("Not without a fighttimer.\n\r",ch);
    return;
   }
 if (ch->move < 10000)
  {
   stc("You do not have enough movement to dissolve!\n\r",ch);
   return;
  }
 if (ch->fighting == NULL)
  {
   stc("You can only dissolve while fighting!\n\r",ch);
   return;
  }
    if (ch->fighting != NULL)
    {
    act("$n dematerializes into nothing.",ch,NULL,NULL,TO_ROOM, FALSE);
    stc("You are disintegrated and swept away into nothingness.\n\r",ch);
    if (ch->fighting != NULL)
        stop_fighting( ch, TRUE );
    char_from_room(ch);
    char_to_room(ch, get_random_room(ch));
    stc("You combine from elements within the room to form your normal self.\n\r",ch);
    act("$n condenses from elements within the room!",ch,NULL,NULL,TO_ROOM, FALSE);
    ch->move -= 10000;
    do_look(ch,"auto");
        WAIT_STATE(ch, 10);
    return;
    }
 return;
}
void do_pkflee( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    int attempt;


	if ( IS_NPC(ch) ) return;
    if (ch->race < 30) return;
    if ( ( victim = ch->fighting ) == NULL )
    {
	if ( ch->position == POS_FIGHTING )
	    ch->position = POS_STANDING;
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }
	if (ch->move < 5000)
	{
	send_to_char("You don't have enough movement points to retreat.\n\r", ch);
	return;
	} 
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }
    was_in = ch->in_room;
    
{
	EXIT_DATA *pexit;
	int door;

	send_to_char("You attempt to retreat!\n\r", ch );

for ( attempt =  0; attempt < 6; attempt++ ) 
	{
	door = number_door( );
	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->to_room == NULL
	||   IS_SET(pexit->exit_info, EX_CLOSED) )
	      continue;


	move_char( ch, door );
	if ( ( now_in = ch->in_room ) == was_in )
	      continue;

	ch->in_room = was_in;
	act( "$n has retreated!", ch, NULL, NULL, TO_ROOM, FALSE );
	ch->in_room = now_in;
	
	if ( !IS_NPC(ch) )
	    send_to_char( "You have retreated from battle!\n\r", ch ); 

    ch->move -= 5000;
	stop_fighting( ch, TRUE );
	WAIT_STATE(ch, 6);
	return;
   }
   
}
    return;
}

void pkready( CHAR_DATA *ch)
{
char buf[MSL];
int might;

if (IS_NPC(ch)) return;
if (!IS_SET(ch->special, SPC_NEWBIE))
 {
   return;
 }
might = randMight(ch);

if (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_POWER_LICH))
 {
	if (might < 1250)
	return;
 }
else
 {
  if (might < 1000)
	return;
 }

REMOVE_BIT(ch->special, SPC_NEWBIE);
stc("OK, you are now allowed to pk!\n\r",ch);
sprintf(buf, "#C%s #0has left the ranks of the #Pnewbie#0.#n",ch->pcdata->switchname);
do_update(NULL,buf);
return;
}

void do_pkready( CHAR_DATA *ch, char *argument)
{
char buf[MSL];

if (IS_NPC(ch)) return;
if (!IS_SET(ch->special, SPC_NEWBIE))
 {
   stc("But you are not a newbie...?\n\r",ch);
   return;
 }
if (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_POWER_LICH))
 {
        if (randMight(ch) < 1250)
	{
	stc("You are not strong enough yet, newbie!\n\r",ch);
        return;
	}
 }
else
 {
  if (randMight(ch) < 1000)
	{
	stc("You are not strong enough yet, newbie!\n\r",ch);
        return;
	}
 }
REMOVE_BIT(ch->special, SPC_NEWBIE);
stc("OK, you are now allowed to pk!\n\r",ch);
sprintf(buf, "#C%s #0has left the ranks of the #Pnewbie#0.#n",ch->pcdata->switchname);
do_update(NULL,buf);
return;
} 
void do_donate( CHAR_DATA *ch, char *argument)
{
   OBJ_DATA *pit;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *original;
   char arg[MAX_INPUT_LENGTH];
   char buf[MSL];
   argument = one_argument(argument, arg);


   if (arg[0] == '\0' )
   {
      send_to_char("Donate what?\n\r",ch);
      return;
   }

   original = ch->in_room;
   if (ch->position == POS_FIGHTING)
   {
      send_to_char(" You're fighting!\n\r",ch);
      return;
   }

   if ( (obj = get_obj_carry (ch, arg)) == NULL)
   {
      send_to_char("You do not have that!\n\r",ch);
      return;
   }
   else
   {
      if (!IS_SET(obj->quest, QUEST_ARTIFACT))
      {
         send_to_char("You can only donate artifacts!\n\r",ch);
         return;
      }
      if (ch->in_room != get_room_index(ROOM_VNUM_ALTAR))
         act("$n donates $p.",ch,obj,NULL,TO_ROOM, FALSE);
      act("You donate $p.",ch,obj,NULL,TO_CHAR, FALSE);
         
     
         char_from_room(ch);
         char_to_room(ch,get_room_index(ROOM_VNUM_DONATE));
         pit = get_obj_list(ch, "pit", ch->in_room->contents);
         obj_from_char(obj);
         obj_to_obj(obj, pit);
         char_from_room(ch);
         char_to_room(ch,original);
	 sprintf(buf,"%s sends %s to #0The #rP#Ri#rt #0of #rS#0ac#Rrif#0ic#re#n!",ch->name,obj->short_descr); 
	 log_string(buf);
	 do_info(ch,buf);
         return;
     }   
}
void do_toss( CHAR_DATA *ch, char *argument )
{
 char arg[MSL];
 char arg2[MSL];
 char buf[MSL];
 char buf3[MSL];
 char buf4[MSL];
 char buf5[MSL];
 char buf7[MSL];
 int number;
 int urin;
 int sides;
 int roll;
 int total = 0;
 int easy;
 int remaining;

 argument = one_argument( argument, arg );
 argument = one_argument( argument, arg2);

  remaining = (499 - ch->pcdata->wolf);

  if (ch->pcdata->wolf >= 500 && !IS_IMMORTAL(ch))
  {
   stc("You have diced your last die, my friend.\n\r",ch);
   return;
  }
  if (!IS_SET(ch->special, SPC_NEWBIE) && !IS_IMMORTAL(ch))
  {
   stc("Only newbies can dice for gold.\n\r",ch);
   return;
  }

 if ( arg[0] == '\0' )
 {
  stc("You cannot throw dice without selecting how many to throw.\n\r",ch);
  stc("Syntax: 'toss XX YY'  XX = Number of dice, YY = Sides on die.\n\r",ch);
  return;
 }
 if ( arg2[0] == '\0')
 {
  stc("You must select the odds on the dice you wish to throw.\n\r",ch);
  stc("Syntax: 'toss XX YY'  XX = Number of dice, YY = Sides on die.\n\r",ch);
  return;
 }
 if (!is_number(arg))
 {
  stc("The number of dice must be a number.\n\r",ch);
  return;
 }
 if (!is_number(arg2))
 {
  stc("The number of sides the dice have must be a number.\n\r",ch);
  return;
 }
 else if (is_number(arg) && is_number(arg2) )
 {
  number = atoi(arg);
  sides = atoi(arg2);
  if ((number < 2) || (number > 6))
  {
   stc("You can only toss 2 to 6 dice.\n\r",ch);
   return;
  }
  if ((sides < 4) || (sides > 100))
  {
   stc("The dice must have between 4 and 100 sides, inclusive.\n\r",ch);
   return;
  }
  if (ch->pcdata->quest < number)
  {
   stc("You must have more questpoints then dice you wish to toss!\n\r",ch);
   return;
  }
  if ((number >= 2) && (number <= 6) && (sides >= 4) && (sides <= 100))
  {
   for (urin = 0; urin < number ; urin++)
   {
     roll = number_range(1, sides);
     sprintf(buf, "%s tosses a %d sided die and looks down to see a %d.",ch->name,sides,roll);
     act(buf,ch,NULL,NULL,TO_NOTVICT, TRUE);
     total += roll;
   }
     sprintf(buf3,"%s winds up with a grand total of %d out on %d rolls.",ch->name,total,number);
     act(buf3,ch,NULL,NULL,TO_NOTVICT, FALSE);
     sprintf(buf4,"You wind up with a grand total of %d out on %d rolls. (#0Remaining: %d#n)",total,number,remaining);
     act(buf4,ch,NULL,NULL,TO_CHAR, FALSE);
     ch->pcdata->quest -= number;
     if (total == 600)
     {
       easy = total * total;
       sprintf(buf5,"#7%s #Ris awarded #7%d QP #Rfor tossing a perfect 600!#n",ch->name,easy); 
       do_info(ch, buf5 );
       ch->pcdata->quest += easy;
       ch->pcdata->score[SCORE_QUEST] += easy;
       WAIT_STATE(ch, 16);
       ch->pcdata->wolf += 1;
     }
     if (total >= 500 && total < 600)
     {
       easy = total * 2;
       sprintf(buf7,"#y%s #Gis awarded #y%d QP #Gfor tossing a %d!#n",ch->name,easy,total);     
       do_info(ch, buf7 );
       ch->pcdata->quest += easy;
       ch->pcdata->score[SCORE_QUEST] += easy;
       WAIT_STATE(ch, 16);
       ch->pcdata->wolf += 1;
     }
     if (total >= 400 && total < 500)
     {
       easy = total;
       sprintf(buf7,"#y%s #Gis awarded #y%d QP #Gfor tossing a %d!#n",ch->name,easy,total);
       do_info(ch, buf7);
       ch->pcdata->quest += easy;
       ch->pcdata->score[SCORE_QUEST] += easy;
       WAIT_STATE(ch, 16);
       ch->pcdata->wolf += 1;
     }
     if (total < 400)
     {
       stc("Your dicing skills leave a lot to be desired.\n\r",ch);
       WAIT_STATE(ch, 14);
       ch->pcdata->wolf += 1;
       return;
     }
    return;
  }
 return;
 }   
}
void do_dice( CHAR_DATA *ch, char *argument )
{
 char arg[MSL];
 int chance;
 int roll;
 char buf[MSL];
 char buf2[MSL];
 argument = one_argument( argument, arg );

 if (IS_NPC(ch)) return;
 if ( arg[0] == '\0' )
 {
  stc("You cannot throw a die before selecting your odds.\n\r",ch);
  return;
 }
 if (!is_number(arg))
 {
  stc("The odds are: 1 roll on an X sided die.  Next time enter a numeric value for X.\n\r",ch);
  return;
 }
 else if (is_number(arg))
 {
   chance = atoi(arg);
   if ((chance < 4) || (chance > 100))
   {
    stc("The die must have between 4 and 100 sides, inclusive.\n\r",ch);
    return;
   }
   else if ((chance >= 4) && (chance <= 100))
   {
     roll = number_range(1, chance);
     sprintf(buf, "%s tosses a %d sided die and looks down to see a %d.",ch->name,chance,roll);
     act(buf,ch,NULL,NULL,TO_ROOM, FALSE);
     sprintf(buf2,"You toss a %d sided die and look down to find a %d staring up at you.",chance,roll);
     act(buf2,ch,NULL,NULL,TO_CHAR, FALSE);
   }
 }
}

void do_wish( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MSL];
    OBJ_DATA *obj;
    int chance;
    int qpcost = 5000;
    int minimum = ch->max_hit / 2;
    one_argument( argument, arg );

    
    if ( arg[0] == '\0' )
    {
	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
	    if ( obj->item_type == ITEM_WELL )
		break;
	}

	if ( obj == NULL )
	{
	    send_to_char( "There is no wishing well here to wish upon!\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

   if (ch->lstatus < 1)
   {
    stc("Only those who are legendary can use the benefit of the wishing wells.\n\r",ch);
    return;
   }

   if (ch->wish_timer > 0)
   {
    stc("You have not recovered from your last drink of the wishing well.\n\r",ch);
    return;
   }
   if (ch->fight_timer < 1)
   {
    send_to_char("Not without a fighttimer.\n\r",ch);
    return;
   }
   if (ch->pcdata->quest < qpcost)
   {
    sprintf(buf,"JANK E. POO requires %d qps as payment.\n\r",qpcost);
    stc(buf,ch);
    return;
   }
   if (ch->hit >= minimum)
   {
        send_to_char("Your current hitpoints are too high to benefit!\n\r",ch);
        return;
   }
   if (ch->hit < 0)
   {
        send_to_char("You foolish person, your wishes get you nothing!\n\r",ch);
        return;
   }


    switch ( obj->item_type )
    {
    default:
	send_to_char( "You can't wish there.\n\r", ch );
	break;

    case ITEM_WELL:
   chance = number_range(1,100);
   if (chance > 50)
  {
         send_to_char("Your prayers to JANK E. POO go unanswered.\n\r",ch);
         WAIT_STATE(ch, 4);
         return;
  }
  else if (ch->hit < minimum)
  {
  ch->hit  = minimum;
  if (ch->mana < minimum)
  ch->mana = minimum;
  if (ch->move < minimum)
  ch->move = minimum;
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  if (ch->mana > ch->max_mana) ch->mana = ch->max_mana;
  if (ch->move > ch->max_move) ch->move = ch->max_move;
  ch->loc_hp[0] = 0;
  ch->loc_hp[1] = 0;
  ch->loc_hp[2] = 0;
  ch->loc_hp[3] = 0;
  ch->loc_hp[4] = 0;
  ch->loc_hp[5] = 0;
  ch->loc_hp[6] = 0;
  ch->wish_timer += 10 + ch->race;
  sprintf(buf,"JANK E. POO heals your wounds and takes %d qps as payment.\n\r",qpcost);
  stc(buf,ch);

  sprintf(buf,"#P%s#0 has taken a #Cdelicious drink#0 from a #Cwishing well#0.",ch->pcdata->switchname);
  do_update(NULL,buf);

  WAIT_STATE(ch, 30);
  ch->fight_timer += 10;
  ch->pcdata->quest -= qpcost;
  return;
  }
  break;
	}
}
void do_pkportal2 (CHAR_DATA *ch, char *argument)
{  
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *location;

  one_argument (argument, arg);
    
  if (IS_NPC(ch)) return;
  if (ch->move < 10000)
  {
    send_to_char("You lack the vitality to summon the wormhole.\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Worm your way where?\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->fight_timer == 0)
  {
    send_to_char("They are not engaged in pk.\n\r",ch);
    return;
  }
  if (ch->race <= 0)
  {
    stc("You need at least 1 status to wormhole.\n\r",ch);
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
    stc( "The arena prevents you from doing this.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
  {
    stc( "The arena prevents you from doing this.\n\r",ch);
    return;
  }
  if (ch->move < victim->move)
  {
    stc( "You lack the vitality to enter the wormhole.\n\r",ch);
    return;
  }
  location = victim->in_room;
  act("You step onto the wormhole.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n steps into thin air.", ch, NULL, NULL, TO_ROOM, FALSE);
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  act("You leave the wormhole.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n steps out from behind you.", ch, NULL, NULL, TO_ROOM, FALSE);
  ch->move -= 10000;
  WAIT_STATE(ch, 4);
  return;  
}
void gainarti( CHAR_DATA *ch)
{
  char buf[MSL];
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  int vnum = 0;
  bool hasarti;

  if (IS_NPC(ch)) return;
  if (!IS_NPC(ch))
  {
   vnum = number_range(33960, 33987);
   if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
   {
     send_to_char("Missing artifact, please inform Rand .\n\r",ch);
     return;
   }
   hasarti = FALSE;
   for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
      obj_next = obj->next_content;
      if (IS_SET(obj->quest, QUEST_ARTIFACT))
     {   hasarti = TRUE; break; }

    }
  
   if (hasarti == TRUE) {stc("You cannot have 2 artis.\n\r", ch);}
   else if (hasarti == FALSE)
   {
   obj = create_object(pObjIndex, 50);
   obj_to_char(obj, ch);
   sprintf(buf, "#y%s#0 has been granted %s.#n",ch->pcdata->switchname,obj->short_descr);
   do_update(ch, buf);
   return;
   }
  }
}
void do_shatter( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA  *trial;
    char arg[MIL];
    char buf[MSL];	
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( arg[0] == '\0' )
    {
        send_to_char( "Shatter what?\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
       
        if ( obj->item_type != ITEM_TREASURE )
	{
	stc("You cannot shatter this.\n\r",ch);
	return;
	}
	if ( obj->item_type == ITEM_TREASURE )
        {
 	   if (obj->pIndexData->vnum  != 420002)
           {
	send_to_char("You cannot shatter them, they are not in a #RKin#rdr#Red #0Sp#8ir#0it #RJe#rw#Rel#n\n\r",ch);
	return;
           }
           else if (obj->pIndexData->vnum == 420002)
	    {
	     trial = get_eq_char( ch, WEAR_WIELD );
	     if (trial != NULL && ((trial->pIndexData->vnum == 420003) || 
(trial->pIndexData->vnum == 3012)))
	      {
              stc("You shatter the #RKin#rdr#Red #0Sp#8ir#0it #RJe#rw#Rel#n!\n\r",ch);
	      sprintf(buf, "#7%s#0 has shattered a #RKin#rdr#Red #0Sp#8ir#0it #RJe#rw#Rel#0!#n\n\r",ch->name);
	      do_info(NULL,buf);
              extract_obj(obj);
	      return;
	      }
     	     else if (trial != NULL && ((trial->pIndexData->vnum != 420003) || 
(trial->pIndexData->vnum != 3012)))
      	      {
        send_to_char("You must be wielding the proper tool to shatter the #RKin#rdr#Red #0Sp#8ir#0it #RJe#rw#Rel#n\n\r",ch);
	return;
      	      }
	    }
	}
    }
    return;
}
void do_checkdamcap(CHAR_DATA *ch, char *argument)
{
  char buf[MSL];
  DESCRIPTOR_DATA *d;
  stc("#l--==#L**#l==--==#L**#l==    #7Damcap Status    #l==#L**#l==--==#L**#l==--#n\n\r",ch);
  for (d = first_descriptor; d != NULL; d = d->next)
  {
       if (!d->connected == CON_PLAYING) continue;
       if (d->character == ch) continue;
       if (d->character->level > 6) continue;
       sprintf(buf, "%-12s has %-6d damcap with: %-3d status, %d legend, %-8d primal and %d generation.\n\r",
d->character->pcdata->switchname,d->character->damcap[DAM_CAP],d->character->race,d->character->lstatus,d->character->practice,d->character->generation);
       stc(buf, ch);
  }
}

void do_checkbalance(CHAR_DATA *ch, char *argument)
{
  char buf[MSL];
  DESCRIPTOR_DATA *d;
  stc("#l--==#L**#l==--==#L**#l==    #7Account Status    #l==#L**#l==--==#L**#l==--#n\n\r",ch);
  for (d = first_descriptor; d != NULL; d = d->next)
  {
       if (!d->connected == CON_PLAYING) continue;
       if (d->character == ch) continue;
       if (d->character->level > 6) continue;
       sprintf(buf, "%s has %2.0f gold and %2.0f in the bank.\n\r",
d->character->name,d->character->gold,d->character->pcdata->bank);
       stc(buf, ch);
  }
}

void do_buyqp( CHAR_DATA *ch, char *argument )
 {
    char arg1[MIL];
    char buf[MSL];
    char log[MSL];
    int gold;
    int qpearn;
    argument = one_argument( argument, arg1 );

   if (IS_NPC(ch)) return;
   if (!IS_NPC(ch))
    {  
        if (!is_number(arg1) && strcmp(arg1,"all"))
        {
            send_to_char("Please enter a numeric value, or all.\n\r",ch);
            return;
        }
     
   if (is_number(arg1))
        {
         gold = atoi( arg1 );
         if ((gold < 10000) || (gold > 1000000000))
         {
            send_to_char("Please, between 10000 and 1000000000 gold.\n\r",ch);
            return;
         }
        }
    else if  (!is_number(arg1) && !strcmp(arg1,"all"))
    {
	gold = ch->gold;
    }
    if (gold < 10000)
    {
     stc("You do not have enough gold to benefit from this. (minimum 10000)\n\r",ch);
     return;
    }
    if (gold > ch->gold)
    {
     send_to_char("You do not have that much gold, sir.\n\r",ch);
     return;
    }
     if (gold <= ch->gold) 
    {
        qpearn = gold/10000;
	ch->pcdata->quest += qpearn;
        ch->pcdata->questtotal += qpearn;
	ch->pcdata->questsrun += 1;
        ch->pcdata->score[SCORE_QUEST] += qpearn;
	ch->gold -= gold;
	sprintf(buf,"You spend %d gold on %d qp.\n\r",gold,qpearn);
	sprintf(log,"%s spends %d gold on %d qp.",ch->name,gold,qpearn);
	stc(buf,ch);
	log_string(log);
	return;
    }
 }
}
void do_resetchar( CHAR_DATA *ch, char *argument )
{
CHAR_DATA *victim;
char arg[MIL];
char log_buffer[MSL];
argument = one_argument(argument, arg);
if (IS_NPC(ch)) return;
if (!IS_NPC(ch))
{
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
        do_remove(victim,"all");
        victim->pcdata->kingdom = 0;
        victim->pcdata->bounty = 0;
        victim->race = 0;
        victim->practice = 0;
        victim->max_hit = 2000;
        victim->hit = 2000;
        victim->max_mana = 2000;
        victim->mana = 2000;
        victim->max_move = 2000;
        victim->move = 2000;
        victim->generation = 7;
        victim->pcdata->quest = 0;
        victim->pcdata->score[SCORE_TOTAL_XP] = 0;
        victim->pcdata->score[SCORE_TOTAL_LEVEL] = 0;
        victim->pcdata->score[SCORE_QUEST] = 0;
        victim->pcdata->score[SCORE_HIGH_XP] = 0;
        victim->pcdata->score[SCORE_HIGH_LEVEL] = 0;
        victim->pcdata->score[SCORE_NUM_QUEST] = 0;
        if (victim->wpn[0] > 200) victim->wpn[0] = 200;
        if (victim->wpn[1] > 200) victim->wpn[1] = 200;
        if (victim->wpn[2] > 200) victim->wpn[2] = 200;
        if (victim->wpn[3] > 200) victim->wpn[3] = 200;
        if (victim->wpn[4] > 200) victim->wpn[4] = 200;
        if (victim->wpn[5] > 200) victim->wpn[5] = 200;
        if (victim->wpn[6] > 200) victim->wpn[6] = 200;
        if (victim->wpn[7] > 200) victim->wpn[7] = 200;
        if (victim->wpn[8] > 200) victim->wpn[8] = 200;
        if (victim->wpn[9] > 200) victim->wpn[9] = 200;
        if (victim->wpn[10] > 200) victim->wpn[10] = 200;
        if (victim->wpn[11] > 200) victim->wpn[11] = 200;
        if (victim->wpn[12] > 200) victim->wpn[12] = 200;
        if (victim->spl[RED_MAGIC] > 200) victim->spl[RED_MAGIC] = 200;
        if (victim->spl[GREEN_MAGIC] > 200) victim->spl[GREEN_MAGIC] = 200;
        if (victim->spl[YELLOW_MAGIC] > 200) victim->spl[YELLOW_MAGIC] = 200;
        if (victim->spl[BLUE_MAGIC] > 200) victim->spl[BLUE_MAGIC] = 200;
        if (victim->spl[PURPLE_MAGIC] > 200) victim->spl[PURPLE_MAGIC] = 200;
        victim->pkill = 0;
        victim->mkill = 200;
        victim->pdeath = 0;
        victim->mdeath = 0;
        victim->played = 1;
        victim->pcdata->awins = 0;
        victim->pcdata->alosses = 0;
        victim->pcdata->questsrun = 0;
        victim->pcdata->questtotal = 0;
        victim->pcdata->wolf = 1;
        victim->exp = 0;
        victim->pcdata->souls = 0;
        char_from_room(victim);
        char_to_room(victim,get_room_index(41000));
        sprintf( log_buffer, "%s has been reset!",victim->name);
        log_string(log_buffer);
        do_save(victim,"");
	return; 
    }
}  
void do_buystatus( CHAR_DATA *ch, char *argument )
    {
    char arg1[MIL];
    char buf[MSL];
    int try;

    argument = one_argument( argument, arg1 );

      if (IS_NPC(ch)) return;
	  if (!IS_NPC(ch))
	  {
	    if (!str_cmp(arg1, "on"))
		{
		if (ch->fight_timer > 0)
		{
		send_to_char("You cannot turn on double status with fight timer.\n\r",ch);
		return;
		}
	        if (ch->pcdata->quest < 5000)
		    {
			send_to_char("You do not have enough questpoints to buy double status.\n\r",ch);
			return;
			}
			else if (ch->pcdata->quest >= 5000)
			{
			try = 0;
				if (global_status)
				{
				send_to_char("Double status is already in affect!\n\r",ch);
				return;
				}
				else if (!global_status)
				{
					if (ch->buystatus < 1)
					{
					send_to_char("Your questing has paid off, the gods are happy, and grant you double status!\n\r",ch);
					global_status = TRUE;
					sprintf(buf, "%s has earned the populace double status for every decapitation!\n\r",ch->name);
					do_info(ch, buf);
					ch->pcdata->quest -= 5000;
					try++;
					ch->buystatus = try;
					return;
					}
					else if (ch->buystatus >= 1)
					{
					send_to_char("You have already purchased double status, you need to wait.\n\r",ch);
					return;
					}
				}
			}
		}
	if (!str_cmp(arg1, "off"))
    {
                if (ch->fight_timer > 0)
                {
                send_to_char("You cannot turn off double status with fight timer.\n\r",ch);
                return;
                }

        else if (!global_status)
        {
            send_to_char("How can you turn something off, if it is not turned on?\n\r",ch);
            return;
        }
		else if (global_status)
		{
			if (ch->pcdata->quest < 2000)
			{
			send_to_char("You do not have the required 2000 questpoints to remove the double status.\n\r",ch);
			return;
			}
			else if (ch->pcdata->quest >= 2000)
			{
			global_status = FALSE;
	    	        sprintf(buf, "%s has paid in questpoints so that none will be exploited by double status.\n\r", ch->name );
			do_info( ch, buf );
			send_to_char( "The double status flag is now off!\n\r", ch );
			ch->pcdata->quest -= 2000;
			return;
			}
		}
	}
return;
}
}

void do_kingdomeq( CHAR_DATA *ch, char *argument )
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  char arg[MAX_INPUT_LENGTH];
  int vnum = 0;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

        if (!IS_NPC(ch) && ch->pcdata->kingdom == 0)
	{	
          send_to_char("You are not in a kingdom, how do you propose to make kingdom eq?\n\r",ch);
          return;
        }
  else if (!IS_NPC(ch) && ch->pcdata->kingdom != 0)
  {
  if (arg[0] == '\0')
  {
    send_to_char("Please specify which piece of rune armor you wish to make:\n\rRing Collar Plate Helmet Leggings Boots Gloves Sleeves Cape Belt Bracer Mask\n\rslicer stabber slasher whipper claw blaster pounder crusher grepper biter piercer\n\rsucker unarmed klaive\n\r",ch);
    return;
  }
  if ( ch->pcdata->quest < 10000 )
  {
    send_to_char("You don't have the 10K qps needed.\n\r",ch);
    return;
  }
  if      (!str_cmp(arg,"ring"        )) vnum = 869;
  else if (!str_cmp(arg,"collar"      )) vnum = 870;
  else if (!str_cmp(arg,"plate"       )) vnum = 871;
  else if (!str_cmp(arg,"helmet"      )) vnum = 872;
  else if (!str_cmp(arg,"leggings"    )) vnum = 873;
  else if (!str_cmp(arg,"boots"       )) vnum = 874;
  else if (!str_cmp(arg,"gloves"      )) vnum = 875;
  else if (!str_cmp(arg,"sleeves"     )) vnum = 880;
  else if (!str_cmp(arg,"cape"        )) vnum = 879;
  else if (!str_cmp(arg,"belt"        )) vnum = 876;
  else if (!str_cmp(arg,"bracer"      )) vnum = 877;
  else if (!str_cmp(arg,"mask"        )) vnum = 878;
  else if (!str_cmp(arg,"slicer"   )) vnum = 881;
  else if (!str_cmp(arg,"stabber"  )) vnum = 882;
  else if (!str_cmp(arg,"slasher"      )) vnum = 883;
  else if (!str_cmp(arg,"whipper"      )) vnum = 884;
  else if (!str_cmp(arg,"claw"        )) vnum = 885;
  else if (!str_cmp(arg,"blaster"        )) vnum = 886;
  else if (!str_cmp(arg,"pounder"      )) vnum = 887;
  else if (!str_cmp(arg,"crusher"    )) vnum = 888;
  else if (!str_cmp(arg,"grepper"       )) vnum = 889;
  else if (!str_cmp(arg,"biter"        )) vnum = 890;
  else if (!str_cmp(arg,"piercer"     )) vnum = 891;
  else if (!str_cmp(arg,"sucker"        )) vnum = 892;
  else if (!str_cmp(arg,"unarmed"       )) vnum = 893;
  else if (!str_cmp(arg,"klaive"      )) vnum = 867;
  else
  {
    do_kingdomeq(ch,"");
    return;
  }
  if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
  {
    send_to_char("Missing object, please inform Wobinz .\n\r",ch);
    return;
  }
  if(!IS_IMMORTAL(ch)) ch->pcdata->quest -= 10000;
  obj = create_object(pObjIndex, 50);
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  act("You transform the 10k qp into $p.",ch,obj,NULL,TO_CHAR, FALSE);
  act("$n forms $p out of 10K qp.",ch,obj,NULL,TO_ROOM, FALSE);
  return;
  }
}


void do_assassinate( CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
   AFFECT_DATA af;
	CHAR_DATA *victim;

//	int sn;
	int dam1;
    int x;    
    argument = one_argument( argument, arg );

    if( IS_NPC(ch) )  return;

    if(!IS_CLASS(ch,CLASS_NINJA))
    {
     send_to_char("Huh?\n\r",ch);
     return;
    }
    
	if (IS_CLASS(ch,CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] < 2)
    {
     send_to_char("You need Ningenno level 2 before you can Assassinate.\n\r",ch);
     return;
    }
	if ( arg[0] == '\0' )
    {
     send_to_char( "Assassinate whom?\n\r", ch );
     return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
     send_to_char( "They aren't here.\n\r", ch );
     return;
    }

    if ( victim == ch )
    {
     send_to_char( "How can you assassinate yourself?\n\r", ch );
     return;
    }

    if (is_safe(ch,victim) == TRUE) return;

    if ( victim->fighting != NULL )
    {
     send_to_char( "You can't assassinate a fighting person.\n\r", ch);
     return;
    }
    if (victim->fight_timer > 0)
    {
    send_to_char("They are too alert to be assassinated.\n\r",ch);
    return;
    }
    if ( victim->hit < victim->max_hit )
    {
     act( "$N is hurt and suspicious ... you can't sneak up.",  ch, NULL, victim, TO_CHAR, FALSE );
     return;
    }

	if (!xIS_SET(ch->act, PLR_HIDE) )
    {
     send_to_char("You must be hidden to assassinate!\n\r",ch);
     return;
    }
    if (IS_NPC(victim)) {stc("You cannot assassinate a mob.\n\r",ch);return;}
    if( !IS_NPC(victim) )
    {
    x = number_range(10,25);	
    dam1 = char_damroll(ch) * x;    
    sprintf(buf,"#7%s #0has been assassinated by #G%s#n.#0",victim->name,ch->name);
    do_update(NULL,buf);
    ch->fighting = victim;

    if (dam1 > 150000) dam1 = 150000;
    else dam1 = dam1;
  
    act("$n steps out of the shadows behind $N.",ch,NULL,victim,TO_NOTVICT, FALSE);
    act("You step out of the darkness, right behind $N.",ch,NULL,victim,TO_CHAR, FALSE);
    act("$n steps out of the shadows behind you.",ch,NULL,victim,TO_VICT, FALSE);

    if (xIS_SET(victim->act,PLR_HOLYLIGHT)) xREMOVE_BIT(victim->act,PLR_HOLYLIGHT);
    if (IS_SET(victim->affected_by, AFF_DETECT_HIDDEN)) REMOVE_BIT(victim->affected_by, AFF_DETECT_HIDDEN);
    if (IS_SET(victim->affected_by, AFF_DETECT_INVIS)) REMOVE_BIT(victim->affected_by, AFF_DETECT_INVIS);
    if ( xIS_SET(ch->act, PLR_HIDE) ) xREMOVE_BIT(ch->act, PLR_HIDE);

      af.type      = skill_lookup("blindness");
      af.location  = APPLY_HITROLL;
      af.modifier  = -1000;
      af.duration  = 60;
      af.bitvector = AFF_BLIND;
      affect_to_char( victim, &af );

      af.type      = skill_lookup("web");
      af.location  = APPLY_AC;
      af.modifier  = 200;
      af.duration  = number_range(30,60);
      af.bitvector = AFF_WEBBED;
      affect_to_char( victim, &af );

    send_to_char( "Your eyes tear up from the immense pain!\n\r", victim);

	sprintf(buf,"Your assassination hits $N incredibly hard! [%d]",dam1);
    act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    sprintf(buf,"$n's assassination hits you incredibly hard! [%d]\n\r",dam1);
    act(buf,ch,NULL,victim,TO_VICT, FALSE);
    sprintf(buf,"$n's assassination hits $N incredibly hard! [%d]\n\r",dam1);
    act(buf,ch,NULL,victim,TO_NOTVICT, FALSE);
   hurt_person(ch,victim,dam1);
   set_fighting(ch,victim);

   update_pos(ch);
   update_pos(victim);
   ch->fight_timer += 20;
   victim->fight_timer += 10;
   WAIT_STATE(ch, 25);
   return;
   }
}
	
void do_meta( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
	int qp = 0;
	int qp1 = 0;
	double exp = 0;
        int amount = 0;
        int urin = 0;
	int modifier;
        int urin_counter = 0;
	double gold;
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	if (IS_NPC(ch)) return;
	
	if (!IS_NPC(ch))
	{
	if (str_cmp(arg1,"hp") && str_cmp(arg1,"power"))
	{
	sprintf(buf, "Increase arguments: hp power\n\r");
	stc(buf, ch);
	return;
	}
	if (!str_cmp(arg1,"hp"))
    {
       if (!is_number(arg2))
        {
            send_to_char("Please enter a numeric value.\n\r",ch);
            return;
        }
        if (is_number(arg2))
        {
         amount = atoi( arg2 );
         if ((amount < 1) || (amount > 1000))
         {
            send_to_char("Please enter a value between 1 and 1000.\n\r",ch);
            return;
         }
        }                
	if (ch->max_hit < 300000)
         {
                sprintf(buf, "You do not have enough hp to begin increasing!\n\r");
                stc(buf, ch);
                return;
         }
        if (ch->max_hit > 400000)
                {
                        sprintf(buf, "You cannot increase beyond 400000 hp.\n\r");
                        stc(buf, ch);
                        return;
                }
                if ((ch->max_hit >= 300000) && (ch->max_hit <= 320000))
                modifier = (ch->max_hit * 85);
                else if ((ch->max_hit > 320000) && (ch->max_hit <= 340000))
                modifier = (ch->max_hit * 95);
                else if ((ch->max_hit > 340000) && (ch->max_hit <= 360000))
                modifier = (ch->max_hit * 105);
                else if ((ch->max_hit > 360000) && (ch->max_hit <= 380000))
                modifier = (ch->max_hit * 115);
                else if ((ch->max_hit > 380000) && (ch->max_hit < 400000))
                modifier = (ch->max_hit * 125);

                if (ch->exp < modifier)
                {
                        sprintf(buf, "You do not have enough exp to increase beyond %d hp.\n\r",
ch->max_hit);
                        stc(buf, ch);
                        return;
                }
                if (ch->pcdata->quest < (ch->max_hit / 20000))
                {
                        qp = ch->max_hit / 20000;
                        sprintf(buf, "You do not have the %d qp to increase beyond %d hp.\n\r", 
qp, ch->max_hit);
                        stc(buf, ch);
                        return;
                }
		if (ch->gold < modifier)
		{
			gold = modifier/100;
			sprintf(buf, "You do not have enough gold to increase beyond %d hp.\n\r",
ch->max_hit);
			stc(buf, ch);
			return;
	 	}

        for (urin = 0; urin < amount ; urin++)
        {

		if ((ch->max_hit >= 300000) && (ch->max_hit <= 320000) && (ch->exp >= 
modifier) && (ch->pcdata->quest >= (ch->max_hit / 20000))
&& (ch->gold >= modifier))
		{
                        qp = ch->max_hit / 20000;
                        exp += modifier;
			gold += (modifier/100);
			ch->gold -= (modifier/100);
                        ch->exp -= modifier;
                        ch->pcdata->quest -= qp;;
                        ch->max_hit+=1;
                        urin_counter+=1;
                        qp1 += qp;
		}
                else if ((ch->max_hit > 320000) && (ch->max_hit <= 340000) && (ch->exp >=
modifier) && (ch->pcdata->quest >= (ch->max_hit / 20000)) && (ch->gold >= modifier))
                {
                        qp = ch->max_hit / 20000;
                        exp += modifier;
                        gold += (modifier/100);
                        ch->gold -= (modifier/100);
                        ch->exp -= modifier;
                        ch->pcdata->quest -= qp;;
                        ch->max_hit+=1;
                        urin_counter+=1;
                        qp1 += qp;
		}
                else if ((ch->max_hit > 340000) && (ch->max_hit <= 360000) && (ch->exp >=
modifier) && (ch->pcdata->quest >= (ch->max_hit / 20000)) && (ch->gold >= modifier))
                {
                        qp = ch->max_hit / 20000;
                        exp += modifier;
                        gold += (modifier/100);
                        ch->gold -= (modifier/100);
                        ch->exp -= modifier;
                        ch->pcdata->quest -= qp;;
                        ch->max_hit+=1;
                        urin_counter+=1;
                        qp1 += qp;
                }
                else if ((ch->max_hit > 360000) && (ch->max_hit <= 380000) && (ch->exp >=
modifier) && (ch->pcdata->quest >= (ch->max_hit / 20000)) && (ch->gold >= modifier))
                {
                        qp = ch->max_hit / 20000;
                        exp += modifier;
                        gold += (modifier/100);
                        ch->gold -= (modifier/100);
                        ch->exp -= modifier;
                        ch->pcdata->quest -= qp;;
                        ch->max_hit+=1;
                        urin_counter+=1;
                        qp1 += qp;
                }
                else if ((ch->max_hit > 380000) && (ch->max_hit < 400000) && (ch->exp >= 
modifier) && (ch->pcdata->quest >= (ch->max_hit / 20000)) && (ch->gold >= modifier))
                {
                        qp = ch->max_hit / 20000;
                        exp += modifier;
                        gold += (modifier/100);
                        ch->gold -= (modifier/100);
                        ch->exp -= modifier;
                        ch->pcdata->quest -= qp;;
                        ch->max_hit+=1;
                        urin_counter+=1;
                        qp1 += qp;
                }


	}
        if (urin_counter==0)
        {
                send_to_char( "You cannot increase.\n\r", ch );
        }
        if (urin_counter==1)
        {
                sprintf(buf, "You gain 1 hp at a cost of: %2.0f gold, %d qp, and %2.0f exp.\n\r", gold, qp1, exp );
		stc(buf,ch);
        }
        if (urin_counter>1)
        {
                sprintf( buf, "You gain %d hps at a cost of: %2.0f gold, %d qp and %2.0f exp.\n\r", urin_counter,gold,  qp1, exp );
                send_to_char( buf, ch );
        }
    }
  else if (!str_cmp(arg1,"power"))
  {
       if (!is_number(arg2))
        {
            send_to_char("Please enter a numeric value.\n\r",ch);
            return;
        }
        if (is_number(arg2))
        {
         amount = atoi( arg2 );
         if ((amount < 1) || (amount > 1000))
         {
            send_to_char("Please enter a value between 1 and 1000.\n\r",ch);
            return;
         }
        }

   if (ch->practice < 20000)
   {
    send_to_char( "You do not have enough practice.\n\r",ch);
    return;
   }
   else if (ch->practice >= 100000)
   {   
    send_to_char("You have accomplished all you can.\n\r",ch);
    return;
   }
   else if (ch->practice >= 20000)
   {
    if (ch->practice >= 20000 && ch->practice < 40000)	modifier = 1000;
    else if (ch->practice >= 40000 && ch->practice < 60000) modifier = 800;
    else if (ch->practice >= 60000 && ch->practice < 80000) modifier = 700;
    else if (ch->practice >= 80000 && ch->practice < 100000) modifier = 600;
            if (ch->exp < ((ch->practice / modifier)*50000))
                {
			exp = (ch->practice / modifier)*50000;
                        sprintf(buf, "You do not have the %2.0f exp to increase beyond %d primal.\n\r",exp,ch->practice);
                        stc(buf, ch);
                        return;
                }
                if (ch->pcdata->quest < (ch->practice /modifier))
                {
                        qp = (ch->practice / modifier);           
			sprintf(buf, "You do not have the %d qp to increase beyond %d primal.\n\r",qp, ch->practice);
                        stc(buf, ch);
                        return;
                }
                if (ch->gold < (ch->practice / modifier))
                {
                        gold = (ch->practice / modifier);
                        sprintf(buf, "You do not have enough gold to increase beyond %d primal.\n\r",ch->practice);
                        stc(buf, ch);
                        return;
                }

    else if (ch->exp >= (ch->practice / modifier))
    {
     for (urin = 0; urin < amount ; urin++)
      {
	if (ch->practice >= 20000 && ch->practice < 40000 && 
(ch->exp > (ch->practice / modifier)*50000) &&
(ch->pcdata->quest > (ch->practice / modifier)) &&
(ch->gold > (ch->practice / modifier)))
	{
	exp += (ch->practice / modifier)*50000;
	gold += (ch->practice / modifier);
	qp += (ch->practice / modifier);
	ch->gold -= (ch->practice / modifier);
	ch->exp -= (ch->practice / modifier)*50000;
	ch->pcdata->quest -= (ch->practice / modifier);
	ch->practice+=1;
	urin_counter+=1;
	}
	else if (ch->practice >= 40000 && ch->practice < 60000 &&
(ch->exp > (ch->practice / modifier)*50000) &&
(ch->pcdata->quest > (ch->practice / modifier)) &&
(ch->gold > (ch->practice / modifier)))
        {
        exp += (ch->practice / modifier)*50000;
        gold += (ch->practice / modifier);
        qp += (ch->practice / modifier);
        ch->gold -= (ch->practice / modifier);
        ch->exp -= (ch->practice / modifier)*50000;
        ch->pcdata->quest -= (ch->practice / modifier);
        ch->practice+=1;
        urin_counter+=1;
        }
	else if (ch->practice >= 60000 && ch->practice < 80000 &&
(ch->exp > (ch->practice / modifier)*50000) &&
(ch->pcdata->quest > (ch->practice / modifier)) &&
(ch->gold > (ch->practice / modifier)))
        {
        exp += (ch->practice / modifier)*50000;
        gold += (ch->practice / modifier);
        qp += (ch->practice / modifier);
        ch->gold -= (ch->practice / modifier);
        ch->exp -= (ch->practice / modifier)*50000;
        ch->pcdata->quest -= (ch->practice / modifier)/10;
        ch->practice+=1;
        urin_counter+=1;
        }
	else if (ch->practice >= 80000 && ch->practice < 100000 && 
(ch->exp > (ch->practice / modifier)*50000) &&
(ch->pcdata->quest > (ch->practice / modifier)) &&
(ch->gold > (ch->practice / modifier)))
        {
        exp += (ch->practice / modifier)*50000;
        gold += (ch->practice / modifier);
        qp += (ch->practice / modifier);
        ch->gold -= (ch->practice / modifier);
        ch->exp -= (ch->practice / modifier)*50000;
        ch->pcdata->quest -= (ch->practice / modifier);
        ch->practice+=1;
        urin_counter+=1;
        }

      }
     if (urin_counter == 0)
     {
	stc("You cannot increase your training.\n\r",ch);
	return;
     }
     else if (urin_counter==1)
        {
                sprintf(buf, "You gain 1 primal at a cost of: %2.0f gold, %d qp, and %2.0f exp.\n\r", gold, qp, exp );
                stc(buf,ch);
        }
     else if (urin_counter>1)
        {
                sprintf( buf, "You gain %d primal at a cost of: %2.0f gold, %d qp and %2.0f exp.\n\r", urin_counter,gold,  qp, exp );
                send_to_char( buf, ch );
        }

   }
}
}
			}
return;
    }

void do_wallofswords( CHAR_DATA *ch,char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch,CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( IS_CLASS(ch, CLASS_HIGHLANDER) && !IS_SET(ch->newbits, NEW_BLADESPIN))
  {
    send_to_char("You need to be a True Samurai.\n\r",ch);
    return;
  }
  if (!IS_SET(ch->newbits, NEW_JAWLOCK))
  {
    send_to_char("Your blade will take no quarter.\n\r",ch);
    SET_BIT(ch->newbits, NEW_JAWLOCK);
    return;
  }
  else if (IS_SET(ch->newbits, NEW_JAWLOCK))
  {
    send_to_char("Your blade will allow quarter.\n\r",ch);
    REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
    return;
  }
}


void load_classleader()
{
  FILE *fp;

  if ((fp = fopen("../area/class_leader.txt", "r")) == NULL)
  {   
    log_string("Error: class_leader.txt not found!");
    return;
  }
  class_leader.best_demon_name = fread_string(fp);  
  class_leader.best_demon_number = fread_number(fp);
  class_leader.best_mage_name = fread_string(fp);
  class_leader.best_mage_number = fread_number(fp);
  class_leader.best_werewolf_name = fread_string(fp);
  class_leader.best_werewolf_number = fread_number(fp);
  class_leader.best_vampire_name = fread_string(fp);
  class_leader.best_vampire_number = fread_number(fp);
  class_leader.best_samurai_name = fread_string(fp);
  class_leader.best_samurai_number = fread_number(fp);
  class_leader.best_drow_name = fread_string(fp);
  class_leader.best_drow_number = fread_number(fp);
  class_leader.best_monk_name = fread_string(fp);
  class_leader.best_monk_number = fread_number(fp);
  class_leader.best_ninja_name = fread_string(fp);
  class_leader.best_ninja_number = fread_number(fp);
  class_leader.best_lich_name = fread_string(fp);
  class_leader.best_lich_number = fread_number(fp);
  class_leader.best_shapeshifter_name = fread_string(fp);
  class_leader.best_shapeshifter_number = fread_number(fp);
  class_leader.best_tanarri_name = fread_string(fp);
  class_leader.best_tanarri_number = fread_number(fp);
  class_leader.best_angel_name = fread_string(fp);
  class_leader.best_angel_number = fread_number(fp);
  class_leader.best_undeadknight_name = fread_string(fp);
  class_leader.best_undeadknight_number = fread_number(fp);
  class_leader.best_cyborg_name = fread_string(fp);
  class_leader.best_cyborg_number = fread_number(fp);
  class_leader.best_paladin_name = fread_string(fp);
  class_leader.best_paladin_number = fread_number(fp);
  class_leader.best_psionist_name = fread_string(fp);
  class_leader.best_psionist_number = fread_number(fp);
  fclose(fp);
}

void save_classleader()
{
  FILE *fp;

  if ((fp = fopen("../area/class_leader.txt", "w")) == NULL)
  {   
    log_string("Error: class_leader.txt not found!");
    return;
  }
  fprintf(fp, "%s~\n", class_leader.best_demon_name);
  fprintf(fp, "%d\n", class_leader.best_demon_number);
  fprintf(fp, "%s~\n", class_leader.best_mage_name);
  fprintf(fp, "%d\n", class_leader.best_mage_number);
  fprintf(fp, "%s~\n", class_leader.best_werewolf_name);
  fprintf(fp, "%d\n", class_leader.best_werewolf_number);
  fprintf(fp, "%s~\n", class_leader.best_vampire_name);
  fprintf(fp, "%d\n", class_leader.best_vampire_number);
  fprintf(fp, "%s~\n", class_leader.best_samurai_name);
  fprintf(fp, "%d\n", class_leader.best_samurai_number);
  fprintf(fp, "%s~\n", class_leader.best_drow_name);
  fprintf(fp, "%d\n", class_leader.best_drow_number);
  fprintf(fp, "%s~\n", class_leader.best_monk_name);
  fprintf(fp, "%d\n", class_leader.best_monk_number);
  fprintf(fp, "%s~\n", class_leader.best_ninja_name);
  fprintf(fp, "%d\n", class_leader.best_ninja_number);
  fprintf(fp, "%s~\n", class_leader.best_lich_name);
  fprintf(fp, "%d\n", class_leader.best_lich_number);
  fprintf(fp, "%s~\n", class_leader.best_shapeshifter_name);
  fprintf(fp, "%d\n", class_leader.best_shapeshifter_number);
  fprintf(fp, "%s~\n", class_leader.best_tanarri_name);
  fprintf(fp, "%d\n", class_leader.best_tanarri_number);
  fprintf(fp, "%s~\n", class_leader.best_angel_name);
  fprintf(fp, "%d\n", class_leader.best_angel_number);
  fprintf(fp, "%s~\n", class_leader.best_undeadknight_name);
  fprintf(fp, "%d\n", class_leader.best_undeadknight_number);
  fprintf(fp, "%s~\n", class_leader.best_cyborg_name);
  fprintf(fp, "%d\n", class_leader.best_cyborg_number);
  fprintf(fp, "%s~\n", class_leader.best_paladin_name);
  fprintf(fp, "%d\n", class_leader.best_paladin_number);
  fprintf(fp, "%s~\n", class_leader.best_psionist_name);
  fprintf(fp, "%d\n", class_leader.best_psionist_number);
  fclose (fp);
}

void do_classleader( CHAR_DATA *ch, char *argument )
{    
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  stc("\n\r",ch);
  
stc("#0(<#0*#G>#0-#G<#0-#G<#0-#G<#0*#G>#0-#G<#0-#G<#0-#G<#0*#G>#0-#G<#0-#G<#0-#G<#0*#G>#0) #GClass Leaders #0(#G<#0*#G>#0-#G>#0-#G>#0-#G<#0*#G>#0-#G>#0-#G>#0-#G<#0*#G>#0-#G>#0-#G>#0-#G<#0*#G>#0)#n\n\r",ch); 
  stc("\n\r",ch);
  stc("     #GM#ga#os#yt#Ge#gr #0Demon         #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_demon_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_demon_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Mage          #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_mage_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_mage_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Werewolf      #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_werewolf_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_werewolf_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Vampire       #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_vampire_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_vampire_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Samurai       #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_samurai_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_samurai_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Drow          #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_drow_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_drow_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Monk          #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_monk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_monk_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Ninja         #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_ninja_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_ninja_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Lich          #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_lich_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_lich_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Shapeshifter  #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_shapeshifter_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_shapeshifter_number);
  send_to_char(buf,ch); 

  stc("     #GM#ga#os#yt#Ge#gr #0Tanar#G'#0ri      #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_tanarri_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_tanarri_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Angel         #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_angel_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_angel_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Undead Knight #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_undeadknight_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_undeadknight_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Spider Droid  #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_cyborg_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_cyborg_number);
  send_to_char(buf,ch);

  stc("     #GM#ga#os#yt#Ge#gr #0Paladin       #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_paladin_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_paladin_number);
  send_to_char(buf,ch); 

  stc("     #GM#ga#os#yt#Ge#gr #0Psionist      #G--)>     ",ch);
  sprintf(buf, "#0%-13s",class_leader.best_psionist_name);
  send_to_char(buf,ch);
  sprintf(buf, " #Gwith a #0%d #Gpower rating#n\n\r",class_leader.best_psionist_number);
  send_to_char(buf,ch);

  stc("\n\r",ch);
  
stc("#0(<#0*#G>#0-#G<#0-#G<#0-#G<#0*#G>#0-#G<#0-#G<#0-#G<#0*#G>#0-#G<#0-#G<#0-#G<#0*#G>#0) #GClass Leaders #0(#G<#0*#G>#0-#G>#0-#G>#0-#G<#0*#G>#0-#G>#0-#G>#0-#G<#0*#G>#0-#G>#0-#G>#0-#G<#0*#G>#0)#n\n\r",ch); 
  stc("\n\r",ch);

  return;
} 
void check_CL( CHAR_DATA *ch, char *argument ) {
  bool changed = FALSE;

  if (IS_NPC(ch)) return;
  if (ch->level > 6) return;
 
  if (IS_CLASS(ch, CLASS_DEMON))
  {
	if (randMight(ch) > class_leader.best_demon_number)
	{
    class_leader.best_demon_number = randMight(ch);
    free_string(class_leader.best_demon_name);
    class_leader.best_demon_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_MAGE))
  {
	if (randMight(ch) > class_leader.best_mage_number)
	{
    class_leader.best_mage_number = randMight(ch);
    free_string(class_leader.best_mage_name);
    class_leader.best_mage_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_WEREWOLF))
  {
	if (randMight(ch) > class_leader.best_werewolf_number)
	{
    class_leader.best_werewolf_number = randMight(ch);
    free_string(class_leader.best_werewolf_name);
    class_leader.best_werewolf_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_VAMPIRE))
  {
	if (randMight(ch) > class_leader.best_vampire_number)
	{
    class_leader.best_vampire_number = randMight(ch);
    free_string(class_leader.best_vampire_name);
    class_leader.best_vampire_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_HIGHLANDER))
  {
	if (randMight(ch) > class_leader.best_samurai_number)
	{
    class_leader.best_samurai_number = randMight(ch);
    free_string(class_leader.best_samurai_name);
    class_leader.best_samurai_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_DROW))
  {
	if (randMight(ch) > class_leader.best_drow_number)
	{
    class_leader.best_drow_number = randMight(ch);
    free_string(class_leader.best_drow_name);
    class_leader.best_drow_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_MONK))
  {
	if (randMight(ch) > class_leader.best_monk_number)
	{
    class_leader.best_monk_number = randMight(ch);
    free_string(class_leader.best_monk_name);
    class_leader.best_monk_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_NINJA))
  {
	if (randMight(ch) > class_leader.best_ninja_number)
	{
    class_leader.best_ninja_number = randMight(ch);
    free_string(class_leader.best_ninja_name);
    class_leader.best_ninja_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_POWER_LICH))
  {
	if (randMight(ch) > class_leader.best_lich_number)
	{
    class_leader.best_lich_number = randMight(ch);
    free_string(class_leader.best_lich_name);
    class_leader.best_lich_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
  {
	if (randMight(ch) > class_leader.best_shapeshifter_number)
	{
    class_leader.best_shapeshifter_number = randMight(ch);
    free_string(class_leader.best_shapeshifter_name);
    class_leader.best_shapeshifter_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_TANARRI))
  {
	if (randMight(ch) > class_leader.best_tanarri_number)
	{
    class_leader.best_tanarri_number = randMight(ch);
    free_string(class_leader.best_tanarri_name);
    class_leader.best_tanarri_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_ANGEL))
  {
	if (randMight(ch) > class_leader.best_angel_number)
	{
    class_leader.best_angel_number = randMight(ch);
    free_string(class_leader.best_angel_name);
    class_leader.best_angel_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
  {
	if (randMight(ch) > class_leader.best_undeadknight_number)
	{
    class_leader.best_undeadknight_number = randMight(ch);
    free_string(class_leader.best_undeadknight_name);
    class_leader.best_undeadknight_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_CYBORG))
  {
	if (randMight(ch) > class_leader.best_cyborg_number)
	{
    class_leader.best_cyborg_number = randMight(ch);
    free_string(class_leader.best_cyborg_name);
    class_leader.best_cyborg_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_PALADIN))
  {
	if (randMight(ch) > class_leader.best_paladin_number)
	{
    class_leader.best_paladin_number = randMight(ch);
    free_string(class_leader.best_paladin_name);
    class_leader.best_paladin_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (IS_CLASS(ch, CLASS_PSIONIST))
  {
	if (randMight(ch) > class_leader.best_psionist_number)
	{
    class_leader.best_psionist_number = randMight(ch);
    free_string(class_leader.best_psionist_name);
    class_leader.best_psionist_name = str_dup(ch->name);
    changed = TRUE;
	}
  }
  if (changed) save_classleader();
  return;
}

void do_clear_class_leader(CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (ch->level < 7) return;
  
  free_string(class_leader.best_demon_name);
  class_leader.best_demon_name = str_dup("Noone");
  class_leader.best_demon_number = 0;
  free_string(class_leader.best_mage_name);
  class_leader.best_mage_name = str_dup("Noone");
  class_leader.best_mage_number = 0;
  free_string(class_leader.best_werewolf_name);
  class_leader.best_werewolf_name = str_dup("Noone");
  class_leader.best_werewolf_number = 0;
  free_string(class_leader.best_vampire_name);
  class_leader.best_vampire_name = str_dup("Noone");
  class_leader.best_vampire_number = 0;  
  free_string(class_leader.best_samurai_name);
  class_leader.best_samurai_name = str_dup("Noone");
  class_leader.best_samurai_number = 0;  
  free_string(class_leader.best_drow_name);
  class_leader.best_drow_name = str_dup("Noone");
  class_leader.best_drow_number = 0;  
  free_string(class_leader.best_ninja_name);
  class_leader.best_ninja_name = str_dup("Noone");
  class_leader.best_ninja_number = 0;  
  free_string(class_leader.best_lich_name);
  class_leader.best_lich_name = str_dup("Noone");
  class_leader.best_lich_number = 0;  
  free_string(class_leader.best_shapeshifter_name);
  class_leader.best_shapeshifter_name = str_dup("Noone");
  class_leader.best_shapeshifter_number = 0;  
  free_string(class_leader.best_tanarri_name);
  class_leader.best_tanarri_name = str_dup("Noone");
  class_leader.best_tanarri_number = 0;
  free_string(class_leader.best_angel_name);
  class_leader.best_angel_name = str_dup("Noone");
  class_leader.best_angel_number = 0;  
  free_string(class_leader.best_undeadknight_name);
  class_leader.best_undeadknight_name = str_dup("Noone");
  class_leader.best_undeadknight_number = 0;  
  free_string(class_leader.best_cyborg_name);
  class_leader.best_cyborg_name = str_dup("Noone");
  class_leader.best_cyborg_number = 0;
  free_string(class_leader.best_paladin_name);
  class_leader.best_paladin_name = str_dup("Noone");
  class_leader.best_paladin_number = 0;
  free_string(class_leader.best_psionist_name);
  class_leader.best_psionist_name = str_dup("Noone");
  class_leader.best_psionist_number = 0;
  free_string(class_leader.best_monk_name);
  class_leader.best_monk_name = str_dup("Noone");
  class_leader.best_monk_number = 0;

  save_classleader();
  send_to_char("class leader board cleared.\n\r",ch);
  return;
}
void do_alias(CHAR_DATA *ch, char *argument)
{
  ALIAS_DATA *ali;
  char arg1[MAX_INPUT_LENGTH];
  char testarg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;

  argument = one_argument(argument, arg1);
  one_argument(argument, testarg);

  if ((ch->in_room == NULL || ch->in_room->vnum != 41000))
  {
    send_to_char("You must be at The Sacrifical Altar to create a new alias.\n\r",ch);
    return;
  }
  if (ch->pcdata->alias_count >= MAX_ALIAS)
  {
    send_to_char("Sorry, you have reached the limit for aliases, you need to remove some before you can make more.\n\r",ch);
    return;
  }
  if (arg1[0] == '\0' || argument[0] == '\0')
  {
    send_to_char("Syntax : alias 'short' 'long'\n\r",ch);
    send_to_char("\n\rThe 'short' is the alias, and the 'long' is the string that the alias replaces.\n\r",ch);
    send_to_char("ie. alias tt teleport orc.\n\r",ch);
    return;
  }
  if (strlen2(argument) > 400)
  {
    send_to_char("Behave... that's way to big.\n\r",ch);
    return;
  }
  smash_tilde(arg1);
  smash_tilde(testarg);
  smash_tilde(argument);
  if (!str_cmp(arg1, testarg))
  {
    send_to_char("You cannot alias the alias.\n\r",ch);
    return;
  }
  if (!str_cmp(arg1, "showalias") || !str_cmp(arg1, "alias") || !str_cmp(arg1, "removealias"))
  {
    send_to_char("You really shouldn't alias the alias commands.\n\r", ch);
    return;
  }
  for (ali = ch->pcdata->first_alias; ali; ali = ali->next)
  {
    if (!str_cmp(arg1, ali->short_n))
    {
      send_to_char("You already have such an alias.\n\r",ch);
      return;
    }
    if (!str_cmp(arg1, ali->long_n) || !str_cmp(argument, ali->short_n) || !str_cmp(argument, ali->long_n))
    {
      send_to_char("No looping aliases please.\n\r", ch);
      return;
    }
  }

    CREATE(ali, ALIAS_DATA, 1);

  ali->short_n = str_dup(arg1);
  ali->long_n = str_dup(argument);
  LINK( ali, ch->pcdata->first_alias, ch->pcdata->last_alias, next, prev );
  ch->pcdata->alias_count++;
  send_to_char("Ok.\n\r",ch);
  return;
}

void do_showalias(CHAR_DATA *ch, char *argument)
{
  ALIAS_DATA *ali;
  bool found = FALSE;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  for (ali = ch->pcdata->first_alias; ali; ali = ali->next)
  {
    found = TRUE;
    sprintf(buf, "%s '%s'\n\r", ali->short_n, ali->long_n);
    send_to_char(buf,ch);
  }
  if (!found) send_to_char("You have no aliases.\n\r",ch);
  return;
}
void do_removealias(CHAR_DATA *ch, char *argument)
{
  ALIAS_DATA *ali;
  char arg[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;

  one_argument(argument, arg);

  if (arg[0] == '\0')
  {
    send_to_char("Remove which alias?\n\r",ch);
    return;
  }
  for (ali = ch->pcdata->first_alias; ali; ali = ali->next)
  {
    if (!str_cmp(arg, ali->short_n))
    {
      alias_remove(ch, ali);
      send_to_char("Alias removed.\n\r",ch);
      ch->pcdata->alias_count--;
      return;
    }
  }
  send_to_char("No such alias.\n\r",ch);
  return;
}
void alias_remove(CHAR_DATA * ch, ALIAS_DATA * ali)
{
  if (IS_NPC(ch))
    return;
  if (ch->pcdata->first_alias == NULL)
  {
    bug("Alias_remove: no alias.", 0);
    return;
  }

  UNLINK( ali, ch->pcdata->first_alias, ch->pcdata->last_alias, next, prev);
  DISPOSE( ali );
  return;
}

void do_riftwalk( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;
    char arg[MSL];
    char buf[MSL];
    one_argument (argument, arg);

    if (IS_NPC(ch)) return;

    if (!IS_NPC(ch) && (ch->level < skill_table[gsn_riftwalk].skill_level))
    {
      send_to_char( "You do not possess the skill for walking the rifts in time and space.\n\r", ch );
      return;
    }
    if (arg[0] == '\0')
    {
	stc("You must specify a target to riftwalk to!\n\r",ch);
	return;
    }

    if (IS_NPC(ch)) return;
    if ((victim = get_char_world(ch, arg)) == NULL)
    {
        send_to_char("Whom do you wish to riftwalk to?\n\r", ch );
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
    	stc( "The arena prevents you from doing this.\n\r",ch);
    	return;
    }
    if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
    {
    	stc( "The arena prevents you from doing this.\n\r",ch);
    	return;
    }
    if (victim->in_room->area != ch->in_room->area)
    {
	stc( "You are not close enough, you must be within the same area.\n\r",ch);
	return;
    }
    if (victim->in_room->vnum == ch->in_room->vnum)
    {
    	send_to_char("But you're already there!\n\r",ch);
     	return;
    }

    WAIT_STATE(ch, 4);
    if ( !IS_NPC( ch ) &&  number_percent( ) > ch->pcdata->learned[gsn_riftwalk] )
    {
      act("You stare off into space and contort your face, looking stupid.",ch,NULL,NULL,TO_CHAR, FALSE);
      sprintf(buf, "%s is lost in %s own little world.",ch->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
      act(buf,ch,NULL,NULL,TO_ROOM, FALSE);
      if (!IS_NPC(victim))
      {
        if (number_range(1,100) == 1)
        {
          sprintf(buf, "You learn from your mistakes, and your %s skill improves!\n\r",
            skill_table[gsn_riftwalk].name);
          stc(buf,ch);
          ch->pcdata->learned[gsn_riftwalk] += 1;
        }
      }
      return;
    }
    location = victim->in_room;
    act("You open a rift in space and step into it.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n steps into thin air.", ch, NULL, NULL, TO_ROOM, FALSE);
    char_from_room(ch);
    char_to_room(ch, location);
    do_look(ch, "auto");
    act("You leave the rift you created.", ch, NULL, NULL, TO_CHAR, FALSE);
    act("$n steps out from behind you.", ch, NULL, NULL, TO_ROOM, FALSE);
    return;
}
void do_knock( CHAR_DATA *ch, char *argument )
{
    char arg[MSL];
    char buf[MSL];
    int door;
    EXIT_DATA *pexit;
    ROOM_INDEX_DATA *to_room;
    ROOM_INDEX_DATA *in_room;
    argument = one_argument( argument, arg );
    if (IS_NPC(ch)) return;
    
    if (!IS_NPC(ch) && (ch->level < skill_table[gsn_knock].skill_level))
    {
      send_to_char( "You do not possess the skill for knocking down ice walls.\n\r", ch );
      return;
    }

    if ( arg[0] == '\0')
    {
	stc("You must specify which direction to knock! <north/south/east/west/up/down>\n\r",ch);
	return;
    }
    if (!str_cmp(arg,"south") || !str_cmp(arg,"s"))
    {
	door = DIR_SOUTH;
    }
    else if (!str_cmp(arg,"north") || !str_cmp(arg,"n"))
    {
	door = DIR_NORTH;
    }
    else if (!str_cmp(arg,"west") || !str_cmp(arg,"w"))
    {
	door = DIR_WEST;
    }
    else if (!str_cmp(arg,"east") || !str_cmp(arg,"e"))
    {
	door = DIR_EAST;
    }
    else if (!str_cmp(arg,"up") || !str_cmp(arg,"u"))
    {
	door = DIR_UP;
    }
    else if (!str_cmp(arg,"down") || !str_cmp(arg,"d"))
    {
	door = DIR_DOWN;
    }
    else
    {
	stc("You must specify which direction to knock! <north/south/east/west/up/down>\n\r",ch);
	return;
    }

    if ( door < 0 || door > 5 )
    {
        bug( "Do_move: bad door %d.", door );
        return;
    }
    WAIT_STATE( ch, skill_table[gsn_knock].beats);
    if ( !IS_NPC( ch ) &&  number_percent( ) > ch->pcdata->learned[gsn_knock] )
    {
      act("You knock on the wall as if it were a door, and look stupid.",ch,NULL,NULL,TO_CHAR, FALSE);
      act("$n is knocking on an ice wall, hoping it will open.",ch,NULL,NULL,TO_ROOM, FALSE);
      if (number_range(1,10) == 1)
      {
        sprintf(buf, "You learn from your mistakes, and your %s skill improves!\n\r",
          skill_table[gsn_knock].name);
        stc(buf,ch);
        ch->pcdata->learned[gsn_knock] += 1;
      }
      return;
    }
    in_room = ch->in_room;

    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->to_room      ) == NULL )
    {
	stc("There isnt even an exit in that direction!\n\r",ch);
	return;
    }
    if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ICE_WALL))
    {
	sprintf(buf,"You knock loudly on the wall of ice %s!\n\r", exitname3[door]);
            send_to_char(buf, ch);
	if (number_range(1,3) >= 2)
	{
	  sprintf(buf,"You manage to knock down the wall of ice %s!\n\r", exitname3[door]);
              send_to_char(buf, ch);
	  make_wall(ch->in_room, door, 0);
	  return;
	}
    }
    else
    {
	stc("I don't see a wall in that direction.\n\r",ch);
	return;
    }
 return;
}
