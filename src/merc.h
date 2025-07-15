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

#include <stdlib.h>
#include <limits.h>
#if !defined(WIN32)
#include <sys/cdefs.h>
#endif
#include <sys/time.h>  
#include "monk.h"
#include "garou.h"
#include "memwatch.h"
#include "clans.h"
/*
 * Accommodate old non-Ansi compilers.
 */
#if defined(TRADITIONAL)
#define const
#define args( list )			( )
#define DECLARE_DO_FUN( fun )		void fun( )
#define DECLARE_SPEC_FUN( fun )		bool fun( )
#define DECLARE_SPELL_FUN( fun )	void fun( )
#else
#define args( list )			list
#define DECLARE_DO_FUN( fun )		DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )		SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun
#endif

#define CRASHGUARD
#define HASHSTR
/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	defined(_AIX)
#if	!defined(const)
#define const
#endif
typedef int				sh_int;
typedef int				bool;
#define unix
#else
typedef short int			sh_int;
typedef unsigned char			bool;
#endif

/*
 * This structure is used in bit.c to lookup flags and stats.
 */
struct flag_type
{
    char * name;
    int  bit;
    bool settable;
};

#include "player.h"
//#include "old.h"
/*
 * Structure types.
 */
typedef struct	affect_data		AFFECT_DATA;
typedef struct	area_data		AREA_DATA;
typedef struct	ban_data		BAN_DATA;
typedef struct	char_data		CHAR_DATA;
typedef struct  alias_data              ALIAS_DATA;
typedef struct  kingdom_desc		KINGDOM_DESC;
typedef struct newclan_data     	CLAN_DATA;
typedef struct  clan_member_data        CLAN_MEMBER;
typedef struct  l_board			LEADER_BOARD;
typedef struct  c_leader		CLASS_LEADER;
typedef struct  config_data		CONFIG_DATA;
typedef	struct	editor_data		EDITOR_DATA;
typedef struct  buf_type                BUFFER;
typedef struct  map_type                MAP_TYPE;

typedef struct  balance_data		BALANCE_DATA;
typedef struct	descriptor_data		DESCRIPTOR_DATA;
typedef struct	exit_data		EXIT_DATA;
typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
typedef struct	help_data		HELP_DATA;
typedef struct	kill_data		KILL_DATA;
typedef struct	mob_index_data		MOB_INDEX_DATA;
typedef struct  kingdom_data		KINGDOM_DATA;
typedef struct  war_data		WAR_DATA;
typedef struct	note_data		NOTE_DATA;
typedef struct	obj_data		OBJ_DATA;
typedef struct	obj_index_data		OBJ_INDEX_DATA;
typedef struct	pc_data			PC_DATA;
typedef struct	reset_data		RESET_DATA;
typedef struct	room_index_data		ROOM_INDEX_DATA;
typedef struct	shop_data		SHOP_DATA;
typedef struct	time_info_data		TIME_INFO_DATA;
typedef struct	weather_data		WEATHER_DATA;
//typedef struct  random_data             RANDOM_DATA;
typedef struct  system_data             SYSTEM_DATA;
typedef struct	extended_bitvector	EXT_BV;

typedef struct  disabled_data                 DISABLED_DATA;

#define MAX_TOTAL_EXP 100000000000

#define MAX_ALIAS                  30 //Jobo's Aliases By Rand

/* one disabled command */
struct disabled_data
{
      DISABLED_DATA *next; /* pointer to next node */
      struct cmd_type const *command; /* pointer to the command struct*/
      char *disabled_by; /* name of disabler */
      sh_int level; /* level of disabler */
};
extern                        DISABLED_DATA     *disabled_first;

struct buf_type
{
    BUFFER *    next;
    bool        valid;
    sh_int      state;  /* error state of the buffer */
    sh_int      size;   /* size in k */
    char *      string; /* buffer's string */
};

struct form_type
{
    char * 	lookname;
    char *      name;
    int         hit;
    int         dam;
    int         ac;
    int         hit_mod;
    bool        squishable;
    sh_int	    size;
    bool	    can_cast;
    bool	    can_fly;
    bool	    can_wear;
    int         carry_max;
    int         max_items;
    char *	    say_word;
    char *	    move_word;
    int	    xp_bonus;
    int	    polymorph_level;
    char *	    in_room_desc;
    char *	    short_desc;
    bool	    can_use_claws;
};


struct clanrank_type
{
    char *      rank;                   /* rank name */
    int         min_level;              /* minimum level to gain that rank */
    char *	male_name;		/* rank name for males */
    char * 	female_name;		/* use some imagination */
};

struct race_type
{
    char *      name;                   /* call name of the race */
    char *      ishname;                /* elvish, gnomish etc... */
    bool        pc_race;                /* can be chosen by pcs */
    long        act;                    /* act bits for the race */
    long        aff;                    /* aff bits for the race */
    long        off;                    /* off bits for the race */
    long        imm;                    /* imm bits for the race */
    long        res;                    /* res bits for the race */
    long        vuln;                   /* vuln bits for the race */
    long        form;                   /* default form flag for the race */
    long        parts;                  /* default parts for the race */
};

/*
 * Function types.
 */
typedef	void DO_FUN	args( ( CHAR_DATA *ch, char *argument ) );
typedef bool SPEC_FUN	args( ( CHAR_DATA *ch ) );
typedef void SPELL_FUN	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );



/*
 * String and memory management parameters.
 */
#define	MAX_KEY_HASH		 1024
#define MAX_STRING_LENGTH	 8192
#define MAX_INPUT_LENGTH	 8192

/* I am lazy :) */
#define MSL MAX_STRING_LENGTH
#define MIL MAX_INPUT_LENGTH

//Added by Vampsta
void non_verbose_asave_changed args(( ));

/* 32bit bitvector defines */
#define BV00            (1 <<  0)
#define BV01            (1 <<  1)
#define BV02            (1 <<  2)
#define BV03            (1 <<  3)
#define BV04            (1 <<  4)
#define BV05            (1 <<  5)
#define BV06            (1 <<  6)
#define BV07            (1 <<  7)
#define BV08            (1 <<  8)
#define BV09            (1 <<  9)
#define BV10            (1 << 10)
#define BV11            (1 << 11)
#define BV12            (1 << 12)
#define BV13            (1 << 13)
#define BV14            (1 << 14)
#define BV15            (1 << 15)
#define BV16            (1 << 16)
#define BV17            (1 << 17)
#define BV18            (1 << 18)
#define BV19            (1 << 19)
#define BV20            (1 << 20)
#define BV21            (1 << 21)
#define BV22            (1 << 22)
#define BV23            (1 << 23)
#define BV24            (1 << 24)
#define BV25            (1 << 25)
#define BV26            (1 << 26)
#define BV27            (1 << 27)
#define BV28            (1 << 28)
#define BV29            (1 << 29)
#define BV30            (1 << 30)
#define BV31            (1 << 31)
/* 32 USED! DO NOT ADD MORE! */

/* 
 * Rotains Global Procedures
 */

void alias_remove   args( (CHAR_DATA *ch, ALIAS_DATA  *ali ) );//Jobo's Aliases By Rand
void clear_stats    args( (CHAR_DATA *ch) );
void room_is_total_darkness args( (ROOM_INDEX_DATA *pRoomIndex) );
void improve_wpn    args( (CHAR_DATA *ch,int dtype, int right_hand) );
void improve_stance args( (CHAR_DATA *ch) );
void skillstance    args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void show_spell     args( (CHAR_DATA *ch, int dtype) );
void fightaction    args( (CHAR_DATA *ch, CHAR_DATA *victim, int actype , \
int dtype, int wpntype) ); 
void crack_head     args( (CHAR_DATA *ch, OBJ_DATA *obj, char *argument) );
void critical_hit   args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam) );

void take_item      args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void raw_kill       args( (CHAR_DATA *victim) );
void trip           args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void disarm         args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void make_corpse    args( (CHAR_DATA *ch) );
void one_hit        args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, \
				int handtype) );
void make_part      args( (CHAR_DATA *ch, char *argument) );

void home_write             args( ( ) );

void behead                 args( (CHAR_DATA *victim) );
void paradox                args( (CHAR_DATA *ch) );
void gainarti	   args( (CHAR_DATA *ch) );
void load_kingshit args( (void) );
void save_kingshit args( (void) );
void load_greeting args( (void) );
void load_leaderboard args( (void) );
void save_leaderboard args( (void) );
void load_bans	args( (void) );
void save_bans	args( (void) );
void load_kingdoms args( (void) );
void save_kingdoms args( (void) );
void load_war	args( (void) );
void save_war	args( (void) );

/*
 * Class Leaderboard
 * By Rand
 */
void load_classleader	args((void));
void save_classleader	args((void));
void check_CL  		args( ( CHAR_DATA *ch, char *argument ) );
void clear_class_leader args( ( CHAR_DATA *ch, char *argument ) );

/* 
 * Godwars Game Parameters
 * By Rotain
 */

#define SKILL_ADEPT                75
#define SKILL_THAC0_32             18
#define SKILL_THAC0_00              6
#define VERSION_NUMBER              1
#define DONATION_ROOM_WEAPON     3207
#define DONATION_ROOM_ARMOR      3207
#define DONATION_ROOM_REST       3207
#define MAX_VAMPIRE_POWER           3 
#define MAX_CLAN                   11
#define MAX_DISCIPLINES		   44
#define MAX_ART                    12
#define MAX_FORGET		   10
#define MAX_KINGDOM		   10
#define MAX_WAR			   45 /* MAX_KINGDOM-1 * MAX_KINGDOM /2 total combos*/
#define MAX_SONGS		    1

/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */

/* Size of the map and depth of recursion to undertake */
#define MAPX     10
#define MAPY      8
#define MAXDEPTH  2

#define PARADOX_TICK               30
#define MAX_SKILL		  400
#define MAX_SPELL		   400
#define MAX_LEVEL		   12
#define NO_WATCH		   12
#define LEVEL_HERO		   (MAX_LEVEL - 9)
#define LEVEL_IMMORTAL		   (MAX_LEVEL - 5)

#define LEVEL_MORTAL		   (MAX_LEVEL - 10)
#define LEVEL_AVATAR		   (MAX_LEVEL - 9)
#define LEVEL_APPRENTICE	   (MAX_LEVEL - 8)
#define LEVEL_MAGE		   (MAX_LEVEL - 7)
#define LEVEL_ARCHMAGE		   (MAX_LEVEL - 6)
#define LEVEL_NINJA                (MAX_LEVEL - 6)
#define LEVEL_MONK                 (MAX_LEVEL - 6)
#define LEVEL_BUILDER		   (MAX_LEVEL - 5)
#define LEVEL_QUESTMAKER	   (MAX_LEVEL - 4)
#define LEVEL_ENFORCER		   (MAX_LEVEL - 3)
#define LEVEL_JUDGE		   (MAX_LEVEL - 2)
#define LEVEL_HIGHJUDGE		   (MAX_LEVEL - 1)
#define LEVEL_IMPLEMENTOR	   (MAX_LEVEL)
#define COLOUR_CHAR 		'#'
#define PULSE_PER_SECOND	    4
#define PULSE_VIOLENCE		  ( 3 * PULSE_PER_SECOND)
#define OLOAD_30333		    1
/* Save the database - OLC 1.1b */
#define PULSE_DB_DUMP             (1800* PULSE_PER_SECOND ) /* 30 minutes  */

/*
 * Defines for extended bitvectors
 */
#ifndef INTBITS
  #define INTBITS	32
#endif
#define XBM		31	/* extended bitmask   ( INTBITS - 1 )	*/
#define RSV		5	/* right-shift value  ( sqrt(XBM+1) )	*/
#define XBI		4	/* integers in an extended bitvector	*/
#define MAX_BITS	XBI * INTBITS
/*
 * Structure for extended bitvectors -- Thoric
 */
struct extended_bitvector
{
    int		bits[XBI];
};

/* Status levels, change here to modify - Vic */
/*
#define STAT_AVATAR                      1
#define STAT_IMMORTAL                   20
#define STAT_GODLING                    40
#define STAT_DEMIGOD                    60
#define STAT_LESSER                     80
#define STAT_GREATER                   100
#define STAT_SUPREME                   100
*/

#define PULSE_EMBRACE             ( 4 * PULSE_PER_SECOND)
#define PULSE_MOBILE		  ( 4 * PULSE_PER_SECOND)
#define PULSE_PLAYERS	 	  ( 4 * PULSE_PER_SECOND)
#define PULSE_TICK		  (30 * PULSE_PER_SECOND)
#define PULSE_AREA		  (60 * PULSE_PER_SECOND)
#define PULSE_WW                  ( 4 * PULSE_PER_SECOND)


struct artifact_type
{
  char * player_name;
  int    object_vnum;
};

#include "board.h"

/*
 * Site ban structure.
 */
struct	ban_data
{
    BAN_DATA *  next;
    BAN_DATA *  prev;
    char *      name;
    char *reason;
};



/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define SKY_CLOUDLESS		    0
#define SKY_CLOUDY		    1
#define SKY_RAINING		    2
#define SKY_LIGHTNING		    3

struct	time_info_data
{
    int		hour;
    int		day;
    int		month;
    int		year;
};

struct	weather_data
{
    int		mmhg;
    int		change;
    int		sky;
    int		sunlight;
};



/*
 * Directions.
 * Used in #ROOMS.
 */
typedef enum
{
  DIR_NORTH, DIR_EAST, DIR_SOUTH, DIR_WEST, DIR_UP, DIR_DOWN,
  DIR_NORTHEAST, DIR_NORTHWEST, DIR_SOUTHEAST, DIR_SOUTHWEST,
DIR_SOMEWHERE
} dir_types;
#define SUB_NORTH DIR_NORTH
#define SUB_EAST  DIR_EAST
#define SUB_SOUTH DIR_SOUTH
#define SUB_WEST  DIR_WEST
#define SUB_UP    DIR_UP
#define SUB_DOWN  DIR_DOWN
#define SUB_NE    DIR_NORTHEAST
#define SUB_NW    DIR_NORTHWEST
#define SUB_SE    DIR_SOUTHEAST
#define SUB_SW    DIR_SOUTHWEST


/*
 * Connected state for a channel.
 */
#define CON_PLAYING			 0
#define CON_GET_NAME			 1
#define CON_GET_OLD_PASSWORD		 2
#define CON_CONFIRM_NEW_NAME		 3
#define CON_GET_NEW_PASSWORD		 4
#define CON_CONFIRM_NEW_PASSWORD	 5
#define CON_GET_NEW_SEX			 6
#define CON_GET_NEW_CLASS		 7
#define CON_GET_NEW_VT102                8
#define CON_GET_NEW_ANSI                 9 
#define CON_READ_MOTD			 10
#define CON_NOT_PLAYING			 11
#define CON_COPYOVER_RECOVER            12
#define CON_EDITING 			13
/*
 * Autosave flags
 */
#define SV_UNUSED1                BV00 /* Save on death */
#define SV_UNUSED2                   BV01 /* Save when kill made */
#define SV_PASSCHG                BV02 /* Save on password change */
#define SV_DROP                   BV03 /* Save on drop */
#define SV_PUT                    BV04 /* Save on put */
#define SV_GIVE                   BV05 /* Save on give */
#define SV_AUTO                   BV06 /* Auto save every x minutes (define in cset) */
#define SV_ZAPDROP                BV07 /* Save when eq zaps */
#define SV_UNUSED3                BV08 /* Not used */
#define SV_GET                    BV09 /* Save on get */
#define SV_RECEIVE                BV10 /* Save when receiving */
#define SV_IDLE                   BV11 /* Save when char goes idle */
#define SV_BACKUP                 BV12 /* Make backup of pfile on save */
#define SV_QUITBACKUP             BV13 /* Backup on quit only --Blod */

typedef enum
{
PF_SEX,
PF_LEVEL,
PF_STATUS,
PF_PLAYED,
PF_PKILL,
PF_PDEATH,
PF_AWINS,
PF_ALOSSES,
PF_MKILL,
PF_MDEATH,
PF_GENERATION,
PF_CLASS,
PF_PRACTICE,
PF_LSTATUS,
PF_BOUNTY,
PF_RANDMIGHT
} finger_nums;

typedef enum
{
PF_NAME,
PF_TITLE,
PF_LASTHOST,
PF_LASTTIME,
PF_CONCEP,
PF_CREATETIME,
PF_MARRIAGE
} finger_buf;

/* Copyover.c */
void flog             (char * fmt, ...);
void copyover_recover (void);
typedef struct copyover         COPYOVER;
//Timer defines
struct copyover
{
        sh_int          time;
        char *          name;
};

extern  COPYOVER                    copyover_info;
void    copyover_update             ( void );


/*
 * Character substates
 */
typedef enum
{
  SUB_NONE, SUB_PAUSE, SUB_PERSONAL_DESC, SUB_OBJ_SHORT, SUB_OBJ_LONG,
  SUB_OBJ_EXTRA, SUB_MOB_LONG, SUB_MOB_DESC, SUB_ROOM_DESC, SUB_ROOM_EXTRA,
  SUB_ROOM_EXIT_DESC, SUB_WRITING_NOTE, SUB_MPROG_EDIT, SUB_HELP_EDIT,
  SUB_WRITING_MAP, SUB_PERSONAL_BIO, SUB_REPEATCMD, SUB_RESTRICTED,
  SUB_DEITYDESC,
  /* timer types ONLY below this point */
  SUB_TIMER_DO_ABORT = 128, SUB_TIMER_CANT_ABORT
} char_substates;

/*
 * Timer macros.
 */

#define TIMER(ch, tmr)		((ch)->tick_timer[(tmr)])
#define SET_TIMER(ch, tmr, tm)	((ch)->tick_timer[(tmr)] = (tm))
#define ADD_TIMER(ch, tmr, tm)  ((ch)->tick_timer[(tmr)] += (tm))
#define SUB_TIMER(ch, tmr, tm)  ((ch)->tick_timer[(tmr)] -= (tm))
#define TIME_UP(ch, tmr)	((ch)->tick_timer[(tmr)] == 0 ? TRUE : FALSE)

#define RTIMER(room, rtmr)	 ((room)->tick_timer[(rtmr)])
#define SET_RTIMER(room, rtmr, rtm) ((room)->tick_timer[(rtmr)] = (rtm))
#define ADD_RTIMER(room, rtmr, rtm)  ((room)->tick_timer[(rtmr)] += (rtm))
#define SUB_RTIMER(room, rtmr, rtm)  ((room)->tick_timer[(rtmr)] -= (rtm))
#define RTIME_UP(room, rtmr)	 ((room)->tick_timer[(rtmr)] == 0 ? TRUE : FALSE)

#define TIMER_LAYONHANDS	0
#define TIMER_WRENCH		1
#define TIMER_WRENCHED 		2
#define TIMER_VAMPCALL 		3
#define TIMER_UNCONCIOUS 	4
#define TIMER_VAMP_INHABIT 	5
#define TIMER_DAEMONIC_TRANSFORM 6
#define TIMER_MESMERISE		7
#define TIMER_MESMERISED	8
#define TIMER_FORAGE		9
#define TIMER_NO_ARBOREA	11
#define TIMER_TREE_WALK		12
#define TIMER_CAN_PRAC		13
#define TIMER_CAN_EAT_ARMS	14
#define TIMER_THIRD_ARM_GROWING	15
#define TIMER_FOURTH_ARM_GROWING 16
#define TIMER_THIRD_ARM_GOING	17
#define TIMER_FOURTH_ARM_GOING  18
#define TIMER_SCALPED		19
#define TIMER_CAN_CALL_ROCKS    20
#define TIMER_CANT_BE_TURNED    21
#define TIMER_CANT_TURN    	22
#define TIMER_FIGHT_LAG    	23
#define TIMER_CAN_CHANGE_HAWK   24
#define TIMER_CAN_CREATE_SHARD  25
#define TIMER_CAN_GUST	        26
#define TIMER_CAN_ENTER_STASIS  27
#define TIMER_MAKE_SNOWMAN      28
#define TIMER_ENTOMB            29
#define TIMER_CAN_BREATHE_FROST 30
#define TIMER_HELLFIRE_SUMMON   31
#define TIMER_ON_SPEED	   	32
#define TIMER_ON_LSD		33
#define TIMER_ON_HASH		34
#define TIMER_CAN_CALL_WAR_HORSE 35
#define TIMER_WAR_HORSE_GO      36
#define TIMER_CAN_SPIT_VENOM    37
#define TIMER_CAN_GAIN_VOODOO   38
#define TIMER_CAN_FEATHER       39
#define TIMER_CAN_SHRIEK        40
#define TIMER_CAN_POLYMORPH     41
#define TIMER_DRAGON_GROW       42
#define TIMER_VAMPIRE_GROW      43
#define TIMER_SKILL_LEV1        44
#define TIMER_SKILL_LEV2        45
#define TIMER_CANT_SWARM        46
#define TIMER_CANT_BORROWLIFE   47
#define TIMER_TREE		48
#define TIMER_NEXUS_STUNNED	49
#define TIMER_GOLEM		50
#define TIMER_TAINT		51
#define TIMER_NEWBIE_IMM	52
#define TIMER_CAN_DO_NEXUS	53
#define TIMER_CAN_USE_HEALER	54
#define TIMER_DISCORD		55
#define TIMER_SPHINX_ROAR	56
#define TIMER_INFERNO		57
#define TIMER_CHAOSPORT		58
#define TIMER_CANMAJESTY	59
#define TIMER_MAJESTY		60
#define TIMER_DSLEEP		61

#define MAX_TIMER		62


#define RTIMER_STINKING_CLOUD	0
#define RTIMER_LIFE_VORTEX	1
#define RTIMER_DEATH_VORTEX	2
#define RTIMER_GLYPH_PROTECTION	3
#define RTIMER_HIDE_ROOM	4
#define RTIMER_SWARM_BEES	5
#define RTIMER_SWARM_RATS	6
#define RTIMER_SWARM_BATS	7
#define RTIMER_GHOST_LIGHT	8
#define RTIMER_NEXUS_FLAME	9
#define RTIMER_NEXUS_WATER	10
#define RTIMER_NEXUS_AIR	11
#define RTIMER_NEXUS_EARTH	12
#define RTIMER_NEXUS_ENTROPY	13
#define RTIMER_WALL_NORTH	14
#define RTIMER_WALL_EAST	15
#define RTIMER_WALL_SOUTH	16
#define RTIMER_WALL_WEST	17
#define RTIMER_WALL_UP		18
#define RTIMER_WALL_DOWN	19
#define RTIMER_DISCORD		20
#define RTIMER_DARK_ROOM	21
#define RTIMER_SILENCE		22
#define MAX_RTIMER		30



/*
 * Descriptor (channel) structure.
 */
struct	descriptor_data
{
    DESCRIPTOR_DATA *	next;
    DESCRIPTOR_DATA *	prev;
    DESCRIPTOR_DATA *	snoop_by;
    CHAR_DATA *		character;
    CHAR_DATA *		original;
    char *		host;
    sh_int		descriptor;
    sh_int		connected;
    bool		fcommand;
    bool                vt102;
    char		inbuf		[4 * MAX_INPUT_LENGTH];
    char		incomm		[MAX_INPUT_LENGTH];
    char		inlast		[MAX_INPUT_LENGTH];
    int			repeat;
    bool                multi_comm;
    char *              showstr_head;  /* From ENVY code to compile */
    char *              showstr_point; /* From ENVY code to compile */
    char *		outbuf;
    int			outsize;
    int			outtop; 
    void *              pEdit;          /* OLC */
    char **             pString;        /* OLC */
    int                 editor;         /* OLC */	
};



/*
 * Attribute bonus structures.
 */
struct	str_app_type
{
    sh_int	tohit;
    sh_int	todam;
    sh_int	carry;
    sh_int	wield;
};

struct	int_app_type
{
    sh_int	learn;
};

struct	wis_app_type
{
    sh_int	practice;
};

struct	dex_app_type
{
    sh_int	defensive;
};

struct	con_app_type
{
    sh_int	hitp;
    sh_int	shock;
};



/*
 * TO types for act.
 */
#define TO_ROOM		    0
#define TO_NOTVICT	    1
#define TO_VICT		    2
#define TO_CHAR		    3
#define TO_ALL		    4

/*
 * Help table types.
 */
struct	help_data
{
    HELP_DATA *	next;
    HELP_DATA * prev;
    AREA_DATA * area;
    sh_int	level;
    char *	keyword;
    char *	text;
};



/*
 * Shop types.
 */
#define MAX_TRADE	 5

struct	shop_data
{
    SHOP_DATA *	next;			/* Next shop in list		*/
    int		keeper;			/* Vnum of shop keeper mob	*/
    sh_int	buy_type [MAX_TRADE];	/* Item types shop will buy	*/
    sh_int	profit_buy;		/* Cost multiplier for buying	*/
    sh_int	profit_sell;		/* Cost multiplier for selling	*/
    sh_int	open_hour;		/* First opening hour		*/
    sh_int	close_hour;		/* First closing hour		*/
};



/*
 * Data structure for notes.
 */
struct	note_data
{
    NOTE_DATA *	next;
    NOTE_DATA * prev;
    char *	sender;
    char *	date;
    char *	to_list;
    char *	subject;
    char *	text;
    time_t	date_stamp;
    time_t	expire;
};

/*
struct  random_data
{
        int     vnum;
        int     rarity;
        int     lvnum;
        int     uvnum;

        RANDOM_DATA *next;
        RANDOM_DATA *prev;
};
*/
/*
 * An affect.
 */
struct	affect_data
{
    AFFECT_DATA *	next;
    AFFECT_DATA *	prev;
    sh_int		type;
    sh_int		duration;
    sh_int		location;
    sh_int		modifier;
    int			bitvector;
};



/*
 * A kill structure (indexed by level).
 */
struct	kill_data
{
    sh_int		number;
    sh_int		killed;
};



/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/


/* ASCII conversions -- used so we can have letters in this file */

#define A                       1
#define B                       2
#define C                       4
#define D                       8
#define E                       16
#define F                       32
#define G                       64
#define H                       128
#define I                       256
#define J                       512
#define K                       1024
#define L                       2048
#define M                       4096
#define N                       8192
#define O                       16384
#define P                       32768
#define Q                       65536
#define R                       131072
#define S                       262144
#define T                       524288
#define U                       1048576
#define V                       2097152
#define W                       4194304
#define X                       8388608
#define Y                       16777216
#define Z                       33554432
#define aa                      67108864        /* doubled due to conflicts */
#define bb                      134217728
#define cc                      268435456    
#define ddd                     536870912
#define ee                      1073741824
#define ff                      2147483648
#define gg                      4294967256


/* These are the good old dragon/werewolf types - same variable */
#define WERE_WOLF		          1
#define WERE_RAT		          2
#define WERE_TIGER		    3
#define WERE_BEAR		          4
#define WERE_SHARK		    5

#define DRAGON_RED		    1
#define DRAGON_WHITE		    2
#define DRAGON_BLACK		    3
#define DRAGON_GREEN		    4
#define DRAGON_BLUE		    5
#define DRAGON_SILVER		    6
#define DRAGON_GOLD		    7
#define DRAGON_SHADOW		    8

/* size */
#define SIZE_TINY                       0
#define SIZE_SMALL                      1
#define SIZE_MEDIUM                     2
#define SIZE_LARGE                      3
#define SIZE_HUGE                       4
#define SIZE_GIANT                      5

/*
#define FRM_WOLF               1
#define FRM_BAT                2
#define FRM_MIST               3
#define FRM_DOLPHIN            4
#define FRM_FLAME              5
#define FRM_SPIDER             6
#define FRM_LIGHT              7
#define FRM_COCKROACH          8
#define FRM_RABBIT             9
#define FRM_BLOB               10
#define FRM_FROG               11
#define FRM_IDIOT              12
#define FRM_WHIRLWIND          13
#define FRM_CRIMSON            14
#define FRM_HAWK               15
#define FRM_WEREWOLF           16
#define FRM_FOX	               17
#define FRM_WYVERN             18
#define FRM_GARGANTUA          19
#define FRM_PEASANT            20
#define FRM_WARRIOR            21
#define FRM_BARBARIAN          22
#define FRM_GIRL               23
*/

