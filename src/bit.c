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


/***************************************************************************
 *  File: bit.c                                                            *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was written by Jason Dinkel and inspired by Russ Taylor,     *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
 ***************************************************************************/
/*
 The code below uses a table lookup system that is based on suggestions
 from Russ Taylor.  There are many routines in handler.c that would benefit
 with the use of tables.  You may consider simplifying your code base by
 implementing a system like below with such functions. -Jason Dinkel
*/



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



struct flag_stat_type
{
    const struct flag_type *structure;
    bool stat;
};



/*****************************************************************************
 Name:		flag_stat_table
 Purpose:	This table catagorizes the tables following the lookup
 		functions below into stats and flags.  Flags can be toggled
 		but stats can only be assigned.  Update this table when a
 		new set of flags is installed.
 ****************************************************************************/
const struct flag_stat_type flag_stat_table[] =
{
/*  {	structure		stat	}, */
    {	area_flags,		FALSE	},
    {   sex_flags,		TRUE	},
    {   exit_flags,		FALSE	},
    {   door_resets,		TRUE	},
    {   room_flags,		FALSE	},
    {   sector_flags,		TRUE	},
    {   type_flags,		TRUE	},
    {   extra_flags,		FALSE	},
    {   wear_flags,		FALSE	},
    {   act_flags,		FALSE	},
    {   affect_flags,		FALSE	},
    {   apply_flags,		TRUE	},
    {   wear_loc_flags,		TRUE	},
    {   wear_loc_strings,	TRUE	},
    {   weapon_flags,		TRUE	},
    {   container_flags,	FALSE	},
    {   liquid_flags,		TRUE	},
    {   class_flags,		FALSE	},
    {   legend_flags,		TRUE	},
    {   position_flags,		TRUE	},
    {   0,			0	}
};
    


/*****************************************************************************
 Name:		is_stat( table )
 Purpose:	Returns TRUE if the table is a stat table and FALSE if flag.
 Called by:	flag_value and flag_string.
 Note:		This function is local and used only in bit.c.
 ****************************************************************************/
bool is_stat( const struct flag_type *flag_table )
{
    int flag;

    for (flag = 0; flag_stat_table[flag].structure; flag++)
    {
	if ( flag_stat_table[flag].structure == flag_table
	  && flag_stat_table[flag].stat )
	    return TRUE;
    }
    return FALSE;
}



/*
 * This function is Russ Taylor's creation.  Thanks Russ!
 * All code copyright (C) Russ Taylor, permission to use and/or distribute
 * has NOT been granted.  Use only in this OLC package has been granted.
 */
/*****************************************************************************
 Name:		flag_lookup( flag, table )
 Purpose:	Returns the value of a single, settable flag from the table.
 Called by:	flag_value and flag_string.
 Note:		This function is local and used only in bit.c.
 ****************************************************************************/
int flag_lookup(const char *name, const struct flag_type *flag_table)
{
    int flag;
 
    for (flag = 0; *flag_table[flag].name; flag++)	/* OLC 1.1b */
    {
        if ( !str_cmp( name, flag_table[flag].name )
          && flag_table[flag].settable )
            return flag_table[flag].bit;
    }
 
    return NO_FLAG;
}



/*****************************************************************************
 Name:		flag_value( table, flag )
 Purpose:	Returns the value of the flags entered.  Multi-flags accepted.
 Called by:	olc.c and olc_act.c.
 ****************************************************************************/
int flag_value( const struct flag_type *flag_table, char *argument)
{
    char word[MAX_INPUT_LENGTH];
    int  bit;
    int  marked = 0;
    bool found = FALSE;

    if ( is_stat( flag_table ) )
    {
	one_argument( argument, word );

	if ( ( bit = flag_lookup( word, flag_table ) ) != NO_FLAG )
	    return bit;
	else
	    return NO_FLAG;
    }

    /*
     * Accept multiple flags.
     */
    for (; ;)
    {
        argument = one_argument( argument, word );

        if ( word[0] == '\0' )
	    break;

        if ( ( bit = flag_lookup( word, flag_table ) ) != NO_FLAG )
        {
            SET_BIT( marked, bit );
            found = TRUE;
        }
    }

    if ( found )
	return marked;
    else
	return NO_FLAG;
}



