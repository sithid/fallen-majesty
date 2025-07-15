
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
#include "olc.h"

char *	const	where_name	[] =
{
    "#R[#CLight#R         ]#n ",
    "#R[#COn Finger     #R]#n ",
    "#R[#COn Finger     #R]#n ",
    "#R[#CAround Neck   #R]#n ",
    "#R[#CAround Neck   #R]#n ",
    "#R[#COn Body       #R]#n ",
    "#R[#COn Head       #R]#n ",
    "#R[#COn Legs       #R]#n ",
    "#R[#COn Feet       #R]#n ",
    "#R[#COn Hands      #R]#n ",
    "#R[#COn Arms       #R]#n ",
    "#R[#COff Hand      #R]#n ",
    "#R[#CAround Body   #R]#n ",
    "#R[#CAround Waist  #R]#n ",
    "#R[#CAround Wrist  #R]#n ",
    "#R[#CAround Wrist  #R]#n ",
    "#R[#CRight Hand    #R]#n ",
    "#R[#CLeft Hand     #R]#n ",
    "#R[#CThird Hand    #R]#n ",
    "#R[#CFourth Hand   #R]#n ",
    "#R[#COn Face       #R]#n ",
    "#R[#CAs the Mastery#R]#n ",
    "#R[#CAs the Tattoo #R]#n ",
    "#R[#CIn Left Ear   #R]#n ",
    "#R[#CIn Right Ear  #R]#n ",
    "#R[#CLeft Scabbard #R]#n ",
    "#R[#CRight Scabbard#R]#n ",
};

const char * exitname [6] =
{
	"#Cn#0orth#n",
	"#Ce#0ast#n",
	"#Cs#0outh#n",
	"#Cw#0est#n",
	"#Cu#0p#n",
	"#Cd#0own#n"
};
const char * exitname2 [6] =
{
        "to the north",
        "to the east",
        "to the south",
        "to the west",
        "above you",
        "below you"
};



/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );

void	evil_eye		args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void 	check_left_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_left_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

void	obj_score		args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );


char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';
    if ( IS_SET(obj->quest, QUEST_ARTIFACT)) strcat(buf, "#7(#CAr#ct#Cif#ca#Cct#7)#n "
);
    else if ( IS_SET(obj->quest, QUEST_NEWBIE)) strcat(buf,"#7(#RNEWBIE#7)#n ");
    else if ( IS_SET(obj->quest, QUEST_RUNE)) strcat(buf,"#7(#0RARE#7)#n ");
    else if ( IS_SET(obj->quest, QUEST_RELIC)) strcat(buf,"#R(#yR#oe#yl#oi#yc#R)#n " );
    else if ( IS_SET(obj->quest, QUEST_ADAMANTIUM)) strcat(buf, "#0(#7A#nd#7a#nm#7a#nn#7t#ni#7u#nm#n#0)#n ");
    else if ( obj->points < 750 && obj->points != 0) strcat(buf,"#3(#0Legendary#3)#n " );
    else if ( obj->points < 1250 && obj->points != 0 ) strcat(buf, "#7(#2Mythical#7)#n " );
    else if ( obj->points != 0) strcat(buf, "#6(#GP#gr#Gi#gc#Ge#gl#Ge#gs#Gs#6)#n " );	
    if (IS_OBJ_STAT(obj, ITEM_GLOW)) strcat(buf,"#y(#rGlow#y)#n ");
    if (IS_OBJ_STAT(obj, ITEM_HUM))  strcat(buf,"#y(#rHum#y)#n ");
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "#6(Invis)#n " );
    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "#4(Blue Aura)#n "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "#1(Red Aura)#n "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "#3(Yellow Aura)#n "  );
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "#4(Magical)#n " );
        if (IS_SET(obj->spectype, SITEM_COPPER))  strcat(buf,"#r(Copper)#n ");
    if (IS_SET(obj->spectype, SITEM_IRON))  strcat(buf,"#c(Iron)#n ");
    if (IS_SET(obj->spectype, SITEM_STEEL))  strcat(buf,"#C(Steel)#n ");
    if (IS_SET(obj->spectype, SITEM_ADAMANTITE))  strcat(buf,"#0(#CAdamantite#0)#n ");
    if (IS_SET(obj->spectype, SITEM_ELECTRUM)) strcat(buf,"#0(#7E#ple#Pct#pru#7m#0)#n ");
    if (IS_SET(obj->spectype, SITEM_HILT))  strcat(buf,"#P(Hilted)#n ");
    if (IS_SET(obj->spectype, SITEM_GEMSTONE)) strcat(buf,"#C(#yGemstoned#C)#n ");
    if ( IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	!IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf,
"#0(Shadowplane)#n " );
    if (!IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	 IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf, "#7(Normal plane)#n " );

    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
	if ( obj->condition < 100) strcat(buf, " #1(Damaged)#n");
    }
    else
    {
	if ( obj->description != NULL )
	    strcat( buf, obj->description );
    }

if (strlen(buf)<=0)
    strcat(buf,"This object has no description. Please inform the IMP.");

return buf;
}

int char_damroll(CHAR_DATA *ch)
{
  int value = 0;
  value += ch->damroll;

  if (!IS_NPC(ch)) 
  {
    value += str_app[get_curr_str(ch)].todam;
    value += ch->xdamroll;
  }
  if( ch-> level < LEVEL_AVATAR ) return value;

  if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->rage > 0)
    value += ch->rage*2;
	if (IS_CLASS(ch, CLASS_NINJA) && ch->rage > 0)
  	value += ch->rage*4;
  if (IS_SET(ch->special,SPC_WOLFMAN) && ch->rage > 99)
    value += ch->rage;
  if (IS_CLASS(ch, CLASS_DEMON) && ch->rage > 0)
    value += ch->rage;
  if(IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 7)
    value += 1000;
  if (IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 8 && 
ch->pcdata->souls > 10)
    value += ch->pcdata->souls * 10;
  if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 6 &&
ch->pcdata->powers[HARA_KIRI] > 0)
        value += 400;
  if (IS_CLASS(ch, CLASS_HUNTER) && IS_ITEMAFF(ch, ITEMA_CRITICAL) )
	value += 300;
  if (IS_CLASS(ch, CLASS_HUNTER) && IS_ITEMAFF(ch, ITEMA_SPEED) )
	value += 100;

  if( IS_CLASS(ch, CLASS_MONK) && IS_ITEMAFF(ch, ITEMA_CHAOSHANDS ) )
        value += 200;

        if (IS_CLASS( ch,CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
	value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
	else if (IS_SET(ch->special,SPC_CHAMPION) && ch->pcdata->stats[DEMON_POWER] > 0)
	value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
	else if (IS_CLASS(ch, CLASS_DROW) && ch->pcdata->stats[DEMON_POWER] > 0)
	value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
  if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST))
    value += 500;

    if (IS_CLASS(ch, CLASS_HIGHLANDER) && (get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	int wpn = ch->wpn[1];
	int total = ch->wpn[0] + ch->wpn[2] + ch->wpn[3] + ch->wpn[4] + ch->wpn[5] +
		    ch->wpn[6] + ch->wpn[7] + ch->wpn[8] + ch->wpn[9] + ch->wpn[10] +
		    ch->wpn[11] + ch->wpn[12];
	
	  if (wpn >= 1000)
 		value += 650;
	  else if (wpn >= 500)
		value += 500;
	  else
		value += wpn/2; 
	total = total / 14;
	value += total;
    }
  if (IS_CLASS(ch, CLASS_PALADIN) && (get_eq_char(ch, WEAR_WIELD) != NULL))
  {
	value += 1500;
   }
    if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->newbits, NEW_DROWHATE))
        value += 500;
   if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
   {
    if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
    {
   value += 1750;
    }
    if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
    {
    value += 1500;
    }
    if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
    {
    value += 2000;
    }
    if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
    {
    value += 1500;
    }
   }
    if (!IS_NPC(ch))
    {
      if (ch->lstatus == 0)
	{
        if( value > 5000 )
        value = 5000;
	}
      if (ch->lstatus > 0)
	{
        if (value > (5000 + (ch->lstatus * 300) ))
        value = (5000 + (ch->lstatus * 300));
	}
    }
    if (IS_IMMORTAL(ch)) value = 10000;
    return value;
}


int char_hitroll(CHAR_DATA *ch)
{
    int value = 0;
    value += ch->hitroll;

    if (!IS_NPC(ch))
        value += str_app[get_curr_str(ch)].tohit;

    if( ch-> level < LEVEL_AVATAR ) return value;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->rage > 0)
	  value += ch->rage*2;



	if (IS_CLASS(ch, CLASS_NINJA) && ch->rage > 0)
	value += ch->rage*4;

    if (IS_SET(ch->special,SPC_WOLFMAN) && ch->rage > 99)
	  value += ch->rage;

    if (IS_CLASS(ch, CLASS_DEMON) && ch->rage > 0)
	  value += ch->rage;
  if(IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 7)
    value += 1000;

  if (IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 8 &&
ch->pcdata->souls > 10)
    value += ch->pcdata->souls * 10;
        if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 6
        && ch->pcdata->powers[HARA_KIRI] > 0)
        value += 400;
  if(IS_CLASS(ch, CLASS_HUNTER) && IS_ITEMAFF(ch,ITEMA_CRITICAL) )
	value += 100;
  if(IS_CLASS(ch, CLASS_HUNTER) && IS_ITEMAFF(ch, ITEMA_SPEED) )
	value += 300;
  if( IS_CLASS(ch, CLASS_MONK) && IS_ITEMAFF(ch, ITEMA_CHAOSHANDS ) )
        value += 200;

    if (IS_CLASS( ch,CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
       value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
        else if (IS_SET(ch->special,SPC_CHAMPION) && ch->pcdata->stats[DEMON_POWER] > 0)
        value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);
        else if (IS_CLASS(ch, CLASS_DROW) && ch->pcdata->stats[DEMON_POWER] > 0)
        value += (( ch->pcdata->stats[DEMON_POWER]) * ch->pcdata->stats[DEMON_POWER]);

    if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST))
       value += 500;

    if (IS_CLASS(ch, CLASS_HIGHLANDER) && (get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	int wpn = ch->wpn[1];
	int total = ch->wpn[0] + ch->wpn[2] + ch->wpn[3] + ch->wpn[4] + ch->wpn[5] +
		    ch->wpn[6] + ch->wpn[7] + ch->wpn[8] + ch->wpn[9] + ch->wpn[10] +
		    ch->wpn[11] + ch->wpn[12];
	  
	  if (wpn >= 1000)
		value += 650;
	  else if (wpn >= 500)
		value += 500;
	  else
		value += wpn/2;
	total = total / 14;
	value += total;
    }
  if (IS_CLASS(ch, CLASS_PALADIN) && (get_eq_char(ch, WEAR_WIELD) != NULL))
  {
        value += 1500;
   }

    if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->newbits, NEW_DROWHATE))
	value += 500;
   if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
   {
    if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
    {
   value += 1750;
    }
    if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
    {
    value += 1500;
    }
    if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
    {
    value += 2000;
    }
    if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
    {
    value += 1500;
    }
   }
    if (!IS_NPC(ch))
    {
      if (ch->lstatus == 0)
        {
        if( value > 5000 )
        value = 5000;
        }
      if (ch->lstatus > 0)
        {
        if (value > (5000 + (ch->lstatus * 300) ))
        value = (5000 + (ch->lstatus * 300));
        }
    }
    if (IS_IMMORTAL(ch)) value = 10000;
    return value;
}

int char_ac(CHAR_DATA *ch)
{
    int value = 0;
    value = GET_AC(ch);

    if (IS_AWAKE(ch))
   	value += dex_app[get_curr_dex(ch)].defensive;

    /* Highlander */
    if (IS_CLASS(ch, CLASS_HIGHLANDER) && (get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	int wpn = ch->wpn[1];
	value -= wpn * 3;
    }

    if (IS_CLASS(ch, CLASS_PALADIN) && (get_eq_char(ch, WEAR_WIELD) != NULL))
    {
        int wpn = ch->wpn[1];
        value -= wpn * 1.5;
    }

    if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->newbits, NEW_DROWHATE))
        value -= 500;
  if (IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 8 &&
ch->pcdata->souls > 10)
    value -= ch->pcdata->souls * 10;
  if (IS_CLASS(ch, CLASS_HUNTER) && IS_ITEMAFF(ch, ITEMA_CRITICAL) && IS_ITEMAFF(ch, ITEMA_SPEED) )
	value -= 500;

    if( value > 3000 ) value = 3000;

    return value;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    bool fCombine;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    CREATE( prgpstrShow,	char*,	count );
    CREATE( prgnShow,		int,	count );

    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    { 
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );
	    fCombine = FALSE;

	    if ( IS_NPC(ch) || xIS_SET(ch->act, PLR_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if ( IS_NPC(ch) || xIS_SET(ch->act, PLR_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "(%2d) ", prgnShow[iShow] );
		send_to_char( buf, ch );
	    }
	    else
	    {
		send_to_char( "     ", ch );
	    }
	}
	send_to_char( prgpstrShow[iShow], ch );
	send_to_char( "\n\r", ch );
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || xIS_SET(ch->act, PLR_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }

    /*
     * Clean up.
     */
    DISPOSE( prgpstrShow );
    DISPOSE( prgnShow );

    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char buf4[MAX_STRING_LENGTH];
    char buf6[MAX_STRING_LENGTH];
    char buf7[MAX_STRING_LENGTH];
    char mount2[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    buf[0] = '\0';
    buf2[0] = '\0';
    buf3[0] = '\0';
    buf6[0] = '\0';


    if (!IS_NPC(victim) && victim->pcdata->chobj != NULL )
	return;

    if ((mount = victim->mount) != NULL && IS_SET(victim->mounted, IS_MOUNT))
	return;

    if ( !IS_NPC(victim) && IS_SET(victim->flag2, VAMP_OBJMASK) )
    {
	sprintf(buf, "     %s\n\r",victim->objdesc);
	stc(buf,ch);
	return;
    }


     if (victim->cur_form != 0)
    {
    buf[0] = '\0';
   
    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
	strcat( buf, "     " );
 else {
	if (!IS_NPC(victim) && victim->desc==NULL ) strcat( buf,"#r(Link-Dead)#n ");
    	if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "#L(Invis)#n "   );
    	if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "#y(Hide)#n " );
    	if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "#R(Charmed)#n " );
    	if ( IS_AFFECTED(victim, AFF_PASS_DOOR)  || 
           IS_AFFECTED(victim, AFF_ETHEREAL)    ) strcat( buf, "#p(Translucent)#n ");
    	if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "#P(Pink Aura)#n "  );
    	if ( IS_EVIL(victim)
    	&&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "#R(Red Aura)#n "   );
    	if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "#C(White Aura)#n " );
      }
    if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) 
     && !IS_AFFECTED(victim, AFF_SHADOWPLANE))    strcat( buf, "(Normal Plane) ");
    else if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) 
        && IS_AFFECTED(victim, AFF_SHADOWPLANE))  strcat( buf, "#0(Shadowplane)#n "     );

    
    if (ch->class > 0)
    {
       if (IS_CLASS(victim, CLASS_VAMPIRE))
                strcat(buf, "#0(#RVampire#0)#n ");
       if (IS_CLASS(victim, CLASS_WEREWOLF))
		strcat(buf, "#y(#LWerewolf#y)#n ");
       if (IS_CLASS(victim, CLASS_MONK))
	        strcat(buf, "#0(#lMonk#0)#n ");
       if (IS_CLASS(victim, CLASS_DEMON))
	        strcat(buf, "#R(#0Daemon#R)#n ");
       if (IS_CLASS(victim, CLASS_CYBORG))
	        strcat(buf, "#p(#PDrider#p)#n ");
       if (IS_CLASS(victim, CLASS_DROW))
	        strcat(buf, "#P(#0Drow#P)#n ");
       if (IS_CLASS(victim, CLASS_MAGE))
                strcat(buf, "#R(Warlock)#n ");
    }

    if (victim->cur_form != 0)
    {
	if (victim->position == POS_STANDING)
	{
	    char buf2[MAX_STRING_LENGTH];
	    form_in_room_desc(victim, buf2);
	    strcat(buf, buf2);
	}
	else
	{
	    if (is_vowel(form_data[victim->cur_form].lookname[0]))
	        strcat(buf, "An ");
	    else strcat(buf, "A ");
	    strcat(buf, form_data[victim->cur_form].lookname);
	}
    }
    
    if( !IS_SET(victim->affected_by2, AFF_SEVERED) )
    {
    switch ( victim->position )
    {
    case POS_DEAD:     strcat( buf, " is lying dead on the floor." ); break;
    case POS_MORTAL:   strcat( buf, " is lying here, #Rmortally wounded#n." ); break;
    case POS_INCAP:    strcat( buf, " is lying here #rincapacitated#n." ); break;
    case POS_STUNNED:  strcat( buf, " is lying here #0stunned#n." ); break;
    case POS_SLEEPING: strcat(buf," is sleeping here."); break;
    case POS_RESTING:  strcat( buf, " is resting here." ); break;
    case POS_SITTING:  strcat(buf, " is sitting here."); break;
    case POS_FIGHTING:
	strcat( buf, " is here, fighting " );
	if ( victim->fighting == NULL )
	    strcat( buf, "thin air??" );
	else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	else if ( victim->in_room == victim->fighting->in_room )
	{
	    strcat( buf, PERS( victim->fighting, ch ) );
	    strcat( buf, "." );
	}
	else
	    strcat( buf, "someone who left??" );
	break;
    }
    }
    else
    {
        strcat( buf, "'s upper torso is here, twitching.");	
    }
    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    return;
    }	


    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
	strcat( buf, "     " );
    else {
	if (!IS_NPC(victim) && victim->desc==NULL ) strcat( buf,"#y(Link-Dead)#n ");
    	if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "#L(Invis)#n "   );
    	if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "#0(Hide)#n "    );
    	if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "#R(Charmed)#n " );
    	if ( IS_AFFECTED(victim, AFF_PASS_DOOR)  ||
         IS_AFFECTED(victim, AFF_ETHEREAL)    )
	 strcat( buf, "#l(Translucent)#n ");
    	if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "#P(Pink Aura)#n "  );
    	if ( IS_EVIL(victim)
    	&&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "#R(Red Aura)#n "   );
    	if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "#C(White Aura)#n " );
    }

    if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE))
	strcat( buf, "#0(#CNormal plane#0)#n "     );
    else if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE))
	strcat( buf, "#C(#0Shadowplane#C)#n "     );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim,CLASS_WEREWOLF) && IS_HERO(victim) && IS_HERO(ch))
	strcat( buf, "#y(#LWerewolf#y)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_DEMON) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#R(#0Demon#R)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_NINJA) && IS_HERO(victim) && IS_HERO(ch))     
        strcat( buf, "#R(#yNinja#R)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_MONK) && IS_HERO(victim) && IS_HERO(ch))     
        strcat( buf, "#C(#nMonk#C)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_CYBORG) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#p(#PDrider#p)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_ANGEL) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#0(#7Angel#0)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_TANARRI) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#0(#RTanar'ri#0)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_POWER_LICH) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#0(#GLich#0)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT) && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf, "#y(#0Death Knight#y)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_HIGHLANDER)
 && IS_HERO(victim) && IS_HERO(ch))
 	strcat( buf,"#C(#ySamu#Rrai#C)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_PALADIN)
 && IS_HERO(victim) && IS_HERO(ch))
	strcat( buf,"#y(#7Paladin#y)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_PSIONIST)
 && IS_HERO(victim) && IS_HERO(ch))
        strcat( buf,"#C(#PPsionist#C)#n " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_INQUISITOR)
 && IS_HERO(victim) && IS_HERO(ch))
	strcat(buf, "#7(#0Inquisitor#7)#n ");
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_MAGE) && IS_HERO(victim) && IS_HERO(ch))
	strcat( buf,"{{#CBattlemage#n}} " );
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_DROW) && IS_HERO(victim) && IS_HERO(ch))
	strcat(buf, "#P(#0Drow#P)#n ");
    if ( !IS_NPC(ch) && IS_VAMPAFF(ch,VAM_AUSPEX) && 
	 !IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) &&
	  IS_VAMPAFF(victim, VAM_DISGUISED) )
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_SHAPESHIFTER) && IS_HERO(ch))
        strcat( buf,"#0(#RShapeshifter#0)#n");
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_HUNTER)&& IS_HERO(victim) && IS_HERO(ch))
	strcat(buf, "#o(#0Hunter#7)#n ");

if (!IS_NPC(victim))
    {
	strcat( buf, "(");
	strcat( buf, victim->name);
	strcat( buf, ") ");
    }