/* The demonic forms */
#define FRM_LEMURE             1
#define FRM_NUPPERIBO          2
#define FRM_SPINAGON           3
#define FRM_BARBAZU            4
#define FRM_ABISHAI            5
#define FRM_OSYLUTH            6
#define FRM_HAMATULA           7
#define FRM_ERINYES            8
#define FRM_AMNIZU             9
#define FRM_CORNUGON           10
#define FRM_GELUGON            11
#define FRM_PIT_FIEND          12
#define FRM_PIT_LORD           13
#define FRM_DROWSPIDER	       14

/*
#define FRM_TREE               37

#define FRM_DRAGON_1           38
#define FRM_DRAGON_13          50
*/

#define FRM_WERE_RAT	       1
#define FRM_WERE_BEAR	       1
#define FRM_WERE_TIGER	       1
#define FRM_WERE_SHARK	       1
#define FRM_GOLEM	       1
#define FRM_PIXIE	       1

#define MAX_FORMS	       15

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES. flags2
 */

#define VAMP_ASHES		(A)
#define VAMP_CLONE		(B)
#define VAMP_OBJMASK		(C)
#define AFF_TOTALBLIND		(D)
#define AFF_SPIRITGUARD		(E)
#define AFF_CDONE		(F)

#define AFF_ALLOW_VAMP          (P)
#define AFF_ALLOW_WERE          (Q)
#define AFF_ALLOW_ELAD          (R)
#define AFF_ALLOW_DEMON         (S)
#define AFF_ALLOW_WARLOCK       (T)
#define AFF_CLAW                (L)
#define AFF_BITE                (M)
#define AFF_TAIL                (N)
#define AFF_WING                (O)

// flag3
#define AFF3_BLINK_1ST_RD	(A)
#define AFF3_BLINK_2ND_RD	(B)


/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_CITYGUARD	   3060
#define MOB_VNUM_VAMPIRE	   3404


/*
 * Immunities, for players.  KaVir.
 */
#define IMM_SLASH	      1	/* Resistance to slash, slice. 		*/
#define IMM_STAB	      2	/* Resistance to stab, pierce. 		*/
#define IMM_SMASH	      4	/* Resistance to blast, pound, crush. 	*/
#define IMM_ANIMAL	      8	/* Resistance to bite, claw. 		*/
#define IMM_MISC	     16	/* Resistance to grep, suck, whip. 	*/
#define IMM_CHARM	     32	/* Immune to charm spell. 		*/
#define IMM_HEAT	     64	/* Immune to fire/heat spells. 		*/
#define IMM_COLD	    128	/* Immune to frost/cold spells.		*/
#define IMM_LIGHTNING	    256	/* Immune to lightning spells.		*/
#define IMM_ACID	    512	/* Immune to acid spells.		*/
#define IMM_SUMMON	   1024	/* Immune to being summoned.		*/
#define IMM_VOODOO	   2048	/* Immune to voodoo magic.		*/
#define IMM_VAMPIRE	   4096	/* Allow yourself to become a vampire.	*/
#define IMM_STAKE	   8192	/* Immune to being staked (vamps only). */
#define IMM_SUNLIGHT	  16384	/* Immune to sunlight (vamps only).	*/
#define IMM_SHIELDED	  32768 /* For Obfuscate. Block scry, etc.	*/
#define IMM_HURL	  65536 /* Cannot be hurled.			*/
#define IMM_BACKSTAB	 131072 /* Cannot be backstabbed.		*/
#define IMM_KICK	 262144 /* Cannot be kicked.			*/
#define IMM_DISARM	 524288 /* Cannot be disarmed.			*/
#define IMM_STEAL	1048576 /* Cannot have stuff stolen.		*/
#define IMM_SLEEP	2097152 /* Immune to sleep spell.		*/
#define IMM_DRAIN       4194304 /* Immune to energy drain.		*/
#define IMM_SHIELD2	8388608 /* Chaotic shield			*/
#define IMM_TRANSPORT  16777216 /* Objects can't be transported to you.	*/
#define IMM_TRAVEL     33554432
#define IMM_SCRY       67108864 /* Hightened Senses			*/
/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
typedef enum {
ACT_IS_NPC,		      	/* Auto set for mobs	*/
ACT_SENTINEL,		    	/* Stays in one room	*/
ACT_SCAVENGER,		    	/* Picks up objects	*/
ACT_AGGRESSIVE,		    	/* Attacks PC's		*/
ACT_STAY_AREA,		    	/* Won't leave area	*/
ACT_WIMPY,		    	/* Flees when hurt	*/
ACT_PET,		 	/* Auto set for pets	*/
ACT_TRAIN,		    	/* Can train PC's	*/
ACT_PRACTICE,		    	/* Can practice PC's	*/
ACT_MOUNT,		    	/* Can be mounted	*/
ACT_NOPARTS,		    	/* Dead = no body parts	*/
ACT_NOEXP,		    	/* No exp for killing   */
ACT_PROTOTYPE,		    
ACT_NOAUTOKILL,			
ACT_NOEXP2,			
ACT_AGG_GOOD,		    
ACT_AGG_EVIL,			
ACT_AGG_NEUTRAL			
} mob_act_flag_types;
/*
 * Thingers for Demon Warps
 */

#define WARP_CBODY				1
#define WARP_SBODY				2
#define WARP_STRONGARMS				4
#define WARP_STRONGLEGS				8
#define WARP_VENOMTONG				16
#define WARP_SPIKETAIL				32
#define WARP_BADBREATH				64
#define WARP_QUICKNESS				128
#define WARP_STAMINA				256
#define WARP_HUNT				512
#define WARP_DEVOUR				1024
#define WARP_TERROR				2048
#define WARP_REGENERATE				4096
#define WARP_STEED				8192
#define WARP_WEAPON				16384
#define WARP_INFIRMITY				32768
#define WARP_GBODY				65536
#define WARP_SCARED				131072
#define WARP_MAGMA				262144
#define WARP_WEAK				524288
#define WARP_SLOW				1048576
#define WARP_VULNER				2097152
#define WARP_SHARDS				4194304
#define WARP_WINGS				8388608
#define WARP_CLUMSY				16777216
#define WARP_STUPID				33554432
#define WARP_SPOON				67108864
#define WARP_FORK				134217728
#define WARP_KNIFE				268435456
#define WARP_SALADBOWL				536870912



/* Bits for the Discie thing Numbers.. really.. not bits */

#define DISC_VAMP_CELE			2
#define DISC_VAMP_FORT			3
#define DISC_VAMP_OBTE			4
#define DISC_VAMP_PRES			5
#define DISC_VAMP_QUIE			6
#define DISC_VAMP_THAU			7
#define DISC_VAMP_AUSP			8
#define DISC_VAMP_DOMI			9
#define DISC_VAMP_OBFU			10
#define DISC_VAMP_POTE			11
#define DISC_VAMP_PROT			12
#define DISC_VAMP_SERP			13
#define DISC_VAMP_VICI			14
#define DISC_VAMP_DAIM			15
#define DISC_VAMP_ANIM			16

#define DISC_WERE_BEAR			18
#define DISC_WERE_LYNX			19
#define DISC_WERE_BOAR			20
#define DISC_WERE_OWL			21
#define DISC_WERE_SPID			22
#define DISC_WERE_WOLF			23
#define DISC_WERE_HAWK			24
#define DISC_WERE_MANT			25
#define DISC_WERE_RAPT			26
#define DISC_WERE_LUNA			27
#define DISC_WERE_PAIN			28
#define DISC_WERE_CONG			29

#define DISC_DAEM_HELL			30
#define DISC_DAEM_ATTA			31
#define DISC_DAEM_TEMP			32
#define DISC_DAEM_MORP			33
#define DISC_DAEM_CORR			34
#define DISC_DAEM_GELU			35
#define DISC_DAEM_DISC			36
#define DISC_DAEM_NETH			37
#define DISC_DAEM_IMMU			38
#define DISC_VAMP_CHIM			39
#define DISC_VAMP_THAN			40
#define DISC_VAMP_OBEA			41
#define DISC_VAMP_NECR			42
#define DISC_VAMP_MELP			43

#define DISC_SHIFTINANIMENT     44
#define DISC_SHIFTANIMENT       45
#define DISC_ARMWEAPONS         46
#define DISC_POOLFORMS          47

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND		      1
#define AFF_INVISIBLE		      2
#define AFF_DETECT_EVIL		      4
#define AFF_DETECT_INVIS	      8
#define AFF_DETECT_MAGIC	     16
#define AFF_DETECT_HIDDEN	     32
#define AFF_SHADOWPLANE		     64 /* Creatures in shadow plane - KaVir */
#define AFF_SANCTUARY		    128
#define AFF_FAERIE_FIRE		    256
#define AFF_INFRARED		    512
#define AFF_CURSE		   1024
#define AFF_FLAMING		   2048 /* For burning creatures - KaVir */
#define AFF_POISON		   4096
#define AFF_PROTECT		   8192
#define AFF_ETHEREAL		  16384 /* For ethereal creatures - KaVir */
#define AFF_SNEAK		  32768
#define AFF_HIDE		  65536
#define AFF_SLEEP		 131072
#define AFF_CHARM		 262144
#define AFF_FLYING		 524288
#define AFF_PASS_DOOR		1048576
#define AFF_POLYMORPH		2097152 /* For polymorphed creatures - KaVir */
#define AFF_SHADOWSIGHT		4194304 /* Can see between planes - KaVir */
#define AFF_WEBBED		8388608 /* Cannot move - KaVir */
#define AFF_PROTECT_GOOD       16777216
#define AFF_DROWFIRE           33554432 /* Drow Darkness - Rotain */   
#define AFF_ZULOFORM	       67108864
#define AFF_SHIFT             134217728
#define AFF_PEACE             268435456
#define AFF_INFIRMITY         536870912
#define AFF_CRITICAL		AFF_INFIRMITY * 2

/* Affected_by 2 */
#define AFF_CONTRACEPTION   1
#define AFF_BEASTIE		      2
#define AFF_SEVERED		      4
#define PLR_IMPLAG		      8
#define EXTRA_BLINKY       16
//#define PLR_BRIEF3	    32
#define OBJ_VNUM_LEGS		     46
DECLARE_DO_FUN( do_resetpassword);
DECLARE_DO_FUN( do_pstat        );
DECLARE_DO_FUN( do_vshow );

/*
 * Bits for 'itemaffect'.
 * Used in #MOBILES.
 */
#define ITEMA_SHOCKSHIELD	      1
#define ITEMA_FIRESHIELD	      2
#define ITEMA_ICESHIELD		      4
#define ITEMA_ACIDSHIELD	      8
#define ITEMA_MONKCHI                 16
#define ITEMA_CHAOSSHIELD            32
#define ITEMA_ARTIFACT               64
#define ITEMA_REGENERATE            128
#define ITEMA_SPEED                 256
#define ITEMA_VORPAL                512
#define ITEMA_CRITICAL                1024
#define ITEMA_RIGHT_SILVER         2048
#define ITEMA_LEFT_SILVER          4096
#define ITEMA_REFLECT              8192
#define ITEMA_RESISTANCE          16384
#define ITEMA_VISION              32768
#define ITEMA_STALKER             65536
#define ITEMA_VANISH             131072
#define ITEMA_RAGER              262144
#define ITEMA_BIG         524288
#define ITEMA_CHAOSHANDS	1048576
#define ITEMA_AFFMANTIS		2097152
#define ITEMA_AFFENTROPY	4194304 
#define ITEMA_AFFEYE		8388608
#define ITEMA_MAGESHIELD		16777216
#define ITEMA_STEELSHIELD   33554432        /* Mage tougness */
#define ITEMA_DEFLECTOR     67108864     /* Mage dodge/parry */
#define ITEMA_ILLUSIONS    134217728    /* Mage dodge/parry */
#define ITEMA_BEAST        268435456    /* Mage extra attacks */
#define ITEMA_MANASHIELD   536870912    /* Mage mana shield */
#define ITEMA_DOUBLEEXP   ITEMA_MANASHIELD * 2
/*
 * Newbie Pack Crap
 */
#define ITEM_NP_BREASTPLATE	30333
#define ITEM_NP_HELMET		30334
#define ITEM_NP_SLEEVES		30335
#define ITEM_NP_LEGGINGS	30336
#define ITEM_NP_GAUNTLETS	30337
#define ITEM_NP_BOOTS		30338
#define ITEM_NP_LONGSWORD	30340
#define ITEM_NP_BUCKLER		30341
#define ITEM_NP_RING		30342
#define ITEM_NP_COLLAR		30343
#define ITEM_NP_LANTERN		30344		
 





/*
 * Rune, Glyph and Sigil bits.
 */
#define RUNE_FIRE	      1
#define RUNE_AIR	      2
#define RUNE_EARTH	      4
#define RUNE_WATER	      8
#define RUNE_DARK	      16
#define RUNE_LIGHT	      32
#define RUNE_LIFE	      64
#define RUNE_DEATH	      128
#define RUNE_MIND	      256
#define RUNE_SPIRIT	      512
#define RUNE_MASTER	      1024
#define GLYPH_CREATION	      1
#define GLYPH_DESTRUCTION     2
#define GLYPH_SUMMONING	      4
#define GLYPH_TRANSFORMATION  8
#define GLYPH_TRANSPORTATION  16
#define GLYPH_ENHANCEMENT     32
#define GLYPH_REDUCTION	      64
#define GLYPH_CONTROL	      128
#define GLYPH_PROTECTION      256
#define GLYPH_INFORMATION     512
#define SIGIL_SELF	      1
#define SIGIL_TARGETING       2
#define SIGIL_AREA	      4
#define SIGIL_OBJECT	      8


/*
 * Advanced spells.
 */
#define ADV_DAMAGE	      1
#define ADV_AFFECT	      2
#define ADV_ACTION	      4
#define ADV_AREA_AFFECT       8
#define ADV_VICTIM_TARGET    16
#define ADV_OBJECT_TARGET    32
#define ADV_GLOBAL_TARGET    64
#define ADV_NEXT_PAGE       128
#define ADV_PARAMETER       256
#define ADV_SPELL_FIRST     512
#define ADV_NOT_CASTER     1024
#define ADV_NO_PLAYERS     2048
#define ADV_SECOND_VICTIM  4096
#define ADV_SECOND_OBJECT  8192
#define ADV_REVERSED      16384
#define ADV_STARTED	  32768
#define ADV_FINISHED	  65536
#define ADV_FAILED	 131072
#define ADV_MESSAGE_1	 262144
#define ADV_MESSAGE_2	 524288
#define ADV_MESSAGE_3   1048576	

/*
 * Advanced spell actions.
 */
#define ACTION_NONE	      0
#define ACTION_MOVE	      1
#define ACTION_MOB	      2
#define ACTION_OBJECT	      3

/*
 * Advanced spell affects.
 */
#define ADV_STR			      1
#define ADV_DEX			      2
#define ADV_INT			      4
#define ADV_WIS			      8
#define ADV_CON			     16
#define ADV_SEX			     32
#define ADV_MANA		     64
#define ADV_HIT			    128
#define ADV_MOVE		    256
#define ADV_AC			    512
#define ADV_HITROLL		   1024
#define ADV_DAMROLL		   2048
#define ADV_SAVING_SPELL	   4096



/* Colour scale macros - added 12th Aug 1995 by Calamar */

#define NO_COLOUR       "\e[0m"
#define GREY            "\e[1;30m"
#define D_WHITE         "\e[0;37m"
#define WHITE           "\e[1;37m"
#define D_RED           "\e[0;31m"
#define L_RED           "\e[1;31m"
#define D_GREEN         "\e[0;32m"
#define L_GREEN         "\e[1;32m"
#define D_BLUE          "\e[0;34m"
#define L_BLUE          "\e[1;34m"
#define BROWN           "\e[0;33m"
#define YELLOW          "\e[1;33m"
#define MAGENTA         "\e[0;35m"
#define PINK            "\e[1;35m"
#define D_CYAN          "\e[0;36m"
#define L_CYAN          "\e[1;36m"
#define NORMAL          "\e[0;37m"
#define UNDERLINE       "\e[4m"
#define INVERTED        "\e[7m"
#define FLASHING        "\e[5m"
// BG Colors
#define BG_RED          "\e[1;41m"
#define BG_GREEN        "\e[1;42m"
#define BG_BROWN        "\e[1;43m"
#define BG_BLUE         "\e[1;44m"
#define BG_PURPLE       "\e[1;45m"
#define BG_CYAN         "\e[1;46m"
#define BG_WHITE        "\e[1;47m"
#define BG_CLEAR        "\e[1;47m\e[0m"

#define ADD_COLOUR(_player,_str,_col) {char swh_temp[255]; \
if (!IS_NPC(_player) && xIS_SET(_player->act, PLR_ANSI)) {swh_temp[0] = '\0'; \
strcpy(swh_temp, _col); strcat(swh_temp, _str); strcat(swh_temp, NORMAL); \
strcpy(_str, swh_temp);}} 

#define SCALE_COLS 4

#define COL_SCALE(_swh_str,_swh_ch,_swh_curr,_swh_max) \
	ADD_COLOUR(_swh_ch, _swh_str, \
	(_swh_curr < 1) ? L_RED : \
	(_swh_curr < _swh_max) ? \
	scale[(SCALE_COLS * _swh_curr) / ((_swh_max > 0) \
	? _swh_max : 1)] : L_CYAN)
/* Ensure coord is on the map */
#define BOUNDARY(x, y) (((x) < 0) || ((y) < 0) || ((x) > MAPX) || ((y) > MAPY))

extern char *scale[SCALE_COLS];
char *ColorStrip(char *string);

/*
 * Bits for 'vampire'.
 * Used for player vampires.
 */
#define VAM_FANGS		      1
#define VAM_CLAWS		      2
#define VAM_NIGHTSIGHT		      4
#define VAM_FLYING		      8 /* For flying creatures */
#define VAM_SONIC		     16 /* For creatures with full detect */
#define VAM_CHANGED		     32 /* Changed using a vampire power */

#define VAM_PROTEAN		     64 /* Claws, nightsight, and change */
#define VAM_CELERITY		    128 /* 66%/33% chance 1/2 extra attacks */
#define VAM_FORTITUDE		    256 /* 5 hp less per hit taken */
#define VAM_POTENCE		    512 /* Deal out 1.5 times normal damage */
#define VAM_OBFUSCATE		   1024 /* Disguise and invis */
#define VAM_AUSPEX		   2048 /* Truesight, etc */
#define VAM_OBTENEBRATION	   4096 /* Shadowplane/sight and shadowbody */
#define VAM_SERPENTIS		   8192 /* Eyes/serpent, heart/darkness, etc */

#define VAM_DISGUISED		  16384 /* For the Obfuscate disguise ability */
#define VAM_MORTAL		  32768 /* For Obfuscate mortal ability. */

#define VAM_DOMINATE		  65536 /* Evileye, command */

#define VAM_EVILEYE		 131072 /* Evileye, command */

#define VAM_PRESENCE		 262144 /* Presence discipline */
#define VAM_VICISSITUDE          524288 /* Vicissitude discipline */
#define VAM_THAU                1048576 /* Thaumaturgy discipline */
#define VAM_ANIMAL              2097152 /* Animalism discipline */
#define VAM_SHIFTED             4194304 /* Non-poly shift */
#define VAM_QUIETUS             8388608 /* Quietus discipline */
#define VAM_HEAD	       16777216
#define VAM_TAIL	       33554432
#define VAM_EXOSKELETON        67108864
#define VAM_HORNS	      134217728
#define VAM_WINGS	      268435456


/*
 * Bits for 'polymorph'.
 * Used for players.
 */
#define POLY_BAT		      1
#define POLY_WOLF		      2
#define POLY_MIST		      4
#define POLY_SERPENT		      8
#define POLY_RAVEN		     16
#define POLY_FISH		     32
#define POLY_FROG		     64
#define POLY_ZULOFORM               128
#define POLY_SHIFT                  256
#define POLY_SPIDER		    512
#define POLY_DRAGON		   1024
/*
 * Languages.
 */
#define LANG_COMMON		      0
#define DIA_OLDE		      1
#define DIA_BAD			      2
#define LANG_DARK		      4



/*
 * Score.
 */
#define SCORE_TOTAL_XP		      0
#define SCORE_HIGH_XP		      1
#define SCORE_TOTAL_LEVEL	      2
#define SCORE_HIGH_LEVEL	      3
#define SCORE_QUEST		      4
#define SCORE_NUM_QUEST		      5



/*
 * Zombie Lord.
 */
#define ZOMBIE_NOTHING		      0
#define ZOMBIE_TRACKING		      1
#define ZOMBIE_ANIMATE		      2
#define ZOMBIE_CAST		      3
#define ZOMBIE_REST		      4



/*
 * Damcap values.
 */
#define DAM_CAP		      0
#define DAM_CHANGE	      1


/* return values for check_imm */
#define IS_NORMAL               0
#define IS_DIMMUNE              1
#define IS_RESISTANT            2
#define IS_VULNERABLE           3

/* damage classes */
#define DAM_NONE                0
#define DAM_BASH                1
#define DAM_PIERCE              2
#define DAM_SLASH               3
#define DAM_FIRE                4
#define DAM_COLD                5
#define DAM_LIGHTNING           6
#define DAM_ACID                7
#define DAM_POISON              8
#define DAM_NEGATIVE            9
#define DAM_HOLY                10
#define DAM_ENERGY              11
#define DAM_MENTAL              12
#define DAM_DISEASE             13
#define DAM_DROWNING            14
#define DAM_LIGHT               15
#define DAM_OTHER               16
#define DAM_HARM                17
#define DAM_CHARM               18
#define DAM_SOUND               19

/* IMM bits for mobs */
#define DIMM_SUMMON              (A)
#define DIMM_CHARM               (B)
#define DIMM_MAGIC               (C)
#define DIMM_WEAPON              (D)
#define DIMM_BASH                (E)
#define DIMM_PIERCE              (F)
#define DIMM_SLASH               (G)
#define DIMM_FIRE                (H)
#define DIMM_COLD                (I)
#define DIMM_LIGHTNING           (J)
#define DIMM_ACID                (K)
#define DIMM_POISON              (L)
#define DIMM_NEGATIVE            (M)
#define DIMM_HOLY                (N)
#define DIMM_ENERGY              (O)
#define DIMM_MENTAL              (P)
#define DIMM_DISEASE             (Q)
#define DIMM_DROWNING            (R)
#define DIMM_LIGHT               (S)
#define DIMM_SOUND               (T)
#define DIMM_WOOD                (X)
#define DIMM_SILVER              (Y)
#define DIMM_IRON                (Z)
 
/* RES bits for mobs */
#define RES_SUMMON              (A)
#define RES_CHARM               (B)
#define RES_MAGIC               (C)
#define RES_WEAPON              (D)
#define RES_BASH                (E)
#define RES_PIERCE              (F)
#define RES_SLASH               (G)
#define RES_FIRE                (H)
#define RES_COLD                (I)
#define RES_LIGHTNING           (J)
#define RES_ACID                (K)
#define RES_POISON              (L)
#define RES_NEGATIVE            (M)
#define RES_HOLY                (N)
#define RES_ENERGY              (O)
#define RES_MENTAL              (P)
#define RES_DISEASE             (Q)
#define RES_DROWNING            (R)
#define RES_LIGHT               (S)
#define RES_SOUND               (T)
#define RES_WOOD                (X)
#define RES_SILVER              (Y)
#define RES_IRON                (Z)
 
/* VULN bits for mobs */
#define VULN_SUMMON             (A)
#define VULN_CHARM              (B)
#define VULN_MAGIC              (C)
#define VULN_WEAPON             (D)
#define VULN_BASH               (E)
#define VULN_PIERCE             (F)
#define VULN_SLASH              (G)
#define VULN_FIRE               (H)
#define VULN_COLD               (I)
#define VULN_LIGHTNING          (J)
#define VULN_ACID               (K)
#define VULN_POISON             (L)
#define VULN_NEGATIVE           (M)
#define VULN_HOLY               (N)
#define VULN_ENERGY             (O)
#define VULN_MENTAL             (P)
#define VULN_DISEASE            (Q)
#define VULN_DROWNING           (R)
#define VULN_LIGHT              (S)
#define VULN_SOUND              (T)
#define VULN_WOOD               (X)
#define VULN_SILVER             (Y)
#define VULN_IRON               (Z)


/*
 * Bits for Demonic Champions.
 *
 * new demon fields in player.h and old ones in old.h
 */


/*
 * Mounts
 */
#define IS_ON_FOOT		      0
#define IS_MOUNT		      1
#define IS_RIDING		      2
#define IS_CARRIED		      4
#define IS_CARRYING		      8




/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL		      0
#define SEX_MALE		      1
#define SEX_FEMALE		      2



/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_MONEY_ONE	      2
#define OBJ_VNUM_MONEY_SOME	      3
#define OBJ_VNUM_VENDING	     24
/* Demonic Transformation */

#define OBJ_VNUM_DHELM		     27924
#define OBJ_VNUM_DGREAVES            27925
#define OBJ_VNUM_DCLOAK	             27926
#define OBJ_VNUM_DARMOR              27927
#define OBJ_VNUM_DRING               27928
#define OBJ_VNUM_DSWORD              27929
#define OBJ_VNUM_DSHIELD             27930

#define OBJ_VNUM_BROD		     30073
#define OBJ_VNUM_LKLAIVE             33114
#define OBJ_VNUM_STAKE		     30011
#define OBJ_VNUM_MEDAL               29521
#define OBJ_VNUM_KHORNE		     29664
#define OBJ_VNUM_CORPSE_NPC	     10
#define OBJ_VNUM_CORPSE_PC	     11
#define OBJ_VNUM_SEVERED_HEAD	     12
#define OBJ_VNUM_TORN_HEART	     13
#define OBJ_VNUM_SLICED_ARM	     14
#define OBJ_VNUM_SLICED_LEG	     15
#define OBJ_VNUM_FINAL_TURD	     16

#define OBJ_VNUM_MUSHROOM	     20
#define OBJ_VNUM_LIGHT_BALL	     21
#define OBJ_VNUM_SPRING		     22
#define OBJ_VNUM_BLOOD_SPRING	     23

#define OBJ_VNUM_SCHOOL_MACE	   3700
#define OBJ_VNUM_SCHOOL_DAGGER	   3701
#define OBJ_VNUM_SCHOOL_SWORD	   3702
#define OBJ_VNUM_SCHOOL_VEST	   3703
#define OBJ_VNUM_SCHOOL_SHIELD	   3704
#define OBJ_VNUM_SCHOOL_BANNER     3716
#define OBJ_VNUM_TAPPED_SAFE      41048
/* For KaVir's stuff */
#define OBJ_VNUM_SOULBLADE	  30000
#define OBJ_VNUM_GATE             30042
#define OBJ_VNUM_GATE2		  30072
#define OBJ_VNUM_PORTAL		  30001
#define OBJ_VNUM_EGG		  30002
#define OBJ_VNUM_EMPTY_EGG	  30003
#define OBJ_VNUM_SPILLED_ENTRAILS 30004
#define OBJ_VNUM_QUIVERING_BRAIN  30005
#define OBJ_VNUM_SQUIDGY_EYEBALL  30006
#define OBJ_VNUM_SPILT_BLOOD      30007
#define OBJ_VNUM_VOODOO_DOLL      30010
#define OBJ_VNUM_RIPPED_FACE      30012
#define OBJ_VNUM_TORN_WINDPIPE    30013
#define OBJ_VNUM_CRACKED_HEAD     30014
#define OBJ_VNUM_SLICED_EAR	  30025
#define OBJ_VNUM_SLICED_NOSE	  30026
#define OBJ_VNUM_KNOCKED_TOOTH	  30027
#define OBJ_VNUM_TORN_TONGUE	  30028
#define OBJ_VNUM_SEVERED_HAND	  30029
#define OBJ_VNUM_SEVERED_FOOT	  30030
#define OBJ_VNUM_SEVERED_THUMB	  30031
#define OBJ_VNUM_SEVERED_INDEX	  30032
#define OBJ_VNUM_SEVERED_MIDDLE	  30033
#define OBJ_VNUM_SEVERED_RING	  30034
#define OBJ_VNUM_SEVERED_LITTLE	  30035
#define OBJ_VNUM_SEVERED_TOE	  30036
#define OBJ_VNUM_PROTOPLASM	  30037
#define OBJ_VNUM_QUESTCARD	  30039
#define OBJ_VNUM_QUESTMACHINE	  30040
#define OBJ_VNUM_COPPER           30049
#define OBJ_VNUM_IRON             30050
#define OBJ_VNUM_STEEL            30051
#define OBJ_VNUM_ADAMANTITE       30052
#define OBJ_VNUM_ELECTRUM         30074