/*****************************************************************************
 Name:		flag_string( table, flags/stat )
 Purpose:	Returns string with name(s) of the flags or stat entered.
 Called by:	act_olc.c, olc.c, and olc_save.c.
 ****************************************************************************/
char *flag_string( const struct flag_type *flag_table, int bits )
{
    static char buf[512];
    int  flag;

    buf[0] = '\0';

    for (flag = 0; *flag_table[flag].name; flag++)	/* OLC 1.1b */
    {
	if ( !is_stat( flag_table ) && IS_SET(bits, flag_table[flag].bit) )
	{
	    strcat( buf, " " );
	    strcat( buf, flag_table[flag].name );
	}
	else
	if ( flag_table[flag].bit == bits )
	{
	    strcat( buf, " " );
	    strcat( buf, flag_table[flag].name );
	    break;
	}
    }
    return (buf[0] != '\0') ? buf+1 : "none";
}


char * const save_flag[] =
{ "unused1", "unused2", "passwd", "drop", "put", "give", "auto", "zap",
"unused3", "get", "receive", "idle", "backup", "quitbackup", "r14", "r15",
"r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", "r24", "r25", 
"r26",
"r27", "r28", "r29", "r30", "r31" };


const struct flag_type area_flags[] =
{
    {	"none",			AREA_NONE,		FALSE	},
    {	"changed",		AREA_CHANGED,		FALSE	},
    {	"added",		AREA_ADDED,		FALSE	},
    {	"loading",		AREA_LOADING,		FALSE	},
    {	"verbose",		AREA_VERBOSE,		FALSE	},
    {	"",			0,			0	}
};



const struct flag_type sex_flags[] =
{
    {	"male",			SEX_MALE,		TRUE	},
    {	"female",		SEX_FEMALE,		TRUE	},
    {	"neutral",		SEX_NEUTRAL,		TRUE	},
    {	"",			0,			0	}
};

const struct flag_type class_flags[] =
{
    {	"demon",		CLASS_DEMON,	TRUE	},
    {	"mage",			CLASS_MAGE,	TRUE	},
    {	"werewolf",		CLASS_WEREWOLF, TRUE	},
    {   "vampire",		CLASS_VAMPIRE,  TRUE	},
    {   "samurai",		CLASS_HIGHLANDER, TRUE	},
    {   "drow",			CLASS_DROW,	TRUE	},
    {   "monk",			CLASS_MONK,	TRUE	},
    {   "ninja",		CLASS_NINJA,	TRUE	},
    {   "powerlich",		CLASS_POWER_LICH, TRUE	},
    {   "shapeshifter",		CLASS_SHAPESHIFTER, TRUE	},
    {   "tanarri",		CLASS_TANARRI, TRUE	},
    {   "angel",		CLASS_ANGEL, TRUE	},
    {   "uk",			CLASS_UNDEAD_KNIGHT, TRUE},
    {   "drider",		CLASS_CYBORG, 	TRUE	},
    {   "paladin",		CLASS_PALADIN,	TRUE	},
    {   "psionist",		CLASS_PSIONIST, TRUE	},
    {   "inqusitor",		CLASS_INQUISITOR,TRUE   },
    {	"",			0,		0	}
};

const struct flag_type position_flags[] =
{
   {   "Dead", POS_DEAD, FALSE },
   {   "Mortally Wounded", POS_MORTAL, FALSE },
   {   "Incapacitated", POS_INCAP, FALSE },
   {   "Stunned", POS_STUNNED, FALSE },
   {   "Sleeping", POS_SLEEPING, FALSE },
   {   "Meditating", POS_MEDITATING, FALSE },
   {   "Sitting", POS_SITTING, FALSE },
   {   "Resting", POS_RESTING, FALSE },
   {   "Fighting", POS_FIGHTING, FALSE },
   {   "Standing", POS_STANDING, FALSE },
   {   "",         0,            0     }
};