if (victim->embracing!= NULL)
{
sprintf(buf7,"\n\r...%s is holding %s in a stern embrace!",victim->name,victim->embracing->name);
}
else if (victim->embraced!=NULL)
{
sprintf(buf7,"\n\r...%s is being embraced by %s!",victim->name,victim->embraced->name);
}
    if ( IS_EXTRA(victim, EXTRA_ROT))
     {
        if (IS_NPC(victim)) 
          sprintf(buf6,"\n\r...%s is coated with a layer of rotten skin!",victim->short_descr);
        else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
           sprintf(buf6,"\n\r...%s is coated with a layer of rotten skin!",victim->morph);
        else 
           sprintf(buf6,"\n\r...%s is coated with a layer of rotten skin!",victim->name);
       }


    if ( IS_AFFECTED(victim, AFF_FLAMING) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf2, "\n\r...%s is engulfed in blazing flames!", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf2, "\n\r...%s is engulfed in blazing flames!", victim->morph );
	else
	    sprintf( buf2, "\n\r...%s is engulfed in blazing flames!", victim->name );
    }

    if ( !IS_NPC(victim) && IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
    {
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    sprintf( buf3, "...%s is gagged and blindfolded, kinky!", victim->morph );
	else if (IS_EXTRA(victim,GAGGED))
	    sprintf( buf3, "...%s is gagged!", victim->morph );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    sprintf( buf3, "...%s is blindfolded!", victim->morph );
    }
    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
    {
	strcat( buf, victim->morph );
	strcat( buf, " is lying here." );
    	strcat( buf, buf2 );
    	strcat( buf, buf3 );
    	strcat( buf, buf6 );
        strcat(buf,buf7);   
     strcat( buf, "\n\r" );
    	buf[5] = UPPER(buf[5]);
    	send_to_char( buf, ch );
	return;
    }
    
    if ( IS_EXTRA(victim, TIED_UP) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf3, "\n\r...%s is tied up", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf3, "\n\r...%s is tied up", victim->morph );
	else
	    sprintf( buf3, "\n\r...%s is tied up", victim->name );
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, ", gagged and blindfolded!" );
	else if (IS_EXTRA(victim,GAGGED))
	    strcat( buf3, " and gagged!" );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, " and blindfolded!" );
	else
	    strcat( buf3, "!" );
    }

    if ( IS_AFFECTED(victim, AFF_WEBBED) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->morph );
	else
	    sprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->name );
	strcat( buf3, buf4 );
    }

    if ( !IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH) )
	strcat( buf, victim->morph );
    else if ( victim->position == POS_STANDING && victim->long_descr[0] != '\0' && (mount = victim->mount) == NULL )
    {
	strcat( buf, victim->long_descr );
	send_to_char( buf, ch );
	if ( IS_NPC(ch) || !xIS_SET(ch->act, PLR_BRIEF) )
	{
	    if ( IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) ) 
		act( "...$N is surrounded by a crackling shield of #ylightning#n.", ch,NULL,victim,TO_CHAR, FALSE );
	    if ( IS_ITEMAFF(victim, ITEMA_FIRESHIELD) ) 
		act( "...$N is surrounded by a burning shield of #Rf#yi#Rr#ye#n.", ch,NULL,victim,TO_CHAR, FALSE );
	    if ( IS_ITEMAFF(victim, ITEMA_ICESHIELD) ) 
		act( "...$N is surrounded by a shimmering shield of #Cice#n.", ch,NULL,victim,TO_CHAR, FALSE );
	    if ( IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) ) 
		act( "...$N is surrounded by a bubbling shield of #La#Rc#Li#Rd#n.", ch,NULL,victim,TO_CHAR, FALSE );
	    if ( IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) ) 
		act( "...$N is surrounded by a swirling shield of #0c#Rh#0a#Ro#0s#n.", ch,NULL,victim,TO_CHAR, FALSE );
	    if ( IS_ITEMAFF(victim, ITEMA_REFLECT) ) 
		act( "...$N is surrounded by a flickering shield of #0darkness#n.", ch,NULL,victim,TO_CHAR, FALSE );
	}
	return;
    }
    else
    	strcat( buf, PERS( victim, ch ) );

    if ((mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
	if (IS_NPC(mount))
	    sprintf( mount2, " is here riding %s", mount->short_descr );
	else
	    sprintf( mount2, " is here riding %s", mount->name );
	strcat( buf, mount2 );
	if (victim->position == POS_FIGHTING)
	{
	    strcat( buf, ", fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	}
	else strcat( buf, "." );
    }
    else if ( victim->position == POS_STANDING && IS_AFFECTED(victim, AFF_FLYING) )
	strcat( buf, " is hovering here" );
    else if ( victim->position == POS_STANDING && (!IS_NPC(victim) && (IS_VAMPAFF(victim, VAM_FLYING))) )
	strcat( buf, " is hovering here" );
    else if (victim->position == POS_STANDING && (!IS_NPC(victim) &&
IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)))
	strcat(buf, " is hovering here" );
    else
    {
    	switch ( victim->position )
    	{
    	case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
        case POS_MORTAL:   strcat( buf, " is #Rmortally wounded#n." );   break; 
    	case POS_INCAP:    strcat( buf, " is #rincapacitated#n." );      break;
    	case POS_STUNNED:  strcat( buf, " is lying here #Cstunned#n." ); break;
    	case POS_SLEEPING: strcat( buf, " is sleeping here." );      break;
    	case POS_RESTING:  strcat( buf, " is resting here." );       break;
    	case POS_MEDITATING: strcat( buf, " is meditating here." );  break;
    	case POS_SITTING:  strcat( buf, " is sitting here." );       break;
    	case POS_STANDING:
	    if      (!IS_NPC(victim) && victim->stance[0] == STANCE_NORMAL)
		strcat( buf, " is here, crouched in a fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_VIPER)
		strcat( buf, " is here, crouched in a viper fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_CRANE)
		strcat( buf, " is here, crouched in a crane fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_CRAB)
		strcat( buf, " is here, crouched in a crab fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MONGOOSE)
		strcat( buf, " is here, crouched in a mongoose fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_BULL)
		strcat( buf, " is here, crouched in a bull fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MANTIS)
		strcat( buf, " is here, crouched in a mantis fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_DRAGON)
		strcat( buf, " is here, crouched in a dragon fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_TIGER)
		strcat( buf, " is here, crouched in a tiger fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MONKEY)
		strcat( buf, " is here, crouched in a monkey fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_SWALLOW)
		strcat( buf, " is here, crouched in a swallow fighting stance." );
            else if (!IS_NPC(victim) && victim->stance[0] == STANCE_XKILLA)
                strcat( buf, " is here, crouched in a leopard fighting stance." );
	    else
		strcat( buf, " is here." );
	    break;
    	case POS_FIGHTING:
	    strcat( buf, " is here, fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	    break;
	}
    }

    strcat( buf, buf2 );
    strcat( buf, buf3 );
    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    if ( IS_SET(victim->tag_flags,TAG_RED) ) 
	act("...$N is playing freeze tag and is on the #1[RED]#n team.",ch,NULL,victim,TO_CHAR, FALSE );
    if ( IS_SET(victim->tag_flags,TAG_BLUE)) 
	act("...$N is playing freeze tag and is on the #4[BLUE]#n team.",ch,NULL,victim,TO_CHAR, FALSE );

    if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_BRIEF) ) return;

    if ( IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) ) 
	act( "...$N is surrounded by a crackling shield of lightning.", ch,NULL,victim,TO_CHAR, FALSE );
    if ( IS_ITEMAFF(victim, ITEMA_FIRESHIELD) ) 
	act( "...$N is surrounded by a burning shield of fire.", ch,NULL,victim,TO_CHAR, FALSE );
    if ( IS_ITEMAFF(victim, ITEMA_ICESHIELD) ) 
	act( "...$N is surrounded by a shimmering shield of ice.", ch,NULL,victim,TO_CHAR, FALSE );
    if ( IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) ) 
	act( "...$N is surrounded by a bubbling shield of acid.", ch,NULL,victim,TO_CHAR, FALSE );
    if ( IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) ) 
	act( "...$N is surrounded by a swirling shield of chaos.", ch,NULL,victim,TO_CHAR, FALSE );
    if ( IS_ITEMAFF(victim, ITEMA_REFLECT) ) 
	act( "...$N is surrounded by a flickering shield of darkness.", ch,NULL,victim,TO_CHAR, FALSE );
    return;
}



void evil_eye( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf [MAX_STRING_LENGTH];
    int tabletype;

    if (IS_NPC(victim)) return;
    if (victim == ch) return;
    if (ch->level < 3 || victim->level < 3) return;
    if (victim->power[DISC_DAEM_DISC] < 2) return;
    if (victim->power[DISC_DAEM_DISC] < dice(1, 10)) return;
    if (!str_suffix(victim->poweraction,"gi") ||
	!str_suffix(victim->poweraction,"qui"))
	return;
    if (is_safe(ch, victim) ) return;
    if (victim->powertype != NULL && strlen(victim->powertype) > 1)
    {
	sprintf(buf,"\n\r%s\n\r",victim->powertype);
	send_to_char(buf,ch);
    }
    if (IS_SET(victim->spectype,EYE_SELFACTION) && victim->poweraction != NULL)
	interpret(victim,victim->poweraction);
    if (IS_SET(victim->spectype,EYE_ACTION) && victim->poweraction != NULL)
	interpret(ch,victim->poweraction);
    if (IS_SET(victim->spectype,EYE_SPELL) && victim->specpower > 0)
    {
	tabletype = skill_table[victim->specpower].target;
	(*skill_table[victim->specpower].spell_fun) (victim->specpower,victim->spl[tabletype],victim,ch);
    }
    return;
}


void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    double percent;
    bool found;

    if ( can_see( victim, ch ) )
    {
	act( "$n looks at you.", ch, NULL, victim, TO_VICT, FALSE    );
      if (ch == victim)
	act( "$n looks $mself over.",  ch, NULL, victim, TO_NOTVICT, FALSE );
      else
	act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT, FALSE );
    }

    if (!IS_NPC(ch) && IS_HEAD(victim,LOST_HEAD))
    {
	act( "$N is lying here.", ch, NULL, victim, TO_CHAR, FALSE );
	return;
    }
    if ( victim->description[0] != '\0' )
    {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR, FALSE );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, PERS(victim, ch) );

         if ( percent >= 100 ) strcat( buf, " is in perfect health.\n\r"  );
    else if ( percent >=  90 ) strcat( buf, " is slightly scratched.\n\r" );
    else if ( percent >=  80 ) strcat( buf, " has a few bruises.\n\r"     );
    else if ( percent >=  70 ) strcat( buf, " has some cuts.\n\r"         );
    else if ( percent >=  60 ) strcat( buf, " has several wounds.\n\r"    );
    else if ( percent >=  50 ) strcat( buf, " has many nasty wounds.\n\r" );
    else if ( percent >=  40 ) strcat( buf, " is bleeding freely.\n\r"    );
    else if ( percent >=  30 ) strcat( buf, " is covered in blood.\n\r"   );
    else if ( percent >=  20 ) strcat( buf, " is leaking guts.\n\r"       );
    else if ( percent >=  10 ) strcat( buf, " is almost dead.\n\r"        );
    else                       strcat( buf, " is DYING.\n\r"              );

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
 
    if (!IS_NPC( victim ))
    {
    	if (IS_AFFECTED(victim, AFF_INFRARED) || IS_VAMPAFF(victim, VAM_NIGHTSIGHT)) act("$N's eyes are glowing bright red.",ch,NULL,victim,TO_CHAR, FALSE);
    	if (IS_AFFECTED(victim, AFF_FLYING)) act("$N is hovering in the air.",ch,NULL,victim,TO_CHAR, FALSE);
		if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION))
			act("$N is hovering in the air.", ch,NULL,victim,TO_CHAR, FALSE);
		if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_ARMS))
			act("$N has 6 extra spidery arms protruding from $S body.",ch,NULL,victim,TO_CHAR, FALSE);
	    if (IS_VAMPAFF(victim, VAM_FANGS)) act("$N has a pair of long, pointed fangs.",ch,NULL,victim,TO_CHAR, FALSE);
	    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim, VAM_CLAWS))
			act("$N has razor sharp claws protruding from under $S finger nails.",ch,NULL,victim,TO_CHAR, FALSE);
		else if (IS_CLASS(victim, CLASS_NINJA) && IS_VAMPAFF(victim, VAM_CLAWS))
			act("$N has IronClaws protruding from $S wrists.",ch,NULL,victim,TO_CHAR, FALSE);
	    else if (!IS_NPC(victim) && IS_VAMPAFF(victim, VAM_CLAWS))
			act("$N has razor sharp talons extending from $S fingers.",ch,NULL,victim,TO_CHAR, FALSE);
	    if (!IS_NPC(victim))
	    {
			if (!IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->newbits, NEW_SKIN)) act("$N has scales all over $S body.",ch,NULL,victim,TO_CHAR, FALSE);
			if (IS_VAMPAFF(victim, VAM_HORNS)) act("$N has a pair of pointed horns extending from $S head.",ch,NULL,victim,TO_CHAR, FALSE);
			if (IS_VAMPAFF(victim, VAM_WINGS))
			{
	    		if (IS_DEMAFF(victim, DEM_UNFOLDED))
					act("$N has a pair of batlike wings spread out from behind $S back.",ch,NULL,victim,TO_CHAR, FALSE);
	    		else
			 		act("$N has a pair of batlike wings folded behind $S back.",ch,NULL,victim,TO_CHAR, FALSE);
			}
			if (IS_VAMPAFF(victim, VAM_HEAD))
			 {
			   act("$N's head resembles that of a fierce lion.",ch,NULL,victim,TO_CHAR, FALSE);
			 }
		   if (IS_VAMPAFF(victim, VAM_TAIL))
				act("$N has a sharp tail extending from $S spine.",ch,NULL,victim,TO_CHAR, FALSE); 
		   if (IS_VAMPAFF(victim, VAM_EXOSKELETON))
				act("$N's skin is covered by a hard exoskeleton.",ch,NULL,victim,TO_CHAR, FALSE);
 
	  	}
	}
	found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR, FALSE );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    if (IS_NPC(ch) || ch->pcdata->chobj == NULL || ch->pcdata->chobj != obj)
	    {
	    	send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    	send_to_char( "\n\r", ch );
	    }
	    else
		send_to_char( "you\n\r", ch);
	}
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    &&   !IS_HEAD(victim, LOST_HEAD)
    &&   number_percent( ) < ch->pcdata->learned[gsn_peek] )
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
    }

    return;
}



void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( IS_IMMORTAL(ch) || IS_IMMORTAL(rch) )
	{
	    if ( !IS_NPC(rch)
	    &&   xIS_SET(rch->act, PLR_WIZINVIS)
	    &&   get_trust( ch ) < get_trust( rch ) )
		continue;
	}
	else
	{
	    if ( !IS_NPC(rch)
	    && ( xIS_SET(rch->act, PLR_WIZINVIS)
	    ||   IS_ITEMAFF(rch, ITEMA_VANISH) )
	    &&   !xIS_SET(ch->act, PLR_HOLYLIGHT)
	    &&   !IS_ITEMAFF(ch, ITEMA_VISION) )
		continue;
	}

	if ( !IS_NPC(rch) && IS_HEAD(rch,LOST_HEAD))
	    continue;

	if ( !IS_NPC(rch) && IS_EXTRA(rch,EXTRA_OSWITCH))
	    continue;

	if ( can_see( ch, rch ) )
	    show_char_to_char_0( rch, ch );
	else if ( room_is_dark( ch->in_room )
	&&      ( IS_AFFECTED(rch, AFF_INFRARED)||(!IS_NPC(rch)&&IS_VAMPAFF(rch, VAM_NIGHTSIGHT))))
	{
	    send_to_char( "You see glowing #Rred#n eyes watching YOU!\n\r", ch );
	}
    }

    return;
} 



bool check_blind( CHAR_DATA *ch )
{

    if ( IS_EXTRA(ch, BLINDFOLDED) )
    {
        send_to_char( "You can't see a thing through the blindfold!\n\r", ch );
        return FALSE;
    }     

    if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) )
	return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
    {
	send_to_char( "You have no eyes!\n\r", ch );
	return FALSE;
    }

    if ( IS_AFFECTED(ch, AFF_BLIND) && !IS_AFFECTED(ch, AFF_SHADOWSIGHT))
    {
	send_to_char( "You can't see a thing!\n\r", ch );
	return FALSE;
    }

    return TRUE;
}



void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *wizard;
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    char *pdesc;
    int door;
    bool found;

    if ( ch->desc == NULL && (wizard = ch->wizard) == NULL) return;

    if (ch->in_room == NULL) return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( IS_SET(ch->flag2,AFF_TOTALBLIND))
    {
    send_to_char("You can't see anything because you're blinded!\n\r",ch);
    return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch) && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) &&
	!IS_ITEMAFF(ch, ITEMA_VISION) && !IS_IMMORTAL(ch) && !IS_CLASS(ch, CLASS_CYBORG) &&