#define MOB_VNUM_GUARDIAN	  33001
#define MOB_VNUM_SERVANT          33002
#define MOB_VNUM_MOUNT		  6
#define MOB_VNUM_FROG		  7
#define MOB_VNUM_RAVEN		  8
#define MOB_VNUM_CAT		  9
#define MOB_VNUM_DOG		  10
#define MOB_VNUM_SKWAR            30
#define MOB_VNUM_SKMAGE1          31
#define MOB_VNUM_SKMAGE2          32
#define MOB_VNUM_SKMAGE3          33
#define MOB_VNUM_SKMAGE4          34
#define MOB_VNUM_SKMAGE5          35
#define MOB_VNUM_SKMAGE6          36
#define MOB_VNUM_EYE		  12
#define MOB_VNUM_SATAN            30003
#define MOB_VNUM_DEMON		  30005
#define MOB_VNUM_SERPENT	  33003
#define MOB_VNUM_ILLUSION	  33004
#define MOB_VNUM_FIRE             14  
#define MOB_VNUM_STONE            15  
#define MOB_VNUM_IRON             16  
#define MOB_VNUM_CLAY             17

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT		      1
#define ITEM_SCROLL		      2
#define ITEM_WAND		      3
#define ITEM_STAFF		      4
#define ITEM_WEAPON		      5
#define ITEM_TREASURE		      8
#define ITEM_ARMOR		      9
#define ITEM_POTION		     10
#define ITEM_FURNITURE		     12
#define ITEM_TRASH		     13
#define ITEM_CONTAINER		     15
#define ITEM_DRINK_CON		     17
#define ITEM_KEY		     18
#define ITEM_FOOD		     19
#define ITEM_MONEY		     20
#define ITEM_BOAT		     22
#define ITEM_CORPSE_NPC		     23
#define ITEM_CORPSE_PC		     24
#define ITEM_FOUNTAIN		     25
#define ITEM_PILL		     26
#define ITEM_PORTAL		     27
#define ITEM_EGG		     28
#define ITEM_VOODOO		     29
#define ITEM_STAKE		     30
#define ITEM_MISSILE		 31 /* Ammo vnum, cur, max, type */
#define ITEM_AMMO		     32 /* ???, dam min, dam max, type */
#define ITEM_QUEST		     33
#define ITEM_QUESTCARD		 34
#define ITEM_QUESTMACHINE	 35
#define ITEM_SYMBOL		     36
#define ITEM_BOOK		     37
#define ITEM_PAGE		     38
#define ITEM_TOOL		     39
#define ITEM_WALL            40
#define ITEM_COPPER          41
#define ITEM_IRON		     42
#define ITEM_STEEL		     43
#define ITEM_ADAMANTITE		 44
#define ITEM_GEMSTONE        45
#define ITEM_HILT            46
#define ITEM_DTOKEN		     47
#define ITEM_HEAD		     48
#define ITEM_KINGDOM_POWER   	50
#define	ITEM_DRAGONGEM		 51
#define ITEM_WGATE			 52
#define ITEM_INSTRUMENT		53
#define ITEM_ELECTRUM           54
#define ITEM_PIECE              55
#define ITEM_BANK		56
#define ITEM_WELL		57
#define ITEM_STATUE		58
#define ITEM_TELEPORTER		59
#define ITEM_IOU		60
#define ITEM_MACHINE		61
#define ITEM_EXP_TOKEN		62
/*
 * Weapon Stats
 */

#define WEAPON_FLAMING          (A)
#define WEAPON_FROST            (B)
#define WEAPON_VAMPIRIC         (C)
#define WEAPON_SHARP            (D)
#define WEAPON_VORPAL           (E)
#define WEAPON_TWO_HANDS        (F)
#define WEAPON_SHOCKING         (G)
#define WEAPON_POISON           (H)
#define WEAPON_SUNBLADE         (I)
#define WEAPON_DRAGONLANCE      (J)
#define WEAPON_SILVER           (K)
#define WEAPON_RUNE_FORCE_BOLT  (L)
#define WEAPON_RUNE_SMITE_EVIL  (M)
#define WEAPON_RUNE_BLAZE       (N)
#define WEAPON_RUNE_LIGHTNING   (O)
#define WEAPON_RUNE_DANCING     (P)
#define WEAPON_ELE_FLAME        (Q)
#define WEAPON_ELE_WATER        (R)
#define WEAPON_ELE_EARTH        (S)
#define WEAPON_ELE_AIR          (T)


/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		      1
#define ITEM_HUM		      2
#define ITEM_THROWN		      4
#define ITEM_KEEP		      8
#define ITEM_VANISH		     16
#define ITEM_INVIS		     32
#define ITEM_MAGIC		     64
#define ITEM_NODROP		    128
#define ITEM_BLESS		    256
#define ITEM_ANTI_GOOD		    512
#define ITEM_ANTI_EVIL		   1024
#define ITEM_ANTI_NEUTRAL	   2048
#define ITEM_NOREMOVE		   4096
#define ITEM_INVENTORY		   8192
#define ITEM_LOYAL		  16384
#define ITEM_SHADOWPLANE	  32768
#define ITEM_PROTOTYPE		    (L)
#define ITEM_MENCHANT		  65536
#define ITEM_RARE		  ITEM_MENCHANT * 2
#define ITEM_NOSAC		  ITEM_RARE * 2

/* Item extra flags II, the return of the item flags!  */

#define ITEM_ARM		  (A)
#define ITEM_NYSTUL               (B)
#define ITEM_NO_INTERRUPT         (C)
#define ITEM_DAEMONSEED		  (D)
#define ITEM_JUJU_BAG		  (F)
#define ITEM_VOO_HEAD		  (G)
#define ITEM_VOO_DEAD		  (H)
#define ITEM_VOO_BODY		  (I)
#define ITEM_VOO_THREAD		  (J)
#define ITEM_INGRED_GLOIN	  (K)
#define ITEM_INGRED_FROGBREATH    (L)
#define ITEM_INGRED_PAPACOCO	  (M)
#define ITEM_INGRED_MULDALEAF	  (N)
#define ITEM_INGRED_SCULLYWEED    (O)
#define ITEM_INGRED_WORMWART	  (P)
#define ITEM_INGRED_TILLIFREEN    (Q)
#define ITEM_INGRED_BAJUJU        (R)
#define ITEM_ATTACK_GOOD          (S)
#define ITEM_ITEMHIDE	          (T)
#define ITEM_ICE                  (U)

/* artifact and relic flags */

#define ITEM_TELEPORTS		   (W)  /* teleports upon owner death */
#define ITEM_DESTROYED		   (X)  /* destroyed upon owner death */
#define ITEM_UNIQUE		   (Y)  /* only 1 in game ever */
#define ITEM_DESIRED		   (Z)  /* you can't let go of it ever */
#define ITEM_INDESTRUCTABLE	   (aa) /* guess */
#define ITEM_TELEPORT_PROTECTION   (bb) /* teleports when attempts to destroy it */
#define ITEM_KNOW_OWNER 	   (cc) /* owner displayed on artifact command */

#define ITEM_FLYING	           (dd)
#define ITEM_FORM_MELDED	   (ee)



/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		      1
#define ITEM_WEAR_FINGER	      2
#define ITEM_WEAR_NECK		      4
#define ITEM_WEAR_BODY		      8
#define ITEM_WEAR_HEAD		     16
#define ITEM_WEAR_LEGS		     32
#define ITEM_WEAR_FEET		     64
#define ITEM_WEAR_HANDS		    128 
#define ITEM_WEAR_ARMS		    256
#define ITEM_WEAR_SHIELD	    512
#define ITEM_WEAR_ABOUT		   1024 
#define ITEM_WEAR_WAIST		   2048
#define ITEM_WEAR_WRIST		   4096
#define ITEM_WIELD		   8192
#define ITEM_HOLD		  16384
#define ITEM_WEAR_FACE		  32768
#define ITEM_WEAR_MASTERY	  65536
#define ITEM_WEAR_TATTOO	 ITEM_WEAR_MASTERY * 2
#define ITEM_WEAR_EAR		 ITEM_WEAR_TATTOO * 2
#define ITEM_WEAR_NUMBER	  (ITEM_WEAR_TATTOO * 2) - 1

/*
 * Special types.
 * Used in #OBJECTS for special items - KaVir.
 */
#define SITEM_ACTIVATE		      1
#define SITEM_TWIST		      2
#define SITEM_PRESS		      4
#define SITEM_PULL		      8
#define SITEM_TARGET		      16
#define SITEM_SPELL		      32
#define SITEM_TRANSPORTER	      64
#define SITEM_TELEPORTER	      128
#define SITEM_DELAY1		      256
#define SITEM_DELAY2		      512
#define SITEM_OBJECT		     1024
#define SITEM_MOBILE		     2048
#define SITEM_ACTION		     4096
#define SITEM_MORPH		     8192
#define SITEM_SILVER		    16384
#define SITEM_WOLFWEAPON	    32768
#define SITEM_DROW		    65536
#define SITEM_CHAMPWEAPON	   131072
#define SITEM_DEMONIC		   262144
#define SITEM_HIGHLANDER	   524288
#define SITEM_COPPER              1048576
#define SITEM_MAGE		  2097152
#define SITEM_STEEL		  4194304
#define SITEM_ADAMANTITE	  8388608
#define SITEM_GEMSTONE           16777216
#define SITEM_HILT               33554432
#define SITEM_PDEMONIC		 67108864
#define SITEM_MONK		134217728 
#define SITEM_IRON		268435456
#define SITEM_ELECTRUM          536870912
/*
 * Apply types (for quest affects).
 * Used in #OBJECTS.
 */
#define QUEST_STR		      1
#define QUEST_DEX		      2
#define QUEST_INT		      4
#define QUEST_WIS		      8
#define QUEST_CON		     16
#define QUEST_HITROLL		     32
#define QUEST_DAMROLL		     64
#define QUEST_HIT		    128
#define QUEST_MANA		    256
#define QUEST_MOVE		    512
#define QUEST_AC		   1024

#define QUEST_NAME		   2048
#define QUEST_SHORT		   4096
#define QUEST_LONG		   8192
#define QUEST_FREENAME		  16384

#define QUEST_ENCHANTED		  32768
#define QUEST_SPELLPROOF	  65536
#define QUEST_ARTIFACT		 131072
#define QUEST_IMPROVED		 262144
#define QUEST_ADAMANTIUM	 524288
#define QUEST_RELIC             1048576
#define QUEST_BLOODA            2097152
#define QUEST_CLONED            4194304
#define QUEST_ZOMBIE            8388608
#define QUEST_FORGE            16777216
#define QUEST_NEWBIE	       33554432
#define QUEST_MAGEENCHANT      67108864
#define QUEST_RUNE	      134217728	
#define QUEST_INFUSE          268435456 /*new mage infuse bit*/

/*
 * Tool types.
 */
#define TOOL_PEN		      1
#define TOOL_PLIERS		      2
#define TOOL_SCALPEL		      4



/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE		      0
#define APPLY_STR		      1
#define APPLY_DEX		      2
#define APPLY_INT		      3
#define APPLY_WIS		      4
#define APPLY_CON		      5
#define APPLY_SEX		      6
#define APPLY_CLASS		      7
#define APPLY_LEVEL		      8
#define APPLY_AGE		      9
#define APPLY_HEIGHT		     10
#define APPLY_WEIGHT		     11
#define APPLY_MANA		     12
#define APPLY_HIT		     13
#define APPLY_MOVE		     14
#define APPLY_GOLD		     15
#define APPLY_EXP		     16
#define APPLY_AC		     17
#define APPLY_HITROLL		     18
#define APPLY_DAMROLL		     19
#define APPLY_SAVING_PARA	     20
#define APPLY_SAVING_ROD	     21
#define APPLY_SAVING_PETRI	     22
#define APPLY_SAVING_BREATH	     23
#define APPLY_SAVING_SPELL	     24
#define APPLY_POLY		     25



/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		      1
#define CONT_PICKPROOF		      2
#define CONT_CLOSED		      4
#define CONT_LOCKED		      8



/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO		      2
#define ROOM_VNUM_CHAT		      1
#define ROOM_VNUM_TEMPLE	  41001
#define ROOM_VNUM_ALTAR		  41000
#define ROOM_VNUM_SCHOOL	 101900
#define ROOM_VNUM_HELL		  93420
#define ROOM_VNUM_CRYPT		  30001
#define ROOM_VNUM_DISCONNECTION	      3
#define ROOM_VNUM_IN_OBJECT	  33000
#define ROOM_VNUM_AWINNER            70
#define ROOM_VNUM_ALOSER             69
#define ROOM_VNUM_CAINE		  27000
#define ROOM_VNUM_DEVOUR	  30006
#define ROOM_VNUM_ELKOR 	 100300
#define ROOM_VNUM_NECROPOLIS      41142
#define ROOM_VNUM_DONATE	 420035
#define ROOM_VNUM_QUIT		     23
#define ROOM_VNUM_FFA		     24
#define ROOM_VNUM_SRECALL	  93372
#define ROOM_VNUM_BANK		  41103
/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK		      1
#define ROOM_NO_OTRANS		      2
#define ROOM_NO_MOB		      4
#define ROOM_INDOORS		      8
#define ROOM_SEX		     16
#define ROOM_PRIVATE		    512
#define ROOM_SAFE		   1024
#define ROOM_SOLITARY		   2048
#define ROOM_PET_SHOP		   4096
#define ROOM_NO_RECALL		   8192
#define ROOM_NO_TELEPORT	  16384
#define ROOM_TOTAL_DARKNESS       32768
#define ROOM_BLADE_BARRIER        65536
#define ROOM_ARENA               131072
#define ROOM_FLAMING             262144
#define ROOM_SILENCE             524288
#define ROOM_ASTRAL		1048576
#define ROOM_PROTOTYPE		2097152
#define ROOM_NO_QUIT		4194304
#define	ROOM_NO_CHANT           8388608  
#define ROOM_DAMAGE                32
#define ROOM_HEALING		   64
#define ROOM_NO_FT		  128
#define ROOM_DUEL		  256
/*
 * Room text flags (KaVir).
 * Used in #ROOMS.
 */
#define RT_LIGHTS		      1 /* Toggles lights on/off */
#define RT_SAY			      2 /* Use this if no others powers */
#define RT_ENTER		      4
#define RT_CAST			      8
#define RT_THROWOUT		     16 /* Erm...can't remember ;) */
#define RT_OBJECT		     32 /* Creates an object */
#define RT_MOBILE		     64 /* Creates a mobile */
#define RT_LIGHT		    128 /* Lights on ONLY */
#define RT_DARK			    256 /* Lights off ONLY */
#define RT_OPEN_LIFT		    512 /* Open lift */
#define RT_CLOSE_LIFT		   1024 /* Close lift */
#define RT_MOVE_LIFT		   2048 /* Move lift */
#define RT_SPELL		   4096 /* Cast a spell */
#define RT_PORTAL		   8192 /* Creates a one-way portal */
#define RT_TELEPORT		  16384 /* Teleport player to room */

#define RT_ACTION		  32768
#define RT_BLANK_1		  65536
#define RT_BLANK_2		 131072

#define RT_RETURN		1048576 /* Perform once */
#define RT_PERSONAL		2097152 /* Only shows message to char */
#define RT_TIMER		4194304 /* Sets object timer to 1 tick */



/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH		      0
#define DIR_EAST		      1
#define DIR_SOUTH		      2
#define DIR_WEST		      3
#define DIR_UP			      4
#define DIR_DOWN		      5



/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		      1
#define EX_CLOSED		      2
#define EX_LOCKED		      4
#define EX_PICKPROOF		     32
#define EX_NOPASS                    64
#define EX_EASY                     128
#define EX_HARD                     256
#define EX_INFURIATING              512  
#define EX_NOCLOSE                 1024
#define EX_NOLOCK                  2048
#define EX_ICE_WALL                4096
#define EX_FIRE_WALL               8192
#define EX_SWORD_WALL             16384
#define EX_PRISMATIC_WALL         32768
#define EX_IRON_WALL              65536	
#define EX_MUSHROOM_WALL         131072
#define EX_CALTROP_WALL          262144
#define EX_ASH_WALL              524288
#define EX_WARDING              1048576
#define EX_HIDDEN		EX_WARDING *2
#define MAX_EXFLAG		      21
#define MAX_WALL		       8



/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE		      0
#define SECT_CITY		      1
#define SECT_FIELD		      2
#define SECT_FOREST		      3
#define SECT_HILLS		      4
#define SECT_MOUNTAIN		      5
#define SECT_WATER_SWIM		      6
#define SECT_WATER_NOSWIM	      7
#define SECT_UNUSED		      8
#define SECT_AIR		      9
#define SECT_DESERT		     10
#define SECT_MAX		     11



/*
 * Equipment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE		     -1
#define WEAR_LIGHT		      0
#define WEAR_FINGER_L		      1
#define WEAR_FINGER_R		      2
#define WEAR_NECK_1		      3
#define WEAR_NECK_2		      4
#define WEAR_BODY		      5
#define WEAR_HEAD		      6
#define WEAR_LEGS		      7
#define WEAR_FEET		      8
#define WEAR_HANDS		      9
#define WEAR_ARMS		     10
#define WEAR_SHIELD		     11
#define WEAR_ABOUT		     12
#define WEAR_WAIST		     13
#define WEAR_WRIST_L		     14
#define WEAR_WRIST_R		     15
#define WEAR_WIELD		     16
#define WEAR_HOLD		     17
#define WEAR_THIRD		     18
#define WEAR_FOURTH		     19
#define WEAR_FACE		     20
#define WEAR_MASTERY		     21
#define WEAR_TATTOO		     22
#define WEAR_EAR_LEFT		     23
#define WEAR_EAR_RIGHT		     24
#define WEAR_SCABBARD_L		     25
#define WEAR_SCABBARD_R		     26
#define MAX_WEAR		     27




/*
 * Locations for damage.
 */
#define LOC_HEAD		      0
#define LOC_BODY		      1
#define LOC_ARM_L		      2
#define LOC_ARM_R		      3
#define LOC_LEG_L		      4
#define LOC_LEG_R		      5

/*
 * For Head
 */
#define LOST_EYE_L		       1
#define LOST_EYE_R		       2
#define LOST_EAR_L		       4
#define LOST_EAR_R		       8
#define LOST_NOSE		      16
#define BROKEN_NOSE		      32
#define BROKEN_JAW		      64
#define BROKEN_SKULL		     128
#define LOST_HEAD		     256
#define LOST_TOOTH_1		     512 /* These should be added..... */
#define LOST_TOOTH_2		    1024 /* ...together to caculate... */
#define LOST_TOOTH_4		    2048 /* ...the total number of.... */
#define LOST_TOOTH_8		    4096 /* ...teeth lost.  Total..... */
#define LOST_TOOTH_16		    8192 /* ...possible is 31 teeth.   */
#define LOST_TONGUE		   16384

/*
 * For Body
 */
#define BROKEN_RIBS_1		       1 /* Remember there are a total */
#define BROKEN_RIBS_2		       2 /* of 12 pairs of ribs in the */
#define BROKEN_RIBS_4		       4 /* human body, so not all of  */
#define BROKEN_RIBS_8		       8 /* these bits should be set   */
#define BROKEN_RIBS_16		      16 /* at the same time.          */
#define BROKEN_SPINE		      32
#define BROKEN_NECK		      64
#define CUT_THROAT		     128
#define CUT_STOMACH		     256
#define CUT_CHEST		     512

/*
 * For Arms
 */
#define BROKEN_ARM		       1
#define LOST_ARM		       2
#define LOST_HAND		       4
#define LOST_FINGER_I		       8 /* Index finger */
#define LOST_FINGER_M		      16 /* Middle finger */
#define LOST_FINGER_R		      32 /* Ring finger */
#define LOST_FINGER_L		      64 /* Little finger */
#define LOST_THUMB		     128
#define BROKEN_FINGER_I		     256 /* Index finger */
#define BROKEN_FINGER_M		     512 /* Middle finger */
#define BROKEN_FINGER_R		    1024 /* Ring finger */
#define BROKEN_FINGER_L		    2048 /* Little finger */
#define BROKEN_THUMB		    4096

/*
 * For Legs
 */
#define BROKEN_LEG		       1
#define LOST_LEG		       2
#define LOST_FOOT		       4
#define LOST_TOE_A		       8
#define LOST_TOE_B		      16
#define LOST_TOE_C		      32
#define LOST_TOE_D		      64 /* Smallest toe */
#define LOST_TOE_BIG		     128
#define BROKEN_TOE_A		     256
#define BROKEN_TOE_B		     512
#define BROKEN_TOE_C		    1024
#define BROKEN_TOE_D		    2048 /* Smallest toe */
#define BROKEN_TOE_BIG		    4096

/*
 * For Bleeding
 */
#define BLEEDING_HEAD		       1
#define BLEEDING_THROAT		       2
#define BLEEDING_ARM_L		       4
#define BLEEDING_ARM_R		       8
#define BLEEDING_HAND_L		      16
#define BLEEDING_HAND_R		      32
#define BLEEDING_LEG_L		      64
#define BLEEDING_LEG_R		     128
#define BLEEDING_FOOT_L		     256
#define BLEEDING_FOOT_R		     512


/*
 * For Spec powers on players
 */
#define EYE_SPELL		       1 /* Spell when they look at you */
#define EYE_SELFACTION		       2 /* You do action when they look */
#define EYE_ACTION		       4 /* Others do action when they look */




/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK		      0
#define COND_FULL		      1
#define COND_THIRST		      2



/*
 * Stats - KaVir.
 */
#define STAT_STR		      0
#define STAT_END		      1
#define STAT_REF		      2
#define STAT_FLE		      2



/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_MEDITATING		      5
#define POS_SITTING		      6
#define POS_RESTING		      7
#define POS_FIGHTING                  8
#define POS_STANDING                  9
/* Status of Arena */

#define FIGHT_OPEN                    0
#define FIGHT_START                   1
#define FIGHT_BUSY                    2
#define FIGHT_LOCK                    3

/*
 * ACT bits for players.
 */
/*
#define PLR_EMBRACING		      4	
#define PLR_EMBRACED		    256	
Seriosly bugged bits for some reason dont enable them they arent used*/
typedef enum {
PLR_IS_NPC,
PLR_AUTOGOLD,
PLR_AUTOELEC,
PLR_AUTOEXIT,
PLR_AUTOLOOT,
PLR_AUTOSAC, 
PLR_BLANK,
PLR_BRIEF,
PLR_COMBINE,
PLR_PROMPT,
PLR_TELNET_GA,
PLR_HOLYLIGHT,
PLR_WIZINVIS,
PLR_ANSI,
PLR_SILENCE,
PLR_VT102,  
PLR_INCOG,  
PLR_NO_TELL,
PLR_LOG,
PLR_DENY,
PLR_FREEZE,
PLR_AUTOSLAB,
PLR_BRIEF2,
PLR_WATCHER,
PLR_BRIEF3, 
PLR_CHALLENGER,
PLR_CHALLENGED,
PLR_TAG,       
PLR_NOTAG,
PLR_HIDE,
PLR_MAP,
PLR_MASTERY,
PLR_AUTOSPLIT
} player_act_flag_types;
#define TAG_PLAYING		(A)
#define TAG_FROZEN		(B)
#define TAG_RED			(C)
#define TAG_BLUE		(D)

/*New bits for playrs (Infidel)*/
#define NEW_SLAM       1
#define NEW_QUILLS     2
#define NEW_JAWLOCK    4
#define NEW_PERCEPTION 8
#define NEW_SKIN       16
#define NEW_TIDE       32
#define NEW_COIL       64
#define NEW_REND       128
#define NEW_MONKFLAME  256
#define NEW_SCLAWS     512
#define NEW_SUPREME    1024
#define NEW_MONKCLOAK 2048
#define NEW_MONKADAM  4096
#define NEW_MONKSKIN  8192
#define NEW_MONKFAVOR 16384
#define NEW_CLOAK     32768
#define NEW_DROWHATE  65536
#define NEW_DARKNESS  131072
#define THIRD_HAND    262144
#define FOURTH_HAND   524288
#define NEW_CLANNABLE 1048576
#define NEWBIE_PACK   2097152 // Sigh Sage you make things hard :)
#define NEW_NATURAL   4194304
#define NEW_POWER     8388608
#define NEW_DFORM     16777216
#define NEW_MASTERY     33554432
#define NEW_DARKTENDRILS 67108864
#define NEW_MULTIARMS   134217728
#define NEW_BLADESPIN   268435456
#define NEW_FIGHTDANCE	536870912
#define NEW_CUBEFORM	1073741824  //Please work dammit
//#define NEW_ARTI        2147483648

/* Version bits for extended bitvectors */
#define VERSION_EXTRA  1
#define VERSION_ACT  2
#define VERSION_AFF 4
#define VERSION_DEAF 8
#define MAX_VERSION 15

/*
 * EXTRA bits for players. (KaVir)
 */
typedef enum {
EXTRA_BLANK,
IS_A_BITCH,
FFA_HUNG,
EXTRA_TRUSTED,
EXTRA_NEWPASS,
EXTRA_OSWITCH,
EXTRA_SWITCH, 
EXTRA_FAKE_CON,
TIED_UP,       
GAGGED,        
BLINDFOLDED,   
EXTRA_STANCE,  
EXTRA_DONE,    
EXTRA_EXP,     
EXTRA_PREGNANT,
EXTRA_LABOUR,  
EXTRA_BORN,    
EXTRA_PROMPT,  
EXTRA_MARRIED, 
EXTRA_AFK,     
EXTRA_DRAGON,  
EXTRA_CALL_ALL,
EXTRA_FTALK,   
EXTRA_BSD,     
EXTRA_EARTHMELD,
EXTRA_PLASMA,   
EXTRA_POTENCY,  
EXTRA_AWE,      
EXTRA_ROT,      
EXTRA_ZOMBIE,   
EXTRA_BAAL,     
EXTRA_FLASH,
EXTRA_CHAT,
EXTRA_FIXADA,
EXTRA_JUST_PK,
EXTRA_FIXSTATUS,
EXTRA_FIXSTATS,
EXTRA_RESET,
EXTRA_FIXRESET,
EXTRA_WORMHOLE,
EXTRA_EAGLESIGHT,
EXTRA_LIESENSE,
EXTRA_AWARENESS,
EXTRA_CLAIRVOYANCE,
EXTRA_RETREAT,
EXTRA_RENEW,
EXTRA_OVERSEE,
EXTRA_ABUSE,
EXTRA_SILVERPATH,
EXTRA_DISSOLVE,
EXTRA_CHANGES,
EXTRA_POLICY,
EXTRA_BLEH,
EXTRA_SPELL_RESET,
DOUBLE_GOLD,
EXTRA_NOFOLLOW
} plr_extra_bit_types;

