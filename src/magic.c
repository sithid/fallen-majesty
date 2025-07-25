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



/*
 * Local functions.
 */
void	say_spell	args( ( CHAR_DATA *ch, int sn ) );
void	improve_spl	args( ( CHAR_DATA *ch, int dtype, int sn ) );




/*
 * Improve ability at a certain spell type.  KaVir.
 */
 

void improve_spl( CHAR_DATA *ch, int dtype, int sn )
{
    char buf[MAX_INPUT_LENGTH];
    char bufskill[MAX_INPUT_LENGTH];
    char buftype[MAX_INPUT_LENGTH];
    int dice1;
    int dice2;

    dice1 = number_percent();
    dice2 = number_percent();


         if (dtype == 0 ) sprintf(buftype,"purple");
    else if (dtype == 1 ) sprintf(buftype,"red");
    else if (dtype == 2 ) sprintf(buftype,"blue");
    else if (dtype == 3 ) sprintf(buftype,"green");
    else if (dtype == 4 ) sprintf(buftype,"yellow");
    else if (dtype == 5 ) sprintf(buftype,"orange");
    else if (dtype == 6 ) sprintf(buftype,"indigo");
    else if (dtype == 7 ) sprintf(buftype,"violet");
    else return;
    if (IS_CLASS(ch, CLASS_PSIONIST) && IS_SET(ch->pcdata->powers[PSIONIST_POWER], PSIONIST_MENTAL))
	{
	if (ch->spl[dtype] >= 1000) return;
	}
    else if (IS_CLASS(ch, CLASS_MAGE))
	{
        if (ch->spl[dtype] >= 950) return;
	}
    else if (IS_CLASS(ch, CLASS_POWER_LICH))
	{
        if (ch->spl[dtype] >= 450) return;
	}
    else 
	{
	      if (ch->spl[dtype] >= 200) return;
 	}


   if (
	(dice1 > 50)
      ) 
	ch->spl[dtype] += 1;
    else return;

         if (ch->spl[dtype] == 1  ) sprintf(bufskill,"an apprentice of");
    else if (ch->spl[dtype] == 26 ) sprintf(bufskill,"a student at");
    else if (ch->spl[dtype] == 51 ) sprintf(bufskill,"a scholar at");
    else if (ch->spl[dtype] == 76 ) sprintf(bufskill,"a magus at");
    else if (ch->spl[dtype] == 101) sprintf(bufskill,"an adept at");
    else if (ch->spl[dtype] == 126) sprintf(bufskill,"a mage at");
    else if (ch->spl[dtype] == 151) sprintf(bufskill,"a warlock at");
    else if (ch->spl[dtype] == 176) sprintf(bufskill,"a master wizard at");
    else if (ch->spl[dtype] == 200) sprintf(bufskill,"a grand sorcerer at");
    else if (ch->spl[dtype] == 300) sprintf(bufskill,"the complete master of");
    else if (ch->spl[dtype] == 500) sprintf(bufskill,"possesing lich knowledge of");
    else if (ch->spl[dtype] == 1000) sprintf(bufskill,"a true Psionist at");
    else return;
    sprintf(buf,"#GYou are now %s %s magic.\n\r#n",bufskill,buftype);
    send_to_char(buf,ch);
    return;
}

bool is_obj( CHAR_DATA *ch )
{
    OBJ_DATA *obj;

    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("Huh?\n\r",ch);
	return FALSE;
    }
    if ( obj->chobj == NULL || obj->chobj != ch )
    {
	send_to_char("Huh?\n\r",ch);
	return FALSE;
    }
    if (!IS_CLASS(ch,CLASS_DEMON) && !IS_SET(ch->special,SPC_CHAMPION))
    {
	send_to_char("Huh?\n\r",ch);
	return FALSE;
    }
    if (!IS_DEMPOWER(ch, DEM_MAGIC))
    {
	send_to_char("You haven't been granted the gift of Magic.\n\r",ch);
	return FALSE;
    }
    return TRUE;
}

/*
 * Lookup a skill by name.
 */
int skill_lookup( const char *name )
{
    int sn;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&   !str_prefix( name, skill_table[sn].name ) )
	    return sn;
    }

    return -1;
}



/*
 * Lookup a skill by slot number.
 * Used for object loading.
 */
int slot_lookup( int slot )
{
    extern bool fBootDb;
    int sn;

    if ( slot <= 0 )
	return -1;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( slot == skill_table[sn].slot )
	    return sn;
    }

    if ( fBootDb )
    {
	bug( "Slot_lookup: bad slot %d.", slot );
	abort( );
    }

    return -1;
}



/*
 * Utter mystical words for an sn.
 */
void say_spell( CHAR_DATA *ch, int sn )
{
    char buf  [MAX_STRING_LENGTH];
    char buf2 [MAX_STRING_LENGTH];
    char colour [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct syl_type
    {
	char *	old;
	char *	new;
    };

    static const struct syl_type syl_table[] =
    {
	{ " ",		" "		},
	{ "ar",		"abra"		},
	{ "au",		"kada"		},
	{ "bless",	"fido"		},
	{ "blind",	"nose"		},
	{ "bur",	"mosa"		},
	{ "cu",		"judi"		},
	{ "de",		"oculo"		},
	{ "en",		"unso"		},
	{ "light",	"dies"		},
	{ "lo",		"hi"		},
	{ "mor",	"zak"		},
	{ "move",	"sido"		},
	{ "ness",	"lacri"		},
	{ "ning",	"illa"		},
	{ "per",	"duda"		},
	{ "ra",		"gru"		},
	{ "re",		"candus"	},
	{ "son",	"sabru"		},
	{ "tect",	"infra"		},
	{ "tri",	"cula"		},
	{ "ven",	"nofo"		},
	{ "a", "c" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
	{ "e", "z" }, { "f", "m" }, { "g", "o" }, { "h", "p" },
	{ "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
	{ "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
	{ "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
	{ "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
	{ "y", "l" }, { "z", "k" },
	{ "", "" }
    };

    buf[0]	= '\0';
    for ( pName = skill_table[sn].name; *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( syl_table[iSyl].old, pName ) )
	    {
		strcat( buf, syl_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    sprintf( buf2, "$n utters the words, '%s'.", buf );
    sprintf( buf,  "$n utters the words, '%s'.", skill_table[sn].name );
    if ( skill_table[sn].target == 0 )
    {
    	sprintf(colour,"$n's eyes glow bright purple for a moment.");
    	ADD_COLOUR(ch,colour,MAGENTA);
    	act(colour,ch,NULL,NULL,TO_ROOM, FALSE);
    	sprintf(colour,"Your eyes glow bright purple for a moment.");
    	ADD_COLOUR(ch,colour,MAGENTA);
    	act(colour,ch,NULL,NULL,TO_CHAR, FALSE);
    }
    else if ( skill_table[sn].target == 1 )
    {
    	sprintf(colour,"$n's eyes glow bright red for a moment.");
    	ADD_COLOUR(ch,colour,L_RED);
    	act(colour,ch,NULL,NULL,TO_ROOM, FALSE);
    	sprintf(colour,"Your eyes glow bright red for a moment.");
    	ADD_COLOUR(ch,colour,L_RED);
    	act(colour,ch,NULL,NULL,TO_CHAR, FALSE);
    }
    else if ( skill_table[sn].target == 2 )
    {
    	sprintf(colour,"$n's eyes glow bright blue for a moment.");
    	ADD_COLOUR(ch,colour,L_BLUE);
    	act(colour,ch,NULL,NULL,TO_ROOM, FALSE);
    	sprintf(colour,"Your eyes glow bright blue for a moment.");
    	ADD_COLOUR(ch,colour,L_BLUE);
    	act(colour,ch,NULL,NULL,TO_CHAR, FALSE);
    }
    else if ( skill_table[sn].target == 3 )
    {
    	sprintf(colour,"$n's eyes glow bright green for a moment.");
    	ADD_COLOUR(ch,colour,L_GREEN);
    	act(colour,ch,NULL,NULL,TO_ROOM, FALSE);
    	sprintf(colour,"Your eyes glow bright green for a moment.");
    	ADD_COLOUR(ch,colour,L_GREEN);
    	act(colour,ch,NULL,NULL,TO_CHAR, FALSE);
    }
    else if ( skill_table[sn].target == 4 )
    {
    	sprintf(colour,"$n's eyes glow bright yellow for a moment.");
    	ADD_COLOUR(ch,colour,YELLOW);
    	act(colour,ch,NULL,NULL,TO_ROOM, FALSE);
    	sprintf(colour,"Your eyes glow bright yellow for a moment.");
    	ADD_COLOUR(ch,colour,YELLOW);
    	act(colour,ch,NULL,NULL,TO_CHAR, FALSE);
    }
    return;
}



/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell( int level, CHAR_DATA *victim )
{
    int save;
    int tsave;

    if (!IS_NPC(victim))
    {
	tsave = ( victim->spl[0]+victim->spl[1]+victim->spl[2]+
		  victim->spl[3]+victim->spl[4] ) * 0.05;
    	save = 50 + ( tsave - level - victim->saving_throw ) * 5;
    }
    else
    	save = 50 + ( victim->level - level - victim->saving_throw ) * 5;
    save = URANGE( 15, save, 85 );
    return number_percent( ) < save;
}



/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast( CHAR_DATA *ch, char *argument )
{
    char buf[MSL];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    int dice = number_percent();
    /*
     * Switched NPC's can cast spells, but others can't.
     */
    if ( IS_NPC(ch) && ch->desc == NULL )
	return;

	// Polymorphed players cannot cast spells
    if ( !IS_NPC(ch) && IS_AFFECTED(ch, AFF_POLYMORPH) && !IS_CLASS(ch, CLASS_ANGEL) &&
	!IS_VAMPAFF(ch,VAM_DISGUISED) && !IS_POLYAFF(ch, POLY_SPIDER) && ch->power[DISC_VAMP_OBEA] < 10
	&& !IS_SET(ch->flag2, VAMP_OBJMASK) && !IS_CLASS(ch,CLASS_DEMON) && !IS_CLASS(ch, CLASS_MAGE) && 
	!IS_CLASS(ch, CLASS_CYBORG) && !(IS_CLASS(ch, CLASS_SHAPESHIFTER) && ch->pcdata->powers[FAERIE_LEVEL]>1))
    {
	if ( !is_obj(ch) )
	{
	    send_to_char( "You cannot cast spells in this form.\n\r", ch );
	    return;
	}
    }

    if (IS_CLASS(ch,CLASS_VAMPIRE) && IS_AFFECTED(ch, AFF_PEACE))
    {
	stc("You are unable to cast spells with Majesty on.\n\r",ch);
	return;
    }

    if (IS_CLASS(ch,CLASS_VAMPIRE) && IS_AFFECTED(ch, AFF_POLYMORPH) && ch->power[DISC_VAMP_OBEA] > 9)
    {
        stc("#7Your movement of pure magic guides you#n.\n\r",ch);
    }

    if (form_data[ch->cur_form].can_cast == FALSE && ch->cur_form != 0)
    {
	stc("You cannot cast spells in this form.\n\r",ch);
	return;
    }

    if ( IS_ITEMAFF(ch, ITEMA_REFLECT))
    {
	send_to_char( "You lower your reflection.\n\r", ch );
    }

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Cast which what where?\n\r", ch );
	return;
    }

    if ( ( sn = skill_lookup( arg1 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }
    if ( skill_table[sn].spell_fun == spell_null)
    {
	stc("You cannot cast a skill.\n\r",ch);
	return;
    }  
    if ( ch->position < skill_table[sn].minimum_position )
    {
	if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
	    IS_VAMPAFF(ch, VAM_CELERITY))
	{
	    if ( ch->move < 25 )
	    {
		send_to_char( "You can't concentrate enough.\n\r", ch );
		return;
	    }
	    ch->move = ch->move - 25;
	}
	else
	{
	    if ( ch->move < 50 )
	    {
		send_to_char( "You can't concentrate enough.\n\r", ch );
		return;
	    }
	    ch->move = ch->move - 50;
	}
    }

    mana = IS_NPC(ch) ? 0 : UMAX(
	skill_table[sn].min_mana,
	100 / ( 2 + (ch->level*12) - skill_table[sn].skill_level) );


    if ( !IS_NPC(ch) && IS_SET(ch->special, SPC_WOLFMAN) )
    {
	if (ch->power[DISC_WERE_OWL] < 4) mana *= 2;
    }

    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
      
    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_cast: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Cast the spell on whom?\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}
    if (IS_AFFECTED(victim, AFF_PEACE))
    {
        stc("You are unable to cast on that person.\n\r",ch);
        return;
    }    


        if (is_safe(ch,victim) && ch!=victim) return;
	if ( ch == victim )
	    send_to_char( "Cast this on yourself? Ok...\n\r", ch );

	if ( IS_ITEMAFF(victim, ITEMA_REFLECT) && victim != ch && number_range(1,3) == 1)
	{
	    send_to_char( "You are unable to focus your spell upon them.\n\r", ch );
	    return;
	}

	if (!IS_NPC(victim) && (!CAN_PK(ch)||!CAN_PK(victim)) && (ch != victim))
	{
	    send_to_char( "You are unable to affect them.\n\r", ch );
	    return;
	}

        if (!IS_NPC(victim) && IS_CLASS( victim, CLASS_DROW ))
        {
            if (ch == victim )
            {
                send_to_char("You lower your magical resistance....\n\r", ch);
            }
            else if (number_percent ( )  <= victim->pcdata->stats[DROW_MAGIC])
            {
               send_to_char("Your spell does not affect them.\n\r", ch);
               return;
            }  
        }

	if ( !IS_NPC(ch) )
	{
	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can't do that on your own follower.\n\r",
		    ch );
		return;
	    }
	}

	vo = (void *) victim;
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	if ( IS_ITEMAFF(victim, ITEMA_REFLECT) && ch != victim && number_range(1,3) == 1)
	{
	    send_to_char( "You are unable to focus your spell upon them.\n\r", ch );
	    return;
	}

        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW ))
        {
            if (ch == victim )
            {
                send_to_char("You lower your magical resistance....\n\r", ch);
            }
            else if (number_percent( ) <=victim->pcdata->stats[DROW_MAGIC])
            {
               send_to_char("Your spell does not affect them.\n\r", ch);
               return;
            }  
        }

	vo = (void *) victim;
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( arg2, ch->name ) )
	{
	    send_to_char( "You cannot cast this spell on another.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What should the spell be cast upon?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	break;
    }
	    
    if ( !IS_NPC(ch) && ch->mana < mana )
    {
	send_to_char( "You don't have enough mana.\n\r", ch );
	return;
    }
    
      
    if ( str_cmp( skill_table[sn].name, "ventriloquate" ) )
    	say_spell( ch, sn );
    if (IS_CLASS(ch, CLASS_POWER_LICH))
    {
      WAIT_STATE( ch, skill_table[sn].beats / 2);
    }

    if (IS_CLASS(ch, CLASS_MAGE))
    {
      WAIT_STATE( ch, skill_table[sn].beats / 5);
    }
    if (IS_CLASS(ch, CLASS_PSIONIST))
    {
      WAIT_STATE( ch, skill_table[sn].beats / 2);
    }
    else
    {
      WAIT_STATE( ch, skill_table[sn].beats);
    }

    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[sn] )
    {
	send_to_char( "You lost your concentration.\n\r", ch );
	ch->mana -= mana / 2;
	improve_spl(ch,skill_table[sn].target,sn);
        if (dice > 90)
	{
          ch->pcdata->learned[sn] += 1;
	  sprintf(buf, "You learn from your mistakes, and your %s spell improves!\n\r",
skill_table[sn].name);
	  stc(buf,ch);
	}
    }
    else
    {
	int tempentro = 0;
	if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY))
	tempentro = 50;

	ch->mana -= mana;
	// Check players ability at spell type for spell power
	if (IS_NPC(ch))
	    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo );
	else if ( !IS_CLASS(ch, CLASS_MAGE) )
	{
	    (*skill_table[sn].spell_fun) ( sn, ((ch->spl[skill_table[sn].target]*0.25)+tempentro), ch, vo );
	    improve_spl(ch,skill_table[sn].target,sn);
	}
	else
	{
	  (*skill_table[sn].spell_fun) ( sn, (ch->spl[skill_table[sn].target]*.5+tempentro),ch, vo );
	  improve_spl(ch,skill_table[sn].target,sn);
	 
	}
     }

    if ( skill_table[sn].target == TAR_CHAR_OFFENSIVE
    &&   victim != ch
    &&   victim->master != ch )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;
	    }
	}
    }

    return;
}



/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
    void *vo;

    if ( sn <= 0 )
	return;

	if (IS_NPC(ch)) return;

    if ( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
    {
	bug( "Obj_cast_spell: bad sn %d.", sn );
	return;
    }

    switch ( skill_table[sn].target )
    {
    default:
	bug( "Obj_cast_spell: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	vo = NULL;
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( victim == NULL )
	    victim = ch->fighting;
	if ( victim == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
        if (is_safe(ch,victim) && ch!=victim) return;
	vo = (void *) victim;
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( victim == NULL )
	    victim = ch;
	vo = (void *) victim;
	break;

    case TAR_CHAR_SELF:
	vo = (void *) ch;
	break;

    case TAR_OBJ_INV:
	if ( obj == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
	vo = (void *) obj;
	break;
    }

    target_name = "";
    (*skill_table[sn].spell_fun) ( sn, level, ch, vo );

    if ( skill_table[sn].target == TAR_CHAR_OFFENSIVE
    &&   victim != ch
    &&   victim->master != ch )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;
	    }
	}
    }

    return;
}



/*
 * Spell functions.
 */
void spell_acid_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD)) return;
    dam = dice( level, 6 );
    if (IS_CLASS(ch, CLASS_POWER_LICH)) dam*=4;
    if ( saves_spell( level, victim ) ) dam /= 2;
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_ACID) ) dam = 0;
    if (IS_NPC(victim) && dam >= victim->hit) dam = victim->hit-1;
    if (dam==0) return;
    damage( ch, victim, dam, sn );
    return;
}