!(IS_CLASS(ch, CLASS_DROW)))
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	return;
    }

    if ( !IS_NPC(ch)
    &&   !xIS_SET(ch->act, PLR_HOLYLIGHT)
    &&   !IS_ITEMAFF(ch, ITEMA_VISION)
    &&   !IS_VAMPAFF(ch, VAM_NIGHTSIGHT)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE)
    &&   !(ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
    &&   !IS_NPC(ch) && ch->pcdata->chobj != NULL
    &&   ch->pcdata->chobj->in_obj != NULL)
    &&   room_is_dark( ch->in_room ) )
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	char heh[MSL];
	/* 'look' or 'look auto' */
	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	    act( "$p",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR, FALSE);
    	else if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "The shadow plane\n\r", ch );
        else if (!xIS_SET(ch->act, PLR_MAP) && !xIS_SET(ch->act, PLR_BRIEF))
        {
	    sprintf(buf, "%s", ch->in_room->name);
	    send_to_char( buf, ch );
	        if (!IS_NPC(ch) && ch->level > 6)
        	{
	        sprintf(heh, "  #R[#7%d#R]#n\n\r",ch->in_room->vnum);
        	stc(heh, ch);
        	}
	        if (!IS_NPC(ch) && ch->level < 7)
		{
		stc("\n\r", ch);
		}

	if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOEXIT))
	    do_exits( ch, "auto" );
	}

        if (!xIS_SET(ch->act, PLR_MAP) && xIS_SET(ch->act, PLR_BRIEF))
        {
            sprintf(buf, "%s", ch->in_room->name);
            send_to_char( buf, ch );
                if (!IS_NPC(ch) && ch->level > 6)
                {
                sprintf(heh, "  #R[#7%d#R]#n\n\r",ch->in_room->vnum);
                stc(heh, ch);
                }
                if (!IS_NPC(ch) && ch->level < 7)
                {
                stc("\n\r", ch);
                }

        if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOEXIT))
            do_exits( ch, "auto" );

        }

	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	{
	    act( "You are inside $p.",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR, FALSE);
	    show_list_to_char( ch->pcdata->chobj->in_obj->contains, ch, FALSE, FALSE );
	}
	else if ( (arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) && IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "You are standing in complete darkness.\n\r", ch );
	else if ( !IS_NPC(ch) && 
( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) )
	{
            if (xIS_SET(ch->act, PLR_MAP) && !xIS_SET(ch->act, PLR_BRIEF))
            {
             draw_map(ch, ch->in_room->description);
            }
            else if (xIS_SET(ch->act, PLR_MAP) && xIS_SET(ch->act, PLR_BRIEF))
            {
             draw_map(ch, "");
            }
            else
            {
	     if (!xIS_SET(ch->act, PLR_BRIEF))
	     {
              send_to_char( "#n", ch );
              send_to_char( ch->in_room->description , ch );
	     }
            if ( ch->in_room->sector_type != SECT_INSIDE && IS_OUTSIDE(ch))
                {
                    if (number_range(0,20) < 5 )
                    {
                        if (weather_info.sky == SKY_RAINING)
                        send_to_char("#cA cool wind howls and brings a light rain.#n\n\r",ch);
                        else if (weather_info.sky == SKY_CLOUDY)
                            send_to_char("#cA cold wind is blowing.#n\n\r",ch);
                        else if (weather_info.sky == SKY_LIGHTNING)
                       send_to_char("#cStreaks of #7Lightning #clight up the #Csky#n\n\r",ch);
                    }
                }
            }
            if (ch->in_room->blood > 0)
	    {
            if (ch->in_room->blood == 1000)
		{
                sprintf(buf,"You notice that the room is completely drenched in blood.\n\r");
		send_to_char(buf,ch);
		}
            else if (ch->in_room->blood > 750)
		{
		sprintf(buf,"You notice that there is a very large amount of blood around the room.\n\r");
                send_to_char(buf,ch);
                }
            else if (ch->in_room->blood > 500)
		{
        	sprintf(buf,"You notice that there is a large quantity of blood around the room.\n\r");
                send_to_char(buf,ch);
                }
            else if (ch->in_room->blood > 250)
		{
                sprintf(buf,"You notice a fair amount of blood on the floor.\n\r");
                send_to_char(buf,ch);
                }
            else if (ch->in_room->blood > 100)
		{
                sprintf(buf,"You notice several blood stains on the floor.\n\r");
                send_to_char(buf,ch);
                }
            else if (ch->in_room->blood > 50)
		{
                sprintf(buf,"You notice a few blood stains on the floor.\n\r");
                send_to_char(buf,ch);
                }
            else if (ch->in_room->blood > 25)
		{
                sprintf(buf,"You notice a couple of blood stains on the floor.\n\r");
                send_to_char(buf,ch);
                }
            else if (ch->in_room->blood > 0)
		{
                sprintf(buf,"You notice a few drops of blood on the floor.\n\r");
                send_to_char(buf,ch);
                }
	    }
      }
            if (IS_SET(ch->in_room->room_flags, ROOM_FLAMING))
            send_to_char("..This room is engulfed in flames!\n\r",ch);

	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );

        for (door=0 ; door < 6 ; door++)
	{	
	    if (ch->in_room == NULL) continue;
	    if (ch->in_room->exit[door] == NULL) continue;

	    if (IS_SET(ch->in_room->exit[door]->exit_info,EX_PRISMATIC_WALL)) 	    
	    {
		sprintf(buf, "     You see a shimmering wall of many colours %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ICE_WALL))
	    {
		sprintf(buf, "     You see a glacier of ice %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_CALTROP_WALL))
	    {
		sprintf(buf, "     You see a wall of caltrops %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_FIRE_WALL))
	    {
		sprintf(buf, "     You see a blazing wall of fire %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_SWORD_WALL))
	    {
		sprintf(buf, "     You see a spinning wall of swords %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_MUSHROOM_WALL))
	    {
		sprintf(buf, "     You see a vibrating mound of mushrooms %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_IRON_WALL))
	    {
		sprintf(buf, "    You see a solid wall of iron %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ASH_WALL))
	    {
		sprintf(buf, "    You see a deadly wall of ash %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	}
	show_char_to_char( ch->in_room->people,   ch );
	return;
//	if (xIS_SET(ch->act, PLR_BRIEF))
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) )
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "It doesn't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    if (obj->value[2] == 1 || obj->value[2] == 3)
	    {
		send_to_char( "It seems to be closed.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in it.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "It contains a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "It's about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "It's about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "It is almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "It's completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow it is MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p contains:", ch, obj, NULL, TO_CHAR, FALSE );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
	evil_eye(victim,ch);
	return;
    }

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if (!IS_NPC(vch) && !str_cmp(arg1,vch->morph))
	    {
		show_char_to_char_1( vch, ch );
		evil_eye(vch,ch);
		return;
	    }
	    continue;
	}
    }

    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
    {
	obj = get_obj_in_obj(ch,arg1);
	if (obj != NULL)
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    pdesc = get_extra_descr( arg1, ch->in_room->first_extradesc );
    if ( pdesc != NULL )
    {
	send_to_char( pdesc, ch );
	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }
/*
    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );
*/

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR, FALSE );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR, FALSE );
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
	else
	{
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
    }
    else
    {
	if ((pexit = ch->in_room->exit[door]) == NULL) return;
	if ((pRoomIndex = pexit->to_room) == NULL) return;
	location = ch->in_room;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
	char_from_room(ch);
	char_to_room(ch,location);
    }

    return;
}



void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    ROOM_INDEX_DATA *pRoomIndex;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

//    do_look( ch, arg );

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	switch ( obj->item_type )
	{
	default:
	    break;
        case ITEM_WEAPON:
	{
             if (obj->condition >= 100)
            sprintf( buf, "You notice that %s is in perfect condition.\n\r",obj->short_descr );
        else if (obj->condition >= 75 )
            sprintf( buf, "You notice that %s is in good condition.\n\r",obj->short_descr );
        else if (obj->condition >= 50 )
            sprintf( buf, "You notice that %s is in average condition.\n\r",obj->short_descr );
        else if (obj->condition >= 25 )
            sprintf( buf, "You notice that %s is in poor condition.\n\r",obj->short_descr );
        else
            sprintf( buf, "You notice that %s is in awful condition.\n\r",obj->short_descr );
        send_to_char(buf,ch);
	break;
	}
	case ITEM_ARMOR:
        {
             if (obj->condition >= 100)
            sprintf( buf, "You notice that %s is in perfect condition.\n\r",obj->short_descr );
        else if (obj->condition >= 75 )
            sprintf( buf, "You notice that %s is in good condition.\n\r",obj->short_descr );
        else if (obj->condition >= 50 )
            sprintf( buf, "You notice that %s is in average condition.\n\r",obj->short_descr );
        else if (obj->condition >= 25 )
            sprintf( buf, "You notice that %s is in poor condition.\n\r",obj->short_descr );
        else
            sprintf( buf, "You notice that %s is in awful condition.\n\r",obj->short_descr );
        send_to_char(buf,ch);
	break;
        }

	case ITEM_CONTAINER:
	    {
            send_to_char( "When you look inside, you see:\n\r", ch );
            sprintf( buf, "in %s", arg );
            do_look( ch, buf );
	    break;
            }

	case ITEM_CORPSE_NPC:
	    {
            send_to_char( "When you look inside, you see:\n\r", ch );
            sprintf( buf, "in %s", arg );
            do_look( ch, buf );
	    break;
            }
	case ITEM_TELEPORTER:
            {
	    if (ch->fight_timer > 0)
	    {
		stc("You are too occupied to make out anything special.\n\r",ch);
		break;
	    }
	    pRoomIndex = get_room_index(obj->value[0]);
	    if (pRoomIndex == NULL)
            {
                send_to_char( "It does not seem to do anything.\n\r", ch );
                break;
            }
	    act( "$n slowly fades into $p.", ch, obj, NULL, TO_ROOM, FALSE );
            send_to_char( "Your body begins to dissolve.\n\r", ch );
            char_from_room(ch);
            char_to_room(ch,pRoomIndex);
            send_to_char( "Your body reforms from nothing.\n\r", ch );
	    do_look(ch,"");
            break;
            }
	case ITEM_CORPSE_PC:
	    {
	    send_to_char( "When you look inside, you see:\n\r", ch );
	    sprintf( buf, "in %s", arg );
	    do_look( ch, buf );
	    break;
	    }
	}
    }

    return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    extern char * const cdir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;
    buf[0] = '\0';
    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

    strcpy( buf, fAuto ? "#R[#GExits#7:#C" : "Obvious exits:\n\r" );

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL)
	{
	    found = TRUE;
	    if ( fAuto )
	    {
                if (IS_SET(pexit->exit_info, EX_CLOSED) )
                {
                strcat( buf, " " );
		strcat( buf, (cdir_name[door]) );
                }
		else
		{
		strcat( buf, " " );
		strcat( buf, dir_name[door] );
		}
	    }
	    else
	    {
		sprintf( buf + strlen(buf), "%-5s - %s\n\r",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->to_room )
			?  "Too dark to tell"
			: pexit->to_room->name
		    );
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto ? " none" : "None.\n\r" );

    if ( fAuto )
	strcat( buf, "#R]#n\n\r" );

    send_to_char( buf, ch );
    return;
}



const char * transformation_message[2][12] =
{
{
"\n\rYour body shudders and convulses as your form transforms. Arms and legs grow out of your slimy torso, your skin turns a sickly veined green.\n\r" ,
"\n\rYour demonic form is reduced to a pool of pulsating jelly, the fires of hell heat and char your semi-liquid form until you stiffen.  Limbs grow and spiny talons extend from your arms and back.\n\r" ,
"\n\rYou start the transformation.  Your gargoylish apperance is changed as you grow another two feet in height.  Scales, dripping with acid, begin to cover your skin.\n\r" ,
"\n\rThe fires of hell surround you once more.  Sharp claws start to grow from your hands, and you grow a viscious barbed tail.  Small flames play over your burnt and blackened skin.\n\r" ,
"\n\rYour next form is that of a sickly, gaunt skeleton.  Your leathery flesh held tight over a demonic frame.  A huge tail, with a poisonous barb on the end protrudes out of the base of your spine.\n\r" ,
"\n\rAs you transform, you grow another four feet in height. This time, wicked barbs cover your entire body.  You find you have the ability to wield weapons, and use armor.\n\r" ,
"\n\rWhat?  What is happening, with the transformation into an Erinyes, your form is no longer repulsive and demonic, but you are now a beautiful mortal, with long auburn hair and huge white wings growing out of your back.  You can see the world again with your mortal eyes, but the  passions of a demon will always stay.\n\r",
"\n\rYour skin melts and you transform once more, but this time you become a gruesome pug nosed, vile dwarf-like creature with large leathery wings and a mouth filled of razor sharp teeth, an Amnizu.\n\r" ,
"\n\rThe intense heat of the fires of Baator surround you. Your skin is wrenched apart and you transform into a grotesque 9-foot tall monstrosity with huge wings, a snaking, prehensile tail, and a long barbed whip.  A true demon, a Cornugon.\n\r",
"\n\rThe power of Baator now fills your very soul.  You grow again as the chill winds of Caina tear the flesh from your bones.  You look to see what you have become, you look alien, with a twelve foot long icy insect-like body, your head bulging with multi-faceted eyes and you have a long tail covered with razor-sharp spikes.\n\r",
"\n\rAaargh!  You are cast into the pit of flame. The fires, more intense than ever before sear and scar your flesh as it bubbles and boils. You have become the most terrible demon, the Pit Fiend, a huge bat winged  humanoid, twelve feet tall, with large fangs that drip with a vile green liquid and a hulking red scaly body that bursts into flame when you are angered or excited.\n\r",
"\n\rYour fiery skin blackens as you are infused with the embodiment of ultimate evil.  You are the most powerful demon, the lord of all.  You are everything, nothing can oppose you.  Twenty foot tall, your torso drips with the vile acid of the Abyss.  A ravenous hunger is gnawing at your bones.  Welcome to Immortality..."
	},
	{
"Your first transformation is quite remarkable, you grow to over seven feet tall, and your skin starts to dull and lose all signs of vitality.  Your teeth begin to extend.  The world starts to look different, you have left mortality behind.\n\r",
"You start to grow, the base of your spine slowly extends into a short stump of a tail.  The metamorphosis is tearing you apart.  Your skin begin to harden, and your hands twist into claws.\n\r",
"Once again the painful metamorphosis strikes.  Your bones rend through your scaled flesh as jagged spines shoot through your back.  You feel as if you are being wrenched apart.  You are now over ten feet tall, and your skin is covered with thick green scales.\n\r",
"You transform once again.  This time a small pair of wings burst through your back.  Your hands and feet have become viscious claws, and your nose has extended into a snout.  Long jagged teeth begin to grow in your mouth, and your tail now reaches the floor.\n\r",
"You fall to the floor in agony.  Clutching your chest as the transformation strikes.  You grow to over fifteen feet in length, towering above all the mortals that you have left behind.  Impenetrable scales now cover your body,  and smoke issues from your nostrils when you breathe.\n\r",
"You grow to over twenty feet in length, your tail and claws grow at an alarming rate.  Your wings are now so powerful that you can fly.  Nobody can oppose you now.  Suddenly it hits you, a hunger so intense that you cannot ignore it. You must feed.\n\r",
"",
"",
"",
"",
"You have reached the pinnacle of power for a dragon.  As your body undergoes the by now familiar transformation you grow to over fifty feet tall, with incredibly sharp claws and fangs, a powerful tail and a fiery breath weapon. Nobody can oppose you now.  You are the lord of the realm.\n\r",
""
	}
};


void do_weaplist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    if (IS_NPC(ch)) return;
 
 
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char(
 "                    Weapon, Stance, and Spell Skill           \n\r",ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char(
 "                            Weapon\n\r",ch);
sprintf( buf,
"Unarm:%-4d Slice:%-4d Stab:%-4d Slash:%-4d Whip:%-4d Claw:%-4d Blast:%-4d\n\r",
ch->wpn[0],ch->wpn[1],ch->wpn[2],ch->wpn[3],ch->wpn[4],ch->wpn[5],ch->wpn[6]);
send_to_char(buf,ch);
sprintf( buf,
"Pound  :%-4d Crush  :%-4d Grep   :%-4d Bite    :%-4d Pierce:%-4d Suck   :%-4d\n\r",
ch->wpn[7],ch->wpn[8],ch->wpn[9],ch->wpn[10],ch->wpn[11],ch->wpn[12]);
send_to_char(buf,ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char(
 "                            Stances\n\r",ch);
sprintf( buf,
"Viper : %-4d Crane: %-4d Crab  : %-4d Mongoose: %-4d Bull: %-4d Mantis: %-4d\n\r",
ch->stance[1],ch->stance[2],ch->stance[3],ch->stance[4],ch->stance[5],ch->stance[6]);
send_to_char(buf,ch);
sprintf( buf,
"Dragon: %-4d Tiger: %-4d Monkey: %-4d Swallow: %-4d Leopard: %-4d\n\r",
ch->stance[7],ch->stance[8],ch->stance[9],ch->stance[10],ch->stance[11]);
send_to_char(buf,ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char(
 "                            Magic\n\r",ch);
sprintf( buf,
"Purple: %-4d  Red: %-4d  Blue: %-4d  Green: %-4d  Yellow: %-4d\n\r",ch->spl[0],ch->spl[1],ch->spl[2],ch->spl[3],ch->spl[4]);
send_to_char(buf,ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
 
 
return;
}

 
void do_far( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   ROOM_INDEX_DATA *chroom;
   ROOM_INDEX_DATA *victimroom;
   char arg1[MAX_STRING_LENGTH];

   argument = one_argument (argument, arg1);

    if ( IS_NPC(ch) )
	return;

	if (arg1[0] == '\0')
	{
	   send_to_char("Who do you wish to use farcommand on?\n\r",ch);
	   return;
	}
/*  
        if (arg2[0] == '\0')
	{
	   send_to_char("What do you wish to do to them?\n\r",ch);
	   return;
	}

	if (arg3[0] == '\0')
	{
	   send_to_char("Possess or command who?\n\r",ch);
	   return;
	}
*/
  	if (!IS_CLASS(ch, CLASS_VAMPIRE))
  	{
  	send_to_char("Huh?\n\r", ch);
  	return;
  	}
  	
  	if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_PRES] < 9 )
  	{
  	send_to_char("You need level 9 Presence to use this power.\n\r", ch);
	return;
	}

      if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
 
     if ( !IS_NPC(victim) )
	{
	   send_to_char("Their mind is too strong!\n\r",ch);
	   return;
	}
    if ( victim->level > 250)
    {
    send_to_char("They are too big!\n\r",ch);
    return;
    } 
  
    chroom = ch->in_room;
    victimroom = victim->in_room;
    char_from_room(ch);
    char_to_room(ch,victimroom);
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    send_to_char("You possess their body.\n\r",ch);
    WAIT_STATE(ch, 50);
    char_from_room(ch);
    char_to_room(ch,chroom);  	
    
    return;
}

void do_stat( CHAR_DATA *ch, char *argument )
{
    char buf   [MAX_STRING_LENGTH];
    char lin   [MAX_STRING_LENGTH];
    char age   [MAX_STRING_LENGTH];
    int blood;
    int bloodpool;

    if (ch->generation < 0)
	ch->generation = 0;

    {
    if (IS_SET(ch->newbits, NEW_TIDE))
    bloodpool = (3000 / (ch->generation +1));
    else bloodpool = (2000 / (ch->generation +1));
    }

    blood = ch->pcdata->condition[COND_THIRST];
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_VAMPIRE)) return;
    if (ch->pcdata->rank == AGE_ANCILLA)         sprintf( age, "Ancilla");
    else if (ch->pcdata->rank == AGE_CHILDE)     sprintf( age, "Childe");
    else if (ch->pcdata->rank == AGE_NEONATE)    sprintf( age, "Neonate");
    else if (ch->pcdata->rank == AGE_ELDER)      sprintf( age, "Elder");
    else if (ch->pcdata->rank == AGE_METHUSELAH) sprintf( age, "Methuselah");
    else if( ch->pcdata->rank == AGE_LA_MAGRA)   sprintf( age, "La Magra");
    else if( ch->pcdata->rank == AGE_TRUEBLOOD)  sprintf( age, "TrueBlood");
    sprintf( lin,
"---------------------------------------------------------------------------\n\r");
    send_to_char( lin,ch);
    send_to_char(
"                              Vampire Status\n\r",ch);
send_to_char(lin,ch);
sprintf(buf,
"Generation:%d  Bloodpool:%d  Age:%s  Blood:%d\n\r",
ch->generation,bloodpool,age,blood);
send_to_char(buf,ch);
send_to_char(lin,ch);
send_to_char(
"                              Disciplines\n\r",ch);
send_to_char(lin,ch);
sprintf(buf,
"Animalism:    [%d]             Celerity:   [%d]             Fortitude: [%d]\n\r",
ch->power[DISC_VAMP_ANIM],ch->power[DISC_VAMP_CELE],
ch->power[DISC_VAMP_FORT]);
send_to_char(buf,ch);
sprintf(buf,
"Obtenebration:[%d]             Presence:   [%d]             Quietus:   [%d]\n\r",
ch->power[DISC_VAMP_OBTE],ch->power[DISC_VAMP_PRES],
ch->power[DISC_VAMP_QUIE]);
send_to_char(buf,ch);
sprintf(buf,
"Thaumaturgy:  [%d]             Auspex:     [%d]             Dominate:  [%d]\n\r",
ch->power[DISC_VAMP_THAU],ch->power[DISC_VAMP_AUSP],
ch->power[DISC_VAMP_DOMI]);
send_to_char(buf,ch);
sprintf(buf,
"Obfuscate:    [%d]             Potence:    [%d]             Protean:   [%d]\n\r",
ch->power[DISC_VAMP_OBFU],ch->power[DISC_VAMP_POTE],
ch->power[DISC_VAMP_PROT]);
send_to_char(buf,ch);
sprintf(buf,
"Serpentis:    [%d]             Vicissitude:[%d]             Daimoinon: [%d]\n\r",
ch->power[DISC_VAMP_SERP],ch->power[DISC_VAMP_VICI],
ch->power[DISC_VAMP_DAIM]);
send_to_char(buf,ch);
sprintf(buf,			 
"Chimerstry:   [%d]             Obeah:      [%d]             Melpominee:[%d]\n\r",
ch->power[DISC_VAMP_CHIM], ch->power[DISC_VAMP_OBEA], ch->power[DISC_VAMP_MELP]);
send_to_char(buf,ch);
sprintf(buf,
"Necromancy:   [%d]                                         Thanatosis:[%d]\n\r",
ch->power[DISC_VAMP_NECR], ch->power[DISC_VAMP_THAN] );
stc(buf,ch);
send_to_char(lin,ch);
 
return;
}


void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char ss1[MAX_STRING_LENGTH];
    char ss2[MAX_STRING_LENGTH];
    int a_c = char_ac(ch);

	if (IS_NPC(ch)) return;

    if (!IS_NPC(ch) && (IS_EXTRA(ch,EXTRA_OSWITCH) || IS_HEAD(ch,LOST_HEAD)))
	{obj_score(ch,ch->pcdata->chobj);return;}
    sprintf( buf,
	"#n[#0You are #G%s%s#n][#0You have been playing for #G%d #0hours#n]\n\r",
	ch->pcdata->switchname,
	IS_NPC(ch) ? "" : ch->pcdata->title,
	(get_age(ch) - 17) * 2 );
    send_to_char( buf, ch );
    birth_date(ch, TRUE);
    if (!IS_NPC(ch) && IS_EXTRA(ch, EXTRA_PREGNANT)) birth_date(ch, FALSE);

    if ( get_trust( ch ) != ch->level )
    {
	sprintf( buf, "You are trusted at level %d.\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }

    sprintf( buf,
	"#n[#0You have #G%d#0/#G%d #0hit#n][#G%d#0/#G%d#0 mana#n][#G%d#0/#G%d#0 movement#n]\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move);
    send_to_char( buf, ch );

    sprintf( buf,
	"#n[#0You are carrying #G%d#0/#G%d #0items with weight #G%d#0/#G%d#0 kg#n]\n\r",
	ch->carry_number, can_carry_n(ch),
	ch->carry_weight, can_carry_w(ch) );
    send_to_char( buf, ch );

    sprintf( buf,
	"#n[#0Str: #G%d#n][#0Int: #G%d#n][#0Wis: #G%d#n][#0Dex: #G%d#n][#0Con: #G%d#n]\n\r",
	get_curr_str(ch),
	get_curr_int(ch),
	get_curr_wis(ch),
	get_curr_dex(ch),
	get_curr_con(ch) );
    send_to_char( buf, ch );

    sprintf( buf,
	"#n[#0You have scored #G%2.0f#0 exp#n][#G%d#0 primal energy#n]\n\r",
	ch->exp,  ch->practice );
    send_to_char( buf, ch );

    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_DEMON) || IS_SET(ch->special,SPC_CHAMPION)))
    {
	sprintf( buf,
	"#n[#0You have #G%d#0 out of #G%d #0points of demonic power stored#n]\n\r",
	ch->pcdata->stats[DEMON_CURRENT],  ch->pcdata->stats[DEMON_TOTAL] );
	send_to_char( buf, ch );
    }
if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_CYBORG)))
{
 
sprintf( buf,"#n[#0You have #G%d #0spiderdroid points stored#n]\n\r",        ch->pcdata->stats[CYBORG_POWER] );
 send_to_char( buf, ch );
}
	if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_TANARRI)))
	{
	sprintf( buf, "#n[#0You have #G%d #0Tanar'ri points stored#n]\n\r",
 ch->pcdata->stats[TPOINTS] );
	send_to_char( buf, ch);
	}
        if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_PSIONIST)))
        {
        sprintf( buf, "#n[#0You have #G%d #0Psionist points stored#n]\n\r", ch->pcdata->stats[PPOINTS] );
        send_to_char( buf, ch);
        }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON) && ch->generation < 13)
    {
	sprintf( buf,
	"#n[#0You have obtained #G%d #0souls, which you keep in a little jar#n]\n\r",
	ch->pcdata->souls);
	stc(buf, ch);
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PSIONIST))
    {
        sprintf(buf,"#n[#0You have trapped #G%d#0 minds within your mind trap.#n]\n\r"
                , ch->pcdata->souls);
        stc(buf, ch);
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_INQUISITOR) && ch->pcdata->powers[PINQUISITOR] > 0)
    {
        sprintf( buf,
        "#n[#0You have taken #G%d #0souls from the wicked#n]\n\r",
        ch->pcdata->souls);
        stc(buf, ch);
    }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW)) {
	sprintf( buf, "#n[#0You have #G%d#0 drow power points stored#n]\n\r",
	ch->pcdata->stats[DROW_POWER] );
	send_to_char( buf, ch );
	sprintf( buf, "#n[#0You have #G%d #0points of magic resistance#n]\n\r",
	ch->pcdata->stats[DROW_MAGIC] );
	send_to_char( buf, ch ); 
	if (weather_info.sunlight == SUN_DARK) 
	send_to_char("#n[#0You feel strong in the night#n]\n\r", ch );}

    sprintf( buf,
	"#n[#0Auto Exit: #G%s#n][#0Auto Loot: #G%s#n][#0Auto Sac: #G%s#n][#0Auto Split: #G%s#n]\n\r[#0Auto Adamantite: #G%s#n][#0Auto Electrum: #G%s#n][#0Auto Gold: #G%s#n]\n\r",
	(!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOEXIT)) ? "ON" : "OFF",
	(!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOLOOT)) ? "ON" : "OFF",
	(!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOSAC) ) ? "ON" : "OFF",
	(!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOSPLIT))? "ON" : "OFF",
	(!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOSLAB)) ? "ON" : "OFF",
        (!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOELEC)) ? "ON" : "OFF",
        (!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOGOLD)) ? "ON" : "OFF");


    send_to_char( buf, ch );
    
    sprintf( buf, "#n[#0Wimpy set to #G%d #0hit points#n][#0Gold: #G%2.0f#n]", ch->wimpy, ch->gold );
    if ( !IS_NPC(ch) && IS_EXTRA(ch, EXTRA_DONE) )
	strcat(buf,"#n[#0You've done the deed#n]\n\r");
    else strcat(buf,"\n\r");
    send_to_char( buf, ch );


    if(!IS_NPC(ch))
    {
       sprintf(buf,"#n[#0Arena Stats#n][#0Wins: #G%d#n][#0losses: #G%d#n]\n\r",
       ch->pcdata->awins, ch->pcdata->alosses );
       send_to_char( buf, ch);
       if(ch->challenger != NULL)
       {
          sprintf(buf,"#n[#0Arena#n][#0You have been challenged by #G%s#n]\n\r",
          ch->challenger->name);
          send_to_char( buf, ch);
       }
       if(ch->challenged != NULL)
       {
          sprintf(buf,"#n[#0Arena#n][#0You have challenged #G%s#n]\n\r",
          ch->challenged->name);
          send_to_char( buf, ch);
       }
       if(ch->gladiator != NULL)
       {
          sprintf(buf,"#n[#0Arena bet#n][#0You have a #G%d #0gold bet on #G%s#n]\n\r",
          ch->pcdata->plr_wager, ch->gladiator->name);
          send_to_char( buf, ch);
       }
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
      sprintf(buf,"#n[#0Your shapeshifter counter is : #G%d#n]\n\r",ch->pcdata->powers[SHAPE_COUNTER]);
      stc(buf,ch);
      sprintf(buf,"#n[#0Your phase counter is        : #G%d#n]\n\r",ch->pcdata->powers[PHASE_COUNTER]);
      stc(buf,ch);
    }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
    {
      sprintf(buf,"#n[#0Angelic Justice   : #G%d#n]\n\r",ch->pcdata->powers[ANGEL_JUSTICE]);
      stc(buf,ch);
      sprintf(buf,"#n[#0Angelic Love      : #G%d#n]\n\r",ch->pcdata->powers[ANGEL_LOVE]);
      stc(buf,ch);
      sprintf(buf,"#n[#0Angelic Harmony   : #G%d#n]\n\r",ch->pcdata->powers[ANGEL_HARMONY]);
      stc(buf,ch);
      sprintf(buf,"#n[#0Angelic Peace     : #G%d#n]\n\r",ch->pcdata->powers[ANGEL_PEACE]);
      stc(buf,ch);
   }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
    {
      sprintf(buf,"#n[#0Your current beast is : #G%d#n]\n\r",ch->beast);
      stc(buf,ch);

    }
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) )
    {
      sprintf(buf,"#n[#0Your block counter is currently: #G%d#n]\n\r",ch->monkblock);
      stc(buf,ch);
      sprintf(buf,"#n[#0Your current level of chi:       #G%d#n]\n\r",ch->chi[CURRENT]);
      stc(buf,ch);
      sprintf(buf,"#n[#0Your maximum level of chi:       #G%d#n]\n\r",ch->chi[MAXIMUM]);
      stc(buf,ch);
      sprintf(buf,"#n[#0Your current focus points:       #G%d#n]\n\r",ch->focus[CURRENT]);
      stc(buf,ch);
      sprintf(buf,"#n[#0Your maximum focus points:       #G%d#n]\n\r",ch->focus[MAXIMUM]);
      stc(buf,ch);
    }
    if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && ch->gnosis[GMAXIMUM] > 0 )
    {
	sprintf( buf, "#n[#0Current Gnosis:            #G%d#n]\n\r", ch->gnosis[GCURRENT] );
	stc( buf, ch);
	sprintf( buf, "#n[#0Maximum gnosis:            #G%d#n]\n\r", ch->gnosis[GMAXIMUM] );
	stc( buf, ch);
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "#n[#0You are drunk#n]\n\r",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] ==  0 )
	send_to_char( "#n[#0You are thirsty#n]\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   ==  0 )
	send_to_char( "#n[#0You are hungry#n]\n\r",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 100 )
	send_to_char( "#n[#0You are feeling extremely horny#n]\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 50 )
	send_to_char( "#n[#0You are feeling pretty randy#n]\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 1 )
	send_to_char( "#n[#0You are feeling rather kinky#n]\n\r",  ch );

    if ( !IS_NPC(ch) && ch->pcdata->stage[1] > 0 && ch->position == POS_STANDING )
    {
	send_to_char( "#n[#0You are having sexual intercourse#n]\n\r",  ch );
	if (!IS_NPC(ch) && (ch->pcdata->stage[2] + 25) >= ch->pcdata->stage[1])
	    send_to_char( "#n[#0You are on the verge of having an orgasm#n]\n\r",  ch );
    }
    else switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char( "#n[#0You are #RD#0E#RA#0D#R!#0!#n]\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "#n[#0You are #Rmortally #nwounded#n]\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "#n[#0You are incapacitated#n]\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "#n[#0You are stunned#n]\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "#n[#0You are sleeping#n]\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "#n[#0You are resting#n]\n\r",		ch );
	break;
    case POS_MEDITATING:
	send_to_char( "#n[#0You are meditating#n]\n\r",	ch );
	break;
    case POS_SITTING:
	send_to_char( "#n[#0You are sitting#n]\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "#n[#0You are standing#n]\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "#n[#0You are fighting#n]\n\r",		ch );
	break;
    }

    if ( ch->level >= 0 )
    {
	sprintf( buf, "#n[#0AC: #G%d#n]", a_c );
	send_to_char( buf, ch );
    }

    send_to_char( "#n[#0You are ", ch );
         if ( a_c >=  101 ) send_to_char( "#Gnaked#n]\n\r", ch );
    else if ( a_c >=   80 ) send_to_char( "#Gbarely clothed#n]\n\r",   ch );
    else if ( a_c >=   60 ) send_to_char( "#Gwearing clothes#n]\n\r",  ch );
    else if ( a_c >=   40 ) send_to_char( "#Gslightly armored#n]\n\r", ch );
    else if ( a_c >=   20 ) send_to_char( "#Gsomewhat armored#n]\n\r", ch );
    else if ( a_c >=    0 ) send_to_char( "#Garmored#n]\n\r",          ch );
    else if ( a_c >=  -50 ) send_to_char( "#Gwell armored#n]\n\r",     ch );
    else if ( a_c >= -100 ) send_to_char( "#Gstrongly armored#n]\n\r", ch );
    else if ( a_c >= -250 ) send_to_char( "#Gheavily armored#n]\n\r",  ch );
    else if ( a_c >= -500 ) send_to_char( "#Gsuperbly armored#n]\n\r", ch );
    else if ( a_c >= -749 ) send_to_char( "#Gdivinely armored#n]\n\r", ch );
    else                    send_to_char( "#Gultimately armored#n]\n\r",ch );

    sprintf( buf, "#n[#0Hitroll: #G%d#n][#0Damroll: #G%d#n][#0Damcap: #G%d#n] ",
char_hitroll(ch), char_damroll(ch), ch->damcap[0]);
    send_to_char( buf, ch );
    

    if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) )
    {
	sprintf( buf, "#n[#0Blood: #G%d#n]\n\r", ch->pcdata->condition[COND_THIRST] );
	send_to_char( buf, ch );

/*	sprintf( buf, "#R[#nBeast: #C%d#R]", ch->beast );
	send_to_char( buf, ch );
	if      (ch->beast <  0 ) send_to_char("#R[#nYou are a cheat#R]\n\r",ch);
	else if (ch->beast == 0 ) send_to_char("#R[#nYou have attained Golconda#R]\n\r",ch);
	else if (ch->beast <= 5 ) send_to_char("#R[#nYou have almost reached Golconda#R]\n\r",ch);
	else if (ch->beast <= 10) send_to_char("#R[#nYou are nearing Golconda#R]\n\r",ch);
	else if (ch->beast <= 15) send_to_char("#R[#nYou have great control over your beast#R]\n\r",ch);
	else if (ch->beast <= 20) send_to_char("#R[#nYour beast has little influence over your actions#R]\n\r",ch);
	else if (ch->beast <= 30) send_to_char("#R[#nYou are in control of your beast#R]\n\r",ch);
	else if (ch->beast <= 40) send_to_char("#R[#nYou are able to hold back the beast#R]\n\r",ch);
	else if (ch->beast <= 60) send_to_char("#R[#nYou are constantly struggling for control of your beast#R]\n\r",ch);
	else if (ch->beast <= 75) send_to_char("#R[#nYour beast has great control over your actions#R]\n\r",ch);
	else if (ch->beast <= 90) send_to_char("#R[#nThe power of the beast overwhelms you#R]\n\r",ch);
	else if (ch->beast <= 99) send_to_char("#R[#nYou have almost lost your battle with the beast#R]\n\r",ch);
	else                      send_to_char("#R[#nThe beast has taken over#R]\n\r",ch);
*/
    }
    else if (ch->level >= 0)
	send_to_char( "\n\r", ch );

    if ( ch->level >= 0 )
    {
	sprintf( buf, "#n[#0Alignment: #G%d#n]", ch->alignment );
	send_to_char( buf, ch );
    }

    send_to_char( "#n[#0You are ", ch );
         if ( ch->alignment >  900 ) send_to_char( "#7angelic#n]\n\r", ch );
    else if ( ch->alignment >  700 ) send_to_char( "#7sai#yn#7tly#n]\n\r", ch );
    else if ( ch->alignment >  350 ) send_to_char( "#yg#7oo#yd#n]\n\r",    ch );
    else if ( ch->alignment >  100 ) send_to_char( "#ykind#n]\n\r",    ch );
    else if ( ch->alignment > -100 ) send_to_char( "#Lneutral#n]\n\r", ch );
    else if ( ch->alignment > -350 ) send_to_char( "#0mean#n]\n\r",    ch );
    else if ( ch->alignment > -700 ) send_to_char( "#0e#Rvi#0l#n]\n\r",    ch );
    else if ( ch->alignment > -900 ) send_to_char( "#Rdem#0o#Rnic#n]\n\r", ch );
    else                             send_to_char( "#Rsatanic#n]\n\r", ch );
    
    if ( !IS_NPC(ch) && ch->level >= 0 )
    {
	sprintf( buf, "#n[#0Status: #G%d#n]", ch->race );
	send_to_char( buf, ch );
    }


    if (!IS_NPC(ch)) send_to_char( "#n[#0You are ", ch );

         if (!IS_NPC(ch) && ch->level == 1 ) send_to_char( "#na Mortal]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 2 ) send_to_char( "#na Mortal]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 7 ) send_to_char( "#na Builder]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 8 ) send_to_char( "#na Builder]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 9 ) send_to_char( "#nan Enforcer]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 10) send_to_char( "#na Judge]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 11) send_to_char( "#na High Judge]\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 12) send_to_char( "#nan Implementor]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 0 ) send_to_char( "#nan #RAvatar#n]\n\r",ch);
    else if (!IS_NPC(ch) && ch->race <  4 ) send_to_char( "#na #GSquire#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  7 ) send_to_char( "#nan #yApprentice#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  10) send_to_char( "#na #LScout#n]\n\r",ch);
    else if (!IS_NPC(ch) && ch->race <  13) send_to_char( "#na #PSoldier#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  16) send_to_char( "#na #CLieutenant#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  19) send_to_char( "#na #7Captain#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  22) send_to_char( "#na #0Guard#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  25) send_to_char( "#na #RKni#Gght#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  28) send_to_char( "#na #yRan#Lger#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  31) send_to_char( "#na #PHun#Cter#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  34) send_to_char( "#nan #7Ass#0assin#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  37) send_to_char( "#na #LL#Ro#Prd#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  40) send_to_char( "#na #RB#Car#7on#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  43) send_to_char( "#na #PD#7uk#Re#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  46) send_to_char( "#na #0Pr#Gin#Pce#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  51) send_to_char( "#na #CK#0i#7n#Gg#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <  76) send_to_char( "#nan #REm#7per#Ror#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race < 100) send_to_char( "#nan #LOve#yrl#Lord#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race < 125) send_to_char( "#na #CDe#Pmig#Cod#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race < 150) send_to_char( "#na #RLe#0ss#Rer #0Go#Rd#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race < 199) send_to_char( "#na #yM#Gighty #yG#God#n]\n\r", ch);
    else if (!IS_NPC(ch) && ch->race >= 199) send_to_char( "#nthe #0Reaper#n]\n\r", ch);
    else if (!IS_NPC(ch)) send_to_char( "#na Bugged Character!\n\r", ch);

    if ( !IS_NPC(ch) )
    {
	if      (ch->pkill  == 0) sprintf(ss1,"#Gno#0 players");
	else if (ch->pkill  == 1) sprintf(ss1,"#G%d#0 player",ch->pkill);
	else                      sprintf(ss1,"#G%d#0 players",ch->pkill);
	if      (ch->pdeath == 0) sprintf(ss2,"#Gno#0 players");
	else if (ch->pdeath == 1) sprintf(ss2,"#G%d#0 player",ch->pdeath);
	else                      sprintf(ss2,"#G%d#0 players",ch->pdeath);
	sprintf( buf, "#n[#0You have killed %s and have been killed by %s#n]\n\r", ss1, ss2 );
	send_to_char( buf, ch );
	if      (ch->mkill  == 0) sprintf(ss1,"#Gno#0 mobs");
	else if (ch->mkill  == 1) sprintf(ss1,"#G%d#0 mob",ch->mkill);
	else                      sprintf(ss1,"#G%d#0 mobs",ch->mkill);
	if      (ch->mdeath == 0) sprintf(ss2,"#Gno#0 mobs");
	else if (ch->mdeath == 1) sprintf(ss2,"#G%d#0 mob",ch->mdeath);
	else                      sprintf(ss2,"#G%d#0 mobs",ch->mdeath);
	sprintf( buf, "#n[#0You have killed %s and have been killed by %s#n]\n\r", ss1, ss2 );
	send_to_char( buf, ch );
    }

    if( !IS_NPC(ch) )
    {
	sprintf( buf, "#n[#0You have purchased QP #G%d #0times totalling #G%d #0points.#n]\n\r", ch->pcdata->questsrun, ch->pcdata->questtotal );
	stc( buf, ch);
    }
    if (!IS_NPC(ch))
    {
	sprintf(buf, "#n[#0You have earned a total of #G%d#0 QP so far.#n]\n\r",ch->pcdata->score[SCORE_QUEST]);
	stc(buf,ch);
    }
    if ( !IS_NPC(ch) && ch->pcdata->quest > 0)
    {
	if (ch->pcdata->quest == 1)
	    sprintf( buf, "#n[#0You have a #Gsingle#0 quest point#n]\n\r" );
	else
	    sprintf( buf, "#n[#0You have #G%d #0quest points#n]\n\r", ch->pcdata->quest );
	send_to_char( buf, ch );
    }
    if (!IS_NPC(ch))
    {
	sprintf(buf, "#n[#0You have #G%d #0save_vs_spells#n]\n\r",ch->saving_throw);
	stc(buf,ch);

        if (IS_SET(ch->special, SPC_NEWBIE))
        {
           chprintf(ch, "#n[#0You have #G%d#0/#G500#0 dice rolls#n]#n\n\r", ch->pcdata->wolf);
        }

	if (ch->lstatus >= 0)
	{
	if (ch->lstatus == 0)
	sprintf(buf, "#n[#0You have #Gnot#0 obtained a #GLegend Status#n]\n\r");
	if (ch->lstatus == 1)
        sprintf(buf, "#n[#0You have reached the Legend Status of #GZealot#0#n]\n\r");        
	if (ch->lstatus == 2)
        sprintf(buf, "#n[#0You have reached the Legend Status of #GChampion#0#n]\n\r");
	if (ch->lstatus == 3)
        sprintf(buf, "#n[#0You have reached the Legend Status of #GHero#0#n]\n\r");
	if (ch->lstatus == 4)
        sprintf(buf, "#n[#0You have reached the Legend Status of #GMartyr#0#n]\n\r");
        if (ch->lstatus == 5)
        sprintf(buf, "#n[#0You have reached the Legend Status of #GGuardian#0#n]\n\r");
        if (ch->lstatus == 6)
        sprintf(buf, "#n[#0You have reached the Legend Status of #GSaint#0#n]\n\r");
        if (ch->lstatus == 7)
        sprintf(buf, "#n[#0You have reached the Legend Status of #GSavior#0#n]\n\r");
        if (ch->lstatus == 8)
        sprintf(buf, "#n[#0You have reached the Legend Status of #GMyth#0#n]\n\r");
        if (ch->lstatus == 9)
        sprintf(buf, "#n[#0You have reached the Legend Status of #GLegend#0#n]\n\r");
	stc(buf,ch);
	}
//	sprintf(buf, "#n[#0You have stored #G%d#0 Legend Points#n]\n\r",ch->lexp);
//	stc(buf,ch);

        chprintf( ch, "#n[#0Your stack character is currently #G%c#0#n]\n\r", ch->pcdata->stackchar);
        chprintf( ch, "#n[#0Your speedwalk character is currently #G%c#0#n]\n\r", ch->pcdata->speedchar);
    }
   
    if (IS_AFFECTED(ch,AFF_HIDE)) send_to_char( "#n[#0You are keeping yourself hidden from those around you#n]\n\r", ch );

    if ( !IS_NPC(ch) )
    {
	if ( IS_CLASS(ch,CLASS_WEREWOLF) && ch->siltol > 0)
	{
	    sprintf(buf,"#n[#0You have attained #G%d #0points of silver tolerance#n]\n\r",ch->siltol);
	    send_to_char( buf, ch );
	}
	if ( IS_CLASS(ch,CLASS_VAMPIRE) && ch->rage > 0)
	{
	    sprintf(buf,"#n[#0The beast is in control of your actions:  Affects Hitroll and Damroll by #G+%d#n]\n\r",ch->rage);
	    send_to_char( buf, ch );
	}
	if ( IS_CLASS(ch, CLASS_WEREWOLF) && IS_SET(ch->special,SPC_WOLFMAN) && ch->rage > 0)
	{
	    sprintf(buf,"#n[#0You are raging:  Affects Hitroll and Damroll by #G+%d#n]\n\r",ch->rage);
	    send_to_char( buf, ch );
	}
	if (IS_CLASS(ch, CLASS_NINJA) && ch->rage > 0) {
	sprintf(buf,"#n[#0Your michi gives you an extra #G%d#0 hitroll and damroll#n]\n\r", ch->rage);
	send_to_char(buf,ch);
	}
	if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 6
	&& ch->pcdata->powers[HARA_KIRI] > 0) {
	sprintf(buf,"#n[#0You receive the power of HaraKiri for #G%d#0 more ticks#n]\n\r",ch->pcdata->powers[HARA_KIRI]);
	send_to_char(buf,ch);
	}

        if ( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_VAMPIRE) &&ch->rage > 0 && !IS_CLASS(ch, CLASS_NINJA))
	{
	   sprintf(buf,"#n[#0You are in a mad frenzy, adding #G%d#0 Hitroll and Damroll#n]\n\r",ch->rage);
	   stc(buf, ch);
	}

	else if ( IS_CLASS( ch, CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
	{
	    sprintf(buf,"#n[#0You are wearing demonic armour:  Affects Hitroll and Damroll by #G+%d#n]\n\r",
		((ch->pcdata->stats[DEMON_POWER]) *ch->pcdata->stats[DEMON_POWER]));
	    send_to_char( buf, ch );
	}
        else if ( IS_CLASS( ch, CLASS_DROW) && ch->pcdata->stats[DEMON_POWER] > 0)
        {
            sprintf(buf,"#n[#0You are wearing drow armour:  Affects Hitroll and Damroll by #G+%d#n]\n\r",
                ((ch->pcdata->stats[DEMON_POWER]) *ch->pcdata->stats[DEMON_POWER]));  
            send_to_char( buf, ch );
        }

    }	
	if (xIS_SET(ch->extra, IS_A_BITCH))
	{
	sprintf(buf,"#n[#0You feel shitty#n]#n\n\r");
	stc(buf,ch);
        }

	if (ch->fight_timer > 0) {
	sprintf(buf,"#n[#0You have #G%d#0 rounds left on your fight timer#n]#n\n\r", ch->fight_timer);
	send_to_char(buf,ch);
	}
        if (ch->wish_timer > 0) {
        sprintf(buf,"#n[#0You have #G%d#0 rounds left on your wish timer#n]#n\n\r",ch->wish_timer);
        send_to_char(buf,ch);
        }
	if (ch->fury_timer > 0) {
        sprintf(buf,"#n[#0You have #G%d#0 rounds left of unholy fury#n]#n\n\r",ch->fury_timer);
        send_to_char(buf,ch);
        }
        if (ch->quit_timer > 0) {
        sprintf(buf,"#n[#0You have #G%d#0 rounds left before you can quit#n]#n\n\r",ch->quit_timer);
        send_to_char(buf,ch);
        }

       if (ch->bomb_timer > 0)
        {
        if (IS_CLASS(ch, CLASS_ANGEL))
        sprintf(buf, "#n[#0You have #G%d#0 rounds left on your divine wrath timer#n]#n\n\r",ch->bomb_timer);

        if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
        sprintf(buf, "#n[#0You have #G%d#0 rounds left on your faerie blink timer#n]#n\n\r",ch->bomb_timer);

        if (IS_CLASS(ch, CLASS_MAGE))
        sprintf(buf, "#n[#0You have #G%d#0 rounds left on your bomb timer#n]#n\n\r",ch->bomb_timer);

        if (IS_CLASS(ch, CLASS_INQUISITOR))
        sprintf(buf, "#n[#0You have #G%d#0 rounds left on your fury timer#n]#n\n\r",ch->bomb_timer);
	
	if (IS_CLASS(ch, CLASS_PALADIN))
	sprintf(buf, "#n[#0You have #G%d#0 rounds left on your bladeslash timer#n]#n\n\r",ch->bomb_timer);
	stc(buf,ch);
	
	if (IS_CLASS(ch, CLASS_HUNTER))
	sprintf(buf, "#n[#0You have #G%d#0 rounds left until you can ensnare again#n]#n\n\r",ch->bomb_timer);
	stc(buf,ch);
        }

	if (randMight(ch) > 0)
	{
	sprintf(buf,"#n[#0Your #Gpower rating#0 is #G%d#0.#n]#n\n\r",randMight(ch));
	stc(buf,ch);
	}	
	if (xIS_SET(ch->extra, FFA_HUNG))
	{
	sprintf(buf,"#n[#0You have been hung in the #RFREE#y FOR #RALL#n]\n\r");
	stc(buf,ch);
	}
	
    return;
}



char *	const	day_name	[] =
{
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};

char *	const	month_name	[] =
{
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"It is %d o'clock %s, Day of %s, %d%s the Month of %s.\n\rDystopia started up at %s\rThe system time is %s\r",

	(time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month],
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );
    return;
}

void    rand_mud_title args ( ( CHAR_DATA *ch,int random) );
void    rand_avatar args ( ( CHAR_DATA *ch, int random ));
void    rand_mortal args ( ( CHAR_DATA *ch, int random ));
void 	rand_newbie args ( ( CHAR_DATA *ch, int random ));

void rand_mortal( CHAR_DATA *ch, int random)
{
  switch(random)
  {
  case 0:
  cent_to_char("[#R****#n]#0=------=#n[#7***********#n]#7<[ #0M#no#7r#nt#0a#nl#7s#n #7]>#n[#7***********#n]#0=------=#n[#R****#n]#n",ch);
  break;
  case 1:
cent_to_char("#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L|#c  M#Co#7r#0t#7a#Cl#cs#L  |#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n",ch);
  break;
  case 2:
cent_to_char("#o<>#C*#o<>#C*#o<>#C*#o<>#G+#g-#G-#g-#G-#g-#G-#g-#G-#g+#G#y[#8+#7+#8+#0+#8+#7+#8+#y]#o> #yM#oo#gr#Gt#ya#ol#gs #o<#y[#7#8+#7+#8+#0+#8+#7+#8+#y]#g+#G-#g-#G-#g-#G-#g-#G-#g-#G+#o<>#C*#o<>#C*#o<>#C*#o<>#n",ch);
  break;
  case 3:
cent_to_char("#7<=-=-=-=-=-=-=-=)[#0*#n*#0*#n*#0*#7]==> #0]#R< #RM#ro#nr#7t#Ra#rl#ns #R>#0[ #7<==[#0*#n*#0*#n*#0*#7](=-=-=-=-=-=-=-=>#n",ch);
  break;
  case 4:
cent_to_char("#R(#0---------------#R+#y[#C*#y][#C*#y]#7><><><#n#0[ #CMort#cals #0]#7><><><#n#y[#C*#y][#C*#y]#R+#0---------------#R)#n",ch);
  break;
  }
}
void rand_avatar( CHAR_DATA *ch, int random)
{
  switch(random)
  {
  case 0:
  cent_to_char("[#R****#n]#0=------=#n[#7***********#n]#7<[ #0C#no#7m#nb#0a#nt#7a#nn#0t#ns #7]>#n[#7***********#n]#0=------=#n[#R****#n]#n",ch);
  break;
  case 1:
 cent_to_char("#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L|#c  #cC#Co#7m#0b#7a#Ct#ca#Cn#7t#0s#L  |#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n",ch);
  break;
  case 2:
cent_to_char("#o<>#C*#o<>#C*#o<>#C*#o<>#G+#g-#G-#g-#G-#g-#G-#g-#G-#g+#G#y[#8+#7+#8+#0+#8+#7+#8+#y]#o> #yC#oo#gm#Gb#ya#ot#ga#Gn#yt#os #o<#y[#7#8+#7+#8+#0+#8+#7+#8+#y]#g+#G-#g-#G-#g-#G-#g-#G-#g-#G+#o<>#C*#o<>#C*#o<>#C*#o<>#n",ch);
  break;
  case 3:
cent_to_char("#7<=-=-=-=-=-=-=-=)[#0*#n*#0*#n*#0*#7]==> #0]#R< #RC#ro#nm#7b#Ra#rt#na#7n#Rt#rs #R>#0[ #7<==[#0*#n*#0*#n*#0*#7](=-=-=-=-=-=-=-=>#n",ch);
  break;
  case 4:
cent_to_char("#R(#0--------------#R+#y[#C*#y][#C*#y]#7><><><#n#0[ #CComba#ctants #0]#7><><><#n#y[#C*#y][#C*#y]#R+#0---------------#R)#n",ch);
  break;
  }
}
void rand_newbie( CHAR_DATA *ch, int random)
{
  switch(random)
  {
  case 0:
  cent_to_char("[#R****#n]#0=------=#n[#7***********#n]#7<[ #0N#ne#7w#nb#0i#ne#7s#n #7]>#n[#7***********#n]#0=------=#n[#R****#n]#n",ch);
  break;
  case 1:
cent_to_char("#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L|#c  N#Ce#7w#0b#7i#Ce#cs#L  |#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n",ch);
  break;
  case 2:
cent_to_char("#o<>#C*#o<>#C*#o<>#C*#o<>#G+#g-#G-#g-#G-#g-#G-#g-#G-#g+#G#y[#8+#7+#8+#0+#8+#7+#8+#y]#o> #yN#oe#gw#Gb#yi#oe#gs #o<#y[#7#8+#7+#8+#0+#8+#7+#8+#y]#g+#G-#g-#G-#g-#G-#g-#G-#g-#G+#o<>#C*#o<>#C*#o<>#C*#o<>#n",ch);
  break;
  case 3:
cent_to_char("#7<=-=-=-=-=-=-=-=)[#0*#n*#0*#n*#0*#7]==> #0]#R< #RN#re#nw#7b#Ri#re#ns #R>#0[ #7<==[#0*#n*#0*#n*#0*#7](=-=-=-=-=-=-=-=>#n",ch);
  break;
  case 4:
cent_to_char("#R(#0---------------#R+#y[#C*#y][#C*#y]#7><><><#n#0[ #CNewb#cies #0]#7><><><#n#y[#C*#y][#C*#y]#R+#0---------------#R)#n",ch);
  break;
  }
}
void rand_mud_title( CHAR_DATA *ch, int random)
{
  switch(random)
  {
  case 0:
  cent_to_char("[#R****#n]#0=------=#n[#7***********#n]#7<[ #0F#na#7l#nl#0e#nn #7M#na#0j#ne#7s#nt#0y#n #7]>#n[#7***********#n]#0=------=#n[#R****#n]#n",ch);
  break;
  case 1:
  cent_to_char("#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L|#c  #cF#Ca#7l#0l#7e#Cn #cM#Ca#7j#0e#7s#Ct#cy  #L|#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n",ch);
  break;
  case 2:
cent_to_char("#o<>#C*#o<>#C*#o<>#C*#o<>#G+#g-#G-#g-#G-#g-#G-#g-#G-#g+#G#y[#8+#7+#8+#0+#8+#7+#8+#y]#o> #gF#Ga#yl#ol#ge#Gn #yM#oa#gj#Ge#ys#Gt#gy #o<#y[#7#8+#7+#8+#0+#8+#7+#8+#y]#g+#G-#g-#G-#g-#G-#g-#G-#g-#G+#o<>#C*#o<>#C*#o<>#C*#o<>#n",ch);
  break;
  case 3:
cent_to_char("#7<=-=-=-=-=-=-=-=)[#0*#n*#0*#n*#0*#7]==> #0]#R< #RF#ra#nl#7l#Re#rn #nM#7a#Rj#re#ns#7t#Ry #R>#0[ #7<==[#0*#n*#0*#n*#0*#7](=-=-=-=-=-=-=-=>#n",ch);
  break;
  case 4:
cent_to_char("#R(#0--------------#R+#y[#C*#y][#C*#y]#7><><><#n#0[ #CFal#clen #CMaje#csty #0]#7><><><#n#y[#C*#y][#C*#y]#R+#0---------------#R)#n",ch);
  break;
  }
}
void rand_imms( CHAR_DATA *ch, int random)
{
  switch(random)
  {
  case 0:
   cent_to_char("[#R****#n]#0=------=#n[#7***********#n]#7<[ #0I#nm#7m#no#0r#nt#0a#nl#0s#n #7]>#n[#7***********#n]#0=------=#n[#R****#n]#n",ch);
   break;
  case 1:
   
cent_to_char("#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L|  #cI#Cm#7m#no#7r#Ct#ca#Cl#7s#L  |#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n",ch);
   break;
  case 2:
   
cent_to_char("#o<>#C*#o<>#C*#o<>#C*#o<>#G+#g-#G-#g-#G-#g-#G-#g-#G-#g+#G#y[#8+#7+#8+#0+#8+#7+#8+#y]#o> #yI#om#gm#Go#yr#ot#ga#Gl#ys#n #o<#y[#7#8+#7+#8+#0+#8+#7+#8+#y]#g+#G-#g-#G-#g-#G-#g-#G-#g-#G+#o<>#C*#o<>#C*#o<>#C*#o<>#n",ch);
   break;
  case 3:
   cent_to_char("#7<=-=-=-=-=-=-=-=)[#0*#n*#0*#n*#0*#7]==> #0]#R< #RI#rm#nm#7o#Rr#rt#na#7l#Rs#n #R>#0[ #7<==[#0*#n*#0*#n*#0*#7](=-=-=-=-=-=-=-=>#n",ch);
   break;
  case 4:
   cent_to_char("#R(#0---------------#R+#y[#C*#y][#C*#y]#7><><><#n#0[ #CImmo#crtals #0]#7><><><#n#y[#C*#y][#C*#y]#R+#0---------------#R)#n",ch);
   break;
  }
}
 


/*
 * New 'who' command originally by Alander of Rivers of Mud.
 * Redone by Rand for Fallen Majesty
 */
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MSL];
    char buf3[MAX_STRING_LENGTH];
    char buf4[MAX_STRING_LENGTH];
    char buf5[MAX_STRING_LENGTH];
    char buf6[MAX_STRING_LENGTH];
    char buf7[MAX_STRING_LENGTH];
    char buf8[MAX_STRING_LENGTH];
    char buf9[MAX_STRING_LENGTH];
    char buf10[MAX_STRING_LENGTH];
    char buf11[MAX_STRING_LENGTH];
    char buf12[MAX_STRING_LENGTH];
    char buf13[MAX_STRING_LENGTH];
    char buf14[MAX_STRING_LENGTH];
    char buf15[MAX_STRING_LENGTH];
    char buf16[MAX_STRING_LENGTH];
    char buf17[MAX_STRING_LENGTH];
    char buf18[MAX_STRING_LENGTH];
    char buf19[MAX_STRING_LENGTH];
    char buf20[MAX_STRING_LENGTH];
    char buf21[10];
    char buf99[MSL];
    char sStatus[MSL];
    char kav[MAX_STRING_LENGTH];
    char king[MAX_STRING_LENGTH];
    char openb[15];
    char closeb[15];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *wch;
    CHAR_DATA *victim;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    bool fClassRestrict;
    bool fImmortalOnly;
    bool immortal = FALSE,avatar = FALSE,mortal = FALSE, mains = FALSE, newbie = FALSE; 
    bool imm = FALSE,ava = FALSE, mor = FALSE, new = FALSE;
    bool a2,a3,a4,a5,a6,a99,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16;
    int random = number_range(0,4);
    if (IS_NPC(ch)) return;

    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    fClassRestrict = FALSE;
    fImmortalOnly  = FALSE;

    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	if ( is_number( arg ) )
	{
	    send_to_char("Enter 'Avatar' for level 3's, or 'God' for level 4's and 5's.\n\r",ch);
	    return;
	}
	else
	{
	    /*
	     * Look for classes to turn on.
	     */
	    arg[3]    = '\0';
	    if (   !str_cmp( arg, "imm" ) || !str_cmp( arg, "immortal" )
		|| !str_cmp( arg, "ava" ) || !str_cmp( arg, "avatar"   ) )
	    {
		fClassRestrict = TRUE;
	    }
	    else if ( !str_cmp( arg, "god" ) || !str_cmp( arg, "imp" ) )
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
		send_to_char("Enter 'Avatar' for level 3's, or 'God' for level 4's and 5's.\n\r",ch);
		return;
	    }
	}
    }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    buf3[0] = '\0';
    buf4[0] = '\0';
    buf5[0] = '\0';
    buf6[0] = '\0';
    buf7[0] = '\0';
    buf8[0] = '\0';
    buf9[0] = '\0';
    buf10[0] = '\0';
    buf11[0] = '\0';
    buf12[0] = '\0';
    buf13[0] = '\0';
    buf14[0] = '\0';
    buf15[0] = '\0';
    buf16[0] = '\0';
    buf17[0] = '\0';
    buf18[0] = '\0';
    buf19[0] = '\0';
    buf20[0] = '\0';
    buf21[0] = '\0';
    buf99[0] = '\0'; 
    for ( d = first_descriptor; d != NULL; d = d->next )
    {
	char const *class;
        char const *lstatus;

	/*
	 * Check for match against restrictions.
	 * Don't use trust as that exposes trusted mortals.
	 */

	if ( (d->connected != CON_PLAYING ) || (!can_see( ch, d->character ) && 
(!xIS_SET(ch->act, PLR_WATCHER)) )  )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;
	if ( wch->level < iLevelLower
	||   wch->level > iLevelUpper
	|| ( fImmortalOnly  && wch->level <  LEVEL_IMMORTAL )
	|| ( fClassRestrict && wch->level != LEVEL_HERO     ) )
	    continue;

	nMatch++;

	/*
	 * Figure out what to print for class.
	 */
	class = " ";
        lstatus = " ";
		if (wch->lstatus < 0)
		   lstatus = "#n[#cU#Cn#7k#0n#7o#Cw#cn#n ]#n";
		else
		   lstatus = flag_string(legend_flags, wch->lstatus);
/*
	switch( wch->lstatus )
        {
          default:    lstatus = "#n[#cU#Cn#7k#0n#7o#Cw#cn#n ]#n"; break;
          case L_CIT: lstatus = "#n[#GZ#ge#7al#8ot#n  ]#n"; break;
	  case L_LEG: lstatus = "#n[#8Ch#La#lmp#Lio#8n#n]#n"; break;
          case L_MYT: lstatus = "#n[#gH#Ge#yro#n    ]#n"; break;
          case L_TIT: lstatus = "#n[#RM#ra#or#yt#Ry#rr#n  ]#n"; break;
          case L_ORA: lstatus = "#n[#yG#ou#7a#8rd#7i#oa#yn#n]#n"; break;
          case L_IMM: lstatus = "#n[#8S#7a#yi#7n#8t#n   ]#n"; break;
          case L_DEM: lstatus = "#n[#RS#ra#8vi#ro#Rr#n  ]#n"; break;
          case L_DEI: lstatus = "#n[#CM#cy#8t#ch#n    ]#n"; break;
          case L_ALM: lstatus = "#n[#cL#8e#7g#ce#8n#7d#n  ]#n"; break;
	}
*/
        if ((IS_HEAD(wch,LOST_HEAD) || IS_EXTRA(wch,EXTRA_OSWITCH)) && wch->pcdata->chobj != NULL)
	{
	    if (wch->pcdata->chobj->pIndexData->vnum == 12)
	    	class = "#n[#7A Head#n    ]";
	    else if (wch->pcdata->chobj->pIndexData->vnum == 30005)
	    	class = "#n[#0A Brain#n   ]";
	    else
	    	class = "#n[#7An Object#n ]";
	} else
	switch ( wch->level )
	{
	default: break;
	case MAX_LEVEL -  0:
		     if ( IS_EXTRA(wch, EXTRA_AFK) )
                        class = "#n[#7 Not Here #n ]";
	        else if (wch->prefix != NULL)
			class = wch->prefix;
else if (!str_cmp(wch->pcdata->switchname,"Jank"))  class =     "#n[#PJANK #GE. #CPOO#n]#n";
else if (!str_cmp(wch->pcdata->switchname,"Rand"))  class =     "#n[           ]#n";
else if (!str_cmp(wch->pcdata->switchname,"Snard")) class =     "#n[ #lS#Ln#ca#Crd#ca#Ls#ls #n ]#n";
else class = " Implementor ";
	break;
  	case MAX_LEVEL -  1: 
                     if ( IS_EXTRA(wch, EXTRA_AFK) )
                        class = "#n[#7 Not Here #n ]";
                else if (wch->prefix != NULL)
                        class = wch->prefix;
		else class = "#n[#RH#7igh #RJ#7udge#n ]"; break;
	case MAX_LEVEL -  2: 
                     if ( IS_EXTRA(wch, EXTRA_AFK) )
                        class = "#n[#7 Not Here #n ]";
                else if (wch->prefix != NULL)
                        class = wch->prefix;


		else class = "#n[   #CJ#cu#7d#cg#Ce#n   ]"; break;
	case MAX_LEVEL -  3: 
		class = "#n[ #RE#Lnforcer#n  ]"; break;

	case MAX_LEVEL -  4: 
                     if ( IS_EXTRA(wch, EXTRA_AFK) )
                        class = "#n[#7 Not Here #n ]";
                else if (wch->prefix != NULL)
                        class = wch->prefix;
                else class = "#n[  #GBuilder#n  ]"; break;

	case MAX_LEVEL -  5: class = "#n[  #yBuilder#n  ]"; break;

	case MAX_LEVEL -  6: 
	case MAX_LEVEL -  7:
	case MAX_LEVEL -  8:
	case MAX_LEVEL -  9:
	             if (wch->prefix != NULL) class = wch->prefix;
	             if ( IS_EXTRA(wch, EXTRA_AFK) )
                        class = "#n[#7 Not Here #n]";
	     victim = wch->fighting;


                if (wch->race <= 0 ) class = "#n[#oW#yo#or#ot#0h#ol#ye#os#0s#n ]#n";
                else if (wch->race <= 3 ) class = "#n[#GS#gq#7u#8i#Gr#ge#n    ]#n";
                else if (wch->race <= 6 ) class = "#n[#yA#op#7p#8r#ye#on#7t#8i#yc#oe#n]#n";
                else if (wch->race <= 9 ) class = "#n[#LS#lc#7o#8ut#n     ]#n";
                else if (wch->race <= 12) class = "#n[#PS#po#0l#7d#8i#Pe#pr#n   ]#n";
                else if (wch->race <= 15) class = "#n[#CL#ci#7e#8u#Ct#ce#7n#8a#Cn#ct#n]#n";
                else if (wch->race <= 18) class = "#n[#8C#7a#0pta#7i#8n#n   ]#n";
                else if (wch->race <= 21) class = "#n[#0G#8u#7a#8r#0d#n     ]#n";
                else if (wch->race <= 24) class = "#n[#RK#rn#gi#Gght#n    ]#n";
                else if (wch->race <= 27) class = "#n[#yR#oa#gn#Gg#ye#or#n    ]#n";
                else if (wch->race <= 30) class = "#n[#7H#8u#Cnt#cer#n    ]#n";
                else if (wch->race <= 33) class = "#n[#8Ass#7ass#0in#n  ]#n";
                else if (wch->race <= 36) class = "#n[#lL#Lo#8r#Ld#n      ]#n";
                else if (wch->race <= 39) class = "#n[#RB#ra#0r#Ro#rn#n     ]#n";
                else if (wch->race <= 42) class = "#n[#PD#pu#7k#0e#n      ]#n";
                else if (wch->race <= 45) class = "#n[#0P#7ri#yn#oce#n    ]#n";
                else if (wch->race <= 50) class = "#n[#GK#gi#7n#8g#n      ]#n";
                else if (wch->race <= 75) class = "#n[#RE#rm#0p#8e#0r#ro#Rr#n   ]#n";
                else if (wch->race <= 99) class = "#n[#yO#ov#le#Lrl#lo#or#yd#n  ]#n";
                else if (wch->race <= 124) class = "#n[#CD#ce#7m#Ci#cg#7o#Cd#n   ]#n";
                else if (wch->race <= 149) class = "#n[#RL#re#0s#7s#8e#0r #7G#ro#Rd#n]#n";
                else if (wch->race <= 198) class = "#n[#yM#Gi#ggh#Gty #yG#Go#Gd#n]#n";
                else if (wch->race >= 199) class = "#n[#0  R#7e#8a#0p#7e#8r  #n]#n";


             if (wch->fight_timer > 0)
		class = "#n[#7F#Ri#7gh#RtT#7im#Re#7r#n]#n";
              if ((wch->fighting != NULL) && !IS_NPC(victim) && !IS_SET(wch->in_room->room_flags, ROOM_ARENA))
                {
                strcpy(openb, "#n[#n");
                strcpy(closeb, "#n]#n");
                sprintf(buf, "%s #y%-8s#n %s", openb, victim->name, closeb);
                class = buf;
                }

             if (IS_SET(wch->special, SPC_NEWBIE))
		class = "#n[#7<-N#0e#nwb#0i#7e->#n]#n";
             if (IS_SET(wch->in_room->room_flags, ROOM_ARENA) && wch->in_room != NULL)
                class = "#n[#RFREE#yFOR#RALL#n]#n";

		break;

	case MAX_LEVEL - 10: class = "#n[#1Mortal#n    ]"; break;
	case MAX_LEVEL - 11: class = "#n[#6Newbie#n    ]"; break;
	case MAX_LEVEL - 12: class = "#n[#7Undefined#n ]"; break;
	}
	/*
	 * Format it up.
	 */
  if (IS_NPC(ch)) return;
  if (IS_WHO_CLASS(wch, CLASS_TANARRI))
    {strcpy(openb, "#y=#n"); strcpy(closeb, "#y=#n");}
  else if (IS_WHO_CLASS(wch, CLASS_ANGEL))
        {strcpy( openb, "#y.x#0[#n" ); strcpy( closeb, "#0]#yx.#n" );}
  else if (IS_WHO_CLASS(wch, CLASS_UNDEAD_KNIGHT))
        {strcpy( openb, "#0|[#n" ); strcpy( closeb, "#0]|#n" );}
  else if (IS_WHO_CLASS(wch, CLASS_SHAPESHIFTER))
        {strcpy( openb, "#0[#P*#0]#n" ); strcpy( closeb, "#0[#P*#0]#n" );}
  else if (IS_WHO_CLASS(wch, CLASS_VAMPIRE))
  	{strcpy( openb, "#R<<#n" ); strcpy( closeb, "#R>>#n" );}
  else if (IS_WHO_CLASS(wch, CLASS_POWER_LICH))
        {strcpy( openb, "#G>#7*#G<#n" ); strcpy( closeb, "#G>#7*#G<#n" );}
  else if ( IS_WHO_CLASS(wch, CLASS_WEREWOLF))
    {strcpy( openb, "#y((#n" ); strcpy( closeb, "#y))#n" );}
  else if ( IS_WHO_CLASS(wch,CLASS_MAGE))
    {strcpy( openb, "{{" ); strcpy( closeb, "}}" );}
  else if ( IS_WHO_CLASS(wch,CLASS_CYBORG))
    {strcpy( openb, "#p(#0-#p)#n");strcpy( closeb, "#p(#0-#p)#n");}
  else if ( IS_WHO_CLASS(wch, CLASS_HIGHLANDER))
    {strcpy( openb, "#C-=#n" ); strcpy( closeb, "#C=-#n" );}
  else if ( IS_WHO_CLASS(wch, CLASS_PALADIN))
    {strcpy( openb, "#y-#7=#n" ); strcpy( closeb, "#7=#y-#n" );}
  else if ( IS_WHO_CLASS(wch, CLASS_DROW))
    {strcpy( openb, "#P.o0"); strcpy(closeb, "#P0o.#n" );}
  else if ( IS_WHO_CLASS(wch, CLASS_NINJA))
    {strcpy( openb, "#C***#n"); strcpy(closeb, "#C***#n" );}
  else if ( IS_WHO_CLASS(wch, CLASS_MONK))
    {strcpy(openb, "#0.x[#n"); strcpy(closeb, "#0]x.#n");}
  else if ( IS_WHO_CLASS(wch, CLASS_PSIONIST))
    {strcpy( openb, "#C(#Po#C)#n"); strcpy(closeb, "#C(#Po#C)#n");}
  else if ( IS_WHO_CLASS(wch, CLASS_INQUISITOR))
    {strcpy( openb, "#0<#7^#0>#n"); strcpy(closeb, "#0<#7^#0>#n");}
  else if ( IS_WHO_CLASS(wch, CLASS_HUNTER))
    {strcpy( openb, "#o:[#n"); strcpy(closeb, "#o:]#n");}
  else
    {strcpy( openb, "#0[#n" ); strcpy( closeb, "#0]#n" );}

if ( wch->class > 0)
  {
  if (IS_WHO_CLASS(wch, CLASS_MAGE) )
  {
    if (wch->generation == 0)
      sprintf(kav, "%s#CGrand High Archmage#n%s   ", openb, closeb);
    else if (wch->generation == 1)
      sprintf(kav, "%s#CHigh Archmage#n%s         ", openb, closeb);
    else if(wch->generation == 2)
      sprintf(kav, "%s#CArchmage#n%s              ", openb, closeb);
    else if(wch->generation == 3)
      sprintf(kav, "%s#CLord of Spells#n%s        ", openb, closeb);
    else if(wch->generation == 4)
      sprintf(kav, "%s#CHigh Invoker#n%s          ", openb, closeb);
    else if(wch->generation == 5)
      sprintf(kav, "%s#CWizard of War#n%s         ", openb, closeb);
    else if(wch->generation == 6)
      sprintf(kav, "%s#CBattlemage#n%s            ", openb, closeb);
    else if(wch->generation == 7)
      sprintf(kav, "%s#CApprentice#n%s            ", openb, closeb);
    else
      sprintf(kav, "%s#CMagician#n%s              ", openb, closeb);
  }  
  else if (IS_WHO_CLASS(wch, CLASS_CYBORG))
  {
    if (wch->generation == 0)
      sprintf(kav, "%s#0Lloth's Avenger#n%s     ", openb, closeb);
    else if (wch->generation == 1)
      sprintf(kav,"%s#0Drider Lord%s         ",closeb,closeb);
    else if(wch->generation == 2)
      sprintf(kav,"%s#0Master of the Web%s   ",closeb,closeb);
  	else
      sprintf(kav,"%s#0Spider Droid%s        ",closeb,closeb);
  }
  else if( IS_WHO_CLASS(wch, CLASS_SHAPESHIFTER) )
  {
    if (wch->generation == 0)
      sprintf( kav, "%s#RPlay-Doh#n%s            ", openb, closeb);
    else if (wch->generation == 1)
      sprintf( kav, "%s#RSpawn of Malaug#n%s     ", openb, closeb );
    else if (wch->generation == 2)
      sprintf( kav, "%s#RShadowmaster#n%s        ", openb, closeb );
    else if (wch->generation == 3)
      sprintf( kav, "%s#RMalaugrym Elder#n%s     ", openb, closeb );
    else if (wch->generation == 4)
      sprintf( kav, "%s#RMalaugrym#n%s           ", openb, closeb );
    else if (wch->generation == 5)
      sprintf( kav, "%s#RShapeshifter#n%s        ", openb, closeb );
    else if (wch->generation == 6)
      sprintf( kav, "%s#RDoppleganger#n%s        ", openb, closeb );
    else if (wch->generation == 7)
      sprintf( kav, "%s#RMass of tentacles#n%s   ", openb, closeb );
    else
      sprintf( kav, "%s#RPlay-Doh#n%s            ", openb, closeb );
  }
  else if( IS_WHO_CLASS(wch, CLASS_ANGEL) )
  {
    if (wch->generation == 0)
      sprintf( kav, "%s#7God's Champion#n%s      ", openb, closeb );
    else if (wch->generation == 1)
      sprintf( kav, "%s#7Arch Angel#n%s          ", openb, closeb );
    else if (wch->generation == 2)
      sprintf( kav, "%s#7Cherubim#n%s            ", openb, closeb );
    else if (wch->generation == 3)
      sprintf( kav, "%s#7Seraphim#n%s            ", openb, closeb );
    else if (wch->generation == 4)
      sprintf( kav, "%s#7Guardian Angel#n%s      ", openb, closeb );
    else if (wch->generation == 5)
      sprintf( kav, "%s#7Angel#n%s               ", openb, closeb );
    else
      sprintf( kav, "%s#7Nephalim#n%s            ", openb, closeb );
  }
  else if( IS_WHO_CLASS(wch, CLASS_UNDEAD_KNIGHT) )
  {
    if (wch->generation == 0)
      sprintf( kav, "%s#LDeath's Harbinger#n%s     ", openb, closeb);
    else if (wch->generation == 1)
      sprintf( kav, "%s#LUndead Champion#n%s       ", openb, closeb );
    else if (wch->generation == 2)
      sprintf( kav, "%s#LUndead Knight#n%s         ", openb, closeb ); 
    else if (wch->generation == 3)
      sprintf( kav, "%s#LUndead General#n%s        ", openb, closeb );
    else if (wch->generation == 4)
      sprintf( kav, "%s#LUndead Colonel#n%s        ", openb, closeb );
    else if (wch->generation == 5)
      sprintf( kav, "%s#LUndead Legionairre#n%s    ", openb, closeb );
    else if (wch->generation == 6)
      sprintf( kav, "%s#LUndead Squire#n%s         ", openb, closeb );
    else if (wch->generation == 7)
      sprintf( kav, "%s#LUndead Page#n%s           ", openb, closeb );
    else
      sprintf( kav, "%s#LUndead Page#n%s           ", openb, closeb );
  }
  else if ( IS_WHO_CLASS(wch, CLASS_WEREWOLF))
  {
    if (wch->generation == 0)
      sprintf( kav, "%s#LRage of Gaia#n%s          ", openb, closeb);
    else if (wch->generation == 1)
      sprintf( kav, "%s#LChieftain#n%s             ", openb, closeb );
    else if (wch->generation == 2)
      sprintf( kav, "%s#LTribe Shaman#n%s          ", openb, closeb );
    else if (wch->generation == 3)
      sprintf( kav, "%s#LPack Leader#n%s           ", openb, closeb );
    else if (wch->generation == 4)
      sprintf( kav, "%s#LFenris Wolf#n%s           ", openb, closeb );
    else if (wch->generation == 5)
      sprintf( kav, "%s#LStalker#n%s               ", openb, closeb );
    else if (wch->generation == 6)
      sprintf( kav, "%s#LHunter#n%s                ", openb, closeb );
    else if (wch->generation == 7)
      sprintf( kav, "%s#LPup of Gaia#n%s           ", openb, closeb );
    else
      sprintf( kav, "%s#LHalfbreed Bastard#n%s     ", openb, closeb );
  }
  else if ( IS_WHO_CLASS(wch, CLASS_MONK))
  {
    if (wch->generation == 0)
      sprintf(kav,"%s#lHigh Abbot#n%s          ", openb, closeb);
    else if (wch->generation == 1)
      sprintf(kav,"%s#lAbbot#n%s               ", openb, closeb);
    else if (wch->generation == 2)
      sprintf(kav,"%s#lHigh Priest#n%s         ", openb, closeb);
    else if (wch->generation == 3)
      sprintf(kav,"%s#lPriest#n%s              ", openb, closeb);
    else if (wch->generation == 4)
      sprintf(kav,"%s#lFather#n%s              ", openb, closeb);
    else if (wch->generation == 5)
      sprintf(kav,"%s#lMonk#n%s                ", openb, closeb);
    else if (wch->generation == 6)
      sprintf(kav,"%s#lBrother#n%s             ", openb, closeb);
    else if (wch->generation == 7)
      sprintf(kav,"%s#lAcolyte#n%s             ", openb, closeb);
    else
      sprintf(kav,"%s#lFanatic#n%s             ", openb, closeb);
  }
  else if ( IS_WHO_CLASS(wch, CLASS_DROW))
  {
    if (wch->generation == 0)
      sprintf(kav, "%s#0Lloth Incarnate#n%s     ", openb, closeb);
    else if (wch->generation == 1)
     {
       if (wch->sex == SEX_FEMALE)
      sprintf(kav,"%s#0Matron Mother#n%s       ", openb, closeb);
       else if (wch->sex == SEX_MALE)
      sprintf(kav,"%s#0Lord of the Blades#n%s  ", openb, closeb);
       else if (wch->sex == SEX_NEUTRAL)
      sprintf(kav,"%s#0Sexless Fodder#n%s      ", openb, closeb);
     }
    else if (wch->generation == 2)
      sprintf(kav,"%s#0Weaponmaster#n%s        ", openb, closeb);
    else if (wch->generation == 3)
     {
       if (wch->sex == SEX_FEMALE)
      sprintf(kav,"%s#0High Priestess#n%s      ", openb, closeb);
       else if (wch->sex == SEX_MALE)
      sprintf(kav,"%s#0High Priest#n%s	      ", openb, closeb);
       else if (wch->sex == SEX_NEUTRAL)
      sprintf(kav,"%s#0Sexless Fodder#n%s      ", openb, closeb);
     }

    else if (wch->generation == 4)
      sprintf(kav,"%s#0Favored of Lloth#n%s    ", openb, closeb);
    else if (wch->generation == 5)
      sprintf(kav,"%s#0Black Widow#n%s         ", openb, closeb);
    else if (wch->generation == 6)
      sprintf(kav,"%s#0Drow#n%s                ", openb, closeb);
    else if (wch->generation == 7)
      sprintf(kav,"%s#0Drow Male#n%s           ", openb, closeb);
    else
      sprintf(kav,"%s#0Drider#n%s              ", openb, closeb);
  }
  else if ( IS_WHO_CLASS(wch, CLASS_VAMPIRE))
  {
      if (wch->generation == 0)
        sprintf( kav, "%s#RC#0hilde #Ro#0f #RC#0aine#n%s       ", openb, closeb);
      else if (wch->generation == 1)
        sprintf( kav, "%s#RI#0nner #RC#0ircle#n%s          ", openb, closeb);
      else if (wch->generation == 2)
        sprintf( kav, "%s#RV#0ampire #RJ#0usticar#n%s      ", openb, closeb);
      else if (wch->generation == 3)
        sprintf( kav, "%s#RV#0ampire #RP#0rince#n%s        ", openb, closeb);
      else if (wch->generation == 4)
        sprintf( kav, "%s#RV#0ampire #RA#0ncilla#n%s       ", openb, closeb);
      else if (wch->generation == 5)
        sprintf( kav, "%s#RV#0ampire #RA#0rchon#n%s        ", openb, closeb);
      else if (wch->generation == 6)
        sprintf( kav, "%s#RV#0ampire#n%s               ", openb, closeb);
      else if (wch->generation == 7)
        sprintf( kav, "%s#RV#0ampire #RA#0narch#n%s        ", openb, closeb);
      else
        sprintf( kav, "%s#RB#0loodsucker#n%s           ", openb, closeb);

  }
  else if ( IS_WHO_CLASS(wch, CLASS_TANARRI))
  {
    if (wch->generation == 0)
      sprintf( kav, "%s#RTanar'ri Molydeus#n%s       ", openb, closeb);
    else if (wch->generation == 1)
      sprintf( kav, "%s#RTanar'ri Balor#n%s          ", openb, closeb );
    else if (wch->generation == 2)
      sprintf( kav, "%s#RTanar'ri Marilith#n%s       ", openb, closeb );
    else if (wch->generation == 3)
      sprintf( kav, "%s#RGreater Tanar'ri#n%s        ", openb, closeb );
    else if (wch->generation == 4)
      sprintf( kav, "%s#RTrue Tanar'ri#n%s           ", openb, closeb );
    else if (wch->generation == 5)
      sprintf( kav, "%s#RTanar'ri#n%s                ", openb, closeb );
    else
      sprintf( kav, "%s#RCambion#n%s                 ", openb, closeb );
  }
  else if (IS_WHO_CLASS(wch, CLASS_PSIONIST))
  {
    if (wch->generation == 0)
      sprintf( kav, "%s#PMental Masterpiece#n%s  ", openb, closeb);
    else if (wch->generation == 1)
      sprintf( kav, "%s#PMental Marvel#n%s       ", openb, closeb);
    else if (wch->generation == 2)
      sprintf( kav, "%s#PMentally Superior#n%s   ", openb, closeb);
    else if (wch->generation == 3)
      sprintf( kav, "%s#PMentally Gifted#n%s     ", openb, closeb);
    else if (wch->generation == 4)
      sprintf( kav, "%s#PMentally Capable#n%s    ", openb, closeb);
    else if (wch->generation == 5)
      sprintf( kav, "%s#PMentally Capable#n%s    ", openb, closeb);
    else
      sprintf( kav, "%s#PMentally Challenged#n%s ", openb, closeb );
  }

  else if ( IS_WHO_CLASS(wch, CLASS_POWER_LICH))
  {
    if (wch->generation == 0)
      sprintf( kav, "%s#7Lord Demilich#n%s       ", openb, closeb);
    else if (wch->generation == 1)
      sprintf( kav, "%s#7Demilich#n%s            ", openb, closeb );
    else if (wch->generation == 2)
      sprintf( kav, "%s#7Lich Lord#n%s           ", openb, closeb );
    else if (wch->generation == 3)
      sprintf( kav, "%s#7Power Lich#n%s          ", openb, closeb );
    else if (wch->generation == 4)
      sprintf( kav, "%s#7Ancient Lich#n%s        ", openb, closeb );
    else
      sprintf( kav, "%s#7Lich#n%s                ", openb, closeb );
  }
  else if ( IS_WHO_CLASS(wch, CLASS_PALADIN) )
   { if (wch->generation == 0)
        sprintf(kav, "%s#yG#7rand #yH#7igh #yP#7aladin#n%s    ", openb, closeb);
     else if (wch->generation == 1)
        sprintf(kav, "%s#yP#7aladin #yK#7night#n%s        ", openb, closeb);
     else if (wch->generation == 2)
        sprintf(kav, "%s#yH#7igh #yT#7emplar#n%s          ", openb, closeb);
     else if (wch->generation == 3)
        sprintf(kav, "%s#yK#7night #yT#7emplar#n%s        ", openb, closeb);
     else if (wch->generation == 4)
        sprintf(kav, "%s#yT#7emplar #yS#7quire#n%s        ", openb, closeb);
     else if (wch->generation == 5)
        sprintf(kav, "%s#yT#7emplar #yA#7pprentice#n%s    ", openb, closeb);
     else if (wch->generation == 6)
        sprintf(kav, "%s#yT#7emplar #yS#7tudent#n%s       ", openb, closeb);
     else if (wch->generation >= 7)
        sprintf(kav, "%s#yT#7emplar #yS#7tudent#n%s       ", openb, closeb);
     else
        sprintf(kav, "%s#yT#7emplar #yS#7tudent#n%s       ", openb, closeb);
   }
  else if ( IS_WHO_CLASS(wch, CLASS_INQUISITOR))
   {
     if (wch->generation == 0)
	sprintf(kav, "%s#nL#7o#nr#0d #nH#7i#ng#0h #nI#7n#nq#0u#ni#7s#ni#0t#no#7r#n%s",openb, closeb);
     else if (wch->generation == 1)
	sprintf(kav, "%s#nH#7i#ng#0h #nI#7n#nq#0u#ni#7s#ni#0t#no#7r#n%s     ",openb,closeb);
     else if (wch->generation == 2)
        sprintf(kav, "%s#nI#7n#nq#0u#ni#7s#ni#0t#no#7r#n%s          ",openb,closeb);
     else if (wch->generation == 3)
	sprintf(kav, "%s#nI#7n#nq#0u#ni#7s#ni#0t#no#7r#n T#7r#na#0i#nn#7e#0e#n%s  ",openb,closeb);
     else if (wch->generation == 4)
	sprintf(kav, "%s#nI#7n#nq#0u#ni#7s#ni#0t#no#7r#n P#7u#np#0i#nl#7%s    ",openb,closeb);
     else
        sprintf(kav, "%s#nQ#0u#ne#7s#nt#0i#no#7n#ne#0r#n%s          ",openb, closeb);

   }
  else if ( IS_WHO_CLASS(wch, CLASS_HUNTER) )
   {
     if (wch->generation == 0)
	sprintf(kav, "%s#RSu#rpe#Rrn#rat#Rur#ral #RHu#rnt#Rer%s   ", openb, closeb);
//   else if (wch->generation == 1)
//	sprintf(kav, "%s#RE#rl#Ri#rt#Re #rH#Ru#rn#Rt#re#Rr$s         ", openb, closeb);
//     else if (wch->generation == 2)
//        sprintf(kav, "%s#RV#re#Rt#re#Rr#ra#Rn #rH#Ru#rn#Rt#re#Rr%s        ", openb, closeb);
//     else
//	sprintf(kav, "%s#RH#ru#Rn#rt#Re#rr               ", openb, closeb);
   }
	
  else if ( IS_WHO_CLASS(wch, CLASS_HIGHLANDER) )
   {
     if (wch->generation == 0)
	sprintf(kav, "%s#RLotus W#yarrior#n%s         ", openb, closeb);
     else if (wch->generation == 1)
        sprintf(kav, "%s#RSho#ygun#n%s                ", openb, closeb);
     else if (wch->generation == 3)
        sprintf(kav, "%s#RDai#ymyo#n%s                ", openb, closeb);
     else if (wch->generation == 2)
        sprintf(kav, "%s#RHata#ymoto#n%s              ", openb, closeb);
     else if (wch->generation == 4)
        sprintf(kav, "%s#RSamu#yrai#n%s               ", openb, closeb);
     else 
	sprintf(kav, "%s#RRon#yin#n%s                 ", openb, closeb);
   }
  else if ( IS_WHO_CLASS(wch, CLASS_NINJA) )
    {
      if (wch->generation == 0)
        sprintf( kav, "%s#yGhost Faced Killer#n%s  ", openb, closeb);
      else if (wch->generation == 1)
        sprintf( kav, "%s#yMaster Assassin#n%s     ", openb, closeb);
      else if (wch->generation == 2)
        sprintf( kav, "%s#yExpert Assassin#n%s     ", openb, closeb);
      else if (wch->generation == 3)
        sprintf( kav, "%s#yShadowlord#n%s          ", openb, closeb);
      else if (wch->generation == 4)
        sprintf( kav, "%s#yShadow Warrior#n%s      ", openb, closeb);
      else if (wch->generation == 5)
        sprintf( kav, "%s#yShadow#n%s              ", openb, closeb);
      else if (wch->generation == 6)
        sprintf( kav, "%s#yNinja#n%s               ", openb, closeb);
      else if (wch->generation == 7)
        sprintf( kav, "%s#yNinja Apprentice#n%s    ", openb, closeb);
      else
        sprintf( kav, "%s#yThug#n%s                ", openb, closeb);
    }
    else if ( IS_WHO_CLASS(wch, CLASS_DEMON) )
    {
      if (wch->generation == 0)
        sprintf( kav, "%s#RDemon Lord#n%s              ", openb, closeb);
      else if (wch->generation == 1)
        sprintf( kav, "%s#RPit Lord#n%s                ", openb, closeb);
      else if (wch->generation == 2)
        sprintf( kav, "%s#RPit Fiend#n%s               ", openb, closeb);
      else if (wch->generation == 3)
        sprintf( kav, "%s#RGateKeeper#n%s              ", openb, closeb);
      else if (wch->generation == 4)
        sprintf( kav, "%s#RImp Lord#n%s                ", openb, closeb);
      else if (wch->generation == 5)
        sprintf( kav, "%s#RHell Spawn#n%s              ", openb, closeb);
      else if (wch->generation == 6)
        sprintf( kav, "%s#RDemon#n%s                   ", openb, closeb);
      else if (wch->generation == 7)
        sprintf( kav, "%s#RSpawnling#n%s               ", openb, closeb);
      else  
        sprintf( kav, "%s#RImp#n%s                     ", openb, closeb);
    }
    else if (wch->pcdata->chobj != NULL )
	sprintf( kav, "#nNone#n		               ");	
}
  else if (wch->class == 0)
  {
	if (!str_cmp(wch->pcdata->switchname,"dummy"))
	sprintf( kav, "#0T#Rh#0e #RL#0o#Rr#0d #Rof #0D#Re#0s#Rt#0r#Ru#0c#Rt#0i#Ro#0n#n   " );
	else if (!str_cmp(wch->pcdata->switchname,"Rand")) 
	sprintf( kav, "#yT#rh#Re #yD#rr#Rago#yn #yR#re#Rbor#yn#n         " );
	else
	sprintf( kav, "#nNone#n                      " );
  }


        if (wch->pcdata->title != NULL)
	sprintf(king,wch->pcdata->title);
	cprintf(sStatus, "#n[%s#n]", lstatus);
	if ( wch->level > 6 )
        {

	sprintf( buf3 + strlen(buf3), "     %s%s %s#7%s#n%s\n\r",
	    class,
	    sStatus,
	    kav,
	    wch->pcdata->switchname,
	    king);
	  imm = TRUE;
	}    
 
	else if ( wch->level >= 3 && wch->level <= 6 )
        {
          if ( wch->race >= 43 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf4 + strlen(buf4), "     %s%s %s#7%s#n%s\n\r",
              class,
        	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a16 = TRUE;
		ava = TRUE;
          }
          else if ( wch->race >= 40 && wch->race <= 42 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf6 + strlen(buf6), "     %s%s %s#7%s#n%s\n\r",
              class,
	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a15 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 37 && wch->race <= 39 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf7 + strlen(buf7), "     %s%s %s#7%s#n%s\n\r",
              class,
	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a14 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 34 && wch->race <= 36 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf8 + strlen(buf8), "     %s%s %s#7%s#n%s\n\r",
              class,
	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a13 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 31 && wch->race <= 33 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf9 + strlen(buf9), "     %s%s %s#7%s#n%s\n\r",
              class,
		sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a12 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 28 && wch->race <= 30 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf10 + strlen(buf10), "     %s%s %s#7%s#n%s\n\r",
              class,
		sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a11 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 25 && wch->race <= 27 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf11 + strlen(buf11), "     %s%s %s#7%s#n%s\n\r",
              class,
	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a10 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 22 && wch->race <= 24 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf12 + strlen(buf12), "     %s%s %s#7%s#n%s\n\r",
              class,
		sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a9 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 19 && wch->race <= 21 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf13 + strlen(buf13), "     %s%s %s#7%s#n%s\n\r",
              class,
	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a8 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 16 && wch->race <= 18 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf14 + strlen(buf14), "     %s%s %s#7%s#n%s\n\r",
              class,
	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a7 = TRUE;
                ava = TRUE;

          }
	  else if ( wch->race >= 13 && wch->race <= 15 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf99 + strlen(buf99), "     %s%s %s#7%s#n%s\n\r",
              class,
        sStatus,
              kav,
              wch->pcdata->switchname,
              king);
                a99 = TRUE;
                ava = TRUE;
          }

          else if ( wch->race >= 10 && wch->race <= 12 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf15 + strlen(buf15),"     %s%s %s#7%s#n%s\n\r",
              class,
	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a6 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 7 && wch->race <= 9 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf16 + strlen(buf16), "     %s%s %s#7%s#n%s\n\r",
              class,
		sStatus,
              kav,
              wch->pcdata->switchname,
              king);
		a5 = TRUE;
                ava = TRUE;

          }
          else if ( wch->race >= 4 && wch->race <= 6 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf17 + strlen(buf17), "     %s%s %s#7%s#n%s\n\r",
              class,
	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
	    a4 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race >= 1 && wch->race <= 3 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf18 + strlen(buf18),"     %s%s %s#7%s#n%s\n\r",
              class,
	sStatus,
              kav,
              wch->pcdata->switchname,
              king);
	    a3 = TRUE;
                ava = TRUE;
          }
          else if ( wch->race == 0 && !IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf19 + strlen(buf19), "     %s%s %s#7%s#n%s\n\r",
              class,
		sStatus,
              kav,
              wch->pcdata->switchname,
              king);
	    a2 = TRUE;
                ava = TRUE;
          }
          else if (IS_SET(wch->special, SPC_NEWBIE))
          {
            sprintf( buf20 + strlen(buf20), "     %s%s %s#7%s#n%s\n\r",
              class,
		sStatus,
              kav,
              wch->pcdata->switchname,
              king);
  //            a1 = TRUE;
              new = TRUE;
          }
        }

        else if ( wch->level < 3 )
        {
        sprintf( buf5 + strlen(buf5), "     %s%s %s#7%s#n%s\n\r",
            class,
		sStatus,
            kav,
            wch->pcdata->switchname,
            king);
          mor = TRUE;
        }

   if ( !mains )
   {
//      stc("\n\r",ch);
      rand_mud_title(ch,random);
      mains = TRUE;
   }
     
  }  

    if ( imm )
    {
      if ( !immortal )
      {
	rand_imms(ch,random);
	immortal = TRUE;
      }
	stc(buf3,ch);
	stc("\n\r",ch);
    }

    if ( ava )
    {
//      if ( immortal ) stc("\n\r",ch);
      if ( !avatar )
      {
	rand_avatar(ch,random);
	avatar = TRUE;
      }

      if ( a16 ) stc(buf4, ch);
      if ( a15 ) stc(buf6, ch);
      if ( a14 ) stc(buf7, ch);
      if ( a13 ) stc(buf8, ch);
      if ( a12 ) stc(buf9, ch);
      if ( a11 ) stc(buf10,ch);
      if ( a10 ) stc(buf11,ch);
      if ( a9  ) stc(buf12,ch);
      if ( a8  ) stc(buf13,ch);
      if ( a7  ) stc(buf14,ch);
      if ( a99 ) stc(buf99,ch);
      if ( a6  ) stc(buf15,ch);
      if ( a5  ) stc(buf16,ch);
      if ( a4  ) stc(buf17,ch);
      if ( a3  ) stc(buf18,ch);
      if ( a2  ) stc(buf19,ch);
//      if ( a1  ) stc(buf20,ch);
        stc("\n\r",ch);

    }

    if ( new )
    {
//      if ( avatar ) stc("\n\r",ch);
//      if ( immortal && !avatar) stc("\n\r",ch);
      if ( !newbie )
      {
        rand_newbie(ch,random);
        newbie = TRUE;
      }
      stc(buf20,ch);
        stc("\n\r",ch);
    }

    if ( mor )
    {
//      if ( avatar ) stc("\n\r",ch);
//      if ( immortal && !avatar ) stc("\n\r",ch);
//      if ( newbie ) stc("\n\r",ch); 
      if ( !mortal )
      { 
	rand_mortal(ch,random);
 	mortal = TRUE;
       }
      stc(buf5,ch);
        stc("\n\r",ch);

    }

     