/* 
* AGE Bits.
 */
#define AGE_CHILDE		      0
#define AGE_NEONATE		      1
#define AGE_ANCILLA		      2
#define AGE_ELDER		      3
#define AGE_METHUSELAH		      4
#define AGE_LA_MAGRA		      5
#define AGE_TRUEBLOOD                 6
#define BELT_ONE			7
#define BELT_TWO                        8
#define BELT_THREE                        9
#define BELT_FOUR                        10
#define BELT_FIVE                        11
#define BELT_SIX                       12
#define BELT_SEVEN                        13
#define BELT_EIGHT                      14
#define BELT_NINE                      15
#define BELT_TEN                       16

/*
 * Stances for combat
 */
#define STANCE_NONE		     -1
#define STANCE_NORMAL		      0
#define STANCE_VIPER		      1
#define STANCE_CRANE		      2
#define STANCE_CRAB		      3
#define STANCE_MONGOOSE		      4
#define STANCE_BULL		      5
#define STANCE_MANTIS		      6
#define STANCE_DRAGON		      7
#define STANCE_TIGER		      8
#define STANCE_MONKEY		      9
#define STANCE_SWALLOW		     10
#define STANCE_WOLF                  11
#define STANCE_XKILLA		     12
#define STANCE_SS1		     13
#define STANCE_SS2                   14
#define STANCE_SS3                   15
#define STANCE_SS4                   16
#define STANCE_SS5                   17

/*
 *  Bits for superstances
 */
#define STANCEPOWER_DODGE              1  /* more dodge */
#define STANCEPOWER_PARRY              2  /* more parry */
#define STANCEPOWER_SPEED              4  /* 2 extra attack */
#define STANCEPOWER_BYPASS             8  /* bypass dodge/parry */
#define STANCEPOWER_DAMAGE_1          16  /* lesser increase damage and chance to hit */
#define STANCEPOWER_DAMAGE_2          32  /* greater increase damage and chance to hit */ 
#define STANCEPOWER_DAMAGE_3          64  /* supreme increase damage and chance to hit */
#define STANCEPOWER_RESIST_1         128  /* lesser resist damage */
#define STANCEPOWER_RESIST_2         256  /* greater resist damage */
#define STANCEPOWER_RESIST_3         512  /* supreme resist damage */
#define STANCEPOWER_DAMCAP_1        1024  /* lesser damcap bonus */
#define STANCEPOWER_DAMCAP_2        2048  /* greater damcap bonus */
#define STANCEPOWER_DAMCAP_3        4096  /* supreme damcap bonus */
#define STANCEPOWER_REV_DAMCAP_1    8192  /* lesser damcap penalty for opponent */
#define STANCEPOWER_REV_DAMCAP_2   16384  /* greater damcap penalty for opponent */
#define STANCEPOWER_REV_DAMCAP_3   32768  /* supreme damcap penalty for opponent */


/* other shit */
#define COMM_NOARENA		0
#define COMM_AFK		1
#define COMM_NORESTORE		2
#define SHL_INVISIBLE		3

/*
 * Obsolete bits.
 */
#if 0
#define PLR_NO_SHOUT		 131072	/* Obsolete	*/
#endif



/*
 * Channel bits.
 */
typedef enum
{
CHANNEL_INQUISITOR,
CHANNEL_CHAT,
CHANNEL_ANGEL,
CHANNEL_IMMTALK,
CHANNEL_MUSIC, 
CHANNEL_NEWBIE, 
CHANNEL_SHOUT,
CHANNEL_YELL, 
CHANNEL_VAMPTALK, 
CHANNEL_HOWL, 
CHANNEL_LOG, 
CHANNEL_PRAY, 
CHANNEL_INFO, 
CHANNEL_FLAME, 
CHANNEL_TELL, 
CHANNEL_MAGETALK, 
CHANNEL_HIGHTALK, 
CHANNEL_HINT,
CHANNEL_SIGN,
CHANNEL_MONK,
CHANNEL_MIKTALK,
CHANNEL_TELEPATH,  
CHANNEL_COMMUNICATE,
CHANNEL_KNIGHTTALK,
CHANNEL_LICHTALK,
CHANNEL_TANTALK,  
CHANNEL_PALADINTALK,
CHANNEL_UPDATE,
CHANNEL_GSOCIAL,
CHANNEL_PSIONIST,
CHANNEL_FTALK,
CHANNEL_NECROMANCER,
CHANNEL_HUNTER
} channel_flags;

struct clan_member_data {
        char            *name;
        char            *title;
	double		bank;
        EXT_BV          flags;
        CLAN_DATA       *clan;

        CLAN_MEMBER     *next;
        CLAN_MEMBER     *prev;
};

struct newclan_data
{
    CLAN_DATA *  prev; /* previous clan in the linked list */
    CLAN_DATA *  next; /* next clan in the linked list */
    int          type; /* structure of clan (see defines above */
    AREA_DATA *  area;
    char *       name;
    char *       title;
    char	*info;
    int		ext_sec;
    int		int_sec;
    CLAN_MEMBER     *first_member;
    CLAN_MEMBER     *last_member;
    double       bank;
    int          prereq[REQ_MAX];
    double	app_fee;
    int          top_member;
};

struct kingdom_desc
{
  char *princes;
  char *members;
  char *reqs;
  int  class;
};

struct c_leader
{
	char *best_demon_name;
	int best_demon_number;
        char *best_mage_name;
        int best_mage_number;
        char *best_werewolf_name;
        int best_werewolf_number;
        char *best_vampire_name;
        int best_vampire_number;
        char *best_samurai_name;
        int best_samurai_number;
        char *best_drow_name;
        int best_drow_number;
        char *best_monk_name;
        int best_monk_number;
        char *best_ninja_name;
        int best_ninja_number;
        char *best_lich_name;
        int best_lich_number;
        char *best_shapeshifter_name;
        int best_shapeshifter_number;
        char *best_tanarri_name;
        int best_tanarri_number;
        char *best_angel_name;
        int best_angel_number;
        char *best_undeadknight_name;
        int best_undeadknight_number;
        char *best_cyborg_name;
        int best_cyborg_number;
        char *best_paladin_name;
        int best_paladin_number;
        char *best_psionist_name;
        int best_psionist_number;
};
struct l_board 
{
  char *pk_name;
  int pk_number;
  char *pd_name;
  int pd_number;
  char *tt_name;
  int tt_number;
  char *qc_name;
  int qc_number;
  char *mk_name;
  int mk_number;
  char *md_name;
  int md_number;
  char *bestpk_name;
  int bestpk_number;
  char *aw_name;
  int aw_number;
  char *al_name;
  int al_number;
  char *primal_name;
  int primal_number;
  char *status_name;
  int status_number;
};
/* Structure for the map itself */
struct map_type
{
  char tegn;                    /* Character to print at this map coord */
  int vnum;                     /* Room this coord represents */
  int depth;                    /* Recursive depth this coord was found at */
  int info;
  bool can_see;
};
/*
struct king_data
{
KING_DATA *next;
KING_DATA *prev;
};
*/
struct kingdom_data
{
	OBJ_DATA *object;
	int num;
	char *name;
	char *who_name;
	char *leader_name;
	int pk;
	int pd;
	int recall;
	int altar;
};
struct war_data
{
	int one;
	int two;
};

/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct	mob_index_data
{
    MOB_INDEX_DATA *	next;
    SPEC_FUN *		spec_fun;
    SHOP_DATA *		pShop;
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    AREA_DATA *         area;           /* OLC */
    char *		hunting;
    //char *		pkilling;
    int 		huntnumber; /* Krule 04/30/02 */
    int 		temp;
    char *		player_name;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		lord;
    char *		morph;
    char *		createtime;
    char *		pload;
    char *		lasttime;
    char *		lasthost;
    char *		powertype;
    char *		poweraction;
    char *		prompt;
    char *		cprompt;
    sh_int		spectype;
    sh_int		specpower;
    int		        loc_hp	[7];
    int			vnum;
    sh_int		count;
    sh_int		killed;
    sh_int		sex;
    sh_int		mounted;
    sh_int		home;
    int		        level;
    int			immune;
    int			polyaff;
    int			vampaff;
    int			itemaffect;
    int			vamppass;
    int			form;
    EXT_BV		act;
    int			extra;
    int	affected_by;
    int	affected_by2;
    int		alignment;
    int		hitroll;		/* Unused */
    int		ac;			/* Unused */
    int		hitnodice;		/* Unused */
    int		hitsizedice;		/* Unused */
    int		hitplus;		/* Unused */
    int		damnodice;		/* Unused */
    int		damsizedice;		/* Unused */
    int		damplus;		/* Unused */
    int			gold;			/* Unused */
   /*int                 special;
    int                 class; */
};

struct balance_data
{
    int			pkills;
    int			pkilled;
    int			members;
    int			total_score;
    int			position;
    int			total_gen;
    int			num_tertiary;
    int			total_alignment;
};

struct	editor_data
{
    sh_int		numlines;
    sh_int		on_line;
    sh_int		size;
    char		line[49][81];
};

struct config_data
{
    int		demons;
    int		mages;
    int		vampires;
    int		werewolves;
    int		drow;
    int		monks;  
    int		ninjas;
};

/*
 * Used to keep track of system settings and statistics         -Thoric
 */
struct  system_data
{
    int         maxplayers;             /* Maximum players this boot		*/
    int         alltimemax;             /* Maximum players ever			*/
    char *      time_of_max;            /* Time of max ever			*/
    char *      mud_name;               /* Name of mud 				*/
    sh_int      save_frequency;         /* How often to autosave someone 	*/
    int         save_flags;             /* Toggles for saving conditions 	*/
    double      max_tot_exp;		/* Max exp a player has at 1 time	*/
    int		exp_timer;		/* Timer for double exps		*/
    int		qp_timer;		/* Timer for double qps			*/
    int 	ffa_timer;		/* Timer for Free for All		*/
    time_t	curr_change;		/* Date of most recent change		*/ 
    int		max_items;		/* Max items a player can quit with */
};


/*
 * One character (PC or NPC).
 */
struct char_data
{
    CHAR_DATA *		next;
    CHAR_DATA *		prev_in_room;
    CHAR_DATA *		next_in_room;
    CHAR_DATA *		master;
    CHAR_DATA *		leader;
    CHAR_DATA *		fighting;
    CHAR_DATA *         embracing;
    CHAR_DATA *         embraced;
    CHAR_DATA *		blinkykill;
    CHAR_DATA *		reply;
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    CHAR_DATA *         challenger; /*  person who challenged you */
    CHAR_DATA *         challenged; /*  person who you challenged */
    CHAR_DATA *         gladiator;  /*  ARENA player wagered on */
    SPEC_FUN *		spec_fun;
    MOB_INDEX_DATA *	pIndexData;
    DESCRIPTOR_DATA *	desc;
    AFFECT_DATA *	first_affect;
    AFFECT_DATA *	last_affect;
    OBJ_DATA *		carrying;
    ROOM_INDEX_DATA *	in_room;
    ROOM_INDEX_DATA *	was_in_room;
    ROOM_INDEX_DATA *   from_room;
    PC_DATA *		pcdata;
    DO_FUN *		last_cmd;
    DO_FUN *		prev_cmd;   /* mapping */
    int		tag_flags;
    char *		hunting;
    //char *		pkilling;
    int			huntnumber; /* Krule 04/30/02 */
    int			num_items;
    char *		name;
    char *              lastcap;
    char *	        pload;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		lord;
    char *		morph;
    char *		createtime;
    char *		lasttime;
    char *		lasthost;
    char *		poweraction;
    char *		powertype;
    char *		prompt;
    char *		cprompt;
    char *		prefix;
    sh_int		sex;
    sh_int		race;
    int			class;
    int			armweaps;
    int			shiftdrag;
    int			immune;
    int 		mflags;
    int 		polyaff;
    int			vampaff_a;
    int			fight_timer;
    int                 bomb_timer;
    int 		wish_timer;
    int			ffa_timer;
    int 		fury_timer;
    int			quit_timer;
    sh_int              amount_attacks_recieved;
    sh_int              amount_attacks_dealt;
    int                 amount_damage_recieved;
    int                 amount_damage_dealt;
    int			itemaffect;
    int			vamppass_a;
    int			form;
    int			warp;
    int			explevel;
    int			expgained;
    int			levelexp;
    int                 power[MAX_DISCIPLINES];
    int			xhitroll;
    int			xdamroll;
    int			denied;
/* SMAUUUUUUUUUUUG */
    void *		dest_buf;
    void *		spare_ptr;
    int			tempnum;
    EDITOR_DATA *	editor;
    sh_int		substate;
    int			pagelen;                        /* BUILD INTERFACE */
    sh_int		inter_page;                     /* BUILD INTERFACE */
    sh_int		inter_type;                     /* BUILD INTERFACE */
    char  		*inter_editing;                 /* BUILD INTERFACE */
    int			inter_editing_vnum;             /* BUILD INTERFACE */
    sh_int		inter_substate;                 /* BUILD INTERFACE */
/* End O' Smaug */

    /* Dh Flags */
    int		cclan;
    int		flag2;
    int		flag3;
    int		flag4;
    sh_int 	      generation;
    sh_int		primary;
    sh_int		proper_size;
    sh_int		size;
    sh_int		cur_form;
    sh_int	        dragtype;
    sh_int		rage;
    sh_int		siltol;
/* end */
    sh_int		tick_timer[MAX_TIMER];
    sh_int		warpcount;
    sh_int		vampgen_a;
    sh_int		spectype;
    sh_int		specpower;
    sh_int		loc_hp	[7];
    sh_int		wpn	[13];
    sh_int		spl	[8];
    sh_int		cmbt	[8];
    int  		stance	[24];
    sh_int		beast;
    sh_int		mounted;
    int		home;
    int		level;
    sh_int		trust;
    int			played;
    time_t		logon;
    time_t		save_time;
    sh_int		timer;
    sh_int		wait;
    int			pkill;
    int			pdeath;
    int			mkill;
    int			mdeath;
    int		        hit;
    int		        max_hit;
    int		mana;
    int		max_mana;
    int		move;
    int		max_move;
    double		gold;
    double		exp;
    EXT_BV		act;
    EXT_BV		extra;
    int                 newbits;
    int                 special;
    int 	affected_by;
    int 	affected_by2;
    int		position;
    int		practice;
    int		carry_weight;
    int		carry_number;
    int		saving_throw;
    int		alignment;
    int		hitroll;
    int		damroll;
    int		armor;
    int		wimpy;
    EXT_BV		deaf;
    int		paradox		[3];
    int		damcap		[2];
    int			monkstuff;
    int			monkcrap;
    int			monkab [4];
    sh_int		chi [2];
    char *		clan;
    int			gifts[21];
    int			garou1;
    int			garou2;
    sh_int		gnosis[2];
    int 		lstatus;
    int			lexp;
    CHAR_DATA *		unveil;
    char *		objdesc;
    sh_int		monkblock;
    sh_int		focus[2];
	int			spheres[9];/*mages*/
	int			imms[3];/*mages*/
	int			quint[3];/*mages*/
	int		buystatus;
};



/*
 * Data which only PC's have.
 */
struct pc_data
{
    PC_DATA *		next;
    CHAR_DATA *		familiar;
    CHAR_DATA *		partner;
    CHAR_DATA *		propose;
    CHAR_DATA *         pfile;
    char  *		killedby;
    OBJ_DATA *		chobj;
    OBJ_DATA *		memorised;
    BOARD_DATA *	board; /* The current board */
    time_t		last_note[MAX_BOARD]; /* last note for the boards */
    time_t		last_change;
    NOTE_DATA *		in_progress;
    CLAN_DATA *         clan;
    int                 clanrank;
    int			version;
    int			permissions;
    char *		pwd;
    char *		bamfin;
    char *		bamfout;
    char *		title;
//	char *              kingtitle;
    char *		conception;
    char *		parents;
    char *		cparents;
    char *		marriage;
    char *		email;
    char *		forget[MAX_FORGET];
    char *		switchname;
    char *        	custom_enter_room;
    char *        	enter_room;
    char *		subprompt;	/* Substate prompt */
    char * 		smite;
 //   char *		last_cap;
    sh_int		souls;
    sh_int		perm_str;
    sh_int		perm_int;
    sh_int		perm_wis;
    sh_int		perm_dex;
    sh_int		perm_con;
    sh_int		mod_str;
    sh_int		mod_int;
    sh_int		mod_wis;
    sh_int		mod_dex;
    sh_int		mod_con;
    int 		questsrun;
    int			questtotal;
    int			quest;
    int	 		kingdom;
    int 		pagelen;
    int                 gentick;
    int			sit_safe;
    sh_int		mortal;
    int			powers[20];
    int 		stats[12];
    int			disc_points;
    int			disc_research;
    bool                lwrgen;
    sh_int		wolf;
    sh_int		rank;
    sh_int		demonic_a;
    int			language[2];
    sh_int		stage[3];
    sh_int		wolfform[2];
    int			score[6];
    sh_int		runes[4];
    sh_int		disc_a[11];
    int			genes[10];
    sh_int		fake_skill;
    sh_int		fake_stance;
    sh_int		fake_hit;
    sh_int		fake_dam;
    sh_int		fake_hp;
    sh_int		fake_mana;
    sh_int		fake_move;
    int			fake_ac;
    int			obj_vnum;
    sh_int		condition	[3];
    sh_int		learned		[MAX_SKILL];
    sh_int		stat_ability	[4];
    sh_int		stat_amount	[4];
    sh_int		stat_duration	[4];
    sh_int		exhaustion;
    sh_int              followers;
    double                 bank;
    double                 bankqp;
	int			dragonaff;		/*dragons*/
	int			dragonage;		/*dragons*/
	int			hatch;			/*dragons*/
	int			dragon_sleep;	/*dragons*/
	char *			breath[5]; /* dragons */
    int                 plr_wager; /*  ARENA amount wagered     */
    int                 awins;     /*  ARENA number of wins     */
    int                 alosses;   /*  ARENA number of losses  */
    int			comm;
    int                 security;       /* OLC - Builder security */
    int			bounty;
    int			changling[10];
    int			magejunk[10];
    char		stackchar;
    char		speedchar;
    ALIAS_DATA *        first_alias;
    ALIAS_DATA *        last_alias;
    sh_int              alias_count;
};



/*
 * Liquids.
 */
#define LIQ_WATER        0
#define LIQ_MAX		19

struct	liq_type
{
    char *	liq_name;
    char *	liq_color;
    sh_int	liq_affect[3];
};



/*
 * Extra description data for a room or object.
 */
struct	extra_descr_data
{
    EXTRA_DESCR_DATA *next;	/* Next in list                     */
    EXTRA_DESCR_DATA *prev;	/* Previous			    */
    char *keyword;              /* Keyword in look/examine          */
    char *description;          /* What to see                      */
};
// Jobo's Aliases from Rand
struct alias_data
{
  ALIAS_DATA *next;
  ALIAS_DATA *prev;
  char *short_n;
  char *long_n;
};

/*
 * Prototype for an object.
 */
struct	obj_index_data
{
    OBJ_INDEX_DATA *	next;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AFFECT_DATA *	first_affect;
    AFFECT_DATA *	last_affect; 
    AREA_DATA *         area;           /* OLC */
    char *		name;
    char *		short_descr;
    char *		description;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    int			vnum;
    sh_int		item_type;
    int			extra_flags;
    sh_int		extra_flags2;
    int	        	wear_flags;
    sh_int		count;
    sh_int		weight;
    sh_int		weapflags;
    int			spectype;
    int			specpower;
    sh_int		condition;
    sh_int		toughness;
    sh_int		resistance;
    int			quest;
    sh_int		points;
    int			cost;			/* Unused */
    int			value	[4];
};



/*
 * One object.
 */
struct	obj_data
{
    OBJ_DATA *		next;
    OBJ_DATA *		next_content;
    OBJ_DATA *		contains;
    OBJ_DATA *		in_obj;
    CHAR_DATA *		carried_by;
    CHAR_DATA *		chobj;
    char *		killedby;
    char *		player;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AFFECT_DATA *	first_affect;
    AFFECT_DATA *	last_affect;
    OBJ_INDEX_DATA *	pIndexData;
    ROOM_INDEX_DATA *	in_room;
    char *		name;
    char *		short_descr;
    char *		description;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    int		item_type;
    int		extra_flags;
    int		extra_flags2;
    int		wear_flags;
    int		wear_loc;
    int		weight;
    int		weapflags;
    int			spectype;
    int			specpower;
    int		condition;
    int		toughness;
    int		resistance;
    int			quest;
    sh_int		points;
    int			cost;
    int		level;
    int		timer;
    int			value	[4];
};



/*
 * Exit data.
 */
struct	exit_data
{
    EXIT_DATA *		prev;		/* previous exit in linked list	*/
    EXIT_DATA *		next;		/* next exit in linked list	*/
    EXIT_DATA *		rexit;		/* Reverse exit pointer		*/
    ROOM_INDEX_DATA *	to_room;	/* Pointer to destination room	*/
    char *		keyword;	/* Keywords for exit or door	*/
    char *		description;	/* Description of exit		*/
    int			vnum;		/* Vnum of room exit leads to	*/
    int			rvnum;		/* Vnum of room in opposite dir	*/
    int			exit_info;	/* door states & other flags	*/
    int			key;		/* Key vnum			*/
    sh_int		vdir;		/* 0,5 N\E\S\W\U\D shit		*/
    int                 rs_flags;       /* OLC */
    int                 orig_door;      /* OLC */

};

/*
 * Room text checking data.
 */
typedef struct roomtext_data
{
    int				type;
    int				power;
    int				mob;
    char *			input;
    char *			output;
    char *			choutput;
    char *			name;
    struct roomtext_data	*next;
} ROOMTEXT_DATA;



/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile 
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct	reset_data
{
    RESET_DATA *	next;
    char		command;
    int		arg1;
    int		arg2;
    int		arg3;
};



/*
 * Area definition.
 */
struct  area_data   
{
    AREA_DATA *         next;
    AREA_DATA *         prev;
    char *              name;
    int                 recall;
    int                 age;
    int			perm_age;
    int                 nplayer;
    char *              filename;       /* OLC */
    char *              builders;       /* OLC - Listing of builders */
    int                 security;       /* OLC - Value 0-infinity  */
    int                 lvnum;          /* OLC - Lower vnum */
    int                 uvnum;          /* OLC - Upper vnum */
    int                 vnum;           /* OLC - Area vnum  */
    int                 area_flags;     /* OLC */
};


/*
 * Room type.
 */
struct	room_index_data
{
    ROOM_INDEX_DATA *	next;
    ROOM_INDEX_DATA *   next_room;
    CHAR_DATA *		people;
    CHAR_DATA *		first_person;
    CHAR_DATA *		last_person;
    OBJ_DATA *		contents;
    OBJ_DATA *		first_content;
    OBJ_DATA *		last_content;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AREA_DATA *		area;
    EXIT_DATA *		exit	  [6];
    EXIT_DATA *		first_exit;
    EXIT_DATA *		last_exit;
    ROOMTEXT_DATA *	roomtext;
    RESET_DATA *        reset_first;    /* OLC */
    RESET_DATA *        reset_last;     /* OLC */

    char *		track     [5];
    char *		name;
    char *		description;
    int			vnum;
    int		        room_flags;
    sh_int		light;
    sh_int		blood;
    sh_int		track_dir [5];
    sh_int		sector_type;
    sh_int		tick_timer[MAX_RTIMER];
};



/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000



/*
 *  Target types.
 */
#define TAR_IGNORE		    0
#define TAR_CHAR_OFFENSIVE	    1
#define TAR_CHAR_DEFENSIVE	    2
#define TAR_CHAR_SELF		    3
#define TAR_OBJ_INV		    4

#define TAR_OBJ_CHAR_DEF            5
#define TAR_OBJ_CHAR_OFF            6
#define TAR_OBJ_ROOM		    7
#define TAR_EXIT		    8
#define TAR_CHAR_WORLD		    9

#define TARGET_CHAR                 0
#define TARGET_OBJ                  1
#define TARGET_ROOM                 2
#define TARGET_NONE                 3

#define PURPLE_MAGIC		    0
#define RED_MAGIC		    1
#define BLUE_MAGIC		    2
#define GREEN_MAGIC		    3
#define YELLOW_MAGIC		    4



/*
 * Skills include spells as a particular case.
 */
struct	skill_type
{
    char *	name;			/* Name of skill		*/
    sh_int	skill_level;	        /* Level needed by class	*/
    SPELL_FUN *	spell_fun;		/* Spell pointer (for spells)	*/
    sh_int	target;			/* Legal targets		*/
    sh_int	minimum_position;	/* Position for caster / user	*/
    sh_int *	pgsn;			/* Pointer to associated gsn	*/
    sh_int	slot;			/* Slot for #OBJECT loading	*/
    int	min_mana;		/* Minimum mana used		*/
    sh_int	beats;			/* Waiting time after use	*/
    char *	noun_damage;		/* Damage message		*/
    char *	msg_off;		/* Wear off message		*/
};



/*
 * These are skill_lookup return values for common skills and spells.
 */
extern  sh_int	gsn_stuntubes;
extern  sh_int	gsn_laser;
extern  sh_int  gsn_stinger;
extern  sh_int  gsn_music;
extern  sh_int  gsn_quills;
extern  sh_int  gsn_cheapshot;
extern  sh_int  gsn_shred;
extern  sh_int  gsn_heavenlyaura;
extern  sh_int  gsn_bladespin;
extern	sh_int	gsn_fiery;
extern  sh_int  gsn_hooves;
extern  sh_int	gsn_claws;
extern  sh_int  gsn_fireball;
extern  sh_int  gsn_deathaura;
extern  sh_int  gsn_lightningslash;
extern  sh_int  gsn_wrathofgod;
extern  sh_int  gsn_darktendrils;
extern  sh_int  gsn_mageshield;
extern  sh_int  gsn_venomtong;
extern  sh_int  gsn_spiketail;
extern  sh_int  gsn_badbreath;
extern  sh_int  gsn_magma;
extern  sh_int  gsn_hellfire;
extern  sh_int  gsn_shards;
extern  sh_int  gsn_spiderform;
extern  sh_int  gsn_garotte;
extern	sh_int	gsn_backstab;
extern	sh_int	gsn_hide;
extern	sh_int	gsn_peek;
extern	sh_int	gsn_pick_lock;
extern	sh_int	gsn_sneak;
extern	sh_int	gsn_steal;
extern  sh_int	gsn_godbless;  /* Vic - Monks */
extern  sh_int	gsn_totalblind; /* Vic - Monks */
extern	sh_int	gsn_tendrils;
extern	sh_int	gsn_berserk;
extern	sh_int	gsn_punch;
extern	sh_int	gsn_headbutt;
extern  sh_int  gsn_spiket;
extern  sh_int  gsn_venomt;
extern  sh_int  gsn_magma;
extern  sh_int  gsn_shards;
extern  sh_int  gsn_shiroken;
extern  sh_int  gsn_blinky;
extern  sh_int  gsn_inferno;
extern  sh_int  gsn_fangs;
extern  sh_int  gsn_buffet;
extern  sh_int  gsn_rfangs;
extern	sh_int	gsn_sweep;
extern	sh_int	gsn_knee;
extern  sh_int  gsn_spinkick;
extern  sh_int  gsn_backfist;
extern  sh_int  gsn_jumpkick;
extern  sh_int  gsn_monksweep;
extern  sh_int  gsn_thrustkick;
extern  sh_int  gsn_spinkick;
extern  sh_int  gsn_elbow;
extern  sh_int  gsn_palmstrike;
extern  sh_int  gsn_shinkick;
extern  sh_int  gsn_lightningkick;
extern  sh_int  gsn_tornadokick;
extern	sh_int	gsn_disarm;
extern	sh_int	gsn_hurl;
extern	sh_int	gsn_kick;
extern  sh_int  gsn_chillhand;
extern  sh_int  gsn_circle;
extern  sh_int  gsn_booming;
extern	sh_int	gsn_rescue;
extern	sh_int	gsn_track;
extern	sh_int	gsn_polymorph;
extern	sh_int	gsn_web;
extern  sh_int  gsn_infirmity;
extern  sh_int  gsn_drowfire;
extern	sh_int	gsn_blindness;
extern	sh_int	gsn_charm_person;
extern	sh_int	gsn_curse;
extern	sh_int	gsn_invis;
extern	sh_int	gsn_mass_invis;
extern	sh_int	gsn_poison;
extern	sh_int	gsn_sleep;
extern	sh_int	gsn_paradox;
extern  sh_int  gsn_spew;
extern	sh_int	gsn_darkness;
extern  sh_int  gsn_multiplearms;
extern  sh_int  gsn_arrowiv;
extern  sh_int  gsn_sweepblow;
extern  sh_int	gsn_assassinate;
extern  sh_int  gsn_shadowblast;
extern  sh_int  gsn_sanctuary;
extern  sh_int  gsn_spin;
extern  sh_int	gsn_leech;
extern	sh_int	gsn_divinewrath;
extern  sh_int	gsn_riposte;
extern  sh_int	gsn_feint;
extern  sh_int	gsn_enfeeble;
extern  sh_int	gsn_mindblast;
extern  sh_int  gsn_critical_hit;
extern  sh_int  gsn_slide;
extern  sh_int  gsn_hiltstrike;
extern  sh_int  gsn_shinku;
extern  sh_int  gsn_choke;
extern  sh_int  gsn_kisetsufuu;
extern  sh_int  gsn_kaminari;
extern  sh_int  gsn_inabikari;
extern  sh_int	gsn_inquisitor;
extern  sh_int	gsn_unholytorment;
extern  sh_int	gsn_purify;
extern  sh_int	gsn_slash;
extern  sh_int	gsn_knock;
extern  sh_int  gsn_riftwalk;
extern  sh_int  gsn_recall;
/*
 * Utility macros.
 */