void spell_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;
    af.type      = sn;
    af.duration  = 44;
    af.modifier  = -60;
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is shrouded in a suit of translucent glowing armor.", victim, NULL, NULL, 
TO_ROOM, FALSE );
    send_to_char( "You are shrouded in a suit of translucent glowing armor.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_godbless( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    if (IS_CLASS(ch, CLASS_MONK))
    {
    if ( is_affected( victim, sn ) ) return;
    af.type      = sn;
    af.duration  = 300;
    af.location  = APPLY_HITROLL;
    af.modifier  = 300;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - 300;
    affect_to_char( victim, &af );

    af.location  = APPLY_DAMROLL;
    af.modifier  = 300;
    affect_to_char( victim, &af );
    }
    if (!IS_CLASS(ch, CLASS_MONK))
    {
    if ( is_affected( victim, sn ) ) return;
    af.type      = sn;
    af.duration  = 150;
    af.location  = APPLY_HITROLL;
    af.modifier  = 150;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - 150;
    affect_to_char( victim, &af );

    af.location  = APPLY_DAMROLL;
    af.modifier  = 150;
    affect_to_char( victim, &af );
    }

    act( "$n is filled with God's Blessing.", victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char( "You are filled with God's Blessing.\n\r", victim );
    return;
}
void spell_bless( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    if (!IS_CLASS(ch, CLASS_MAGE) || !IS_CLASS(ch, CLASS_POWER_LICH))
    {
    if ( is_affected( victim, sn ) ) return;
    af.type      = sn;
    af.duration  = 10+level;
    af.location  = APPLY_HITROLL;
    af.modifier  = level/2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 1 - level / 6;
    affect_to_char( victim, &af );
    }
    else if (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_POWER_LICH))
    {
    if ( is_affected( victim, sn ) ) return;
    af.type      = sn;
    af.duration  = level * 2;
    af.location  = APPLY_HITROLL;
    af.modifier  = level / 3;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 1 - level / 3;
    affect_to_char( victim, &af );
    }
    act( "$n is blessed.", victim, NULL, NULL, TO_ROOM , FALSE);
    send_to_char( "You feel righteous.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    char buf [MAX_INPUT_LENGTH];

    if ( IS_AFFECTED(victim, AFF_BLIND) || saves_spell( level, victim) )
	return;
    if (IS_NPC(victim)) return;
    if (!IS_NPC(victim))
    {
    af.type      = sn;
    af.location  = APPLY_HITROLL;
    af.modifier  = -40;
    af.duration  = 1+level;
    af.bitvector = AFF_BLIND;
    affect_to_char( victim, &af );
    send_to_char( "You are blinded!\n\r", victim );
    if (!IS_NPC(victim))
	sprintf(buf,"%s is blinded!\n\r",victim->name);
    else
	sprintf(buf,"%s is blinded!\n\r",victim->short_descr);
    send_to_char(buf,ch);
    return;
    }
  return;
}



void spell_burning_hands( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 4,
	 6,  8, 10, 12,	14,	17, 20, 23, 26, 29,
	29, 29, 30, 30,	31,	31, 32, 32, 33, 33,
	34, 34, 35, 35,	36,	36, 37, 37, 38, 38,
	39, 39, 40, 40,	41,	41, 42, 42, 43, 43,
	44, 44, 45, 45,	46,	46, 47, 47, 48, 48,
	50, 60, 70, 80,	90,	100,125,150,175,200
    };
    int dam;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    hp = victim->hit;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) )
    {
    	damage( ch, victim, (dam*2), sn );
	hp = ((hp - victim->hit)/2) + victim->hit;
    }
    else
    	damage( ch, victim, dam, sn );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_HEAT) )
	victim->hit = hp;
    return;
}



void spell_call_lightning( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    int hp;

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You must be out of doors.\n\r", ch );
	return;
    }

    if ( weather_info.sky < SKY_RAINING )
    {
	send_to_char( "You need bad weather.\n\r", ch );
	return;
    }

    dam = dice(level/2, 8);

    send_to_char( "God's lightning strikes your foes!\n\r", ch );
    act( "$n calls God's lightning to strike $s foes!",
	ch, NULL, NULL, TO_ROOM , FALSE);

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) ) )
	    {
		if (IS_ITEMAFF(vch, ITEMA_SHOCKSHIELD)) continue;
    		if ( saves_spell( level, vch ) )
		    dam /= 2;
    		hp = vch->hit;
    		damage( ch, vch, dam, sn );
    		if (!IS_NPC(vch) && IS_IMMUNE(vch, IMM_LIGHTNING) )
		    vch->hit = hp;
	    }
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area
	&&   IS_OUTSIDE(vch)
	&&   IS_AWAKE(vch) )
	    send_to_char( "Lightning flashes in the sky.\n\r", vch );
    }
    return;
}




void spell_cause_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage( ch, (CHAR_DATA *) vo, dice(1, 8) + level / 3, sn );
    return;
}



void spell_cause_critical( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage( ch, (CHAR_DATA *) vo, dice(3, 8) + level - 6, sn );
    return;
}



void spell_cause_serious( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage( ch, (CHAR_DATA *) vo, dice(2, 8) + level / 2, sn );
    return;
}



void spell_change_sex( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    // Spell is currently disabled
    return;
    return;
}



void spell_charm_person( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim == ch )
    {
	send_to_char( "You like yourself even better!\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_CHARM) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    // Prevent charming of ethereal beings (ghosts, spirits, etc.)
    if ( IS_NPC(victim) && IS_AFFECTED(victim, AFF_ETHEREAL) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if (IS_IMMORTAL( victim ))
    {
	send_to_char( "You cannot cast puny mortal magic on immortals!\n\r",ch);
	return;
    }

    if ( IS_AFFECTED(victim, AFF_CHARM)
    ||   IS_AFFECTED(ch, AFF_CHARM)
    ||   level < victim->level
    ||   saves_spell( level, victim ) )
	return;

    if ( victim->master )
	stop_follower( victim );
    add_follower( victim, ch );
    af.type      = sn;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    act( "Isn't $n just so nice?", ch, NULL, victim, TO_VICT , FALSE);
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_chill_touch( int sn, int level, CHAR_DATA *ch, void *vo )
{
    bool no_dam = FALSE;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 9,
	10, 10, 10, 11, 11,	12, 12, 13, 13, 13,
	14, 14, 14, 15, 15,	15, 16, 16, 16, 17,
	17, 17, 18, 18, 18,	19, 19, 19, 20, 20,
	20, 21, 21, 21, 22,	22, 22, 23, 23, 23,
	24, 24, 24, 25, 25,	25, 26, 26, 26, 27,
	30, 40, 50, 60, 70,	80, 90,100,110,120
    };
    AFFECT_DATA af;
    int dam;
    int hp;

   if ( is_affected( victim, sn ) )  return;

    if (IS_ITEMAFF(victim, ITEMA_ICESHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_COLD)) no_dam = TRUE;
    if ( !no_dam && (!saves_spell( level, victim ) ||
	  IS_NPC(victim) || !IS_CLASS(victim, CLASS_VAMPIRE) ) )
    {
	af.type      = sn;
	af.duration  = 6;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }
    if (IS_CLASS(ch, CLASS_POWER_LICH))
    dam *= 10;
    if (IS_NPC(victim) && dam >= victim->hit) dam = victim->hit-1;
    hp = victim->hit;
    if (dam==0) return;
    damage( ch, victim, dam, sn );
    if (no_dam) victim->hit = hp;
    return;
}



void spell_colour_spray( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	10,
	15, 15, 15, 15, 15,	20, 20, 20, 20, 20,
	30, 35, 40, 45, 50,	55, 55, 55, 56, 57,
	58, 58, 59, 60, 61,	61, 62, 63, 64, 64,
	65, 66, 67, 67, 68,	69, 70, 70, 71, 72,
	73, 73, 74, 75, 76,	76, 77, 78, 79, 79,
	85, 95,110,125,150,	175,200,250,300,350
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2,  dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;

    damage( ch, victim, dam, sn );
    return;
}



void spell_continual_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *light;

    light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
    obj_to_room( light, ch->in_room );
    act( "$n twiddles $s thumbs and $p appears.",   ch, light, NULL, TO_ROOM , FALSE);
    act( "You twiddle your thumbs and $p appears.", ch, light, NULL, TO_CHAR , FALSE);
    return;
}



void spell_control_weather( int sn, int level, CHAR_DATA *ch, void *vo )
{
    if ( !str_cmp( target_name, "better" ) )
	weather_info.change += dice( level / 3, 4 );
    else if ( !str_cmp( target_name, "worse" ) )
	weather_info.change -= dice( level / 3, 4 );
    else
	send_to_char ("Do you want it to get better or worse?\n\r", ch );

    send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_create_food( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *mushroom;

    mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ), 0 );
    mushroom->value[0] = 5 + level;
    obj_to_room( mushroom, ch->in_room );
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_ROOM , FALSE);
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_CHAR , FALSE);
    return;
}



void spell_create_spring( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *spring;

    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE))
	return;
    else
    	spring = create_object( get_obj_index( OBJ_VNUM_SPRING ), 0 );
    spring->timer = level;
    obj_to_room( spring, ch->in_room );
    act( "$p flows from the ground.", ch, spring, NULL, TO_ROOM , FALSE);
    act( "$p flows from the ground.", ch, spring, NULL, TO_CHAR , FALSE);
    return;
}



void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int water;

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "It is unable to hold water.\n\r", ch );
	return;
    }

    if ( obj->value[2] != LIQ_WATER && obj->value[1] != 0 )
    {
	send_to_char( "It contains some other liquid.\n\r", ch );
	return;
    }

    water = UMIN(
		level * (weather_info.sky >= SKY_RAINING ? 4 : 2),
		obj->value[0] - obj->value[1]
		);
  
    if ( water > 0 )
    {
	obj->value[2] = LIQ_WATER;
	obj->value[1] += water;
	if ( !is_name( "water", obj->name ) )
	{
	    char buf[MAX_STRING_LENGTH];

	    sprintf( buf, "%s water", obj->name );
	    free_string( obj->name );
	    obj->name = str_dup( buf );
	}
	act( "$p is filled.", ch, obj, NULL, TO_CHAR , FALSE);
    }

    return;
}



void spell_cure_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if ( !is_affected( victim, gsn_blindness ) )
	return;
    affect_strip( victim, gsn_blindness );
    send_to_char( "Your vision returns!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    if (IS_SET(victim->affected_by, AFF_BLIND))
    {
    send_to_char("Ok.\n\r",ch);
    REMOVE_BIT(victim->affected_by, AFF_BLIND);
    }
 return;
}



void spell_cure_critical( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;
    if ((victim->fight_timer > 0) && (ch != victim))
    {
    stc("You cannot aid those in battle.\n\r",ch);
    return;
    }

    heal = dice(3, 8) + level - 6;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    if( IS_NPC(victim) && victim->hit >= victim->max_hit) victim->hit = victim->max_hit-100;
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    return;
}



void spell_cure_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;
    if ((victim->fight_timer > 0) && (ch != victim))
    {
    stc("You cannot aid those in battle.\n\r",ch);
    return;
    }

    heal = dice(1, 8) + level / 3;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    if( IS_NPC(victim) && victim->hit >= victim->max_hit) victim->hit = victim->max_hit-100;
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    return;
}



void spell_cure_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if ( is_affected( victim, gsn_poison ) )
    {
	affect_strip( victim, gsn_poison );
	act( "$N looks better.", ch, NULL, victim, TO_NOTVICT, FALSE );
	send_to_char( "A warm feeling runs through your body.\n\r", victim );
	send_to_char( "Ok.\n\r", ch );
    	if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    }
    return;
}



void spell_cure_serious( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;
    if ((victim->fight_timer > 0) && (ch != victim))
    {
    stc("You cannot aid those in battle.\n\r",ch);
    return;
    }

    heal = dice(2, 8) + level /2 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    if( IS_NPC(victim) && victim->hit >= victim->max_hit) victim->hit = victim->max_hit-100;
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    return;
}



void spell_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MSL];
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_CURSE) || saves_spell( level, victim ) )
	return;
    af.type      = sn;
    af.duration  = 5*level;
    af.location  = APPLY_HITROLL;
    af.modifier  = -10;
    af.bitvector = AFF_CURSE;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 10;
    affect_to_char( victim, &af );

    send_to_char( "You feel unclean.\n\r", victim );
    if ( ch != victim )
	{
	if (IS_NPC(victim))
	sprintf(buf, "%s has been cursed!\n\r",victim->short_descr);
	if (!IS_NPC(victim))
        sprintf(buf, "%s has been cursed!\n\r",victim->name);
	send_to_char( buf, ch );
	}
    return;
}



void spell_detect_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_EVIL) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_EVIL;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_hidden( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_HIDDEN) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_DETECT_HIDDEN;
    affect_to_char( victim, &af );
    send_to_char( "Your awareness improves.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_INVIS) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_INVIS;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_MAGIC) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_MAGIC;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD )
    {
	if ( obj->value[3] != 0 )
	    send_to_char( "You smell poisonous fumes.\n\r", ch );
	else
	    send_to_char( "It looks very delicious.\n\r", ch );
    }
    else
    {
	send_to_char( "It doesn't look poisoned.\n\r", ch );
    }

    return;
}



void spell_dispel_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
  
    if ( !IS_NPC(ch) && IS_EVIL(ch) )
	victim = ch;
  
    if ( IS_GOOD(victim) )
    {
	act( "God protects $N.", ch, NULL, victim, TO_ROOM , FALSE);
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
	act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR , FALSE);
	return;
    }

    dam = dice( level, 4 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn );
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    return;
}



void spell_dispel_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( victim != ch &&
	(saves_spell( level, victim ) || level < victim->level) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( !(victim->first_affect) && !IS_SET(victim->flag2, AFF_TOTALBLIND))
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    while (victim->last_affect)
    {
	affect_remove(victim,victim->last_affect);
    }

    if ( ch == victim )
    {
    	act("You remove all magical affects from yourself.",ch,NULL,NULL,TO_CHAR, FALSE);
    	act("$n has removed all magical affects from $mself.",ch,NULL,NULL,TO_ROOM, FALSE);
	if ( IS_SET(victim->flag2, AFF_TOTALBLIND) )
        {
	  REMOVE_BIT(victim->flag2, AFF_TOTALBLIND);
	  stc("The searing blindness leaves your eyes.\n\r",victim);
	}
        WAIT_STATE(ch, 10);
    }
    else
    {
    	act("You remove all magical affects from $N.",ch,NULL,victim,TO_CHAR, FALSE);
    	act("$n has removed all magical affects from $N.",ch,NULL,victim,TO_NOTVICT, FALSE);
    	act("$n has removed all magical affects from you.",ch,NULL,victim,TO_VICT, FALSE);
	if ( IS_SET(victim->flag2, AFF_TOTALBLIND) )
        {
          REMOVE_BIT(victim->flag2, AFF_TOTALBLIND);
          stc("The searing blindness leaves your eyes.\n\r",victim);
        }
    }

    return;
}



void spell_earthquake( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    send_to_char( "The earth trembles beneath your feet!\n\r", ch );
    act( "$n makes the earth tremble and shiver.", ch, NULL, NULL, TO_ROOM, FALSE );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL || !can_see(ch, vch))
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) ) )
		damage( ch, vch, level + dice(2, 8), sn );
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area )
	    send_to_char( "The earth trembles and shivers.\n\r", vch );
    }

    return;
}



void spell_enchant_weapon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;

    if ( obj->item_type != ITEM_WEAPON
    ||   IS_SET(obj->quest, QUEST_ENCHANTED)
    ||   IS_SET(obj->quest, QUEST_ARTIFACT)
    ||   obj->chobj != NULL )
    {
	send_to_char("You are unable to enchant this weapon.\n\r",ch);
	return;
    }

    CREATE( paf, AFFECT_DATA, 1 );
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_HITROLL;
    paf->modifier	= level / 5;
    paf->bitvector	= 0;

    LINK( paf, obj->first_affect, obj->last_affect, next, prev );

    CREATE( paf, AFFECT_DATA, 1 );
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_DAMROLL;
    paf->modifier	= level / 10;
    paf->bitvector	= 0;

    LINK( paf, obj->first_affect, obj->last_affect, next, prev );

    if ( IS_GOOD(ch) )
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_EVIL);
	SET_BIT(obj->quest, QUEST_ENCHANTED);
	act( "$p glows blue.", ch, obj, NULL, TO_CHAR , FALSE);
	act( "$p glows blue.", ch, obj, NULL, TO_ROOM , FALSE);
    }
    else if ( IS_EVIL(ch) )
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
	SET_BIT(obj->quest, QUEST_ENCHANTED);
	act( "$p glows red.", ch, obj, NULL, TO_CHAR , FALSE);
	act( "$p glows red.", ch, obj, NULL, TO_ROOM , FALSE);
    }
    else
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_EVIL);
	SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
	SET_BIT(obj->quest, QUEST_ENCHANTED);
	act( "$p glows yellow.", ch, obj, NULL, TO_CHAR , FALSE);
	act( "$p glows yellow.", ch, obj, NULL, TO_ROOM , FALSE);
    }
    return;
}
void spell_enchant_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;

    if ( obj->item_type != ITEM_ARMOR
    ||   IS_SET(obj->quest, QUEST_ENCHANTED)
    ||   IS_SET(obj->quest, QUEST_ARTIFACT)
    ||   obj->chobj != NULL )
    {
	send_to_char("You are unable to enchant this weapon.\n\r",ch);
	return;
    }

    CREATE( paf, AFFECT_DATA, 1);
/*
    paf->type		= 0;
*/
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_HITROLL;
    paf->modifier	= level / 5;
    paf->bitvector	= 0;
    LINK( paf, obj->first_affect, obj->last_affect, next, prev );

    CREATE( paf, AFFECT_DATA, 1);