//    stc("\n\r",ch);
    switch(random)
    {
    case 0:
    sprintf(buf2,"[#R****#n]#0=------=#n[#7***********#n]#7#7<[ #0P#nl#7a#ny#0e#nr#7s#n: #0%d #7]>#n[#7***********#n]#0=------=#n[#R****#n]#n",nMatch);
    cent_to_char(buf2,ch);
    break;
    case 1:
    sprintf(buf2,"#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L| #cP#Cl#7a#0y#7e#Cr#cs#0: #7%-2d#L|#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n",nMatch);
    cent_to_char(buf2,ch);
    break;
    case 2:
    sprintf(buf2,"#o<>#C*#o<>#C*#o<>#C*#o<>#G+#g-#G-#g-#G-#g-#G-#g-#G-#g+#G#y[#8+#7+#8+#0+#8+#7+#8+#y]#o> #yP#ol#Ga#gy#ye#or#Gs#n: #y%-2d#o<#y[#7#8+#7+#8+#0+#8+#7+#8+#y]#g+#G-#g-#G-#g-#G-#g-#G-#g-#G+#o<>#C*#o<>#C*#o<>#C*#o<>#n",nMatch);
    cent_to_char(buf2,ch);
    break;
    case 3:
    sprintf(buf2,"#7<=-=-=-=-=-=-=-=)[#0*#n*#0*#n*#0*#7]==> #0]#R< #RP#rl#na#7y#Re#rr#ns: #7%d #R>#0[ #7<==[#0*#n*#0*#n*#0*#7](=-=-=-=-=-=-=-=>#n",nMatch);
    cent_to_char(buf2,ch);
    break;
    case 4:
    sprintf(buf2,"#R(#0---------------#R+#y[#C*#y][#C*#y]#7><><><#n#0[ #CPlay#cers: #C%d #0]#7><><><#n#y[#C*#y][#C*#y]#R+#0--------------#R)#n",nMatch);
    cent_to_char(buf2,ch);
    break;
    }
    return;
 
}