const struct flag_type legend_flags[] =
{
   {   "#cU#Cn#7k#0n#7o#Cw#cn#n ", L_UNK, FALSE },
   {   "#GZ#ge#7al#8ot  ", L_CIT, FALSE },
   {   "#8Ch#La#lmp#Lio#8n", L_LEG,  FALSE },
   {   "#gH#Ge#yro    ", L_MYT,  FALSE },
   {   "#RM#ra#or#yt#Ry#rr  ", L_TIT,  FALSE },
   {   "#yG#ou#7a#8rd#7i#oa#yn", L_ORA,  FALSE },
   {   "#8S#7a#yi#7n#8t   ", L_IMM,  FALSE },
   {   "#RS#ra#8vi#ro#Rr  ", L_DEM, FALSE },
   {   "#CM#cy#8t#ch    ", L_DEI, FALSE },
   {   "#cL#8e#7g#ce#8n#7d  ", L_ALM, FALSE },
    {   "",                     0,              0       }
};

const struct flag_type exit_flags[] =
{
    {   "door",			EX_ISDOOR,		TRUE    },
    {	"closed",		EX_CLOSED,		TRUE	},
    {	"locked",		EX_LOCKED,		TRUE	},
    {	"pickproof",		EX_PICKPROOF,		TRUE	},
    { 	"ice_wall",		EX_ICE_WALL,		TRUE	},
    { 	"fire_wall",		EX_FIRE_WALL,		TRUE	},
    { 	"sword_wall",		EX_SWORD_WALL,		TRUE	},
    { 	"prismatic_wall",	EX_PRISMATIC_WALL,	TRUE	},
    { 	"iron_wall",		EX_IRON_WALL,		TRUE	},
    { 	"mushroom_wall",	EX_MUSHROOM_WALL,	TRUE	},
    { 	"caltrop_wall",		EX_CALTROP_WALL,	TRUE	},
    { 	"ash_wall",		EX_ASH_WALL,		TRUE	},
    { 	"warding",		EX_WARDING,		TRUE	},
    {   "hidden",		EX_HIDDEN,		TRUE	},
    {	"",			0,			0	}
};



const struct flag_type door_resets[] =
{
    {	"open and unlocked",	0,		TRUE	},
    {	"closed and unlocked",	1,		TRUE	},
    {	"closed and locked",	2,		TRUE	},
    {	"",			0,		0	}
};



const struct flag_type room_flags[] =
{
    {	"dark",			ROOM_DARK,		TRUE	},
    {	"no_mob",		ROOM_NO_MOB,		TRUE	},
    {	"indoors",		ROOM_INDOORS,		TRUE	},
    {	"private",		ROOM_PRIVATE,		TRUE	},
    {	"safe",			ROOM_SAFE,		TRUE	},
    {   "damage",               ROOM_DAMAGE,            TRUE    },
    {   "healing",		ROOM_HEALING,		TRUE	},
    {	"solitary",		ROOM_SOLITARY,		TRUE	},
    {	"pet_shop",		ROOM_PET_SHOP,		TRUE	},
    {	"no_recall",		ROOM_NO_RECALL,		TRUE	},
	{   "no_chant",         ROOM_NO_CHANT,      TRUE    },
    {	"no_otrans",		ROOM_NO_OTRANS,		TRUE	},
    { 	"no_teleport",		ROOM_NO_TELEPORT,	TRUE	},
    {  	"total_darkness",	ROOM_TOTAL_DARKNESS,	TRUE	},
    { 	"blade_barrier",	ROOM_BLADE_BARRIER,	TRUE	},
    { 	"arena",		ROOM_ARENA,		TRUE	},
    { 	"flaming",		ROOM_FLAMING,		TRUE	},
    {   "silence",		ROOM_SILENCE,		TRUE	},
    {   "astral",		ROOM_ASTRAL,		TRUE	},
    {   "no_quit",		ROOM_NO_QUIT,		TRUE	},
    {   "orgy",			ROOM_SEX,		TRUE    },
    {   "no_ft",		ROOM_NO_FT,		TRUE	},
    {   "duel",			ROOM_DUEL,		TRUE	},
    {   "",			0,			0       }
};