#define IS_VALID(data)          ((data) != NULL && (data)->valid)
#define VALIDATE(data)          ((data)->valid = TRUE)
#define INVALIDATE(data)        ((data)->valid = FALSE)
#define IS_VALID_SN(sn)         ( (sn) >=0 && (sn) < MAX_SKILL               \
                                && skill_table[(sn)]                         \
                                && skill_table[(sn)]->name )

#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))
#define TOGGLE_BIT(var, bit)	((var) ^= (bit))
#define IS_FIGHTING(ch)		((ch)->fighting)
#define IS_FIGHTING_PLAYER(ch)          ((ch)->fighting ? (IS_NPC(ch->fighting) ? FALSE : TRUE) : FALSE )
#define COLOUR(ch,color,bit)    (ADD_COLOUR((ch),(bit),(color));)

bool is_fighting_player ( CHAR_DATA *victim, CHAR_DATA *attacker );


/*
 * Here are the extended bitvector macros:
 */
#define xIS_SET(var, bit)	((var).bits[(bit) >> RSV] & 1 << ((bit) & XBM))
#define xSET_BIT(var, bit)	((var).bits[(bit) >> RSV] |= 1 << ((bit) & XBM))
#define xSET_BITS(var, bit)	(ext_set_bits(&(var), &(bit)))
#define xREMOVE_BIT(var, bit)	((var).bits[(bit) >> RSV] &= ~(1 << ((bit) & XBM)))
#define xREMOVE_BITS(var, bit)	(ext_remove_bits(&(var), &(bit)))
#define xTOGGLE_BIT(var, bit)	((var).bits[(bit) >> RSV] ^= 1 << ((bit) & XBM))
#define xTOGGLE_BITS(var, bit)	(ext_toggle_bits(&(var), &(bit)))
#define xCLEAR_BITS(var)	(ext_clear_bits(&(var)))
#define xIS_EMPTY(var)		(ext_is_empty(&(var)))
#define xHAS_BITS(var, bit)	(ext_has_bits(&(var), &(bit)))
#define xSAME_BITS(var, bit)	(ext_same_bits(&(var), &(bit)))
#define CAN_CLAN(ch, var, bit) (xIS_SET(var, CL_LEADER) ||xIS_SET(var, bit) || IS_IMMORTAL( ch))

/*
 * Memory allocation macros.
 */

#define CREATE(result, type, number)				\
do								\
{								\
   if (!((result) = (type *) calloc ((number), sizeof(type))))	\
   {                                                            \
     perror("malloc failure");                                  \
     fprintf(stderr, "Malloc failure @ %s:%d\n", __FILE__, __LINE__ ); \
     abort();                                                   \
   }                                                            \
} while(0)

#define RECREATE(result,type,number)				\
do								\
{								\
  if (!((result) = (type *) realloc ((result), sizeof(type) * (number))))\
  {                                                             \
     perror("realloc failure");                                 \
     fprintf(stderr, "Realloc failure @ %s:%d\n", __FILE__, __LINE__ ); \
     abort(); }                                                 \
} while(0)


#define DISPOSE(point) 						\
do								\
{								\
  if (!(point))							\
  {								\
	bug( "Freeing null pointer in %s, line %d",  __FILE__, __LINE__  ); \
	fprintf( stderr, "DISPOSEing NULL in %s, line %d\n", __FILE__, __LINE__ ); \
  }								\
  else 								\
  {								\
     free(point);						\
     point = NULL;						\
  }									\
} while(0)

#ifdef HASHSTR
#define STRALLOC(point)		str_dup((point))
#define QUICKLINK(point)	quick_link((point))
#define QUICKMATCH(p1, p2)	(int) (p1) == (int) (p2)
#define STRFREE(point)						\
do								\
{								\
   if (!(point)) 			\
   {             			\
      bug( "Freeing null pointer in %s, line %d", __FILE__, __LINE__  ); \
      fprintf( stderr, "STRFREEing bad pointer in %s, line %d\n", __FILE__, __LINE__ ); \
   }					\
   else					\
   {					\
      free_string((point));		\
      point = NULL; 			\
   }					\
} while(0)
#else
#define STRALLOC(point)		str_dup((point))
#define QUICKLINK(point)	str_dup((point))
#define QUICKMATCH(p1, p2)	strcmp((p1), (p2)) == 0
#define STRFREE(point)						\
do								\
{								\
  if ((point))							\
     free_string((point));						\
  point = NULL;							\
} while(0)
#endif

/* double-linked list handling macros -Thoric */

#define LINK(link, first, last, next, prev)			\
do								\
{								\
    if ( !(first) )						\
      (first)			= (link);			\
    else							\
      (last)->next		= (link);			\
    (link)->next		= NULL;				\
    (link)->prev		= (last);			\
    (last)			= (link);			\
} while(0)

#define INSERT(link, insert, first, next, prev)			\
do								\
{								\
    (link)->prev		= (insert)->prev;		\
    if ( !(insert)->prev )					\
      (first)			= (link);			\
    else							\
      (insert)->prev->next	= (link);			\
    (insert)->prev		= (link);			\
    (link)->next		= (insert);			\
} while(0)

#define UNLINK(link, first, last, next, prev)			\
do								\
{								\
    if ( !(link)->prev )					\
      (first)			= (link)->next;			\
    else							\
      (link)->prev->next	= (link)->next;			\
    if ( !(link)->next )					\
      (last)			= (link)->prev;			\
    else							\
      (link)->next->prev	= (link)->prev;			\
} while(0)


#define CHECK_LINKS(first, last, next, prev, type)		\
do {								\
  type *ptr, *pptr = NULL;					\
  if ( !(first) && !(last) )					\
    break;							\
  if ( !(first) )						\
  {								\
    bug( "CHECK_LINKS: last with NULL first!  %s.",		\
        __STRING(first) );					\
    for ( ptr = (last); ptr->prev; ptr = ptr->prev );		\
    (first) = ptr;						\
  }								\
  else if ( !(last) )						\
  {								\
    bug( "CHECK_LINKS: first with NULL last!  %s.",		\
        __STRING(first) );					\
    for ( ptr = (first); ptr->next; ptr = ptr->next );		\
    (last) = ptr;						\
  }								\
  if ( (first) )						\
  {								\
    for ( ptr = (first); ptr; ptr = ptr->next )			\
    {								\
      if ( ptr->prev != pptr )					\
      {								\
        bug( "CHECK_LINKS(%s): %p:->prev != %p.  Fixing.",	\
            __STRING(first), ptr, pptr );			\
        ptr->prev = pptr;					\
      }								\
      if ( ptr->prev && ptr->prev->next != ptr )		\
      {								\
        bug( "CHECK_LINKS(%s): %p:->prev->next != %p.  Fixing.",\
            __STRING(first), ptr, ptr );			\
        ptr->prev->next = ptr;					\
      }								\
      pptr = ptr;						\
    }								\
    pptr = NULL;						\
  }								\
  if ( (last) )							\
  {								\
    for ( ptr = (last); ptr; ptr = ptr->prev )			\
    {								\
      if ( ptr->next != pptr )					\
      {								\
        bug( "CHECK_LINKS (%s): %p:->next != %p.  Fixing.",	\
            __STRING(first), ptr, pptr );			\
        ptr->next = pptr;					\
      }								\
      if ( ptr->next && ptr->next->prev != ptr )		\
      {								\
        bug( "CHECK_LINKS(%s): %p:->next->prev != %p.  Fixing.",\
            __STRING(first), ptr, ptr );			\
        ptr->next->prev = ptr;					\
      }								\
      pptr = ptr;						\
    }								\
  }								\
} while(0)


#define ASSIGN_GSN(gsn, skill)					\
do								\
{								\
    if ( ((gsn) = skill_lookup((skill))) == -1 )		\
	fprintf( stderr, "ASSIGN_GSN: Skill %s not found.\n",	\
		(skill) );					\
} while(0)

#define CHECK_SUBRESTRICTED(ch)					\
do								\
{								\
    if ( (ch)->substate == SUB_RESTRICTED )			\
    {								\
	send_to_char( "You cannot use this command from within another command.\n\r", ch );	\
	return;							\
    }								\
} while(0)


/*
 * Character macros.
 */
#define IS_COMB(ch, sn)		(IS_SET((ch)->monkcrap, (sn)))
#define IS_FS(ch, sn)		(IS_SET((ch)->monkstuff, (sn)))
#define IS_NEWFLAG(ch, sn)      (IS_SET((ch)->flag2, (sn)))
#define IS_CREATOR(ch)		(get_trust(ch) >= MAX_LEVEL)
#define IS_BASE_FORM(ch)	(((ch)->cur_form >= 38 && (ch)->cur_form <= 50) || ((ch)->cur_form >=23 && (ch)->cur_form <= 36) || ((ch)->cur_form >= FRM_WERE_RAT && (ch)->cur_form <= FRM_WERE_SHARK) || (ch)->cur_form == FRM_PIXIE)
#define GET_FORM(ch)            ((form_data[(ch)->cur_form].short_desc == NULL || form_data[(ch)->cur_form].short_desc[0] == '\0') ? form_data[(ch)->cur_form].name : "")
#define GET_PROPER_NAME(ch)     (IS_NPC((ch)) ? (ch)->short_descr : (ch)->pcdata->switchname)
#define GET_PC_NAME(ch)     	(IS_NPC((ch)) ? "<npc>" : (ch)->pcdata->switchname)

#define IS_NPC(ch)		(xIS_SET((ch)->act, ACT_IS_NPC))
#define IS_JUDGE(ch)		(get_trust(ch) >= LEVEL_ENFORCER)
#define IS_IMMORTAL(ch)		(get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_HERO(ch)		(get_trust(ch) >= LEVEL_HERO)
#define IS_IMPLEMENTOR(ch)	(get_trust(ch) >= MAX_LEVEL)
#define IS_NEWBIE(wch)		((get_age(wch)-17)<4)
#define IS_FAIR_GAME(ch)        ((get_age(ch)-17)<4)
#define IS_DAY()	    	((weather_info.sunlight == SUN_RISE || weather_info.sunlight == SUN_LIGHT))
#define IS_NIGHT()	    	((!IS_DAY()))

#define CAN_PK(ch)		(get_trust(ch) >= 3 && get_trust(ch)<= 12)
#define IS_AFFECTED(ch, sn)	(IS_SET((ch)->affected_by, (sn)))
#define IS_AFF2(ch, sn)		(IS_SET((ch)->affected_by2, (sn)))
#define IS_SPEAKING(ch, sn)	(IS_SET((ch)->pcdata->language[0], (sn)))
#define CAN_SPEAK(ch, sn)	(IS_SET((ch)->pcdata->language[1], (sn)))
#define IS_ITEMAFF(ch, sn)	(IS_SET((ch)->itemaffect, (sn)))
#define IS_IMMUNE(ch, sn)	(IS_SET((ch)->immune, (sn)))
#define IS_VAMPAFF(ch, sn)	(IS_SET((ch)->pcdata->stats[UNI_AFF], (sn)))
#define IS_VAMPPASS(ch, sn)	(IS_SET((ch)->pcdata->stats[UNI_CURRENT], (sn))) 
#define IS_FORM(ch, sn)		(1)
#define IS_POLYAFF(ch, sn)	(IS_SET((ch)->polyaff, (sn)))
#define IS_EXTRA(ch, sn)	(xIS_SET((ch)->extra, (sn)))
#define IS_STANCE(ch, sn)	(ch->stance[0] == sn)
#define IS_DEMPOWER(ch, sn)	(IS_SET((ch)->pcdata->powers[DPOWER_FLAGS], (sn)))
#define IS_DEMAFF(ch, sn)	(IS_SET((ch)->pcdata->powers[DPOWER_CURRENT], (sn))) 
#define IS_CLASS(ch, CLASS)	(IS_SET((ch)->class, CLASS) && (ch->level >= LEVEL_AVATAR))
#define IS_WHO_CLASS(ch, CLASS)	(IS_SET((ch)->class, CLASS))
#define IS_HEAD(ch, sn)		(IS_SET((ch)->loc_hp[0], (sn)))
#define IS_BODY(ch, sn)		(IS_SET((ch)->loc_hp[1], (sn)))
#define IS_ARM_L(ch, sn)	(IS_SET((ch)->loc_hp[2], (sn)))
#define IS_ARM_R(ch, sn)	(IS_SET((ch)->loc_hp[3], (sn)))
//#define IS_ARM_T(ch, sn)	(IS_SET((ch)->loc_hp[7], (sn)))
//#define IS_ARM_F(ch, sn)	(IS_SET((ch)->loc_hp[8], (sn)))
#define IS_LEG_L(ch, sn)	(IS_SET((ch)->loc_hp[4], (sn)))
#define IS_LEG_R(ch, sn)	(IS_SET((ch)->loc_hp[5], (sn)))
#define IS_BLEEDING(ch, sn)	(IS_SET((ch)->loc_hp[6], (sn)))

#define IS_PLAYING( d )         (d->connected==CON_PLAYING)
 
#define IS_GOOD(ch)		(ch->alignment >= 1)
#define IS_EVIL(ch)		(ch->alignment <= -1)
#define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))

#define IS_AWAKE(ch)		(ch->position > POS_SLEEPING)
#define GET_AC(ch)		((ch)->armor				    \
				    + ( IS_AWAKE(ch)			    \
				    ? dex_app[get_curr_dex(ch)].defensive   \
				    : 0 ))
#define GET_HITROLL(ch)		((ch)->hitroll+str_app[get_curr_str(ch)].tohit)
#define GET_DAMROLL(ch)		((ch)->damroll+str_app[get_curr_str(ch)].todam)

#define IS_OUTSIDE(ch)		(!IS_SET(				    \
				    (ch)->in_room->room_flags,		    \
				    ROOM_INDOORS))

#define WAIT_STATE(ch, npulse)	\
if (!IS_IMMORTAL((ch))) \
((ch)->wait = UMAX((ch)->wait, (npulse)))



/*
 * Object Macros.
 */
#define CAN_WEAR(obj, part)	(IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(IS_SET((obj)->extra_flags, (stat)))
#define IS_OBJ_STAT2(obj, stat) (IS_SET((obj)->extra_flags2,(stat)))
#define IS_WEAP(obj, stat)      (IS_SET((obj)->weapflags,  (stat)))


/*
 * Description macros.
 */
#define PERS(ch, looker)	( can_see( looker, (ch) ) ?		\
				( IS_NPC(ch) ? (ch)->short_descr	\
                                : ( IS_AFFECTED( (ch), AFF_POLYMORPH) ? \
				(ch)->morph : (ch)->name ) )		\
				: "someone" )


/*
 * Structure for a command in the command lookup table.
 */
struct  cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
    sh_int		position;
    sh_int		level;
    sh_int		log;
    int              race; /* 0 = all, other = specific race */
    sh_int		discipline; /* USE THE DISC_VAMP_???? etc.... */
    sh_int	       	disclevel;  /* level in disc the command is granted */
};



struct hint_type
{
    char *	text;
};

/*
 * Structure for a social in the socials table.
 */
struct	social_type
{
    char * 	name;
    char * 	char_no_arg;
    char * 	others_no_arg;
    char * 	char_found;
    char * 	others_found;
    char * 	vict_found;
    char * 	char_auto;
    char * 	others_auto;
};


/*
 * Structure for an X-social in the socials table.
 */
struct	xsocial_type
{
    char * const	name;
    char * const	char_no_arg;
    char * const	others_no_arg;
    char * const	char_found;
    char * const	others_found;
    char * const	vict_found;
    char * const	char_auto;
    char * const	others_auto;
    sh_int		gender;
    sh_int		stage;
    sh_int		position;
    sh_int		self;
    sh_int		other;
    sh_int              extra;
    bool		chance;
};



/*
 * Global constants.
 */


extern	const	struct	str_app_type	str_app		[36];
extern	const	struct	int_app_type	int_app		[36];
extern	const	struct	wis_app_type	wis_app		[36];
extern	const	struct	dex_app_type	dex_app		[36];
extern	const	struct	con_app_type	con_app		[36];

extern	const	struct	cmd_type	cmd_table	[];
extern	const	struct	liq_type	liq_table	[LIQ_MAX];
extern	const	struct	skill_type	skill_table	[MAX_SKILL];
extern		struct	artifact_type	artifact_table	[MAX_ART];
extern  const   char    *		discipline	[MAX_DISCIPLINES];
extern  const   char    *               wwgift          [MAX_GIFTS];
extern 		struct 	hint_type 	*hint_table;
extern 		struct	social_type 	*social_table;
extern	const	struct	xsocial_type	xsocial_table	[];

extern  const   struct  race_type       race_table      [];
extern  const   char    * colour_name                   [8];
extern  const   char	* circlesymbol			[7];
extern  const   struct	clanrank_type	clanrank	[9][14];
extern  const   struct  form_type       form_data       [MAX_FORMS];

/*
 * Global variables.
 */

extern          ALIAS_DATA        *     first_alias;
extern          ALIAS_DATA        *     last_alias;
extern		HELP_DATA	  *	help_first;
extern		HELP_DATA	  *	help_last;

extern		SHOP_DATA	  *	shop_first;

extern		HELP_DATA	  *	first_help;
extern		HELP_DATA	  *	last_help;
extern		BAN_DATA	  *	first_ban;
extern		BAN_DATA	  *	last_ban;
extern		CHAR_DATA	  *	char_list;
extern		DESCRIPTOR_DATA   *	first_descriptor;
extern		DESCRIPTOR_DATA   *	last_descriptor;
extern		NOTE_DATA	  *	note_list;
extern		OBJ_DATA	  *	object_list;

extern		BALANCE_DATA	  	balance_info[7];
extern		CONFIG_DATA	  *	config_info;
extern          ROOM_INDEX_DATA   *     room_list;
extern		CHAR_DATA	  *	char_free;
extern		EXTRA_DESCR_DATA  *	extra_descr_free;
extern		ROOMTEXT_DATA     *	roomtext_free;
extern		NOTE_DATA	  *	note_free;
extern		OBJ_DATA	  *	obj_free;
extern          BAN_DATA          *     ban_free;
extern		CHAR_DATA	  *	quitting_char;
extern		char			bug_buf		[];
extern		time_t			current_time;
extern		bool			fLogAll;
extern		FILE *			fpReserve;
extern		KILL_DATA		kill_table	[];
extern		char			log_buf		[];
extern		TIME_INFO_DATA		time_info;
extern		WEATHER_DATA		weather_info;
extern          int                 	arena;
extern          int                 	arena_timer;
extern		bool			global_exp;
extern		int			global_exp_timer;
extern		bool			global_qp;
extern		int			global_qp_timer;
extern		int			global_ffa_timer;
extern		bool			global_status;
extern		bool			extra_log;
extern		int			doxlimit;
extern		int			players_logged;
extern          int                     players_decap;
extern          int                     players_gstolen;
extern  struct  system_data             sysdata;

/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */
int             chprintf( CHAR_DATA* ch, const char *format, ... );

/* Jasons Commands */
DECLARE_DO_FUN( do_logcmd );
DECLARE_DO_FUN( do_todo );
DECLARE_DO_FUN( do_quote );
DECLARE_DO_FUN( do_instaroom );
DECLARE_DO_FUN( do_swearlist );
DECLARE_DO_FUN( do_multiplay );
DECLARE_DO_FUN( do_multiple );
DECLARE_DO_FUN( do_cls );
DECLARE_DO_FUN( do_sedit );
DECLARE_DO_FUN( do_gsocial );
DECLARE_DO_FUN( do_global );
DECLARE_DO_FUN( do_nuke );
DECLARE_DO_FUN( do_delete );
DECLARE_DO_FUN( do_ftalk );
DECLARE_DO_FUN( do_flist );
DECLARE_DO_FUN( do_backup );
DECLARE_DO_FUN( do_cset );
DECLARE_DO_FUN( do_rename );
DECLARE_DO_FUN( do_balance );
DECLARE_DO_FUN( do_deposit );
DECLARE_DO_FUN( do_withdraw );
DECLARE_DO_FUN( do_nofollow );
DECLARE_DO_FUN( do_newclan );
DECLARE_DO_FUN( do_showclan );
DECLARE_DO_FUN( do_save_clans );
/* Jope */
void jope_newbits(CHAR_DATA *ch, char *argument);
void jope_act(CHAR_DATA *ch, char *argument);
void jope_action(CHAR_DATA *ch, char *argument);
void jope_inventory(CHAR_DATA *ch, char *argument);
void jope_wear(CHAR_DATA *ch, char *argument);
void jope_drop(CHAR_DATA *ch, char *argument);
void jope_equipment(CHAR_DATA *ch, char *argument);
void jope_get(CHAR_DATA *ch, char *argument);
void jope_look(CHAR_DATA *ch, char *argument);
void jope_remove(CHAR_DATA *ch, char *argument);
void jope_qps(CHAR_DATA *ch, char *argument);
void jope_primal(CHAR_DATA *ch, char *argument);
void jope_level(CHAR_DATA *ch, char *argument);
void jope_trust(CHAR_DATA *ch, char *argument);
void jope_hit(CHAR_DATA *ch, char *argument);
void jope_mana(CHAR_DATA *ch, char *argument);
void jope_move(CHAR_DATA *ch, char *argument);
void jope_mdeath(CHAR_DATA *ch, char *argument);
void jope_mkill(CHAR_DATA *ch, char *argument);
void jope_pdeath(CHAR_DATA *ch, char *argument);
void jope_pkill(CHAR_DATA *ch, char *argument);
void jope_stances(CHAR_DATA *ch, char *argument);
void jope_weapons(CHAR_DATA *ch, char *argument);
void jope_spells(CHAR_DATA *ch, char *argument);
void jope_exp(CHAR_DATA *ch, char *argument);
void jope_list(CHAR_DATA *ch, char *argument);
void jope_show(CHAR_DATA *ch, char *argument);
void jope_done(CHAR_DATA *ch, char *argument);
void jope_interp(CHAR_DATA *ch, char *argument);
void jope_free_char(CHAR_DATA *ch);

/* Wobinz/Mobius/Rand  */
DECLARE_DO_FUN(do_riftwalk	);
DECLARE_DO_FUN(do_exptoken	);
DECLARE_DO_FUN(do_blessing	);
DECLARE_DO_FUN(do_knock		);
DECLARE_DO_FUN(do_buy		);
DECLARE_DO_FUN(do_hint		);
DECLARE_DO_FUN(do_hints		);
DECLARE_DO_FUN(do_scatter	);
DECLARE_DO_FUN(do_showtele	);
DECLARE_DO_FUN(do_policy	);
DECLARE_DO_FUN(do_groups	);
DECLARE_DO_FUN(do_srecall	);
DECLARE_DO_FUN(do_naturalharmony);
DECLARE_DO_FUN(do_pkwhere	);
DECLARE_DO_FUN(do_godsfire	);
DECLARE_DO_FUN(do_clear_class_leader);
DECLARE_DO_FUN(do_classleader	);
DECLARE_DO_FUN(do_newbie	);
DECLARE_DO_FUN(do_purchase	);
DECLARE_DO_FUN(do_fixstats	);
DECLARE_DO_FUN( do_alias                );  // Jobo's Aliases from Rand
DECLARE_DO_FUN( do_showalias            ); // Jobo's Aliases from Rand
DECLARE_DO_FUN( do_removealias          );// Jobo's Aliases from Rand
DECLARE_DO_FUN(do_pscore	); // Pscore by Huw
/* Inquisitor by Rand */
DECLARE_DO_FUN(do_fixfury	);
DECLARE_DO_FUN(do_staff		);
DECLARE_DO_FUN(do_enlist	);
DECLARE_DO_FUN(do_walk		);
DECLARE_DO_FUN(do_purify	);
DECLARE_DO_FUN(do_torture	);
DECLARE_DO_FUN(do_torment	);
DECLARE_DO_FUN(do_chains	);
DECLARE_DO_FUN(do_unholyritual	);
DECLARE_DO_FUN(do_unholyfury	);
DECLARE_DO_FUN(do_inquire	);
/* Samurai from Rand */
DECLARE_DO_FUN(do_inabikari     );
DECLARE_DO_FUN(do_safaia        );
DECLARE_DO_FUN(do_strike        );
DECLARE_DO_FUN(do_shinku        );
DECLARE_DO_FUN(do_midori        );
DECLARE_DO_FUN(do_shiroi        );
DECLARE_DO_FUN(do_kisetsufuu    );
DECLARE_DO_FUN(do_kaminari      );

/*Psionist by Rand*/
 DECLARE_DO_FUN(do_mindgate);
 DECLARE_DO_FUN(do_enhance);
 DECLARE_DO_FUN(do_psi_eq);
 DECLARE_DO_FUN(do_wrath);
 DECLARE_DO_FUN(do_mindblast);
 DECLARE_DO_FUN(do_enfeeble);
 DECLARE_DO_FUN(do_meddle);
 DECLARE_DO_FUN(do_entrapment);
 DECLARE_DO_FUN(do_massassinate);
 DECLARE_DO_FUN(do_psionisttalk);