void do_inventory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    bool found;

    if (!IS_NPC(ch) && IS_HEAD(ch,LOST_HEAD))
        {send_to_char( "You are not a container.\n\r", ch ); return;}
    else if (!IS_NPC(ch) && IS_EXTRA(ch,EXTRA_OSWITCH))
    {
        if ( !IS_NPC(ch) && (obj = ch->pcdata->chobj) == NULL)
	    {send_to_char( "You are not a container.\n\r", ch ); return;}
	switch ( obj->item_type )
	{
	default:
	    send_to_char( "You are not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "You don't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "You are empty.\n\r", ch );
		break;
	    }
	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in you.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "You contain a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "You're about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "You're about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "You are almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "You're completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow you are MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    act( "$p contain:", ch, obj, NULL, TO_CHAR, FALSE );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	    continue;

	send_to_char( where_name[iWear], ch );
	if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
	else
	{
	    send_to_char( "something.\n\r", ch );
	}
	found = TRUE;
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}


void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}



void do_wizlist( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "wizlist" );
    return;
}



void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
       if(IS_IMMORTAL(ch))
       {
         stc( "Players in the World:\n\r", ch);
        for ( d = first_descriptor; d != NULL; d = d->next )
        {
            if ( ( d->connected == CON_PLAYING
            ||   d->connected == CON_EDITING)
            && ( victim = d->character ) != NULL
            &&   !IS_NPC(victim)
            &&   victim->in_room != NULL
            &&   victim->pcdata->chobj == NULL
            &&   can_see( ch, victim) )
            {
                found = TRUE;
                chprintf( ch, "%-14s [%9d] - %-20s - %s\n\r",
                    victim->name, victim->in_room->vnum, victim->in_room->area->name, victim->in_room->name );
            }
         }
        return;
       }


        send_to_char( "Players near you:\n\r", ch );
        found = FALSE;
        for ( d = first_descriptor; d != NULL; d = d->next )
        {
            if ( ( d->connected == CON_PLAYING
            ||   d->connected == CON_EDITING)
            && ( victim = d->character ) != NULL
            &&   !IS_NPC(victim)
            &&   victim->in_room != NULL
            &&   victim->in_room->area == ch->in_room->area
            &&   victim->pcdata->chobj == NULL
            &&   can_see( ch, victim ) )
            {
                found = TRUE;
                chprintf( ch, "%-28s - %-28s - %s\n\r",
                    victim->name, victim->in_room->area->name, victim->in_room->name );
            }
        }
        if ( !found )
            send_to_char( "None\n\r", ch );
    }
    else
    {
        found = FALSE;
        for ( victim = char_list; victim != NULL; victim = victim->next )
        {
            if ( victim->in_room != NULL
            &&   victim->in_room->area == ch->in_room->area
            &&   !IS_AFFECTED(victim, AFF_HIDE)
            &&   !IS_AFFECTED(victim, AFF_SNEAK)
            &&   can_see( ch, victim )
            &&   is_name( arg, victim->name ) )
            {
                found = TRUE;
                sprintf( buf, "%-28s - %-28s - %s\n\r",
                    PERS(victim, ch), victim->in_room->area->name,  victim->in_room->name );
                send_to_char( buf, ch );
                break;
            }
        }
        if ( !found )
            act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR, FALSE );
    }

    return;
}