const struct flag_type sector_flags[] =
{
    {	"inside",	SECT_INSIDE,		TRUE	},
    {	"city",		SECT_CITY,		TRUE	},
    {	"field",	SECT_FIELD,		TRUE	},
    {	"forest",	SECT_FOREST,		TRUE	},
    {	"hills",	SECT_HILLS,		TRUE	},
    {	"mountain",	SECT_MOUNTAIN,		TRUE	},
    {	"swim",		SECT_WATER_SWIM,	TRUE	},
    {	"noswim",	SECT_WATER_NOSWIM,	TRUE	},
    {	"air",		SECT_AIR,		TRUE	},
    {	"desert",	SECT_DESERT,		TRUE	},
    {	"",		0,			0	}
};



const struct flag_type type_flags[] =
{
    {	"light",		ITEM_LIGHT,		TRUE	},
    {	"scroll",		ITEM_SCROLL,		TRUE	},
    {	"wand",			ITEM_WAND,		TRUE	},
    {	"staff",		ITEM_STAFF,		TRUE	},
    {	"weapon",		ITEM_WEAPON,		TRUE	},
    {	"treasure",		ITEM_TREASURE,		TRUE	},
    {	"instrument",		ITEM_INSTRUMENT,	TRUE	},
    {	"armor",		ITEM_ARMOR,		TRUE	},
    {	"potion",		ITEM_POTION,		TRUE	},
    {	"furniture",		ITEM_FURNITURE,		TRUE	},
    {	"trash",		ITEM_TRASH,		TRUE	},
    {	"container",		ITEM_CONTAINER,		TRUE	},
    {	"drink-container",	ITEM_DRINK_CON,		TRUE	},
    {	"key",			ITEM_KEY,		TRUE	},
    {	"food",			ITEM_FOOD,		TRUE	},
    {	"money",		ITEM_MONEY,		TRUE	},
    {	"boat",			ITEM_BOAT,		TRUE	},
    {	"npc corpse",		ITEM_CORPSE_NPC,	TRUE	},
    {	"pc corpse",		ITEM_CORPSE_PC,		FALSE	},
    {	"fountain",		ITEM_FOUNTAIN,		TRUE	},
    {	"pill",			ITEM_PILL,		TRUE	},
    { 	"portal",		ITEM_PORTAL,		TRUE	},
    {	"egg",			ITEM_EGG,		TRUE	},
    {	"voodoo",		ITEM_VOODOO,		TRUE	},
    {	"stake",		ITEM_STAKE,		TRUE	},
    { 	"missile",		ITEM_MISSILE,		TRUE	},
    {	"ammo",			ITEM_AMMO,		TRUE	},
    { 	"quest",		ITEM_QUEST,		TRUE	},
    {	"questcard",		ITEM_QUESTCARD,		TRUE	},
    {	"questmachine",		ITEM_QUESTMACHINE,	TRUE	},
    { 	"symbol",		ITEM_SYMBOL,		TRUE	},
    {   "book",			ITEM_BOOK,		TRUE	},
    {	"page",			ITEM_PAGE,		TRUE	},
    {	"tool",			ITEM_TOOL,		TRUE	},
    {	"wall",			ITEM_WALL,		TRUE	},
    {	"copper",		ITEM_COPPER,		TRUE	},
    {	"iron",			ITEM_IRON,		TRUE	},
    {	"steel",		ITEM_STEEL,		TRUE	},
    { 	"adamantite",		ITEM_ADAMANTITE,	TRUE	},
    {   "electrum",             ITEM_ELECTRUM,          TRUE    },
    {   "gemstone",		ITEM_GEMSTONE,		TRUE	},
    {	"hilt",			ITEM_HILT,		TRUE	},
    {   "kingdom",		ITEM_KINGDOM_POWER,	TRUE	},
    {	"bank",			ITEM_BANK,		TRUE	},
    {   "well",			ITEM_WELL,		TRUE	},
    {   "statue",		ITEM_STATUE,		TRUE	},
    {   "teleporter",		ITEM_TELEPORTER,	TRUE	},
    {   "quest-IOU",		ITEM_IOU,		TRUE	},
    {   "vending",		ITEM_MACHINE,		TRUE	},
    {   "exp-token",		ITEM_EXP_TOKEN,		TRUE	},
    {	"",			0,			0	}
};


