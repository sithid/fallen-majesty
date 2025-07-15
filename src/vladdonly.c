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
#include<types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


void do_topxp(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
	CHAR_DATA *vch;
	CHAR_DATA *top[5];
    DESCRIPTOR_DATA *d;
	int i;
	bool showme;
	int players = 0;

	if (IS_NPC(ch)) return;

	send_to_char("Counting only online players:\n\r", ch);
	for (i = 0; i < 5; i++) top[i] = NULL;
	for (i = 0; i < 5; i++) 
	{
		for (d = first_descriptor; d != NULL; d = d->next) 
		{
			if (d->connected != CON_PLAYING) continue;
			if ((vch = d->character) == NULL) continue;
			if (IS_IMMORTAL(vch)) continue;
			if (!can_see(ch,vch)) continue;
			if (!strcmp(vch->name, "SuoMarte")) continue;
			players++;
			if (top[i] == NULL || vch->pcdata->score[SCORE_TOTAL_XP] > top[i]->pcdata->score[SCORE_TOTAL_XP]) 
			{
				if (top[0] == vch) continue;
				if (top[1] == vch) continue;
				if (top[2] == vch) continue;
				if (top[3] == vch) continue;
				top[i] = vch;
			}
		}
	}
	players = UMIN(players, 5);
	for (i = 0; i < players; i++)
		if (top[i] == NULL) top[i] = ch;
	for (i = 0; i < players; i++) 
	{
		sprintf(buf, "  #7%d#0: #R%-12s#n with #7%-11d#0 XP#n and #7%-6d#n earnt#0 QP#n.\n\r", 
		i+1, top[i]->name, top[i]->pcdata->score[SCORE_TOTAL_XP], top[i]->pcdata->score[SCORE_QUEST]);
		send_to_char(buf, ch);
	}
	showme = TRUE;
	for (i = 0; i < players; i++)
		if (top[i] == ch) showme = FALSE;
	if (showme) 
	{
		sprintf(buf, "#7You#0: #R%-12s#n with #7%-11d#0 XP#n and #7%-6d#n earnt#0 QP#n.\n\r", 
		ch->name, ch->pcdata->score[SCORE_TOTAL_XP], ch->pcdata->score[SCORE_QUEST]);
		send_to_char(buf, ch);
	}
}