void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    int diff;
    int overall;
    int con_hit;
    int con_dam;
    int con_ac;
    int con_hp;

    one_argument( argument, arg );
    overall = 0;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    act( "You examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_CHAR, FALSE );
    act( "$n examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_NOTVICT, FALSE );
    act( "$n examines you closely, looking for your weaknesses.", ch, NULL, victim, TO_VICT, FALSE );

    if (!IS_NPC(victim)) do_skill(ch,victim->name);

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && 
	IS_EXTRA(victim, EXTRA_FAKE_CON))
    {
	con_hit = victim->pcdata->fake_hit;
	con_dam = victim->pcdata->fake_dam;
	con_ac = victim->pcdata->fake_ac;
	con_hp = victim->pcdata->fake_hp;
    }
    else
    {
	con_hit = char_hitroll(victim);
	con_dam = char_damroll(victim);
	con_ac = char_ac(victim);
	con_hp = victim->hit;
    }
    if (con_hp < 1) con_hp = 1;

    diff = victim->level - ch->level + con_hit - char_hitroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more skilled than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as skilled as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as skilled as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as skilled as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more skilled than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more skilled than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more skilled than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR, FALSE );

    diff = victim->level - ch->level + con_dam - char_damroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more powerful than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as powerful as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as powerful as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as powerful as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more powerful than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more powerful than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more powerful than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR, FALSE );

    diff = ch->hit * 100 / con_hp;
         if ( diff <=  10 ) {msg = "$E is currently FAR healthier than you are."; overall = overall - 3;}
    else if ( diff <=  50 ) {msg = "$E is currently much healthier than you are."; overall = overall - 2;}
    else if ( diff <=  75 ) {msg = "$E is currently slightly healthier than you are."; overall = overall - 1;}
    else if ( diff <= 125 ) {msg = "$E is currently about as healthy as you are.";}
    else if ( diff <= 200 ) {msg = "You are currently slightly healthier than $M."; overall = overall + 1;}
    else if ( diff <= 500 ) {msg = "You are currently much healthier than $M."; overall = overall + 2;}
    else                    {msg = "You are currently FAR healthier than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR, FALSE );

    diff = con_ac - char_ac(ch);
         if ( diff <= -100) {msg = "$E is FAR better armoured than you."; overall = overall - 3;}
    else if ( diff <= -50 ) {msg = "$E looks much better armoured than you."; overall = overall - 2;}
    else if ( diff <= -25 ) {msg = "$E looks better armoured than you."; overall = overall - 1;}
    else if ( diff <=  25 ) {msg = "$E seems about as well armoured as you.";}
    else if ( diff <=  50 ) {msg = "You are better armoured than $M."; overall = overall + 1;}
    else if ( diff <=  100) {msg = "You are much better armoured than $M."; overall = overall + 2;}
    else                    {msg = "You are FAR better armoured than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR, FALSE );

    diff = overall;
         if ( diff <= -11 ) msg = "Conclusion: $E would kill you in seconds.";
    else if ( diff <=  -7 ) msg = "Conclusion: You would need a lot of luck to beat $M.";
    else if ( diff <=  -3 ) msg = "Conclusion: You would need some luck to beat $N.";
    else if ( diff <=   2 ) msg = "Conclusion: It would be a very close fight.";
    else if ( diff <=   6 ) msg = "Conclusion: You shouldn't have a lot of trouble defeating $M.";
    else if ( diff <=  10 ) msg = "Conclusion: $N is no match for you.  You can easily beat $M.";
    else                    msg = "Conclusion: $E wouldn't last more than a few seconds against you.";
    act( msg, ch, NULL, victim, TO_CHAR, FALSE );

    return;
}


void set_prefix( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }


    cprintf(buf, "[%11.11s#n]", title);

    free_string( ch->prefix );
    ch->prefix = str_dup( buf );
    return;
}



void do_prefix( CHAR_DATA *ch, char *argument )
{
 
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your prefix to what?\n\r", ch );
	return;
    }

//    if ( strlen(argument) > 13 )
//	argument[13] = '\0';

    argument = string_restrict(argument, 11);
    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);
    set_prefix( ch, argument );
    send_to_char( "Prefix Set.\n\r", ch );
}

void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( isalpha(title[0]) || isdigit(title[0]) || title[0] == '#' || 
title[0] == '{' )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}
void set_kingtitle( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];
    
    if ( IS_NPC(ch) )
    {
        bug( "Set_title: NPC.", 0 );
        return;
    }
     
    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
        buf[0] = ' ';
        strcpy( buf+1, title );
    }
    else
    {
        strcpy( buf, title );
    }
    
    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );   
    return;
}    


void do_title( CHAR_DATA *ch, char *argument )
{
 
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;
    if (IS_SET(ch->special, SPC_NOTITLE))
    {
	stc("You cannot do that.\n\r",ch);
	return;
    }
     if (!xIS_SET(ch->extra, EXTRA_CHAT))
        {
          stc("You have not yet been allowed to communicate.\n\r",ch);
          return;
        }
    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your title to what?\n\r", ch );
	return;
    }

//    if ( strlen(argument) > 17 )
//	argument[17] = '\0';

    argument = string_restrict(argument, 17);
    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);
    set_title( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}
void do_kingrank( CHAR_DATA *ch, char *argument )
{
     
    char buf[MAX_STRING_LENGTH];
        
    if ( IS_NPC(ch) )
        return;
    
    if ( argument[0] == '\0' )
    {
        send_to_char( "Change your title to what?\n\r", ch );
        return;
    }   
     
    if ( strlen(argument) > 17 )
        argument[17] = '\0';
 
    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);
    set_kingtitle( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}
void do_afk( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

	if ( IS_NPC(ch) )
	{
	return;
        }
	if (ch->fight_timer>0)
	{
	send_to_char("Not with a fight timer, coward.\n\r",ch);
	return;
	}
        if (!IS_SET(ch->in_room->room_flags,ROOM_SAFE) && !IS_IMMORTAL(ch))
  	{
   	 send_to_char( "Only in a safe room can you be AFK!\n\r", ch );
   	 return;
  	}
        if (ch->level <= 3)
	{
	send_to_char( "Sorry, its been abused again.\n\r",ch);
	return;
	}
        if (xIS_SET(ch->extra, EXTRA_AFK))
	{
	send_to_char( "You are no longer AFK.\n\r", ch );
	sprintf(buf,"%s is no longer AFK!",ch->pcdata->switchname);
        ADD_COLOUR(ch,buf,L_RED);
	do_info(ch,buf);
	xREMOVE_BIT(ch->extra,EXTRA_AFK);
        return;
	}
	
	else if (!xIS_SET(ch->extra,EXTRA_AFK))
	{
	send_to_char( "You are now AFK.\n\r", ch );
	sprintf(buf,"%s is now AFK!",ch->pcdata->switchname);
	ADD_COLOUR(ch,buf,L_RED);
	do_info(ch,buf);
	xSET_BIT(ch->extra,EXTRA_AFK);
	WAIT_STATE(ch, 25);
        return;
	}

else
return;

}


void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

	if ( strlen2(buf) + strlen2(argument) >= MAX_STRING_LENGTH - 2 )
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    char hit_str[MAX_INPUT_LENGTH];
    char mana_str[MAX_INPUT_LENGTH];
    char move_str[MAX_INPUT_LENGTH];
    char mhit_str[MAX_INPUT_LENGTH];
    char mmana_str[MAX_INPUT_LENGTH];
    char mmove_str[MAX_INPUT_LENGTH];
    char exp_str[MAX_INPUT_LENGTH];
    sprintf(hit_str, "%d", ch->hit);
    COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
    sprintf(mana_str, "%d", ch->mana);
    COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
    sprintf(move_str, "%d", ch->move);
    COL_SCALE(move_str, ch, ch->move, ch->max_move);
    sprintf(exp_str, "#C%2.0f#n", ch->exp);
//    COL_SCALE(exp_str, ch, ch->exp, 1000);
    sprintf(mhit_str, "%d", ch->max_hit);
    ADD_COLOUR(ch, mhit_str, L_CYAN);
    sprintf(mmana_str, "%d", ch->max_mana);
    ADD_COLOUR(ch, mmana_str, L_CYAN);
    sprintf(mmove_str, "%d", ch->max_move);
    ADD_COLOUR(ch, mmove_str, L_CYAN);
    sprintf( buf,
	"You report: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	hit_str,  mhit_str,
	mana_str, mmana_str,
	move_str, mmove_str,
	exp_str   );

    send_to_char( buf, ch );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch == NULL ) continue;
	if ( vch == ch ) continue;
	if ( vch->in_room == NULL ) continue;
	if ( vch->in_room != ch->in_room ) continue;
    	sprintf(hit_str, "%d", ch->hit);
    	COL_SCALE(hit_str, vch, ch->hit, ch->max_hit);
    	sprintf(mana_str, "%d", ch->mana);
    	COL_SCALE(mana_str, vch, ch->mana, ch->max_mana);
    	sprintf(move_str, "%d", ch->move);
    	COL_SCALE(move_str, vch, ch->move, ch->max_move);
    	sprintf(exp_str, "#C%2.0f#n", ch->exp);
//    	COL_SCALE(exp_str, vch, ch->exp, 1000);
    	sprintf(mhit_str, "%d", ch->max_hit);
    	ADD_COLOUR(vch, mhit_str, L_CYAN);
    	sprintf(mmana_str, "%d", ch->max_mana);
    	ADD_COLOUR(vch, mmana_str, L_CYAN);
    	sprintf(mmove_str, "%d", ch->max_move);
    	ADD_COLOUR(vch, mmove_str, L_CYAN);
    	if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    ch->morph,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
        else
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    IS_NPC(ch) ? capitalize(ch->short_descr) : ch->name,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
	buf[0] = UPPER(buf[0]);
    	send_to_char( buf, vch );
    }
    return;
}