/*
    paf->type		= -1;
*/
    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_DAMROLL;
    paf->modifier	= level / 5;
    paf->bitvector	= 0;
    LINK( paf, obj->first_affect, obj->last_affect, next, prev );

    paf->type		= sn;
    paf->duration	= -1;
    paf->location	= APPLY_AC;
    paf->modifier	= -level / 5;
    paf->bitvector	= 0;
    LINK( paf, obj->first_affect, obj->last_affect, next, prev );

    if ( IS_GOOD(ch) )
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_EVIL);
	SET_BIT(obj->quest, QUEST_ENCHANTED);
	act( "$p glows blue.", ch, obj, NULL, TO_CHAR , FALSE);
	act( "$p glows blue.", ch, obj, NULL, TO_ROOM , FALSE);
    }
    else if ( IS_EVIL(ch) )
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
	SET_BIT(obj->quest, QUEST_ENCHANTED);
	act( "$p glows red.", ch, obj, NULL, TO_CHAR , FALSE);
	act( "$p glows red.", ch, obj, NULL, TO_ROOM , FALSE);
    }
    else
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_EVIL);
	SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
	SET_BIT(obj->quest, QUEST_ENCHANTED);
	act( "$p glows yellow.", ch, obj, NULL, TO_CHAR , FALSE);
	act( "$p glows yellow.", ch, obj, NULL, TO_ROOM , FALSE);
    }
    return;
}



/*
 * Drain XP, MANA, HP.
 * Caster gains HP.
 */
void spell_energy_drain( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hp;

    if ( saves_spell( level, victim ) )
	return;

    if ( !IS_HERO(ch))
        ch->alignment = UMAX(-1000, ch->alignment - 200);

    if ( victim->level <= 2 )
    {
	dam		 = ch->hit + 1;
    }
    else
    {
	dam		 = dice(1, level);
    	if (IS_NPC(victim) || !IS_IMMUNE(victim, IMM_DRAIN) )
	{
	    victim->mana	/= 2;
	    victim->move	/= 2;
	    ch->hit		+= dam;
	    if (ch->hit > (2*ch->max_hit))
	    	ch->hit = (2 * ch->max_hit);
	}
    }

    hp = victim->hit;
    damage( ch, victim, dam, sn );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_DRAIN) )
	victim->hit = hp;
    if (!IS_NPC(ch) && ch != victim) do_beastlike(ch,"");

    return;
}

void spell_desanct( int sn, int level, CHAR_DATA *ch, void *vo)
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   char buf[MSL];

   if ( saves_spell( level, victim ) )
   {
      sprintf( buf, "You fail to overpower %s's mind!\n\r", victim->name );
      stc( buf, ch );
      sprintf( buf, "You overpower %s's magical power!\n\r", ch->name );
      stc( buf, victim );
   }
   else if ( !saves_spell( level, victim ) || IS_NPC(ch) )
   {
      affect_strip( victim, gsn_sanctuary );

      if ( IS_AFFECTED(victim, AFF_SANCTUARY) ) 
      {
         REMOVE_BIT(victim->affected_by, AFF_SANCTUARY);
      }

      send_to_char( "Your white aura fades.\n\r", victim );
      send_to_char( "#sDESANCT!!!! #yMuhahahaha.\n\r#n", ch );
      WAIT_STATE( victim, 15 );
   }
}

void spell_imp_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int random = number_range(10000,15000);

  victim->hit += random;
  if (victim->hit > victim->max_hit) victim->hit = victim->max_hit;
  update_pos( victim );
  send_to_char( "A warm feeling fills your body.\n\r", victim );
  if (ch == victim)
    act( "$n heals $mself.", ch, NULL, NULL, TO_ROOM , FALSE);
  else
    act( "$n heals $N.", ch, NULL, victim, TO_NOTVICT , FALSE); 
  if ( ch != victim )
    send_to_char( "#sSUPER HEAL!!!.\n\r#n", ch );   
  return;
}

void spell_imp_fireball( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;
 
  dam = number_range( 1000, 1500 );
  if ( saves_spell( level, victim ) )
    dam /= 2;
  damage( ch, victim, dam, sn );
  send_to_char("#sSUPER FIREBALL#n\n\r",ch);
  return;
}

void spell_imp_faerie_fire( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  
  if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) return;
  af.type      = sn;
  af.duration  = level;
  af.location  = APPLY_HITROLL;
  af.modifier  = -level;
  af.bitvector = AFF_FAERIE_FIRE;
  affect_to_char( victim, &af );

  send_to_char( "You are surrounded by a glowing pink outline.\n\r", victim );
  act( "$n is surrounded by a glowing pink outline.", victim, NULL, NULL, TO_ROOM , FALSE);
  return;
}

void spell_imp_teleport( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  CHAR_DATA *mount;
  ROOM_INDEX_DATA *pRoomIndex;
  int to_room;

  if ( victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_TELEPORT)
    || ( victim != ch && ( saves_spell( level, victim ) || saves_spell( level, victim ) ) ) )
    {
        send_to_char( "You failed the #sTELEPORT#n spell.\n\r", ch );
        return;
    }
  for ( ; ; )
  {
    to_room = number_range( 0, 65535 );
    pRoomIndex = get_room_index( to_room );
    if ( pRoomIndex != NULL )  
    if ( !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
      &&   !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY)
      &&   !IS_SET(pRoomIndex->room_flags, ROOM_NO_TELEPORT)
      &&   to_room != 30008 && to_room != 30002 )
            break;
  }
  act( "$n slowly fades out of existence.", victim, NULL, NULL, TO_ROOM, FALSE );
  char_from_room( victim );
  char_to_room( victim, pRoomIndex );
  act( "$n slowly fades into existence.", victim, NULL, NULL, TO_ROOM, FALSE );
  do_look( victim, "auto" );
  if ( (mount = ch->mount) == NULL ) return;
  char_from_room( mount );
  char_to_room( mount, ch->in_room );
  do_look( mount, "auto" );
  return;
}

void spell_fireball( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 20,
	 20,  20,  20,  20,  20,	 25,  25,  25,  25,  25,
	 30,  30,  30,  30,  30,	 35,  40,  45,  50,  55,
	 60,  65,  70,  75,  80,	 82,  84,  86,  88,  90,
	 92,  94,  96,  98, 100,	102, 104, 106, 108, 110,
	112, 114, 116, 118, 120,	122, 124, 126, 128, 130,
	150, 200, 250, 300, 400,	500, 650, 750, 850,1000
    };
    int dam;

    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    if (IS_CLASS(ch, CLASS_POWER_LICH)) dam *= 4;
    if (IS_NPC(victim) && dam >= victim->hit) dam = victim->hit-1;
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_HEAT)) dam =0;
    if (dam == 0) return;
    	damage( ch, victim, dam, sn );
    return;
}



void spell_flamestrike( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD)) return;
    dam = dice(6, 8);
    if ( saves_spell( level, victim ) )
	dam /= 2;
  if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_HEAT) ) dam = 0;

    	damage( ch, victim, dam, sn );
    return;
}



void spell_faerie_fire( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = 5 * level;
    af.bitvector = AFF_FAERIE_FIRE;
    affect_to_char( victim, &af );
    send_to_char( "You are surrounded by a pink outline.\n\r", victim );
    act( "$n is surrounded by a pink outline.", victim, NULL, NULL, TO_ROOM, FALSE );
    return;
}
/*
void spell_tendrils( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];
    sprintf(buf,
    "A look of concentration passes over your face.\n\r");
    send_to_char(buf,ch);
    sprintf(buf,
    "A look of concentration passes over %s's face.\n\r",ch->name);
    
    act(buf,ch,NULL,NULL,TO_ROOM, FALSE);
 
 
    if ( IS_AFFECTED(victim, AFF_TENDRILS) )
    {
        send_to_char( "They are already entangled by tendrils!\n\r", ch );
	return;
    }
 
    if ( is_safe(ch, victim) ) return;
 
    if ( saves_spell( level, victim ) && victim->position >= POS_FIGHTING
)
    {
        send_to_char( "You escape from your enemy's tendrils!\n\r", victim
);
        act("$n escapes the tendrils!",victim,NULL,NULL,TO_ROOM, FALSE);
	return;
    }
 
    af.type      = sn;
    af.location  = APPLY_AC;
    af.modifier  = 100;
    af.duration  = number_range(2,3);
    af.bitvector = AFF_TENDRILS;
    affect_to_char( victim, &af );
    send_to_char( "You are entrapped by tendrils of darkness!\n\r", victim
);
    act("$n is entrapped by tendrils of darkness",victim,NULL,NULL,TO_ROOM, FALSE);
    return;
}
*/
void spell_spew( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    if (IS_CLASS(ch, CLASS_VAMPIRE)) dam = ch->damcap[0];
    else dam = 250 * level;  
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
      	vch_next = vch->next_in_room;
        if (vch->trust>6) continue;
	if (ch == vch) continue;
        if (!IS_NPC(vch)) continue;
        damage( ch, vch, dam, sn );
      
    }
    return;
}

void spell_infirmity( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INFIRMITY) )
{
        act("A look of concentration passes over $n's face.",ch,NULL,NULL,TO_ROOM, FALSE);
        send_to_char("A look of concentration passes over your face.\n\r",ch);
        send_to_char("They are already weak.\n\r",ch);
   	return;
} 
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_DEX;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_WIS;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_CON;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_INT;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_DAMROLL;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_HITROLL;
    af.modifier  = 0 - level;
    af.bitvector = AFF_INFIRMITY;
    affect_to_char( victim, &af );
    send_to_char("A look of concentration passes over your face.\n\r",ch);
    act("A look of concentration passes over $n's face.",ch,NULL,NULL,TO_ROOM, FALSE);
    send_to_char( "You feel weaker.\n\r", victim );
    send_to_char( "You feel slower.\n\r", victim );
    send_to_char( "You feel less healthy.\n\r", victim );
    act( "$n looks weaker.", victim, NULL, NULL, TO_ROOM , FALSE);
    act( "$n looks slower.", victim, NULL, NULL, TO_ROOM , FALSE);
    act( "$n looks less healthy.",victim,NULL,NULL, TO_ROOM, FALSE);
    return;
}



void spell_faerie_fog( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *ich;

    if (( ch->in_room->vnum == 420032) && !IS_NPC(ch))
    {
        send_to_char( "You cannot do this in this room.\n\r",ch);
        return;
    }

  act( "$n conjures a cloud of purple smoke.", ch, NULL, NULL, TO_ROOM, FALSE );
  send_to_char( "You conjure a cloud of purple smoke.\n\r", ch );
  
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
    if (IS_AFFECTED(ich,AFF_SHADOWPLANE)) REMOVE_BIT(ich->affected_by, AFF_SHADOWPLANE);
    if (IS_AFFECTED(ich, AFF_PEACE)) REMOVE_BIT(ich->affected_by, AFF_PEACE);
    if (IS_AFFECTED(ich,AFF_ETHEREAL)) REMOVE_BIT(ich->affected_by, AFF_ETHEREAL);

    if (IS_SET(ich->newbits, NEW_DARKNESS))
    {
      REMOVE_BIT(ich->newbits, NEW_DARKNESS);
    }


    act( "$n is revealed!", ich, NULL, NULL, TO_ROOM , FALSE);
    send_to_char( "You are revealed!\n\r", ich );
  }
  return;
}



void spell_fly( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FLYING) )
	return;
    af.type      = sn;
    af.duration  = level + 3;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_FLYING;
    affect_to_char( victim, &af );
    send_to_char( "You rise up off the ground.\n\r", victim );
    act( "$n rises up off the ground.", victim, NULL, NULL, TO_ROOM, FALSE );
    return;
}



void spell_gate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    // Gate spell functionality has been disabled
    stc("It doesn't do anything.\n\r",ch);
    return;
}



/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
void spell_general_purpose( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = number_range( 25, 100 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn );
    return;
}



void spell_giant_strength( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = 2 + (level >= 18) + (level >= 25);
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel stronger.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_harm( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = UMAX(  20, victim->hit - dice(1,4) );
    if ( saves_spell( level, victim ) )
	dam = UMIN( 50, dam / 4 );
    dam = UMIN( 100, dam );
    damage( ch, victim, dam, sn );
    return;
}

void spell_group_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *ich;

  for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
  {
    if (IS_NPC(ich)) continue;
    if (ich->fight_timer > 0) continue;
    if (is_safe(ch,ich)) continue;
    if (is_same_group( ich, ch ) && (ich->fight_timer == 0))
    {
      ich->hit += number_range(150,250);
      if (ich->hit > ich->max_hit) ich->hit=ich->max_hit;
      update_pos( ch );
      send_to_char("You feel healed.\n\r",ich);
    }
  }
  return;
}

void spell_miracle( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if (!IS_SET(ch->special, SPC_NEWBIE))
    {
       stc("Only newbies may call upon the miracle healing.\n\r",ch);
       return;
    }
    if (!IS_SET(victim->special, SPC_NEWBIE))
    {
       stc("Only newbies may be touched by miracle healing.\n\r",ch);
       return;
    }
    if (victim->position == POS_FIGHTING)
    {
	stc("They are fighting, your spell fails!\n\r",ch);
	return;
    }
    if (ch->position == POS_FIGHTING)
    {
        stc("You are fighting, your spell fails!\n\r",ch);
        return;
    }

    victim->hit = victim->max_hit;
    update_pos( victim );
    send_to_char( "You have been touched by a miracle.\n\r", victim );
    if (ch == victim)
        act( "$n miraculously heals $mself.", ch, NULL, NULL, TO_ROOM, FALSE );
    else
        act( "$n miraculously heals $N.", ch, NULL, victim, TO_NOTVICT, FALSE );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    return;
}

void spell_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if (is_range(ch,victim) && !IS_NPC(victim))
    {
      if (ch != victim) return;
    }
    if ((victim->fight_timer > 0) && (ch != victim))
    {
      stc("You cannot aid those in battle.\n\r",ch);
      return;
    }
    victim->hit = UMIN( victim->hit + 1000, victim->max_hit );

    if( IS_NPC(victim) && victim->hit >= victim->max_hit) victim->hit = victim->max_hit-100;
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if (ch == victim)
	act( "$n heals $mself.", ch, NULL, NULL, TO_ROOM, FALSE );
    else
	act( "$n heals $N.", ch, NULL, victim, TO_NOTVICT, FALSE );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    return;
}



/*
 * Spell for mega1.are from Glop/Erkenbrand.
 */
void spell_high_explosive( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
	return;
    dam = dice( level, 150 );
    if ( saves_spell( level, victim ) ) dam *= 0.5;
    damage( ch, victim, dam, sn );
    return; 
}

