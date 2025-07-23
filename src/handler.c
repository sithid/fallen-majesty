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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include "merc.h"





/*
 * Local functions.
 */



/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust( CHAR_DATA *ch )
{
    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    if ( ch->trust != 0 )
	return ch->trust;

    if ( IS_NPC(ch) && ch->level >= LEVEL_HERO )
	return LEVEL_HERO - 1;
    else
	return ch->level;
}



/*
 * Retrieve a character's age.
 */
int get_age( CHAR_DATA *ch )
{
    return 17 + ( ch->played + (int) (current_time - ch->logon) ) / 7200;
}



/*
 * Retrieve character's current strength.
 */
int get_curr_str( CHAR_DATA *ch )
{
    int max;

    if ( IS_NPC(ch) )
	return 13;

    max = 25;
    if(IS_CLASS(ch, CLASS_HIGHLANDER) || IS_IMMORTAL(ch) ) max = 35;

    return URANGE( 3, ch->pcdata->perm_str + ch->pcdata->mod_str, max );
}



/*
 * Retrieve character's current intelligence.
 */
int get_curr_int( CHAR_DATA *ch )
{
    int max;

    if ( IS_NPC(ch) )
	return 13;

    max = 25;
    if(IS_CLASS(ch, CLASS_HIGHLANDER) || IS_IMMORTAL(ch) ) max = 35;

    return URANGE( 3, ch->pcdata->perm_int + ch->pcdata->mod_int, max );
}



/*
 * Retrieve character's current wisdom.
 */
int get_curr_wis( CHAR_DATA *ch )
{
    int max = 25;

    if ( IS_NPC(ch) )
	return 13;

    max = 25;
    if(IS_CLASS(ch, CLASS_HIGHLANDER) || IS_IMMORTAL(ch) ) max = 35;

    return URANGE( 3, ch->pcdata->perm_wis + ch->pcdata->mod_wis, max );
}



/*
 * Retrieve character's current dexterity.
 */
int get_curr_dex( CHAR_DATA *ch )
{
    int max;

    if ( IS_NPC(ch) )
	return 13;

    max = 25;
    if(IS_CLASS(ch, CLASS_HIGHLANDER) || IS_IMMORTAL(ch) ) max = 35;

    return URANGE( 3, ch->pcdata->perm_dex + ch->pcdata->mod_dex, max );
}



/*
 * Retrieve character's current constitution.
 */
int get_curr_con( CHAR_DATA *ch )
{
    int max;

    if ( IS_NPC(ch) )
	return 13;

    max = 25;
    if(IS_CLASS(ch, CLASS_HIGHLANDER) || IS_IMMORTAL(ch) ) max = 35;

    return URANGE( 3, ch->pcdata->perm_con + ch->pcdata->mod_con, max );
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 100000;

    if ( IS_NPC(ch) && xIS_SET(ch->act, ACT_PET) )
	return 0;

    return sysdata.max_items;
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 1000000;

    if ( IS_NPC(ch) && xIS_SET(ch->act, ACT_PET) )
	return 0;

    return str_app[get_curr_str(ch)].carry;
}




/*
 * See if a string is one of the names of an object.
 */

bool is_name ( char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
    char *list, *string;

    /* fix crash on NULL namelist */
    if (namelist == NULL || namelist[0] == '\0')
        return FALSE;

    /* fixed to prevent is_name on "" returning TRUE */
    if (str[0] == '\0')
        return FALSE;

    string = str;
    /* we need ALL parts of string to match part of namelist */
    for ( ; ; )  /* start parsing string */
    {
        str = one_argument(str,part);

        if (part[0] == '\0' )
            return TRUE;

        /* check to see if this is part of namelist */
        list = namelist;
        for ( ; ; )  /* start parsing namelist */
        {
            list = one_argument(list,name);
            if (name[0] == '\0')  /* this name was not found */
                return FALSE;

            if (!str_prefix(string,name))
                return TRUE; /* full pattern match */

            if (!str_prefix(part,name))
                break;
        }
    }
}


bool is_full_name( const char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];

    for ( ; ; )
    {
	namelist = one_argument( namelist, name );
	if ( name[0] == '\0' )
	    return FALSE;
	if ( !str_cmp( str, name ) )
	    return TRUE;
    }
}



/*
 * Apply or remove an affect to a character.
 */
void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd, OBJ_DATA *obj )
{
    OBJ_DATA *wield;
    int mod;

    mod = paf->modifier;

    if ( fAdd )
    {
	SET_BIT( ch->affected_by, paf->bitvector );
    }
    else
    {
	REMOVE_BIT( ch->affected_by, paf->bitvector );
	mod = 0 - mod;
    }

    if ( IS_NPC(ch) )
    {
    	switch ( paf->location )
    	{
	    default: break;
	    case APPLY_NONE:						break;
	    case APPLY_MANA:          ch->max_mana		+= mod;	break;
	    case APPLY_HIT:           ch->max_hit		+= mod;	break;
	    case APPLY_MOVE:          ch->max_move		+= mod;	break;
	    case APPLY_AC:            ch->armor			+= mod;	break;
	    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
	    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
	    case APPLY_SAVING_PARA:   ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
	    case APPLY_POLY:	      ch->polyaff		+= mod;	break;
	}
	return;
    }
 if( !IS_NPC(ch) && obj != NULL && IS_CLASS(ch, CLASS_HIGHLANDER))
  {
   if ( obj->pIndexData->vnum == 33177  || IS_SET(obj->quest, QUEST_ARTIFACT) || CAN_WEAR( obj, ITEM_WEAR_MASTERY ) ||
IS_SET(obj->quest,QUEST_RELIC) || IS_SET(obj->quest,QUEST_RUNE) || IS_SET(obj->quest, 
QUEST_NEWBIE) || IS_SET(obj->quest, QUEST_ADAMANTIUM) )
 	   {
    switch ( paf->location )
    {
    default:
	bug( "Affect_modify: unknown location %d.", paf->location );
	return;

    case APPLY_NONE:						break;
    case APPLY_STR:           ch->pcdata->mod_str	+= mod;	break;
    case APPLY_DEX:           ch->pcdata->mod_dex	+= mod;	break;
    case APPLY_INT:           ch->pcdata->mod_int	+= mod;	break;
    case APPLY_WIS:           ch->pcdata->mod_wis	+= mod;	break;
    case APPLY_CON:           ch->pcdata->mod_con	+= mod;	break;
/*
    case APPLY_SEX:           ch->sex			+= mod;	break;
*/
    case APPLY_SEX:						break;
    case APPLY_CLASS:						break;
    case APPLY_LEVEL:						break;
    case APPLY_AGE:						break;
    case APPLY_HEIGHT:						break;
    case APPLY_WEIGHT:						break;
    case APPLY_MANA:          ch->max_mana		+= mod;	break;
    case APPLY_HIT:           ch->max_hit		+= mod;	break;
    case APPLY_MOVE:          ch->max_move		+= mod;	break;
    case APPLY_GOLD:						break;
    case APPLY_EXP:						break;
    case APPLY_AC:            ch->armor			+= mod; break;
    case APPLY_HITROLL:       ch->hitroll		+= mod; break;
    case APPLY_DAMROLL:       ch->damroll		+= mod; break;
    case APPLY_SAVING_PARA:   ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
    case APPLY_POLY:	      ch->polyaff		+= mod;	break;
    }
	return;
    }
    }
    if (!IS_CLASS(ch, CLASS_HIGHLANDER))
    {
    switch ( paf->location )
    {
    default:
	bug( "Affect_modify: unknown location %d.", paf->location );
	return;

    case APPLY_NONE:						break;
    case APPLY_STR:           ch->pcdata->mod_str	+= mod;	break;
    case APPLY_DEX:           ch->pcdata->mod_dex	+= mod;	break;
    case APPLY_INT:           ch->pcdata->mod_int	+= mod;	break;
    case APPLY_WIS:           ch->pcdata->mod_wis	+= mod;	break;
    case APPLY_CON:           ch->pcdata->mod_con	+= mod;	break;
    case APPLY_SEX:						break;
    case APPLY_CLASS:						break;
    case APPLY_LEVEL:						break;
    case APPLY_AGE:						break;
    case APPLY_HEIGHT:						break;
    case APPLY_WEIGHT:						break;
    case APPLY_MANA:          ch->max_mana		+= mod;	break;
    case APPLY_HIT:           ch->max_hit		+= mod;	break;
    case APPLY_MOVE:          ch->max_move		+= mod;	break;
    case APPLY_GOLD:						break;
    case APPLY_EXP:						break;
    case APPLY_AC:            ch->armor			+= mod;	break;
    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
    case APPLY_SAVING_PARA:   ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
    case APPLY_POLY:	      ch->polyaff		+= mod;	break;
    }
   }

    /*
     * Check for weapon wielding.
     * Guard against recursion (for weapons with affects).
     */
    if ( ( wield = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   wield->item_type == ITEM_WEAPON
    &&   get_obj_weight(wield) > str_app[get_curr_str(ch)].wield )
    {
	static int depth;

	if ( depth == 0 )
	{
	    depth++;
	    act( "You drop $p.", ch, wield, NULL, TO_CHAR, FALSE );
	    act( "$n drops $p.", ch, wield, NULL, TO_ROOM, FALSE );
	    obj_from_char( wield );
	    obj_to_room( wield, ch->in_room );
	    depth--;
	}
    }

    return;
}

/*
 * Remove an exit from a room					-Thoric
 */
void extract_exit( ROOM_INDEX_DATA *room, EXIT_DATA *pexit )
{
    UNLINK( pexit, room->first_exit, room->last_exit, next, prev );
    if ( pexit->rexit )
      pexit->rexit->rexit = NULL;
    STRFREE( pexit->keyword );
    STRFREE( pexit->description );
    DISPOSE( pexit );
}


void set_learnable_disciplines (CHAR_DATA *ch)
{
    /* Put the extra disciplines in here, by race */
    if (IS_CLASS(ch, CLASS_VAMPIRE))
    {
	if( ch->power[DISC_VAMP_FORT] < 0 )
            ch->power[DISC_VAMP_FORT] = 0;
	if( ch->power[DISC_VAMP_CELE] < 0 )
        ch->power[DISC_VAMP_CELE] = 0;
	if( ch->power[DISC_VAMP_OBTE] < 0 )
        ch->power[DISC_VAMP_OBTE] = 0;
	if( ch->power[DISC_VAMP_PRES] < 0 )
        ch->power[DISC_VAMP_PRES] = 0;
	if( ch->power[DISC_VAMP_QUIE] < 0 )
        ch->power[DISC_VAMP_QUIE] = 0;
	if( ch->power[DISC_VAMP_THAU] < 0 )
        ch->power[DISC_VAMP_THAU] = 0;
	if( ch->power[DISC_VAMP_AUSP] < 0 )
        ch->power[DISC_VAMP_AUSP] = 0;
	if( ch->power[DISC_VAMP_DOMI] < 0 )
        ch->power[DISC_VAMP_DOMI] = 0;
	if( ch->power[DISC_VAMP_OBFU] < 0 )
        ch->power[DISC_VAMP_OBFU] = 0;
	if( ch->power[DISC_VAMP_POTE] < 0 )
        ch->power[DISC_VAMP_POTE] = 0;
	if( ch->power[DISC_VAMP_PROT] < 0 )
        ch->power[DISC_VAMP_PROT] = 0;
	if( ch->power[DISC_VAMP_SERP] < 0 )
        ch->power[DISC_VAMP_SERP] = 0;
	if( ch->power[DISC_VAMP_VICI] < 0 )
        ch->power[DISC_VAMP_VICI] = 0;
	if( ch->power[DISC_VAMP_DAIM] < 0 )
        ch->power[DISC_VAMP_DAIM] = 0;
	if( ch->power[DISC_VAMP_ANIM] < 0 )
        ch->power[DISC_VAMP_ANIM] = 0;
	if( ch->power[DISC_VAMP_CHIM] < 0 )
	ch->power[DISC_VAMP_CHIM] = 0;
	if( ch->power[DISC_VAMP_MELP] < 0 )
	ch->power[DISC_VAMP_MELP] = 0;
	if( ch->power[DISC_VAMP_NECR] < 0 )
	ch->power[DISC_VAMP_NECR] = 0;
	if( ch->power[DISC_VAMP_THAN] < 0 )
	ch->power[DISC_VAMP_THAN] = 0;
	if( ch->power[DISC_VAMP_OBEA] < 0 )
	ch->power[DISC_VAMP_OBEA] = 0;
    }
    if (IS_CLASS(ch, CLASS_WEREWOLF))
    {
	if( ch->power[DISC_WERE_BEAR] < 0 )
        ch->power[DISC_WERE_BEAR] = 0;
	if( ch->power[DISC_WERE_LYNX] < 0 )
        ch->power[DISC_WERE_LYNX] = 0;
	if( ch->power[DISC_WERE_BOAR] < 0 )
        ch->power[DISC_WERE_BOAR] = 0;
	if( ch->power[DISC_WERE_OWL] < 0 )
        ch->power[DISC_WERE_OWL]  = 0;
	if( ch->power[DISC_WERE_SPID] < 0 )
        ch->power[DISC_WERE_SPID] = 0;
	if( ch->power[DISC_WERE_WOLF] < 0 )
        ch->power[DISC_WERE_WOLF] = 0;
	if( ch->power[DISC_WERE_HAWK] < 0 )
        ch->power[DISC_WERE_HAWK] = 0;
	if( ch->power[DISC_WERE_MANT] < 0 )
        ch->power[DISC_WERE_MANT] = 0;
	if( ch->power[DISC_WERE_RAPT] < 0 )
        ch->power[DISC_WERE_RAPT] = 0;
	if( ch->power[DISC_WERE_LUNA] < 0 )
        ch->power[DISC_WERE_LUNA] = 0;
	if( ch->power[DISC_WERE_PAIN] < 0 )
        ch->power[DISC_WERE_PAIN] = 0;
	if( ch->power[DISC_WERE_CONG] < 0 )
        ch->power[DISC_WERE_CONG] = 0;
    }
  

    if (IS_CLASS(ch, CLASS_DEMON))
    {
	if( ch->power[DISC_DAEM_ATTA] < 0 )
	ch->power[DISC_DAEM_ATTA] = 0;
	if( ch->power[DISC_DAEM_IMMU] < 0 )
	ch->power[DISC_DAEM_IMMU] = 0;
	if( ch->power[DISC_DAEM_TEMP] < 0 )
	ch->power[DISC_DAEM_TEMP] = 0;
	if( ch->power[DISC_DAEM_CORR] < 0 )
	ch->power[DISC_DAEM_CORR] = 0;
	if( ch->power[DISC_DAEM_HELL] < 0 )
	ch->power[DISC_DAEM_HELL] = 0;
	if( ch->power[DISC_DAEM_GELU] < 0 )
	ch->power[DISC_DAEM_GELU] = 0;
	if( ch->power[DISC_DAEM_MORP] < 0 )
	ch->power[DISC_DAEM_MORP] = 0;
	if( ch->power[DISC_DAEM_NETH] < 0 )
	ch->power[DISC_DAEM_NETH] = 0;
	if( ch->power[DISC_DAEM_DISC] < 0 )
	ch->power[DISC_DAEM_DISC] = 0;
    }

    save_char_obj(ch);
}



/*
 * Give an affect to a char.
 */
void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    CREATE( paf_new, AFFECT_DATA, 1);
    LINK( paf_new, ch->first_affect, ch->last_affect, next, prev );

    paf_new->type       = paf->type;
    paf_new->duration	= paf->duration;
    paf_new->location	= paf->location;
    paf_new->modifier	= paf->modifier;
    paf_new->bitvector	= paf->bitvector;

    affect_modify( ch, paf_new, TRUE, NULL );
    return;
}