void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;

    if ( IS_NPC(ch) )
	return;
    if (argument[0] == '\0')
    {
     int col, i;

     col = 0;
     for (i = 0; i < 5; i++)
     {
      for (sn = 0; sn < MAX_SKILL; sn++)
      {
        if (skill_table[sn].name == NULL)
          break;
        if (ch->level < skill_table[sn].skill_level)
          continue;
        if (skill_table[sn].target != i)
          continue;
        switch (i)
        {
          case 0:
            sprintf(buf, "#p%18s #7%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 1:
            sprintf(buf, "#R%18s #7%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 2:
            sprintf(buf, "#L%18s #7%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 3:
            sprintf(buf, "#G%18s #7%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 4:
            sprintf(buf, "#y%18s #7%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          default:
            sprintf(buf, " ");
            break;
        }
        send_to_char(buf, ch);
        if (++col % 3 == 0)
          send_to_char("\n\r", ch);
      }
     }
      send_to_char("#n\n\r", ch);

      if (col % 3 != 0)
        send_to_char("\n\r", ch);

      sprintf(buf, "You have %2.0f exp left.\n\r", ch->exp);
      send_to_char(buf, ch);
    }
    else if (!strcmp(argument,"all"))
    {
        if (ch->exp < 1000000)
        {
            send_to_char("This costs one million exp, and you don't have that much.\n\r",ch);
            return;
        }
        for ( sn = 0; sn < MAX_SKILL; sn++ )
        {
            if (
		(skill_table[sn].name != NULL) && 
		(ch->level >= skill_table[sn].skill_level)
	       )     
           ch->pcdata->learned[sn]     = SKILL_ADEPT; 
        }
        ch->exp -= 1000000;
        send_to_char("All skills available for your level has been trained.\n\r",ch);
        return;
    }
 
    else
    {
	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

	if ( ch->exp <= 0 )
	{
	    send_to_char( "You have no exp left.\n\r", ch );
	    return;
	}

	if ( ( sn = skill_lookup( argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   ch->level < skill_table[sn].skill_level ) )
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}

	
	if ( ch->pcdata->learned[sn] >= SKILL_ADEPT )
	{
	    sprintf( buf, "You are already an adept of %s.\n\r",
	         skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else if ( ch->pcdata->learned[sn] > 0 &&
		ch->exp < 5000 )
	{
	    sprintf( buf, "You need 5000 exp to increase %s any more.\n\r",
		(skill_table[sn].name));
	    send_to_char( buf, ch );
	}
	else if ( ch->pcdata->learned[sn] == 0 && ch->exp < 5000 )
	{
	    sprintf( buf, "You need 5000 exp to increase %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    if (ch->pcdata->learned[sn] == 0)
	    {
	    	ch->exp -= 5000;
	    	ch->pcdata->learned[sn] = SKILL_ADEPT;
	    }
	    else
	    {
	    	ch->exp -= (ch->pcdata->learned[sn]/2);
	    	ch->pcdata->learned[sn] += int_app[get_curr_int(ch)].learn;
	    }
	    if ( ch->pcdata->learned[sn] < SKILL_ADEPT )
	    {
		act( "You practice $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR, FALSE );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = SKILL_ADEPT;
		act( "You are now an adept of $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR, FALSE );
	    }
	}
    }
    return;
}



/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
	wimpy = ch->max_hit / 5;
    else
	wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }

    if ( wimpy > ch->max_hit * 0.35 )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;
    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    return;
}


void do_password( CHAR_DATA *ch, char *argument )
{
    char buf[MSL];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    //
     // Can't use one_argument here because it smashes case.
     // So we just steal all its code.  Bleagh.
     //
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp( arg1, ch->pcdata->pwd ) 
&&         strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ))
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

     // No tilde allowed because of player file format.
    pwdnew = crypt( arg2, ch->pcdata->switchname );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    if (!IS_EXTRA(ch,EXTRA_NEWPASS)) xSET_BIT(ch->extra,EXTRA_NEWPASS);
    if ( IS_SET(sysdata.save_flags, SV_PASSCHG) )
       save_char_obj( ch );
    if (ch->desc != NULL && ch->desc->connected == CON_PLAYING )
	send_to_char( "Ok.\n\r", ch );
    sprintf(buf, "%s password: XXXXXXXXX XXXXXXXXX",ch->pcdata->switchname);
    log_string(buf);
    return;
}


void do_socials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; iSocial < maxSocial; iSocial++ )
    {
	sprintf( buf, "%-12s", social_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}



void do_xsocials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; xsocial_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	sprintf( buf, "%-12s", xsocial_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}



void do_spells( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;
    int col;

    col = 0;
    for ( sn = 0; sn < MAX_SKILL && skill_table[sn].name != NULL; sn++ )
    {
	sprintf( buf, "%-12s", skill_table[sn].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}


struct sort_struct {
  int sort_pos;
} *cmd_sort_info = NULL;

int num_of_cmds;

void sort_commands(void)
{
  int a, b, tmp;

  num_of_cmds = 0;

  /*
   * first, count commands (num_of_commands is actually one greater than
   * the number of commands; it inclues the '\n'.
   */
  while(cmd_table[num_of_cmds].name[0] != '\0')
    num_of_cmds++;

  /* create data array */
  CREATE(cmd_sort_info, struct sort_struct, num_of_cmds);

  /* initialize it */
  for (a = 1; a < num_of_cmds; a++) {
    cmd_sort_info[a].sort_pos = a;
  }

  for (a = 0; a < num_of_cmds; a++)
    for (b = a + 1; b < num_of_cmds; b++)
      if (strcmp(cmd_table[cmd_sort_info[a].sort_pos].name,
		 cmd_table[cmd_sort_info[b].sort_pos].name) > 0) {
	tmp = cmd_sort_info[a].sort_pos;
	cmd_sort_info[a].sort_pos = cmd_sort_info[b].sort_pos;
	cmd_sort_info[b].sort_pos = tmp;
      }
}


/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
    int i;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        i = cmd_sort_info[cmd].sort_pos;
        if ( cmd_table[i].level <= LEVEL_HERO
        &&   cmd_table[i].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-12s", cmd_table[i].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}

void do_wizhelp(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];
    char buf2[MSL];
    int cmd;
    int col;
    int i;
    int sex = 0;
    int level;

    col = 0;
    for ( level = LEVEL_HERO+1; level <= MAX_LEVEL; level++)
    {
        if (level > get_trust( ch ) )
           break;

        if ( level == LEVEL_HERO+1 )
        {
            sprintf(buf, "\n\r--------------------LOWLEVEL COMMANDS------------------\n\r");
            stc(buf, ch);
        }
        else if (level > 6) 
        {
            sprintf(buf, "\n\r\n\r--------------------LEVEL %d COMMANDS-------------------\n\r", level);
            stc(buf, ch);
        }

        for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
        {
            i = cmd_sort_info[cmd].sort_pos;
            if ( cmd_table[i].level == level
            &&   cmd_table[i].level <= get_trust( ch ) )
            {
                sex++;

                sprintf( buf2, "%-12s", cmd_table[i].name );
                send_to_char( buf2, ch );
                if ( ++col % 6 == 0 )
                    send_to_char( "\n\r", ch );
            }
        }
    col = 0;
    }

    if ( col % 6 != 0 )
        send_to_char( "\n\r", ch );

    sprintf( buf, "\n\r#n%d total imm commands available to you.\n\r", 
sex);
    stc(buf, ch);
    send_to_char("\n\r#nLevels: #p7 #w8 #r9 #g10 #y11 #c12\n\r", ch);
    return;
}


void do_channels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( arg[0] == '\0' )
    {
	if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_SILENCE) )
	{
	    send_to_char( "You are silenced.\n\r", ch );
	    return;
	}

	send_to_char( "Channels:", ch );

	send_to_char( !xIS_SET(ch->deaf, CHANNEL_CHAT)
	    ? " +CHAT"
	    : " -chat",
	    ch );


	send_to_char( !xIS_SET(ch->deaf, CHANNEL_FLAME)
	    ? " +FLAME"
	    : " -flame",
	    ch );

	if ( IS_IMMORTAL(ch) )
	{
	    send_to_char( !xIS_SET(ch->deaf, CHANNEL_IMMTALK)
		? " +IMMTALK"
		: " -immtalk",
		ch );
	}

	send_to_char( !xIS_SET(ch->deaf, CHANNEL_MUSIC)
	    ? " +MUSIC"
	    : " -music",
	    ch );

        send_to_char( !xIS_SET(ch->deaf, CHANNEL_NEWBIE)
            ? " +NEWBIE"
            : " -newbie",
            ch );

	send_to_char( !xIS_SET(ch->deaf, CHANNEL_SHOUT)
	    ? " +SHOUT"
	    : " -shout",
	    ch );

	send_to_char( !xIS_SET(ch->deaf, CHANNEL_HOWL)
	    ? " +HOWL"
	    : " -howl",
	    ch );

	send_to_char( !xIS_SET(ch->deaf, CHANNEL_UPDATE)
	    ? " +UPDATE"
	    : " -update",
	    ch );
	if (IS_IMMORTAL(ch))
	{
	    send_to_char( !xIS_SET(ch->deaf, CHANNEL_LOG)
		? " +LOG"
		: " -log",
		ch );
	}

	if ( IS_CLASS(ch, CLASS_SHAPESHIFTER) || IS_IMMORTAL(ch))
	{
		send_to_char( !xIS_SET(ch->deaf, CHANNEL_TELEPATH)
			? " +TELE"
			: " -tele",
			ch );
	}

        if (IS_CLASS( ch, CLASS_MAGE) || IS_IMMORTAL(ch))
        {
            send_to_char( !xIS_SET(ch->deaf, CHANNEL_MAGETALK)
                ? " +MAGE"
                : " -mage",
                ch );
        }

        if (IS_CLASS( ch, CLASS_MONK) || IS_IMMORTAL(ch))
        {
            send_to_char( !xIS_SET(ch->deaf, CHANNEL_MONK)
                ? " +MONK"
                : " -monk",
                ch );
        }

	if (IS_CLASS(ch, CLASS_DEMON) || IS_IMMORTAL(ch))
	{
	    send_to_char( !xIS_SET(ch->deaf, CHANNEL_PRAY)
		? " +PRAY"
		: " -pray",
		ch );
	}

	send_to_char( !xIS_SET(ch->deaf, CHANNEL_INFO)
	    ? " +INFO"
	    : " -info",
	    ch );

        send_to_char( !xIS_SET(ch->deaf, CHANNEL_HINT)
            ? " +HINT"
            : " -hint",
            ch );

	if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_IMMORTAL(ch))
	{
	    send_to_char( !xIS_SET(ch->deaf, CHANNEL_VAMPTALK)
		? " +VAMP"
		: " -vamp",
		ch );
	}

	if (IS_CLASS(ch, CLASS_HIGHLANDER) || IS_IMMORTAL(ch)) {
		send_to_char( !xIS_SET(ch->deaf, CHANNEL_HIGHTALK)
			? " +SAMT"
			: " -samt",
			ch ); }
        if (IS_CLASS(ch, CLASS_PALADIN) || IS_IMMORTAL(ch)) {
                send_to_char( !xIS_SET(ch->deaf, CHANNEL_PALADINTALK)
                        ? " +RITUAL"
                        : " -ritual",
                        ch ); }
	if (IS_CLASS(ch, CLASS_INQUISITOR) || IS_IMMORTAL(ch)) {
		stc(!xIS_SET(ch->deaf, CHANNEL_INQUISITOR)
			? " +INQUIRE"
			: " -ritual",
			ch );}
	if (IS_CLASS(ch, CLASS_HUNTER) || IS_IMMORTAL(ch)) {
		stc(!xIS_SET(ch->deaf, CHANNEL_HUNTER)
			? " +TRANSMIT"
			: " -transmit",
			ch );}
	if( IS_CLASS(ch, CLASS_DROW) || IS_IMMORTAL(ch)) {
		send_to_char( !xIS_SET(ch->deaf, CHANNEL_SIGN)
			? " +SIGN"
			: " -sign",
			ch ); }

	send_to_char( !xIS_SET(ch->deaf, CHANNEL_TELL)
	    ? " +TELL"
	    : " -tell",
	    ch );

	send_to_char( ".\n\r", ch );
    }
    else
    {
	bool fClear;
	int bit;

	     if ( arg[0] == '+' ) fClear = TRUE;
	else if ( arg[0] == '-' ) fClear = FALSE;
	else
	{
	    send_to_char( "Channels -channel or +channel?\n\r", ch );
	    return;
	}

        if ( !str_cmp( arg+1, "chat"     ) ) bit = CHANNEL_CHAT;
	else if ( !str_cmp( arg+1, "immtalk"  ) ) bit = CHANNEL_IMMTALK;
        else if ( !str_cmp( arg+1, "flame"  ) ) bit = CHANNEL_FLAME;
	else if ( !str_cmp( arg+1, "music"    ) ) bit = CHANNEL_MUSIC;
        else if ( !str_cmp( arg+1, "newbie"   ) ) bit = CHANNEL_NEWBIE;
	else if ( !str_cmp( arg+1, "shout"    ) ) bit = CHANNEL_SHOUT;
	else if ( !str_cmp( arg+1, "yell"     ) ) bit = CHANNEL_YELL;
	else if ( !str_cmp( arg+1, "howl"     ) ) bit = CHANNEL_HOWL;
	else if (IS_IMMORTAL(ch) && !str_cmp( arg+1, "log") ) bit = CHANNEL_LOG;
        else if ( !str_cmp( arg+1, "monk"     ) ) bit = CHANNEL_MONK;          
        else if ( !str_cmp( arg+1, "mage"     ) ) bit = CHANNEL_MAGETALK;
	else if ( !str_cmp( arg+1, "info"     ) ) bit = CHANNEL_INFO;
	else if ( !str_cmp( arg+1, "vamp"     ) ) bit = CHANNEL_VAMPTALK;
	else if ( !str_cmp( arg+1, "tell"     ) ) bit = CHANNEL_TELL;
	else if ( !str_cmp( arg+1, "update"   ) ) bit = CHANNEL_UPDATE;
	else if ( !str_cmp( arg+1, "chant"    ) ) bit = CHANNEL_PALADINTALK;
	else if ( !str_cmp( arg+1, "hint"     ) ) bit = CHANNEL_HINT;
	else
	{
	    send_to_char( "Set or clear which channel?\n\r", ch );
	    return;
	}

	if ( fClear )
	    xREMOVE_BIT (ch->deaf, bit);
	else
	    xSET_BIT    (ch->deaf, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}



/*
 * Contributed by Grodyn.
 */
void do_config( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "[ Keyword  ] Option\n\r", ch );

	send_to_char(  xIS_SET(ch->act, PLR_ANSI)
            ? "[+ANSI     ] You have ansi colour on.\n\r"
	    : "[-ansi     ] You have ansi colour off.\n\r"
	    , ch );

	send_to_char(  xIS_SET(ch->act, PLR_AUTOEXIT)
            ? "[+AUTOEXIT ] You automatically see exits.\n\r"
	    : "[-autoexit ] You don't automatically see exits.\n\r"
	    , ch );

	send_to_char(  xIS_SET(ch->act, PLR_AUTOLOOT)
	    ? "[+AUTOLOOT ] You automatically loot corpses.\n\r"
	    : "[-autoloot ] You don't automatically loot corpses.\n\r"
	    , ch );
        send_to_char(  xIS_SET(ch->act, PLR_AUTOSLAB)
            ? "[+AUTOSLAB ] You automatically loot adamantite from corpses.\n\r"
            : "[-autoslab ] You don't automatically loot adamantite from corpses.\n\r"
            , ch );
        send_to_char( xIS_SET(ch->act, PLR_AUTOELEC)
            ? "[+AUTOELECTRUM] You automatially loot Electrum from corpses.\n\r"
            : "[-autoelectrum ] You dont automatically loot Electrum from corpses.\n\r"
            , ch );
        send_to_char(  xIS_SET(ch->act, PLR_AUTOGOLD)
            ? "[+AUTOGOLD ] You automatically loot gold from corpses.\n\r"
            : "[-autogold ] You don't automatically loot gold from corpses.\n\r"
            , ch );

	send_to_char(  xIS_SET(ch->act, PLR_AUTOSAC)
	    ? "[+AUTOSAC  ] You automatically sacrifice corpses.\n\r"
	    : "[-autosac  ] You don't automatically sacrifice corpses.\n\r"
	    , ch );

	send_to_char(  xIS_SET(ch->act, PLR_BLANK)
	    ? "[+BLANK    ] You have a blank line before your prompt.\n\r"
	    : "[-blank    ] You have no blank line before your prompt.\n\r"
	    , ch );

	send_to_char(  xIS_SET(ch->act, PLR_BRIEF)
	    ? "[+BRIEF    ] You see brief descriptions.\n\r"
	    : "[-brief    ] You see long descriptions.\n\r"
	    , ch );

        send_to_char(  xIS_SET(ch->act, PLR_BRIEF2)
            ? "[+BRIEF2   ] You do not see other people's attacks.\n\r"
            : "[-brief2   ] You see other people's attacks.\n\r"
	    , ch);
        send_to_char(  xIS_SET(ch->act, PLR_BRIEF3)
            ? "[+BRIEF3   ] Your combat is super-brief.\n\r"
            : "[-brief3   ] Your combat is normal.\n\r"
            , ch);

	send_to_char(  xIS_SET(ch->act, PLR_COMBINE)
	    ? "[+COMBINE  ] You see object lists in combined format.\n\r"
	    : "[-combine  ] You see object lists in single format.\n\r"
	    , ch );

	send_to_char(  xIS_SET(ch->act, PLR_PROMPT)
	    ? "[+PROMPT   ] You have a prompt.\n\r"
	    : "[-prompt   ] You don't have a prompt.\n\r"
	    , ch );

	send_to_char(  xIS_SET(ch->act, PLR_TELNET_GA)
	    ? "[+TELNETGA ] You receive a telnet GA sequence.\n\r"
	    : "[-telnetga ] You don't receive a telnet GA sequence.\n\r"
	    , ch );

        send_to_char(  xIS_SET(ch->act, PLR_AUTOSLAB)
            ? "[+AUTOADAMANTITE ] You automatically loot adamantite from corpses.\n\r"
            : "[-autoadamantite ] You don't automatically loot adamantite from corpses.\n\r"
            , ch );

	send_to_char(  xIS_SET(ch->act, PLR_SILENCE)
	    ? "[+SILENCE  ] You are silenced.\n\r"
	    : ""
	    , ch );
        send_to_char( xIS_SET(ch->act, PLR_MAP)
      ? "#n[+MAP      ] You see the automapper.\n\r"
      : "#n[-map      ] You don't see the automapper.\n\r", ch);

        send_to_char(  xIS_SET(ch->act, PLR_AUTOSPLIT)
            ? "[+AUTOSPLIT] You automatically split gold from corpses.\n\r"
            : "[-autosplit] You don't automatically split gold from corpses.\n\r"
            , ch );

	send_to_char( !xIS_SET(ch->act, PLR_NO_TELL)
	    ? ""
	    : "[-tell     ] You can't use 'tell'.\n\r"
	    , ch );
    }
    else
    {
	bool fSet;
	int bit;

	     if ( arg[0] == '+' ) fSet = TRUE;
	else if ( arg[0] == '-' ) fSet = FALSE;
	else
	{
	    send_to_char( "Config -option or +option?\n\r", ch );
	    return;
	}
        if ( !str_cmp( arg+1, "ansi"     ) ) bit = PLR_ANSI;
        else if ( !str_cmp( arg+1, "autoexit" ) ) bit = PLR_AUTOEXIT;
	else if ( !str_cmp( arg+1, "autoloot" ) ) bit = PLR_AUTOLOOT;
	else if ( !str_cmp( arg+1, "autoadamantite" ) ) bit = PLR_AUTOSLAB;
        else if ( !str_cmp( arg+1, "autoelectrum" ) ) bit = PLR_AUTOELEC;
	else if ( !str_cmp( arg+1, "autosac"  ) ) bit = PLR_AUTOSAC;
	else if ( !str_cmp( arg+1, "autogold" ) ) bit = PLR_AUTOGOLD;
	else if ( !str_cmp( arg+1, "blank"    ) ) bit = PLR_BLANK;
	else if ( !str_cmp( arg+1, "brief"    ) ) bit = PLR_BRIEF;
        else if ( !str_cmp( arg+1, "brief2"    ) ) bit = PLR_BRIEF2;
        else if ( !str_cmp( arg+1, "brief3"    ) ) bit = PLR_BRIEF3;
	else if ( !str_cmp( arg+1, "combine"  ) ) bit = PLR_COMBINE;
        else if ( !str_cmp( arg+1, "prompt"   ) ) bit = PLR_PROMPT;
	else if ( !str_cmp( arg+1, "telnetga" ) ) bit = PLR_TELNET_GA;
        else if ( !str_cmp( arg+1, "map"))  bit = PLR_MAP;
	else if ( !str_cmp( arg+1, "autosplit" )) bit = PLR_AUTOSPLIT;
	else
	{
	    send_to_char( "Config which option?\n\r", ch );
	    return;
	}

	if ( fSet )
	    xSET_BIT    (ch->act, bit);
	else
	    xREMOVE_BIT (ch->act, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}

void do_ansi( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_ANSI)) do_config(ch,"-ansi");
    else do_config(ch,"+ansi");
    return;
}

void do_autoexit( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_AUTOEXIT)) do_config(ch,"-autoexit");
    else do_config(ch,"+autoexit");
    return;
}

void do_autoloot( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_AUTOLOOT)) do_config(ch,"-autoloot");
    else do_config(ch,"+autoloot");
    return;
}
void do_autosplit( CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_AUTOSPLIT)) do_config(ch,"-autosplit");
    else do_config(ch,"+autosplit");
    return;
}

void do_autoadamantite( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_AUTOSLAB)) do_config(ch,"-autoadamantite");
    else do_config(ch,"+autoadamantite");
    return;
}

void do_autoelectrum( CHAR_DATA *ch, char *argument )
{    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_AUTOELEC)) do_config(ch,"-autoelectrum");
    else do_config(ch,"+autoelectrum");
    return;
}

void do_autogold( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_AUTOGOLD)) do_config(ch,"-autogold");
    else do_config(ch,"+autogold");
    return;
}


void do_autosac( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_AUTOSAC)) do_config(ch,"-autosac");
    else do_config(ch,"+autosac");
    return;
}

void do_blank( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_BLANK)) do_config(ch,"-blank");
    else do_config(ch,"+blank");
    return;
}

void do_brief( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_BRIEF)) do_config(ch,"-brief");
    else do_config(ch,"+brief");
    return;
}
void do_map( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_MAP)) do_config(ch,"-map");
    else do_config(ch,"+map");
    return;
}

void do_brief2( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_BRIEF2)) do_config(ch,"-brief2");
    else do_config(ch,"+brief2");
    return;
}
void do_brief3( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (xIS_SET(ch->act, PLR_BRIEF3)) do_config(ch,"-brief3");
    else do_config(ch,"+brief3");
    return;
}