void do_classself(CHAR_DATA *ch, char *argument)
{

char arg1[MAX_STRING_LENGTH];
argument = one_argument( argument, arg1 );

if (IS_NPC(ch))return;

if (!IS_NPC(ch) && ch->class != 0) return;

if (ch->level < 3) return;

if (!xIS_SET(ch->extra, EXTRA_POLICY))
  {
	stc("You cannot selfclass until you accept the policy. (help policy).\n\r",ch);
	return;
  }
if ( arg1[0] == '\0' )
{
  send_to_char("Classes: Type selfclass <class> to get classed.\n\r\n\r",ch);
  send_to_char("#R[#0Demon#R]#n             #y((#LWerewolf#y))#n         #P.o0#0Drow#P0o.#n        #n{{#CBattlemage#n}}\n\r", ch);
  send_to_char("#C***#yNinja#C***#n         #0<<#RVampire#0>>#n          #0.x[#lMonk#0]x.       #C(#Po#C) #PPsionist #C(#Po#C)#n     \n\r",ch);
  send_to_char("#C-=#RSamu#yrai#C=-#n       #y=#RTanar'ri#y=#n           #0[#P*#0]#rShapeshifter#0[#P*#0]#n   #G>*<#7Lich#G>*<\n\r",ch);
  send_to_char("#y.x#0(#7Angel#0)#yx.#n     #0|[#LUndead Knight#0]|#n    #p(#0-#p)#0Spider Droid#p(#0-#p)#n    #y-#7=#yP#7aladi#yn#7=#y- \n\r",ch);
  return;
}
if (!str_cmp(arg1,"demon")) 
{
do_clearstats2(ch,"");
ch->class = CLASS_DEMON;
set_learnable_disciplines(ch);
send_to_char( "You have chosen the #RDemonic#n path, may god have mercy on yer soul.\n\r", ch);  
}

else if (!str_cmp(arg1,"werewolf"))
{
do_clearstats2(ch,"");
ch->class = CLASS_WEREWOLF;
set_learnable_disciplines(ch);
send_to_char( "You have chosen the path of the #0Garou#n, may Gaia guide you.\n\r", ch);
}

else if (!str_cmp(arg1,"drow"))
{
do_clearstats2(ch,"");
ch->class = CLASS_DROW;
send_to_char( "Choose your profession, and #PLloth#n will guide you.\n\r", ch);
}

else if (!str_cmp(arg1,"ninja"))
{
do_clearstats2(ch,"");
ch->class = CLASS_NINJA;
send_to_char( "You have chosen a life in the #0shadows#n, assassinate at will.\n\r", ch);
}

else if (!str_cmp(arg1,"samurai"))
{
do_clearstats2(ch,"");
ch->class = CLASS_HIGHLANDER;
send_to_char( "You have chosen the way of the Samurai.\n\r", ch);
}
else if (!str_cmp(arg1,"paladin"))
{
do_clearstats2(ch,"");
ch->class = CLASS_PALADIN;
stc("You have become one of the Templars.\n\r",ch);
}
else if (!str_cmp(arg1,"psionist"))
{
do_clearstats2(ch,"");
ch->class = CLASS_PSIONIST;
stc("You have become one of the Mindbenders.\n\r",ch);
}
else if (!str_cmp(arg1,"tanarri"))
{
do_clearstats2(ch,"");
ch->class = CLASS_TANARRI;
send_to_char( "You're mad with demonic rage as the spirit of a Tanar'ri molds with yours.\n\r", ch);
}

else if (!str_cmp(arg1,"shapeshifter"))
{
do_clearstats2(ch,"");
ch->class = CLASS_SHAPESHIFTER;
send_to_char( "You scream in pain as your body begins to reshape.\n\r", ch);
}

else if (!str_cmp(arg1,"angel"))
{
do_clearstats2(ch,"");
ch->class = CLASS_ANGEL;
send_to_char( "You have chosen the way of the Light.\n\r", ch);
}

else if (!str_cmp(arg1,"undeadknight") || !str_cmp(arg1,"undead knight") || !str_cmp(arg1, "undead"))
{
do_clearstats2(ch,"");
ch->class = CLASS_UNDEAD_KNIGHT;
send_to_char( "You rise from the grave, the taste of vengence on your lips.\n\r", ch);
}

else if (!str_cmp(arg1,"spider droid") || !str_cmp(arg1,"spiderdroid"))
{
do_clearstats2(ch,"");
ch->class = CLASS_CYBORG;
send_to_char( "You are recreated with mechanical parts.\n\r", ch);
}

else if (!str_cmp(arg1,"vampire"))
{
do_clearstats2(ch,"");
ch->class = CLASS_VAMPIRE;
ch->beast = 30;
set_learnable_disciplines(ch);
send_to_char( "Fear the #ySun#n nosferatu, god's curse lives in you.\n\r", ch);
}

else if (!str_cmp(arg1,"monk"))
{
do_clearstats2(ch,"");
ch->class = CLASS_MONK;
send_to_char( "Your faith in God will guide you, destroy #7EVIL#n.\n\r", ch);
}

else if (!str_cmp(arg1,"lich"))
{
  if (ch->max_mana >= 5000 && ch->spl[RED_MAGIC]>99 && ch->spl[BLUE_MAGIC]>99
  && ch->spl[YELLOW_MAGIC]>99 && ch->spl[GREEN_MAGIC]>99 && ch->spl[PURPLE_MAGIC]>99)
  {
    do_clearstats2(ch,"");
    ch->class = CLASS_POWER_LICH;
    send_to_char( "You rise from the grave with knowledge of forgotten arcane magic.\n\r", ch);
  }
  else
  {
    send_to_char("You need 5K mana and 100 in all your spellcolors.\n\r",ch);
   }
}
  else if (!str_cmp(arg1,"mage") || !str_cmp(arg1,"battlemage"))
  {
  if (ch->max_mana >= 5000 && ch->spl[RED_MAGIC]>99 && ch->spl[BLUE_MAGIC]>99 
  && ch->spl[YELLOW_MAGIC]>99 && ch->spl[GREEN_MAGIC]>99 && ch->spl[PURPLE_MAGIC]>99)
  {
    do_clearstats2(ch,"");
    ch->class = CLASS_MAGE;
    send_to_char( "You start down the path of power, the #Rarcane#n is your weapon.\n\r", ch);
  }
  else
  {
    send_to_char("You need 5K mana and 100 in all your spellcolors.\n\r",ch);
    return;
  }
}
          if (IS_CLASS(ch, CLASS_TANARRI)) ch->alignment = -1000;
          if (IS_CLASS(ch, CLASS_ANGEL)) ch->alignment = 1000;
          if (IS_CLASS(ch, CLASS_DEMON)) ch->alignment = -1000;
          if (IS_CLASS(ch, CLASS_WEREWOLF)) ch->alignment = 0;
          if (IS_CLASS(ch, CLASS_NINJA)) ch->alignment = -500;
          if (IS_CLASS(ch, CLASS_VAMPIRE)) ch->alignment = -1000;
          if (IS_CLASS(ch, CLASS_MONK)) ch->alignment = 500;
          if (IS_CLASS(ch, CLASS_MAGE)) ch->alignment = 1000;
          if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) ch->alignment = 0;
          if (IS_CLASS(ch, CLASS_CYBORG)) ch->alignment = -1000;
          if (IS_CLASS(ch, CLASS_HIGHLANDER)) ch->alignment = 1000;
          if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) ch->alignment = -1000;
          if (IS_CLASS(ch, CLASS_POWER_LICH)) ch->alignment = -1000;
          if (IS_CLASS(ch, CLASS_DROW)) ch->alignment = -1000;
	  if (IS_CLASS(ch, CLASS_PALADIN)) ch->alignment = 1000;