/* Paladin by Rand */
DECLARE_DO_FUN( do_quarter);
DECLARE_DO_FUN( do_paladineye);
DECLARE_DO_FUN( do_paladintravel);
DECLARE_DO_FUN( do_paladinweapons);
DECLARE_DO_FUN( do_paladintalk);
DECLARE_DO_FUN( do_reflect);
DECLARE_DO_FUN( do_bladeslash);
DECLARE_DO_FUN( do_charge);
DECLARE_DO_FUN( do_adamantiumeq);
DECLARE_DO_FUN( do_supreme);
DECLARE_DO_FUN( do_divinewrath);
DECLARE_DO_FUN( do_breakfree);
DECLARE_DO_FUN( do_tip		);
DECLARE_DO_FUN( do_update	);
DECLARE_DO_FUN( do_legendexp 	);
DECLARE_DO_FUN( do_pkguardian	);
DECLARE_DO_FUN( do_ffa		);
DECLARE_DO_FUN( do_general	);
DECLARE_DO_FUN( do_pkdamage	);
DECLARE_DO_FUN( do_pkteleport	);
DECLARE_DO_FUN( do_pkflee	);
DECLARE_DO_FUN( do_pkready	);
DECLARE_DO_FUN( do_donate	);
DECLARE_DO_FUN( do_toss		);
DECLARE_DO_FUN( do_dice		);
DECLARE_DO_FUN( do_wish		);
DECLARE_DO_FUN( do_pkportal2	);
DECLARE_DO_FUN( do_checkbalance );
DECLARE_DO_FUN( do_checkdamcap  );
DECLARE_DO_FUN( do_buystatus );
DECLARE_DO_FUN( do_mobius_bm    );
DECLARE_DO_FUN( do_mobius       );
DECLARE_DO_FUN( do_mobius_heal  );
DECLARE_DO_FUN( do_kingeq       );
DECLARE_DO_FUN( do_kingdomeq    );
DECLARE_DO_FUN( do_meta		);
DECLARE_DO_FUN( do_autogold     );
DECLARE_DO_FUN( do_autoadamantite);
DECLARE_DO_FUN( do_autoelectrum );
DECLARE_DO_FUN( do_buyqp	);
DECLARE_DO_FUN( do_shatter      );
DECLARE_DO_FUN( do_email        );
DECLARE_DO_FUN( do_range	);
DECLARE_DO_FUN( do_resetchar    );
DECLARE_DO_FUN( do_mercy	);
/* New Commands */
DECLARE_DO_FUN( do_challenge );
DECLARE_DO_FUN( do_decline );
DECLARE_DO_FUN( do_agree );
DECLARE_DO_FUN( do_changes );
DECLARE_DO_FUN( do_affects     );
DECLARE_DO_FUN( do_smite	);
DECLARE_DO_FUN( do_showchar	);
DECLARE_DO_FUN( do_airmail	);
DECLARE_DO_FUN( do_chands	);

/* SPIDER DROID */
DECLARE_DO_FUN( do_cubeform	);
DECLARE_DO_FUN( do_dridereq	);
DECLARE_DO_FUN( do_stuntubes	);
DECLARE_DO_FUN( do_infravision	);
DECLARE_DO_FUN( do_implant	);

/* Tag */
DECLARE_DO_FUN( do_notag        );

DECLARE_DO_FUN( do_ftag );
DECLARE_DO_FUN( do_ztag );
DECLARE_DO_FUN( do_red );
DECLARE_DO_FUN( do_blue );

/* Mages */
DECLARE_DO_FUN( do_thirdeye	  );
DECLARE_DO_FUN( do_objectgate	  );
DECLARE_DO_FUN( do_discharge	  );
DECLARE_DO_FUN( do_enchant	  );


// Vampies
// Daimoinon
DECLARE_DO_FUN( do_guardian 	);
DECLARE_DO_FUN( do_fear		);
DECLARE_DO_FUN( do_portal	);
DECLARE_DO_FUN( do_curse	);
DECLARE_DO_FUN( do_vtwist	);

// Melpominee
DECLARE_DO_FUN( do_scream	);

// Chimerstry
DECLARE_DO_FUN( do_mirror	);
DECLARE_DO_FUN( do_formillusion );
//DECLARE_DO_FUN( do_clone	);
DECLARE_DO_FUN( do_control	);
//DECLARE_DO_FUN( do_objmask	);

// Necromancy
DECLARE_DO_FUN( do_thanatosis	);
DECLARE_DO_FUN( do_preserve	);
DECLARE_DO_FUN( do_spiritgate	);
DECLARE_DO_FUN( do_spiritguard	);
DECLARE_DO_FUN( do_zombie	);

// Thanatosis
DECLARE_DO_FUN( do_hagswrinkles );
DECLARE_DO_FUN( do_rot		);
DECLARE_DO_FUN( do_withering	);
DECLARE_DO_FUN( do_drain	);

// Auspex
DECLARE_DO_FUN( do_truesight	);
DECLARE_DO_FUN( do_readaura	);
DECLARE_DO_FUN( do_scry		);
DECLARE_DO_FUN( do_astralwalk   );
DECLARE_DO_FUN( do_unveil	);

// Obfuscate
DECLARE_DO_FUN( do_vanish	);
DECLARE_DO_FUN( do_mask		);
DECLARE_DO_FUN( do_shield	);
DECLARE_DO_FUN( do_mortal	);
DECLARE_DO_FUN( do_objectmask	);


/* Garou */

// Persuasion
DECLARE_DO_FUN( do_staredown    );
DECLARE_DO_FUN( do_disquiet     );
DECLARE_DO_FUN( do_reshape      );
DECLARE_DO_FUN( do_cocoon	);
DECLARE_DO_FUN( do_quills	);
DECLARE_DO_FUN( do_burrow	);
DECLARE_DO_FUN( do_nightsight	);
DECLARE_DO_FUN( do_wither	);
DECLARE_DO_FUN( do_totemgift	);

DECLARE_DO_FUN( do_razorclaws   );


/* End of Garou */

DECLARE_DO_FUN( do_research	);
DECLARE_DO_FUN( do_disciplines  );
DECLARE_DO_FUN( do_classself    );
DECLARE_DO_FUN( do_knee		);
DECLARE_DO_FUN( do_elbow	);
DECLARE_DO_FUN( do_sweep	);
DECLARE_DO_FUN( do_reverse	);
DECLARE_DO_FUN( do_palmstrike	);
DECLARE_DO_FUN( do_shinkick	);
DECLARE_DO_FUN( do_chi		);
DECLARE_DO_FUN( do_thrustkick	);
DECLARE_DO_FUN( do_spinkick	);
DECLARE_DO_FUN( do_backfist	);
DECLARE_DO_FUN( do_spiritpower	);
DECLARE_DO_FUN( do_healingtouch );
DECLARE_DO_FUN( do_deathtouch	);
DECLARE_DO_FUN( do_relax	);

DECLARE_DO_FUN( do_learn	);
DECLARE_DO_FUN( do_noarm	);
DECLARE_DO_FUN( do_testarm	);
DECLARE_DO_FUN( do_testcmd	);
DECLARE_DO_FUN( do_pfile	);
DECLARE_DO_FUN( do_new_pload	);
DECLARE_DO_FUN( do_mitsukeru	);
DECLARE_DO_FUN( do_koryou	);
DECLARE_DO_FUN( do_udc		);
DECLARE_DO_FUN( do_labour	);
DECLARE_DO_FUN( do_abortion     );
DECLARE_DO_FUN( do_god_fight    ); 
DECLARE_DO_FUN( do_damn		);
DECLARE_DO_FUN( do_darkness	);
DECLARE_DO_FUN( do_refresh      );
DECLARE_DO_FUN( do_vt102        );
DECLARE_DO_FUN( do_retrn        );
DECLARE_DO_FUN( do_far		);
DECLARE_DO_FUN( do_dragon       );
DECLARE_DO_FUN( do_rot          );
DECLARE_DO_FUN( do_forget       );
DECLARE_DO_FUN( do_forge        );
DECLARE_DO_FUN( do_spew         );
DECLARE_DO_FUN( do_spiderform	);
DECLARE_DO_FUN( do_flash        );
DECLARE_DO_FUN( do_infirmity	);
DECLARE_DO_FUN( do_coil         );
DECLARE_DO_FUN( do_tide         );
DECLARE_DO_FUN( do_death        );
DECLARE_DO_FUN( do_acid         );
DECLARE_DO_FUN( do_awe          );
DECLARE_DO_FUN( do_satan        );
/*ninjas*/
DECLARE_DO_FUN( do_screen);
DECLARE_DO_FUN( do_ninjaarmor   );
DECLARE_DO_FUN( do_hara_kiri	);
DECLARE_DO_FUN( do_miktalk	);
DECLARE_DO_FUN( do_principles   );
DECLARE_DO_FUN( do_michi        );
DECLARE_DO_FUN( do_artteach     );
DECLARE_DO_FUN( do_kakusu       );
DECLARE_DO_FUN( do_kanzuite     );
DECLARE_DO_FUN( do_mienaku      );
DECLARE_DO_FUN( do_flashbomb    );
DECLARE_DO_FUN( do_belt         );
DECLARE_DO_FUN( do_target       );
DECLARE_DO_FUN( do_bomuzite     );
DECLARE_DO_FUN( do_tsume	);
DECLARE_DO_FUN( do_circle	);
DECLARE_DO_FUN( do_ignore	);
DECLARE_DO_FUN( do_unignore	);
DECLARE_DO_FUN( do_omni		);
DECLARE_DO_FUN( do_showemail	);
DECLARE_DO_FUN( do_testform	);
DECLARE_DO_FUN( do_frostbreath	);
DECLARE_DO_FUN( do_dgate	);
DECLARE_DO_FUN( do_tick		);
DECLARE_DO_FUN( do_form		);
DECLARE_DO_FUN( do_resetarea	);
DECLARE_DO_FUN( do_graft	);
DECLARE_DO_FUN( do_racestats	);
DECLARE_DO_FUN( do_blink	);
DECLARE_DO_FUN( do_dinferno	);
DECLARE_DO_FUN( do_immolate	);
DECLARE_DO_FUN( do_seed		);
DECLARE_DO_FUN( do_hellfire	);
DECLARE_DO_FUN( do_newbiepack   );
DECLARE_DO_FUN( do_runeeq	);
DECLARE_DO_FUN( do_holyeq  	);
DECLARE_DO_FUN( do_mithrileq    );
DECLARE_DO_FUN( do_chaoseq   	);
DECLARE_DO_FUN( do_aligneq      );
DECLARE_DO_FUN( do_classeq	);
DECLARE_DO_FUN( do_afk          );
DECLARE_DO_FUN( do_unnerve	);
DECLARE_DO_FUN( do_wfreeze	);
DECLARE_DO_FUN( do_chaosport    );
DECLARE_DO_FUN( do_caust	);
DECLARE_DO_FUN( do_gust		);
DECLARE_DO_FUN( do_entomb	);
DECLARE_DO_FUN( do_leech	);
DECLARE_DO_FUN( do_configure	);
DECLARE_DO_FUN( do_stealsoul	);
DECLARE_DO_FUN( do_deathsense	);
DECLARE_DO_FUN( do_glamour	);
DECLARE_DO_FUN( do_prefix	);

/* OLC Stuff Ooo */
DECLARE_DO_FUN( do_hlist	);
DECLARE_DO_FUN( do_hedit	);
DECLARE_DO_FUN( do_hset		);
DECLARE_DO_FUN( do_ocreate	);
DECLARE_DO_FUN( do_mcreate	);
DECLARE_DO_FUN( do_redit	);

/* Dunkirk's shit SuckDick */
DECLARE_DO_FUN( do_bloodwall	);
DECLARE_DO_FUN( do_bloodrod	);

DECLARE_DO_FUN( do_sclaws	);
/* Protean */
DECLARE_DO_FUN( do_healing	);

/* Obtene */
DECLARE_DO_FUN( do_grab		);
DECLARE_DO_FUN( do_shadowgaze	);

/* Luna Powers */

DECLARE_DO_FUN( do_tribe	);
DECLARE_DO_FUN( do_flameclaws   );
DECLARE_DO_FUN( do_moonarmour	);
DECLARE_DO_FUN( do_motherstouch	);
DECLARE_DO_FUN( do_gmotherstouch);
DECLARE_DO_FUN( do_moongate	);
DECLARE_DO_FUN( do_moonbeam	);

/* No more Luna Powers */

DECLARE_DO_FUN( do_skin		);
DECLARE_DO_FUN( do_bonemod	);
DECLARE_DO_FUN( do_vset		);
DECLARE_DO_FUN( do_cauldron	);
DECLARE_DO_FUN( do_flamehands	);
DECLARE_DO_FUN( do_conceal	);
DECLARE_DO_FUN( do_drain	);
DECLARE_DO_FUN( do_shroud	);
DECLARE_DO_FUN( do_share	);
DECLARE_DO_FUN( do_frenzy	);
DECLARE_DO_FUN( do_chaosblast	);
DECLARE_DO_FUN(	do_accept	);
DECLARE_DO_FUN( do_obtain	);
DECLARE_DO_FUN( do_warps	);
DECLARE_DO_FUN( do_setdisciplines);

/* End of Dunkirk's Shit */

DECLARE_DO_FUN(	do_activate	);
DECLARE_DO_FUN(	do_alignment	);
DECLARE_DO_FUN(	do_allow	);
DECLARE_DO_FUN(	do_secset	);
DECLARE_DO_FUN(	do_addlag	);
DECLARE_DO_FUN(	do_bounty	);
DECLARE_DO_FUN(	do_bountylist	);
DECLARE_DO_FUN(	do_ansi		);
DECLARE_DO_FUN(	do_areas	);
DECLARE_DO_FUN(	do_artifact	);
DECLARE_DO_FUN( do_rares	);
DECLARE_DO_FUN(	do_at		);
DECLARE_DO_FUN(	do_auction	);
DECLARE_DO_FUN(	do_autoexit	);
DECLARE_DO_FUN(	do_autoloot	);
DECLARE_DO_FUN( do_autosplit	);
DECLARE_DO_FUN(	do_autosac	);
DECLARE_DO_FUN(	do_autosave	);
DECLARE_DO_FUN( do_autostance	);
DECLARE_DO_FUN(	do_backstab	);
DECLARE_DO_FUN(	do_bamfin	);
DECLARE_DO_FUN(	do_bamfout	);
DECLARE_DO_FUN(	do_ban		);
DECLARE_DO_FUN(	do_beastlike	);
DECLARE_DO_FUN(	do_berserk	);
DECLARE_DO_FUN( do_spin	);
DECLARE_DO_FUN(	do_bind		);
DECLARE_DO_FUN(	do_birth	);
DECLARE_DO_FUN(	do_blank	);
DECLARE_DO_FUN(	do_blindfold	);
DECLARE_DO_FUN(	do_bloodline	);
DECLARE_DO_FUN(	do_brandish	);
DECLARE_DO_FUN(	do_breakup	);
DECLARE_DO_FUN(	do_brief	);
DECLARE_DO_FUN( do_brief2	);
DECLARE_DO_FUN( do_brief3       );
DECLARE_DO_FUN( do_map		);
DECLARE_DO_FUN(	do_bug		);
DECLARE_DO_FUN(	do_call		);
DECLARE_DO_FUN(	do_calm		);
DECLARE_DO_FUN(	do_cast		);
DECLARE_DO_FUN(	do_change	);
DECLARE_DO_FUN(	do_changelight	);
DECLARE_DO_FUN(	do_channels	);
DECLARE_DO_FUN( do_knightarmor  );
DECLARE_DO_FUN( do_gain		);
DECLARE_DO_FUN( do_weaponpractice );
DECLARE_DO_FUN( do_aura		);
DECLARE_DO_FUN( do_powerword	);
DECLARE_DO_FUN( do_knighttalk	);
DECLARE_DO_FUN( do_ride		);
DECLARE_DO_FUN( do_unholyrite	);
/* Mages */
DECLARE_DO_FUN(	do_chant	);
DECLARE_DO_FUN( do_reveal	);
DECLARE_DO_FUN( do_magics       );
DECLARE_DO_FUN( do_blast        );
DECLARE_DO_FUN( do_teleport     );
DECLARE_DO_FUN( do_invoke       );
DECLARE_DO_FUN( do_magearmor	);
DECLARE_DO_FUN( do_chaosmagic	);
DECLARE_DO_FUN( do_infuse       );
DECLARE_DO_FUN( do_bomb         );
/* Necromancer by Huw */
DECLARE_DO_FUN( do_absorb       );
DECLARE_DO_FUN( do_deatheyes    );
DECLARE_DO_FUN( do_decayflesh   );
/* Angels */
DECLARE_DO_FUN( do_gpeace        );
DECLARE_DO_FUN( do_innerpeace    );
DECLARE_DO_FUN( do_houseofgod    );
DECLARE_DO_FUN( do_angelicaura   );
DECLARE_DO_FUN( do_gbanish       );
DECLARE_DO_FUN( do_harmony       );
DECLARE_DO_FUN( do_gsenses       );
DECLARE_DO_FUN( do_gfavor        );
DECLARE_DO_FUN( do_forgivness    );
DECLARE_DO_FUN( do_martyr        );
DECLARE_DO_FUN( do_swoop         );
DECLARE_DO_FUN( do_awings        );
DECLARE_DO_FUN( do_halo          );
DECLARE_DO_FUN( do_sinsofthepast );
DECLARE_DO_FUN( do_eyeforaneye   );
DECLARE_DO_FUN( do_angelicarmor  );
DECLARE_DO_FUN( do_angeltalk	 );
DECLARE_DO_FUN( do_touchofgod	 );
DECLARE_DO_FUN( do_spiritform	 );
DECLARE_DO_FUN( do_repent	 );

/* Shapeshifters */
DECLARE_DO_FUN( do_shift	);
DECLARE_DO_FUN( do_formlearn	);
DECLARE_DO_FUN( do_camouflage	);
DECLARE_DO_FUN( do_shapeshift	);
DECLARE_DO_FUN( do_hatform	);
DECLARE_DO_FUN( do_mistwalk	);
DECLARE_DO_FUN( do_shapearmor	);
DECLARE_DO_FUN( do_shaperoar	);
DECLARE_DO_FUN( do_stampede	);
DECLARE_DO_FUN( do_faerieblink	);
DECLARE_DO_FUN( do_stomp	);
DECLARE_DO_FUN( do_tigercurse   );
DECLARE_DO_FUN( do_phase	);
DECLARE_DO_FUN( do_breath	);

DECLARE_DO_FUN( do_monkarmor	);
DECLARE_DO_FUN( do_vampirearmor );
DECLARE_DO_FUN( do_purification    );
DECLARE_DO_FUN( do_chaosblast	);
DECLARE_DO_FUN(	do_chat		);
DECLARE_DO_FUN(	do_claim	);
DECLARE_DO_FUN( do_flame        );
DECLARE_DO_FUN( do_clan		);
DECLARE_DO_FUN( do_clanlist	);
DECLARE_DO_FUN( do_kingdom	);
DECLARE_DO_FUN( do_warlist	);
DECLARE_DO_FUN( do_decwar	);
DECLARE_DO_FUN( do_warpeace	);
DECLARE_DO_FUN( do_krecall	);
DECLARE_DO_FUN( do_kwhere	);
DECLARE_DO_FUN( do_showkingdesc	);
DECLARE_DO_FUN( do_leader	);
DECLARE_DO_FUN( do_leaderclear	);
DECLARE_DO_FUN( do_kingdesc	);
DECLARE_DO_FUN( do_ktalk	);
DECLARE_DO_FUN(	do_clandisc	);
DECLARE_DO_FUN( do_introduce	);
DECLARE_DO_FUN( do_clannum	);
DECLARE_DO_FUN( do_clanset	);
DECLARE_DO_FUN(	do_show_clan_info );
DECLARE_DO_FUN(	do_clanname	);
DECLARE_DO_FUN( do_clantalk	);
DECLARE_DO_FUN( do_induct	);
DECLARE_DO_FUN( do_clannable	);
DECLARE_DO_FUN( do_prince	);
DECLARE_DO_FUN( do_outcast	);
DECLARE_DO_FUN(	do_claws	);
DECLARE_DO_FUN( do_class	);
DECLARE_DO_FUN(	do_clearstats	);
DECLARE_DO_FUN( do_clearstats2  );
DECLARE_DO_FUN(	do_close	);
DECLARE_DO_FUN(	do_command	);
DECLARE_DO_FUN(	do_commands	);
DECLARE_DO_FUN(	do_config	);
DECLARE_DO_FUN(	do_consent	);
DECLARE_DO_FUN(	do_consider	);
DECLARE_DO_FUN(	do_cprompt	);
DECLARE_DO_FUN(	do_crack	);
DECLARE_DO_FUN(	do_create	);
DECLARE_DO_FUN(	do_credits	);
DECLARE_DO_FUN(	do_darkheart	);
DECLARE_DO_FUN( do_vampdarkness	);
DECLARE_DO_FUN(	do_decapitate	);
DECLARE_DO_FUN( do_gifts	);
DECLARE_DO_FUN( do_trueform	);
DECLARE_DO_FUN( do_cone		);
DECLARE_DO_FUN( do_dstake	);
DECLARE_DO_FUN(	do_demonarmour	);
DECLARE_DO_FUN(	do_deny		);
DECLARE_DO_FUN(	do_description	);
DECLARE_DO_FUN(	do_desc		);
DECLARE_DO_FUN(	do_diagnose	);
DECLARE_DO_FUN(	do_dismount	);
DECLARE_DO_FUN(do_disable);
DECLARE_DO_FUN(	do_disarm	);
DECLARE_DO_FUN(	do_disconnect	);
DECLARE_DO_FUN(	do_divorce	);
DECLARE_DO_FUN(	do_down		);
DECLARE_DO_FUN(	do_draw		);
DECLARE_DO_FUN(	do_drink	);
DECLARE_DO_FUN(	do_drop		);
// drows
DECLARE_DO_FUN( do_shadowblast  );
DECLARE_DO_FUN( send_grantlist);
DECLARE_DO_FUN( do_drowcreate	);
DECLARE_DO_FUN( do_heal		);
DECLARE_DO_FUN( do_drowfire	);
DECLARE_DO_FUN( do_drowhate	);
DECLARE_DO_FUN( do_drowpowers	);
DECLARE_DO_FUN( do_drowshield	);
DECLARE_DO_FUN( do_lloth	);
DECLARE_DO_FUN( do_drowsight	);
DECLARE_DO_FUN( do_darktendrils);
DECLARE_DO_FUN( do_fightdance);
DECLARE_DO_FUN(	do_ntrust	);
DECLARE_DO_FUN(	do_offer	);
DECLARE_DO_FUN(	do_grant	);
DECLARE_DO_FUN( do_burrow      );
DECLARE_DO_FUN( do_earthmeld    );
DECLARE_DO_FUN(	do_east		);
DECLARE_DO_FUN(	do_eat		);
DECLARE_DO_FUN(	do_echo		);
DECLARE_DO_FUN(	do_empty	);
DECLARE_DO_FUN(	do_escape	);
DECLARE_DO_FUN(	do_emote	);
DECLARE_DO_FUN(	do_enter	);
DECLARE_DO_FUN(	do_equipment	);
DECLARE_DO_FUN(	do_evileye	);
DECLARE_DO_FUN(	do_examine	);
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN( do_exlist	);
DECLARE_DO_FUN( do_eyespy	);
DECLARE_DO_FUN(	do_familiar	);
DECLARE_DO_FUN(	do_fangs	);
DECLARE_DO_FUN( do_favor	);
DECLARE_DO_FUN(	do_favour	);
DECLARE_DO_FUN(	do_fcommand	);
DECLARE_DO_FUN(	do_fightstyle	);
DECLARE_DO_FUN(	do_fileupdate	);
DECLARE_DO_FUN(	do_fill		);
DECLARE_DO_FUN(	do_finger	);
DECLARE_DO_FUN(	do_see_pfile	);
DECLARE_DO_FUN(	do_flee		);
DECLARE_DO_FUN(	do_flex		);
DECLARE_DO_FUN(	do_follow	);
DECLARE_DO_FUN(do_for);
DECLARE_DO_FUN(	do_force	);
DECLARE_DO_FUN(	do_forceauto	);
DECLARE_DO_FUN(	do_freeze	);
DECLARE_DO_FUN( do_earthshatter	);
DECLARE_DO_FUN( do_confuse	);
DECLARE_DO_FUN(	do_gag		);
DECLARE_DO_FUN( do_garotte	);
DECLARE_DO_FUN( do_dark_garotte	);
DECLARE_DO_FUN(	do_get		);
DECLARE_DO_FUN( do_generation	);
DECLARE_DO_FUN(	do_gift		);
DECLARE_DO_FUN(	do_give		);
DECLARE_DO_FUN(	do_goto		);
DECLARE_DO_FUN( do_rank		);
DECLARE_DO_FUN( do_grant	);
DECLARE_DO_FUN(	do_group	);
DECLARE_DO_FUN(	do_gtell	);/*
DECLARE_DO_FUN(do_heal	);*/
DECLARE_DO_FUN(	do_help		);
DECLARE_DO_FUN(	do_newhelp	);
DECLARE_DO_FUN(	do_hide		);
DECLARE_DO_FUN( do_hightalk	);
DECLARE_DO_FUN( do_monktalk	);
DECLARE_DO_FUN(	do_holylight	);
DECLARE_DO_FUN(	do_home		);
DECLARE_DO_FUN(	do_horns	);
DECLARE_DO_FUN(	do_howl		);
DECLARE_DO_FUN(	do_telepath	);
DECLARE_DO_FUN( do_communicate  );
DECLARE_DO_FUN(	do_huh		);
DECLARE_DO_FUN(	do_humanform	);
DECLARE_DO_FUN(	do_humanity	);
DECLARE_DO_FUN(	do_hunt		);
DECLARE_DO_FUN(	do_hurl		);
DECLARE_DO_FUN(	do_idea		);
DECLARE_DO_FUN(	do_immune	);
DECLARE_DO_FUN(	do_immtalk	);
DECLARE_DO_FUN( do_incog        );
DECLARE_DO_FUN(	do_inconnu	);
DECLARE_DO_FUN(	do_info		);
DECLARE_DO_FUN( do_infof	);
DECLARE_DO_FUN(	do_inpart	);
DECLARE_DO_FUN( do_demonarmour	);
DECLARE_DO_FUN(	do_inventory	);
DECLARE_DO_FUN(	do_invis	);
DECLARE_DO_FUN(	do_kick		);
DECLARE_DO_FUN( do_godsfavor    );

/* Tanar'ri */
DECLARE_DO_FUN( do_earthquake	);
DECLARE_DO_FUN( do_tornado	);
DECLARE_DO_FUN( do_infernal	);
DECLARE_DO_FUN( do_bloodsac	);
DECLARE_DO_FUN( do_enmity	);
DECLARE_DO_FUN( do_enrage	);
DECLARE_DO_FUN( do_booming	);
DECLARE_DO_FUN( do_chaosgate	);
DECLARE_DO_FUN( do_fury		);
DECLARE_DO_FUN( do_tantalk	);
DECLARE_DO_FUN( do_taneq	);
DECLARE_DO_FUN( do_lavablast	);
DECLARE_DO_FUN( do_chaossurge	);

/* Power Lich */
DECLARE_DO_FUN( do_lore         );
DECLARE_DO_FUN( do_studylore    );
DECLARE_DO_FUN( do_lichtalk	);
DECLARE_DO_FUN( do_chaosshield	);
DECLARE_DO_FUN( do_summongolem  );
DECLARE_DO_FUN( do_planartravel );
DECLARE_DO_FUN( do_planarstorm	);
DECLARE_DO_FUN( do_powertransfer);
DECLARE_DO_FUN( do_polarity	);
DECLARE_DO_FUN( do_chillhand	);
DECLARE_DO_FUN( do_creepingdoom	);
DECLARE_DO_FUN( do_painwreck	);
DECLARE_DO_FUN( do_earthswallow	);
DECLARE_DO_FUN( do_licharmor	);
DECLARE_DO_FUN( do_soulsuck	);
DECLARE_DO_FUN( do_pentagram	);
DECLARE_DO_FUN( do_planeshift	);