void do_diagnose( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg  [MAX_INPUT_LENGTH];
    int teeth = 0;
    int ribs = 0;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (arg == '\0')
    {
	send_to_char("Who do you wish to diagnose?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char("Nobody here by that name.\n\r",ch);
	return;
    }
    act("$n examines $N carefully, diagnosing $S injuries.",ch,NULL,victim,TO_NOTVICT, FALSE);
    act("$n examines you carefully, diagnosing your injuries.",ch,NULL,victim,TO_VICT, FALSE);
    act("Your diagnoses of $N reveals the following...",ch,NULL,victim,TO_CHAR, FALSE);
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    if ( ( victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + 
	   victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
	   victim->loc_hp[6] ) == 0 )
    {
	act("$N has no apparent injuries.",ch,NULL,victim,TO_CHAR, FALSE);
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
	return;
    }
    /* Check head */
    if (IS_HEAD(victim,LOST_EYE_L) && IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost both of $S eyes.",ch,NULL,victim,TO_CHAR, FALSE);
    else if (IS_HEAD(victim,LOST_EYE_L))
	act("$N has lost $S left eye.",ch,NULL,victim,TO_CHAR, FALSE);
    else if (IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost $S right eye.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_HEAD(victim,LOST_EAR_L) && IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost both of $S ears.",ch,NULL,victim,TO_CHAR, FALSE);
    else if (IS_HEAD(victim,LOST_EAR_L))
	act("$N has lost $S left ear.",ch,NULL,victim,TO_CHAR, FALSE);
    else if (IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost $S right ear.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_HEAD(victim,LOST_NOSE))
	act("$N has lost $S nose.",ch,NULL,victim,TO_CHAR, FALSE);
    else if (IS_HEAD(victim,BROKEN_NOSE))
	act("$N has got a broken nose.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_HEAD(victim,BROKEN_JAW))
	act("$N has got a broken jaw.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_HEAD(victim,LOST_HEAD))
    {
	act("$N has had $S head cut off.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_HEAD))
	    act("...Blood is spurting from the stump of $S neck.",ch,NULL,victim,TO_CHAR, FALSE);
    }
    else
    {
	if (IS_BODY(victim,BROKEN_NECK))
	    act("$N has got a broken neck.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BODY(victim,CUT_THROAT))
	{
	    act("$N has had $S throat cut open.",ch,NULL,victim,TO_CHAR, FALSE);
	    if (IS_BLEEDING(victim,BLEEDING_THROAT))
		act("...Blood is pouring from the wound.",ch,NULL,victim,TO_CHAR, FALSE);
	}
    }
    if (IS_HEAD(victim,BROKEN_SKULL))
	act("$N has got a broken skull.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_HEAD(victim,LOST_TOOTH_1 )) teeth += 1;
    if (IS_HEAD(victim,LOST_TOOTH_2 )) teeth += 2;
    if (IS_HEAD(victim,LOST_TOOTH_4 )) teeth += 4;
    if (IS_HEAD(victim,LOST_TOOTH_8 )) teeth += 8;
    if (IS_HEAD(victim,LOST_TOOTH_16)) teeth += 16;
    if (teeth > 0)
    {
	sprintf(buf,"$N has had %d teeth knocked out.",teeth);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    if (IS_HEAD(victim,LOST_TONGUE))
	act("$N has had $S tongue ripped out.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_HEAD(victim,LOST_HEAD))
    {
    	send_to_char("----------------------------------------------------------------------------\n\r",ch);
    	return;
    }
    /* Check body */
    if (IS_BODY(victim,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(victim,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(victim,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(victim,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(victim,BROKEN_RIBS_16)) ribs += 16;
    if (ribs > 0)
    {
	sprintf(buf,"$N has got %d broken ribs.",ribs);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    if (IS_BODY(victim,BROKEN_SPINE))
	act("$N has got a broken spine.",ch,NULL,victim,TO_CHAR, FALSE);
    /* Check arms */
    check_left_arm(ch,victim);
    check_right_arm(ch,victim);
    check_left_leg(ch,victim);
    check_right_leg(ch,victim);
    send_to_char("----------------------------------------------------------------------------\n\r",ch);
    return;
}

void check_left_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S arms.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L) && IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR, FALSE);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR, FALSE);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_ARM_L(victim,LOST_ARM))
    {
	act("$N has lost $S left arm.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N arms are both broken.",ch,NULL,victim,TO_CHAR, FALSE);
    else if (IS_ARM_L(victim,BROKEN_ARM))
	act("$N's left arm is broken.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND) &&
	!IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S hands.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L) && IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR, FALSE);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR, FALSE);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_ARM_L(victim,LOST_HAND))
    {
	act("$N has lost $S left hand.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    fingers = 0;
    if (IS_ARM_L(victim,BROKEN_FINGER_I) && !IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_M) && !IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_R) && !IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_L) && !IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    return;
}

void check_right_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	return;
    if (IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost $S right arm.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (!IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR, FALSE);
    else if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND))
	return;
    if (IS_ARM_R(victim,LOST_HAND))
    {
	act("$N has lost $S right hand.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    fingers = 0;
    if (IS_ARM_R(victim,BROKEN_FINGER_I) && !IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_M) && !IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_R) && !IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_L) && !IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    return;
}

void check_left_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost both of $S legs.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L) && IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR, FALSE);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR, FALSE);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_LEG_L(victim,LOST_LEG))
    {
	act("$N has lost $S left leg.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N legs are both broken.",ch,NULL,victim,TO_CHAR, FALSE);
    else if (IS_LEG_L(victim,BROKEN_LEG))
	act("$N's left leg is broken.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost both of $S feet.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L) && IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR, FALSE);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR, FALSE);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_LEG_L(victim,LOST_FOOT))
    {
	act("$N has lost $S left foot.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    toes = 0;
    if (IS_LEG_L(victim,BROKEN_TOE_A) && !IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_B) && !IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_C) && !IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_D) && !IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    return;
}

void check_right_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	return;
    if (IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost $S right leg.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (!IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N's right leg is broken.",ch,NULL,victim,TO_CHAR, FALSE);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
	return;
    if (IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost $S right foot.",ch,NULL,victim,TO_CHAR, FALSE);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR, FALSE);
	return;
    }
    if (IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    toes = 0;
    if (IS_LEG_R(victim,BROKEN_TOE_A) && !IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_B) && !IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_C) && !IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_D) && !IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    else if (IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR, FALSE);
    }
    return;
}

void obj_score( CHAR_DATA *ch, OBJ_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int itemtype;

    sprintf( buf,"You are %s.\n\r",obj->short_descr);
    send_to_char( buf, ch );

    sprintf( buf,"Type %s, Extra flags %s.\n\r",item_type_name(obj),
	extra_bit_name(obj->extra_flags));
    send_to_char( buf, ch );

    sprintf( buf,"You weigh %d pounds and are worth %d gold coins.\n\r",obj->weight,obj->cost);
    send_to_char( buf, ch );

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1 &&
	obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You were created by %s, and are owned by %s.\n\r", obj->questmaker,obj->questowner );
	send_to_char( buf, ch );
    }
    else if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	sprintf( buf, "You were created by %s.\n\r", obj->questmaker );
	send_to_char( buf, ch );
    }
    else if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You are owned by %s.\n\r", obj->questowner );
	send_to_char( buf, ch );
    }

    switch ( obj->item_type )
    {
    case ITEM_SCROLL: 
    case ITEM_POTION:
	sprintf( buf, "You contain level %d spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_QUEST:
	sprintf( buf, "Your quest point value is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_WAND: 
    case ITEM_STAFF: 
	sprintf( buf, "You have %d(%d) charges of level %d",
	    obj->value[1], obj->value[2], obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;
      
    case ITEM_WEAPON:
	sprintf( buf, "You inflict %d to %d damage in combat (average %d).\n\r",
	    obj->value[1], obj->value[2],
	    ( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] - ((obj->value[0] / 1000) * 1000);
	else
	    itemtype = obj->value[0];

	if (itemtype > 0)
	{
	    if (obj->level < 10)
		sprintf(buf,"You are a minor spell weapon.\n\r");
	    else if (obj->level < 20)
		sprintf(buf,"You are a lesser spell weapon.\n\r");
	    else if (obj->level < 30)
		sprintf(buf,"You are an average spell weapon.\n\r");
	    else if (obj->level < 40)
		sprintf(buf,"You are a greater spell weapon.\n\r");
	    else if (obj->level < 50)
		sprintf(buf,"You are a major spell weapon.\n\r");
	    else
		sprintf(buf,"You are a supreme spell weapon.\n\r");
	    send_to_char(buf,ch);
	}

	if (itemtype == 1)
	    sprintf (buf, "You are dripping with corrosive acid.\n\r");
	else if (itemtype == 4)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 30)
	    sprintf (buf, "You are the bane of all evil.\n\r");
	else if (itemtype == 34)
	    sprintf (buf, "You drink the souls of your victims.\n\r");
	else if (itemtype == 37)
	    sprintf (buf, "You have been tempered in hellfire.\n\r");
	else if (itemtype == 48)
	    sprintf (buf, "You crackle with sparks of lightning.\n\r");
	else if (itemtype == 53)
	    sprintf (buf, "You are dripping with a dark poison.\n\r");
	else if (itemtype > 0)
	    sprintf (buf, "You have been imbued with the power of %s.\n\r",skill_table[itemtype].name);
	if (itemtype > 0)
	    send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] / 1000;
	else
	    break;

	if (itemtype == 4 || itemtype == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 27 || itemtype == 2)
	    sprintf (buf, "You allow your wielder to see invisible things.\n\r");
	else if (itemtype == 39 || itemtype == 3)
	    sprintf (buf, "You grant your wielder the power of flight.\n\r");
	else if (itemtype == 45 || itemtype == 4)
	    sprintf (buf, "You allow your wielder to see in the dark.\n\r");
	else if (itemtype == 46 || itemtype == 5)
	    sprintf (buf, "You render your wielder invisible to the human eye.\n\r");
	else if (itemtype == 52 || itemtype == 6)
	    sprintf (buf, "You allow your wielder to walk through solid doors.\n\r");
	else if (itemtype == 54 || itemtype == 7)
	    sprintf (buf, "You protect your wielder from evil.\n\r");
	else if (itemtype == 57 || itemtype == 8)
	    sprintf (buf, "You protect your wielder in combat.\n\r");
	else if (itemtype == 9)
	    sprintf (buf, "You allow your wielder to walk in complete silence.\n\r");
	else if (itemtype == 10)
	    sprintf (buf, "You surround your wielder with a shield of lightning.\n\r");
	else if (itemtype == 11)
	    sprintf (buf, "You surround your wielder with a shield of fire.\n\r");
	else if (itemtype == 12)
	    sprintf (buf, "You surround your wielder with a shield of ice.\n\r");
	else if (itemtype == 13)
	    sprintf (buf, "You surround your wielder with a shield of acid.\n\r");
	else if (itemtype == 14)
	    sprintf (buf, "You channel the power of god into your wielder.\n\r");
	else if (itemtype == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (itemtype == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (itemtype == 17)
	    sprintf (buf, "You enable your wielder to move at supernatural speed.\n\r");
	else if (itemtype == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (itemtype == 19)
	    sprintf (buf, "You protect your wielder from player attacks.\n\r");
	else if (itemtype == 20)
	    sprintf (buf, "You surround your wielder with a shield of darkness.\n\r");
	else if (itemtype == 21)
	    sprintf (buf, "You grant your wielder superior protection.\n\r");
	else if (itemtype == 22)
	    sprintf (buf, "You grant your wielder supernatural vision.\n\r");
	else if (itemtype == 23)
	    sprintf (buf, "You make your wielder fleet-footed.\n\r");
	else if (itemtype == 24)
	    sprintf (buf, "You conceal your wielder from sight.\n\r");
	else if (itemtype == 25)
	    sprintf (buf, "You invoke the power of your wielders beast.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (itemtype > 0)
	    send_to_char( buf, ch );
	break;

    case ITEM_ARMOR:
	sprintf( buf, "Your armor class is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	if (obj->value[3] < 1)
	    break;
	if (obj->value[3] == 4 || obj->value[3] == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (obj->value[3] == 27 || obj->value[3] == 2)
	    sprintf (buf, "You allow your wearer to see invisible things.\n\r");
	else if (obj->value[3] == 39 || obj->value[3] == 3)
	    sprintf (buf, "You grant your wearer the power of flight.\n\r");
	else if (obj->value[3] == 45 || obj->value[3] == 4)
	    sprintf (buf, "You allow your wearer to see in the dark.\n\r");
	else if (obj->value[3] == 46 || obj->value[3] == 5)
	    sprintf (buf, "You render your wearer invisible to the human eye.\n\r");
	else if (obj->value[3] == 52 || obj->value[3] == 6)
	    sprintf (buf, "You allow your wearer to walk through solid doors.\n\r");
	else if (obj->value[3] == 54 || obj->value[3] == 7)
	    sprintf (buf, "You protect your wearer from evil.\n\r");
	else if (obj->value[3] == 57 || obj->value[3] == 8)
	    sprintf (buf, "You protect your wearer in combat.\n\r");
	else if (obj->value[3] == 9)
	    sprintf (buf, "You allow your wearer to walk in complete silence.\n\r");
	else if (obj->value[3] == 10)
	    sprintf (buf, "You surround your wearer with a shield of lightning.\n\r");
	else if (obj->value[3] == 11)
	    sprintf (buf, "You surround your wearer with a shield of fire.\n\r");
	else if (obj->value[3] == 12)
	    sprintf (buf, "You surround your wearer with a shield of ice.\n\r");
	else if (obj->value[3] == 13)
	    sprintf (buf, "You surround your wearer with a shield of acid.\n\r");
	else if (obj->value[3] == 14)
	    sprintf (buf, "You protect your wearer from attacks from DarkBlade clan guardians.\n\r");
	else if (obj->value[3] == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (obj->value[3] == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (obj->value[3] == 17)
	    sprintf (buf, "You enable your wearer to move at supernatural speed.\n\r");
	else if (obj->value[3] == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (obj->value[3] == 19)
	    sprintf (buf, "You protect your wearer from player attacks.\n\r");
	else if (obj->value[3] == 20)
	    sprintf (buf, "You surround your wearer with a shield of darkness.\n\r");
	else if (obj->value[3] == 21)
	    sprintf (buf, "You grant your wearer superior protection.\n\r");
	else if (obj->value[3] == 22)
	    sprintf (buf, "You grant your wearer supernatural vision.\n\r");
	else if (obj->value[3] == 23)
	    sprintf (buf, "You make your wearer fleet-footed.\n\r");
	else if (obj->value[3] == 24)
	    sprintf (buf, "You conceal your wearer from sight.\n\r");
	else if (obj->value[3] == 25)
	    sprintf (buf, "You invoke the power of your wearers beast.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (obj->value[3] > 0)
	    send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->pIndexData->first_affect; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    for ( paf = obj->first_affect; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }
    return;
}

/* Do_prompt from Morgenes from Aldara Mud */
void do_prompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "prompt" );
       return;
   }

   if( !strcmp( argument, "on" ) )
   {
      if (IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt on!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         xSET_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "off" ) )
   {
      if (!IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt off!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         xREMOVE_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->prompt);
      ch->prompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 50 )
	  argument[50] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 

/* Do_prompt from Morgenes from Aldara Mud */
void do_cprompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "cprompt" );
       return;
   }

   if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->cprompt);
      ch->cprompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 50 )
	  argument[50] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->cprompt );
   ch->cprompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 

void do_finger( CHAR_DATA *ch, char *argument )
{

    char arg[MAX_STRING_LENGTH];
    char strsave[MAX_INPUT_LENGTH];
    char *buf;
    char *addy;
    char buf2[MAX_INPUT_LENGTH];
    FILE *fp;
    int  num;
    int  num2;
    int  class;
    EXT_BV  extra;

    if ( IS_NPC(ch) ) return;
    one_argument(argument,arg);

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( !char_exists(TRUE,argument) )
    {
	send_to_char( "Cannot find a player with that name.\n\r", ch );
	return;
    }
    fclose( fpReserve );
    sprintf( strsave, "%sbackup/%s", PLAYER_DIR, capitalize(argument) );
    if((fp=fopen(strsave,"r"))!=NULL)
    {
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
	buf=fread_string( fp );
	send_to_char(buf,ch);
	send_to_char(" ",ch);
	buf=fread_string( fp );
	send_to_char(buf,ch);
	send_to_char(".\n\r",ch);
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
	send_to_char("Last connected ",ch);
        addy=fread_string( fp );
        if (IS_IMPLEMENTOR(ch))
        {
           send_to_char( "from ", ch);
           stc( addy, ch);
           send_to_char(" ",ch);
        }
	send_to_char("at ",ch);
	buf=fread_string( fp );
	send_to_char(buf,ch);
	send_to_char(".\n\r",ch);
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
	extra=fread_bitvector( fp );
	num=fread_number( fp );
        send_to_char("Sex: ",ch);
	buf=fread_string( fp );
	if (num == SEX_MALE) send_to_char("Male. ",ch);
	else if (num == SEX_FEMALE) 
	{
	    send_to_char("Female. ",ch);
	    if (xIS_SET(extra, EXTRA_PREGNANT)) other_age(ch,extra,TRUE,buf);
	}
	else send_to_char("None. ",ch);
	buf=fread_string( fp );
	other_age(ch,extra,FALSE,buf);
	num=fread_number( fp );
	num2=fread_number( fp );
	switch ( num )
	{
	    default:
		sprintf(buf2,"They are mortal with #C%d#n status,",num2);
		break;
	    case LEVEL_AVATAR:
	    case LEVEL_APPRENTICE:
	    case LEVEL_MAGE:
	    case LEVEL_ARCHMAGE:
	    {
		if (num2 < 1)
                    sprintf(buf2,"They are a Piss Ant with #C%d#n status, ",num2);
		else if (num2 < 4)
                    sprintf(buf2,"They are a Squire with #C%d#n status, ",num2);
		else if (num2 < 7)
                    sprintf(buf2,"They are an Apprentice with #C%d#n status, ",num2);
		else if (num2 < 10)
                    sprintf(buf2,"They are a Scout with #C%d#n status, ",num2);
		else if (num2 < 13)
                    sprintf(buf2,"They are a Soldier with #C%d#n status, ",num2);
		else if (num2 < 16)
                    sprintf(buf2,"They are a Lieutenant with #C%d#n status, ",num2);
                else if (num2 < 19)
                    sprintf(buf2,"They are a Captain with #C%d#n status, ",num2);
                else if (num2 < 22)
                    sprintf(buf2,"They are a Guard with #C%d#n status, ",num2);
		else if (num2 < 25)
                    sprintf(buf2,"They are a Knight with #C%d#n status, ",num2);
  		else if (num2 < 28)
                    sprintf(buf2,"They are a Ranger with #C%d#n status, ",num2);
  		else if (num2 < 31)
                    sprintf(buf2,"They are a Hunter with #C%d#n status, ",num2);
		else if (num2 < 34)
                    sprintf(buf2,"They are an Assassin with #C%d#n status, ",num2);
		else if (num2 < 37)
                    sprintf(buf2,"They are a Lord with #C%d#n status, ",num2);
		else if (num2 < 40)
                    sprintf(buf2,"They are a Baron with #C%d#n status, ",num2);
		else if (num2 < 43)
                    sprintf(buf2,"They are a Duke with #C%d#n status, ",num2);
		else if (num2 < 46)
                    sprintf(buf2,"They are a Prince with #C%d#n status, ",num2);
                else if (num2 < 51)
                    sprintf(buf2,"They are a King with #C%d#n status, ",num2);
                else if (num2 < 76)
                    sprintf(buf2,"They are an Emperor with #C%d#n status, ",num2);
                else if (num2 < 101)
                    sprintf(buf2,"They are an Overlord with #C%d#n status, ",num2);
                else if (num2 < 126)
                    sprintf(buf2,"They are a Demigod with #C%d#n status, ",num2);
                else if (num2 < 151)
                    sprintf(buf2,"They are a Lesser God with #C%d#n status, ",num2);
                else if (num2 < 198) 
                    sprintf(buf2,"They are a Mighty God with #C%d#n status, ",num2);
                else if (num2 >= 199)
                    sprintf(buf2,"They are the Reaper with #C%d#n status, ",num2);
		break;
	    }
	    case LEVEL_BUILDER:
		sprintf(buf2,"They are a Builder with #C%d#n status, ",num2);
		break;
	    case LEVEL_QUESTMAKER:
		sprintf(buf2,"They are a Quest Maker with #c%d#n status, ",num2);
		break;
	    case LEVEL_ENFORCER:
		sprintf(buf2,"They are an Enforcer with #C%d#n status, ",num2);
		break;
	    case LEVEL_JUDGE:
		sprintf(buf2,"They are a Judge with #C%d#n status, ",num2);
		break;
	    case LEVEL_HIGHJUDGE:
		sprintf(buf2,"They are a High Judge with #C%d#n status, ",num2);
		break;
	    case LEVEL_IMPLEMENTOR:
		if (!str_cmp(arg,"Rand")) 
		sprintf(buf2, "They are #yT#rh#Re #yD#rr#Rago#yn #yR#re#Rbor#yn#n, "); 
		else if (!str_cmp(arg,"dummy"))
                sprintf(buf2,"They are #0T#Rh#0e #RL#0o#Rr#0d #Rof #0D#Re#0s#Rt#0r#Ru#0c#Rt#0i#Ro#0n#n, ");
		else 	
        	sprintf(buf2,"They are an Implementor with #C%d#n status, ",num2);
		break;
	}
        send_to_char(buf2,ch);
	num=fread_number( fp );
	if (num > 0) num2 = (2*(num / 7200)); else num2 = 0;
	sprintf(buf2,"and have been playing for %d hours.\n\r",num2);
	send_to_char(buf2,ch);
	buf=fread_string( fp );
	if (strlen(buf) > 2)
	{
	    if (xIS_SET(extra, EXTRA_MARRIED))
		sprintf(buf2,"They are married to %s.\n\r",buf);
	    else
		sprintf(buf2,"They are engaged to %s.\n\r",buf);
	    send_to_char(buf2,ch);
	}
	num=fread_number( fp );
	num2=fread_number( fp );

	sprintf(buf2,"#RPlayer kills#n: #r%d, #RPlayer Deaths#n: #r%d.#n\n\r",num,num2);
	send_to_char(buf2,ch);
	num=fread_number( fp );
	num2=fread_number( fp );
	sprintf(buf2,"#yArena kills#n: %d, #yArena Deaths#n: %d.#n\n\r",num,num2);
        send_to_char(buf2,ch);
        num=fread_number(fp);
        num2=fread_number(fp);
        sprintf(buf2,"#PMob Kills:#n #p%d,#P Mob Deaths#n: #p%d.#n\n\r",num,num2);
	send_to_char(buf2,ch);
	num=fread_number(fp);
	num2=fread_number(fp);
	class = num2;
	switch (num2)
        {
	case CLASS_DEMON:
	 sprintf(buf2, "#GClass#n: Demon, #CGeneration#n: %d.\n\r",num);	
	 break;
        case CLASS_WEREWOLF:
	 sprintf(buf2, "#GClass#n: Werewolf, #CGeneration#n: %d.\n\r",num);
	 break;
        case CLASS_DROW:
         sprintf(buf2, "#GClass#n: Drow, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_NINJA:
         sprintf(buf2, "#GClass#n: Ninja, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_VAMPIRE:
         sprintf(buf2, "#GClass#n: Vampire, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_MONK:
         sprintf(buf2, "#GClass#n: Monk, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_MAGE:
         sprintf(buf2, "#GClass#n: Battlemage, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_TANARRI:
         sprintf(buf2, "#GClass#n: Tanarri, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_SHAPESHIFTER:
         sprintf(buf2, "#GClass#n: Shapeshifter, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_CYBORG:
         sprintf(buf2, "#GClass#n: Spider Droid, #CGeneration#n: %d.\n\r",num);
         break;
	case CLASS_PALADIN:
	 sprintf(buf2, "#GClass#n: Paladin, #CGeneration#n: %d.\n\r",num);
	 break;
        case CLASS_HIGHLANDER:
         sprintf(buf2, "#GClass#n: Samurai, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_UNDEAD_KNIGHT:
         sprintf(buf2, "#GClass#n: Undead Knight, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_ANGEL:
         sprintf(buf2, "#GClass#n: Angel, #CGeneration#n: %d.\n\r",num);
         break;
        case CLASS_POWER_LICH:
         sprintf(buf2, "#GClass#n: Lich, #CGeneration#n: %d.\n\r",num);
         break;
	case CLASS_PSIONIST:
	 sprintf(buf2, "#GClass#n: Psionist, #CGeneration#n: %d,\n\r",num);
	 break;
	case CLASS_INQUISITOR:
	 sprintf(buf2, "#GClass#n: Inquisitor, #CGeneration#n: %d\n\r",num);
	 break;
	case CLASS_HUNTER:
	 sprintf(buf2, "#GClass#n: Hunter, #CGeneration#n: %d\n\r",num);
	 break;
	default:
	 sprintf(buf2, "Class: None, Generation: %d.\n\r",num);
	 break;
	}
	stc(buf2,ch);
	num = fread_number(fp);
	sprintf(buf2, "#0Primal Energy#n: %d.\n\r",num);
        stc(buf2,ch);

        num = fread_number(fp);
	switch(num)
	{
	case L_UNK:
         sprintf(buf2, "#CLegend#n: Unknown.\n\r");
         break;
	case L_CIT:
         sprintf(buf2, "#CLegend#n: Zealot.\n\r");
         break;
	case L_LEG:
	 sprintf(buf2, "#CLegend#n: Champion.\n\r");
         break;
	case L_MYT:
         sprintf(buf2, "#CLegend#n: Hero.\n\r");
         break;
	case L_TIT:
         sprintf(buf2, "#CLegend#n: Martyr.\n\r");
         break;
	case L_ORA:
         sprintf(buf2, "#CLegend#n: Guardian.\n\r");
         break;
	case L_IMM:
         sprintf(buf2, "#CLegend#n: Saint.\n\r");
         break;
	case L_DEM:
         sprintf(buf2, "#CLegend#n: Savior.\n\r");
         break;
	case L_DEI:
         sprintf(buf2, "#CLegend#n: Myth.\n\r");
         break;
	case L_ALM:
         sprintf(buf2, "#CLegend#n: Legend.\n\r");
         break;
	 }
	stc(buf2,ch);
	num = fread_number(fp);
	sprintf(buf2, "#LBounty#n: #y%d qp.#n\n\r",num);
        stc(buf2,ch);
	num = fread_number(fp);
	sprintf(buf2, "#GPower Rating#n: #0%d#n.\n\r",num);
	stc(buf2,ch);
	send_to_char("----------------------------------------------------------------------------\n\r",ch);
    }
   else
    {
	bug( "Do_finger: fopen", 0 );
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}
/* Find non-existant help files. -Sadiq */
void do_nohelps(CHAR_DATA *ch, char *argument)
{
  AREA_DATA *tArea;
  char arg[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int col=0, cmd, i;

   argument = one_argument( argument, arg );

   if(!IS_IMMORTAL(ch) || IS_NPC(ch) )
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }


   if ( arg[0] == '\0' || !str_cmp(arg, "all") )
   {
      do_nohelps(ch, "commands");
      send_to_char( "\n\r", ch);
/*      do_nohelps(ch, "skills");
      send_to_char( "\n\r", ch); */
      do_nohelps(ch, "areas");
      send_to_char( "\n\r", ch);
      return;
   }

  if(!str_cmp(arg, "commands") )
  {
    send_to_char("#CCommands for which there are no help files:\n\r\n\r", ch);
        for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
        {
            i = cmd_sort_info[cmd].sort_pos;
            if (!get_help(ch, cmd_table[i].name) )
            {
             sprintf( buf, "%-12s", cmd_table[i].name );
            send_to_char( buf, ch );
//              chprintf(ch, "#w%-15s", cmd_table[i].name);
               if ( ++col % 6 == 0 )
                    send_to_char( "\n\r", ch );
            }
        }

        send_to_char("\n\r", ch);
        return;
   }
/*
   if(!str_cmp(arg, "skills") || !str_cmp(arg, "spells") )
   {
     send_to_char_color("&CSkills/Spells for which there are no help 
files:\n\r\n\r", ch);

     for ( sn = 0; sn < top_sn && skill_table[sn] && 
skill_table[sn]->name; sn++ )
     {
        if(!get_help(ch, skill_table[sn]->name))
        {
           chprintf_color(ch, "&W%-20s", skill_table[sn]->name);
           if ( ++col % 4 == 0 )
             {
                send_to_char("\n\r", ch);
             }
        }
     }

        send_to_char("\n\r", ch);
        return;
   }
*/
   if(!str_cmp(arg, "areas") )
   {
        send_to_char("#gAreas for which there are no help files:\n\r\n\r", 
ch);

        for (tArea = first_area; tArea;tArea = tArea->next)
        {
          if(!get_help(ch, tArea->name) )
           {
              chprintf(ch, "#W%-35s", tArea->name);
              if ( ++col % 2 == 0 )
               {
                send_to_char("\n\r", ch);
               }
           }
        }

           send_to_char( "\n\r", ch);
           return;
    }

        send_to_char("Syntax:  nohelps <all|areas|commands>\n\r", ch);
        return;
}

/*Pscore added by Huw aka Ryan Wilcox*/
void do_pscore( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int a_c = char_ac(ch);

	if (IS_NPC(ch)) return;

if (!IS_NPC(ch) && (IS_EXTRA(ch,EXTRA_OSWITCH) || IS_HEAD(ch,LOST_HEAD)))
	{obj_score(ch,ch->pcdata->chobj);return;}

stc("#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#n\n\r", ch);
sprintf( buf, "#R= #0You are #7%-45s#0 #R=\n\r",
	ch->pcdata->switchname);
stc( buf, ch );

sprintf( buf, "#R= #0Hours: #7%d#0- Level- #7%d #0Trust- #7%-24d #R=\n\r",
	ch->played / 3600,
	ch->level,
	ch->trust);

stc( buf, ch);
stc("#R= = = = = = = = = = = = = = = = = = = = = = = = = = = = =#n\n\r", ch);
sprintf( buf, "#R= #R%d#0/#r%d#0-HP, #L%d/#l%d#0-MP, #P%d#0/#p%-2d#0-MV #R=\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move);
stc( buf, ch );

sprintf( buf, "#R= #0Hitroll: #7%d#0, #0Damroll: #7%d#0, AC: #7%-17d #R=\n\r",
	char_hitroll(ch),
	char_damroll(ch), 
	a_c);
stc( buf, ch );

sprintf( buf, "#R= #0Damcap: #7%d#0, Save-vs-Spell: #7%d#0, Wimpy: #7%-14d #R=\n\r",
	ch->damcap[0],
	ch->saving_throw,
	ch->wimpy);
stc( buf, ch );

stc("#R= = = = = = = = = = = = = = = = = = = = = = = = = = = = =#n\n\r", ch);
sprintf( buf, "#R= #0Status: #7%d#0, Legend: #7%d#0, Gold: #7%2.0f #R=\n\r",
	ch->race,
	ch->lstatus,
	ch->gold);
stc( buf, ch );


sprintf( buf, "#R= #0Quest Points: #7%d#0, Generation: #7%-19d #R=\n\r",
	ch->pcdata->quest,
	ch->generation);
stc( buf, ch );

stc("#R= = = = = = = = = = = = = = = = = = = = = = = = = = = = =#n\n\r", ch);

	if (ch->fight_timer > 0) 
	{
	sprintf(buf,"#R=#0You have #L%d#0 rounds left on your fight timer#R=#n\n\r", 
	ch->fight_timer);
	stc(buf,ch);
	}
      if (ch->wish_timer > 0)
	{
      sprintf(buf,"#R=#0You have #L%d#0 rounds left on your wish timer#R=#n\n\r",
	ch->wish_timer);
      stc(buf,ch);
      }
	if (ch->fury_timer > 0) 
	{
      sprintf(buf,"#R=#0You have #L%d#0 rounds left of unholy fury#R=#n\n\r",
	ch->fury_timer);
      stc(buf,ch);
      }
      if (ch->quit_timer > 0) 
	{
      sprintf(buf,"#R=#0You have #L%d#0 rounds left before you can quit#R=#n\n\r",
	ch->quit_timer);
      stc(buf,ch);
      }
   if (ch->bomb_timer > 0)
   {
      if (IS_CLASS(ch, CLASS_ANGEL))
      {
	sprintf(buf,"#R=#0You have #G%d#0 rounds left on your divine wrath timer#R=#n\n\r",
	ch->bomb_timer);
	stc(buf,ch);
	}
      if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
      {
	sprintf(buf,"#R=#0You have #G%d#0 rounds left on your faerie blink timer#R=#n\n\r",
	ch->bomb_timer);
	stc(buf,ch);
	}
      if (IS_CLASS(ch, CLASS_MAGE))
      {
	sprintf(buf,"#R=#0You have #G%d#0 rounds left on your bomb timer#R=#n\n\r",
	ch->bomb_timer);
	stc(buf,ch);
	}
      if (IS_CLASS(ch, CLASS_INQUISITOR))
      {
	sprintf(buf,"#R=#0You have #G%d#0 rounds left on your fury timer#R=#n\n\r",
	ch->bomb_timer);
	stc(buf,ch);	
	}
	if (IS_CLASS(ch, CLASS_PALADIN))
	{
	sprintf(buf,"#R=#0You have #G%d#0 rounds left on your bladeslash timer#R=#n\n\r",
	ch->bomb_timer);
	stc(buf,ch);
	}
	if (IS_CLASS(ch, CLASS_HUNTER))
	{
	sprintf(buf,"#R=#0You have #G%d#0 rounds left until you can ensnare again#R=#n\n\r",
	ch->bomb_timer);
	stc(buf,ch);
	}
   }
stc("#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#r-#R=#n\n\r", ch);
return;
}