void spell_identify( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int itemtype;

    act("You examine $p carefully.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$n examines $p carefully.",ch,obj,NULL,TO_ROOM, FALSE);

    sprintf( buf,
	"Object '%s' is type %s, extra flags %s.\n\rWeight is %d, value is %d.\n\r",

	obj->name,
	item_type_name( obj ),
	extra_bit_name( obj->extra_flags ),
	obj->weight,
	obj->cost
	);
    send_to_char( buf, ch );

    if (obj->points > 0 && obj->item_type != ITEM_QUEST
	 && obj->item_type != ITEM_PAGE)
    {
	sprintf( buf, "Quest point value is %d.\n\r", obj->points );
	send_to_char( buf, ch );
    }
    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1 &&
	obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "This object was created by %s, and is owned by %s.\n\r", obj->questmaker,obj->questowner );
	send_to_char( buf, ch );
    }
    else if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	sprintf( buf, "This object was created by %s.\n\r", obj->questmaker );
	send_to_char( buf, ch );
    }
    else if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "This object is owned by %s.\n\r", obj->questowner );
	send_to_char( buf, ch );
    }
    if (obj->timer > 0)
    {
	sprintf(buf, "This object will decay in: %d ticks.\n\r",obj->timer);
	stc(buf,ch);
    }
    if (IS_SET(obj->spectype, SITEM_COPPER))
        send_to_char("This item is forged with copper.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_IRON))
        send_to_char("This item is forged with iron.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_STEEL))
        send_to_char("This item is forged with steel.\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_ELECTRUM))
        send_to_char("This item is forged with #0(#7E#ple#Pct#pru#7m#0)#n!\n\r",ch);
    if (IS_SET(obj->spectype, SITEM_ADAMANTITE))
        send_to_char("This item is forged with adamantite.\n\r",ch);
    if (IS_SET(obj->quest, QUEST_BLOODA))
        send_to_char( "This weapon is dripping with kindred blood.\n\r",ch);
    if (IS_SET(obj->quest, QUEST_ENCHANTED))
	send_to_char( "This item has been enchanted.\n\r", ch );
    if (IS_SET(obj->quest, QUEST_SPELLPROOF))
	send_to_char( "This item is resistant to offensive spells.\n\r", ch );
    if( IS_SET(obj->extra_flags, ITEM_MENCHANT))
	stc( "This weapon is indeed perfect.\n\r", ch);
    if (IS_SET(obj->spectype, SITEM_DEMONIC))
	send_to_char( "This item is crafted from demonsteel.\n\r", ch );
    else if (IS_SET(obj->spectype, SITEM_SILVER))
	send_to_char( "This item is crafted from gleaming silver.\n\r", ch );

    switch ( obj->item_type )
    {
    case ITEM_PILL: 
    case ITEM_SCROLL: 
    case ITEM_POTION:
	sprintf( buf, "Level %d spells of:", obj->value[0] );
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
	sprintf( buf, "Quest point value is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_QUESTCARD:
	sprintf( buf, "Quest completion reward is %d quest points.\n\r", obj->level );
	send_to_char( buf, ch );
	break;

    case ITEM_WAND:
    case ITEM_STAFF: 
	sprintf( buf, "Has %d(%d) charges of level %d",
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
	sprintf( buf, "Damage is %d to %d (average %d).\n\r",
	    obj->value[1], obj->value[2],
	    ( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );
	if (obj->value[3] > 0)
	{
	    if (obj->value[3] == 0)
		sprintf(buf, "Weapon type: hit.\n\r");
            if (obj->value[3] == 1)
                sprintf(buf, "Weapon type: slice.\n\r");
            if (obj->value[3] == 2)
                sprintf(buf, "Weapon type: stab.\n\r");
            if (obj->value[3] == 3)
                sprintf(buf, "Weapon type: slash.\n\r");
            if (obj->value[3] == 4)
                sprintf(buf, "Weapon type: whip.\n\r");
            if (obj->value[3] == 5)
                sprintf(buf, "Weapon type: claw.\n\r");
            if (obj->value[3] == 6)
                sprintf(buf, "Weapon type: blast.\n\r");
            if (obj->value[3] == 7)
                sprintf(buf, "Weapon type: pound.\n\r");
            if (obj->value[3] == 8)
                sprintf(buf, "Weapon type: crush.\n\r");
            if (obj->value[3] == 9)
                sprintf(buf, "Weapon type: grep.\n\r");
            if (obj->value[3] == 10)
                sprintf(buf, "Weapon type: bite.\n\r");
            if (obj->value[3] == 11)
                sprintf(buf, "Weapon type: pierce.\n\r");
            if (obj->value[3] == 12)
                sprintf(buf, "Weapon type: suck.\n\r");
	    stc(buf,ch);
	}
	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] - ((obj->value[0] / 1000) * 1000);
	else
	    itemtype = obj->value[0];

	if (itemtype > 0)
	{
	    if (obj->level < 10)
		sprintf(buf,"%s is a minor spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level < 20)
		sprintf(buf,"%s is a lesser spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level < 30)
		sprintf(buf,"%s is an average spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level < 40)
		sprintf(buf,"%s is a greater spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level < 50)
		sprintf(buf,"%s is a major spell weapon.\n\r",capitalize(obj->short_descr));
	    else if (obj->level > 50)
		sprintf(buf,"%s is an ultimate spell weapon.\n\r",capitalize(obj->short_descr));
	    else
		sprintf(buf,"%s is a supreme spell weapon.\n\r",capitalize(obj->short_descr));
	    send_to_char(buf,ch);
	}

	if (itemtype == 1)
	    sprintf (buf, "This weapon is dripping with corrosive acid.\n\r");
	else if (itemtype == 4)
	    sprintf (buf, "This weapon radiates an aura of darkness.\n\r");
	else if (itemtype == 30)
	    sprintf (buf, "This ancient relic is the bane of all evil.\n\r");
	else if (itemtype == 34)
	    sprintf (buf, "This vampiric weapon drinks the souls of its victims.\n\r");
	else if (itemtype == 37)
	    sprintf (buf, "This weapon has been tempered in hellfire.\n\r");
	else if (itemtype == 48)
	    sprintf (buf, "This weapon crackles with sparks of lightning.\n\r");
	else if (itemtype == 53)
	    sprintf (buf, "This weapon is dripping with a dark poison.\n\r");
	else if (itemtype > 0)
	    sprintf (buf, "This weapon has been imbued with the power of %s.\n\r",skill_table[itemtype].name);
	if (itemtype > 0)
	    send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] / 1000;
	else
	    break;

	if (itemtype == 4)
	    sprintf (buf, "This weapon radiates an aura of darkness.\n\r");
	else if (itemtype == 27 || itemtype == 2)
	    sprintf (buf, "This weapon allows the wielder to see invisible things.\n\r");
	else if (itemtype == 39 || itemtype == 3)
	    sprintf (buf, "This weapon grants the power of flight.\n\r");
	else if (itemtype == 45 || itemtype == 1)
	    sprintf (buf, "This weapon allows the wielder to see in the dark.\n\r");
	else if (itemtype == 46 || itemtype == 5)
	    sprintf (buf, "This weapon renders the wielder invisible to the human eye.\n\r");
	else if (itemtype == 52 || itemtype == 6)
	    sprintf (buf, "This weapon allows the wielder to walk through solid doors.\n\r");
	else if (itemtype == 54 || itemtype == 7)
	    sprintf (buf, "This holy weapon protects the wielder from evil.\n\r");
        else if (itemtype == 139)
            sprintf (buf, "This unholy weapon protects the wielder from good.\n\r");
	else if (itemtype == 57 || itemtype == 8)
	    sprintf (buf, "This ancient weapon protects the wielder in combat.\n\r");
	else if (itemtype == 9)
	    sprintf (buf, "This crafty weapon allows the wielder to walk in complete silence.\n\r");
	else if (itemtype == 10)
	    sprintf (buf, "This powerful weapon surrounds its wielder with a shield of lightning.\n\r");
	else if (itemtype == 11)
	    sprintf (buf, "This powerful weapon surrounds its wielder with a shield of fire.\n\r");
	else if (itemtype == 12)
	    sprintf (buf, "This powerful weapon surrounds its wielder with a shield of ice.\n\r");
	else if (itemtype == 13)
	    sprintf (buf, "This powerful weapon surrounds its wielder with a shield of acid.\n\r");
	else if (itemtype == 14)
	    sprintf (buf, "This weapon protects its wielder from clan DarkBlade guardians.\n\r");
	else if (itemtype == 15)
	    sprintf (buf, "This ancient weapon surrounds its wielder with a shield of chaos.\n\r");
	else if (itemtype == 16)
	    sprintf (buf, "This ancient weapon regenerates the wounds of its wielder.\n\r");
	else if (itemtype == 26)
	    sprintf (buf, "This ancient weapon gives the power of enhanced dodge and parry.\n\r");
	else if (itemtype == 17)
	    sprintf (buf, "This ancient weapon allows its wielder to move at supernatural speed.\n\r");
	else if (itemtype == 18)
	    sprintf (buf, "This razor sharp weapon can slice through armour without difficulty.\n\r");
	else if (itemtype == 19)
	    sprintf (buf, "This ancient weapon gives its wielder the power to strike devastating blows.\n\r");
	else if (itemtype == 20)
	    sprintf (buf, "This ancient weapon surrounds its wielder with a shield of darkness.\n\r");
	else if (itemtype == 21)
	    sprintf (buf, "This ancient weapon grants superior protection to its wielder.\n\r");
	else if (itemtype == 22)
	    sprintf (buf, "This ancient weapon grants its wielder supernatural vision.\n\r");
	else if (itemtype == 23)
	    sprintf (buf, "This ancient weapon makes its wielder fleet-footed.\n\r");
	else if (itemtype == 24)
	    sprintf (buf, "This ancient weapon conceals its wielder from sight.\n\r");
	else if (itemtype == 25)
	    sprintf (buf, "This ancient weapon invokes the power of the beast.\n\r");
	else if (itemtype == 28)
	    sprintf (buf, "This ancient weapon increases the level of your spells by 20.\n\r");
	else
	    sprintf (buf, "This item is bugged...please report it.\n\r");
	if (itemtype > 0)
	    send_to_char( buf, ch );
	break;

    case ITEM_ARMOR:
	sprintf( buf, "Armor class is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	if (obj->value[3] < 1)
	    break;
	if (obj->value[3] == 4)
	    sprintf (buf, "This object radiates an aura of darkness.\n\r");
	else if (obj->value[3] == 27 || obj->value[3] == 2)
	    sprintf (buf, "This item allows the wearer to see invisible things.\n\r");
	else if (obj->value[3] == 39 || obj->value[3] == 3)
	    sprintf (buf, "This object grants the power of flight.\n\r");
	else if (obj->value[3] == 45 || obj->value[3] == 1)
	    sprintf (buf, "This item allows the wearer to see in the dark.\n\r");
	else if (obj->value[3] == 46 || obj->value[3] == 5)
	    sprintf (buf, "This object renders the wearer invisible to the human eye.\n\r");
	else if (obj->value[3] == 52 || obj->value[3] == 6)
	    sprintf (buf, "This object allows the wearer to walk through solid doors.\n\r");
	else if (obj->value[3] == 54 || obj->value[3] == 7)
	    sprintf (buf, "This holy relic protects the wearer from evil.\n\r");
        else if (obj->value[3] == 139)
            sprintf (buf, "This unholy relic protects the wearer from good.\n\r");
	else if (obj->value[3] == 57 || obj->value[3] == 8)
	    sprintf (buf, "This ancient relic protects the wearer in combat.\n\r");
	else if (obj->value[3] == 9)
	    sprintf (buf, "This crafty item allows the wearer to walk in complete silence.\n\r");
	else if (obj->value[3] == 10)
	    sprintf (buf, "This powerful item surrounds its wearer with a shield of lightning.\n\r");
	else if (obj->value[3] == 11)
	    sprintf (buf, "This powerful item surrounds its wearer with a shield of fire.\n\r");
	else if (obj->value[3] == 12)
	    sprintf (buf, "This powerful item surrounds its wearer with a shield of ice.\n\r");
	else if (obj->value[3] == 13)
	    sprintf (buf, "This powerful item surrounds its wearer with a shield of acid.\n\r");
	else if (obj->value[3] == 14)
	    sprintf (buf, "This object channels the power of god into its wearer.\n\r");
	else if (obj->value[3] == 15)
	    sprintf (buf, "This ancient item surrounds its wearer with a shield of chaos.\n\r");
	else if (obj->value[3] == 16)
	    sprintf (buf, "This ancient item regenerates the wounds of its wearer.\n\r");
	else if (obj->value[3] == 17)
	    sprintf (buf, "This ancient item allows its wearer to move at supernatural speed.\n\r");
	else if (obj->value[3] == 18)
	    sprintf (buf, "This powerful item allows its wearer to shear through armour without difficulty.\n\r");
	else if (obj->value[3] == 19)
	    sprintf (buf, "This ancient item gives its wearer the power to strike devastating blows.\n\r");
	else if (obj->value[3] == 20)
	    sprintf (buf, "This ancient item surrounds its wearer with a shield of darkness.\n\r");
	else if (obj->value[3] == 21)
	    sprintf (buf, "This ancient item grants superior protection to its wearer.\n\r");
	else if (obj->value[3] == 22)
	    sprintf (buf, "This ancient item grants its wearer supernatural vision.\n\r");
	else if (obj->value[3] == 23)
	    sprintf (buf, "This ancient item makes its wearer fleet-footed.\n\r");
	else if (obj->value[3] == 24)
	    sprintf (buf, "This ancient item conceals its wearer from sight.\n\r");
	else if (obj->value[3] == 25)
	    sprintf (buf, "This ancient item invokes the power of the beast.\n\r");
        else if (obj->value[3] == 28)
            sprintf (buf, "This ancient item invokes the power of ancient magic.\n\r");
	else if (obj->value[3] == 29)
	    sprintf (buf, "This ancient artifact grants its wearer the power of the third eye.\n\r"); 
	else if (obj->value[3] == 30)
	    sprintf (buf, "This ancient item grants the wearer the gift of manashield.\n\r");
	else if (obj->value[3] == 31)
	    sprintf (buf, "This superb item gives you double experience!\n\r");
	else if (obj->value[3] == 32)
	    sprintf(buf, "This is the splendid +500 damcap rare!\n\r");
	else if (obj->value[3] == 33)
	    sprintf(buf, "This is a nice +200 damcap item!\n\r");
	else if (obj->value[3] == 34)
	    sprintf(buf, "This is a superb +1000 damcap item!\n\r");
	else if (obj->value[3] == 35)
	    sprintf(buf, "This superb item gives you double gold!\n\r");
	else
	    sprintf (buf, "This item is bugged...please report it.\n\r");
	if (obj->value[3] > 0)
	    send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->pIndexData->first_affect; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "Affects %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    for ( paf = obj->first_affect; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "Affects %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    return;
}



void spell_infravision( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INFRARED) )
	return;
    act( "$n's eyes glow red.\n\r", ch, NULL, NULL, TO_ROOM, FALSE );
    af.type      = sn;
    af.duration  = 2 * level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INFRARED;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes glow red.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE) )
	return;

    act( "$n fades out of existence.", victim, NULL, NULL, TO_ROOM , FALSE);
    af.type      = sn;
    af.duration  = 24;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INVISIBLE;
    affect_to_char( victim, &af );
    send_to_char( "You fade out of existence.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_know_alignment( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char *msg;
    int ap;

    ap = victim->alignment;

         if ( ap >  700 ) msg = "$N has an aura as white as the driven snow.";
    else if ( ap >  350 ) msg = "$N is of excellent moral character.";
    else if ( ap >  100 ) msg = "$N is often kind and thoughtful.";
    else if ( ap > -100 ) msg = "$N doesn't have a firm moral commitment.";
    else if ( ap > -350 ) msg = "$N lies to $S friends.";
    else if ( ap > -700 ) msg = "$N's slash DISEMBOWELS you!";
    else msg = "I'd rather just not say anything at all about $N.";

    act( msg, ch, NULL, victim, TO_CHAR, FALSE );
    return;
}



void spell_lightning_bolt( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	10,
	15, 15, 15, 20, 20,	25, 25, 25, 25, 28,
	31, 34, 37, 40, 40,	41, 42, 42, 43, 44,
	44, 45, 46, 46, 47,	48, 48, 49, 50, 50,
	51, 52, 52, 53, 54,	54, 55, 56, 56, 57,
	58, 58, 59, 60, 60,	61, 62, 62, 63, 64,
	70, 80, 90,120,150,	200,250,300,350,400
    };
    int dam;
    int hp;

if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    if (IS_CLASS(ch, CLASS_POWER_LICH)) dam *= 5;
    hp = victim->hit;
    if (IS_NPC(victim) && dam >= victim->hit) dam = victim->hit-1;
    if (dam==0) return;
    damage( ch, victim, dam, sn );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_LIGHTNING))
	victim->hit = hp;
    return;
}



void spell_locate_object( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    bool found;
    int count=0;

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || !is_name( target_name, obj->name ) || 
(obj->item_type == ITEM_STEEL) )   
        continue;

        if ( (ch->level <= 10)
	     && (
		(obj->item_type == ITEM_ELECTRUM)
	     || (obj->item_type == ITEM_ADAMANTITE)
	     || (obj->quest == QUEST_RUNE)
	     || (obj->quest == QUEST_ARTIFACT) 
             || (obj->item_type == ITEM_TELEPORTER)
	     || (obj->item_type == ITEM_PAGE)
	     || (obj->item_type == ITEM_IOU)
		)
	    )
         continue;
	

//	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj );

/*	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s carried by %s.\n\r",
		obj->short_descr, PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    sprintf( buf, "%s in %s.\n\r",
		obj->short_descr, in_obj->in_room == NULL
		    ? "somewhere" : in_obj->in_room->name );
	}
*/
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
	    found = FALSE;

	if (found)
	{
	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );
	}
        if (count > 50 )
            break;
        else
	   count++;


    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void spell_magic_missile( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 0,
	 3,  3,  4,  4,  5,	 6,  6,  6,  6,  6,
	 7,  7,  7,  7,  7,	 8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,	10, 10, 10, 10, 10,
	11, 11, 11, 11, 11,	12, 12, 12, 12, 12,
	13, 13, 13, 13, 13,	14, 14, 14, 14, 14,
	15, 20, 25, 30, 35,	40, 45, 55, 65, 75
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn );
    return;
}



void spell_mass_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    CHAR_DATA *gch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( !is_same_group( gch, ch ) || IS_AFFECTED(gch, AFF_INVISIBLE) )
	    continue;
	act( "$n slowly fades out of existence.", gch, NULL, NULL, TO_ROOM, FALSE );
	send_to_char( "You slowly fade out of existence.\n\r", gch );
	af.type      = sn;
	af.duration  = 24;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_INVISIBLE;
	affect_to_char( gch, &af );
    }
    send_to_char( "Ok.\n\r", ch );

    return;
}



void spell_null( int sn, int level, CHAR_DATA *ch, void *vo )
{
    send_to_char( "That's not a spell!\n\r", ch );
    return;
}



void spell_pass_door( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PASS_DOOR) )
	return;
    af.type      = sn;
    af.duration  = number_fuzzy( level );
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PASS_DOOR;
    affect_to_char( victim, &af );
    act( "$n turns translucent.", victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char( "You turn translucent.\n\r", victim );
    return;
}



void spell_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    char buf [MAX_INPUT_LENGTH];

    /* Ghosts cannot be poisoned - KaVir */
    if ( IS_NPC(victim) && IS_AFFECTED(victim, AFF_ETHEREAL) ) return;
          return;
    if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) &&
	IS_VAMPAFF(victim, VAM_SERPENTIS) ) return;
    else if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) &&
	victim->power[DISC_WERE_SPID] > 2 ) return;
    else if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powers[1], DPOWER_DROWPOISON)) return;

    if ( saves_spell( level, victim ) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = 0 - number_range(1,3);
    af.bitvector = AFF_POISON;
    affect_join( victim, &af );
    send_to_char( "You feel very sick.\n\r", victim );
    if ( ch == victim ) return;
    if (!IS_NPC(victim))
	sprintf(buf,"%s looks very sick as your poison takes affect.\n\r",victim->name);
    else
	sprintf(buf,"%s looks very sick as your poison takes affect.\n\r",victim->short_descr);
    send_to_char(buf,ch);
    return;
}

void spell_readaura(int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
/*    char      arg [MAX_INPUT_LENGTH];*/
    char      buf [MAX_INPUT_LENGTH];
            
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
        sprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),
                get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
        send_to_char(buf,ch);
    }
    sprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,
                victim->move,victim->max_move);
    send_to_char(buf,ch);
    if (!IS_NPC(victim)) sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),
                char_ac(victim));
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
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT) && ch->sex == SEX_FEMALE)
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

void spell_protection_vs_good( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PROTECT_GOOD) )
        return;
    if (!IS_EVIL(victim))
    {
      send_to_char("The spell fails.\n\r",ch);
      return;
    }
    af.type      = sn;
    af.duration  = 24;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PROTECT_GOOD;
    affect_to_char( victim, &af );
    send_to_char( "You feel protected.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_protection( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PROTECT) )
	return;
    if (!IS_GOOD(victim))
    {
      send_to_char("The spell fails.\n\r",ch);
      return;
    }
    af.type      = sn;
    af.duration  = 24;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PROTECT;
    affect_to_char( victim, &af );
    send_to_char( "You feel protected.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_refresh( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->move += number_range(100,1000);
    if (victim->move >= victim->max_move) victim->move = victim->max_move;
    act("$n looks less tired.",victim,NULL,NULL,TO_ROOM, FALSE);
    send_to_char( "You feel less tired.\n\r", victim );
    if (!IS_NPC(victim) && victim->sex == SEX_MALE && 
	victim->pcdata->stage[0] < 1 && victim->pcdata->stage[2] > 0)
	victim->pcdata->stage[2] = 0;
    return;
}



void spell_remove_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];

    one_argument( target_name, arg );

    if ( arg[0] == '\0')
    {
	send_to_char( "Remove curse on what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, target_name ) ) != NULL )
    {
        if (IS_SET(victim->affected_by, AFF_CURSE))
        {
         REMOVE_BIT(victim->affected_by, AFF_CURSE);
         send_to_char( "Ok.\n\r", ch);
        }

	if ( is_affected( victim, gsn_curse ) )
    	{
	    affect_strip( victim, gsn_curse );
	    send_to_char( "You feel better.\n\r", victim );
	    if ( ch != victim )
		send_to_char( "Ok.\n\r", ch );
	    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
	}
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg ) ) != NULL )
    {
	if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
	{
	    REMOVE_BIT(obj->extra_flags, ITEM_NOREMOVE);
	    act( "$p flickers with energy.", ch, obj, NULL, TO_CHAR, FALSE );
	}
	else if ( IS_SET(obj->extra_flags, ITEM_NODROP) )
	{
	    REMOVE_BIT(obj->extra_flags, ITEM_NODROP);
	    act( "$p flickers with energy.", ch, obj, NULL, TO_CHAR, FALSE );
	}
	return;
    }
    send_to_char( "No such creature or object to remove curse on.\n\r", ch );
    return;
}