const struct flag_type extra_flags[] =
{
    {	"glow",			ITEM_GLOW,		TRUE	},
    {	"hum",			ITEM_HUM,		TRUE	},
    {	"thrown",		ITEM_THROWN,		TRUE	},
    {	"keep",			ITEM_KEEP,		TRUE	},
    {	"vanish",		ITEM_VANISH,		TRUE	},
    {	"invis",		ITEM_INVIS,		TRUE	},
    {	"magic",		ITEM_MAGIC,		TRUE	},
    {	"nodrop",		ITEM_NODROP,		TRUE	},
    {	"bless",		ITEM_BLESS,		TRUE	},
    {	"anti-good",		ITEM_ANTI_GOOD,		TRUE	},
    {	"anti-evil",		ITEM_ANTI_EVIL,		TRUE	},
    {	"anti-neutral",		ITEM_ANTI_NEUTRAL,	TRUE	},
    {	"noremove",		ITEM_NOREMOVE,		TRUE	},
    {	"inventory",		ITEM_INVENTORY,		TRUE	},
    {	"shadowplane",		ITEM_SHADOWPLANE,	TRUE	},
    { 	"loyal",		ITEM_LOYAL,		TRUE	},
    {   "nosacrifice",		ITEM_NOSAC,		TRUE	},
    {	"",			0,			0	}
};



const struct flag_type wear_flags[] =
{
    {	"take",			ITEM_TAKE,		TRUE	},
    {	"finger",		ITEM_WEAR_FINGER,	TRUE	},
    {	"neck",			ITEM_WEAR_NECK,		TRUE	},
    {	"body",			ITEM_WEAR_BODY,		TRUE	},
    {	"head",			ITEM_WEAR_HEAD,		TRUE	},
    {	"legs",			ITEM_WEAR_LEGS,		TRUE	},
    {	"feet",			ITEM_WEAR_FEET,		TRUE	},
    {	"hands",		ITEM_WEAR_HANDS,	TRUE	},
    {	"arms",			ITEM_WEAR_ARMS,		TRUE	},
    {	"shield",		ITEM_WEAR_SHIELD,	TRUE	},
    {	"about",		ITEM_WEAR_ABOUT,	TRUE	},
    {	"waist",		ITEM_WEAR_WAIST,	TRUE	},
    {	"wrist",		ITEM_WEAR_WRIST,	TRUE	},
    {	"wield",		ITEM_WIELD,		TRUE	},
    {	"hold",			ITEM_HOLD,		TRUE	},
    {	"face",			ITEM_WEAR_FACE,		TRUE	},
    {	"mastery",		ITEM_WEAR_MASTERY,	TRUE	},
    {   "tattoo",		ITEM_WEAR_TATTOO,	TRUE	},
    {   "earring",		ITEM_WEAR_EAR,		TRUE	},
    {   "",                     0,                      0       }
};



const struct flag_type act_flags[] =
{
    {	"npc",			ACT_IS_NPC,		FALSE	},
    {	"sentinel",		ACT_SENTINEL,		TRUE	},
    {	"scavenger",		ACT_SCAVENGER,		TRUE	},
    {	"aggressive",		ACT_AGGRESSIVE,		TRUE	},
    {	"stay_area",		ACT_STAY_AREA,		TRUE	},
    {	"wimpy",		ACT_WIMPY,		TRUE	},
    {	"pet",			ACT_PET,		TRUE	},
    {	"train",		ACT_TRAIN,		TRUE	},
    {	"practice",		ACT_PRACTICE,		TRUE	},
    {	"mount",		ACT_MOUNT,		TRUE	},
    {	"no_parts",		ACT_NOPARTS,		TRUE	},
    {	"no_exp",		ACT_NOEXP,		TRUE	},
    {   "no_autokill",		ACT_NOAUTOKILL,		TRUE	},
    {   "aggressive_neutral",	ACT_AGG_NEUTRAL,	TRUE	},
    {   "aggressive_good",	ACT_AGG_GOOD,		TRUE	},
    {   "aggressive_evil",	ACT_AGG_EVIL,		TRUE	},
    {	"",			0,			0	}
};