return;
}
void do_reimb(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA	*vch;
	char		arg[MAX_INPUT_LENGTH];
	char		arg2[MAX_INPUT_LENGTH];
	int v;

	argument = one_argument(argument, arg);
	if (arg[0] == '\0') {
		send_to_char("Reimburse <target> <xp|qp> <amount>\n\r", ch);
		return;
	}
	if ((vch = get_char_world(ch, arg)) == NULL) {
		send_to_char("They aren't logged on.\n\r", ch);
		return;
	}
	if (IS_NPC(vch)) {
		send_to_char("That is a mob.\n\r", ch);
		return;
	}
	argument = one_argument(argument, arg);
	argument = one_argument(argument, arg2);
	v = atoi(arg2);
	if (arg[0] == '\0' || arg2[0] == '\0'
	  || (!is_number(arg2) && v >= 0)) {
		do_reimb(ch, "");
		return;
	}
	if (!str_cmp(arg, "xp")) {
		vch->exp += v;
		vch->pcdata->score[SCORE_TOTAL_XP] += v;
	} else if (!str_cmp(arg, "qp")) {
		vch->pcdata->quest += v;
                vch->pcdata->score[SCORE_QUEST] += v;
		vch->pcdata->questtotal += v;
	} else {
		send_to_char("Please specify XP or QP.\n\r", ch);
		return;
	}
	if (vch->mkill < 5) {
		vch->mkill = 5;
		do_autosave(ch, "");
	}
	sprintf(arg2, "%s reimbursed %d %s.\n\r", vch->name, v, arg);
	send_to_char(arg2, ch);
	sprintf(arg2, "%s has reimbursed you %d %s.\n\r", ch->name, v, arg);
	send_to_char(arg2, vch);
}