void spell_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
	return;
    af.type      = sn;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SANCTUARY;
    affect_to_char( victim, &af );
    act( "$n is surrounded in a white aura.", victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char( "You are surrounded by a white aura!\n\r", victim );
    return;
}



void spell_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;
    af.type      = sn;
    af.duration  = 10 + level;
    af.location  = APPLY_AC;
    af.modifier  = -50;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a force shield.", victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char( "You are surrounded by a force shield.\n\r", victim );
    return;
}



void spell_shocking_grasp( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const int dam_each[] = 
    {
	10,
	10, 10, 15, 15, 15,	20, 20, 25, 29, 33,
	36, 39, 39, 39, 40,	40, 41, 41, 42, 42,
	43, 43, 44, 44, 45,	45, 46, 46, 47, 47,
	48, 48, 49, 49, 50,	50, 51, 51, 52, 52,
	53, 53, 54, 54, 55,	55, 56, 56, 57, 57,
	60, 70, 85,100,125,	150,175,200,225,300
    };
    int dam;
    int hp;

    if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD)) return;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    hp = victim->hit;
    damage( ch, victim, dam, sn );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_LIGHTNING) )
	victim->hit = hp;
    return;
}



void spell_sleep( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
  
    if ( IS_AFFECTED(victim, AFF_SLEEP)
    ||   level < victim->level
/*    || ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_SLEEP) )*/
    || (  IS_NPC(victim) && IS_AFFECTED(victim, AFF_ETHEREAL) )
    ||   saves_spell( level, victim ) )
	return;

    af.type      = sn;
    af.duration  = 4 + level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
	send_to_char( "You feel very sleepy ..... zzzzzz.\n\r", victim );
	act( "$n goes to sleep.", victim, NULL, NULL, TO_ROOM, FALSE );
	victim->position = POS_SLEEPING;
    }

    return;
}



void spell_stone_skin( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -40;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin turns to stone.", victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char( "Your skin turns to stone.\n\r", victim );
    return;
}



void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;
    CHAR_DATA *mount;

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->fighting != NULL
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    ||   (IS_NPC(victim) && IS_AFFECTED(victim, AFF_ETHEREAL) )
    ||   (IS_NPC(victim) && saves_spell( level, victim ) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    act( "$n disappears suddenly.", victim, NULL, NULL, TO_ROOM, FALSE );
    char_from_room( victim );
    char_to_room( victim, ch->in_room );
    act( "$n arrives suddenly.", victim, NULL, NULL, TO_ROOM, FALSE );
    act( "$N has summoned you!", victim, NULL, ch,   TO_CHAR, FALSE );
    do_look( victim, "auto" );
    if ( (mount = victim->mount) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, get_room_index(victim->in_room->vnum) );
    do_look( mount, "auto" );
    return;
}



void spell_teleport( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *pRoomIndex;
    int to_room;

    if ( victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_TELEPORT)
    || ( !IS_NPC(ch) && victim->fighting != NULL )
    || ( !IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    || ( victim != ch
    && ( saves_spell( level, victim ) || saves_spell( level, victim ) ) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    for ( ; ; )
    {
	to_room = number_range( 0, 65535 );
	pRoomIndex = get_room_index( to_room );
	if ( pRoomIndex != NULL )
	if ( !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
	&&   !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY)
	&&   !IS_SET(pRoomIndex->room_flags, ROOM_NO_TELEPORT))
	    break;
    }

    act( "$n slowly fades out of existence.", victim, NULL, NULL, TO_ROOM, FALSE );
    char_from_room( victim );
    char_to_room( victim, pRoomIndex );
    act( "$n slowly fades into existence.", victim, NULL, NULL, TO_ROOM, FALSE );
    do_look( victim, "auto" );
    WAIT_STATE( ch, 7 );
    if ( (mount = ch->mount) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    do_look( mount, "auto" );
    return;
}



void spell_ventriloquate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char speaker[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;

    if (!IS_NPC(ch) && xIS_SET(ch->act, PLR_SILENCE))
    {
	stc("You cannot make yourself heard.\n\r",ch);
	return;
    }
    target_name = one_argument( target_name, speaker );

    sprintf( buf1, "%s says '%s'.\n\r",              speaker, target_name );
    sprintf( buf2, "Someone makes %s say '%s'.\n\r", speaker, target_name );
    buf1[0] = UPPER(buf1[0]);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if ( !is_name( speaker, vch->name ) )
	    send_to_char( saves_spell( level, vch ) ? buf2 : buf1, vch );
    }

    return;
}



void spell_weaken( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || saves_spell( level, victim ) )
	return;
    af.type      = sn;
    af.duration  = level / 2;
    af.location  = APPLY_STR;
    af.modifier  = -2;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel weaker.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



/*
 * This is for muds that _want_ scrolls of recall.
 * Ick.
 */
void spell_word_of_recall( int sn, int level, CHAR_DATA *ch, void *vo )
{
    do_recall( (CHAR_DATA *) vo, "" );
    return;
}



/*
 * NPC spells.
 */
void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hpch;

    /*if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD)) return;*/

    hpch = UMAX( 10, ch->hit );
    dam  = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) ) dam /=2;
    dam = cap_dam(ch,victim,dam);
    if (!IS_CLASS(ch, CLASS_MAGE))
    {
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_ACID)) dam = 0;
    }
    damage( ch, victim, dam, sn );
    return;
}

void spell_prismatic_beam( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hpch;
    int number;
    int balance;

    hpch = UMAX( 10, ch->hit );
    if (!IS_NPC(ch)) return;
    if (IS_NPC(ch))
    {
    if ( IS_ITEMAFF(victim, ITEMA_REFLECT) && number_range(1,3) == 1)
    {
        send_to_char( "You are unable to focus your spell on them.\n\r", ch );
        return;
    }

    dam  = number_range( ch->hit, ch->max_hit);
    if (IS_AFFECTED(victim, AFF_SANCTUARY))
    dam /= 2;
    dam = cap_dam(ch,victim,dam);
    balance = ch->level / 2000;
    for (number = 0;number <= balance; number++)
    damage( ch, victim, dam, sn );
    return;
    }
}

void spell_fire_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;


    if (!IS_CLASS(ch, CLASS_MAGE)) dam = 0;
    if (!IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam = 0;
    dam = (ch->hitroll + ch->damroll);
    if (IS_AFFECTED(victim, AFF_SANCTUARY))
    dam /= 2;
    dam = cap_dam(ch,victim,dam);
    damage( ch, victim, dam, sn );
    return;
}



void spell_frost_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hpch;

/*    if (IS_ITEMAFF(victim, ITEMA_ICESHIELD)) return;*/

    hpch = UMAX( 10, ch->hit );
    dam  = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) )  dam /= 2;
    dam = cap_dam(ch,victim,dam);
    if (!IS_CLASS(ch, CLASS_MAGE))
    {
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_COLD)) dam = 0;
    }
    damage( ch, victim, dam, sn );
    return;
}
void spell_super_blind( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    AFFECT_DATA af;

    if (!IS_NPC(ch)) return;
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
	{
        vch_next = vch->next_in_room;
         act("$n points his finger at $N's and screams.",ch,NULL,vch,TO_NOTVICT, FALSE);
         act("$n points at you and screams.",ch,NULL,vch,TO_VICT, FALSE);
         if (xIS_SET(vch->act,PLR_HOLYLIGHT))
             xREMOVE_BIT(vch->act,PLR_HOLYLIGHT);
         if (IS_SET(vch->affected_by, AFF_DETECT_HIDDEN))
             REMOVE_BIT(vch->affected_by, AFF_DETECT_HIDDEN);
         if (IS_SET(vch->affected_by, AFF_DETECT_INVIS))
             REMOVE_BIT(vch->affected_by, AFF_DETECT_INVIS);
         af.type      = sn;
         af.location  = APPLY_HITROLL;
         af.modifier  = -1000;
         af.duration  = 1000;
         af.bitvector = AFF_BLIND;
         affect_to_char( vch, &af );
         af.type      = sn;
         af.location  = APPLY_DAMROLL;
         af.modifier  = -1000;
         af.duration  = 1000;
         af.bitvector = AFF_BLIND;
         affect_to_char( vch, &af );
	}
      return;
    }
  return;
}
void spell_super_gas( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    int hpch;
    int chhp;
    int number;
    int balance;
    if (!IS_NPC(ch)) return;
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;
        if (can_see(ch, vch))
        if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
        {
            if ( IS_ITEMAFF(vch, ITEMA_REFLECT) && number_range(1,3) == 1) continue;

            hpch = UMAX( 10, ch->hit );
            dam  = number_range( hpch/6 + 1, hpch/3 );
            if ( saves_spell( level, vch ) )
                dam /= 2;
                dam /= 2;
        chhp = vch->hit;
      if (dam > ch->damcap[DAM_CAP])
 dam = number_range( (ch->damcap[DAM_CAP] - 200), (ch->damcap[DAM_CAP] + 100) );
       if ( IS_AFFECTED(vch, AFF_SANCTUARY) ) dam /=2;
       balance = ch->level / 2000;
       for (number = 0;number <= balance; number++)
       damage( ch, vch, dam, sn );
        }
    }
    return;
}

void spell_gas_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    int hpch;
    int chhp;

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;
        if (is_same_group(ch, vch)) continue;
	if (can_see(ch, vch)) 
	if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
	{
	    if ( IS_ITEMAFF(vch, ITEMA_REFLECT) && number_range(1,3) == 1) continue;

	    hpch = UMAX( 10, ch->hit );
	    dam  = number_range( hpch/6 + 1, hpch/3 );
	    if ( saves_spell( level, vch ) )
		dam /= 2;
		dam /= 2;    
	chhp = vch->hit;
      if (dam > ch->damcap[DAM_CAP]) dam = number_range( (ch->damcap[DAM_CAP] - 200),
 (ch->damcap[DAM_CAP] + 100) );
       if ( IS_AFFECTED(vch, AFF_SANCTUARY) ) dam /=2;
	    damage( ch, vch, dam, sn );
	}
    }
    return;
}



void spell_lightning_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hpch;

/*    if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD)) return;*/

    hpch = UMAX( 10, ch->hit );
    dam = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) ) dam /= 2;
    if (!IS_CLASS(ch, CLASS_MAGE))
    {
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_LIGHTNING)) dam = 0;
    }
    dam = cap_dam(ch,victim,dam);    
    damage( ch, victim, dam, sn );
    return;
}

/* Extra spells written by KaVir. */

void spell_guardian( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if (ch->pcdata->followers > 4)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }
    ch->pcdata->followers++;

    victim=create_mobile( get_mob_index( MOB_VNUM_GUARDIAN ) );
    victim->level = level;
    victim->hit = 100*level;
    victim->max_hit = 100*level;
    victim->hitroll = level;
    victim->damroll = level;
    victim->armor = 100 - (level*7);

    strcpy(buf,"Come forth, creature of darkness, and do my bidding!");
    do_say( ch, buf );

    send_to_char( "A demon bursts from the ground and bows before you.\n\r",ch );
    act( "$N bursts from the ground and bows before $n.", ch, NULL, victim, TO_ROOM, FALSE 
);

    char_to_room( victim, ch->in_room );


    add_follower( victim, ch );
    af.type      = sn;
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    return;
}

void spell_soulblade( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA    *obj = (OBJ_DATA *) vo;
    char buf     [MAX_STRING_LENGTH];
    char wpnname [MAX_INPUT_LENGTH];
    int wt = number_range(0,12);

    obj = create_object( get_obj_index( OBJ_VNUM_SOULBLADE ), 0 );
    if (wt == 0) strcpy(wpnname,"hitter");
    if (wt == 1) strcpy(wpnname,"razor");
    if (wt == 2) strcpy(wpnname,"dirk");
    if (wt == 3) strcpy(wpnname,"sword");
    if (wt == 4) strcpy(wpnname,"whip");
    if (wt == 5) strcpy(wpnname,"claw");
    if (wt == 6) strcpy(wpnname,"cannon");
    if (wt == 7) strcpy(wpnname,"hammer");
    if (wt == 8) strcpy(wpnname,"mace");
    if (wt == 9) strcpy(wpnname,"tentacle");
    if (wt == 10) strcpy(wpnname,"fang");
    if (wt == 11) strcpy(wpnname,"dagger");
    if (wt == 12) strcpy(wpnname,"vaccum");


    free_string(obj->name);
    sprintf(buf,"%s soul %s",ch->name,wpnname);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    if (IS_NPC(ch)) sprintf(buf,"%s's soul %s",ch->short_descr,wpnname);
    else sprintf(buf,"%s's soul %s",ch->pcdata->switchname,wpnname);
    buf[0] = UPPER(buf[0]);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    if (IS_NPC(ch)) sprintf(buf,"%s's soul %s is lying here.",ch->short_descr,wpnname);
    else sprintf(buf,"%s's soul %s is lying here.",ch->name,wpnname);
    buf[0] = UPPER(buf[0]);
    obj->description=str_dup(buf);

    if (IS_NPC(ch)) obj->level = ch->level;
    else if (ch->spl[2] > 4) obj->level = ch->spl[2]/4;
    else obj->level = 1;
    if (obj->level > 60) obj->level = 60;
    if (number_range(1,2) == 1)
      obj->value[0] = 17024;
    else
      obj->value[0] = 18024;
    obj->value[1] = number_range(100,200);
    obj->value[2] = number_range(200,300);
    obj->value[3] = wt;
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    if (!IS_NPC(ch))
    {
	if (obj->questowner != NULL) free_string(obj->questowner);
	obj->questowner = str_dup(ch->pcdata->switchname);
    }
    obj_to_char(obj,ch);
    act("$p fades into existance in your hand.", ch, obj, NULL, TO_CHAR, FALSE);
    act("$p fades into existance in $n's hand.", ch, obj, NULL, TO_ROOM, FALSE);
    return;
}

void spell_mana( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
	IS_VAMPAFF(ch, VAM_CELERITY))
    {
	if ( ch->move < 25 )
	{
	    send_to_char( "You are too exhausted to do that.\n\r", ch );
	    return;
	}
	ch->move = ch->move - 25;
    }
    else
    {
	if ( ch->move < 50 )
	{
	    send_to_char( "You are too exhausted to do that.\n\r", ch );
	    return;
	}
	ch->move = ch->move - 50;
    }
    victim->mana = UMIN( victim->mana + level + 10, victim->max_mana);
    update_pos(ch);
    update_pos(victim);
    if (ch == victim)
    {
        send_to_char("You draw in energy from your surrounding area.\n\r",
                ch);
        act("$n draws in energy from $s surrounding area.", ch, NULL, NULL,TO_ROOM, FALSE);
        return;
    }
    act("You draw in energy from around you and channel it into $N.",
            ch, NULL, victim, TO_CHAR, FALSE);
    act("$n draws in energy and channels it into $N.",
            ch, NULL, victim, TO_NOTVICT, FALSE);
    act("$n draws in energy and channels it into you.",
            ch, NULL, victim, TO_VICT, FALSE);
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    return;
}

void spell_frenzy( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    
    if (!IS_CLASS(ch, CLASS_MAGE) || !IS_CLASS(ch, CLASS_POWER_LICH))
    {
    if ( is_affected( victim, sn )) return;
    af.type      = sn;
    af.duration  = 1 + level / 5;
    af.location  = APPLY_HITROLL;
    af.modifier  = level / 3;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_DAMROLL;
    af.modifier  = level / 3;
    affect_to_char( victim, &af );
    }
    else if (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_POWER_LICH))
    {
    if ( is_affected( victim, sn )) return;
    af.type      = sn;
    af.duration  = 1 + level / 3;
    af.location  = APPLY_HITROLL;
    af.modifier  = level / 2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_DAMROLL;
    af.modifier  = level / 2;
    affect_to_char( victim, &af );
    }
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    act( "$n is consumed with rage.", victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char( "You are consumed with rage!\n\r", victim );
    if (!IS_NPC(victim)) do_beastlike(victim,"");
    return;
}

void spell_darkblessing( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    if (!IS_CLASS(ch, CLASS_MAGE) || !IS_CLASS(ch, CLASS_POWER_LICH))
    {
    if ( is_affected( victim, sn ) ) return;

    af.type      = sn;
    af.duration  = level / 2;
    af.location  = APPLY_HITROLL;
    af.modifier  = 1 + level / 10;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location  = APPLY_DAMROLL;
    af.modifier  = 1 + level / 10;
    affect_to_char( victim, &af );
    }
    else if (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_POWER_LICH))
    {
    if ( is_affected( victim, sn ) ) return;

    af.type      = sn;
    af.duration  = level * 0.75;
    af.location  = APPLY_HITROLL;
    af.modifier  = 1 + level / 5;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location  = APPLY_DAMROLL;
    af.modifier  = 1 + level / 5;
    affect_to_char( victim, &af );
    }
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    act( "$n looks wicked.", victim, NULL, NULL, TO_ROOM, FALSE );
    send_to_char( "You feel wicked.\n\r", victim );
    return;
}

