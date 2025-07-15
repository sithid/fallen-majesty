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


/* Special Bits */



#define SPC_CHAMPION	1 /* PLR_CHAMPION 4 */
#define SPC_DEMON_LORD	2 /* OLD_DEMON Flag */
#define SPC_WOLFMAN	4 /* old PLR_WOLFMAN */
#define SPC_PRINCE      8 /* old EXTRA_PRINCE */
#define SPC_SIRE	16 /* Old EXTRA_SIRE */
#define SPC_ANARCH      32 /* old extra_anarch */
#define SPC_INCONNU     64 /* old extra_inconnu */
#define SPC_DROW_WAR	128
#define SPC_DROW_MAG	256
#define SPC_DROW_CLE	512
#define SPC_ROGUE       1024
#define SPC_GENERAL	2048 /* Kingdom General */
#define SPC_PALADIN_EYE	4096
#define SPC_NOTITLE	8192
#define SPC_ALLOW_CHAT  16384
#define SPC_NEWBIE      32768
#define SPC_MASTERY2	65536

#define SPC_TORTURE	131072  // 
#define SPC_TORMENT	262144  // Inquisitor's 
#define SPC_CHAINS	524288  //  courtesy
#define SPC_FURY	1048576 //   of
#define SPC_STAFF	2097152 //  Rand
/* Class Bits */


#define CLASS_DEMON	   1 
#define CLASS_MAGE	   2 
#define CLASS_WEREWOLF	   4 
#define CLASS_VAMPIRE	   8 
#define CLASS_HIGHLANDER  16 
#define CLASS_DROW	  32
#define CLASS_MONK 	  64
#define CLASS_NINJA      128
#define CLASS_POWER_LICH 256
#define CLASS_SHAPESHIFTER  512
#define CLASS_TANARRI   1024 
#define CLASS_ANGEL      2048 
#define CLASS_UNDEAD_KNIGHT 4096 
#define CLASS_CYBORG    8192
#define CLASS_PALADIN     16384
#define CLASS_PSIONIST  32768
#define CLASS_INQUISITOR    65536  
#define CLASS_NECROMANCER	131072
#define CLASS_HUNTER CLASS_NECROMANCER * 2
extern const struct flag_type class_flags[];

/*
 * Bits or defines for Lichs
 */
#define CON_LORE     1
#define LIFE_LORE    2
#define DEATH_LORE   3
#define CHAOS_MAGIC  4
#define NECROMANTIC  5
#define GOLEMS_SUMMON 6

#define HAS_SUMMONED_IRON  1
#define HAS_SUMMONED_STONE 2
#define HAS_SUMMONED_CLAY  4
#define HAS_SUMMONED_FIRE  8

/* 
 * Bits For Highlanders
 */

#define HPOWER_WPNSKILL 9

/*
 * Angel
 */

#define ANGEL_PEACE	1
#define ANGEL_LOVE	2
#define ANGEL_JUSTICE	3
#define ANGEL_HARMONY	4
#define ANGEL_POWERS	5
#define ANGEL_PEACE_COUNTER 6

#define ANGEL_WINGS	1
#define ANGEL_HALO	2
#define ANGEL_AURA	4
#define ANGEL_EYE	8

/*
 * Inquisitor
 */
/*
 * Shapeshifters
 */

#define BULL_FORM	1
#define HYDRA_FORM	2
#define FAERIE_FORM	4
#define TIGER_FORM	8

#define BULL_LEVEL	1
#define HYDRA_LEVEL	2
#define FAERIE_LEVEL	3
#define TIGER_LEVEL	4
#define SHAPE_FORM	5
#define SHAPE_COUNTER	6
#define SHAPE_POWERS	7
#define PHASE_COUNTER   8

/*
 * Death Knights
 */

#define NECROMANCY	1
#define INVOCATION	2
#define UNDEAD_SPIRIT	3
#define WEAPONSKILL	4
#define AURAS		5
#define POWER_TICK	6

#define BOG_AURA	1
#define DEATH_AURA	2
#define FEAR_AURA	4
#define MIGHT_AURA	8

#define NPOWER_SORA 1
#define NPOWER_CHIKYU 2
#define NPOWER_NINGENNO 3
#define HARA_KIRI 5

/*
 * Necromancer by Huw
 */

#define NEC_SIGHT 1
#define NEC_DEATH 2
#define NEC_POISON 4
#define NEC_SUMMON   8
#define NEC_LIFE   16

/*
 * Bits for Werewolves 
 */
/* Stats */