/* Monk */
DECLARE_DO_FUN( do_reseteq      );
DECLARE_DO_FUN( do_mantra	);
DECLARE_DO_FUN( do_wrathofgod );
DECLARE_DO_FUN( do_guide	);
DECLARE_DO_FUN( do_makezerg     );
DECLARE_DO_FUN( do_monk		);
DECLARE_DO_FUN( do_zergpower    );
DECLARE_DO_FUN( do_zclan	);
DECLARE_DO_FUN( do_superinvis	);
DECLARE_DO_FUN( do_supershield	);
DECLARE_DO_FUN( do_celestial    );
DECLARE_DO_FUN( do_godseye	);
DECLARE_DO_FUN( do_godsbless    );
DECLARE_DO_FUN( do_flaminghands );
DECLARE_DO_FUN( do_cloak	);
DECLARE_DO_FUN( do_prayofages   );
DECLARE_DO_FUN( do_darkblaze	);
DECLARE_DO_FUN( do_adamantium	);
DECLARE_DO_FUN( do_steelskin	);
DECLARE_DO_FUN( do_godsheal	);
DECLARE_DO_FUN( do_ghold	);
DECLARE_DO_FUN( do_callgod	);
DECLARE_DO_FUN( do_savecmd	);
DECLARE_DO_FUN( do_wallofswords );
DECLARE_DO_FUN( do_holytrain	);
DECLARE_DO_FUN( do_holyvision	);
DECLARE_DO_FUN( do_prayer	);
DECLARE_DO_FUN( do_palmthrust	);
DECLARE_DO_FUN( do_sacredinvis  );
DECLARE_DO_FUN(	do_boot		);
DECLARE_DO_FUN(	do_kill		);
DECLARE_DO_FUN(	do_buymod	);
DECLARE_DO_FUN(	do_page		);
DECLARE_DO_FUN(	do_killperson	);
DECLARE_DO_FUN(	do_leap		);
DECLARE_DO_FUN(	do_lifespan	);
DECLARE_DO_FUN(	do_locate	);
DECLARE_DO_FUN(	do_lock		);
DECLARE_DO_FUN(	do_log		);
DECLARE_DO_FUN(	do_look		);
DECLARE_DO_FUN(	do_majesty	);
DECLARE_DO_FUN( do_makepreg	);
DECLARE_DO_FUN(	do_marry	);
DECLARE_DO_FUN(	do_mask		);
DECLARE_DO_FUN(	do_mclear	);
DECLARE_DO_FUN( do_clearsuper       );
DECLARE_DO_FUN(	do_meditate	);
DECLARE_DO_FUN(	do_memory	);
DECLARE_DO_FUN(	do_mfind	);
DECLARE_DO_FUN(	do_mload	);
DECLARE_DO_FUN(	do_mount	);
DECLARE_DO_FUN(	do_morph	);
DECLARE_DO_FUN(	do_mortal	);
DECLARE_DO_FUN(	do_mortalvamp	);
DECLARE_DO_FUN( do_undeny       );
DECLARE_DO_FUN(	do_mstat	);
DECLARE_DO_FUN(	do_mwhere	);
DECLARE_DO_FUN(	do_music	);
DECLARE_DO_FUN(	do_nightsight	);
DECLARE_DO_FUN(	do_noemote	);
DECLARE_DO_FUN(	do_north	);
DECLARE_DO_FUN(	do_note		);
DECLARE_DO_FUN(	do_notell	);
DECLARE_DO_FUN(	do_oclone	);
DECLARE_DO_FUN(	do_ofind	);
DECLARE_DO_FUN( do_owhere       );
DECLARE_DO_FUN( do_nohelps      );
DECLARE_DO_FUN( do_odelete      );
DECLARE_DO_FUN( do_mdelete      );
DECLARE_DO_FUN(	do_oload	);
DECLARE_DO_FUN(	do_open		);
DECLARE_DO_FUN(	do_order	);
DECLARE_DO_FUN(	do_oreturn	);
DECLARE_DO_FUN( do_outcast	);
DECLARE_DO_FUN( do_reimb    );
DECLARE_DO_FUN( do_reimburse    );
DECLARE_DO_FUN( do_pack         );
DECLARE_DO_FUN(	do_oset		);
DECLARE_DO_FUN(	do_ostat	);
DECLARE_DO_FUN(	do_oswitch	);
DECLARE_DO_FUN(	do_otransfer	);
DECLARE_DO_FUN( do_champions	);
DECLARE_DO_FUN( do_paradox	);
DECLARE_DO_FUN(	do_password	);
DECLARE_DO_FUN(	do_peace	);
DECLARE_DO_FUN(	do_pick		);
DECLARE_DO_FUN( do_plasma       );
DECLARE_DO_FUN(	do_pload	);
DECLARE_DO_FUN(	do_punload	);
DECLARE_DO_FUN(	do_poison	);
DECLARE_DO_FUN( do_possession   );
DECLARE_DO_FUN(	do_practice	);
DECLARE_DO_FUN(	do_pray		);
DECLARE_DO_FUN(	do_press	);
DECLARE_DO_FUN(	do_preturn	);
DECLARE_DO_FUN(	do_prompt	);
DECLARE_DO_FUN(	do_propose	);
DECLARE_DO_FUN(	do_pset		);
DECLARE_DO_FUN(	do_pull		);
DECLARE_DO_FUN(	do_punch	);
DECLARE_DO_FUN(	do_purge	);
DECLARE_DO_FUN(	do_put		);
DECLARE_DO_FUN(	do_quaff	);
DECLARE_DO_FUN(	do_qset		);
DECLARE_DO_FUN(	do_qstat	);
DECLARE_DO_FUN(	do_qtrust	);
DECLARE_DO_FUN(	do_quest	);
DECLARE_DO_FUN(	do_newbie	);
DECLARE_DO_FUN(	do_qui		);
DECLARE_DO_FUN(	do_quit		);
DECLARE_DO_FUN(	do_rage		);
DECLARE_DO_FUN(	do_readaura	);
DECLARE_DO_FUN(	do_reboo	);
DECLARE_DO_FUN(	do_reboot	);
DECLARE_DO_FUN(	do_copyover	);
DECLARE_DO_FUN(	do_recall	);
DECLARE_DO_FUN(	do_recho	);
DECLARE_DO_FUN(	do_recite	);
DECLARE_DO_FUN(	do_regenerate	);
DECLARE_DO_FUN(	do_release	);
DECLARE_DO_FUN(	do_relevel	);
DECLARE_DO_FUN( do_nike		);
DECLARE_DO_FUN( do_gflags	);
DECLARE_DO_FUN(	do_reload	);
DECLARE_DO_FUN( do_hreload	);
DECLARE_DO_FUN(	do_remove	);
DECLARE_DO_FUN(	do_rent		);
DECLARE_DO_FUN(	do_reply	);
DECLARE_DO_FUN(	do_report	);
DECLARE_DO_FUN(	do_rescue	);
DECLARE_DO_FUN(	do_rest		);
DECLARE_DO_FUN(	do_restore	);
DECLARE_DO_FUN(	do_return	);
DECLARE_DO_FUN(	do_roll		);
DECLARE_DO_FUN(	do_rset		);
DECLARE_DO_FUN(	do_rstat	);
DECLARE_DO_FUN(	do_sacrifice	);
DECLARE_DO_FUN(	do_safe		);
DECLARE_DO_FUN(	do_save		);
DECLARE_DO_FUN(	do_say		);
DECLARE_DO_FUN(	do_scan		);
DECLARE_DO_FUN( do_stat         );
DECLARE_DO_FUN( do_spit         );
DECLARE_DO_FUN( do_tongue       );
DECLARE_DO_FUN( do_mindblast    );
DECLARE_DO_FUN( do_powers       );
DECLARE_DO_FUN(	do_score	);
DECLARE_DO_FUN( do_weaplist     );
DECLARE_DO_FUN( do_level	);
DECLARE_DO_FUN( do_mastery      );
DECLARE_DO_FUN( do_upgrade	);
DECLARE_DO_FUN( do_healme	);
DECLARE_DO_FUN( do_pkpowers	);
DECLARE_DO_FUN( do_gensteal     );
DECLARE_DO_FUN( do_setstance	);
DECLARE_DO_FUN( do_mudstat	);
DECLARE_DO_FUN(	do_scry		);
DECLARE_DO_FUN( do_pkscry	);
DECLARE_DO_FUN( do_fixexp	);
DECLARE_DO_FUN( do_pkaura	);
DECLARE_DO_FUN( do_pkportal	);
DECLARE_DO_FUN( do_serenity     );
DECLARE_DO_FUN(	do_serpent	);
DECLARE_DO_FUN(	do_shadowplane	);
DECLARE_DO_FUN(	do_shadowsight	);
DECLARE_DO_FUN( do_shadowstep	);
DECLARE_DO_FUN( do_shadowwalk	);
DECLARE_DO_FUN(	do_sheath	);
DECLARE_DO_FUN(	do_shield	);
DECLARE_DO_FUN(	do_shoot	);
DECLARE_DO_FUN(	do_shout	);
DECLARE_DO_FUN(	do_shutdow	);
DECLARE_DO_FUN(	do_shutdown	);
DECLARE_DO_FUN(	do_show_artifacts);
DECLARE_DO_FUN(	do_side		);
DECLARE_DO_FUN( do_learn	);
DECLARE_DO_FUN( do_hologramtransfer);
DECLARE_DO_FUN( do_stalk	);
DECLARE_DO_FUN( do_bladespin    );
DECLARE_DO_FUN( do_sign		);
DECLARE_DO_FUN(	do_silence	);
DECLARE_DO_FUN(	do_sit		);
DECLARE_DO_FUN(	do_skill	);
DECLARE_DO_FUN(	do_sla		);
DECLARE_DO_FUN(	do_slay		);
DECLARE_DO_FUN(	do_sleep	);
DECLARE_DO_FUN(	do_slookup	);
DECLARE_DO_FUN(	do_special	);
DECLARE_DO_FUN(	do_spell	);
DECLARE_DO_FUN( do_tendrils     );
DECLARE_DO_FUN( do_lamprey      );
DECLARE_DO_FUN(	do_stake	);
DECLARE_DO_FUN(	do_stance	);
DECLARE_DO_FUN(	do_smother	);
DECLARE_DO_FUN(	do_sneak	);
DECLARE_DO_FUN(	do_snoop	);
DECLARE_DO_FUN(	do_socials	);
DECLARE_DO_FUN(	do_south	);
DECLARE_DO_FUN(	do_split	);
DECLARE_DO_FUN(	do_spy		);
DECLARE_DO_FUN(	do_spydirection	);
DECLARE_DO_FUN(	do_sset		);
DECLARE_DO_FUN(	do_stand	);
DECLARE_DO_FUN(	do_steal	);
DECLARE_DO_FUN( do_summon	);
DECLARE_DO_FUN(	do_nosummon	);
DECLARE_DO_FUN( do_notravel	);
DECLARE_DO_FUN(	do_switch	);
DECLARE_DO_FUN( do_embrace      );
DECLARE_DO_FUN( do_diablerise   );
DECLARE_DO_FUN( do_entrance     );
DECLARE_DO_FUN( do_fleshcraft   );
DECLARE_DO_FUN( do_zombie       );
DECLARE_DO_FUN( do_quills);
DECLARE_DO_FUN(do_perception);
DECLARE_DO_FUN(do_jawlock);
DECLARE_DO_FUN(do_rend);
DECLARE_DO_FUN(do_hardskin);
DECLARE_DO_FUN(do_slam);
DECLARE_DO_FUN(do_roar);
DECLARE_DO_FUN(  do_shred);

DECLARE_DO_FUN(  do_congregation);
//DECLARE_DO_FUN( do_run          );
DECLARE_DO_FUN( do_talons       );
DECLARE_DO_FUN( do_devour       );
DECLARE_DO_FUN( do_inferno      );
DECLARE_DO_FUN( do_wall         );
DECLARE_DO_FUN( do_facade       );
DECLARE_DO_FUN( do_baal         );
DECLARE_DO_FUN( do_obj          );
DECLARE_DO_FUN( do_dragonform   );
DECLARE_DO_FUN( do_bloodwater   );
DECLARE_DO_FUN( do_gourge       );
DECLARE_DO_FUN( do_sharpen);
DECLARE_DO_FUN(do_dub);
DECLARE_DO_FUN(do_shroud);
DECLARE_DO_FUN( do_taste        );
DECLARE_DO_FUN( do_clone        );
DECLARE_DO_FUN( do_assassinate  );
DECLARE_DO_FUN( do_offer        );
DECLARE_DO_FUN( do_gate         );
DECLARE_DO_FUN( do_pigeon       );
DECLARE_DO_FUN( do_bloodagony   );
DECLARE_DO_FUN( do_decay        );
DECLARE_DO_FUN(	do_teach	);
DECLARE_DO_FUN(	do_tear		);
DECLARE_DO_FUN(	do_disassemble		);
DECLARE_DO_FUN(	do_tell		);
DECLARE_DO_FUN( do_theft        );
DECLARE_DO_FUN(	do_throw	);
DECLARE_DO_FUN(	do_tie		);
DECLARE_DO_FUN(	do_time		);
DECLARE_DO_FUN(	do_title	);
DECLARE_DO_FUN( do_kingrank        );
DECLARE_DO_FUN(	do_token	);
DECLARE_DO_FUN( do_tokeneat     );
DECLARE_DO_FUN( do_topxp	);
DECLARE_DO_FUN(	do_totems	);
DECLARE_DO_FUN(	do_track	);
DECLARE_DO_FUN(	do_tradition	);
DECLARE_DO_FUN(	do_train	);
DECLARE_DO_FUN(	do_transfer	);
DECLARE_DO_FUN(	do_transport	);
DECLARE_DO_FUN(	do_travel	);
DECLARE_DO_FUN(	do_ztravel	);
DECLARE_DO_FUN(	do_truesight	);
DECLARE_DO_FUN(	do_trust	);
DECLARE_DO_FUN(	do_turn		);
DECLARE_DO_FUN(	do_twist	);
DECLARE_DO_FUN(	do_typo		);
DECLARE_DO_FUN(	do_unload	);
DECLARE_DO_FUN(	do_unlock	);
DECLARE_DO_FUN(	do_unpolymorph	);
DECLARE_DO_FUN(	do_untie	);
DECLARE_DO_FUN(	do_unwerewolf	);
DECLARE_DO_FUN(	do_up		);
DECLARE_DO_FUN(	do_upkeep	);
DECLARE_DO_FUN(	do_users	);
DECLARE_DO_FUN(	do_vampire	);
DECLARE_DO_FUN(	do_vamptalk	);
DECLARE_DO_FUN( do_tail		);
DECLARE_DO_FUN(	do_hooves	);
DECLARE_DO_FUN(	do_magetalk	);
DECLARE_DO_FUN(	do_vanish	);
DECLARE_DO_FUN(	do_version	);
DECLARE_DO_FUN(	do_visible	);
DECLARE_DO_FUN( do_vlist	);
DECLARE_DO_FUN(	do_voodoo	);
DECLARE_DO_FUN(	do_web		);
DECLARE_DO_FUN(	do_wake		);
DECLARE_DO_FUN(	do_watcher	);
DECLARE_DO_FUN(	do_watching	);
DECLARE_DO_FUN(	do_weaponform	);
DECLARE_DO_FUN(	do_wear		);
DECLARE_DO_FUN(	do_wearaffect	);
DECLARE_DO_FUN(	do_werewolf	);
DECLARE_DO_FUN(	do_west		);
DECLARE_DO_FUN(	do_where	);
DECLARE_DO_FUN(	do_whisper	);
DECLARE_DO_FUN(	do_who		);
DECLARE_DO_FUN(	do_wimpy	);
DECLARE_DO_FUN(	do_wings	);
DECLARE_DO_FUN(	do_wizhelp	);
DECLARE_DO_FUN( do_linkdead	);
DECLARE_DO_FUN(	do_wizlist	);
DECLARE_DO_FUN( do_closemud	);
DECLARE_DO_FUN(	do_wizlock	);
DECLARE_DO_FUN(	do_xemote	);
DECLARE_DO_FUN(	do_xsocials	);
DECLARE_DO_FUN(	do_yell		);
DECLARE_DO_FUN(	do_zap		);
DECLARE_DO_FUN( do_zuloform     );
DECLARE_DO_FUN( do_demonform	);
DECLARE_DO_FUN( do_beckon       );
DECLARE_DO_FUN( do_illusion	);
DECLARE_DO_FUN( do_cserpent	);
DECLARE_DO_FUN( do_scales	);
DECLARE_DO_FUN( do_guardian     );
DECLARE_DO_FUN( do_servant      );
DECLARE_DO_FUN( do_contraception );
DECLARE_DO_FUN( do_relearn );
DECLARE_DO_FUN( do_hotboot      );

/*MECHA*/
DECLARE_DO_FUN( do_learn );
DECLARE_DO_FUN( do_mechatalk );

/*SAIYAJIN*/
DECLARE_DO_FUN( do_learn );
DECLARE_DO_FUN( do_scouttalk );
DECLARE_DO_FUN( do_travel );

/*DECLARE_DO_FUN( do_weapmod      );
*/
/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN(	spell_contraception	);
DECLARE_SPELL_FUN(      spell_spew              );
DECLARE_SPELL_FUN(	spell_infirmity		);
DECLARE_SPELL_FUN(	spell_null		);
DECLARE_SPELL_FUN(      spell_make_bag          );
DECLARE_SPELL_FUN(	spell_acid_blast	);

//DECLARE_SPELL_FUN(      spell_tendrils          );

DECLARE_SPELL_FUN(	spell_armor		);
DECLARE_SPELL_FUN(	spell_godbless		);
DECLARE_SPELL_FUN(	spell_bless		);
DECLARE_SPELL_FUN(	spell_blindness		);
DECLARE_SPELL_FUN(	spell_burning_hands	);
DECLARE_SPELL_FUN(	spell_call_lightning	);
DECLARE_SPELL_FUN(	spell_cause_critical	);
DECLARE_SPELL_FUN(	spell_cause_light	);
DECLARE_SPELL_FUN(	spell_cause_serious	);
DECLARE_SPELL_FUN(	spell_change_sex	);
DECLARE_SPELL_FUN(	spell_charm_person	);
DECLARE_SPELL_FUN(	spell_chill_touch	);
DECLARE_SPELL_FUN(	spell_colour_spray	);
DECLARE_SPELL_FUN(	spell_continual_light	);
DECLARE_SPELL_FUN(	spell_control_weather	);
DECLARE_SPELL_FUN(	spell_create_food	);
DECLARE_SPELL_FUN(	spell_create_spring	);
DECLARE_SPELL_FUN(	spell_create_water	);
DECLARE_SPELL_FUN(	spell_cure_blindness	);
DECLARE_SPELL_FUN(	spell_cure_critical	);
DECLARE_SPELL_FUN(	spell_cure_light	);
DECLARE_SPELL_FUN(	spell_cure_poison	);
DECLARE_SPELL_FUN(	spell_cure_serious	);
DECLARE_SPELL_FUN(	spell_curse		);
DECLARE_SPELL_FUN(	spell_darkness		);
DECLARE_SPELL_FUN(	spell_detect_evil	);
DECLARE_SPELL_FUN(	spell_detect_hidden	);
DECLARE_SPELL_FUN(	spell_detect_invis	);
DECLARE_SPELL_FUN(	spell_detect_magic	);
DECLARE_SPELL_FUN(	spell_detect_poison	);
DECLARE_SPELL_FUN(	spell_dispel_evil	);
DECLARE_SPELL_FUN(	spell_dispel_magic	);
DECLARE_SPELL_FUN(	spell_drowfire	);
DECLARE_SPELL_FUN(	spell_earthquake	);
DECLARE_SPELL_FUN(	spell_enchant_weapon	);
DECLARE_SPELL_FUN(	spell_enchant_armor	);
DECLARE_SPELL_FUN(	spell_energy_drain	);
DECLARE_SPELL_FUN(	spell_faerie_fire	);
DECLARE_SPELL_FUN(	spell_faerie_fog	);
DECLARE_SPELL_FUN(	spell_fireball		);
DECLARE_SPELL_FUN(	spell_desanct		);
DECLARE_SPELL_FUN(      spell_imp_heal		);
DECLARE_SPELL_FUN(      spell_imp_fireball	);
DECLARE_SPELL_FUN(      spell_imp_faerie_fire	);
DECLARE_SPELL_FUN(      spell_imp_teleport	);
DECLARE_SPELL_FUN(	spell_flamestrike	);
DECLARE_SPELL_FUN(	spell_fly		);
DECLARE_SPELL_FUN(	spell_gate		);
DECLARE_SPELL_FUN(	spell_general_purpose	);
DECLARE_SPELL_FUN(	spell_giant_strength	);
DECLARE_SPELL_FUN(	spell_harm		);
DECLARE_SPELL_FUN(	spell_heal		);
DECLARE_SPELL_FUN(	spell_group_heal	);
DECLARE_SPELL_FUN(	spell_high_explosive	);
DECLARE_SPELL_FUN(	spell_identify		);
DECLARE_SPELL_FUN(	spell_readaura		);
DECLARE_SPELL_FUN(	spell_infravision	);
DECLARE_SPELL_FUN(	spell_invis		);
DECLARE_SPELL_FUN(	spell_know_alignment	);
DECLARE_SPELL_FUN(	spell_lightning_bolt	);
DECLARE_SPELL_FUN(	spell_locate_object	);
DECLARE_SPELL_FUN(	spell_magic_missile	);
DECLARE_SPELL_FUN(	spell_mass_invis	);
DECLARE_SPELL_FUN(	spell_pass_door		);
DECLARE_SPELL_FUN(	spell_poison		);
DECLARE_SPELL_FUN(	spell_protection	);
DECLARE_SPELL_FUN(	spell_protection_vs_good);
DECLARE_SPELL_FUN(	spell_refresh		);
DECLARE_SPELL_FUN(	spell_remove_curse	);
DECLARE_SPELL_FUN(	spell_sanctuary		);
DECLARE_SPELL_FUN(	spell_shocking_grasp	);
DECLARE_SPELL_FUN(	spell_shield		);
DECLARE_SPELL_FUN(	spell_sleep		);
DECLARE_SPELL_FUN(	spell_stone_skin	);
DECLARE_SPELL_FUN(	spell_summon		);
DECLARE_SPELL_FUN(	spell_teleport		);
DECLARE_SPELL_FUN(	spell_ventriloquate	);
DECLARE_SPELL_FUN(	spell_weaken		);
DECLARE_SPELL_FUN(	spell_word_of_recall	);
DECLARE_SPELL_FUN(	spell_acid_breath	);
DECLARE_SPELL_FUN(	spell_prismatic_beam    );
DECLARE_SPELL_FUN( 	spell_super_gas		);
DECLARE_SPELL_FUN(	spell_fire_breath	);
DECLARE_SPELL_FUN(	spell_frost_breath	);
DECLARE_SPELL_FUN(	spell_gas_breath	);
DECLARE_SPELL_FUN(	spell_godbless		);
DECLARE_SPELL_FUN(	spell_lightning_breath	);
DECLARE_SPELL_FUN(	spell_cone		);
DECLARE_SPELL_FUN(	spell_guardian		);
DECLARE_SPELL_FUN(	spell_soulblade		);
DECLARE_SPELL_FUN(	spell_mana		);
DECLARE_SPELL_FUN(	spell_frenzy		);
DECLARE_SPELL_FUN(	spell_darkblessing	);
DECLARE_SPELL_FUN(	spell_portal		);
DECLARE_SPELL_FUN(	spell_energyflux	);
DECLARE_SPELL_FUN(	spell_voodoo		);
DECLARE_SPELL_FUN(	spell_transport		);
DECLARE_SPELL_FUN(	spell_regenerate	);
DECLARE_SPELL_FUN(	spell_clot		);
DECLARE_SPELL_FUN(	spell_mend		);
DECLARE_SPELL_FUN(	spell_quest		);
DECLARE_SPELL_FUN(	spell_minor_creation	);
DECLARE_SPELL_FUN(	spell_spiritkiss	);
DECLARE_SPELL_FUN(	spell_brew		);
DECLARE_SPELL_FUN( 	spell_jailwater		);
DECLARE_SPELL_FUN(	spell_scribe		);
DECLARE_SPELL_FUN(	spell_carve		);
DECLARE_SPELL_FUN(	spell_engrave		);
DECLARE_SPELL_FUN(	spell_bake		);
DECLARE_SPELL_FUN(	spell_mount		);
DECLARE_SPELL_FUN(	spell_scan		);
DECLARE_SPELL_FUN(	spell_repair		);
DECLARE_SPELL_FUN(	spell_spellproof	);
DECLARE_SPELL_FUN(	spell_preserve		);
DECLARE_SPELL_FUN(	spell_major_creation	);
DECLARE_SPELL_FUN(	spell_copy		);
DECLARE_SPELL_FUN(	spell_insert_page	);
DECLARE_SPELL_FUN(	spell_chaos_blast	);
DECLARE_SPELL_FUN(	spell_resistance	);
DECLARE_SPELL_FUN(	spell_web		);
DECLARE_SPELL_FUN(	spell_polymorph		);
DECLARE_SPELL_FUN(	spell_contraception	);
DECLARE_SPELL_FUN(	spell_find_familiar	);
DECLARE_SPELL_FUN(	spell_improve		);
DECLARE_SPELL_FUN(      spell_clay              );
DECLARE_SPELL_FUN(	spell_super_blind	);
DECLARE_SPELL_FUN(	spell_miracle		);
/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if	defined(_AIX)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(apollo)
int	atoi		args( ( const char *string ) );
void *	calloc		args( ( unsigned nelem, size_t size ) );
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(hpux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(interactive)
#endif

#if	defined(linux)
char *	crypt		args( ( const char *key, const char *salt
) );
#endif

#if	defined(macintosh) || defined(WIN32)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(MIPS_OS)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(MSDOS)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(NeXT)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(sequent)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(sun)
char *	crypt		args( ( const char *key, const char *salt
) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
#if 	defined(SYSV)
size_t 	fread		args( ( void *ptr, size_t size, size_t n, 
				FILE *stream ) );
#else
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
#endif
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(ultrix)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif



/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	defined(NOCRYPT)
#define crypt(s1, s2)	(s1)
#endif



/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */
#if defined(macintosh)

#define PLAYER_DIR	""		/* Player files			*/
#define NULL_FILE	"proto.are"	/* To reserve one stream	*/
#define BACKUP_DIR      "" /*reimburse shit -Infidel*/
#endif

#if defined(MSDOS)
#define PLAYER_DIR	""		/* Player files                 */
#define NULL_FILE	"nul"		/* To reserve one stream	*/
#endif

#if defined( WIN32 )
#define PLAYER_DIR       "..\\player\\"	/* Player files                 */
#define NULL_FILE        "nul"		/* To reserve one stream        */
#define BACKUP_DIR	"..\\backup\\"
#endif

#if defined(unix)
#define PLAYER_DIR	"../player/"	/* Player files			*/
#define BACKUP_DIR      "../player/backup/" /*reimb shit*/

#define NULL_FILE	"/dev/null"	/* To reserve one stream	*/
#endif

#define AREA_LIST	"area.lst"	/* List of areas		*/
#define CLAN_LIST	"../area/clans/clan.lst"	/* List of clans		*/
#define ART_LIST	"art1.txt"	/* List of artifacts            */
#define HOME_AREA	"../area/homes.are" /* Mages towers, etc        */
#define BAN_LIST	"../area/ban.txt"	/* baaan. */
#define BUG_FILE	"bugs.txt"      /* For 'bug' and bug( )		*/
#define IDEA_FILE	"ideas.txt"	/* For 'idea'			*/
#define TYPO_FILE	"typos.txt"     /* For 'typo'			*/
#define NOTE_FILE	"notes.txt"	/* For 'notes'			*/
#define SHUTDOWN_FILE	"shutdown.txt"	/* For 'shutdown'		*/
#define QUEST_TXT	"quest.txt"     /* Show quests hehe whee	*/
#define KINGDOM_FILE    "../area/kingshit.txt"  /* New database - TEST */
#define LOG_FILE	"../db/logs.db"  /* logged commands		*/
#define TODO_LIST       "../area/todo.txt"            /* Todo list                            */
#define QUOTE_LIST       "../area/quote.txt"            /* Quote list                            */
#define SWEARLIST_FILE  "../area/swearlist.txt"       /* Swearlist                            */
#define MULTILIST_FILE  "../area/multilist.txt"       /* Swearlist                            */
#define RANDOM_FILE     "../db/random.dat"             /* Random Loading Items                     */
#define HINT_FILE	"../area/hints.txt"	      /* Hint list */
/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN
#define ED	EXIT_DATA

/* act_comm.c */
void	add_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void	stop_follower	args( ( CHAR_DATA *ch ) );
void	die_follower	args( ( CHAR_DATA *ch ) );
bool	is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
void	room_text	args( ( CHAR_DATA *ch, char *argument ) );
char    *strlower       args( ( char * ip ) );
void	excessive_cpu	args( ( int blx ) );
bool	check_parse_name args( ( char *name ) );

void    room_message    args( ( ROOM_INDEX_DATA *room, char *message ) );

/* act_info.c */
void	set_title	args( ( CHAR_DATA *ch, char *title ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
int	char_hitroll	args( ( CHAR_DATA *ch ) );
int	char_damroll	args( ( CHAR_DATA *ch ) );
int	char_ac		args( ( CHAR_DATA *ch ) );

/* darkheart.c */

MID   * get_random_mob	args( ( ) );
bool    is_demonic_form  args( ( int form ) );
char  * dragon_colour   args( (sh_int dragtype) );
char  * dragon_colour2   args( (sh_int dragtype) );
int     get_ch_size	args( (CHAR_DATA *ch) );
void    set_enter_room_text args( ( CHAR_DATA *ch, char *argument ) );
void    set_form        args( ( CHAR_DATA *ch , sh_int form ) );
void    look_name	args( ( CHAR_DATA *ch ) );
void    set_pc_name     args( ( CHAR_DATA *ch, char *title ) );
void    set_switchname  args( ( CHAR_DATA *ch, char *title ) );
char *  get_who_race    args( ( CHAR_DATA *ch, CHAR_DATA *wch) );
bool    is_clan		args( ( CHAR_DATA *ch ) );
void    form_in_room_desc args( ( CHAR_DATA *ch, char *buf ) );
bool    is_vowel	args( ( char arg ) );
int     get_normal_form   args( (CHAR_DATA *ch) );

/* act_move.c */
void	move_char	args( ( CHAR_DATA *ch, int door ) );
void	open_lift	args( ( CHAR_DATA *ch ) );
void	close_lift	args( ( CHAR_DATA *ch ) );
void	move_lift	args( ( CHAR_DATA *ch, int to_room ) );
void	move_door	args( ( CHAR_DATA *ch ) );
void	thru_door	args( ( CHAR_DATA *ch, int doorexit ) );
void	open_door	args( ( CHAR_DATA *ch, bool be_open ) );
bool	is_open		args( ( CHAR_DATA *ch ) );
bool	same_floor	args( ( CHAR_DATA *ch, int cmp_room ) );
void	check_hunt	args( ( CHAR_DATA *ch ) );

int   disc_points_needed args( (CHAR_DATA *ch) );
void  gain_disc_points  args( (CHAR_DATA *ch, int points ) );
RID  *get_random_room   args ( (CHAR_DATA *ch) );
RID  *get_rand_room   args ( ( ) );
RID  *get_treemeld_room   args ( ( ) );
RID  *get_rand_room_by_sect   args ( (int sect ) );


/* act_obj.c */
bool	is_ok_to_wear	args( ( CHAR_DATA *ch, bool wolf_ok, char *argument ) );
void	quest_object	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	remove_obj	args( ( CHAR_DATA *ch, int iWear, bool fReplace ) );
void	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );

/* act_wiz.c */
void   	get_reboot_string args( ( void ) );
void	bind_char	args( ( CHAR_DATA *ch ) );
void	logchan		args( ( char *argument ) );


/* build.c */
ED *	get_exit	args( ( ROOM_INDEX_DATA *room, sh_int dir ) );
ED *	get_exit_to	args( ( ROOM_INDEX_DATA *room, sh_int dir,int vnum ) );
ED *	get_exit_num	args( ( ROOM_INDEX_DATA *room, sh_int count ) );

char *	copy_buffer	args( ( CHAR_DATA *ch ) );
void	edit_buffer	args( ( CHAR_DATA *ch, char *argument ) );
char *	strip_cr	args( ( char *str  ) );
void    start_editing   args( ( CHAR_DATA *ch, char *data ) );  
void    stop_editing    args( ( CHAR_DATA *ch ) );
int 	social_lookup 	args( ( const char *name) );
/* comm.c */
void	close_socket	args( ( DESCRIPTOR_DATA *dclose ) );
void	close_socket2	args( ( DESCRIPTOR_DATA *dclose, bool kickoff ) );
void	write_to_buffer	args( ( DESCRIPTOR_DATA *d, const char *txt,
			    int length ) );
void	send_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
int     send_to_world   args( ( char *argument, bool showprompt ) );
void	act		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type, bool brief ) );
void	act2		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	kavitem		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void    banner_to_char  args( ( char *txt, CHAR_DATA *ch ) );
void    banner2_to_char args( ( char *txt, CHAR_DATA *ch ) );
void    divide_to_char  args( ( CHAR_DATA *ch ) );
void    divide2_to_char args( ( CHAR_DATA *ch ) );
void    divide3_to_char args( ( CHAR_DATA *ch ) );
void    divide4_to_char args( ( CHAR_DATA *ch ) );
void    divide5_to_char args( ( CHAR_DATA *ch ) );
void    divide6_to_char args( ( CHAR_DATA *ch ) );
void    stc	        args( ( const char *txt, CHAR_DATA *ch ) );
void    cent_to_char    args( ( char *txt, CHAR_DATA *ch ) );

#define DISABLED_FILE "disabled.txt"  /* disabled commands */

/* prototypes from db.c */
void  load_disabled   args( ( void ) );
void  save_disabled   args( ( void ) );
#define GET_OBJ_WEIGHT(obj)     ((obj)->weight)

/* db.c */
void	boot_db		args( (bool fCopyOver ) );
void	area_update	args( ( bool ForceReset ) );
CD *	create_mobile	args( ( MOB_INDEX_DATA *pMobIndex ) );
OD *	create_object	args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
void	clear_char	args( ( CHAR_DATA *ch ) );
void	free_char	args( ( CHAR_DATA *ch ) );
char *	get_extra_descr	args( ( char *name, EXTRA_DESCR_DATA *ed ) );
char *	get_roomtext	args( ( const char *name, ROOMTEXT_DATA    *rt ) );
MID *	get_mob_index	args( ( int vnum ) );
OID *	get_obj_index	args( ( int vnum ) );
RID *	get_room_index	args( ( int vnum ) );
char	fread_letter	args( ( FILE *fp ) );
int	fread_number	args( ( FILE *fp ) );
char *	fread_string	args( ( FILE *fp ) );
void	fread_to_eol	args( ( FILE *fp ) );
char *	fread_word	args( ( FILE *fp ) );
void *	alloc_mem	args( ( int sMem ) );
void *	alloc_perm	args( ( int sMem ) );
void	free_mem	args( ( void *pMem, int sMem ) );
char *	str_dup		args( ( const char *str ) );
void	free_string	args( ( char *pstr ) );
int	number_fuzzy	args( ( int number ) );
int	number_range	args( ( int from, int to ) );
int	number_percent	args( ( void ) );
int	number_door	args( ( void ) );
int	number_bits	args( ( int width ) );
int	number_mm	args( ( void ) );
int	dice		args( ( int number, int size ) );
int	interpolate	args( ( int level, int value_00, int value_32 ) );
void	smash_tilde	args( ( char *str ) );
bool	str_cmp		args( ( const char *astr, const char *bstr ) );
bool	str_prefix	args( ( const char *astr, const char *bstr ) );
bool	str_infix	args( ( const char *astr, const char *bstr ) );
bool	str_suffix	args( ( const char *astr, const char *bstr ) );
char *	capitalize	args( ( const char *str ) );
void	append_file	args( ( CHAR_DATA *ch, char *file, char *str ) );
//void	bug		args( ( const char *str, int param ) );
void     bug             args( ( const char *str, ... ) );
//void	log_string	args( ( const char *str ) );
void     log_string      args( ( const char *cpcFormat, ...) );
void	tail_chain	args( ( void ) );

void	add_help	args( ( HELP_DATA *pHelp ) );
char *	strupper	args( ( const char *str ) );
/*
RID *	make_room	args( ( int vnum ) );
OID *	make_object	args( ( int vnum, int cvnum, char *name ) );
MID *	make_mobile	args( ( int vnum, int cvnum, char *name ) );
ED  *	make_exit	args( ( ROOM_INDEX_DATA *pRoomIndex, ROOM_INDEX_DATA *to_room, int door ) );
*/


int   strlen2                   args (( const char *b ));


/* daemon.c */

ROOM_INDEX_DATA * locate_obj	args ( ( OBJ_DATA *obj ) );
void shock_effect  args( ( void *vo,int level, int dam, int target ) );
void cold_effect   args( ( void *vo,int level, int dam, int target ) );
void acid_effect   args( ( void *vo,int level, int dam, int target ) );
void fire_effect   args( ( void *vo,int level, int dam, int target ) );
void make_wall     args( ( ROOM_INDEX_DATA *room, int dir, int wall ) );

/* fight.c */

int  cap_dam		args( ( CD *ch, CD *victim, int dam));
int  randomize_damage	args( ( CHAR_DATA *ch, int dam, int am ) );

bool    check_gang	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	violence_update	args( ( void ) );
void	multi_hit	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	damage		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ) );
void	update_pos	args( ( CHAR_DATA *victim ) );
void	stop_fighting	args( ( CHAR_DATA *ch, bool fBoth ) );
void    stop_embrace    args( ( CHAR_DATA *ch, CHAR_DATA *victim));
bool	is_safe		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	is_range	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	hurt_person	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );
void    set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim));
bool    has_timer	args( ( CHAR_DATA *ch ) );
bool    has_bad_chars   args( ( CHAR_DATA *ch, char *argument ) );
void    check_leaderboard  args( ( CHAR_DATA *ch, char *argument ) );