const struct flag_type affect_flags[] =
{
    {	"blind",		AFF_BLIND,		TRUE	},
    {	"invisible",		AFF_INVISIBLE,		TRUE	},
    {	"detect-evil",		AFF_DETECT_EVIL,	TRUE	},
    {	"detect-invis",		AFF_DETECT_INVIS,	TRUE	},
    {	"detect-magic",		AFF_DETECT_MAGIC,	TRUE	},
    {	"detect-hidden",	AFF_DETECT_HIDDEN,	TRUE	},
    {	"shadowplane",		AFF_SHADOWPLANE,	TRUE	},
    {	"sanctuary",		AFF_SANCTUARY,		TRUE	},
    {	"faerie-fire",		AFF_FAERIE_FIRE,	TRUE	},
    {	"infrared",		AFF_INFRARED,		TRUE	},
    {	"curse",		AFF_CURSE,		TRUE	},
    {	"flaming",		AFF_FLAMING,		TRUE	},
    {	"poison",		AFF_POISON,		TRUE	},
    {	"protect",		AFF_PROTECT,		TRUE	},
    {	"ethereal",		AFF_ETHEREAL,		TRUE	},
    {	"sneak",		AFF_SNEAK,		TRUE	},
    {	"hide",			AFF_HIDE,		TRUE	},
    {	"sleep",		AFF_SLEEP,		TRUE	},
    {	"charm",		AFF_CHARM,		TRUE	},
    {	"flying",		AFF_FLYING,		TRUE	},
    {	"pass-door",		AFF_PASS_DOOR,		TRUE	},
    {	"polymorph",		AFF_POLYMORPH,		FALSE	},
    {	"shadowsight",		AFF_SHADOWSIGHT,	TRUE	},
    {	"web",			AFF_WEBBED,		TRUE	},
//    {	"tendrils",		AFF_TENDRILS,		TRUE	},
    {	"drowfire",		AFF_DROWFIRE,		TRUE	},
    {	"zuloform",		AFF_ZULOFORM,		FALSE	},
    {	"peace",		AFF_PEACE,		TRUE	},
    {	"infirmity",		AFF_INFIRMITY,		TRUE	},
    {	"shift",		AFF_SHIFT,		FALSE	},
    {	"",			0,			0	}
};


/*
 * Used when adding an affect to tell where it goes.
 * See addaffect and delaffect in act_olc.c
 */
const struct flag_type apply_flags[] =
{
    {	"none",			APPLY_NONE,		TRUE	},
    {	"strength",		APPLY_STR,		TRUE	},
    {	"dexterity",		APPLY_DEX,		TRUE	},
    {	"intelligence",		APPLY_INT,		TRUE	},
    {	"wisdom",		APPLY_WIS,		TRUE	},
    {	"constitution",		APPLY_CON,		TRUE	},
    {	"sex",			APPLY_SEX,		TRUE	},
    {	"class",		APPLY_CLASS,		TRUE	},
    {	"level",		APPLY_LEVEL,		TRUE	},
    {	"age",			APPLY_AGE,		TRUE	},
    {	"height",		APPLY_HEIGHT,		TRUE	},
    {	"weight",		APPLY_WEIGHT,		TRUE	},
    {	"mana",			APPLY_MANA,		TRUE	},
    {	"hp",			APPLY_HIT,		TRUE	},
    {	"move",			APPLY_MOVE,		TRUE	},
    {	"gold",			APPLY_GOLD,		TRUE	},
    {	"experience",		APPLY_EXP,		TRUE	},
    {	"ac",			APPLY_AC,		TRUE	},
    {	"hitroll",		APPLY_HITROLL,		TRUE	},
    {	"damroll",		APPLY_DAMROLL,		TRUE	},
    {	"saving-para",		APPLY_SAVING_PARA,	TRUE	},
    {	"saving-rod",		APPLY_SAVING_ROD,	TRUE	},
    {	"saving-petri",		APPLY_SAVING_PETRI,	TRUE	},
    {	"saving-breath",	APPLY_SAVING_BREATH,	TRUE	},
    {	"saving-spell",		APPLY_SAVING_SPELL,	TRUE	},
    {	"polymorph",		APPLY_POLY,		TRUE	},
    {	"",			0,			0	}
};