#define UNI_GEN        	0 /* vampgen */


#define UNI_AFF		1 /* vampaff */


#define UNI_CURRENT	2 /* vamppass */


#define UNI_RAGE	3 /* ch->wolf = how big rage they are in*/


#define UNI_FORM0      	4/* wolfform[0] */


#define UNI_FORM1      	5 /* wolfform[1] */





#define	WOLF_POLYAFF	6



/* 
 * Powers 
 * Totems for werewolves.
 */

#define WPOWER_MANTIS	      0


#define WPOWER_BEAR	      1


#define WPOWER_LYNX	      2


#define WPOWER_BOAR	      3


#define WPOWER_OWL	      4


#define WPOWER_SPIDER	      5


#define WPOWER_WOLF	      6


#define WPOWER_HAWK	      7


#define WPOWER_RAPTOR         8


#define WPOWER_LUNA           9


#define WPOWER_SILVER	     10


#define WPOWER_PAIN          11


#define WPOWER_CONGREGATION  12


#define VPOWER_ANIMAL         1


#define VPOWER_AUSPEX         2


#define VPOWER_CELERITY       3 


#define VPOWER_DOMINATE       4


#define VPOWER_FORTITUDE      5


#define VPOWER_OBFUSCATE      6


#define VPOWER_OBTENEBRATION  7


#define VPOWER_POTENCE        8


#define VPOWER_PRESENCE       9


#define VPOWER_PROTEAN       10


#define VPOWER_QUIETUS       11


#define VPOWER_SERPENTIS     12


#define VPOWER_THAU          13
#define VPOWER_VICISSITUDE   14
#define VPOWER_DAIM          15
#define VPOWER_NECROMANCY    16
#define VPOWER_THANATOSIS    17
#define VPOWER_MELPOMINEE    18
#define VPOWER_OBJ_VNUM      19


#define DROW_POWER	8


#define DROW_TOTAL	9


#define DROW_MAGIC	11


/* Psionist */
#define PSIONIST_NOVICE	 	1
#define PSIONIST_APPRENTICE 	2
#define PSIONIST_JOURNEYMAN	3
#define PSIONIST_PSIONIST	4
#define PSIONIST_MASTER		5
#define PSIONIST_MINDBENDER	6

#define PSIONIST_POWER		1
#define PSIONIST_POWER_COUNTER	2
#define PPOINTS			4
#define PSIONIST_WRATH_ON	8

#define PSIONIST_MINDBLAST	1
#define PSIONIST_TRUESIGHT	2
#define PSIONIST_MEDDLE		4
#define PSIONIST_MINDREAD	8
#define PSIONIST_MINDTRAP	16
#define PSIONIST_ENFEEBLE	32
#define PSIONIST_BODY		64
#define PSIONIST_MENTAL		128
#define PSIONIST_WRATH		256
#define PSIONIST_CONTROL	512
#define PSIONIST_PRISON		1024
#define PSIONIST_TRAVEL		2048
#define PSIONIST_QUICKNESS	4096
#define PSIONIST_AWE		8192
#define PSIONIST_ENERGY		16384
#define PSIONIST_CONFUSE	32768
#define PSIONIST_ENTRAPMENT	65536
#define PSIONIST_ASSASSINATE	131072
/* Tanar'ri */

#define TANARRI_FODDER   1
#define TANARRI_FIGHTER  2
#define TANARRI_ELITE    3
#define TANARRI_CAPTAIN  4
#define TANARRI_WARLORD  5
#define TANARRI_BALOR    6

#define TANARRI_POWER	        1
#define TANARRI_POWER_COUNTER	2
#define TANARRI_FURY_ON             3
#define TANARRI_QUAKE_TIMER	4
#define TPOINTS		 8

#define TANARRI_WEB		1
#define TANARRI_CLAWS		2
#define TANARRI_TRUESIGHT	4
#define TANARRI_EARTHQUAKE	8
#define TANARRI_EXOSKELETON	16
#define TANARRI_FANGS		32
#define TANARRI_TORNADO		64
#define TANARRI_SPEED		128
#define TANARRI_MIGHT		256
#define TANARRI_CHAOSGATE	512
#define TANARRI_FIERY		1024
#define TANARRI_FURY		2048
#define TANARRI_HEAD		4096
#define TANARRI_BOOMING		8192
#define TANARRI_ENRAGE		16384
#define TANARRI_FLAMES		32768
#define TANARRI_TENDRILS	65536
#define TANARRI_LAVA		131072
#define TANARRI_EMNITY		262144

/*Stats*/