void spell_portal( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char       arg [MAX_INPUT_LENGTH];
    int        duration;

    one_argument( target_name, arg );

    if ( arg[0] == '\0')
    {
	send_to_char( "Who do you wish to create a portal to?\n\r", ch );
	return;
    }

    victim = get_char_world( ch, arg );

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   IS_NPC(ch)
    ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) && victim->fight_timer == 0)
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_ASTRAL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   victim->in_room->vnum == ch->in_room->vnum)
    {
	send_to_char( "You failed.\n\r", ch );
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


    duration = number_range(2,3);

    obj = create_object( get_obj_index( OBJ_VNUM_PORTAL ), 0 );
    obj->value[0] = victim->in_room->vnum;
    obj->value[3] = ch->in_room->vnum;
    obj->timer = duration;
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, ch->in_room );

    obj = create_object( get_obj_index( OBJ_VNUM_PORTAL ), 0 );
    obj->value[0] = ch->in_room->vnum;
    obj->value[3] = victim->in_room->vnum;
    obj->timer = duration;
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE))
	obj->extra_flags = ITEM_SHADOWPLANE;
    obj_to_room( obj, victim->in_room );

    act( "$p appears in front of $n.", ch, obj, NULL, TO_ROOM, FALSE );
    act( "$p appears in front of you.", ch, obj, NULL, TO_CHAR, FALSE );
    act( "$p appears in front of $n.", victim, obj, NULL, TO_ROOM, FALSE );
    act( "$p appears in front of you.", ch, obj, victim, TO_VICT, FALSE );
    return;
}

/* This spell is designed for potions */
void spell_energyflux( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    victim->mana = UMIN( victim->mana + 50, victim->max_mana);
    update_pos(victim);
    send_to_char("You feel mana channel into your body.\n\r",victim);
    return;
}

void spell_voodoo( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA  *obj;
    char       buf [MAX_INPUT_LENGTH];
    char       arg [MAX_INPUT_LENGTH];
    char     part1 [MAX_INPUT_LENGTH];
    char     part2 [MAX_INPUT_LENGTH];
    int       worn;

    one_argument( target_name, arg );

    victim = get_char_world( ch, arg );

    if (ch->practice < 5)
    {
	send_to_char( "It costs 5 points of primal energy to create a voodoo doll.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL )
    {
	send_to_char( "Nobody by that name is playing.\n\r", ch );
	return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
    {
    	if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
	{
	    send_to_char( "You are not holding any body parts.\n\r", ch );
	    return;
	}
	else worn = WEAR_HOLD;
    }
    else worn = WEAR_WIELD;

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if      (obj->value[2] == 12) sprintf(part1,"head %s",victim->name);
    else if (obj->value[2] == 13) sprintf(part1,"heart %s",victim->name);
    else if (obj->value[2] == 14) sprintf(part1,"arm %s",victim->name);
    else if (obj->value[2] == 15) sprintf(part1,"leg %s",victim->name);
    else if (obj->value[2] == 30004) sprintf(part1,"entrails %s",victim->name);
    else if (obj->value[2] == 30005) sprintf(part1,"brain %s",victim->name);
    else if (obj->value[2] == 30006) sprintf(part1,"eye eyeball %s",victim->name);
    else if (obj->value[2] == 30012) sprintf(part1,"face %s",victim->name);
    else if (obj->value[2] == 30013) sprintf(part1,"windpipe %s",victim->name);
    else if (obj->value[2] == 30014) sprintf(part1,"cracked head %s",victim->name);
    else if (obj->value[2] == 30025) sprintf(part1,"ear %s",victim->name);
    else if (obj->value[2] == 30026) sprintf(part1,"nose %s",victim->name);
    else if (obj->value[2] == 30027) sprintf(part1,"tooth %s",victim->name);
    else if (obj->value[2] == 30028) sprintf(part1,"tongue %s",victim->name);
    else if (obj->value[2] == 30029) sprintf(part1,"hand %s",victim->name);
    else if (obj->value[2] == 30030) sprintf(part1,"foot %s",victim->name);
    else if (obj->value[2] == 30031) sprintf(part1,"thumb %s",victim->name);
    else if (obj->value[2] == 30032) sprintf(part1,"index finger %s",victim->name);
    else if (obj->value[2] == 30033) sprintf(part1,"middle finger %s",victim->name);
    else if (obj->value[2] == 30034) sprintf(part1,"ring finger %s",victim->name);
    else if (obj->value[2] == 30035) sprintf(part1,"little finger %s",victim->name);
    else if (obj->value[2] == 30036) sprintf(part1,"toe %s",victim->name);
    else
    {
	sprintf(buf,"%s isn't a part of %s!\n\r",obj->name,victim->name);
	send_to_char( buf, ch );
	return;
    }

    sprintf(part2,obj->name);

    if ( str_cmp(part1,part2) )
    {
	sprintf(buf,"But you are holding %s, not %s!\n\r",obj->short_descr,victim->name);
	send_to_char( buf, ch );
	return;
    }

    act("$p vanishes from your hand in a puff of smoke.", ch, obj, NULL, TO_CHAR, FALSE);
    act("$p vanishes from $n's hand in a puff of smoke.", ch, obj, NULL, TO_ROOM, FALSE);
    obj_from_char(obj);
    extract_obj(obj);

    obj = create_object( get_obj_index( OBJ_VNUM_VOODOO_DOLL ), 0 );

    sprintf(buf,"%s voodoo doll",victim->name);
    free_string(obj->name);
    obj->name=str_dup(buf);

    sprintf(buf,"a voodoo doll of %s",victim->name);
    free_string(obj->short_descr);
    obj->short_descr=str_dup(buf);

    sprintf(buf,"A voodoo doll of %s lies here.",victim->name);
    free_string(obj->description);
    obj->description=str_dup(buf);

    obj_to_char(obj,ch);
    equip_char(ch,obj,worn);

    act("$p appears in your hand.", ch, obj, NULL, TO_CHAR, FALSE);
    act("$p appears in $n's hand.", ch, obj, NULL, TO_ROOM, FALSE);

    ch->practice -= 5;

    return;
}

void spell_transport( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Transport which object?\n\r", ch );
        return;
    }

    if ( arg2[0] == '\0' )
    {
        send_to_char( "Transport who whom?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that item.\n\r", ch );
	return;
    }

	if (IS_SET(obj->quest, QUEST_ARTIFACT)) {
	send_to_char("You can't transport that.\n\r", ch );
	return;}

	if (obj->item_type == ITEM_KINGDOM_POWER) {
	send_to_char("You can't transport that.\n\r", ch );
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

    if (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_TRANSPORT))
    {
	send_to_char( "You are unable to transport anything to them.\n\r", ch );
	return;
    }

    act("$p vanishes from your hands in an swirl of smoke.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$p vanishes from $n's hands in a swirl of smoke.",ch,obj,NULL,TO_ROOM, FALSE);
    obj_from_char(obj);
    obj_to_char(obj,victim);
    act("$p appears in your hands in an swirl of smoke.",victim,obj,NULL,TO_CHAR, FALSE);
    act("$p appears in $n's hands in an swirl of smoke.",victim,obj,NULL,TO_ROOM, FALSE);
    do_autosave(ch,"");
    do_autosave(victim,"");
    return;
}

void spell_regenerate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int teeth = 0;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Which body part?\n\r", ch );
        return;
    }

    if ( arg2[0] == '\0' )
    {
        send_to_char( "Regenerate which person?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (victim->loc_hp[6] > 0)
    {
	send_to_char( "You cannot regenerate someone who is still bleeding.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that item.\n\r", ch );
	return;
    }

    if (IS_HEAD(victim,LOST_TOOTH_1  )) teeth += 1;
    if (IS_HEAD(victim,LOST_TOOTH_2  )) teeth += 2;
    if (IS_HEAD(victim,LOST_TOOTH_4  )) teeth += 4;
    if (IS_HEAD(victim,LOST_TOOTH_8  )) teeth += 8;
    if (IS_HEAD(victim,LOST_TOOTH_16 )) teeth += 16;

    if (obj->pIndexData->vnum == OBJ_VNUM_SLICED_ARM)
    {
	if (!IS_ARM_L(victim,LOST_ARM) && !IS_ARM_R(victim,LOST_ARM))
	    {send_to_char("They don't need an arm.\n\r",ch); return;}
	if (IS_ARM_L(victim,LOST_ARM))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_L],LOST_ARM);
	else if (IS_ARM_R(victim,LOST_ARM))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_R],LOST_ARM);
	act("You press $p onto the stump of $N's shoulder.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto the stump of $N's shoulder.",ch,obj,victim,TO_NOTVICT, 
FALSE);
	act("$n presses $p onto the stump of your shoulder.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SLICED_LEG)
    {
	if (!IS_LEG_L(victim,LOST_LEG) && !IS_LEG_R(victim,LOST_LEG))
	    {send_to_char("They don't need a leg.\n\r",ch); return;}
	if (IS_LEG_L(victim,LOST_LEG))
	    REMOVE_BIT(victim->loc_hp[LOC_LEG_L],LOST_LEG);
	else if (IS_LEG_R(victim,LOST_LEG))
	    REMOVE_BIT(victim->loc_hp[LOC_LEG_R],LOST_LEG);
	act("You press $p onto the stump of $N's hip.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto the stump of $N's hip.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto the stump of your hip.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SQUIDGY_EYEBALL)
    {
	if (!IS_HEAD(victim,LOST_EYE_L) && !IS_HEAD(victim,LOST_EYE_R))
	    {send_to_char("They don't need an eye.\n\r",ch); return;}
	if (IS_HEAD(victim,LOST_EYE_L))
	    REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_EYE_L);
	else if (IS_HEAD(victim,LOST_EYE_R))
	    REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_EYE_R);
	act("You press $p into $N's empty eye socket.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p into $N's empty eye socket.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p into your empty eye socket.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SLICED_EAR)
    {
	if (!IS_HEAD(victim,LOST_EAR_L) && !IS_HEAD(victim,LOST_EAR_R))
	    {send_to_char("They don't need an ear.\n\r",ch); return;}
	if (IS_HEAD(victim,LOST_EAR_L))
	    REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_EAR_L);
	else if (IS_HEAD(victim,LOST_EAR_R))
	    REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_EAR_R);
	act("You press $p onto the side of $N's head.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto the side of $N's head.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto the side of your head.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SLICED_NOSE)
    {
	if (!IS_HEAD(victim,LOST_NOSE))
	    {send_to_char("They don't need a nose.\n\r",ch); return;}
	REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_NOSE);
	act("You press $p onto the front of $N's face.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto the front of $N's face.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto the front of your face.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SEVERED_HAND)
    {
	if (!IS_ARM_L(victim,LOST_ARM) && IS_ARM_L(victim,LOST_HAND))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_L],LOST_HAND);
	else if (!IS_ARM_R(victim,LOST_ARM) && IS_ARM_R(victim,LOST_HAND))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_R],LOST_HAND);
	else
	{
	    send_to_char("They don't need a hand.\n\r",ch);
	    return;
	}
	act("You press $p onto the stump of $N's wrist.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto the stump of $N's wrist.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto the stump of your wrist.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SEVERED_FOOT)
    {
	if (!IS_LEG_L(victim,LOST_LEG) && IS_LEG_L(victim,LOST_FOOT))
	    REMOVE_BIT(victim->loc_hp[LOC_LEG_L],LOST_FOOT);
	else if (!IS_LEG_R(victim,LOST_LEG) && IS_LEG_R(victim,LOST_FOOT))
	    REMOVE_BIT(victim->loc_hp[LOC_LEG_R],LOST_FOOT);
	else
	{
	    send_to_char("They don't need a foot.\n\r",ch);
	    return;
	}
	act("You press $p onto the stump of $N's ankle.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto the stump of $N's ankle.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto the stump of your ankle.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SEVERED_THUMB)
    {
	if (!IS_ARM_L(victim,LOST_ARM) && !IS_ARM_L(victim,LOST_HAND)
		&& IS_ARM_L(victim,LOST_THUMB))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_L],LOST_THUMB);
	else if (!IS_ARM_R(victim,LOST_ARM) && !IS_ARM_R(victim,LOST_HAND)
		&& IS_ARM_R(victim,LOST_THUMB))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_R],LOST_THUMB);
	else
	{
	    send_to_char("They don't need a thumb.\n\r",ch);
	    return;
	}
	act("You press $p onto $N's hand.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto $N's hand.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto your hand.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SEVERED_INDEX)
    {
	if (!IS_ARM_L(victim,LOST_ARM) && !IS_ARM_L(victim,LOST_HAND)
		&& IS_ARM_L(victim,LOST_FINGER_I))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	else if (!IS_ARM_R(victim,LOST_ARM) && !IS_ARM_R(victim,LOST_HAND)
		&& IS_ARM_R(victim,LOST_FINGER_I))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	else
	{
	    send_to_char("They don't need an index finger.\n\r",ch);
	    return;
	}
	act("You press $p onto $N's hand.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto $N's hand.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto your hand.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SEVERED_MIDDLE)
    {
	if (!IS_ARM_L(victim,LOST_ARM) && !IS_ARM_L(victim,LOST_HAND)
		&& IS_ARM_L(victim,LOST_FINGER_M))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	else if (!IS_ARM_R(victim,LOST_ARM) && !IS_ARM_R(victim,LOST_HAND)
		&& IS_ARM_R(victim,LOST_FINGER_M))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	else
	{
	    send_to_char("They don't need a middle finger.\n\r",ch);
	    return;
	}
	act("You press $p onto $N's hand.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto $N's hand.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto your hand.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SEVERED_RING)
    {
	if (!IS_ARM_L(victim,LOST_ARM) && !IS_ARM_L(victim,LOST_HAND)
		&& IS_ARM_L(victim,LOST_FINGER_R))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	else if (!IS_ARM_R(victim,LOST_ARM) && !IS_ARM_R(victim,LOST_HAND)
		&& IS_ARM_R(victim,LOST_FINGER_R))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	else
	{
	    send_to_char("They don't need a ring finger.\n\r",ch);
	    return;
	}
	act("You press $p onto $N's hand.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto $N's hand.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto your hand.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (obj->pIndexData->vnum == OBJ_VNUM_SEVERED_LITTLE)
    {
	if (!IS_ARM_L(victim,LOST_ARM) && !IS_ARM_L(victim,LOST_HAND)
		&& IS_ARM_L(victim,LOST_FINGER_L))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_L],LOST_FINGER_L);
	else if (!IS_ARM_R(victim,LOST_ARM) && !IS_ARM_R(victim,LOST_HAND)
		&& IS_ARM_R(victim,LOST_FINGER_L))
	    REMOVE_BIT(victim->loc_hp[LOC_ARM_R],LOST_FINGER_L);
	else
	{
	    send_to_char("They don't need a little finger.\n\r",ch);
	    return;
	}
	act("You press $p onto $N's hand.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p onto $N's hand.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p onto your hand.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
	return;
    }
    else if (teeth > 0)
    {
    	if (IS_HEAD(victim,LOST_TOOTH_1 ))
	    REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_1);
    	if (IS_HEAD(victim,LOST_TOOTH_2 ))
	    REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_2);
    	if (IS_HEAD(victim,LOST_TOOTH_4 ))
	    REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_4);
    	if (IS_HEAD(victim,LOST_TOOTH_8 ))
	    REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_8);
    	if (IS_HEAD(victim,LOST_TOOTH_16))
	    REMOVE_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_16);
	teeth -= 1;
	if (teeth >= 16) {teeth -= 16;
	    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_16);}
	if (teeth >= 8 ) {teeth -= 8;
	    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_8);}
	if (teeth >= 4 ) {teeth -= 4;
	    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_4);}
	if (teeth >= 2 ) {teeth -= 2;
	    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_2);}
	if (teeth >= 1 ) {teeth -= 1;
	    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_1);}
	act("You press $p into $N's mouth.",ch,obj,victim,TO_CHAR, FALSE);
	act("$n presses $p into $N's mouth.",ch,obj,victim,TO_NOTVICT, FALSE);
	act("$n presses $p into your mouth.",ch,obj,victim,TO_VICT, FALSE);
	extract_obj(obj);
    }
    else
    {
	act("There is nowhere to stick $p on $N.",ch,obj,victim,TO_CHAR, FALSE);
	return;
    }
    return;
}