/*
 * What is seen.
 */
const struct flag_type wear_loc_strings[] =
{
    {	"in the inventory",	WEAR_NONE,	TRUE	},
    {	"as a light",		WEAR_LIGHT,	TRUE	},
    {	"on the left finger",	WEAR_FINGER_L,	TRUE	},
    {	"on the right finger",	WEAR_FINGER_R,	TRUE	},
    {	"around the neck (1)",	WEAR_NECK_1,	TRUE	},
    {	"around the neck (2)",	WEAR_NECK_2,	TRUE	},
    {	"on the body",		WEAR_BODY,	TRUE	},
    {	"over the head",	WEAR_HEAD,	TRUE	},
    {	"on the legs",		WEAR_LEGS,	TRUE	},
    {	"on the feet",		WEAR_FEET,	TRUE	},
    {	"on the hands",		WEAR_HANDS,	TRUE	},
    {	"on the arms",		WEAR_ARMS,	TRUE	},
    {	"about the body",	WEAR_ABOUT,	TRUE	},
    {	"around the waist",	WEAR_WAIST,	TRUE	},
    {	"on the left wrist",	WEAR_WRIST_L,	TRUE	},
    {	"on the right wrist",	WEAR_WRIST_R,	TRUE	},
    {	"on the face",		WEAR_FACE,	TRUE	},
    {	"as a shield",		WEAR_SHIELD,	TRUE	},
    {	"first hand",		WEAR_WIELD,	TRUE	},
    {	"second hand",		WEAR_HOLD,	TRUE	},
    {	"third hand",		WEAR_THIRD,	TRUE	},
    {	"fourth hand",		WEAR_FOURTH,	TRUE	},
    {	"left scabbard",	WEAR_SCABBARD_L,TRUE	},
    {	"right scabbard",	WEAR_SCABBARD_R,TRUE	},
    {   "as the mastery",	WEAR_MASTERY,	TRUE	},
    {   "as the tattoo",	WEAR_TATTOO,	TRUE	},
    {   "in the left ear",   	WEAR_EAR_LEFT,  TRUE    },
    {   "in the right ear",  	WEAR_EAR_RIGHT,  TRUE    },
    {	"",			0			
}
};


/*
 * What is typed.
 * Neck2 should not be settable for loaded mobiles.
 */
const struct flag_type wear_loc_flags[] =
{
    {	"none",		WEAR_NONE,	TRUE	},
    {	"light",	WEAR_LIGHT,	TRUE	},
    {	"lfinger",	WEAR_FINGER_L,	TRUE	},
    {	"rfinger",	WEAR_FINGER_R,	TRUE	},
    {	"neck",		WEAR_NECK_1,	TRUE	},
    {	"body",		WEAR_BODY,	TRUE	},
    {	"head",		WEAR_HEAD,	TRUE	},
    {	"legs",		WEAR_LEGS,	TRUE	},
    {	"feet",		WEAR_FEET,	TRUE	},
    {	"hands",	WEAR_HANDS,	TRUE	},
    {	"arms",		WEAR_ARMS,	TRUE	},
    {	"shield",	WEAR_SHIELD,	TRUE	},
    {	"about",	WEAR_ABOUT,	TRUE	},
    {	"waist",	WEAR_WAIST,	TRUE	},
    {	"lwrist",	WEAR_WRIST_L,	TRUE	},
    {	"rwrist",	WEAR_WRIST_R,	TRUE	},
    {	"wielded",	WEAR_WIELD,	TRUE	},
    {	"hold",		WEAR_HOLD,	TRUE	},
    {	"face",		WEAR_FACE,	TRUE	},
    {	"mastery",	WEAR_MASTERY,	TRUE	},
    {   "tattoo",	WEAR_TATTOO,    TRUE	},
    {   "earring_l",	WEAR_EAR_LEFT,	TRUE	},
    {   "earring_r",    WEAR_EAR_RIGHT, TRUE    },
    {	"",		0,		0	}
};