#define DEMON_CURRENT		      8	/* POWER_CURRENT 0*/
#define DEMON_TOTAL		      9	/* POWER_TOTAL   1*/
#define DEMON_POWER		     10 /* TOTAL ARMOUR BOUNS */
#define DEMON_PPOWER		     11 /* ARMOUR BONUS FROM PLAYERS */


/*Powers*/


#define DPOWER_FLAGS		      0 /* C_POWERS  1 */
#define DPOWER_CURRENT		      1 /* C_CURRENT 2 */
#define DPOWER_HEAD		      2 /* C_HEAD    3 */
#define DPOWER_TAIL		      3 /* C_TAIL    4 */
#define DPOWER_OBJ_VNUM		      4 /* DISC[10]  5 */
#define VPOWER_CURRENT		     18

#define DEM_UNFOLDED		      0
#define DEM_FANGS		      1
#define DEM_CLAWS		      2
#define DEM_DAMN		      3
#define DEM_HORNS		      4
#define DEM_HOOVES		      8
#define DEM_EYES		     16
#define DEM_WINGS		     32
#define DEM_MIGHT		     64
#define DEM_TOUGH		    128


#define DEM_SPEED		    256


#define DEM_TRAVEL		    512


#define DEM_SCRY		   1024


#define DEM_SHADOWSIGHT		   2048


/* Object Powers */


#define DEM_MOVE		   4096   /* 1 Can roll as an object */


#define DEM_LEAP		   8192   /* 2 Can leap out of someone's hands */


#define DEM_MAGIC		  16384   /* 4 Can cast spells as an object */


#define DEM_LIFESPAN		  32768   /* 8 Can change lifespan */
#define DEM_LONGSWORD		  65536
#define DEM_HEAD		 131072


#define DEM_TAIL		 262144


#define DEM_SHIELD		 524288


#define DEM_TRUESIGHT		1048576


#define DEM_GRAFT		2097152
#define DEM_IMMOLATE		4194304
#define DEM_INFERNO		8388608
#define DEM_CAUST	       16777216
#define DEM_ENTOMB	       33554432
#define DEM_FREEZEWEAPON       67108864
#define DEM_UNNERVE	      134217728
#define DEM_LEECH	      268435456
#define DEM_BLINK	      536870912
#define DEM_SHORTSWORD       1073741824


#define HEAD_NORMAL		      0


#define HEAD_WOLF		      1


#define HEAD_EAGLE		      2


#define HEAD_LION		      4


#define HEAD_SNAKE		      8





#define TAIL_NONE		      0


#define TAIL_POISONOUS		      1


#define TAIL_SCORPION		      2




/* Drow Powers */

#define DPOWER_DROWFIRE		1
#define DPOWER_DARKNESS		2
#define DPOWER_LIGHTS		4
#define DPOWER_DROWSIGHT	8
#define DPOWER_LEVITATION	16
#define DPOWER_DROWSHIELD	32
#define DPOWER_DROWPOISON	64
#define DPOWER_SHADOWWALK	128
#define DPOWER_GAROTTE		256
#define DPOWER_ARMS			512
#define DPOWER_DROWHATE		1024
#define DPOWER_SPIDERFORM	2048
#define DPOWER_WEB			4096
#define DPOWER_DGAROTTE		8192
#define DPOWER_CONFUSE		16384
#define DPOWER_GLAMOUR		32768
#define DPOWER_EARTHSHATTER	65536
#define DPOWER_SPEED		131072
#define DPOWER_TOUGHSKIN	262144
#define DPOWER_DARKTENDRILS 524288
#define DPOWER_FIGHTDANCE	1048576
#define DPOWER_SHADOWBLAST      2097152

/*new demon powers*/


#define DEM_CONE		1


#define DEM_STRENGTH		2


#define DEM_FORM		3


#define DEM_STAKE		4


#define DEM_SPEEDY		5


#define DEM_TRUE		6


/* Monk Power Bit */


#define PMONK			0


/* Used to be Zerg Powers - now it is NEW_MAGE powers :-) - JOBO */
#define PINVOKE			0

#define PINQUISITOR 		0

/* Changling Powers */
#define REFORM		0
#define MULTIPLEARMS    1
#define CAMOFLAGE       2
#define CHCONSUME       3
#define SHIFTDRAGON     4

#define CYBORG_POWER	8

/*cyborg powers*/
#define CYBORG_FACE	0
#define CYBORG_LIMBS	1
#define CYBORG_BODY	2
#define CYBORG_ADAPT    3