/*NEW, added by Adjani*/
  void affect_to_obj(OBJ_DATA *obj, AFFECT_DATA *paf)
  {
    AFFECT_DATA *paf_new;

    CREATE( paf_new, AFFECT_DATA, 1 );

    LINK( paf_new, obj->first_affect, obj->last_affect, next, prev );
    paf_new->duration	= paf->duration;
    paf_new->location	= paf->location;
    paf_new->modifier	= paf->modifier;
    paf_new->bitvector	= paf->bitvector;
    return;
  }
/*end*/


/*
 * Remove an affect from a char.
 */
void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    if ( ch->first_affect == NULL )
    {
	bug( "Affect_remove: no affect.", 0 );
	return;
    }

    affect_modify( ch, paf, FALSE, NULL );

    UNLINK( paf, ch->first_affect, ch->last_affect, next, prev );

    DISPOSE( paf );
    return;
}



/*
 * Strip all affects of a given sn.
 */
void affect_strip( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = ch->first_affect; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove( ch, paf );
    }

    return;
}



/*
 * Return true if a char is affected by a spell.
 */
bool is_affected( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = ch->first_affect; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_old;
    bool found;

    found = FALSE;
    for ( paf_old = ch->first_affect; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
	    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove( ch, paf_old );
	    break;
	}
    }

    affect_to_char( ch, paf );
    return;
}



/*
 * Move a char out of a room.
 */
void char_from_room( CHAR_DATA *ch )
{
    OBJ_DATA *obj;

    if ( ch->in_room == NULL )
    {
	bug( "Char_from_room: NULL.", 0 );
	return;
    }


    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;
    else if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;

    if ( ch == ch->in_room->people )
    {
	ch->in_room->people = ch->next_in_room;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
	{
	    if ( prev->next_in_room == ch )
	    {
		prev->next_in_room = ch->next_in_room;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Char_from_room: ch not found.", 0 );
    }

    ch->in_room      = NULL;
    ch->next_in_room = NULL;
    return;
}


void repop_char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    OBJ_DATA *obj;

    if ( !pRoomIndex )
    {
	char buf[MAX_STRING_LENGTH];

	sprintf( buf, "Char_to_room: %s -> NULL room!  Putting char in limbo (%d)", ch->name, ROOM_VNUM_LIMBO );
	bug( buf, 0 );
        /* This used to just return, but there was a problem with crashing
           and I saw no reason not to just put the char in limbo. -Narn */
        pRoomIndex = get_room_index( ROOM_VNUM_LIMBO );
    }

    ch->in_room		= pRoomIndex;
    ch->next_in_room	= pRoomIndex->people;
    pRoomIndex->people	= ch;



    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0 )
	++ch->in_room->light;
    else if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0 )
	++ch->in_room->light;

    if (ch->loc_hp[6] > 0 && ch->in_room->blood < 1000) ch->in_room->blood += 1;
    if (IS_NPC(ch))
       act( "$n pops into existance.",  ch, NULL, NULL, TO_ROOM, FALSE );

    return;
}
/*
 * Move a char into a room.
 */
void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    OBJ_DATA *obj;

    if ( !pRoomIndex )
    {
	char buf[MAX_STRING_LENGTH];

	sprintf( buf, "Char_to_room: %s -> NULL room!  Putting char in limbo (%d)", ch->name, ROOM_VNUM_LIMBO );
	bug( buf, 0 );
        /* This used to just return, but there was a problem with crashing
           and I saw no reason not to just put the char in limbo. -Narn */
        pRoomIndex = get_room_index( ROOM_VNUM_LIMBO );
    }

    ch->in_room		= pRoomIndex;
    ch->next_in_room	= pRoomIndex->people;
    pRoomIndex->people	= ch;



    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0 )
	++ch->in_room->light;
    else if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0 )
	++ch->in_room->light;

    if (ch->loc_hp[6] > 0 && ch->in_room->blood < 1000) ch->in_room->blood += 1;
    return;
}



/*
 * Give an obj to a char.
 */
void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )
{
    if (obj == NULL) return;
    obj->next_content	 = ch->carrying;
    ch->carrying	 = obj;
    obj->carried_by	 = ch;
    obj->in_room	 = NULL;
    obj->in_obj		 = NULL;
    ch->carry_number	+= 1;
    if (obj->contains != NULL)
       add_contents( obj->contains, ch);
    ch->carry_weight	+= get_obj_weight( obj );
}



/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA *obj )
{
    CHAR_DATA *ch;

    if (obj == NULL) return;
    if ( ( ch = obj->carried_by ) == NULL )
    {
	log_string( "Obj_from_char: %s, not being carried.", obj->short_descr);
        if ( obj->in_room != NULL )
	   log_string( "               on floor in room %d.", obj->in_room->vnum );
	else if ( obj->in_obj != NULL )
	   log_string( "               in object %s.", obj->in_obj->short_descr );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
	unequip_char( ch, obj );

    if ( ch->carrying == obj )
    {
	ch->carrying = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	  
  log_string( "Obj_from_char: obj not in list.");
    }

    obj->carried_by	 = NULL;
    obj->next_content	 = NULL;
    ch->carry_number	-= 1;
    if (obj->contains != NULL)
       subtract_contents( obj->contains, ch);
    ch->carry_weight	-= get_obj_weight( obj );
    return;
}



/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac( OBJ_DATA *obj, int iWear )
{
    if ( obj->item_type != ITEM_ARMOR )
	return 0;

    switch ( iWear )
    {
    case WEAR_BODY:	return 3 * obj->value[0];
    case WEAR_HEAD:	return 2 * obj->value[0];
    case WEAR_LEGS:	return 2 * obj->value[0];
    case WEAR_FEET:	return     obj->value[0];
    case WEAR_HANDS:	return     obj->value[0];
    case WEAR_ARMS:	return     obj->value[0];
    case WEAR_SHIELD:	return     obj->value[0];
    case WEAR_FINGER_L:	return     obj->value[0];
    case WEAR_FINGER_R: return     obj->value[0];
    case WEAR_NECK_1:	return     obj->value[0];
    case WEAR_NECK_2:	return     obj->value[0];
    case WEAR_ABOUT:	return 2 * obj->value[0];
    case WEAR_WAIST:	return     obj->value[0];
    case WEAR_WRIST_L:	return     obj->value[0];
    case WEAR_WRIST_R:	return     obj->value[0];
    case WEAR_HOLD:	return     obj->value[0];
    case WEAR_FACE:	return     obj->value[0];
    case WEAR_MASTERY:	return     obj->value[0];
    case WEAR_TATTOO:   return	   obj->value[0];
    case WEAR_EAR_LEFT: return     obj->value[0];
    case WEAR_EAR_RIGHT: return     obj->value[0];
    case WEAR_SCABBARD_L:return 0;
    case WEAR_SCABBARD_R:return 0;
    }

    return 0;
}



/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == iWear )
	    return obj;
    }

    return NULL;
}



/*
 * Equip a char with an obj.
 */
void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
    CHAR_DATA   *chch;
    AFFECT_DATA *paf;
    int sn;

    if (obj->item_type == ITEM_ARMOR)
      sn = obj->value[3];
    else
      sn = obj->value[0] / 1000;