void spell_clot( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if (IS_BLEEDING(victim,BLEEDING_HEAD))
    {
	act("$n's head stops bleeding.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your head stops bleeding.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_HEAD);
    }
    else if (IS_BLEEDING(victim,BLEEDING_THROAT))
    {
	act("$n's throat stops bleeding.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your throat stops bleeding.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_THROAT);
    }
    else if (IS_BLEEDING(victim,BLEEDING_ARM_L))
    {
	act("The stump of $n's left arm stops bleeding.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("The stump of your left arm stops bleeding.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
    }
    else if (IS_BLEEDING(victim,BLEEDING_ARM_R))
    {
	act("The stump of $n's right arm stops bleeding.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("The stump of your right arm stops bleeding.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
    }
    else if (IS_BLEEDING(victim,BLEEDING_LEG_L))
    {
	act("The stump of $n's left leg stops bleeding.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("The stump of your left leg stops bleeding.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
    }
    else if (IS_BLEEDING(victim,BLEEDING_LEG_R))
    {
	act("The stump of $n's right leg stops bleeding.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("The stump of your right leg stops bleeding.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
    }
    else if (IS_BLEEDING(victim,BLEEDING_HAND_L))
    {
	act("The stump of $n's left wrist stops bleeding.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("The stump of your left wrist stops bleeding.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
    }
    else if (IS_BLEEDING(victim,BLEEDING_HAND_R))
    {
	act("The stump of $n's right wrist stops bleeding.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("The stump of your right wrist stops bleeding.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
    }
    else if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
    {
	act("The stump of $n's left ankle stops bleeding.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("The stump of your left ankle stops bleeding.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
    }
    else if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
    {
	act("The stump of $n's right ankle stops bleeding.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("The stump of your right ankle stops bleeding.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
    }
    else send_to_char("They have no wounds to clot.\n\r",ch);
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    return;
}

void spell_mend( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int ribs = 0;

    if (IS_BODY(victim,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(victim,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(victim,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(victim,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(victim,BROKEN_RIBS_16)) ribs += 16;

    if (ribs > 0)
    {
    	if (IS_BODY(victim,BROKEN_RIBS_1 ))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_1);
    	if (IS_BODY(victim,BROKEN_RIBS_2 ))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_2);
	if (IS_BODY(victim,BROKEN_RIBS_4 ))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_4);
	if (IS_BODY(victim,BROKEN_RIBS_8 ))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_8);
	if (IS_BODY(victim,BROKEN_RIBS_16))
	    REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_16);
	ribs -= 1;
	if (ribs >= 16) {ribs -= 16;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
	if (ribs >= 8 ) {ribs -= 8;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
	if (ribs >= 4 ) {ribs -= 4;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
	if (ribs >= 2 ) {ribs -= 2;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
	if (ribs >= 1 ) {ribs -= 1;
	    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
	act("One of $n's ribs snap back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("One of your ribs snap back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
    }
    else if (IS_HEAD(victim,BROKEN_NOSE) && !IS_HEAD(victim,LOST_NOSE))
    {
	act("$n's nose snaps back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your nose snaps back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (IS_HEAD(victim,BROKEN_JAW))
    {
	act("$n's jaw snaps back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your jaw snaps back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    else if (IS_HEAD(victim,BROKEN_SKULL))
    {
	act("$n's skull knits itself back together.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your skull knits itself back together.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_HEAD],BROKEN_SKULL);
    }
    else if (IS_BODY(victim,BROKEN_SPINE))
    {
	act("$n's spine knits itself back together.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your spine knits itself back together.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_BODY],BROKEN_SPINE);
    }
    else if (IS_BODY(victim,BROKEN_NECK))
    {
	act("$n's neck snaps back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your neck snaps back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);
    }
    else if (IS_ARM_L(victim,BROKEN_ARM) && !IS_ARM_L(victim,LOST_ARM))
    {
	act("$n's left arm snaps back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your left arm snaps back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_ARM);
    }
    else if (IS_ARM_R(victim,BROKEN_ARM) && !IS_ARM_R(victim,LOST_ARM))
    {
	act("$n's right arm snaps back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your right arm snaps back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_ARM);
    }
    else if (IS_LEG_L(victim,BROKEN_LEG) && !IS_LEG_L(victim,LOST_LEG))
    {
	act("$n's left leg snaps back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your left leg snaps back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_LEG_L],BROKEN_LEG);
    }
    else if (IS_LEG_R(victim,BROKEN_LEG) && !IS_LEG_R(victim,LOST_LEG))
    {
	act("$n's right leg snaps back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your right leg snaps back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_LEG_R],BROKEN_LEG);
    }
    else if (IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_THUMB))
    {
	act("$n's left thumb snaps back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your left thumb snaps back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(victim,BROKEN_FINGER_I) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_FINGER_I))
    {
	act("$n's left index finger snaps back into place.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("Your left index finger snaps back into place.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(victim,BROKEN_FINGER_M) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_FINGER_M))
    {
	act("$n's left middle finger snaps back into place.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("Your left middle finger snaps back into place.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(victim,BROKEN_FINGER_R) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_FINGER_R))
    {
	act("$n's left ring finger snaps back into place.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("Your left ring finger snaps back into place.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(victim,BROKEN_FINGER_L) && !IS_ARM_L(victim,LOST_ARM)
	&& !IS_ARM_L(victim,LOST_HAND) && !IS_ARM_L(victim,LOST_FINGER_L))
    {
	act("$n's left little finger snaps back into place.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("Your left little finger snaps back into place.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_THUMB))
    {
	act("$n's right thumb snaps back into place.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("Your right thumb snaps back into place.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(victim,BROKEN_FINGER_I) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_FINGER_I))
    {
	act("$n's right index finger snaps back into place.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("Your right index finger snaps back into place.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(victim,BROKEN_FINGER_M) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_FINGER_M))
    {
	act("$n's right middle finger snaps back into place.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("Your right middle finger snaps back into place.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(victim,BROKEN_FINGER_R) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_FINGER_R))
    {
	act("$n's right ring finger snaps back into place.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("Your right ring finger snaps back into place.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if (IS_ARM_R(victim,BROKEN_FINGER_L) && !IS_ARM_R(victim,LOST_ARM)
	&& !IS_ARM_R(victim,LOST_HAND) && !IS_ARM_R(victim,LOST_FINGER_L))
    {
	act("$n's right little finger snaps back into place.",victim,NULL,NULL,TO_ROOM, 
FALSE);
	act("Your right little finger snaps back into place.",victim,NULL,NULL,TO_CHAR, 
FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if (IS_BODY(victim,CUT_THROAT))
    {
    	if (IS_SET(victim->loc_hp[6], BLEEDING_THROAT))
    	{
	    send_to_char( "But their throat is still bleeding!\n\r", ch );
	    return;
    	}
	act("The wound in $n's throat closes up.",victim,NULL,NULL,TO_ROOM, FALSE);
	act("The wound in your throat closes up.",victim,NULL,NULL,TO_CHAR, FALSE);
	REMOVE_BIT(victim->loc_hp[LOC_BODY],CUT_THROAT);
    }
    else send_to_char("They have no bones to mend.\n\r",ch);
    if (!IS_NPC(ch) && ch != victim) do_humanity(ch,"");
    return;
}

void spell_quest( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;

    if (ch->practice < 1)
    {
	send_to_char( "It costs at least 1 point of primal energy to create a quest card.\n\r", ch );
	return;
    }

    if ( (pObjIndex = get_obj_index( OBJ_VNUM_QUESTCARD )) == NULL)
    {
	send_to_char("Missing object, please inform KaVir.\n\r",ch);
	return;
    }
    if (ch->in_room == NULL) return;
    obj = create_object(pObjIndex, 0);
    obj_to_char(obj, ch);
    quest_object(ch,obj);
    if (ch->practice >= 300) {ch->practice -= 300; obj->level = 300;}
    else {obj->level = ch->practice; ch->practice = 0;}
    act("$p fades into existance in your hands.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$p fades into existance in $n's hands.",ch,obj,NULL,TO_ROOM, FALSE);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(ch->name);
    return;
}

void spell_minor_creation( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char itemkind[10];
    int itemtype;

    target_name = one_argument( target_name, arg );

    if      (!str_cmp(arg,"potion")) {itemtype = ITEM_POTION;sprintf(itemkind,"potion");}
    else if (!str_cmp(arg,"scroll")) {itemtype = ITEM_SCROLL;sprintf(itemkind,"scroll");}
    else if (!str_cmp(arg,"wand"  )) {itemtype = ITEM_WAND;sprintf(itemkind,"wand");}
    else if (!str_cmp(arg,"staff" )) {itemtype = ITEM_STAFF;sprintf(itemkind,"staff");}
    else if (!str_cmp(arg,"pill"  )) {itemtype = ITEM_PILL;sprintf(itemkind,"pill");}
    else
    {
	send_to_char( "Item can be one of: Potion, Scroll, Wand, Staff or Pill.\n\r", ch );
	return;
    }
    obj = create_object( get_obj_index( OBJ_VNUM_PROTOPLASM ), 0 );
    obj->item_type = itemtype;

    sprintf(buf,"%s %s",ch->name,itemkind);
    free_string(obj->name);
    obj->name=str_dup(buf);
    sprintf(buf,"%s's %s",ch->name, itemkind);
    free_string(obj->short_descr);
    obj->short_descr=str_dup(buf);
    sprintf(buf,"%s's %s lies here.",ch->name,itemkind);
    free_string(obj->description);
    obj->description=str_dup(buf);

    obj->weight = 10;

    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker=str_dup(ch->name);

    obj_to_char( obj, ch );
    act( "$p suddenly appears in your hands.", ch, obj, NULL, TO_CHAR, FALSE );
    act( "$p suddenly appears in $n's hands.", ch, obj, NULL, TO_ROOM, FALSE );
    return;
}

void spell_brew( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to brew, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->pcdata->learned[sn] < 100 )
    {
	sprintf(buf,"You have NOT finished practicing %s!\n\r",skill_table[sn].name);
	stc(buf,ch);
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
	send_to_char( "That is not a potion.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an empty potion bottle.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = ch->spl[0]/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = ch->spl[1]/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = ch->spl[2]/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = ch->spl[3]/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = ch->spl[4]/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = sn;
    if (obj->value[0] >= 25) obj->value[2] = sn; else obj->value[2] = -1;
    if (obj->value[0] >= 50) obj->value[3] = sn; else obj->value[3] = -1;
    free_string(obj->name);
    sprintf(buf,"%s potion %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s potion of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s potion is lying here.",col);
    obj->description=str_dup(buf);
    act("You brew $p.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$n brews $p.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}

void spell_scribe( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to scribe, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }
    
    if (!str_cmp(arg2, "voodoo"))
    return;
    if ( ch->pcdata->learned[sn] < 100 )
    {
        sprintf(buf,"You have NOT finished practicing %s!\n\r",skill_table[sn].name);
        stc(buf,ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_SCROLL )
    {
	send_to_char( "That is not a scroll.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an empty scroll parchment.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = ch->spl[0]/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = ch->spl[1]/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = ch->spl[2]/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = ch->spl[3]/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = ch->spl[4]/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = sn;
    if (obj->value[0] >= 25) obj->value[2] = sn; else obj->value[2] = -1;
    if (obj->value[0] >= 50) obj->value[3] = sn; else obj->value[3] = -1;
    free_string(obj->name);
    sprintf(buf,"%s scroll %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s scroll of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s scroll is lying here.",col);
    obj->description=str_dup(buf);
    act("You scribe $p.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$n scribes $p.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}

void spell_carve( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to carve, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->pcdata->learned[sn] < 100 )
    {
        sprintf(buf,"You have NOT finished practicing %s!\n\r",skill_table[sn].name);
        stc(buf,ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_WAND )
    {
	send_to_char( "That is not a wand.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an unenchanted wand.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = ch->spl[0]/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = ch->spl[1]/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = ch->spl[2]/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = ch->spl[3]/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = ch->spl[4]/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = (obj->value[0] / 5) + 1;
    obj->value[2] = (obj->value[0] / 5) + 1;
    obj->value[3] = sn;
    free_string(obj->name);
    sprintf(buf,"%s wand %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s wand of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s wand is lying here.",col);
    obj->description=str_dup(buf);
    obj->wear_flags = ITEM_TAKE + ITEM_HOLD;
    act("You carve $p.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$n carves $p.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}

void spell_engrave( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to engrave, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->pcdata->learned[sn] < 100 )
    {
        sprintf(buf,"You have NOT finished practicing %s!\n\r",skill_table[sn].name);
        stc(buf,ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_STAFF )
    {
	send_to_char( "That is not a staff.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an unenchanted staff.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = (ch->spl[0]+1)/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = (ch->spl[1]+1)/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = (ch->spl[2]+1)/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = (ch->spl[3]+1)/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = (ch->spl[4]+1)/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = (obj->value[0] / 10) + 1;
    obj->value[2] = (obj->value[0] / 10) + 1;
    obj->value[3] = sn;
    free_string(obj->name);
    sprintf(buf,"%s staff %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s staff of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s staff is lying here.",col);
    obj->description=str_dup(buf);
    obj->wear_flags = ITEM_TAKE + ITEM_HOLD;
    act("You engrave $p.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$n engraves $p.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}

void spell_bake( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char col[10];
    OBJ_DATA *obj;

    target_name = one_argument( target_name, arg1 );
    target_name = one_argument( target_name, arg2 );

    if (IS_NPC(ch)) return;
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "What spell do you wish to bake, and on what?\n\r", ch );
        return;
    }

    if ( ( sn = skill_lookup( arg2 ) ) < 0
    || ( !IS_NPC(ch) && ch->level < skill_table[sn].skill_level ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->pcdata->learned[sn] < 100 )
    {
        sprintf(buf,"You have NOT finished practicing %s!\n\r",skill_table[sn].name);
        stc(buf,ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_PILL )
    {
	send_to_char( "That is not a pill.\n\r", ch );
	return;
    }

    if ( obj->value[0] != 0 || obj->value[1] != 0 ||
    	 obj->value[2] != 0 || obj->value[3] != 0 )
    {
	send_to_char( "You need an unused pill.\n\r", ch );
	return;
    }
    if      ( skill_table[sn].target == 0 )
    	{obj->value[0] = ch->spl[0]/4;sprintf(col,"purple");}
    else if ( skill_table[sn].target == 1 )
    	{obj->value[0] = ch->spl[1]/4;sprintf(col,"red");}
    else if ( skill_table[sn].target == 2 )
    	{obj->value[0] = ch->spl[2]/4;sprintf(col,"blue");}
    else if ( skill_table[sn].target == 3 )
    	{obj->value[0] = ch->spl[3]/4;sprintf(col,"green");}
    else if ( skill_table[sn].target == 4 )
    	{obj->value[0] = ch->spl[4]/4;sprintf(col,"yellow");}
    else
    {
	send_to_char( "Oh dear...big bug...please inform KaVir.\n\r", ch );
	return;
    }
    obj->value[1] = sn;
    if (obj->value[0] >= 25) obj->value[2] = sn; else obj->value[2] = -1;
    if (obj->value[0] >= 50) obj->value[3] = sn; else obj->value[3] = -1;
    free_string(obj->name);
    sprintf(buf,"%s pill %s %s",ch->name,col,skill_table[sn].name);
    obj->name=str_dup(buf);
    free_string(obj->short_descr);
    sprintf(buf,"%s's %s pill of %s",ch->name,col,skill_table[sn].name);
    obj->short_descr=str_dup(buf);
    free_string(obj->description);
    sprintf(buf,"A %s pill is lying here.",col);
    obj->description=str_dup(buf);
    obj->weight = 20;
    act("You bake $p.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$n bakes $p.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}
void spell_mount( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;

    if (ch->pcdata->followers > 4)
    {
	send_to_char("Nothing happens.\n\r",ch);
	return;
    }
    ch->pcdata->followers++;

    if (IS_CLASS(ch, CLASS_DEMON))
    {
	victim=create_mobile( get_mob_index(93420));
	victim->level = level*2;
	victim->armor = 0 - (10*level);
	victim->hitroll = level*2;
	victim->damroll = level*2;
	victim->hit = 250*level;
	victim->max_hit = 250*level;
	victim->gold = 0;
	free_string(victim->lord);
	victim->lord = str_dup(ch->name);
	xSET_BIT(victim->act, ACT_NOEXP);
	xSET_BIT(victim->act, ACT_MOUNT);
	char_to_room( victim, ch->in_room );
	act( "$N fades into existance.", ch, NULL, victim, TO_CHAR, FALSE );
	act( "$N fades into existance.", ch, NULL, victim, TO_ROOM, FALSE );
	return;
    }
	else if (IS_CLASS(ch, CLASS_DROW)) {
        victim=create_mobile( get_mob_index( 93300 ) );
        victim->level = level*2;
        victim->armor = 0 - (10*level);
        victim->hitroll = level*2;
        victim->damroll = level*2;
        victim->hit = 250*level;
        victim->gold = 0;
        victim->max_hit = 250*level;
        free_string(victim->lord);
        victim->lord = str_dup(ch->name);
	xSET_BIT(victim->act, ACT_NOEXP);
	xSET_BIT(victim->act, ACT_MOUNT);
        char_to_room( victim, ch->in_room );
        act( "$N fades into existance.", ch, NULL, victim, TO_CHAR , FALSE);
        act( "$N fades into existance.", ch, NULL, victim, TO_ROOM , FALSE);
        return;
}
    victim=create_mobile( get_mob_index( MOB_VNUM_MOUNT ) );
    victim->level = level;
    victim->armor = 0 - (2*level);
    victim->hitroll = level;
    victim->damroll = level;
    victim->hit = 100*level;
    victim->max_hit = 100*level;
        victim->gold = 0;
    free_string(victim->lord);
    victim->lord = str_dup(ch->name);
    SET_BIT(victim->affected_by,AFF_FLYING);
    xSET_BIT(victim->act, ACT_NOEXP);
    if (IS_GOOD(ch))
    {
	free_string(victim->name);
        victim->gold = 0;
	victim->name = str_dup("mount horse pegasus");
	sprintf(buf,"%s's pegasus",ch->name);
	free_string(victim->short_descr);
	victim->short_descr = str_dup(buf);
	free_string(victim->long_descr);
	victim->long_descr = str_dup("A beautiful white pegasus stands here.\n\r");
    }
    else if (IS_NEUTRAL(ch))
    {
	free_string(victim->name);
	victim->name = str_dup("mount griffin");
	sprintf(buf,"%s's griffin",ch->name);
        victim->gold = 0;
	free_string(victim->short_descr);
	victim->short_descr = str_dup(buf);
	free_string(victim->long_descr);
	victim->long_descr = str_dup("A vicious looking griffin stands here.\n\r");
    }
    else
    {
	free_string(victim->name);
	victim->name = str_dup("mount horse nightmare");
	sprintf(buf,"%s's nightmare",ch->name);
        victim->gold = 0;
	free_string(victim->short_descr);
	victim->short_descr = str_dup(buf);
	free_string(victim->long_descr);
	victim->long_descr = str_dup("A large black demonic horse stands here.\n\r");
    }
    char_to_room( victim, ch->in_room );
    act( "$N fades into existance.", ch, NULL, victim, TO_CHAR, FALSE );
    act( "$N fades into existance.", ch, NULL, victim, TO_ROOM, FALSE );
    return;
}


void spell_scan( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found = FALSE;

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->condition < 100 && can_see_obj( ch, obj ) )
	{
	    found = TRUE;
	    act("$p needs repairing.",ch,obj,NULL,TO_CHAR, FALSE);
	}
    }
    if ( !found )
    {
	send_to_char( "None of your equipment needs repairing.\n\r", ch );
	return;
    }
    return;
}

void spell_repair( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found = FALSE;

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->condition < 100 && can_see_obj( ch, obj ) )
	{
	    found = TRUE;
	    obj->condition = 100;
	    act("$p magically repairs itself.",ch,obj,NULL,TO_CHAR, FALSE);
	    act("$p magically repairs itself.",ch,obj,NULL,TO_ROOM, FALSE);
	}
    }
    if ( !found )
    {
	send_to_char( "None of your equipment needs repairing.\n\r", ch );
	return;
    }
    return;
}

void spell_spellproof( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->chobj != NULL )
    {
	send_to_char("Your spell has no affect.\n\r",ch);
	return;
    }

    if ( IS_SET(obj->quest, QUEST_SPELLPROOF) )
    {
	send_to_char( "That item is already resistance to spells.\n\r", ch );
	return;
    }

    SET_BIT(obj->quest, QUEST_SPELLPROOF);
    act("$p shimmers for a moment.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$p shimmers for a moment.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}

void spell_preserve( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if (IS_SET(obj->quest, QUEST_CLONED))
	{
	send_to_char("You cant preserve cloned items.\n\r",ch);
	return;
	}
    if (IS_SET(obj->quest, QUEST_RUNE))
    {
	stc("You cannot preserve rare equipment!\n\r",ch);
	return;
    }
    if (IS_SET(obj->quest, QUEST_RELIC))
    {
        stc("You cannot preserve relics!\n\r",ch);
        return;
    }
    if (IS_SET(obj->quest, QUEST_ARTIFACT))
    {
	stc("You cannot preserve artifacts!\n\r",ch);
	return;
    }
    if ( obj->timer < 1 )
    {
	send_to_char( "That item doesn't require preserving.\n\r", ch );
	return;
    }
    if ( obj->chobj != NULL )
    {
	send_to_char( "You cannot preserve that.\n\r", ch );
	return;
    }
    obj->timer = -1;
    act("$p shimmers for a moment.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$p shimmers for a moment.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}


void enhance_stat( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, int apply_bit, int bonuses, int affect_bit )
{
    AFFECT_DATA af;

    if ( IS_ITEMAFF(victim, ITEMA_REFLECT)&& number_range(1,3) == 1)
    {
	send_to_char( "You are unable to focus your spell.\n\r", ch );
	return;
    }

    if (IS_SET(affect_bit, AFF_WEBBED) && IS_AFFECTED(victim, AFF_WEBBED))
	affect_bit -= AFF_WEBBED;
    else if (IS_SET(affect_bit, AFF_WEBBED) && is_safe(ch,victim))
	affect_bit -= AFF_WEBBED;
    if (IS_SET(affect_bit, AFF_CHARM) && !IS_AFFECTED(victim, AFF_CHARM))
    {
	if (victim->level <= 50 && IS_NPC(victim) )
	{
	    if ( victim->master )
	        stop_follower( victim );
            add_follower( victim, ch );
	}
	else
	{
	    send_to_char("The spell failed.\n\r",ch);
	    return;
	}
    }

    af.type      = sn;
    af.duration  = level;
    af.location  = apply_bit;
    af.modifier  = bonuses;
    af.bitvector = affect_bit;
    affect_to_char( victim, &af );

    return;
}

void spell_chaos_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam = ch->damroll;
	if (IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD)) return;
	if ( saves_spell( level, victim ) ) dam /= 2;
	if (IS_AFFECTED(victim, AFF_SANCTUARY) ) dam /= 2;
	if (!IS_NPC(victim)) dam *= 0.04;
        if (IS_NPC(victim) && dam >= victim->hit) dam = victim->hit-1;
        if (dam==0) return;
	damage( ch, victim, dam, sn );
        return;
}

/*
void spell_make_bag( int sn, int level, CHAR_DATA *ch,void *vo)
{
   static char *headers[]  =  { "corpse of the ", "corpse of The ",
                                "corpse of an ", "corpse of An ",
                                "corpse of a ", "corpse of A ",
                                "corpse of " };
  OBJ_DATA *obj = (OBJ_DATA *) vo;
  char buf[MAX_STRING_LENGTH];
  int i;
  
  if ( obj->item_type != ITEM_CORPSE_NPC && obj->item_type !=ITEM_CORPSE_PC)
  return;

  for (i = 0; i < 7; i++)
    {
    int len = strlen(headers[i]);
    if ( memcmp(obj->short_descr, headers[i], len) == 0)
        {
         sprintf( buf, "bag %s", obj->short_descr+len );
         free_string( obj->name );
         obj->name = str_dup(buf);
         sprintf( buf, "A bag of fine %s hide catches your eye. ",
                 obj->short_descr+len );
         free_string( obj->description );
         obj->description = str_dup( buf );
         sprintf(buf, "bag made from %s hide", obj->short_descr+len);
         free_string( obj->short_descr );
         obj->short_descr = str_dup( buf );

         break;
    }
}
obj->item_type = ITEM_CONTAINER;
obj->wear_flags = ITEM_HOLD|ITEM_TAKE;
obj->timer = 0;
obj->weight = 5;
obj->level = 1;
obj->cost = 100;
obj->value[0] = 200;
obj->value[1] = 1;
obj->value[2] = -1;
obj->pIndexData = get_obj_index( 3373 );

act( "Everyone is jealous of your new $p.", ch, obj, NULL, TO_CHAR, FALSE );
act( "You are jealous of $n's new $p.", ch, obj, NULL, TO_ROOM , FALSE);

send_to_char( "Ok.\n\r", ch );
return;

}
*/
void spell_resistance( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->chobj != NULL )
    {
	send_to_char("Your spell has no affect.\n\r",ch);
	return;
    }
    if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
    {
	send_to_char( "Not on artifacts.\n\r", ch );
	return;
    }
    if ( obj->resistance <= 10 )
    {
	send_to_char( "You cannot make that item any more resistant.\n\r", ch );
	return;
    }
    obj->resistance = 10;
    act("$p sparkles for a moment.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$p sparkles for a moment.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}

void spell_drowfire( int sn, int level, CHAR_DATA *ch, void *vo) {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af1;
	AFFECT_DATA af2;
	AFFECT_DATA af3;

	act("You chant some arcane words on $N.",ch,NULL,victim,TO_CHAR, FALSE);
	act("$n chants some arcane words on $N.",ch,NULL,victim,TO_NOTVICT, FALSE);
	act("$n chants some arcane words on you.",ch,NULL,victim,TO_VICT, FALSE);

	if (IS_AFFECTED(victim, AFF_DROWFIRE)) {
		send_to_char("They are already affected by drowfire.\n\r",ch);
		return;}
	if (is_safe(ch, victim)) return;

	af1.type	= sn;
	af1.location	= APPLY_AC;
	af1.modifier	= 1000;
	af1.duration	= 2;
	af1.bitvector	= AFF_DROWFIRE;
	affect_to_char(victim, &af1);

	af2.type	= sn;
	af2.location	= APPLY_DAMROLL;
	af2.modifier	= -1000;
	af2.duration	= 2;
	af2.bitvector	= AFF_CURSE;
	affect_to_char(victim, &af2);

	af3.type	= sn;
	af3.location	= APPLY_HITROLL;
	af3.modifier	= -1000;
	af3.duration	= 2;
	af3.bitvector	= AFF_BLIND;
	affect_to_char(victim, &af3);

	send_to_char("You are engulfed in drowfire!\n\r", victim);
	act("$n is engulfed in drowfire!",victim,NULL,NULL,TO_ROOM, FALSE);
	return;
}


void spell_web( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    act("#GYou point your finger at#y $N#G and a web flies from your hand!",ch,NULL,victim,TO_CHAR, FALSE);
    act("#y$n#g points $s #Gfinger at #y$N and a web flies from $s hand!#n",ch,NULL,victim,TO_NOTVICT, FALSE);
    act("$n points $s finger at you and a web flies from $s hand!",ch,NULL,victim,TO_VICT, FALSE);

    if ( IS_AFFECTED(victim, AFF_WEBBED) )
    {
	send_to_char( "But they are already webbed!\n\r", ch );
	return;
    }
    if ( IS_AFFECTED(ch, AFF_WEBBED))
    {
	stc("#GYour web fails to penetrate those stuck around you!#n\n\r",ch);
	return;
    }

    if ( is_safe(ch, victim) ) return;

    if ( saves_spell( level, victim ) && victim->position >= POS_FIGHTING )
    {
	send_to_char( "#sYou dodge the web!#n\n\r", victim );
	act("$n dodges the web!",victim,NULL,NULL,TO_ROOM, FALSE);
	return;
    }

    af.type      = sn;
    af.location  = APPLY_AC;
    af.modifier  = 200;
    af.duration  = number_range(1,2);
    af.bitvector = AFF_WEBBED;
    affect_to_char( victim, &af );
    send_to_char( "#PYou are coated in a sticky web!#n\n\r", victim );
    act("$n is coated in a sticky web!",victim,NULL,NULL,TO_ROOM, FALSE);
    return;
}

void spell_clay( int sn, int level, CHAR_DATA *ch, void *vo )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  act("You point your finger at $N and a clay ball flies from your hand!",ch,NULL,victim,TO_CHAR, FALSE);
  act("$n points $s finger at $N and a clay ball flies from $s hand!",ch,NULL,victim,TO_NOTVICT, FALSE);
  act("$n points $s finger at you and a clay ball flies from $s hand!",ch,NULL,victim,TO_VICT, FALSE);
  if (IS_AFFECTED(victim, AFF_WEBBED)) return;
    if ( IS_AFFECTED(ch, AFF_WEBBED))
    {
        stc("#GYour web fails to penetrate those stuck around you!#n\n\r",ch);
        return;
    }

  if ( saves_spell( level, victim ) && victim->position >= POS_FIGHTING )
  {
    send_to_char( "You dodge the clay!\n\r", victim );
    act("$n dodges your clay ball!",victim,NULL,NULL,TO_ROOM, FALSE);
    return;
  } 
  af.type      = sn;
  af.location  = APPLY_AC;
  af.modifier  = 200;
  af.duration  = number_range(1,2);
  af.bitvector = AFF_WEBBED;
  affect_to_char( victim, &af );
  send_to_char( "You are coated in a sticky clay ball!\n\r", victim );
  act("$n is coated in a sticky clay ball!",victim,NULL,NULL,TO_ROOM, FALSE);
  return;
}

void spell_polymorph( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    char buf [MAX_INPUT_LENGTH];

    if (IS_AFFECTED(ch, AFF_POLYMORPH))
    {
	send_to_char("You cannot polymorph from this form.\n\r",ch);
	return;
    }
    if ( ch->position == POS_FIGHTING || is_affected( ch, sn ) )
	return;

    if ( !str_cmp( target_name, "frog" ) )
    {
	if (!IS_NPC(ch) && ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
	act("$n polymorphs into a frog!",ch,NULL,NULL,TO_ROOM, FALSE);
	send_to_char("You polymorph into a frog!\n\r",ch);
	af.type      = sn;
	af.duration  = number_range(3,5);
	af.location  = APPLY_POLY;
	af.modifier  = POLY_FROG;
	af.bitvector = AFF_POLYMORPH;
	affect_to_char( ch, &af );
	sprintf(buf,"%s the frog",ch->name);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
    }
    else if ( !str_cmp( target_name, "fish" ) )
    {
	if (!IS_NPC(ch) && ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
	act("$n polymorphs into a fish!",ch,NULL,NULL,TO_ROOM, FALSE);
	send_to_char("You polymorph into a fish!\n\r",ch);
	af.type      = sn;
	af.duration  = number_range(3,5);
	af.location  = APPLY_POLY;
	af.modifier  = POLY_FISH;
	af.bitvector = AFF_POLYMORPH;
	affect_to_char( ch, &af );
	sprintf(buf,"%s the fish",ch->name);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
    }
    else if ( !str_cmp( target_name, "raven" ) )
    {
	if (!IS_NPC(ch) && ch->stance[0] != -1) do_stance(ch,"");
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");
	act("$n polymorphs into a raven!",ch,NULL,NULL,TO_ROOM, FALSE);
	send_to_char("You polymorph into a raven!\n\r",ch);
	af.type      = sn;
	af.duration  = number_range(3,5);
	af.location  = APPLY_AC;
	af.modifier  = -150;
	if (IS_AFFECTED(ch, AFF_FLYING)) af.bitvector = AFF_POLYMORPH;
	else af.bitvector = AFF_POLYMORPH + AFF_FLYING;
	affect_to_char( ch, &af );
	af.location  = APPLY_POLY;
	af.modifier  = POLY_RAVEN;
	affect_to_char( ch, &af );
	sprintf(buf,"%s the raven",ch->name);
	free_string(ch->morph);
	ch->morph = str_dup(buf);
	return;
    }
    send_to_char( "You can polymorph into a frog, a fish, or an raven.\n\r", ch );
    return;
}
/*
void spell_contraception( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_CONTRACEPTION) )
	return;
    if (victim->sex != SEX_FEMALE) return;
    af.type      = sn;
    af.duration  = 24;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CONTRACEPTION;
    affect_to_char( victim, &af );
    return;
}
*/
void spell_find_familiar( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;

/* kavirpoint 
	send_to_char( "This spell has been temporarily disabled.\n\r", ch );
	return;
*/
    if ( target_name[0] == '\0' )
    {
	send_to_char( "Your familiar can be a frog, a raven, a cat or a dog.\n\r", ch );
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
    {
	send_to_char( "But you already have a familiar!\n\r", ch );
	return;
    }

    if ( !str_cmp( target_name, "frog" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_FROG ) );
	if (victim == NULL)
	{send_to_char("Error - please inform KaVir.\n\r",ch); return;}
    }
    else if ( !str_cmp( target_name, "raven" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_RAVEN ) );
	if (victim == NULL)
	{send_to_char("Error - please inform KaVir.\n\r",ch); return;}
    }
    else if ( !str_cmp( target_name, "cat" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_CAT ) );
	if (victim == NULL)
	{send_to_char("Error - please inform KaVir.\n\r",ch); return;}
    }
    else if ( !str_cmp( target_name, "dog" ) )
    {
	victim = create_mobile( get_mob_index( MOB_VNUM_DOG ) );
	if (victim == NULL)
	{send_to_char("Error - please inform KaVir.\n\r",ch); return;}
    }
    else 
    {
	send_to_char( "Your familiar can be a frog, a raven, a cat or a dog.\n\r", ch );
	return;
    }

    act( "You make a few gestures and $N appears in a swirl of smoke.", ch, NULL, victim, 
TO_CHAR, FALSE );
    act( "$n makes a few gestures and $N appears in a swirl of smoke.", ch, NULL, victim, 
TO_ROOM, FALSE );
    char_to_room( victim, ch->in_room );

    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    return;
}

void spell_improve( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
    {
	send_to_char( "Not on artifacts.\n\r", ch );
	return;
    }
        if (obj->pIndexData->vnum == 420000 || obj->pIndexData->vnum == 220004)
      {
        send_to_char("I don't think the Guardian would like that.\n\r",ch);
        return;
      }

    if ( obj->pIndexData->vnum != OBJ_VNUM_PROTOPLASM )
    {
	send_to_char( "You cannot enhance this object.\n\r", ch );
	return;
    }
    if (IS_SET(obj->quest, QUEST_IMPROVED))
    {
	send_to_char( "This item has already been improved.\n\r", ch );
	return;
    }
    else if ( obj->points < 750 && obj->item_type != ITEM_WEAPON )
    {
	send_to_char( "The object must be worth at least 750 quest points and must be created.\n\r", ch );
	return;
    }
    else if ( obj->points < 1500 && obj->item_type == ITEM_WEAPON )
    {
	send_to_char( "The object must be worth at least 1500 quest points and must be created.\n\r", ch );
	return;
    }
    REMOVE_BIT(obj->quest, QUEST_STR);
    REMOVE_BIT(obj->quest, QUEST_DEX);
    REMOVE_BIT(obj->quest, QUEST_INT);
    REMOVE_BIT(obj->quest, QUEST_WIS);
    REMOVE_BIT(obj->quest, QUEST_CON);
    REMOVE_BIT(obj->quest, QUEST_HITROLL);
    REMOVE_BIT(obj->quest, QUEST_DAMROLL);
    REMOVE_BIT(obj->quest, QUEST_HIT);
    REMOVE_BIT(obj->quest, QUEST_MANA);
    REMOVE_BIT(obj->quest, QUEST_MOVE);
    REMOVE_BIT(obj->quest, QUEST_AC);
    SET_BIT(obj->quest, QUEST_IMPROVED);
    act("$p flickers for a moment.",ch,obj,NULL,TO_CHAR, FALSE);
    act("$p flickers for a moment.",ch,obj,NULL,TO_ROOM, FALSE);
    return;
}
void spell_spiritkiss( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    if (!IS_CLASS(ch, CLASS_MAGE))
    {
      if ( is_affected( victim, sn ) ) return;
      af.type      = sn;
      af.duration  = 2*level;
      af.location  = APPLY_HITROLL;
      af.modifier  = level;
      af.bitvector = 0;
      affect_to_char( victim, &af );

      af.type      = sn;
      af.duration  = 2*level;
      af.location  = APPLY_DAMROLL;
      af.modifier  = level;
      af.bitvector = 0;
      affect_to_char( victim, &af );

      af.location  = APPLY_SAVING_SPELL;
      af.modifier  = 0 - level;
      affect_to_char( victim, &af );
    }
    else if (IS_CLASS(ch, CLASS_MAGE))
    {
      if ( is_affected( victim, sn ) ) return;
      af.type      = sn;
      af.duration  = level;
      af.location  = APPLY_HITROLL;
      af.modifier  = level /3;
      af.bitvector = 0;
      affect_to_char( victim, &af );

      af.type      = sn;
      af.duration  = level;
      af.location  = APPLY_DAMROLL;
      af.modifier  = level /3;
      af.bitvector = 0;
      affect_to_char( victim, &af );

      af.location  = APPLY_SAVING_SPELL;
      af.modifier  = 0 - level;
      affect_to_char( victim, &af );
    }

    act( "$n is filled with spiritual power.", victim, NULL, NULL, TO_ROOM , FALSE);
    send_to_char( "You are blessed by the spirits.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

/*void spell_jailwater( int sn, int level, CHAR_DATA *ch, void *vo )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
    int door;
    ROOM_INDEX_DATA *old_room = ch->in_room;

    if( victim == NULL ) return;
    if( victim->in_room == NULL ) return;

    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    for (door = 0 ; door < 4 ; door++)
    {
	if (ch->in_room->exit[door] != NULL)
	    make_wall(ch->in_room, door, EX_ICE_WALL);
    }

    ch->hit += (ch->hit *3/2);
       if (ch->hit > ch->max_hit)
		ch->hit = ch->max_hit;

    act("$n calls forth sheets of ice, blocking your path in every direction.", ch, NULL, 
NULL, TO_ROOM, FALSE);
    send_to_char("You call forth sheets of ice to entrap your prey.\n\r", ch);
    send_to_char("You grin wickedly.\n\r",ch);
    do_say(ch,"Prepare to die! Muahahahaha.");
    char_from_room(ch);
    char_to_room(ch,old_room);
    
   return;*/