/* handler.c */

void	extract_exit	args( ( ROOM_INDEX_DATA *room, EXIT_DATA *pexit ) );

int	get_trust	args( ( CHAR_DATA *ch ) );
int	get_age		args( ( CHAR_DATA *ch ) );
int	get_curr_str	args( ( CHAR_DATA *ch ) );
int	get_curr_int	args( ( CHAR_DATA *ch ) );
int	get_curr_wis	args( ( CHAR_DATA *ch ) );
int	get_curr_dex	args( ( CHAR_DATA *ch ) );
int	get_curr_con	args( ( CHAR_DATA *ch ) );
int	can_carry_n	args( ( CHAR_DATA *ch ) );
int	can_carry_w	args( ( CHAR_DATA *ch ) );
bool	is_name		args( ( char *str, char *namelist ) );
bool	is_full_name	args( ( const char *str, char *namelist ) );
void	affect_to_char	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_remove	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_strip	args( ( CHAR_DATA *ch, int sn ) );
bool	is_affected	args( ( CHAR_DATA *ch, int sn ) );
void	affect_join	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	char_from_room	args( ( CHAR_DATA *ch ) );
void	char_to_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void    repop_char_to_room    args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void	obj_from_char	args( ( OBJ_DATA *obj ) );
int	apply_ac	args( ( OBJ_DATA *obj, int iWear ) );
OD *	get_eq_char	args( ( CHAR_DATA *ch, int iWear ) );
void	equip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void	unequip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
int	count_obj_list	args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
void	obj_from_room	args( ( OBJ_DATA *obj ) );
void	obj_to_room	args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_obj	args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void	obj_from_obj	args( ( OBJ_DATA *obj ) );
void	extract_obj	args( ( OBJ_DATA *obj ) );
void	extract_char	args( ( CHAR_DATA *ch, bool fPull ) );
CD *	get_char_room	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world2	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_type	args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *	get_obj_list	args( ( CHAR_DATA *ch, char *argument,
			    OBJ_DATA *list ) );
OD *	get_obj_in_obj	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_carry	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_wear	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_here	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_room	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world2	args( ( CHAR_DATA *ch, char *argument ) );
OD *	create_money	args( ( int amount ) );
int	get_obj_number	args( ( OBJ_DATA *obj ) );
int	get_obj_weight	args( ( OBJ_DATA *obj ) );
bool	room_is_dark	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	room_is_private	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool    room_is_noft   args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	can_see		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_see_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	can_drop_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
char *	item_type_name	args( ( OBJ_DATA *obj ) );
char *	affect_loc_name	args( ( int location ) );
char *	affect_bit_name	args( ( int vector ) );
char *	extra_bit_name	args( ( int extra_flags ) );
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd, OBJ_DATA *obj ) );
void    affect_to_obj   args( ( OBJ_DATA *obj, AFFECT_DATA *paf) );

void    set_learnable_disciplines args( ( CHAR_DATA *ch ) );
void    update_disc	args( ( CHAR_DATA *ch) );

/* interp.c */
void	interpret	args( ( CHAR_DATA *ch, char *argument ) );
bool	is_number	args( ( char *arg ) );
int	number_argument	args( ( char *argument, char *arg ) );
char *	one_argument	args( ( char *argument, char *arg_first ) );
void	stage_update	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int stage, char *argument ) );
void    make_preg       args( ( CHAR_DATA *mother, CHAR_DATA *father ) );

/* magic.c */
int	skill_lookup	args( ( const char *name ) );
int	slot_lookup	args( ( int slot ) );
bool	saves_spell	args( ( int level, CHAR_DATA *victim ) );
void	obj_cast_spell	args( ( int sn, int level, CHAR_DATA *ch,
				    CHAR_DATA *victim, OBJ_DATA *obj ) );
void	enhance_stat	args( ( int sn, int level, CHAR_DATA *ch, 
				    CHAR_DATA *victim, int apply_bit,
				    int bonuses, int affect_bit ) );

/* stat_sav.c */

void    initialise_config	args( ( void ) );

/* save.c */
void	save_char_obj		args( ( CHAR_DATA *ch ) );
void	save_char_obj_backup	args( ( CHAR_DATA *ch ) );
bool	load_char_obj		args( ( DESCRIPTOR_DATA *d, char *name ) );
bool	load_char_short		args( ( DESCRIPTOR_DATA *d, char *name ) );

/* special.c */
SF *	spec_lookup	args( ( const char *name ) );

/* update.c */
void	gain_exp	args( ( CHAR_DATA *ch, int gain ) );
void	gain_condition	args( ( CHAR_DATA *ch, int iCond, int value ) );
void	update_handler	args( ( void ) );
void    mobile_update   args( ( void ) );
void    weather_update  args( ( void ) );
void    char_update     args( ( void ) );
void    obj_update      args( ( void ) );
void    aggr_update     args( ( void ) );
void    ww_update       args( ( void ) );

void    room_update	args( ( void ) );

/* kav_fight.c */
void	special_move	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/* kav_info.c */
void	birth_date	args( ( CHAR_DATA *ch, bool is_self ) );
void	other_age	args( ( CHAR_DATA *ch, EXT_BV extra, bool is_preg, 
				char *argument ) );
int	years_old	args( ( CHAR_DATA *ch ) );

/* kav_wiz.c */
void oset_affect args( ( CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest) );
void rand_oset_affect args( ( CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest) );
/* clan.c */
void	reg_mend	args( ( CHAR_DATA *ch ) );
void	vamp_rage	args( ( CHAR_DATA *ch ) );
bool	char_exists	args( ( bool backup, char *argument ) );
OD *	get_page	args( ( OBJ_DATA *book, int page_num ) );

/* vic.c */
DECLARE_DO_FUN( do_relevel2 );
void reset_weapon   args( (CHAR_DATA *ch, int dtype) );
void reset_spell    args( (CHAR_DATA *ch, int dtype) );

#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF
#undef  ED

/*****************************************************************************
 *                                    OLC                                    *
 *****************************************************************************/

/*
 * This structure is used in special.c to lookup spec funcs and
 * also in olc_act.c to display listings of spec funcs.
 */
struct spec_type
{
    char *	spec_name;
    SPEC_FUN *	spec_fun;
};



struct clan_cmds
{
    char * name;
    int bit;
};

struct jope_type
{
    char * name;
    DO_FUN * do_fun;
    int level;
};

/*
 * Object defined in limbo.are
 * Used in save.c to load objects that don't exist.
 */
#define OBJ_VNUM_DUMMY	1



/*
 * Area flags.
 */
#define         AREA_NONE       0
#define         AREA_CHANGED    1	/* Area has been modified. */
#define         AREA_ADDED      2	/* Area has been added to. */
#define         AREA_LOADING    4	/* Used for counting in db.c */
#define 	AREA_VERBOSE	8
#define		AREA_CLAN	16      /* Area is part of a clanfile */

#define MAX_DIR	6
#define NO_FLAG -99	/* Must not be used in flags or stats. */



/*
 * Interp.c
 */
DECLARE_DO_FUN( do_aedit        );	/* OLC 1.1b */
DECLARE_DO_FUN( do_redit        );	/* OLC 1.1b */
DECLARE_DO_FUN( do_oedit        );	/* OLC 1.1b */
DECLARE_DO_FUN( do_medit        );	/* OLC 1.1b */
DECLARE_DO_FUN( do_asave	);
DECLARE_DO_FUN( do_alist	);
DECLARE_DO_FUN( do_resets	);


/*
 * Global Constants
 */
extern	char *	const	dir_name        [];
extern  char *  const   cdir_name	[];
extern	const	sh_int	rev_dir         [];
extern	const	struct	spec_type	spec_table	[];



/*
 * Global variables
 */

extern		int			nAllocPerm;
extern		int			nAllocString;

//extern          AREA_DATA *             area_first;
//extern          AREA_DATA *             area_last;
extern          AREA_DATA *             first_area;
extern          AREA_DATA *             last_area;
extern  	SHOP_DATA *             shop_last;

extern          int                     top_affect;
extern          int                     top_area;
extern          int                     top_ed;
extern          int                     top_exit;
extern          int                     top_help;
extern          int                     top_mob_index;
extern          int                     top_obj_index;
extern          int                     top_reset;
extern          int                     top_room;
extern          int                     top_shop;

extern          int                     top_vnum_mob;
extern          int                     top_vnum_obj;
extern          int                     top_vnum_room;

extern          char                    str_empty       [1];

extern  MOB_INDEX_DATA *        mob_index_hash  [MAX_KEY_HASH];
extern  OBJ_INDEX_DATA *        obj_index_hash  [MAX_KEY_HASH];
extern  ROOM_INDEX_DATA *       room_index_hash [MAX_KEY_HASH];

//extern          RANDOM_DATA     *       first_rand;
//extern          RANDOM_DATA     *       last_rand;


/* db.c */
void	reset_area      args( ( AREA_DATA * pArea ) );
void	reset_room	args( ( ROOM_INDEX_DATA *pRoom ) );

CLASS_LEADER class_leader;
LEADER_BOARD leader_board;
KINGDOM_DATA king_table[MAX_KINGDOM];
KINGDOM_DESC king_desc_list[MAX_KINGDOM];
WAR_DATA war_table[MAX_WAR];

/* string.c */
void	string_edit	args( ( CHAR_DATA *ch, char **pString ) );
void    string_append   args( ( CHAR_DATA *ch, char **pString ) );
char *	string_replace	args( ( char * orig, char * old, char * new ) );
void    string_add      args( ( CHAR_DATA *ch, char *argument ) );
char *  format_string   args( ( char *oldstring /*, bool fSpace */ ) );
char *  first_arg       args( ( char *argument, char *arg_first, bool fCase ) );
char *	string_unpad	args( ( char * argument ) );
char *	string_proper	args( ( char * argument ) );
char * all_capitalize	args( ( const char *str) );

/* olc.c */
bool	run_olc_editor	args( ( DESCRIPTOR_DATA *d ) );
char	*olc_ed_name	args( ( CHAR_DATA *ch ) );
char	*olc_ed_vnum	args( ( CHAR_DATA *ch ) );

/* special.c */
char *	spec_string	args( ( SPEC_FUN *fun ) );	/* OLC */


/* bit.c */
extern const struct flag_type 	area_flags[];
extern const struct flag_type	sex_flags[];
extern const struct flag_type	exit_flags[];
extern const struct flag_type	door_resets[];
extern const struct flag_type	room_flags[];
extern const struct flag_type	sector_flags[];
extern const struct flag_type	type_flags[];
extern const struct flag_type	extra_flags[];
extern const struct flag_type	wear_flags[];
extern const struct flag_type	act_flags[];
extern const struct flag_type	affect_flags[];
extern const struct flag_type	apply_flags[];
extern const struct flag_type	wear_loc_strings[];
extern const struct flag_type	wear_loc_flags[];
extern const struct flag_type	weapon_flags[];
extern const struct flag_type	container_flags[];
extern const struct flag_type	liquid_flags[];
extern const struct flag_type	class_flags[];
extern const struct flag_type	legend_flags[];
extern const struct flag_type	position_flags[];
extern char *const save_flag[];
int get_saveflag( char *name );

/*
 * Bits for dragons, their 25 skills
 */
#define DRA_WINGS			1
#define DRA_FANGS			2
#define DRA_LICK			4
#define DRA_FROST_BREATH		8
#define DRA_FIRE_BREATH			16
#define DRA_LIGHTNING_BREATH		32
#define DRA_ACID_BREATH			64
#define DRA_GAS_BREATH			128
#define DRA_TAIL			256
#define DRA_TERROR			512
#define DRA_CONFUSE			1024
#define DRA_MASK			2048
#define DRA_VENOM			4096
#define DRA_MAGIC			8192
#define DRA_POLYMORPH			16384
#define DRA_LUCK			32768
#define DRA_FLY				65536
#define DRA_SCRY			131072
#define DRA_PEER			262144
#define DRA_TRUESIGHT			524288
#define DRA_TOUGHNESS			1048576	

/*
 * Necromancer stuff
 */
#define NEC_SIGHT			1
#define NEC_STRENGTH			2
#define NEC_RITUAL			4

/* Jasons stuff in here */

/* Structure definitions must come before extern declarations */
struct swear_list {
    char word[32];
};

struct quote_list {
    char quote[MAX_STRING_LENGTH];
    char author[100];
};

struct multiplay_list {
    char word[32];
};

struct log_list {
    char cmd[32];
};

extern	CHAR_DATA *supersnard;
extern struct swear_list swearlist[999];
extern struct quote_list quotelist[999];
extern struct multiplay_list multiplaylist[999];
void count_item( CHAR_DATA *ch, OBJ_DATA *obj );
void quote( CHAR_DATA *ch );
bool check_speedwalk( CHAR_DATA *ch, char *argument);
void retake_inventory( CHAR_DATA *ch );
void do_sochelp( CHAR_DATA *ch, char *argument );
void init_supersnard();
void load_quote();
void read_multilist();
void add_contents( OBJ_DATA *obj, CHAR_DATA *ch );
void subtract_contents( OBJ_DATA *obj, CHAR_DATA *ch );
char *short_date( time_t time );
void save_hint (const struct hint_type *h, FILE *fp);
void save_hint_table();
void load_hint_table ();
void load_clanlist ();
CLAN_DATA *get_plr_clan( char *name );
CLAN_MEMBER *get_member( char *name, CLAN_DATA *pClan );


extern struct log_list loglist[999];

extern int num_of_logs;

/* End Jason */

/* NOTE: This file only includes two examples! write your own! */

struct slay_type
{
  char *        owner;		/* only this player can use this option */
  char *        title;		/* each one must have a unique title!   */
  char *        char_msg;	/* is sent to the "slayer"		*/
  char *        vict_msg;	/* is sent to the "slayee" (poor sucker)*/
  char *        room_msg;	/* is sent to everyone else in the room */
};


void 	flog 			args((char * fmt, ...));

void copyover_recover args((void));

DECLARE_DO_FUN( do_implag	);
DECLARE_DO_FUN( do_doublexp	);

#include "changes.h"
void load_social_table();
void save_social_table();

extern int maxSocial; /* max number of socials */
extern int maxHint; /* max number of hints */

#define BAN_FILE        "ban.txt"             /* For bans.                            */
#define BAN_PERMANENT   0
#define BAN_PREFIX      1
#define BAN_SUFFIX      2
#define BAN_NEWBIES     4
#define BAN_PERMIT      8
#define BAN_ALL         16

double fread_double( FILE *fp );

void do_crashguard();
void fread_sysdata( SYSTEM_DATA *sys, FILE *fp );

/*
 *
 * Legend Status, Ported by Rand
 */
#define L_UNK   0
#define L_CIT   1
#define L_LEG   2
#define L_MYT   3
#define L_TIT   4
#define L_ORA   5
#define L_IMM   6
#define L_DEM   7
#define L_DEI   8
#define L_ALM   9

int randMight args((CHAR_DATA *ch));
/*
 *  Fixed Snard's Implicit Declarations
 */
HELP_DATA *get_help( CHAR_DATA *ch, char *argument );
bool     process_output          args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
char *colour_strip(char const *str);
int check_swearlist(CHAR_DATA *ch, char *argument);
int check_multilist(CHAR_DATA *ch, CHAR_DATA *gch);
bool is_fighting_player ( CHAR_DATA *victim, CHAR_DATA *attacker );
void  dropinvis       args((CHAR_DATA *ch, CHAR_DATA *victim));
void printf_to_char   args((CHAR_DATA *ch, char *fmt, ...));
void save_sysdata     args(( SYSTEM_DATA sys ));
char *string_flag( int bitvector, char * const flagarray[] );
void load_jason       args(());
bool load_systemdata( SYSTEM_DATA *sys );
bool multicheck(CHAR_DATA *ch);
void sort_commands    args((void));
void read_swearlist   args((void));
void load_todo        args((void));
bool is_contained( const char *astr, const char *bstr );
bool greeting args((void));
EXT_BV meb( int bit );
EXT_BV fread_bitvector( FILE *fp );
char *print_bitvector( EXT_BV *bits );
void ext_remove_bits( EXT_BV *var, EXT_BV *bits );
char *ext_flag_string( EXT_BV *bitvector, char * const flagarray[] );
void ext_clear_bits( EXT_BV *bits );

void convert_to_extbv( CHAR_DATA *ch, int version, int bitv );
EXT_BV bitv_to_extbv( int bitv );
char *ext_string_flag( const struct flag_type *flag_table, EXT_BV bits );
EXT_BV multimeb( int bit, ... );

void do_new_fread_to_eol( FILE *fp );
void do_forceall( char *argument);
void do_setstacker( CHAR_DATA *ch, char *argument );
void do_setspeeder( CHAR_DATA *ch, char *argument );
void resetchar args(( CHAR_DATA *ch, char *argument));
void cleartimer args(( CHAR_DATA *ch));
void ranthint args(( CHAR_DATA *ch, char *argument));
void split args(( CHAR_DATA *ch, char *argument));
int     insta_to_char   args( ( CHAR_DATA *ch, char *argument, bool 
showprompt ) );

void check_multi_cmd ( CHAR_DATA * ch, char *orig_cmd, char *final_cmd );
void drop_unowned args (( CHAR_DATA *ch, OBJ_DATA *obj));
int cprintf   ( char *buf, char *ptr, ... ) __attribute__ ((format(printf, 2, 3)));
int cnprintf  ( char *buf, int maxlen, char *ptr, ... ) __attribute__ ((format(printf, 3, 4)));
int   collen           ( const char *str );
char *string_restrict(char *str, int size);
char *get_exits                 args (( CHAR_DATA *ch ));
void  draw_map           args (( CHAR_DATA * ch, char *desc ));
void pkready args (( CHAR_DATA *ch ));
void randhint args (( CHAR_DATA *ch ));
void do_cleanup args (( CHAR_DATA *ch, char *argument));
void fixadam args (( CHAR_DATA *ch ));
void fixstatus args (( CHAR_DATA *ch ));
extern char * last_command;