//    if (((get_age(ch)-17)<4) && IS_SET(obj->quest, QUEST_ARTIFACT) ) 
    if (IS_SET(ch->special, SPC_NEWBIE) && IS_SET(obj->quest, QUEST_ARTIFACT))
    {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );
	stc("Newbies with artifacts? Yeah right.\n\r",ch);
        obj_from_char( obj );
        obj_to_room( obj, ch->in_room );
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
	return;
    }
    if (IS_SET(ch->special, SPC_NEWBIE) && IS_SET(obj->quest, QUEST_RUNE))
    {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );
        stc("Newbies with rares? Yeah right.\n\r",ch);
        obj_from_char( obj );
        obj_to_room( obj, ch->in_room );
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
    }
    if ((sn == 7 || sn == 54) && !IS_GOOD(ch))
    {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );
        obj_from_char( obj );
        obj_to_room( obj, ch->in_room );
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
    }
    if (sn == 139 && !IS_EVIL(ch))
    {   
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char( obj );
        obj_to_room( obj, ch->in_room );
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
    }    

    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
    {
	/*
	 * Thanks to Morgenes for the bug fix here!
	 */
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );	
obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
	return;
    }

        /* Mage EQ */
        
        if (IS_SET(obj->spectype, SITEM_MAGE))
        if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_MAGE)) {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char( obj );
        obj_to_room( obj, ch->in_room );
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }


        /* Drow EQ */

	if (IS_SET(obj->spectype, SITEM_DROW))
	if (IS_NPC(ch) || !IS_CLASS(ch, CLASS_DROW) ) {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );	
obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
	return;
	} 

        /* Monk EQ */
        
        if ((obj->pIndexData->vnum >= 33020 && obj->pIndexData->vnum <= 33039)
         && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_MONK)  )) {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }

        /* vampire EQ */
        
        if ((obj->pIndexData->vnum >= 33040 && obj->pIndexData->vnum <= 33059)
         && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_VAMPIRE) ) ) 
	{
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }

        /* Angel EQ */
        
        if ((obj->pIndexData->vnum >= 33180 && obj->pIndexData->vnum <= 33199)
         && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_ANGEL))) {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }

        /* Tanar'ri EQ */

        if ((obj->pIndexData->vnum >= 33200 && obj->pIndexData->vnum <= 33219)
         && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_TANARRI)) ) 
{
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }

        /* Lich EQ */

        if ((obj->pIndexData->vnum >= 33220 && obj->pIndexData->vnum <= 33240)
         && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_POWER_LICH)  ) ){
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;  
        }


        /* Ninja EQ */

	if ((obj->pIndexData->vnum >= 33080 && obj->pIndexData->vnum <= 33099)
         && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_NINJA)) ) {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );	
obj_from_char(obj);
	obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
	return;
	}

        if ((obj->pIndexData->vnum >= 29740 && obj->pIndexData->vnum <= 29750)
         && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_NINJA)) ) {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }


        /* Shapeshifter EQ */

        if (((obj->pIndexData->vnum >= 33160 && obj->pIndexData->vnum <= 33175))
         && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_SHAPESHIFTER) ) ){
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }

       if ((obj->pIndexData->vnum == 33176 || obj->pIndexData->vnum == 33177) && (IS_NPC(ch) 
|| !IS_CLASS(ch,CLASS_HIGHLANDER) ) ){
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }

       if ((obj->pIndexData->vnum == 33342 ) && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_PALADIN) ) ){
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }
       if ((obj->pIndexData->vnum == 33341 ) && (IS_NPC(ch) || !IS_CLASS(ch,CLASS_INQUISITOR) ) ){
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }

       if ((obj->pIndexData->vnum >= 33343 && obj->pIndexData->vnum <= 33356) && 
(IS_NPC(ch) || !IS_CLASS(ch,CLASS_PSIONIST) ) ){
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
        return;
        }



    /* Demon EQ */

    if ((IS_NPC(ch) || !IS_CLASS(ch,  CLASS_DEMON)) && IS_SET(obj->spectype, SITEM_DEMONIC) )
    {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );	
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
        if ( IS_SET( sysdata.save_flags, SV_ZAPDROP ) )
  	   save_char_obj( ch );
	return;
    }
    
    /* Spider Droid Eq */

    if(obj->pIndexData->vnum >= 33140 && obj->pIndexData->vnum <= 33159 && 
!IS_CLASS(ch,CLASS_CYBORG)  )
    {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char( obj );
        obj_to_room( obj, ch->in_room );
        if( IS_SET(sysdata.save_flags, SV_ZAPDROP)) save_char_obj(ch);
        return;
    }

    /* Death Knight EQ */

    if(obj->pIndexData->vnum >= 29975 && obj->pIndexData->vnum <= 29991 && !IS_CLASS(ch,CLASS_UNDEAD_KNIGHT) )
    {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );        
obj_from_char( obj );
        obj_to_room( obj, ch->in_room ); 
        if( IS_SET(sysdata.save_flags, SV_ZAPDROP)) save_char_obj(ch);
        return;
    }


    /* Wolf EQ */

    if ((IS_SET(obj->spectype,SITEM_WOLFWEAPON) || (obj->pIndexData->vnum >=33100 && obj->pIndexData->vnum <=33119))
	&& !IS_CLASS(ch, CLASS_WEREWOLF) )
    {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );	
obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	if( IS_SET(sysdata.save_flags, SV_ZAPDROP)) save_char_obj(ch);
	return;
    }

    if (IS_NPC(ch) && (IS_SET(obj->quest,QUEST_RELIC) || IS_SET(obj->quest,QUEST_RUNE)))
    {
        act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR, FALSE );
        act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM, FALSE );	
obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	if( IS_SET(sysdata.save_flags, SV_ZAPDROP)) save_char_obj(ch);
	return;
    }

    if ( iWear == WEAR_SCABBARD_L ||
    	iWear == WEAR_SCABBARD_R ) 
    {
	obj->wear_loc = iWear;
	return;
    }

    if ( IS_NPC(ch) || !IS_CLASS(ch, CLASS_HIGHLANDER) || (IS_CLASS(ch, CLASS_HIGHLANDER)
	 && obj->pIndexData->vnum == 29694))
	ch->armor -= apply_ac( obj, iWear );
    obj->wear_loc = iWear;

    for ( paf = obj->pIndexData->first_affect; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, TRUE, obj );
    for ( paf = obj->first_affect; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, TRUE, obj );

    if ( obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room != NULL )
	++ch->in_room->light;

    if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_DEMON))
    {
    	if ( (chch = get_char_world(ch,ch->name)) == NULL ) return;
    	if ( chch->desc != ch->desc) return;
    }

    if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	&& str_cmp(obj->chpoweron,"(null)")
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	{
	    kavitem(obj->chpoweron,ch,obj,NULL,TO_CHAR);
	    if (IS_SET(obj->spectype, SITEM_ACTION))
		kavitem(obj->chpoweron,ch,obj,NULL,TO_ROOM);
	}
    if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	&& str_cmp(obj->victpoweron,"(null)")
	&& !IS_SET(obj->spectype, SITEM_ACTION)
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	kavitem(obj->victpoweron,ch,obj,NULL,TO_ROOM);

    if ( obj->wear_loc == WEAR_NONE )
	return;
    if (   ((obj->item_type == ITEM_ARMOR ) && (obj->value[3] >= 1   ))
	|| ((obj->item_type == ITEM_WEAPON) && (obj->value[0] >= 1000))
	|| IS_SET(obj->spectype, SITEM_SILVER)
	|| IS_SET(obj->spectype, SITEM_DEMONIC)
	|| IS_SET(obj->spectype, SITEM_DROW)
	|| IS_SET(obj->quest, QUEST_ARTIFACT) )
    {

	if      ((sn ==  4) && (IS_AFFECTED(ch, AFF_BLIND)))        return;
	else if ((sn == 27) && (IS_AFFECTED(ch, AFF_DETECT_INVIS))) return;
	else if ((sn == 39) && (IS_AFFECTED(ch, AFF_FLYING)))       return;
	else if ((sn == 45) && (IS_AFFECTED(ch, AFF_INFRARED)))     return;
	else if ((sn == 46) && (IS_AFFECTED(ch, AFF_INVISIBLE)))    return;
	else if ((sn == 52) && (IS_AFFECTED(ch, AFF_PASS_DOOR)))    return;
	else if ((sn == 54) && (IS_AFFECTED(ch, AFF_PROTECT)))      return;
        else if ((sn == 139) && (IS_AFFECTED(ch, AFF_PROTECT_GOOD))) return;
	else if ((sn == 57) && (IS_AFFECTED(ch, AFF_SANCTUARY)))    return;
	else if ((sn == 2 ) && (IS_AFFECTED(ch, AFF_DETECT_INVIS))) return;
	else if ((sn == 3 ) && (IS_AFFECTED(ch, AFF_FLYING)))       return;
	else if ((sn == 1 ) && (IS_AFFECTED(ch, AFF_INFRARED)))     return;
	else if ((sn == 5 ) && (IS_AFFECTED(ch, AFF_INVISIBLE)))    return;
	else if ((sn == 6 ) && (IS_AFFECTED(ch, AFF_PASS_DOOR)))    return;
	else if ((sn == 7 ) && (IS_AFFECTED(ch, AFF_PROTECT)))      return;
	else if ((sn == 8 ) && (IS_AFFECTED(ch, AFF_SANCTUARY)))    return;
	else if ((sn == 9 ) && (IS_AFFECTED(ch, AFF_SNEAK)))        return;
	else if ((sn == 10) && (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD))) return;
	else if ((sn == 11) && (IS_ITEMAFF(ch, ITEMA_FIRESHIELD)))  return;
	else if ((sn == 12) && (IS_ITEMAFF(ch, ITEMA_ICESHIELD)))   return;
	else if ((sn == 13) && (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD)))  return;
	else if ((sn == 14) && (IS_ITEMAFF(ch, ITEMA_MONKCHI)))      return;
	else if ((sn == 15) && (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD))) return;
	else if ((sn == 16) && (IS_ITEMAFF(ch, ITEMA_REGENERATE)))  return;
	else if ((sn == 17) && (IS_ITEMAFF(ch, ITEMA_SPEED)))       return;
	else if ((sn == 18) && (IS_ITEMAFF(ch, ITEMA_VORPAL)))      return;
	else if ((sn == 19) && (IS_ITEMAFF(ch, ITEMA_CRITICAL)))       return;
	else if ((sn == 20) && (IS_ITEMAFF(ch, ITEMA_REFLECT)))     return;
	else if ((sn == 21) && (IS_ITEMAFF(ch, ITEMA_RESISTANCE)))  return;
	else if ((sn == 22) && (IS_ITEMAFF(ch, ITEMA_VISION)))      return;
	else if ((sn == 23) && (IS_ITEMAFF(ch, ITEMA_STALKER)))     return;
	else if ((sn == 24) && (IS_ITEMAFF(ch, ITEMA_VANISH)))      return;
	else if ((sn == 25) && (IS_ITEMAFF(ch, ITEMA_RAGER)))       return;
	else if ((sn == 60) && (IS_AFFECTED(ch, AFF_DETECT_HIDDEN)))return;
	else if ((sn == 26) && (IS_ITEMAFF(ch, ITEMA_AFFMANTIS)))   return;
	else if ((sn == 28) && (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)))  return;
	else if ((sn == 29) && (IS_ITEMAFF(ch, ITEMA_AFFEYE)))      return;
	else if ((sn == 30) && (IS_ITEMAFF(ch, ITEMA_MANASHIELD)))   return;
	else if ((sn == 31) && (IS_ITEMAFF(ch, ITEMA_DOUBLEEXP)))   return;
	if (sn == 4)
	{
	    SET_BIT(ch->affected_by, AFF_BLIND);
	    send_to_char( "You cannot see a thing!\n\r", ch );
	    act("$n seems to be blinded!",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 27 || sn == 2)
	{
	    SET_BIT(ch->affected_by, AFF_DETECT_INVIS);
	    send_to_char( "Your eyes tingle.\n\r", ch );
	    act("$n's eyes flicker with light.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 39 || sn == 3)
	{
	    SET_BIT(ch->affected_by, AFF_FLYING);
	    send_to_char( "Your feet rise off the ground.\n\r", ch );
	    act("$n's feet rise off the ground.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 45 || sn == 1)
	{
	    SET_BIT(ch->affected_by, AFF_INFRARED);
	    send_to_char( "Your eyes glow red.\n\r", ch );
	    act("$n's eyes glow red.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 46 || sn == 5)
	{
	    SET_BIT(ch->affected_by, AFF_INVISIBLE);
	    send_to_char( "You fade out of existance.\n\r", ch );
	    act("$n fades out of existance.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 52 || sn == 6)
	{
	    SET_BIT(ch->affected_by, AFF_PASS_DOOR);
	    send_to_char( "You turn translucent.\n\r", ch );
	    act("$n turns translucent.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 60)
	{
	    SET_BIT(ch->affected_by, AFF_DETECT_HIDDEN);
	    send_to_char( "You awarenes improves.\n\r", ch );
	    act("$n eyes tingle.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 54 || sn == 7)
	{
	    SET_BIT(ch->affected_by, AFF_PROTECT);
	    send_to_char( "You are surrounded by a divine aura.\n\r", ch );
	    act("$n is surrounded by a divine aura.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
        else if (sn == 139)
        {
            SET_BIT(ch->affected_by, AFF_PROTECT_GOOD);
            send_to_char( "You are surrounded by an unholy aura.\n\r", ch ); 
            act("$n is surrounded by an unholy aura.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
	else if (sn == 57 || sn == 8)
	{
	    SET_BIT(ch->affected_by, AFF_SANCTUARY);
	    send_to_char( "You are surrounded by a white aura.\n\r", ch );
	    act("$n is surrounded by a white aura.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 9)
	{
	    SET_BIT(ch->affected_by, AFF_SNEAK);
	    send_to_char( "Your footsteps stop making any sound.\n\r", ch );
	    act("$n's footsteps stop making any sound.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 10)
	{
	    SET_BIT(ch->itemaffect, ITEMA_SHOCKSHIELD);
	    send_to_char( "You are surrounded by a crackling shield of lightning.\n\r", ch );
	    act("$n is surrounded by a crackling shield of lightning.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 11)
	{
	    SET_BIT(ch->itemaffect, ITEMA_FIRESHIELD);
	    send_to_char( "You are surrounded by a burning shield of flames.\n\r", ch );
	    act("$n is surrounded by a burning shield of flames.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 12)
	{
	    SET_BIT(ch->itemaffect, ITEMA_ICESHIELD);
	    send_to_char( "You are surrounded by a shimmering shield of ice.\n\r", ch );
	    act("$n is surrounded by a shimmering shield of ice.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 13)
	{
	    SET_BIT(ch->itemaffect, ITEMA_ACIDSHIELD);
	    send_to_char( "You are surrounded by a bubbling shield of acid.\n\r", ch );
	    act("$n is surrounded by a bubbling shield of acid.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 14)
	{
	    SET_BIT(ch->itemaffect, ITEMA_MONKCHI);
	    send_to_char( "You feel the power of God pulse through you.\n\r", ch );
		if (IS_CLASS(ch, CLASS_MONK) && ch->chi[CURRENT] < 6 )
		{
		ch->chi[CURRENT] = 6;
		return;
		}
	}
	else if (sn == 15)
	{
	    SET_BIT(ch->itemaffect, ITEMA_CHAOSSHIELD);
	    send_to_char( "You are surrounded by a swirling shield of chaos.\n\r", ch );
	    act("$n is surrounded by a swirling shield of chaos.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 16)
	{
	    SET_BIT(ch->itemaffect, ITEMA_REGENERATE);
	    send_to_char( "Your wounds will now tend to themselves.\n\r", ch);
	    act("$n's looks more healthy and vitalized.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 17)
	{
	    SET_BIT(ch->itemaffect, ITEMA_SPEED);
	    send_to_char( "You start moving faster than the eye can follow.\n\r", ch );
	    act("$n starts moving faster than the eye can follow.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 18)
	    SET_BIT(ch->itemaffect, ITEMA_VORPAL);
	else if (sn == 19)
        {
            SET_BIT(ch->itemaffect, ITEMA_CRITICAL);
            send_to_char( "You may hit a critical hit in combat.\n\r", ch);
            act("$n's has obtained some vital knoweldge.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
	else if (sn == 20)
	{
	    SET_BIT(ch->itemaffect, ITEMA_REFLECT);
	    send_to_char( "You are surrounded by flickering shield of darkness.\n\r", ch );
	    act("$n is surrounded by a flickering shield of darkness.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 21)
	{
	    SET_BIT(ch->itemaffect, ITEMA_RESISTANCE);
	    send_to_char( "You feel more protected.\n\r",ch);
	    act("$n is protected by the gods.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 22)
	{
	    SET_BIT(ch->itemaffect, ITEMA_VISION);
	    send_to_char( "Your eyes begin to glow bright white.\n\r", ch );
	    act("$n's eyes begin to glow bright white.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 23)
	    SET_BIT(ch->itemaffect, ITEMA_STALKER);
	else if (sn == 24)
	{
	    SET_BIT(ch->itemaffect, ITEMA_VANISH);
	    send_to_char( "You blend into the shadows.\n\r", ch );
	    act("$n gradually fades into the shadows.",ch,NULL,NULL,TO_ROOM, FALSE);
	}

	else if (sn == 26)
	{
	    SET_BIT(ch->itemaffect, ITEMA_AFFMANTIS);
	    stc(" You are now more skilled at dodge and parrying.\n\r",ch);
	    act(" $n can now dodge and parry more efficiently.",ch,NULL,NULL,TO_ROOM, FALSE);
	}

	else if (sn == 28)
	{
	   SET_BIT(ch->itemaffect, ITEMA_AFFENTROPY);
	}

	else if (sn == 29)
	{
	   SET_BIT(ch->itemaffect, ITEMA_AFFEYE);
	}
	else if (sn == 30)
	{
	SET_BIT(ch->itemaffect, ITEMA_MANASHIELD);
        stc("A swirling shield of mana spirals around you.\n\r",ch);
	act("$n is engulfed by a swirling shield of mana.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 31)
	{
	SET_BIT(ch->itemaffect, ITEMA_DOUBLEEXP);
	stc("Rand al'Thor helps you earn double exp!\n\r",ch);
	act("$n hears the voice of Rand al'Thor.",ch,NULL,NULL,TO_ROOM, FALSE);
	} 
	else if (sn == 32)
	{
	SET_BIT(ch->itemaffect, ITEMA_BIG);
	stc("Rand al'Thor gives you +500 damcap!\n\r",ch);
        act("$n hears the voice of Rand al'Thor.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
	else if (sn == 33)
	{
	stc("Rand al'Thor gives you +500 damcap!\n\r",ch);
        act("$n hears voices.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
        else if (sn == 34)
        {
        stc("Rand al'Thor gives you +1000 damcap!\n\r",ch);
        act("$n possesses the strength of dragons!",ch,NULL,NULL,TO_ROOM, FALSE);
        }
	else if (sn == 35)
	{
	xSET_BIT(ch->extra, DOUBLE_GOLD);
	stc("Rand al'Thor gives you double gold!\n\r",ch);
        act("$n gathers the hoard of dragons!",ch,NULL,NULL,TO_ROOM, FALSE);
        }

        else if (sn == 25)
	{
	    SET_BIT(ch->itemaffect, ITEMA_RAGER);
	    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->rage < 100)
	    {
		ch->rage = 300;
		do_werewolf(ch,"");
	    }
	    else if (IS_CLASS(ch, CLASS_WEREWOLF))
		ch->rage = 300;
	    else if (IS_CLASS(ch, CLASS_VAMPIRE))
		ch->rage = 300;
	    else if (IS_CLASS(ch, CLASS_DEMON))
		ch->rage = 125;
	    else if (IS_CLASS(ch, CLASS_NINJA))
		ch->rage = 200;
	}

	if (IS_SET(obj->spectype, SITEM_DEMONIC) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_POWER] < 15)
	    ch->pcdata->stats[DEMON_POWER] += 1;
	if( IS_SET(obj->spectype, SITEM_DROW) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_POWER] < 15)
            ch->pcdata->stats[DEMON_POWER] += 1;
	if (IS_SET(obj->spectype, SITEM_PDEMONIC) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_PPOWER] < 15)
	    ch->pcdata->stats[DEMON_PPOWER] += 1;

	if (IS_SET(obj->quest, QUEST_ARTIFACT))
	    SET_BIT(ch->itemaffect, ITEMA_ARTIFACT);
	if (IS_SET(obj->spectype, SITEM_SILVER) && obj->wear_loc == WEAR_WIELD)
	    SET_BIT(ch->itemaffect, ITEMA_RIGHT_SILVER);
	if (IS_SET(obj->spectype, SITEM_SILVER) && obj->wear_loc == WEAR_HOLD)
	    SET_BIT(ch->itemaffect, ITEMA_LEFT_SILVER);
    }
    return;
}

/*
 * Unequip a char with an obj.
 */
void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
    CHAR_DATA   *chch;
    AFFECT_DATA *paf;
    int sn;
    int oldpos = obj->wear_loc;

    if ( obj->wear_loc == WEAR_NONE )
    {
	bug( "Unequip_char: already unequipped.", 0 );
	return;
    }

    if ( obj->wear_loc == WEAR_SCABBARD_L ||
    	obj->wear_loc == WEAR_SCABBARD_R ) {obj->wear_loc = -1;return;}

    if ( IS_NPC(ch) || !IS_CLASS(ch, CLASS_HIGHLANDER) || (IS_CLASS(ch, CLASS_HIGHLANDER)
	&& obj->pIndexData->vnum == 29694 ))
	ch->armor += apply_ac(obj, obj->wear_loc);
    obj->wear_loc	 = -1;

    for ( paf = obj->pIndexData->first_affect; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, FALSE, obj );
    for ( paf = obj->first_affect; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, FALSE, obj );

    if ( obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room != NULL
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;

    if ( (chch = get_char_world(ch,ch->name)) == NULL ) return;
    if ( chch->desc != ch->desc) return;

    if (!IS_NPC(ch) && (ch->desc != NULL && ch->desc->connected != CON_PLAYING)) return;

    if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	&& str_cmp(obj->chpoweroff,"(null)")
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	{
	    kavitem(obj->chpoweroff,ch,obj,NULL,TO_CHAR);
	    if (IS_SET(obj->spectype, SITEM_ACTION))
		kavitem(obj->chpoweroff,ch,obj,NULL,TO_ROOM);
	}
    if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	&& str_cmp(obj->victpoweroff,"(null)")
	&& !IS_SET(obj->spectype, SITEM_ACTION)
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	kavitem(obj->victpoweroff,ch,obj,NULL,TO_ROOM);

    if ( ((obj->item_type == ITEM_ARMOR ) && (obj->value[3] >= 1   ))
        || ((obj->item_type == ITEM_WEAPON) && (obj->value[0] >= 1000))
	|| IS_SET(obj->spectype, SITEM_SILVER)
	|| IS_SET(obj->spectype, SITEM_DEMONIC)
	|| IS_SET(obj->spectype, SITEM_DROW)
	|| IS_SET(obj->quest, QUEST_ARTIFACT) )
    {
	if (obj->item_type == ITEM_ARMOR)
	    sn = obj->value[3];
	else
	    sn = obj->value[0] / 1000;
	if (IS_AFFECTED(ch, AFF_BLIND) && (sn == 4)
		&& !is_affected(ch, 4))
	{
	    REMOVE_BIT(ch->affected_by, AFF_BLIND);
	    send_to_char( "You can see again.\n\r", ch );
	    act("$n seems to be able to see again.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_AFFECTED(ch, AFF_DETECT_INVIS) && (sn == 27 || sn == 2)
		&& !is_affected(ch, 27))
	{
	    REMOVE_BIT(ch->affected_by, AFF_DETECT_INVIS);
	    send_to_char( "Your eyes stop tingling.\n\r", ch );
	    act("$n's eyes stop flickering with light.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_AFFECTED(ch, AFF_FLYING) && (sn == 39 || sn == 3)
		&& !is_affected(ch, 39))
	{
	    REMOVE_BIT(ch->affected_by, AFF_FLYING);
	    send_to_char( "You slowly float to the ground.\n\r", ch );
	    act("$n slowly floats to the ground.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_AFFECTED(ch, AFF_INFRARED) && (sn == 45 || sn == 1)
		&& !is_affected(ch, 45))
	{
	    REMOVE_BIT(ch->affected_by, AFF_INFRARED);
	    send_to_char( "Your eyes stop glowing red.\n\r", ch );
	    act("$n's eyes stop glowing red.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_AFFECTED(ch, AFF_INVISIBLE) && (sn == 46 || sn == 5)
		&& !is_affected(ch, 46))
	{
	    REMOVE_BIT(ch->affected_by, AFF_INVISIBLE);
	    send_to_char( "You fade into existance.\n\r", ch );
	    act("$n fades into existance.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_AFFECTED(ch, AFF_PASS_DOOR) && (sn == 52 || sn == 6)
		&& !is_affected(ch, 52))
	{
	    REMOVE_BIT(ch->affected_by, AFF_PASS_DOOR);
	    send_to_char( "You feel solid again.\n\r", ch );
	    act("$n is no longer translucent.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_AFFECTED(ch, AFF_PROTECT) && (sn == 54 || sn == 7)
		&& !is_affected(ch, 54))
	{
	    REMOVE_BIT(ch->affected_by, AFF_PROTECT);
	    send_to_char( "The divine aura around you fades.\n\r", ch );
	    act("The divine aura around $n fades.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
        else if (IS_AFFECTED(ch, AFF_PROTECT_GOOD) && sn == 139
                && !is_affected(ch, 139))
        {
            REMOVE_BIT(ch->affected_by, AFF_PROTECT_GOOD);
            send_to_char( "The unholy aura around you fades.\n\r", ch );
            act("The unholy aura around $n fades.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
	else if (IS_AFFECTED(ch, AFF_SANCTUARY) && (sn == 57 || sn == 8)
		&& !is_affected(ch, 57))
	{
	    REMOVE_BIT(ch->affected_by, AFF_SANCTUARY);
	    send_to_char( "The white aura around your body fades.\n\r", ch );
	    act("The white aura about $n's body fades.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_AFFECTED(ch, AFF_SNEAK) && (sn == 9)
		&& !is_affected(ch, 80))
	{
	    REMOVE_BIT(ch->affected_by, AFF_SNEAK);
	    send_to_char( "You are no longer moving so quietly.\n\r", ch );
	    act("$n is no longer moving so quietly.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_AFFECTED(ch, AFF_DETECT_HIDDEN) && (sn == 60))
	{
	    REMOVE_BIT(ch->affected_by, AFF_DETECT_HIDDEN);
	    send_to_char( "You feel less aware of your surrondings.\n\r",ch );
	    act("$n eyes tingle.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD) && (sn == 10))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_SHOCKSHIELD);
	    send_to_char( "The crackling shield of lightning around you fades.\n\r", ch );
	    act("The crackling shield of lightning around $n fades.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_ITEMAFF(ch, ITEMA_FIRESHIELD) && (sn == 11))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_FIRESHIELD);
	    send_to_char( "The burning shield of fire around you fades.\n\r", ch );
	    act("The burning shield of fire around $n fades.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_ITEMAFF(ch, ITEMA_ICESHIELD) && (sn == 12))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_ICESHIELD);
	    send_to_char( "The shimmering shield of ice around you fades.\n\r", ch );
	    act("The shimmering shield of ice around $n fades.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD) && (sn == 13))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_ACIDSHIELD);
	    send_to_char( "The bubbling shield of acid around you fades.\n\r", ch );
	    act("The bubbling shield of acid around $n fades.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_ITEMAFF(ch, ITEMA_MONKCHI) && (sn == 14))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_MONKCHI);
	    send_to_char( "You no longer feel the presence of God inside you.\n\r", ch );
	}
	else if (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD) && (sn == 15))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_CHAOSSHIELD);
	    send_to_char( "The swirling shield of chaos around you fades.\n\r", ch );
	    act("The swirling shield of chaos around $n fades.",ch,NULL,NULL,TO_ROOM, FALSE);
   	}
        else if (IS_ITEMAFF(ch, ITEMA_MANASHIELD) && (sn == 30))
        {
    	    REMOVE_BIT(ch->itemaffect, ITEMA_MANASHIELD);
            send_to_char( "The swirling shield of mana around you fades.\n\r", ch );
            act("The swirling shield of mana around $n fades.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (IS_ITEMAFF(ch, ITEMA_DOUBLEEXP) && (sn == 31))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_DOUBLEEXP);
	    send_to_char( "The voice of Rand al'Thor leaves your head\n\r",ch);
	    act("$n looks sad as the voice of Rand al'Thor leaves.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
	else if (IS_ITEMAFF(ch, ITEMA_BIG) && (sn == 32))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_BIG);
	    stc("You really miss the +500 damcap.\n\r",ch);
	    act("$n remembers the days of +500 damcap.. then sighs.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
        else if (sn == 33)
        {
        stc("You really miss the +500 damcap!\n\r",ch);
        act("$n laughs when the voices depart.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
        else if (sn == 34)
        {
        stc("You really miss the +1000 damcap!\n\r",ch);
        act("$n returns to normal strength.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
        else if (sn == 35)
        {
        xREMOVE_BIT(ch->extra, DOUBLE_GOLD);
        stc("You no longer gain double gold!\n\r",ch);
        act("$n is no longer a gold whore!",ch,NULL,NULL,TO_ROOM, FALSE);
        }


	else if (sn == 16)
	    REMOVE_BIT(ch->itemaffect, ITEMA_REGENERATE);
	else if (IS_ITEMAFF(ch, ITEMA_SPEED) && (sn == 17))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_SPEED);
	    send_to_char( "Your actions slow down to normal speed.\n\r", ch );
	    act("$n stops moving at supernatural speed.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 26)
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_AFFMANTIS);
	    stc("You forget what you learned of dodging and parrying.\n\r",ch);
	    act("$n looks sad.",ch,NULL,NULL,TO_ROOM, FALSE);
	}

	else if (sn == 28)
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_AFFENTROPY);
	}

	else if (sn == 29)
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_AFFEYE);
	}

	else if (sn == 18)
	    REMOVE_BIT(ch->itemaffect, ITEMA_VORPAL);
        else if (IS_SET(ch->itemaffect, ITEMA_CRITICAL) && (sn == 19))
        {
            REMOVE_BIT(ch->itemaffect, ITEMA_CRITICAL);
            stc("You no longer are capable of critical hits.\n\r",ch);
            act("$n remembers the days of critical hits.. then sighs.",ch,NULL,NULL,TO_ROOM, FALSE);
        }
	else if (IS_ITEMAFF(ch, ITEMA_REFLECT) && sn == 20)
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_REFLECT);
	    send_to_char( "The flickering shield of darkness around you fades.\n\r", ch );
	    act("The flickering shield of darkness around $n fades.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 21)
	    REMOVE_BIT(ch->itemaffect, ITEMA_RESISTANCE);
	else if (IS_ITEMAFF(ch, ITEMA_VISION) && sn == 22)
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_VISION);
	    send_to_char( "Your eyes stop glowing bright white.\n\r", ch );
	    act("$n's eyes stop glowing bright white.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 23)
	    REMOVE_BIT(ch->itemaffect, ITEMA_STALKER);
	else if (IS_ITEMAFF(ch, ITEMA_VANISH) && sn == 24)
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_VANISH);
	    send_to_char( "You emerge from the shadows.\n\r", ch );
	    act("$n gradually fades out of the shadows.",ch,NULL,NULL,TO_ROOM, FALSE);
	}
	else if (sn == 25 && !IS_NPC(ch) && IS_ITEMAFF(ch, ITEMA_RAGER))
	{
	    REMOVE_BIT(ch->itemaffect, ITEMA_RAGER);
/*
	    if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->rage >= 100)
	    {
		ch->rage = 0;
		do_unwerewolf(ch,"");
	    }
	    ch->rage = 0;
*/
	}
	if (IS_SET(obj->spectype, SITEM_DEMONIC) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_POWER] > 0)
	    ch->pcdata->stats[DEMON_POWER] -= 1;
	if( IS_SET(obj->spectype, SITEM_DROW) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_POWER] > 0)
            ch->pcdata->stats[DEMON_POWER] -= 1;
	if (IS_SET(obj->spectype, SITEM_PDEMONIC) && !IS_NPC(ch)
	    && ch->pcdata->stats[DEMON_PPOWER] > 0)
	    ch->pcdata->stats[DEMON_PPOWER] -= 1;

	if (IS_SET(obj->quest, QUEST_ARTIFACT)) 
	    REMOVE_BIT(ch->itemaffect, ITEMA_ARTIFACT);
	if (IS_SET(obj->spectype, SITEM_SILVER) && oldpos == WEAR_WIELD)
	    REMOVE_BIT(ch->itemaffect, ITEMA_RIGHT_SILVER);
	if (IS_SET(obj->spectype, SITEM_SILVER) && oldpos == WEAR_HOLD)
	    REMOVE_BIT(ch->itemaffect, ITEMA_LEFT_SILVER);
    }
    return;
}



/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )
{
    OBJ_DATA *obj;
    int nMatch;

    nMatch = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData == pObjIndex )
	    nMatch++;
    }

    return nMatch;
}



/*
 * Move an obj out of a room.
 */
void obj_from_room( OBJ_DATA *obj )
{
    ROOM_INDEX_DATA *in_room;

    if (obj == NULL) return;
    if ( ( in_room = obj->in_room ) == NULL )
    {
	bug( "obj_from_room: NULL.", 0 );
	return;
    }

    if ( obj == in_room->contents )
    {
	in_room->contents = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = in_room->contents; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_room: obj not found.", 0 );
	    return;
	}
    }

    obj->in_room      = NULL;
    obj->next_content = NULL;
    return;
}



/*
 * Move an obj into a room.
 */
void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )
{
    if (obj == NULL) return;
    if (pRoomIndex == NULL) return;
    obj->next_content		= pRoomIndex->contents;
    pRoomIndex->contents	= obj;
    obj->in_room		= pRoomIndex;
    obj->carried_by		= NULL;
    obj->in_obj			= NULL;
    return;
}


/*
 * Move an object into an object.
 */
void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )
{
OBJ_DATA *tmp_obj;

  if (!obj || !obj_to || obj == obj_to) {
    log_string( "SYSERR: NULL object or same source and target obj passed to obj_to_obj" );
    return;
  }


    obj->next_content           = obj_to->contains;
    obj_to->contains            = obj;
    obj->in_obj                 = obj_to;
    obj->in_room              = NULL;
    obj->carried_by           = NULL;

    for ( tmp_obj = obj->in_obj; tmp_obj->in_obj; tmp_obj =tmp_obj->in_obj )
        GET_OBJ_WEIGHT(tmp_obj) += GET_OBJ_WEIGHT(obj);

//  /* top level object.  Subtract weight from inventory if necessary. */
//  GET_OBJ_WEIGHT(tmp_obj) += GET_OBJ_WEIGHT(obj);
//  if (tmp_obj->carried_by)
//    tmp_obj->carried_by->carry_weight += GET_OBJ_WEIGHT(obj);

//      if ( obj_to->carried_by != NULL && !IS_SET(obj_to->spectype,SITEM_MORPH))
//          obj_to->carried_by->carry_weight += get_obj_weight( obj );

    return;
}

/*
 * Move an object into an object.
 *
void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )
{
    if (obj == NULL) return;
    obj->next_content		= obj_to->contains;
    obj_to->contains		= obj;
    obj->in_obj			= obj_to;
    obj->in_room		= NULL;
    obj->carried_by		= NULL;

    for ( ; obj_to != NULL; obj_to = obj_to->in_obj )
    {
	if ( obj_to->carried_by != NULL && !IS_SET(obj_to->spectype,SITEM_MORPH))
	    obj_to->carried_by->carry_weight += get_obj_weight( obj );
    }

    return;
}
*/


/*
 * Move an object out of an object.
 */
void obj_from_obj( OBJ_DATA *obj )
{
    OBJ_DATA *obj_from;
    CHAR_DATA *ch;

    if (obj == NULL) return;
    if ( ( obj_from = obj->in_obj ) == NULL )
    {
	bug( "Obj_from_obj: (%s) null obj_from.", obj->short_descr );
	return;
    }

    if ( obj == obj_from->contains )
    {
	obj_from->contains = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = obj_from->contains; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_obj: obj not found.", 0 );
	    return;
	}
    }

    obj->next_content = NULL;
    obj->in_obj       = NULL;

    if ((ch = obj_from->carried_by) != NULL)
    {
       ch->carry_number -= 1;
       if (obj->contains != NULL)
          subtract_contents( obj->contains, ch );
    }

    for ( ; obj_from != NULL; obj_from = obj_from->in_obj )
    {
	if ( obj_from->carried_by != NULL && !IS_SET(obj_from->spectype,SITEM_MORPH))
	    obj_from->carried_by->carry_weight -= get_obj_weight( obj );
    }

    return;
}



/*
 * Extract an obj from the world.
 */
void extract_obj( OBJ_DATA *obj )
{
    CHAR_DATA *ch;
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;
    if ( obj == NULL ) return;
    if ( obj->in_room != NULL )
	obj_from_room( obj );
    else if ( obj->carried_by != NULL )
	obj_from_char( obj );
    else if ( obj->in_obj != NULL )
	obj_from_obj( obj );
 
    if ( ( ch = obj->chobj ) != NULL
    &&   !IS_NPC(ch)
    &&   ch->pcdata->chobj == obj
    &&   IS_HEAD(ch,LOST_HEAD))
    {
	REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
	REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
	free_string(ch->morph);
	ch->morph = str_dup("");
	ch->hit = 1;
	char_from_room(ch);
	char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
	ch->pcdata->chobj = NULL;
	obj->chobj = NULL;
	send_to_char("You have been KILLED!!\n\r",ch);
	do_look(ch,"auto");
	ch->position = POS_RESTING;
    }
    else if ( ( ch = obj->chobj ) != NULL
    &&   !IS_NPC(ch)
    &&   ch->pcdata->chobj == obj
    &&   (IS_EXTRA(ch,EXTRA_OSWITCH) || ch->pcdata->obj_vnum != 0))
    {
	if (ch->pcdata->obj_vnum == 420002)
	{
              char_from_room(ch);
              char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
              ch->pcdata->obj_vnum = 0;
              obj->chobj = NULL;
              ch->pcdata->chobj = NULL;
              REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
              xREMOVE_BIT(ch->extra, EXTRA_OSWITCH);
              free_string(ch->morph);
              ch->morph = str_dup("");
              stc("Your are broken out of the #RKin#rdr#Red #0Sp#8ir#0it #RJe#rw#Rel#n.\n\r",ch);
	      do_look(ch, "auto");
	}
	else if (ch->pcdata->obj_vnum != 0)
        {
              char_from_room(ch);
              char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
              ch->pcdata->obj_vnum = 0;
              obj->chobj = NULL;
              ch->pcdata->chobj = NULL;
              REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
              xREMOVE_BIT(ch->extra, EXTRA_OSWITCH);
              free_string(ch->morph);
              ch->morph = str_dup("");
              stc("Your are returned to your body.\n\r",ch);
              do_look(ch, "auto");
        }

	else
	{
	    xREMOVE_BIT(ch->extra,EXTRA_OSWITCH);
	    REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
	    free_string(ch->morph);
	    ch->morph = str_dup("");
	    char_from_room(ch);
	    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
	    ch->pcdata->chobj = NULL;
	    obj->chobj = NULL;
	    send_to_char("You return to your body.\n\r",ch);
              do_look(ch, "auto");
	}
    }
    else if ( obj->chobj != NULL )
    {
	if (!IS_NPC(obj->chobj)) obj->chobj->pcdata->chobj = NULL;
	obj->chobj = NULL;
	bug( "Extract_obj: obj %d chobj invalid.", obj->pIndexData->vnum );
    }

    for ( obj_content = obj->contains; obj_content; obj_content = obj_next )
    {
	obj_next = obj_content->next_content;
	if (obj->contains != NULL)
	{
	 extract_obj( obj->contains );
	}
    } 


    if ( object_list == obj )
    {
	object_list = obj->next;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = object_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == obj )
	    {
		prev->next = obj->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_obj: obj %d not found.", obj->pIndexData->vnum );
	    return;
	}
    }

    /* remove affects */
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	for ( paf = obj->first_affect; paf; paf = paf_next )
	{
	    paf_next    = paf->next;
	    DISPOSE( paf );
	}
	obj->first_affect = obj->last_affect = NULL;
    }

    {
	EXTRA_DESCR_DATA *ed;
	EXTRA_DESCR_DATA *ed_next;

	for ( ed = obj->extra_descr; ed != NULL; ed = ed_next )
	{
	    ed_next		= ed->next;
	    free_string( ed->description );
	    free_string( ed->keyword     );
	    ed->next = extra_descr_free;
	    extra_descr_free	= ed;
	}
    }

    free_string( obj->name        );
    free_string( obj->description );
    free_string( obj->short_descr );
    if (obj->chpoweron    != NULL) free_string( obj->chpoweron   );
    if (obj->chpoweroff   != NULL) free_string( obj->chpoweroff  );
    if (obj->chpoweruse   != NULL) free_string( obj->chpoweruse  );
    if (obj->victpoweron  != NULL) free_string( obj->victpoweron );
    if (obj->victpoweroff != NULL) free_string( obj->victpoweroff);
    if (obj->victpoweruse != NULL) free_string( obj->victpoweruse);
    if (obj->questmaker   != NULL) free_string( obj->questmaker  );
    if (obj->questowner   != NULL) free_string( obj->questowner  );
    --obj->pIndexData->count;
    obj->next	= obj_free;
    obj_free	= obj;
    return;
}



/*
 * Extract a char from the world.
 */
void extract_char( CHAR_DATA *ch, bool fPull )
{
    CHAR_DATA *wch;
    CHAR_DATA *familiar;
    CHAR_DATA *wizard;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( ch == NULL ) return;

    if ( ch->in_room == NULL )
    {
	bug( "Extract_char: NULL.", 0 );
	bug( "%s is in a null room.", ch->name);
	return;
    }

    if ( ch == supersnard )
    {
	bug( "Extract_char: ch == supersnard!", 0 );
	return;
    }

    if ( fPull )
	die_follower( ch );

    stop_fighting( ch, TRUE );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	extract_obj( obj );
    }
    
    char_from_room( ch );

    if ( IS_NPC(ch) )
	--ch->pIndexData->count;
    else if ( ch->pcdata->chobj != NULL )
    {
	ch->pcdata->chobj->chobj = NULL;
	ch->pcdata->chobj = NULL;
    }

    if ( !fPull )
    {
	char_to_room( ch, get_room_index( ROOM_VNUM_ALTAR ) );
	return;
    }

    if ( ch->desc != NULL && ch->desc->original != NULL )
	do_return( ch, "" );

    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch->reply == ch )
	    wch->reply = NULL;
    }

    if ( ch == char_list )
    {
       char_list = ch->next;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = char_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == ch )
	    {
		prev->next = ch->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_char: char not found.", 0 );
	    return;
	}
    }

    if ( ch->desc )
    {
	ch->desc->character = NULL;
	close_socket(ch->desc);
    }
    if ( (wizard = ch->wizard) != NULL)
    {
	if (!IS_NPC(wizard)) wizard->pcdata->familiar = NULL;
	ch->wizard = NULL;
    }
    if ( !IS_NPC(ch) )
    {
	if ((familiar = ch->pcdata->familiar) != NULL)
	{
	    familiar->wizard = NULL;
	    ch->pcdata->familiar = NULL;
	    if (IS_NPC(familiar))
	    {
		act("$n slowly fades away to nothing.",familiar,NULL,NULL,TO_ROOM, FALSE);
		extract_char(familiar,TRUE);
	    }
	}
	if ((familiar = ch->pcdata->partner) != NULL)
	    ch->pcdata->partner = NULL;
	if ((familiar = ch->pcdata->propose) != NULL)
	    ch->pcdata->propose = NULL;
	for ( familiar = char_list; familiar != NULL; familiar = familiar->next)
	{
	    if ( !IS_NPC(familiar) && familiar->pcdata->propose != NULL && 
		familiar->pcdata->propose == ch )
		familiar->pcdata->propose = NULL;
	    if ( !IS_NPC(familiar) && familiar->pcdata->partner != NULL && 
		familiar->pcdata->partner == ch )
		familiar->pcdata->partner = NULL;
	}
    }
    else if (IS_NPC(ch) && strlen(ch->lord) > 1)
    {
	for ( wch = char_list; wch != NULL ; wch = wch->next )
	{
	    if (IS_NPC(wch)) continue;
	    if (str_cmp(wch->name, ch->lord)) continue;
	    if (wch->pcdata->followers > 0) wch->pcdata->followers--;
	}
    }

    free_char( ch );
    return;
}



/*
 * Find a char in the room.
 */
CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    if (!ch)
    {
       bug( "get_char_room: null ch", 0);
       return NULL;
    }

    if ( !str_cmp( arg, "self" ) && (IS_NPC(ch) || ch->pcdata->chobj == NULL))
	return ch;
    if ( ch->in_room->people != NULL )
    {
    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( !IS_NPC(rch) && IS_HEAD(rch, LOST_HEAD) ) continue;
	else if ( !IS_NPC(rch) && IS_EXTRA(rch, EXTRA_OSWITCH) ) continue;
	else if ( !can_see( ch, rch ) || ( !is_name( arg, rch->name ) &&
	 (IS_NPC(rch) || !is_name(arg, rch->pcdata->switchname)) &&
	    ( IS_NPC(rch) || !is_name( arg, rch->morph ))))
	    continue;
	if ( ++count == number )
	    return rch;
    }
    }
    return NULL;
}




/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( ( wch = get_char_room( ch, argument ) ) != NULL )
	return wch;

    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( !IS_NPC(wch) && IS_HEAD(wch, LOST_HEAD) ) continue;
	else if ( !IS_NPC(wch) && IS_EXTRA(wch, EXTRA_OSWITCH) ) continue;
	else if ( !can_see( ch, wch ) || ( !is_name( arg, wch->name ) &&
	 (IS_NPC(wch) || !is_name(arg, wch->pcdata->switchname)) &&

	    ( IS_NPC(wch) || !is_name( arg, wch->morph ))))
	    continue;
	if ( ++count == number )
	    return wch;
    }

    return NULL;
}



/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )
{
    OBJ_DATA *obj;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->pIndexData == pObjIndex )
	    return obj;
    }

    return NULL;
}


/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an object within the object you are in.
 */
OBJ_DATA *get_obj_in_obj( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    if (IS_NPC(ch) || ch->pcdata->chobj == NULL || ch->pcdata->chobj->in_obj == NULL)
	return NULL;
    obj = ch->pcdata->chobj;
    if (obj->in_obj->item_type != ITEM_CONTAINER && obj->in_obj->item_type != ITEM_CORPSE_NPC &&
    	obj->in_obj->item_type != ITEM_CORPSE_PC ) return NULL;

    for ( obj2 = obj->in_obj->contains; obj2 != NULL; obj2 = obj2->next_content )
    {
	if (obj != obj2
	&&  is_name( arg, obj2->name ) )
	{
	    if ( ++count == number )
		return obj2;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == WEAR_NONE
	&&   can_see_obj( ch, obj )
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc != WEAR_NONE
	&&   can_see_obj( ch, obj )
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    obj = get_obj_list( ch, argument, ch->in_room->contents );
    if ( obj != NULL )
	return obj;

    if ( ( obj = get_obj_carry( ch, argument ) ) != NULL )
	return obj;

    if ( ( obj = get_obj_wear( ch, argument ) ) != NULL )
	return obj;

    if ( ( obj = get_obj_in_obj( ch, argument ) ) != NULL )
	return obj;

    return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_room( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    obj = get_obj_list( ch, argument, ch->in_room->contents );
    if ( obj != NULL )
	return obj;

    return NULL;
}



/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    if ( ( obj = get_obj_here( ch, argument ) ) != NULL )
	return obj;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int amount )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    if ( amount <= 0 )
    {
//	bug( "Create_money: zero or negative money %d.", amount );
	amount = 1;
    }

    if ( amount == 1 )
    {
	obj = create_object( get_obj_index( OBJ_VNUM_MONEY_ONE ), 0 );
    }
    else
    {
	obj = create_object( get_obj_index( OBJ_VNUM_MONEY_SOME ), 0 );
	sprintf( buf, obj->short_descr, amount );
	free_string( obj->short_descr );
	obj->short_descr	= str_dup( buf );
	obj->value[0]		= amount;
    }

    return obj;
}



/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight( OBJ_DATA *obj )
{
    int weight;

    weight = obj->weight;
    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
	weight += get_obj_weight( obj );

    return weight;
}



/*
 * True if room is dark.
 */
bool room_is_dark( ROOM_INDEX_DATA *pRoomIndex )
{
    if ( pRoomIndex->light > 0 )
	return FALSE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )
	return TRUE;

    if ( pRoomIndex->sector_type == SECT_INSIDE
    ||   pRoomIndex->sector_type == SECT_CITY )
	return FALSE;

    if ( weather_info.sunlight == SUN_SET
    ||   weather_info.sunlight == SUN_DARK )
	return TRUE;

    return FALSE;
}


bool room_is_noft( ROOM_INDEX_DATA *pRoomIndex)
{

    if ( IS_SET(pRoomIndex->room_flags, ROOM_NO_FT))
        return TRUE;

    return FALSE;
}

/*
 * True if room is private.
 */
bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )
{
    CHAR_DATA *rch;
    int count;

    count = 0;
    for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )
	count++;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )
	return TRUE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )
	return TRUE;

    return FALSE;
}



/*
 * True if char can see victim.
 */
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{

  if ( ch == victim )
    return TRUE;


  if (get_trust(ch)>6)
    return TRUE;

  if (victim->blinkykill != NULL && IS_SET(ch->affected_by2,EXTRA_BLINKY))
  {
    REMOVE_BIT(ch->affected_by2,EXTRA_BLINKY);	
    return TRUE;
  }
  if (!IS_NPC(victim) && xIS_SET(victim->act, PLR_WIZINVIS) && victim->level > 6)
    return FALSE;
  if (victim->quit_timer > 0)
    return TRUE;
  if( IS_SET(ch->tag_flags, TAG_PLAYING))
    return TRUE;
  if (!IS_NPC(victim) && (victim->desc != NULL && victim->desc->connected != CON_PLAYING))
    return FALSE;

  if (!IS_NPC(victim) && xIS_SET(victim->act, PLR_INCOG) && victim->level > 6 && victim->in_room != ch->in_room)
    return FALSE;
  if (!IS_NPC(victim) && xIS_SET(victim->act, PLR_WIZINVIS) && !(victim->class == ch->class))
    return FALSE;
  if (!IS_NPC(victim) && xIS_SET(victim->act, PLR_HIDE) && !(victim->class == ch->class))
    return FALSE;
  if (!IS_NPC(victim) && IS_SET(victim->newbits, NEW_DARKNESS) && !(victim->class == ch->class))
    return FALSE;

    if ( IS_EXTRA(ch, BLINDFOLDED))
	return FALSE;

    if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) )
	return TRUE;

/*    if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && !IS_AFFECTED(victim,AFF_SHADOWPLANE)
	&& !IS_AFFECTED(ch,AFF_SHADOWSIGHT))
	return FALSE;

    if (!IS_AFFECTED(ch,AFF_SHADOWPLANE) && IS_AFFECTED(victim,AFF_SHADOWPLANE)
	&& !IS_AFFECTED(ch,AFF_SHADOWSIGHT))
	return FALSE;
*/

    if ( !IS_NPC(ch) && IS_VAMPAFF(ch, VAM_SONIC) )
	return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
	return FALSE;

    if ( IS_AFFECTED(ch, AFF_BLIND) && !IS_AFFECTED(ch, AFF_SHADOWSIGHT))
	return FALSE;

    if ( room_is_dark( ch->in_room ) && !IS_AFFECTED(ch, AFF_INFRARED) 
	&& (!IS_NPC(ch) && !IS_VAMPAFF(ch, VAM_NIGHTSIGHT)))
	return FALSE;

  if (IS_AFFECTED(victim, AFF_INVISIBLE) && !IS_AFFECTED(ch, AFF_DETECT_INVIS))
	  return FALSE;

  if (IS_AFFECTED(victim, AFF_HIDE) && !IS_AFFECTED(ch, AFF_DETECT_HIDDEN))
    return FALSE;

  if (!IS_NPC(ch) && IS_HEAD(ch, LOST_HEAD))
	  return TRUE;

  if (!IS_NPC(ch) && IS_EXTRA(ch, EXTRA_OSWITCH) )
	  return TRUE;

  if (!IS_NPC(ch) && IS_HEAD(ch, LOST_HEAD) &&ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    return TRUE;

  if (!IS_NPC(ch) && IS_EXTRA(ch, EXTRA_OSWITCH) &&	ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    return TRUE;

  return TRUE;
}



/*
 * True if char can see obj.
 */
bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
  CHAR_DATA *gch;

    if ((gch = obj->carried_by) != NULL)
    {
      if (gch->desc != NULL)
      {
        if (gch->desc->connected != CON_PLAYING) return FALSE;
      }
    }

    if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) )
	return TRUE;

    if (( IS_SET(obj->extra_flags, ITEM_SHADOWPLANE))
    &&   (obj->carried_by == NULL)
    &&   (!IS_AFFECTED(ch, AFF_SHADOWSIGHT))
    &&   (!IS_AFFECTED(ch, AFF_SHADOWPLANE)))
	return FALSE;

    if (( !IS_SET(obj->extra_flags, ITEM_SHADOWPLANE))
    &&   (obj->carried_by == NULL)
    &&   (!IS_AFFECTED(ch, AFF_SHADOWSIGHT))
    &&   (IS_AFFECTED(ch, AFF_SHADOWPLANE)))
	return FALSE;

    if ( !IS_NPC(ch) && IS_VAMPAFF(ch, VAM_SONIC) )
	return TRUE;

    if ( obj->item_type == ITEM_POTION )
	return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
	return FALSE;

    if ( IS_EXTRA(ch, BLINDFOLDED) )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_BLIND ) && !IS_AFFECTED(ch, AFF_SHADOWSIGHT) )
	return FALSE;

    if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
	return TRUE;

    if ( room_is_dark( ch->in_room ) && !IS_AFFECTED(ch, AFF_INFRARED) 
	&& (!IS_NPC( ch ) && !IS_VAMPAFF(ch, VAM_NIGHTSIGHT)))
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_INVIS)
    &&   !IS_AFFECTED(ch, AFF_DETECT_INVIS) )
	return FALSE;

    if ( !IS_NPC(ch) && IS_HEAD(ch, LOST_HEAD) &&
	ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
	return TRUE;

    if ( !IS_NPC(ch) && IS_EXTRA(ch, EXTRA_OSWITCH) &&
	ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
	return TRUE;

    return TRUE;
}



/*
 * True if char can drop obj.
 */
bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_SET(obj->extra_flags, ITEM_NODROP) )
	return TRUE;

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return TRUE;

    return FALSE;
}



/*
 * Return ascii name of an item type.
 */
char *item_type_name( OBJ_DATA *obj )
{
    switch ( obj->item_type )
    {
    case ITEM_LIGHT:		return "light";
    case ITEM_SCROLL:		return "scroll";
    case ITEM_WAND:		return "wand";
    case ITEM_STAFF:		return "staff";
    case ITEM_WEAPON:		return "weapon";
    case ITEM_TREASURE:		return "treasure";
    case ITEM_INSTRUMENT:	return "instrument";
    case ITEM_ARMOR:		return "armor";
    case ITEM_POTION:		return "potion";
    case ITEM_FURNITURE:	return "furniture";
    case ITEM_TRASH:		return "trash";
    case ITEM_CONTAINER:	return "container";
    case ITEM_DRINK_CON:	return "drink container";
    case ITEM_KEY:		return "key";
    case ITEM_FOOD:		return "food";
    case ITEM_MONEY:		return "money";
    case ITEM_BOAT:		return "boat";
    case ITEM_CORPSE_NPC:	return "npc corpse";
    case ITEM_CORPSE_PC:	return "pc corpse";
    case ITEM_FOUNTAIN:		return "fountain";
    case ITEM_PILL:		return "pill";
    case ITEM_PORTAL:		return "portal";
    case ITEM_EGG:		return "egg";
    case ITEM_VOODOO:		return "voodoo doll";
    case ITEM_STAKE:		return "stake";
    case ITEM_MISSILE:		return "missile";
    case ITEM_AMMO:		return "ammo";
    case ITEM_QUEST:		return "quest token";
    case ITEM_QUESTCARD:	return "quest card";
    case ITEM_QUESTMACHINE:	return "quest generator";
    case ITEM_SYMBOL:		return "magical symbol";
    case ITEM_BOOK:		return "book";
    case ITEM_PAGE:		return "page";
    case ITEM_TOOL:		return "tool";
    case ITEM_COPPER:           return "copper";    
    case ITEM_IRON:             return "iron";
    case ITEM_STEEL:            return "steel";
    case ITEM_ADAMANTITE:       return "adamantite";
    case ITEM_ELECTRUM:         return "electrum";
    case ITEM_GEMSTONE:         return "gemstone";
    case ITEM_HILT:             return "hilt";
    case ITEM_HEAD:		return "head";
    case ITEM_BANK:		return "atm";
    case ITEM_WELL:		return "wishing well";
    case ITEM_STATUE:		return "statue";
    case ITEM_TELEPORTER:	return "teleporter";
    case ITEM_IOU:		return "quest-IOU";
    case ITEM_MACHINE:		return "vending machine";
    case ITEM_EXP_TOKEN:	return "exp-token";
}

    bug( "Item_type_name: unknown type %d.", obj->item_type );
    return "(unknown)";
}



/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_NONE:		return "none";
    case APPLY_STR:		return "strength";
    case APPLY_DEX:		return "dexterity";
    case APPLY_INT:		return "intelligence";
    case APPLY_WIS:		return "wisdom";
    case APPLY_CON:		return "constitution";
    case APPLY_SEX:		return "sex";
    case APPLY_CLASS:		return "class";
    case APPLY_LEVEL:		return "level";
    case APPLY_AGE:		return "age";
    case APPLY_MANA:		return "mana";
    case APPLY_HIT:		return "hp";
    case APPLY_MOVE:		return "moves";
    case APPLY_GOLD:		return "gold";
    case APPLY_EXP:		return "experience";
    case APPLY_AC:		return "armor class";
    case APPLY_HITROLL:		return "hit roll";
    case APPLY_DAMROLL:		return "damage roll";
    case APPLY_SAVING_PARA:	return "save vs paralysis";
    case APPLY_SAVING_ROD:	return "save vs rod";
    case APPLY_SAVING_PETRI:	return "save vs petrification";
    case APPLY_SAVING_BREATH:	return "save vs breath";
    case APPLY_SAVING_SPELL:	return "save vs spell";
    case APPLY_POLY:		return "polymorph form";
    }

    bug( "Affect_location_name: unknown location %d.", location );
    return "(unknown)";
}



/*
 * Return ascii name of an affect bit vector.
 */
char *affect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_BLIND         ) strcat( buf, " blind"         );
    if ( vector & AFF_INVISIBLE     ) strcat( buf, " invisible"     );
    if ( vector & AFF_DETECT_EVIL   ) strcat( buf, " detect_evil"   );
    if ( vector & AFF_DETECT_INVIS  ) strcat( buf, " detect_invis"  );
    if ( vector & AFF_DETECT_MAGIC  ) strcat( buf, " detect_magic"  );
    if ( vector & AFF_DETECT_HIDDEN ) strcat( buf, " detect_hidden" );
    if ( vector & AFF_SHADOWPLANE   ) strcat( buf, " shadowplane"   );
    if ( vector & AFF_SANCTUARY     ) strcat( buf, " sanctuary"     );
    if ( vector & AFF_FAERIE_FIRE   ) strcat( buf, " faerie_fire"   );
    if ( vector & AFF_INFRARED      ) strcat( buf, " infrared"      );
    if ( vector & AFF_CURSE         ) strcat( buf, " curse"         );
    if ( vector & AFF_FLAMING       ) strcat( buf, " flaming"       );
    if ( vector & AFF_POISON        ) strcat( buf, " poison"        );
    if ( vector & AFF_PROTECT       ) strcat( buf, " protect"       );
    if ( vector & AFF_ETHEREAL      ) strcat( buf, " ethereal"      );
    if ( vector & AFF_SLEEP         ) strcat( buf, " sleep"         );
    if ( vector & AFF_SNEAK         ) strcat( buf, " sneak"         );
    if ( vector & AFF_HIDE          ) strcat( buf, " hide"          );
    if ( vector & AFF_CHARM         ) strcat( buf, " charm"         );
    if ( vector & AFF_FLYING        ) strcat( buf, " flying"        );
    if ( vector & AFF_PASS_DOOR     ) strcat( buf, " pass_door"     );
    if ( vector & AFF_POLYMORPH     ) strcat( buf, " polymorph"     );
    if ( vector & AFF_SHADOWSIGHT   ) strcat( buf, " shadowsight"   );
    if ( vector & AFF_WEBBED        ) strcat( buf, " web"           );
  /*  if ( vector & AFF_DARKNESS      ) strcat( buf, " darkness" );*/
//    if ( vector & AFF_TENDRILS )     strcat(buf, " tendrils" );
    if ( vector & AFF_INFIRMITY     ) strcat( buf, " infirmity" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}



/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name( int extra_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW         ) strcat( buf, " glow"         );
    if ( extra_flags & ITEM_HUM          ) strcat( buf, " hum"          );
    if ( extra_flags & ITEM_INVIS        ) strcat( buf, " invis"        );
    if ( extra_flags & ITEM_MAGIC        ) strcat( buf, " magic"        );
    if ( extra_flags & ITEM_NODROP       ) strcat( buf, " nodrop"       );
    if ( extra_flags & ITEM_ANTI_GOOD    ) strcat( buf, " anti-good"    );
    if ( extra_flags & ITEM_ANTI_EVIL    ) strcat( buf, " anti-evil"    );
    if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " anti-neutral" );
    if ( extra_flags & ITEM_BLESS        ) strcat( buf, " bless"        );
    if ( extra_flags & ITEM_NOREMOVE     ) strcat( buf, " noremove"     );
    if ( extra_flags & ITEM_INVENTORY    ) strcat( buf, " inventory"    );
    if ( extra_flags & ITEM_LOYAL        ) strcat( buf, " loyal"        );
    if ( extra_flags & ITEM_SHADOWPLANE  ) strcat( buf, " shadowplane"  );
    if ( extra_flags & ITEM_THROWN       ) strcat( buf, " thrown"       );
    if ( extra_flags & ITEM_KEEP         ) strcat( buf, " keep"         );
    if ( extra_flags & ITEM_VANISH       ) strcat( buf, " vanish"       );
    if ( extra_flags & ITEM_NOSAC	 ) strcat( buf, " nosacrifice"  );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

CHAR_DATA *get_char_world2( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *wch;

    if (argument[0] == '\0') return NULL;

    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( IS_NPC(wch) && !str_cmp( argument, wch->short_descr ) ) return wch;
    }

    return NULL;
}

OBJ_DATA *get_obj_world2( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int vnum = 0;

    if (argument[0] == '\0') return NULL;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !str_cmp( argument, obj->short_descr ) )
	{
	    if ((vnum = obj->pIndexData->vnum) == 30037 || vnum == 30041)
		continue;
	    return obj;
	}
    }

    return NULL;
}

/*
 * Is astr contained within bstr ?
 *
 */
bool is_contained( const char *astr, const char *bstr )
{
  int i, alen, blen, count;

  alen = strlen(astr);
  blen = strlen(bstr);
  if (alen > blen) return FALSE;
  for (i = 0; i <= (blen - alen); i++)
  {
    count = 0;
    while (count < alen && UPPER(astr[count]) == UPPER(bstr[i+count])) count++;
    if (count == alen) return TRUE;
  }
  return FALSE;
}

int chprintf( CHAR_DATA* ch, const char *format, ... )
{
        static char buf[MAX_STRING_LENGTH+1];
        va_list arglist;
        int nChars= 0;
        va_start( arglist, format );
        if ( format != NULL && format[0] && ch->desc != NULL )
        {
                nChars = vsprintf( buf, format, arglist );
                assert( nChars <= MAX_STRING_LENGTH );
                write_to_buffer( ch->desc, buf, nChars );
        }
        va_end( arglist );
        return nChars;
}

/*
 * Extended Bitvector Routines					-Thoric
 */

/* check to see if the extended bitvector is completely empty */
bool ext_is_empty( EXT_BV *bits )
{
    int x;

    for ( x = 0; x < XBI; x++ )
	if ( bits->bits[x] != 0 )
	    return FALSE;

    return TRUE;
}

void ext_clear_bits( EXT_BV *bits )
{
    int x;

    for ( x = 0; x < XBI; x++ )
	bits->bits[x] = 0;
}

/* for use by xHAS_BITS() -- works like IS_SET() */
int ext_has_bits( EXT_BV *var, EXT_BV *bits )
{
    int x, bit;

    for ( x = 0; x < XBI; x++ )
	if ( (bit=(var->bits[x] & bits->bits[x])) != 0 )
	    return bit;

    return 0;
}

/* for use by xSAME_BITS() -- works like == */
bool ext_same_bits( EXT_BV *var, EXT_BV *bits )
{
    int x;

    for ( x = 0; x < XBI; x++ )
	if ( var->bits[x] != bits->bits[x] )
	    return FALSE;

    return TRUE;
}

/* for use by xSET_BITS() -- works like SET_BIT() */
void ext_set_bits( EXT_BV *var, EXT_BV *bits )
{
    int x;

    for ( x = 0; x < XBI; x++ )
	var->bits[x] |= bits->bits[x];
}

/* for use by xREMOVE_BITS() -- works like REMOVE_BIT() */
void ext_remove_bits( EXT_BV *var, EXT_BV *bits )
{
    int x;

    for ( x = 0; x < XBI; x++ )
	var->bits[x] &= ~(bits->bits[x]);
}

/* for use by xTOGGLE_BITS() -- works like TOGGLE_BIT() */
void ext_toggle_bits( EXT_BV *var, EXT_BV *bits )
{
    int x;

    for ( x = 0; x < XBI; x++ )
	var->bits[x] ^= bits->bits[x];
}

/*
 * Read an extended bitvector from a file.			-Thoric
 */
EXT_BV fread_bitvector( FILE *fp )
{
    EXT_BV ret;
    int c, x = 0;
    int num = 0;

    memset( &ret, '\0', sizeof(ret) );
    for ( ;; )
    {
	num = fread_number(fp);
	if ( x < XBI )
	    ret.bits[x] = num;
	++x;
	if ( (c=getc(fp)) != '&' )
	{
	    ungetc(c, fp);
	    break;
	}
    }

    return ret;
}

/* return a string for writing a bitvector to a file */
char *print_bitvector( EXT_BV *bits )
{
    static char buf[XBI * 12];
    char *p = buf;
    int x, cnt = 0;

    for ( cnt = XBI-1; cnt > 0; cnt-- )
	if ( bits->bits[cnt] )
	    break;
    for ( x = 0; x <= cnt; x++ )
    {
	sprintf(p, "%d", bits->bits[x]);
	p += strlen(p);
	if ( x < cnt )
	    *p++ = '&';
    }
    *p = '\0';

    return buf;
}

/*
 * Write an extended bitvector to a file			-Thoric
 */
void fwrite_bitvector( EXT_BV *bits, FILE *fp )
{
    fputs( print_bitvector(bits), fp );
}


EXT_BV meb( int bit )
{
    EXT_BV bits;

    xCLEAR_BITS(bits);
    if ( bit >= 0 )
	xSET_BIT(bits, bit);

    return bits;
}


EXT_BV multimeb( int bit, ... )
{
    EXT_BV bits;
    va_list param;
    int b;

    xCLEAR_BITS(bits);
    if ( bit < 0 )
	return bits;

    xSET_BIT(bits, bit);

    va_start(param, bit);

    while ((b = va_arg(param, int)) != -1)
	xSET_BIT(bits, b);

    va_end(param);

    return bits;
}



/* Strip the color codes in a string and return it without them -- Scion 
*/
char *munch_colors(char *word) {
        char buf[MAX_STRING_LENGTH];
        int i;
        int a;

        i = 0;
        a = 0;

        while (word[i] != '\0' && i < MAX_STRING_LENGTH) {
                if (word[i] != '&' && (i==0 || word[i-1] != '&')) {
                        buf[a] = word[i];
                        a++;
                }
                i++;
        }

        buf[a] = '\0';

        word = STRALLOC(buf);

        return word;
}