const struct flag_type weapon_flags[] =
{
    {	"hit",		0,	TRUE	},
    {	"slice",	1,	TRUE	},
    {	"stab",		2,	TRUE	},
    {	"slash",	3,	TRUE	},
    {	"whip",		4,	TRUE	},
    {	"claw",		5,	TRUE	},
    {	"blast",	6,	TRUE	},
    {	"pound",	7,	TRUE	},
    {	"crush",	8,	TRUE	},
    {	"grep",		9,	TRUE	},
    {	"bite",		10,	TRUE	},
    {	"pierce",	11,	TRUE	},
    {	"suck",		12,	TRUE	},
    {	"",		0,	0	}
};


const struct flag_type container_flags[] =
{
    {	"closeable",		1,		TRUE	},
    {	"pickproof",		2,		TRUE	},
    {	"closed",		4,		TRUE	},
    {	"locked",		8,		TRUE	},
    {	"",			0,		0	}
};


const struct flag_type liquid_flags[] =
{
    {	"water",		0,	TRUE	},
    {	"beer",			1,	TRUE	},
    {	"wine",			2,	TRUE	},
    {	"ale",			3,	TRUE	},
    {	"dark-ale",		4,	TRUE	},
    {	"whisky",		5,	TRUE	},
    {	"lemonade",		6,	TRUE	},
    {	"firebreather",		7,	TRUE	},
    {	"local-specialty",	8,	TRUE	},
    {	"slime-mold-juice",	9,	TRUE	},
    {	"milk",			10,	TRUE	},
    {	"tea",			11,	TRUE	},
    {	"coffee",		12,	TRUE	},
    {	"blood",		13,	TRUE	},
    {	"salt-water",		14,	TRUE	},
    {	"cola",			15,	TRUE	},
    {	"cum",			16,	TRUE	},
    {	"cunt-juice",		17,	TRUE	},
    {	"urine",		18,	TRUE	},
    {	"",			0,	0	}
};

char *string_flag( int bitvector, char * const flagarray[] )
{
    static char buf[MAX_STRING_LENGTH];
    int x;

    buf[0] = '\0';
    for ( x = 0; x < 32 ; x++ ) {
      if ( IS_SET( bitvector, 1 << x ) )
      {
        strcat( buf, flagarray[x] );
        strcat( buf, " " );
      }
    }

    if ( (x=strlen( buf )) > 0 )
      buf[--x] = '\0';

    return buf;
}

char *ext_flag_string( EXT_BV *bitvector, char * const flagarray[] )
{
    static char buf[MAX_STRING_LENGTH];
    int x;

    buf[0] = '\0';
    for ( x = 0; x < MAX_BITS ; x++ ) {
	if ( xIS_SET( *bitvector, x ) )
	{
	    strcat( buf, flagarray[x] );
	    strcat( buf, " " );
	}
    }
    if ( (x=strlen(buf)) > 0 )
	buf[--x] = '\0';
    
    return buf;
}

char *ext_string_flag( const struct flag_type *flag_table, EXT_BV bits )
{
    static char buf[512];
    int  flag;

    buf[0] = '\0';

    for (flag = 0; *flag_table[flag].name; flag++)      /* OLC 1.1b */
    {
        if ( !is_stat( flag_table ) && xIS_SET(bits, flag ) )
        {
            strcat( buf, " " );
            strcat( buf, flag_table[flag].name );
        }
/*      else
        if ( flag_table[flag].bit == bits )
        {
            strcat( buf, " " );
            strcat( buf, flag_table[flag].name );
            break;
        } */
    }
    return (buf[0] != '\0') ? buf+1 : "none";
}

int get_saveflag( char *name )
{
    int x;

    for ( x = 0; x < sizeof(save_flag) / sizeof(save_flag[0]); x++ )
      if ( !str_cmp( name, save_flag[x] ) )
        return x;
    return -1;
}