void do_affects( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   AFFECT_DATA *paf;

     if (IS_NPC(ch)) return;
     if ( xIS_SET(ch->act, PLR_HOLYLIGHT) )
	stc("#CTruesight.#n\n\r",ch);
     if (IS_SET(ch->special, SPC_FURY))
	stc("#7U#0nholy #7F#0ury.#n\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_CRITICAL))
   	stc("#RCritical Hit.#n\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_BIG))
	stc("#7+500 Damcap#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_AFFMANTIS))
	stc("#CEnhanced Dodge and Parry.#n\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_DOUBLEEXP))
	stc("#RD#youble #RE#yxp#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_MANASHIELD))
	stc("#CMana Shield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_ICESHIELD))
       send_to_char("#CIceshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD))
       send_to_char("#LLightningshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD))
       send_to_char("#GAcidshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_FIRESHIELD))
       send_to_char("#RFireshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD))
       send_to_char("#pChaosshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_REFLECT))
       send_to_char("#0Darkshield#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_REGENERATE))
       send_to_char("#LEnhanced Regeneration#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_SPEED))
       send_to_char("#yHaste#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_VISION))
       send_to_char("#cEnhanced Truesight#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_RAGER))
       send_to_char("#rEnhanced Rage#n.\n\r",ch);
     if (IS_ITEMAFF(ch, ITEMA_VORPAL))
       send_to_char("#rVorpal#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_SANCTUARY))
       send_to_char("#7Sanctuary#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_INVISIBLE))
	stc("#7Invisible.#n\n\r",ch);
     if ( IS_AFFECTED(ch, AFF_PROTECT))
       send_to_char("#LProtection from evil#n.\n\r",ch);
     if ( IS_AFFECTED(ch, AFF_PROTECT_GOOD))
       send_to_char("#LProtection from good#n.\n\r",ch);
     if ( IS_AFFECTED(ch, AFF_FLYING))
       send_to_char("#cFly#n\n\r",ch);
     if ( IS_AFFECTED(ch, AFF_WEBBED))
	send_to_char("#gWeb#n\n\r",ch);
     if ( IS_ITEMAFF(ch, ITEMA_RESISTANCE))
	send_to_char("#ySuperior Protection#n\n\r",ch);
     if (xIS_SET(ch->act, PLR_HIDE))
	send_to_char("#0Advanced Hide#n\n\r",ch);
     send_to_char("\n\r",ch);
     if ( ch->first_affect == NULL )
        {			   
 		send_to_char( "You have nothing affecting you at this time.\n\r", ch);
        return;
        }
     if ( ch->first_affect != NULL )
       {

         send_to_char( "You are affected by:\n\r", ch );
         for ( paf = ch->first_affect; paf != NULL; paf = paf->next )
            {
               sprintf( buf, "Spell: '%s'", skill_table[paf->type].name );
               send_to_char( buf, ch );
               if ( ch->level >= 0 )
                   {
   sprintf( buf, " modifies %s by %d for %d hours with bits %s.\n\r",
                      affect_loc_name( paf->location ),
                      paf->modifier,                    
                      paf->duration,
                      affect_bit_name( paf->bitvector ) );
                      send_to_char( buf, ch );
                   }

            }

     }

     return;

}

char * plr_bit_name( EXT_BV arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( xIS_SET(arg, PLR_IS_NPC      )) strcat( buf, " npc"           );
    if ( xIS_SET(arg, PLR_AUTOEXIT    ) ) strcat( buf, " autoexit"      );
    if ( xIS_SET(arg, PLR_AUTOLOOT    ) ) strcat( buf, " autoloot"      );
    if ( xIS_SET(arg, PLR_AUTOSAC     ) ) strcat( buf, " autosac"       );
    if ( xIS_SET(arg, PLR_AUTOSLAB)	) strcat( buf, " autoadamantite");
    if ( xIS_SET(arg, PLR_AUTOELEC    ) ) strcat( buf, " autoelectrum"  );
    if ( xIS_SET(arg, PLR_AUTOGOLD    ) ) strcat( buf, " autogold"	);
    if ( xIS_SET(arg, PLR_BLANK       ) ) strcat( buf, " blank"         );
    if ( xIS_SET(arg, PLR_BRIEF       ) ) strcat( buf, " brief"         );
    if ( xIS_SET(arg, PLR_BRIEF2       ) ) strcat( buf, " brief2"         );
    if ( xIS_SET(arg, PLR_BRIEF3       ) ) strcat( buf, " brief3"         );
    if ( xIS_SET(arg, PLR_COMBINE     ) ) strcat( buf, " combine"       );
    if ( xIS_SET(arg, PLR_PROMPT      ) ) strcat( buf, " prompt"        );
    if ( xIS_SET(arg, PLR_TELNET_GA   ) ) strcat( buf, " telnet_ga"     );
    if ( xIS_SET(arg, PLR_HOLYLIGHT   ) ) strcat( buf, " holylight"     );
    if ( xIS_SET(arg, PLR_WIZINVIS    ) ) strcat( buf, " wizinvis"      );
    if ( xIS_SET(arg, PLR_INCOG       ) ) strcat( buf, " incog"         );
    if ( xIS_SET(arg, PLR_ANSI        ) ) strcat( buf, " ansi"          );
    if ( xIS_SET(arg, PLR_SILENCE     ) ) strcat( buf, " silenced"      );
    if ( xIS_SET(arg, PLR_NO_TELL     ) ) strcat( buf, " no_tell"       );
    if ( xIS_SET(arg, PLR_LOG         ) ) strcat( buf, " log"           );
    if ( xIS_SET(arg, PLR_FREEZE      ) ) strcat( buf, " freeze"        );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * extra_plr_bit_name( EXT_BV arg )
{
    static char buf[512];
    
    buf[0] = '\0';
    
    if ( xIS_SET(arg, EXTRA_TRUSTED )           ) strcat( buf, " q_trusted" );
    if ( xIS_SET(arg, EXTRA_NEWPASS )           ) strcat( buf, " newpass" );
    if ( xIS_SET(arg, EXTRA_OSWITCH )           ) strcat( buf, " oswitch" );
    if ( xIS_SET(arg, EXTRA_SWITCH  )           ) strcat( buf, " switch" );
    if ( xIS_SET(arg, EXTRA_FAKE_CON )          ) strcat( buf, " fake_con" );
    if ( xIS_SET(arg, TIED_UP )                 ) strcat( buf, " tied_up" );
    if ( xIS_SET(arg, GAGGED )                  ) strcat( buf, " gagged" );
    if ( xIS_SET(arg, BLINDFOLDED )             ) strcat( buf, " blindfolded" );
    if ( xIS_SET(arg, EXTRA_DONE )              ) strcat( buf, " non_virgin" );
    if ( xIS_SET(arg, EXTRA_EXP )               ) strcat( buf, " got_exp" );
    if ( xIS_SET(arg, EXTRA_PREGNANT )          ) strcat( buf, " pregnant" );
    if ( xIS_SET(arg, EXTRA_LABOUR )            ) strcat( buf, " labour" );
    if ( xIS_SET(arg, EXTRA_BORN )              ) strcat( buf, " born" );
    if ( xIS_SET(arg, EXTRA_PROMPT )            ) strcat( buf, " prompt" );
    if ( xIS_SET(arg, EXTRA_MARRIED )           ) strcat( buf, " married" );
    if ( xIS_SET(arg, EXTRA_CALL_ALL )          ) strcat( buf, " call_all" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}
   char * get_position_name( int arg )
{
    switch( arg )
    {
        case 0: return "dead";
        case 1: return "mortal";
        case 2: return "incap";
        case 3: return "stunned";
        case 4: return "sleeping";
        case 5: return "meditating";
        case 6: return "sitting";
        case 7: return "resting";
        case 8: return "fighting";
        case 9: return "standing";
    }
    bug( "Get_position_name: unknown type %d.", arg );
    return "(unknown)";
}

/*
 * Itemaffect bit names :)
 */
char * itemaffect_bit_name( int arg )
{
    static char buf[512];
    
    buf[0] = '\0';

    if ( arg & ITEMA_SHOCKSHIELD        ) strcat( buf, " Shockshield"  ); 
    if ( arg & ITEMA_FIRESHIELD         ) strcat( buf, " Fireshield"    );
    if ( arg & ITEMA_ICESHIELD          ) strcat( buf, " Iceshield"     );
    if ( arg & ITEMA_ACIDSHIELD         ) strcat( buf, " Acidshield"    );
    if ( arg & ITEMA_CHAOSSHIELD        ) strcat( buf, " Chaoshield"    );
    if ( arg & ITEMA_ARTIFACT           ) strcat( buf, " Artifact"      );
    if ( arg & ITEMA_REGENERATE         ) strcat( buf, " Regeneration"  );
    if ( arg & ITEMA_SPEED              ) strcat( buf, " Speed"         );
    if ( arg & ITEMA_VORPAL             ) strcat( buf, " Vorpal"        );
    if ( arg & ITEMA_CRITICAL           ) strcat( buf, " Critical Hit"  );
    if ( arg & ITEMA_RIGHT_SILVER       ) strcat( buf, " Right Silver"  );
    if ( arg & ITEMA_LEFT_SILVER        ) strcat( buf, " Left Silver"   );
    if ( arg & ITEMA_REFLECT            ) strcat( buf, " Darkshield"    );
    if ( arg & ITEMA_RESISTANCE         ) strcat( buf, " Resistance"    );
    if ( arg & ITEMA_VISION             ) strcat( buf, " Vision"        );
    if ( arg & ITEMA_STALKER            ) strcat( buf, " Stalker"       );
    if ( arg & ITEMA_VANISH             ) strcat( buf, " Vanish"        );
    if ( arg & ITEMA_RAGER              ) strcat( buf, " Rager"         );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

/*
 * Pstat code by Tijer
 */
void do_pstat ( CHAR_DATA *ch, char *argument )
{
    char        arg[MAX_INPUT_LENGTH];
    char        buf[MAX_STRING_LENGTH];
    CHAR_DATA   *victim;
        
    argument = one_argument( argument, arg );
  
    if ( arg[0] == '\0' )
    {
        send_to_char("Pstat whom?\n\r", ch );
        return;
    }
 
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
       send_to_char("They aren't here.\n\r", ch );
        return;
    }
    
    sprintf( buf, "Name : %s.\n\r",
        IS_NPC( victim )        ? victim->short_descr : victim->name );
  send_to_char( buf, ch );
    sprintf( buf, "Sex : %s. Room : %d. Align : %d. Primal : %d.\n\r",
        victim->sex == SEX_MALE         ? "Male"        :
      victim->sex == SEX_FEMALE       ? "Female"      : "None",
        victim->in_room == NULL         ? 0             : victim->in_room->vnum,
        victim->alignment,
        victim->practice
        );
        stc( buf, ch);
     if (!IS_NPC(victim))
     {
       sprintf(buf, "QP: %d.  Total_QP: %d.   Buyqp_number: %d.  Buyqp_total: %d.\n\r", 
victim->pcdata->quest, victim->pcdata->score[SCORE_QUEST],
victim->pcdata->questsrun, victim->pcdata->questtotal );
       stc( buf, ch);
     }
        
    sprintf( buf, "Level : %d. Trust : %d. Gold : %2.0f. Exp : %2.0f.\n\r",
        victim->level,
        victim->trust,
        victim->gold,
        victim->exp );
   send_to_char( buf, ch );
    
       sprintf( buf, "Hit : %d. Dam : %d. AC : %d. Position : %s\n\r",
       char_hitroll( victim ),
        char_damroll( victim ),
        char_ac( victim ),
        capitalize( get_position_name( victim->position ) ));
    send_to_char( buf, ch );
         
    sprintf( buf, "HP %d/%d. Mana %d/%d. Move %d/%d.\n\r",
        victim->hit, victim->max_hit,
        victim->mana, victim->max_mana,
        victim->move, victim->max_move );
    send_to_char( buf, ch );
        
    sprintf( buf, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\n\r",
      get_curr_str(victim),
        get_curr_int(victim),
        get_curr_wis(victim),
        get_curr_dex(victim), 
        get_curr_con(victim) );
    send_to_char( buf, ch );
        
    sprintf( buf, "Fighting : %s. (%d)\n\r",
        victim->fighting ? victim->fighting->name  : "(None)",
        victim->fighting ? victim->fighting->level : 0 ); 
   send_to_char( buf, ch );
        
    sprintf( buf, "Pkill : %d. Pdeath : %d. Mkill : %d. Mdeath : %d.\n\r",
        IS_NPC( victim ) ? 0 : victim->pkill,
        IS_NPC( victim ) ? 0 : victim->pdeath,
        IS_NPC( victim ) ? 0 : victim->mkill,
       IS_NPC( victim ) ? 0 : victim->mdeath );
    send_to_char( buf, ch ); 
  
    sprintf( buf, "Legend Status: %d. Legend Points: %d\n\r",
	IS_NPC(victim) ? 0 : victim->lstatus,
	IS_NPC(victim) ? 0 : victim->lexp);
	stc(buf,ch);
    sprintf( buf, "TotExp  : %12d. TotMobLev  : %10d. TotQuestPoints : %10d.\n\r",
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_XP],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_LEVEL],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_QUEST] );
    send_to_char( buf, ch );
        
    sprintf( buf, "HighExp : %12d. HighMobLev : %10d. Tot#Quests     : %10d.\n\r",
      IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_XP],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_LEVEL],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_NUM_QUEST] );  
    send_to_char( buf, ch );
        
    if ( !IS_NPC( victim ) )
   {
        sprintf( buf, "Unarmed : %4d.", victim->wpn[0] );
        send_to_char( buf, ch );
        sprintf( buf, " Slice   : %4d.", victim->wpn[1] );
        send_to_char( buf, ch );
        sprintf( buf, " Stab    : %4d.", victim->wpn[2] );
        send_to_char( buf, ch );
        sprintf( buf, " Slash   : %4d.", victim->wpn[3] );
        send_to_char( buf, ch );
        sprintf( buf, " Whip    : %4d.\n\r", victim->wpn[4] );
        send_to_char( buf, ch );
        sprintf( buf, "Claw    : %4d.", victim->wpn[5] );
        send_to_char( buf, ch );
        sprintf( buf, " Blast   : %4d.", victim->wpn[6] );
        send_to_char( buf, ch );
        sprintf( buf, " Pound   : %4d.", victim->wpn[7] );
send_to_char( buf, ch );
        sprintf( buf, " Crush   : %4d.", victim->wpn[8] );
        send_to_char( buf, ch );
        sprintf( buf, " Grep    : %4d.\n\r", victim->wpn[9] );
        send_to_char( buf, ch );
        sprintf( buf, "Bite    : %4d.", victim->wpn[10] );
     send_to_char( buf, ch );   
        sprintf( buf, " Pierce  : %4d.", victim->wpn[11] );
        send_to_char( buf, ch );
        sprintf( buf, " Suck    : %4d.\n\r",victim->wpn[12] );
        send_to_char( buf, ch );
        
        sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
            "Purple",   victim->spl[PURPLE_MAGIC],
            "Red",      victim->spl[RED_MAGIC],
            "Blue",     victim->spl[BLUE_MAGIC],
           "Green",    victim->spl[GREEN_MAGIC],
            "Yellow",   victim->spl[YELLOW_MAGIC] );
        send_to_char( buf, ch );
        sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
            "Viper",    victim->stance[STANCE_VIPER],
            "Crane",    victim->stance[STANCE_CRANE],
            "Crab",     victim->stance[STANCE_CRAB],
            "Mongoose", victim->stance[STANCE_MONGOOSE],   
            "Bull",     victim->stance[STANCE_BULL] );
        send_to_char( buf, ch );
        
        sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %-3d. %-8s : %3d.\n\r",
            "Mantis",   victim->stance[STANCE_MANTIS],
            "Dragon",   victim->stance[STANCE_DRAGON],
            "Tiger",    victim->stance[STANCE_TIGER],
            "Monkey",   victim->stance[STANCE_MONKEY],
           "Swallow",  victim->stance[STANCE_SWALLOW] );

        send_to_char( buf, ch );
        sprintf( buf, "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %-3d. %-8s : %3d.\n\r",
            "ss1",   victim->stance[STANCE_SS1],
            "ss2",   victim->stance[STANCE_SS2],
            "ss3",    victim->stance[STANCE_SS3],
            "ss4",   victim->stance[STANCE_SS4],
            "ss5",  victim->stance[STANCE_SS5] );
        send_to_char( buf, ch );

        
	 sprintf( buf, "Act         : %s\n\r", plr_bit_name(victim->act ));
    	send_to_char( buf, ch );
	sprintf( buf, "Extra       : %s\n\r",
        extra_plr_bit_name( victim->extra ) );
    	send_to_char( buf, ch );
	sprintf( buf, "ItemAff     : %s\n\r",
        victim->itemaffect <= 0 ? "(None)" : itemaffect_bit_name(victim->itemaffect ) );
    	send_to_char( buf, ch );
           
              sprintf( buf, "Affected by : %s.\n\r",  
        affect_bit_name( victim->affected_by ) );
    	send_to_char( buf, ch );
	sprintf(buf,"Power Rating:  %d\n\r",randMight(victim));
	stc(buf,ch);            
        if (xIS_SET(victim->extra, EXTRA_JUST_PK))
	{
	sprintf(buf,"JUST-PK\n\r");
	stc(buf,ch);
	}
        if (xIS_SET(victim->extra, EXTRA_RESET))
        {
        sprintf(buf,"BEEN-RESET\n\r");
        stc(buf,ch);
        }

    return;
}       
}

