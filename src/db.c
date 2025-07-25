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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#if !defined(macintosh)
extern	int	_filbuf		args( (FILE *) );
#endif


/*
 * Globals.
 */
SYSTEM_DATA             sysdata;

CLASS_LEADER class_leader;
LEADER_BOARD leader_board;
KINGDOM_DATA king_table[MAX_KINGDOM];
KINGDOM_DESC king_desc_list[MAX_KINGDOM];
WAR_DATA war_table[MAX_WAR];

SHOP_DATA *		shop_first;
SHOP_DATA *		shop_last;

CHAR_DATA *		char_free;
NOTE_DATA *		note_free;
EXTRA_DESCR_DATA *	extra_descr_free;
OBJ_DATA *		obj_free;
BAN_DATA *              ban_free;

char			bug_buf		[2*MAX_INPUT_LENGTH];
CHAR_DATA *		char_list;
char *			help_greeting;
char *			last_command;
char			log_buf		[2*MAX_INPUT_LENGTH];
KILL_DATA		kill_table	[MAX_LEVEL];
OBJ_DATA *		object_list;
TIME_INFO_DATA		time_info;
WEATHER_DATA		weather_info;
COPYOVER                copyover_info;

sh_int			gsn_stuntubes;
sh_int			gsn_laser;
sh_int      gsn_stinger;
sh_int			gsn_backstab;
sh_int                  gsn_shred;
sh_int                  gsn_quills;
sh_int			gsn_bladespin;
sh_int		gsn_fiery;
sh_int		gsn_hooves;
sh_int		gsn_fireball;
sh_int		gsn_deathaura;
sh_int      gsn_wrathofgod;
sh_int		gsn_claws;
sh_int	    gsn_heavenlyaura;
sh_int      gsn_mageshield;
sh_int      gsn_darktendrils;
sh_int 			gsn_cheapshot;
sh_int                  gsn_spit;
sh_int			gsn_venomtong;
sh_int			gsn_spiketail;
sh_int			gsn_badbreath;
sh_int			gsn_magma;
sh_int			gsn_hellfire;
sh_int			gsn_shards;
sh_int			gsn_hide;
sh_int			gsn_peek;
sh_int			gsn_pick_lock;
sh_int			gsn_sneak;
sh_int			gsn_steal;
sh_int			gsn_spiderform;
sh_int			gsn_garotte;
sh_int			gsn_disarm;
sh_int			gsn_tendrils;
sh_int			gsn_berserk;
sh_int			gsn_punch;
sh_int			gsn_headbutt;
sh_int			gsn_spiket;
sh_int			gsn_venomt;
sh_int			gsn_shards;
sh_int			gsn_magma;
sh_int			gsn_shiroken;
sh_int			gsn_inferno;
sh_int			gsn_blinky;
sh_int			gsn_fangs;
sh_int			gsn_buffet;
sh_int			gsn_sweep;
sh_int			gsn_knee;
sh_int			gsn_lightningslash;
sh_int                  gsn_rfangs;
sh_int			gsn_thrustkick;
sh_int			gsn_spinkick;
sh_int			gsn_backfist;
sh_int			gsn_elbow;
sh_int			gsn_shinkick;
sh_int			gsn_palmstrike;
sh_int			gsn_lightningkick;
sh_int			gsn_tornadokick;
sh_int			gsn_jumpkick;
sh_int			gsn_spinkick;
sh_int			gsn_monksweep;
sh_int		gsn_circle;
sh_int		gsn_booming;
sh_int			gsn_chillhand;
sh_int			gsn_kick;
sh_int			gsn_hurl;
sh_int			gsn_rescue;
sh_int			gsn_track;
sh_int			gsn_polymorph;
sh_int			gsn_web;
sh_int			gsn_drowfire;
sh_int                  gsn_infirmity;
sh_int                  gsn_spew;
sh_int			gsn_blindness;
sh_int			gsn_charm_person;
sh_int			gsn_curse;
sh_int			gsn_invis;
sh_int			gsn_mass_invis;
sh_int			gsn_poison;
sh_int			gsn_sleep;
sh_int			gsn_multiplearms;

sh_int			gsn_darkness;
sh_int			gsn_paradox;
sh_int			gsn_music;
sh_int          gsn_arrowiv;
sh_int          gsn_sweepblow;
sh_int		gsn_assassinate;
sh_int		gsn_shadowblast;
sh_int		gsn_sanctuary;
sh_int		gsn_spin;
sh_int		gsn_leech;
sh_int		gsn_divinewrath;
sh_int		gsn_feint;
sh_int		gsn_riposte;
sh_int  gsn_enfeeble;
sh_int  gsn_mindblast;
sh_int  gsn_critical_hit;
sh_int  gsn_slide;
sh_int  gsn_hiltstrike;
sh_int  gsn_shinku;
sh_int  gsn_choke;
sh_int  gsn_kisetsufuu;
sh_int  gsn_kaminari;
sh_int  gsn_inabikari;
sh_int	gsn_inquisitor;
sh_int	gsn_unholytorment;
sh_int	gsn_purify;
sh_int  gsn_slash;
sh_int  gsn_knock;
sh_int  gsn_riftwalk;
sh_int  gsn_recall;
/*
 * Locals.
 */
MOB_INDEX_DATA *	mob_index_hash		[MAX_KEY_HASH];
OBJ_INDEX_DATA *	obj_index_hash		[MAX_KEY_HASH];
ROOM_INDEX_DATA *	room_index_hash		[MAX_KEY_HASH];
char *			db_string_hash		[MAX_KEY_HASH];

AREA_DATA *		first_area;
AREA_DATA *		last_area;

CONFIG_DATA *		config_info;
ROOM_INDEX_DATA *	room_list;

HELP_DATA *		first_help;
HELP_DATA *		last_help;


char *			string_space;
char *			top_string;
char			str_empty	[1];

int			top_affect;
int			top_area;
int			top_rt;
int			top_ed;
int			top_exit;
int			top_help;
int			top_mob_index;
int			top_obj_index;
int			top_reset;
int			top_room;
int			top_shop;
int                     top_vnum_room;  /* OLC */
int                     top_vnum_mob;   /* OLC */
int                     top_vnum_obj;   /* OLC */

bool CHAOS = FALSE;
bool VISOR = FALSE;
bool DARKNESS = FALSE;
bool SPEED = FALSE;
bool BRACELET = FALSE;
bool TORC = FALSE;
bool ARMOUR = FALSE;
bool CLAWS = FALSE;
bool ITEMAFFMANTIS = FALSE;
bool ITEMAFFENTROPY = FALSE;

EXT_BV convert_bitv( int bitv );

/*
 * Memory management.
 * Increase MAX_STRING if you have too.
 * Tune the others only if you understand what you're doing.
 */

#define			MAX_STRING	2072864
#define			MAX_PERM_BLOCK	131072
#define			MAX_MEM_LIST	11

void *			rgFreeList	[MAX_MEM_LIST];
const int		rgSizeList	[MAX_MEM_LIST]	=
{
    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768-64
};

int			nAllocString;
int			sAllocString;
int			nAllocPerm;
int			sAllocPerm;



/*
 * Semi-locals.
 */
bool			fBootDb;
bool			resetAreas;
FILE *			fpArea;
char			strArea[MAX_INPUT_LENGTH];



/*
 * Local booting procedures.
 */
void	init_mm		args( ( void ) );

void	load_area	args( ( FILE *fp ) );
void    load_helps      args( ( FILE *fp ) );
void	load_mobiles	args( ( FILE *fp ) );
void	load_objects	args( ( FILE *fp ) );
void	load_resets	args( ( FILE *fp ) );
void	load_rooms	args( ( FILE *fp ) );
void	load_shops	args( ( FILE *fp ) );
void	load_specials	args( ( FILE *fp ) );
void    load_artifacts  args( ( void ) );
void    new_load_area   args( ( FILE *fp ) );   /* OLC */
void    new_load_rooms  args( ( FILE *fp ) );   /* OLC 1.1b */


void	fix_exits	args( ( void ) );

void	reset_area	args( ( AREA_DATA * pArea ) );


void remove_tilde( char *str );

/*
 * Big mama top level function.
 */
void boot_db(bool fCopyOver)
{
	char buf[MAX_STRING_LENGTH];

    /*
     * Init some data space stuff.
     */
    {
	if ( ( string_space = calloc( 1, MAX_STRING ) ) == NULL )
	{
	    bug( "Boot_db: can't alloc %d string space.", MAX_STRING );
	    exit( 1 );
	}
	top_string	= string_space;
	fBootDb		= TRUE;
    }

    log_string( "Loading sysdata configuration..." );

    /* sysdata default values */
    sysdata.save_frequency              = 20;   /* minutes */
    sysdata.save_flags                  = SV_PASSCHG | SV_AUTO
                                        | SV_PUT | SV_DROP | SV_GIVE
                                        | SV_ZAPDROP | SV_IDLE;
    sysdata.exp_timer = -1;
    sysdata.qp_timer = -1;
    sysdata.ffa_timer = -1;
    sysdata.curr_change = 0;
    sysdata.max_items = 1000;
    last_command = NULL;
    quitting_char	= NULL;


    if ( !load_systemdata(&sysdata) )
    {
        log_string( "Not found.  Creating new configuration." );
        sysdata.alltimemax = 0;
        sysdata.mud_name = str_dup("(NoName)");
    }

    /*
     * Init random number generator.
     */
    {
	init_mm( );
    }
        copyover_info.time      = 0;
        copyover_info.name      = "No one";
    /*
     * Set time and weather.
     */
    {
	long lhour, lday, lmonth;

	lhour		= (current_time - 650336715)
			/ (PULSE_TICK / PULSE_PER_SECOND);
	time_info.hour	= lhour  % 24;
	lday		= lhour  / 24;
	time_info.day	= lday   % 35;
	lmonth		= lday   / 35;
	time_info.month	= lmonth % 17;
	time_info.year	= lmonth / 17;

	     if ( time_info.hour <  5 ) weather_info.sunlight = SUN_DARK;
	else if ( time_info.hour <  6 ) weather_info.sunlight = SUN_RISE;
	else if ( time_info.hour < 19 ) weather_info.sunlight = SUN_LIGHT;
	else if ( time_info.hour < 20 ) weather_info.sunlight = SUN_SET;
	else                            weather_info.sunlight = SUN_DARK;

	weather_info.change	= 0;
	weather_info.mmhg	= 960;
	if ( time_info.month >= 7 && time_info.month <=12 )
	    weather_info.mmhg += number_range( 1, 50 );
	else
	    weather_info.mmhg += number_range( 1, 80 );

	     if ( weather_info.mmhg <=  980 ) weather_info.sky = SKY_LIGHTNING;
	else if ( weather_info.mmhg <= 1000 ) weather_info.sky = SKY_RAINING;
	else if ( weather_info.mmhg <= 1020 ) weather_info.sky = SKY_CLOUDY;
	else                                  weather_info.sky = SKY_CLOUDLESS;

    }

	log_string("Initializing Config");
	initialise_config();


    /*
     * Assign gsn's for skills which have them.
     */
    {
	int sn;

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].pgsn != NULL )
		*skill_table[sn].pgsn = sn;
	}
    }

    /*
     * Read in all the area files.
     */
    {
	FILE *fpList;

	if ( ( fpList = fopen( AREA_LIST, "r" ) ) == NULL )
	{
	    perror( AREA_LIST );
	    exit( 1 );
	}

	for ( ; ; )
	{
	    strcpy( strArea, fread_word( fpList ) );
	    if ( strArea[0] == '$' )
		break;

	    if ( strArea[0] == '-' )
	    {
		fpArea = stdin;
	    }
	    else
	    {
		sprintf(buf,"loading %s",strArea);
		log_string(buf);
		if ( ( fpArea = fopen( strArea, "r" ) ) == NULL )
		{
		    perror( strArea );
		    exit( 1 );
		}
	    }

	    for ( ; ; )
	    {
		char *word;

		if ( fread_letter( fpArea ) != '#' )
		{
		    bug( "Boot_db: # not found.", 0 );
		    exit( 1 );
		}

		word = fread_word( fpArea );
		     if ( word[0] == '$'               )                 break;
		else if ( !str_cmp( word, "AREA"     ) ) load_area(fpArea);
		else if ( !str_cmp( word, "HELPS"    ) ) load_helps   (fpArea);
		else if ( !str_cmp( word, "MOBILES"  ) ) load_mobiles (fpArea);
		else if ( !str_cmp( word, "OBJECTS"  ) ) load_objects (fpArea);
		else if ( !str_cmp( word, "RESETS"   ) ) load_resets  (fpArea);
		else if ( !str_cmp( word, "ROOMS"    ) ) load_rooms   (fpArea);
		else if ( !str_cmp( word, "SHOPS"    ) ) load_shops   (fpArea);
		else if ( !str_cmp( word, "SPECIALS" ) ) load_specials(fpArea);
                else if ( !str_cmp( word, "AREADATA" ) )        /* OLC */
                    new_load_area( fpArea );
                else if ( !str_cmp( word, "ROOMDATA" ) )        /* OLC 1.1b */
                    new_load_rooms( fpArea );

		else
		{
		    bug( "Boot_db: bad section name.", 0 );
		    exit( 1 );
		}
	    }

	    if ( fpArea != stdin )
		fclose( fpArea );
	    fpArea = NULL;
	}
	fclose( fpList );
        strcpy (strArea, "");
    }
    load_clanlist();
    init_supersnard();

    /*
     * Fix up exits.
     * Declare db booting over.
     * Reset all areas once.
     * Load up the notes file.
     */
    {
	log_string("Fixing Exits");
	fix_exits();
	fBootDb	= FALSE;
	log_string("Updating Areas");
	area_update(TRUE);
	log_string("Loading Bans");
	load_bans();
	log_string("Loading Kingshit");
        load_kingshit();
	log_string("Loading Leaderboard");
        load_leaderboard();
	log_string("Loading_kingdoms");
	load_kingdoms();
        log_string("loading class leader");
        load_classleader();
	log_string("Loading War");
	log_string("Loading Boards");
	load_boards();
	log_string("Saving NOtes");
	save_notes();
	log_string("Loading Disable");
	load_disabled();
	log_string("Loading Changes");
        load_changes();
	log_string("Loading Artis");
        load_artifacts();
	log_string("Loading Todo");
        load_todo();
	log_string("Loading Quotes");
        load_quote();
	log_string("Loading Swearlist");
	read_swearlist();
	log_string("Loading Multilist");
	read_multilist();
	log_string("loading socials");
	load_social_table();
	log_string("loading hints");
	load_hint_table();
    }
 	
 	if (fCopyOver)
 		copyover_recover();

    return;
}





/*
 * Load the Artifacts ( whee ).
 */

void load_artifacts( void )
{
    return;
}

/*
 * Snarf an 'area' header line.
 */
void load_area( FILE *fp )
{
    AREA_DATA *pArea;

    CREATE( pArea, AREA_DATA, 1);
    pArea->name		= fread_string( fp );
	log_string( pArea->name );
    pArea->age		= 30;
    pArea->perm_age	= 30;
    pArea->nplayer	= 0;

    pArea->area_flags   = AREA_LOADING;         /* OLC */
    pArea->security     = 3;                    /* OLC */
    pArea->builders     = str_dup( "None" );    /* OLC */
    pArea->lvnum        = 0;                    /* OLC */
    pArea->uvnum        = 0;                    /* OLC */
    pArea->vnum         = top_area;             /* OLC */
    pArea->filename     = str_dup( strArea );   /* OLC */

    REMOVE_BIT(last_area->area_flags, AREA_LOADING); /* OLC */
    LINK( pArea, first_area, last_area, next, prev);

    top_area++;
    return;
}




#if defined(KEY)
#undef KEY
#endif
                    
#define KEY( literal, field, value )                \
                if ( !str_cmp( word, literal ) )    \
                {                                   \
                    field  = value;                 \
                    fMatch = TRUE;                  \
                    break;                          \
                }
    
#define SKEY( string, field )                       \
                if ( !str_cmp( word, string ) )     \
                {                                   \
                    free_string( field );           \
                    field = fread_string( fp );     \
                    fMatch = TRUE;                  \
                    break;                          \
                }
  

/* OLC
 * Snarf an 'area' header line.   Check this format.  MUCH better.  Add fields
 * too.             
dv *
 * #AREAFILE
 * Name   { All } Locke    Newbie School~
 * Repop  A teacher pops in the room and says, 'Repop coming!'~
 * Recall 3001   
 * End
 */
void new_load_area( FILE *fp )
{
    AREA_DATA *pArea;
    char      *word;
    bool      fMatch;
    CREATE( pArea, AREA_DATA, 1);
    pArea->age          = 30;
    pArea->perm_age     = 30;
    pArea->nplayer      = 0;
    pArea->filename     = str_dup( strArea );
    pArea->vnum         = top_area;
    pArea->name         = str_dup( "New Area" );
    pArea->builders     = str_dup( "" );
    pArea->security     = 3;
    pArea->lvnum        = 0;
    pArea->uvnum        = 0;
    pArea->area_flags   = 0;
    pArea->recall       = ROOM_VNUM_TEMPLE;
 
    for ( ; ; )
    {
       word   = feof( fp ) ? "End" : fread_word( fp );
       fMatch = FALSE;
 
       switch ( UPPER(word[0]) )
       {
           case 'N':
            SKEY( "Name", pArea->name );
            break;   
           case 'S':
             KEY( "Security", pArea->security, fread_number( fp ) );
            break;
           case 'V':
            if ( !str_cmp( word, "VNUMs" ) )
            {
                pArea->lvnum = fread_number( fp );
                pArea->uvnum = fread_number( fp );
            }
            break;
           case 'E':
             if ( !str_cmp( word, "End" ) )
             {
                 fMatch = TRUE;
		 LINK( pArea, first_area, last_area, next, prev);
		 pArea->age = (30 - pArea->perm_age);
                 top_area++;
                 return;
            }
            break;  
           case 'B':
            SKEY( "Builders", pArea->builders );
            break;  
           case 'R':
             KEY( "Recall", pArea->recall, fread_number( fp ) );
            break;  
	   case 'A':
	     KEY( "Age", pArea->perm_age, fread_number( fp ) );
	   break;
        }
    }
    log_string( pArea->name );
}
                
             
            
/*
 * Sets vnum range for area using OLC protection features.
 */
void assign_area_vnum( int vnum )
{
    if ( last_area->lvnum == 0 || last_area->uvnum == 0 )
        last_area->lvnum = last_area->uvnum = vnum;
    if ( vnum != URANGE( last_area->lvnum, vnum, last_area->uvnum ) )
	{
        if ( vnum < last_area->lvnum )
		{
            last_area->lvnum = vnum;
		}
        else
		{
            last_area->uvnum = vnum;
		}
	}
    return;  
}




/*
 * Returns an uppercase string.
 */
char *strupper( const char *str )
{
    static char strup[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
	strup[i] = UPPER(str[i]);
    strup[i] = '\0';
    return strup;
}


/*
 * Adds a help page to the list if it is not a duplicate of an existing page.
 * Page is insert-sorted by keyword.			-Thoric
 * (The reason for sorting is to keep do_hlist looking nice)
 */
void add_help( HELP_DATA *pHelp )
{
    HELP_DATA *tHelp;
    int match;

    for ( tHelp = first_help; tHelp; tHelp = tHelp->next )
	if ( (match=strcmp(pHelp->keyword[0]=='\'' ? pHelp->keyword+1 : pHelp->keyword,
			   tHelp->keyword[0]=='\'' ? tHelp->keyword+1 : tHelp->keyword)) < 0
	||   (match == 0 && pHelp->level > tHelp->level) )
	{
	    if ( !tHelp->prev )
		first_help	  = pHelp;
	    else
		tHelp->prev->next = pHelp;
	    pHelp->prev		  = tHelp->prev;
	    pHelp->next		  = tHelp;
	    tHelp->prev		  = pHelp;
	    break;
	}

    if ( !tHelp )
	LINK( pHelp, first_help, last_help, next, prev );

    top_help++;
}


/*
 * Load a help section.
 */
void load_helps( FILE *fp )
{
    HELP_DATA *pHelp;
    for ( ; ; )
    {
	CREATE( pHelp, HELP_DATA, 1 );
	pHelp->level	= fread_number( fp );
	pHelp->keyword	= fread_string( fp );
	if ( pHelp->keyword[0] == '$' )
	{
	    STRFREE( pHelp->keyword );
	    DISPOSE( pHelp );
	    break;
	}
	pHelp->text	= fread_string( fp );
	if ( pHelp->keyword[0] == '\0' )
	{
	    STRFREE( pHelp->text );
	    STRFREE( pHelp->keyword );
	    DISPOSE( pHelp );
	    continue;
	}
	if ( !str_cmp( pHelp->keyword, "greeting" ) )
	    help_greeting = pHelp->text;

	add_help( pHelp );
    }
    return;
}


/*
 * Snarf a mob section.
 */
void load_mobiles( FILE *fp )
{
    MOB_INDEX_DATA *pMobIndex;

    if ( !last_area )   /* OLC */
    {
        bug( "Load_mobiles: no #AREA seen yet.", 0 );
        exit( 1 );
    }
 
    for ( ; ; )
    {
	int vnum;
	char letter;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_mobiles: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_mob_index( vnum ) != NULL )
	{
	    bug( "Load_mobiles: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pMobIndex			= alloc_perm( sizeof(*pMobIndex) );
	pMobIndex->vnum			= vnum;
        pMobIndex->area                 = last_area;            /* OLC */
	pMobIndex->player_name		= fread_string( fp );
	pMobIndex->short_descr		= fread_string( fp );
	pMobIndex->long_descr		= fread_string( fp );
	pMobIndex->description		= fread_string( fp );

	pMobIndex->long_descr[0]	= UPPER(pMobIndex->long_descr[0]);
	pMobIndex->description[0]	= UPPER(pMobIndex->description[0]);

        xCLEAR_BITS(pMobIndex->act);
        pMobIndex->act = fread_bitvector( fp );
	if (!xIS_SET(pMobIndex->act, ACT_IS_NPC) )
	  xSET_BIT(pMobIndex->act, ACT_IS_NPC);
	pMobIndex->affected_by		= fread_number( fp );
	pMobIndex->itemaffect		= 0;
	pMobIndex->pShop		= NULL;
	pMobIndex->alignment		= fread_number( fp );
	letter				= fread_letter( fp );
	pMobIndex->level		= number_fuzzy( fread_number( fp ) );

	// Legacy stats-in-files method - all parameters read but unused
	pMobIndex->hitroll		= fread_number( fp );
	pMobIndex->ac			= fread_number( fp );
	pMobIndex->hitnodice		= fread_number( fp );
	fread_letter( fp );						// 'd' separator
	pMobIndex->hitsizedice		= fread_number( fp );
	fread_letter( fp );						// '+' separator  
	pMobIndex->hitplus		= fread_number( fp );
	pMobIndex->damnodice		= fread_number( fp );
	fread_letter( fp );						// 'd' separator
	pMobIndex->damsizedice		= fread_number( fp );
	fread_letter( fp );						// '+' separator
	pMobIndex->damplus		= fread_number( fp );
	pMobIndex->gold			= fread_number( fp );
	fread_number( fp );						// xp - cannot be used
	fread_number( fp );						// position
	fread_number( fp );						// start position

	/*
	 * Back to meaningful values.
	 */
	pMobIndex->sex			= fread_number( fp );

	if ( letter != 'S' )
	{
	    bug( "Load_mobiles: vnum %d non-S.", vnum );
	    exit( 1 );
	}

	iHash			= vnum % MAX_KEY_HASH;
	pMobIndex->next		= mob_index_hash[iHash];
	mob_index_hash[iHash]	= pMobIndex;
	top_mob_index++;
        top_vnum_mob = top_vnum_mob < vnum ? vnum : top_vnum_mob;  /* OLC */
        assign_area_vnum( vnum );                                  /* OLC */

	kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;
    }

	return;
}

/*
 * Snarf an obj section.
 */
void load_objects( FILE *fp )
{
    OBJ_INDEX_DATA *pObjIndex;


    if ( !last_area )   /* OLC */
    {
        bug( "Load_objects: no #AREA seen yet.", 0 );
        exit( 1 );
    }

    for ( ; ; )
    {
	int vnum;
	char letter;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_objects: # not found, %c found instead", letter );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;


	fBootDb = FALSE;
	if ( get_obj_index( vnum ) != NULL )
	{
	    bug( "Load_objects: vnum %d duplicated.", vnum );
	    exit( 1 );
	}

	fBootDb = TRUE;

	pObjIndex			= alloc_perm( sizeof(*pObjIndex) );
	pObjIndex->vnum			= vnum;
        pObjIndex->area                 = last_area;            /* OLC */
	pObjIndex->name			= fread_string( fp );
	pObjIndex->short_descr		= fread_string( fp );
	pObjIndex->description		= fread_string( fp );
	fread_string( fp );	/* Action description */

	pObjIndex->short_descr[0]	= LOWER(pObjIndex->short_descr[0]);
	pObjIndex->description[0]	= UPPER(pObjIndex->description[0]);

	pObjIndex->item_type		= fread_number( fp );
	pObjIndex->extra_flags		= fread_number( fp );
	pObjIndex->wear_flags		= fread_number( fp );
	switch (pObjIndex->item_type) {
	case ITEM_WAND:
	case ITEM_STAFF:
		pObjIndex->value[0] = fread_number(fp);
		pObjIndex->value[1] = fread_number(fp);
		pObjIndex->value[2] = fread_number(fp);
		pObjIndex->value[3] = skill_lookup(fread_word(fp));
		break;
	case ITEM_POTION:
	case ITEM_PILL:
                pObjIndex->value[0] = fread_number(fp);
                pObjIndex->value[1] = skill_lookup(fread_word(fp));
                pObjIndex->value[2] = skill_lookup(fread_word(fp));
                pObjIndex->value[3] = skill_lookup(fread_word(fp));
                break;
	case ITEM_SCROLL:
		pObjIndex->value[0] = fread_number(fp);
		pObjIndex->value[1] = skill_lookup(fread_word(fp));
		pObjIndex->value[2] = skill_lookup(fread_word(fp));
		pObjIndex->value[3] = skill_lookup(fread_word(fp));
		break;
	default:
	pObjIndex->value[0]		= fread_number( fp );
	pObjIndex->value[1]		= fread_number( fp );
	pObjIndex->value[2]		= fread_number( fp );
	pObjIndex->value[3]		= fread_number( fp );
	break;
	}
	pObjIndex->weight		= fread_number( fp );
	pObjIndex->cost			= fread_number( fp );	// Cleaned up unused comment to modern style
	pObjIndex->first_affect		= NULL;
	pObjIndex->last_affect		= NULL;
	pObjIndex->extra_descr		= NULL;
	pObjIndex->chpoweron		= NULL;
	pObjIndex->chpoweroff		= NULL;
	pObjIndex->chpoweruse		= NULL;
	pObjIndex->victpoweron		= NULL;
	pObjIndex->victpoweroff		= NULL;
	pObjIndex->victpoweruse		= NULL;
	pObjIndex->spectype		= 0;
	pObjIndex->specpower		= 0;
	fread_number( fp );	// Cleaned up cost per day comment to modern style
	for ( ; ; )
	{
	    char letter;

	    letter = fread_letter( fp );
	    if ( letter == 'A' )
	    {
		AFFECT_DATA *paf;

		CREATE(paf, AFFECT_DATA, 1);
		paf->type		= -1;
		paf->duration		= -1;
		paf->location		= fread_number( fp );
		paf->modifier		= fread_number( fp );
		paf->bitvector		= 0;
		LINK( paf, pObjIndex->first_affect, pObjIndex->last_affect, next, prev );
		top_affect++;
	    }

	    else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		ed			= alloc_perm( sizeof(*ed) );
		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= pObjIndex->extra_descr;
		pObjIndex->extra_descr	= ed;
		top_ed++;
	    }

	    else if ( letter == 'Q' )
	    {
		pObjIndex->chpoweron	= fread_string( fp );
		pObjIndex->chpoweroff	= fread_string( fp );
		pObjIndex->chpoweruse	= fread_string( fp );
		pObjIndex->victpoweron	= fread_string( fp );
		pObjIndex->victpoweroff	= fread_string( fp );
		pObjIndex->victpoweruse	= fread_string( fp );
		pObjIndex->spectype	= fread_number( fp );
		pObjIndex->specpower	= fread_number( fp );
	    }

	    else
	    {
		ungetc( letter, fp );
		break;
	    }
	}

	/*
	 * Translate spell "slot numbers" to internal "skill numbers."
	 */
	switch ( pObjIndex->item_type )
	{
	case ITEM_PILL:
	case ITEM_POTION:
	case ITEM_SCROLL:
	    pObjIndex->value[1] = slot_lookup( pObjIndex->value[1] );
	    pObjIndex->value[2] = slot_lookup( pObjIndex->value[2] );
	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
	    break;

	case ITEM_STAFF:
	case ITEM_WAND:
	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
	    break;
	}

	// Special artifact tracking flags
	if ( vnum == 29503 )
		CHAOS = TRUE;
	if ( vnum == 29515 )
		VISOR = TRUE;
	if ( vnum == 29512 )
		DARKNESS = TRUE;
	if ( vnum == 29505 )
		SPEED = TRUE;
	if ( vnum == 29518 )
		BRACELET = TRUE;
	if ( vnum == 29504 )
		TORC = TRUE;
	if ( vnum == 29514 )
		ARMOUR = TRUE;
	if ( vnum == 29516 )
		CLAWS = TRUE;
	if ( vnum == 29555 )
		ITEMAFFMANTIS = TRUE;
	if ( vnum == 2654 || vnum == 29598 )
		ITEMAFFENTROPY = TRUE;

	iHash			= vnum % MAX_KEY_HASH;
	pObjIndex->next		= obj_index_hash[iHash];
	obj_index_hash[iHash]	= pObjIndex;
	top_obj_index++;
        top_vnum_obj = top_vnum_obj < vnum ? vnum : top_vnum_obj;  /* OLC */
        assign_area_vnum( vnum );                                  /* OLC */
 
    }

    return;
}

/*
 * Adds a reset to a room.  OLC
 * Similar to add_reset in olc.c
 */
void new_reset( ROOM_INDEX_DATA *pR, RESET_DATA *pReset )
{
    RESET_DATA *pr;
     
    if ( !pR )
       return;

    pr = pR->reset_last;
        
    if ( !pr )
    {
        pR->reset_first = pReset;
        pR->reset_last  = pReset;
    }
    else
    {
        pR->reset_last->next = pReset;
        pR->reset_last       = pReset;
        pR->reset_last->next = NULL;
    }   

    top_reset++;
    return;
}


/*   
 * Snarf a reset section.       Changed for OLC.
 */      
void load_resets( FILE *fp )
{
    RESET_DATA  *pReset;
    int         iLastRoom = 0;
    int         iLastObj  = 0;
    
    if ( !last_area )
    {
        bug( "Load_resets: no #AREA seen yet.", 0 );
        exit( 1 );
    }    
  
    for ( ; ; )
    {
        EXIT_DATA       *pexit;
        ROOM_INDEX_DATA *pRoomIndex;
        char             letter;
    
        if ( ( letter = fread_letter( fp ) ) == 'S' )
            break;
     
        if ( letter == '*' )
        {
            fread_to_eol( fp );
            continue;
        }

        pReset          = alloc_perm( sizeof( *pReset ) );
        pReset->command = letter;
        fread_number( fp );  // if_flag - unused parameter
        pReset->arg1    = fread_number( fp );
        pReset->arg2    = fread_number( fp );
        pReset->arg3    = ( letter == 'G' || letter == 'R' )
                            ? 0 : fread_number( fp );
                          fread_to_eol( fp );
        
        /*
         * Validate parameters.
         * We're calling the index functions for the side effect.
         */
        switch ( letter )
        {
        default:
            bug( "Load_resets: bad command '%c'.", letter );
            exit( 1 );
            break;
     
        case 'M':
            get_mob_index  ( pReset->arg1 );
            if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
            {
                new_reset( pRoomIndex, pReset );
                iLastRoom = pReset->arg3;
            }
            break;
        
        case 'O':
            get_obj_index  ( pReset->arg1 ); 
            if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
            {
                new_reset( pRoomIndex, pReset );
                iLastObj = pReset->arg3;
            }
            break;
         
        case 'P':
            get_obj_index  ( pReset->arg1 );
            if ( ( pRoomIndex = get_room_index ( iLastObj ) ) )
            {   
                new_reset( pRoomIndex, pReset );
            }
            break;
     
        case 'G':
        case 'E':
            get_obj_index  ( pReset->arg1 );
            if ( ( pRoomIndex = get_room_index ( iLastRoom ) ) )
            {
                new_reset( pRoomIndex, pReset );
                iLastObj = iLastRoom;
            }
            break;
        
        case 'D':
            pRoomIndex = get_room_index( pReset->arg1 );
             
            if (   pReset->arg2 < 0
                || pReset->arg2 > 5
                || !pRoomIndex
                || !( pexit = pRoomIndex->exit[pReset->arg2] )
                || !IS_SET( pexit->rs_flags, EX_ISDOOR ) )
            {
                bug( "Load_resets: 'D': exit %d not door.", pReset->arg2 );
                exit( 1 );
            }   
                
            switch ( pReset->arg3 )     /* OLC 1.1b */
            {
                default:
                    bug( "Load_resets: 'D': bad 'locks': %d." , pReset->arg3);
                case 0:
                    break;
                case 1: SET_BIT( pexit->rs_flags, EX_CLOSED );  
                    break;
                case 2: SET_BIT( pexit->rs_flags, EX_CLOSED | EX_LOCKED );
                    break;
            }
            break;
        
        case 'R':
            if ( pReset->arg2 < 0 || pReset->arg2 > 6 ) /* Last Door. */
            {
                bug( "Load_resets: 'R': bad exit %d.", pReset->arg2 );
                exit( 1 );
            }
                
            if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) )
                new_reset( pRoomIndex, pReset );
                
            break;
        }
    }           
            
            
	return;  
}

/*
 * Snarf a room section.
 */
void load_rooms( FILE *fp )
{
    ROOM_INDEX_DATA *pRoomIndex;
    int i;

    if ( last_area == NULL )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
	int vnum;
	char letter;
	int door;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_rooms: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_room_index( vnum ) != NULL )
	{
	    bug( "Load_rooms: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pRoomIndex			= alloc_perm( sizeof(*pRoomIndex) );
	pRoomIndex->people		= NULL;
	pRoomIndex->contents		= NULL;
	pRoomIndex->first_person	= NULL;
	pRoomIndex->last_person		= NULL;
	pRoomIndex->first_content	= NULL;
	pRoomIndex->last_content	= NULL;
	pRoomIndex->first_extradesc	= NULL;
	pRoomIndex->last_extradesc	= NULL;
	pRoomIndex->area		= last_area;
	pRoomIndex->vnum		= vnum;
	pRoomIndex->name		= fread_string( fp );
	pRoomIndex->description		= fread_string( fp );
	fread_number( fp );	/* Area number */
	pRoomIndex->room_flags		= fread_number( fp );
	pRoomIndex->sector_type		= fread_number( fp );
	pRoomIndex->light		= 0;
	pRoomIndex->blood		= 0;
	pRoomIndex->roomtext		= NULL;
	pRoomIndex->first_exit		= NULL;
	pRoomIndex->last_exit		= NULL;

	for (i = 0 ; i < MAX_RTIMER ; i++)
            pRoomIndex->tick_timer[i] = 0;

	for ( door = 0; door <= 4; door++ )
	{
	    pRoomIndex->track[door] 	= str_dup( "" );
	    pRoomIndex->track_dir[door] = 0;
	}
	for ( door = 0; door <= 5; door++ )
	    pRoomIndex->exit[door] = NULL;


	for ( ; ; )
	{
	    letter = fread_letter( fp );

	    if ( letter == 'S' )
		break;

	    if ( letter == 'D' )
	    {
		EXIT_DATA *pexit;
		int locks;

		door = fread_number( fp );
		if ( door < 0 || door > 5 )
		{
		    bug( "Fread_rooms: vnum %d has bad door number.", vnum );
		    exit( 1 );
		}

		pexit			= alloc_perm( sizeof(*pexit) );
		pexit->description	= fread_string( fp );
		pexit->keyword		= fread_string( fp );
		pexit->exit_info	= 0;
                pexit->rs_flags         = 0;                    /* OLC */ 
		locks			= fread_number( fp );
		pexit->key		= fread_number( fp );
		pexit->vnum		= fread_number( fp );


                switch ( locks )        /* OLC exit_info to rs_flags. */
                {
                case 1: pexit->rs_flags  = EX_ISDOOR;                    break;
                case 2: pexit->rs_flags  = EX_ISDOOR | EX_PICKPROOF;     break;
                }


		pRoomIndex->exit[door]	= pexit;
		top_exit++;
	    }
	    else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		CREATE(ed, EXTRA_DESCR_DATA, 1);
		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		LINK( ed, pRoomIndex->first_extradesc, pRoomIndex->last_extradesc, next, prev);
		top_ed++;
	    }
	    else if ( letter == 'T' )
	    {
		ROOMTEXT_DATA *rt;

		rt			= alloc_perm( sizeof(*rt) );
		rt->input		= fread_string( fp );
		rt->output		= fread_string( fp );
		rt->choutput		= fread_string( fp );
		rt->name		= fread_string( fp );
		rt->type		= fread_number( fp );
		rt->power		= fread_number( fp );
		rt->mob			= fread_number( fp );
		rt->next		= pRoomIndex->roomtext;
		pRoomIndex->roomtext	= rt;
		top_rt++;
	    }
	    else
	    {
		bug( "Load_rooms: vnum %d has flag not 'DES'.", vnum );
		exit( 1 );
	    }
	}

	iHash			= vnum % MAX_KEY_HASH;
	pRoomIndex->next	= room_index_hash[iHash];
	room_index_hash[iHash]	= pRoomIndex;
	pRoomIndex->next_room	= room_list;
	room_list		= pRoomIndex;
	top_room++;
       top_vnum_room = top_vnum_room < vnum ? vnum : top_vnum_room; /* OLC */
        assign_area_vnum( vnum );                                    /* OLC */

    }

    return;
}

void new_load_rooms( FILE *fp )
{   
    ROOM_INDEX_DATA *pRoomIndex;
    
    if ( !last_area )
    {
        bug( "Load_rooms: no #AREA seen yet.", 0 );
        exit( 1 ); 
    }
        
    for ( ; ; )
    {
        char letter;
        int  vnum;
        int  door;
        int  iHash;
    
        letter                          = fread_letter( fp );
        if ( letter != '#' )
        {
            bug( "Load_rooms: # not found.", 0 );  
            exit( 1 );
        }       

        vnum                            = fread_number( fp );
        if ( vnum == 0 )
            break;
    
        fBootDb = FALSE;
        if ( get_room_index( vnum ) )
        {
            bug( "Load_rooms: vnum %d duplicated.", vnum );
            exit( 1 );
        }
        fBootDb = TRUE;
        
        pRoomIndex                      = alloc_perm( sizeof( *pRoomIndex ) );
        pRoomIndex->people              = NULL;
        pRoomIndex->contents            = NULL;
        pRoomIndex->first_extradesc     = NULL;
        pRoomIndex->last_extradesc     = NULL;
        pRoomIndex->area                = last_area;
        pRoomIndex->vnum                = vnum;
        pRoomIndex->name                = fread_string( fp );
        pRoomIndex->description         = fread_string( fp );
        fread_number( fp );	// Area number - not used in current implementation
        pRoomIndex->room_flags          = fread_number( fp );
        pRoomIndex->sector_type         = fread_number( fp );
        pRoomIndex->light               = 0;
	pRoomIndex->blood		= 0;
	pRoomIndex->roomtext		= NULL;
	for ( door = 0; door <= 4; door++)
	{
		pRoomIndex->track[door]= str_dup("");
		pRoomIndex->track_dir[door] = 0;
	}
        for ( door = 0; door <= 5; door++ )
            pRoomIndex->exit[door] = NULL;
        
        for ( ; ; )
        {
            letter = fread_letter( fp );
        
            if ( letter == 'S' || letter == 's' )
            {
                if ( letter == 's' )
                    bug( "Load_rooms: vnum %d has lowercase 's'", vnum );
                break;
            }
        
            if ( letter == 'D' )
            {
                EXIT_DATA *pexit;
                int        locks;
        
                door = fread_number( fp );
                if ( door < 0 || door > 5 )
                {
                    bug( "Fread_rooms: vnum %d has bad door number.", vnum );
                    exit( 1 );
                }
        
                pexit                   = alloc_perm( sizeof( *pexit ) );
                pexit->description      = fread_string( fp );
                pexit->keyword          = fread_string( fp );
                locks                   = fread_number( fp );
                pexit->exit_info        = locks;
                pexit->rs_flags         = locks;
                pexit->key              = fread_number( fp );
                pexit->vnum             = fread_number( fp );
             
                
                pRoomIndex->exit[door]          = pexit;
                top_exit++;
            }
            else if ( letter == 'E' )
            {
                EXTRA_DESCR_DATA *ed;
                
                CREATE( ed, EXTRA_DESCR_DATA, 1);
                ed->keyword             = fread_string( fp );
                ed->description         = fread_string( fp );
		LINK( ed, pRoomIndex->first_extradesc, pRoomIndex->last_extradesc, next, prev);
                top_ed++;
            }
            else if ( letter == 'T' )
            {
                ROOMTEXT_DATA *rt;

                rt                      = alloc_perm( sizeof(*rt) );
                rt->input               = fread_string( fp );
                rt->output              = fread_string( fp );
                rt->choutput            = fread_string( fp );
                rt->name                = fread_string( fp );
                rt->type                = fread_number( fp );
                rt->power               = fread_number( fp );
                rt->mob                 = fread_number( fp );
                rt->next                = pRoomIndex->roomtext;
                pRoomIndex->roomtext    = rt;
                top_rt++;
            }

            else 
            {
                bug( "Load_rooms: vnum %d has flag not 'DES'.", vnum );  
                exit( 1 );
            }
        }
                
        iHash                   = vnum % MAX_KEY_HASH;
        pRoomIndex->next        = room_index_hash[iHash];
        room_index_hash[iHash]  = pRoomIndex;
        top_room++;
        top_vnum_room = top_vnum_room < vnum ? vnum : top_vnum_room;
        assign_area_vnum( vnum );
    }
             
    return;
}

/*
 * Snarf a shop section.
 */
void load_shops( FILE *fp )
{
    SHOP_DATA *pShop;

    for ( ; ; )
    {
	MOB_INDEX_DATA *pMobIndex;
	int iTrade;

	pShop			= alloc_perm( sizeof(*pShop) );
	pShop->keeper		= fread_number( fp );
	if ( pShop->keeper == 0 )
	    break;
	for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
	    pShop->buy_type[iTrade]	= fread_number( fp );
	pShop->profit_buy	= fread_number( fp );
	pShop->profit_sell	= fread_number( fp );
	pShop->open_hour	= fread_number( fp );
	pShop->close_hour	= fread_number( fp );
				  fread_to_eol( fp );
	pMobIndex		= get_mob_index( pShop->keeper );
	pMobIndex->pShop	= pShop;

	if ( shop_first == NULL )
	    shop_first = pShop;
	if ( shop_last  != NULL )
	    shop_last->next = pShop;

	shop_last	= pShop;
	pShop->next	= NULL;
	top_shop++;
    }

    return;
}



/*
 * Snarf spec proc declarations.
 */
void load_specials( FILE *fp )
{
    for ( ; ; )
    {
	MOB_INDEX_DATA *pMobIndex;
	char letter;

	switch ( letter = fread_letter( fp ) )
	{
	default:
	    bug( "Load_specials: letter '%c' not *MS.", letter );
	    exit( 1 );

	case 'S':
	    return;

	case '*':
	    break;

	case 'M':
	    pMobIndex		= get_mob_index	( fread_number ( fp ) );
	    pMobIndex->spec_fun	= spec_lookup	( fread_word   ( fp ) );
	    if ( pMobIndex->spec_fun == 0 )
	    {
		bug( "Load_specials: 'M': vnum %d.", pMobIndex->vnum );
	    }
	    break;
	}

	fread_to_eol( fp );
    }
}


/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits( void )
{
    extern const sh_int rev_dir [];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    EXIT_DATA *pexit_rev;
    int iHash;
    int door;

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for ( pRoomIndex  = room_index_hash[iHash];
	      pRoomIndex != NULL;
	      pRoomIndex  = pRoomIndex->next )
	{
	    bool fexit;

	    fexit = FALSE;
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( ( pexit = pRoomIndex->exit[door] ) != NULL )
		{
		    fexit = TRUE;
		    if ( pexit->vnum <= 0 )
			pexit->to_room = NULL;
		    else
			pexit->to_room = get_room_index( pexit->vnum );
		}
	    }

	    if ( !fexit )
		SET_BIT( pRoomIndex->room_flags, ROOM_NO_MOB );
	}
    }

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for ( pRoomIndex  = room_index_hash[iHash];
	      pRoomIndex != NULL;
	      pRoomIndex  = pRoomIndex->next )
	{
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( ( pexit     = pRoomIndex->exit[door]       ) != NULL
		&&   ( to_room   = pexit->to_room               ) != NULL
		&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
		&&   pexit_rev->to_room != pRoomIndex )
		{
		    sprintf( buf, "Fix_exits: %d:%d -> %d:%d -> %d.",
			pRoomIndex->vnum, door,
			to_room->vnum,    rev_dir[door],
			(pexit_rev->to_room == NULL)
			    ? 0 : pexit_rev->to_room->vnum );
		}
	    }
	}
    }

    return;
}



/*
 * Repopulate areas periodically.
 */
void area_update( bool ForceReset )
{
    AREA_DATA *pArea;

    for ( pArea = first_area; pArea != NULL; pArea = pArea->next )
    {
	CHAR_DATA *pch;

	if ( ++pArea->age < 3 && !ForceReset )
	    continue;

	/*
	 * Check for PC's.
	 */
	if ( pArea->nplayer > 0 && pArea->age == 30 - 1 )
	{
	    for ( pch = char_list; pch != NULL; pch = pch->next )
	    {
		if ( !IS_NPC(pch) && IS_AWAKE(pch) && pch->in_room != NULL && pch->in_room->area == pArea )
		    send_to_char( "You hear the sound of a bell in the distance.\n\r", pch );
	    }
	}

	/*
	 * Check age and reset.
	 * Note: Mud School resets every 3 minutes (not 15).
	 */
	if ( ForceReset || pArea->age >= 30 )
	{
	    ROOM_INDEX_DATA *pRoomIndex;
	    reset_area( pArea );
	    pArea->age = (30 - pArea->perm_age);
	    pRoomIndex = get_room_index( ROOM_VNUM_SCHOOL );
	    if ( pRoomIndex != NULL && pArea == pRoomIndex->area )
		pArea->age = 30 - 3;

	}
    }

    return;
}

/* OLC
 * Reset one room.  Called by reset_area and olc.
 */
void reset_room( ROOM_INDEX_DATA *pRoom )
{
    RESET_DATA  *pReset;
    CHAR_DATA   *pMob;
    OBJ_DATA    *pObj;
    CHAR_DATA   *LastMob = NULL;
    OBJ_DATA    *LastObj = NULL;
    int iExit;
    int level = 0;
    bool last;
    if ( !pRoom )
        return;
   
    pMob        = NULL;
    last        = FALSE;
            
    for ( iExit = 0;  iExit < MAX_DIR;  iExit++ )
    {
        EXIT_DATA *pExit;
        if ( ( pExit = pRoom->exit[iExit] ) )
        {
            pExit->exit_info = pExit->rs_flags;
            if ( ( pExit->to_room != NULL )
              && ( ( pExit = pExit->to_room->exit[rev_dir[iExit]] ) ) )
            {
                pExit->exit_info = pExit->rs_flags;
            }
        }
    }
    
    for ( pReset = pRoom->reset_first; pReset != NULL; pReset = pReset->next )
    {
        MOB_INDEX_DATA  *pMobIndex;
        OBJ_INDEX_DATA  *pObjIndex;
        OBJ_INDEX_DATA  *pObjToIndex;
        ROOM_INDEX_DATA *pRoomIndex;
   
        switch ( pReset->command )
        {
        default:
                bug( "Reset_room: bad command %c.", pReset->command );
                break;
        
        case 'M':
            if ( !( pMobIndex = get_mob_index( pReset->arg1 ) ) )
            {
                bug( "Reset_room: 'M': bad vnum %d.", pReset->arg1 );
                continue;
            }
             
            /*
             * Some hard coding.
             */
    
            if ( pMobIndex->count >= pReset->arg2 )
            {
                last = FALSE;
                break;
            }
        
            pMob = create_mobile( pMobIndex );
        
            /*
             * Some more hard coding.
             */
            if ( room_is_dark( pRoom ) )
                SET_BIT(pMob->affected_by, AFF_INFRARED);
        
            /*
             * Pet shop mobiles get ACT_PET set.
             */
            {
                ROOM_INDEX_DATA *pRoomIndexPrev;
             
                pRoomIndexPrev = get_room_index( pRoom->vnum - 1 );
                if ( pRoomIndexPrev
                    && IS_SET( pRoomIndexPrev->room_flags, ROOM_PET_SHOP ) )
                    xSET_BIT( pMob->act, ACT_PET);
            }
    
            repop_char_to_room( pMob, pRoom );
            LastMob = pMob;  
            level  = URANGE( 0, pMob->level - 2, LEVEL_HERO );
            last = TRUE;
            break;
            
        case 'O':
            if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
            {
                bug( "Reset_room: 'O': bad vnum %d.", pReset->arg1 );
                continue;
            }
        
            if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
            {
                bug( "Reset_room: 'O': bad vnum %d.", pReset->arg3 );
                continue;
            }
             
            if ( pRoom->area->nplayer > 0
              || count_obj_list( pObjIndex, pRoom->contents ) > 0 )
                break;
                    
            pObj = create_object( pObjIndex, number_fuzzy( level ) );
            pObj->cost = 0;
            obj_to_room( pObj, pRoom ); 
            break;
            
        case 'P':
            if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
            {
                bug( "Reset_room: 'P': bad vnum %d.", pReset->arg1 );
                continue;
            }
            if ( !( pObjToIndex = get_obj_index( pReset->arg3 ) ) )  
            {
                bug( "Reset_room: 'P': bad vnum %d.", pReset->arg3 );
                continue;
            }
             
            if ( pRoom->area->nplayer > 0
              || !( LastObj = get_obj_type( pObjToIndex ) )
              || count_obj_list( pObjIndex, LastObj->contains ) > 0 )
                break;
            
            pObj = create_object( pObjIndex, number_fuzzy( level ) );
            obj_to_obj( pObj, LastObj );
                    
            /*
             * Ensure that the container gets reset.    OLC 1.1b
             */
            if ( LastObj->item_type == ITEM_CONTAINER )
            {
                LastObj->value[1] = LastObj->pIndexData->value[1];
            }
            // Container reset handled above for ITEM_CONTAINER type
            break;
            
        case 'G':
        case 'E':
            if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
            {
                bug( "Reset_room: 'E' or 'G': bad vnum %d.", pReset->arg1 );
                continue;
            }
              
            if ( !last )
                break;
            
            if ( !LastMob )
            {       
                bug( "Reset_room: 'E' or 'G': null mob for vnum %d.",
                    pReset->arg1 );
                last = FALSE;
                break;
            }
                
            if ( LastMob->pIndexData->pShop )   /* Shop-keeper? */
            {   
                int olevel;
                    
                switch ( pObjIndex->item_type )
                { 
                default:                olevel = 0;                      break;
                case ITEM_PILL:         olevel = number_range(  0, 10 ); break;
                case ITEM_POTION:       olevel = number_range(  0, 10 ); break;
                case ITEM_SCROLL:       olevel = number_range(  5, 15 ); break;
                case ITEM_WAND:         olevel = number_range( 10, 20 ); break;
                case ITEM_STAFF:        olevel = number_range( 15, 25 ); break;
                case ITEM_ARMOR:        olevel = number_range(  5, 15 ); break;
                case ITEM_WEAPON:       if ( pReset->command == 'G' )
                                            olevel = number_range( 5, 15 );
                                        else
                                            olevel = number_fuzzy( level );
                  break;
                }
              
		if ( pObjIndex->vnum == 29603 && CHAOS ) return;
		if ( pObjIndex->vnum == 29603 ) CHAOS = TRUE;      

                pObj = create_object( pObjIndex, olevel );
                if ( pReset->command == 'G' )
                    SET_BIT( pObj->extra_flags, ITEM_INVENTORY );
            }
            else
            {   
                pObj = create_object( pObjIndex, number_fuzzy( level ) );
            }   
            obj_to_char( pObj, LastMob );
            if ( pReset->command == 'E' )
                equip_char( LastMob, pObj, pReset->arg3 );
            last = TRUE;
            break;
                
        case 'D':
            break;
                
        case 'R':
            break;
        }
    }
                
    return;
}


/* OLC
 * Reset one area.
 */
void reset_area( AREA_DATA *pArea )
{
    ROOM_INDEX_DATA *pRoom;
    int  vnum;
             
    for ( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
        if ( ( pRoom = get_room_index(vnum) ) )
            reset_room(pRoom);

    }
	return;
}

/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile( MOB_INDEX_DATA *pMobIndex )
{
    CHAR_DATA *mob;
    int tempvalue;

    if ( pMobIndex == NULL )
    {
	bug( "Create_mobile: NULL pMobIndex.", 0 );
	exit( 1 );
    }

    if ( char_free == NULL )
    {
	mob		= alloc_perm( sizeof(*mob) );
    }
    else
    {
	mob		= char_free;
	char_free	= char_free->next;
    }

    clear_char( mob );
    mob->pIndexData	= pMobIndex;

    mob->hunting	= str_dup( "" );
    mob->lord		= str_dup( "" );
    mob->morph		= str_dup( "" );
    mob->createtime	= str_dup( "" );
    mob->lasttime	= str_dup( "" );
    mob->lasthost	= str_dup( "" );
    mob->powertype	= str_dup( "" );
    mob->poweraction	= str_dup( "" );
    mob->pload		= str_dup( "" );
    mob->prompt		= str_dup( "" );
    mob->cprompt	= str_dup( "" );
    xCLEAR_BITS(mob->extra);
    mob->name           = str_dup( pMobIndex->player_name );    /* OLC */
    mob->short_descr    = str_dup( pMobIndex->short_descr );    /* OLC */
    mob->long_descr     = str_dup( pMobIndex->long_descr );     /* OLC */
    mob->description    = str_dup( pMobIndex->description );    /* OLC */

    mob->spec_fun	= pMobIndex->spec_fun;

    mob->home		= 3001;
    mob->form		= 32767;
    mob->level		= number_fuzzy( pMobIndex->level );
    mob->act		= pMobIndex->act;
    mob->affected_by	= pMobIndex->affected_by;
    mob->alignment	= pMobIndex->alignment;
    mob->sex		= pMobIndex->sex;

    mob->armor		= interpolate( mob->level, 100, -100 );

    tempvalue		= mob->level * 100;
    if(tempvalue > (mob->level * 100) || tempvalue < 0) 
	mob->max_hit = 10000000;
    else mob->max_hit	= tempvalue;

    mob->hit		= mob->max_hit;

    mob->hitroll	= mob->level;
    mob->damroll	= mob->level;
    mob->practice   = mob->level * (number_range(10,20)/10);
    mob->gold		= mob->level * (number_range(100,500));
    if (mob->gold > 10000000) mob->gold = 10000000;
    if ( xIS_SET(mob->act, ACT_NOEXP))  mob->gold = 0;

    // Random forge object generation

   if (number_percent() > 90)
   {

	OBJ_DATA *obj;
        int ph;
           
        ph = number_range( 1,2 );
        if( ph == 1 ) 
	{
  	  obj = create_object( get_obj_index( number_range( 30049,30071 ) ), 0 );
	  if( obj->pIndexData->vnum == 30052 )
	  {
	   if( number_range(1,10)==1 )
		obj_to_char(obj,mob);
           else
                extract_obj(obj);
          
	  }
	  else
	    obj_to_char(obj,mob);

   	}
   	else
   	{
          obj = create_object( get_obj_index( 8 ), 0);
          if( obj->pIndexData->vnum == 8 )
	  {
             if( number_range(1,200)==1 )
                obj_to_char(obj,mob);
             else
                extract_obj(obj);
	  }
        }
   }
    /*
     * Insert in list.
     */
    mob->next		= char_list;
    char_list		= mob;
    pMobIndex->count++;
    return mob;
}



/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object( OBJ_INDEX_DATA *pObjIndex, int level )
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;

    if ( pObjIndex == NULL )
    {
	bug( "Create_object: NULL pObjIndex.", 0 );
	exit( 1 );
    }

    if ( obj_free == NULL )
    {
	obj		= alloc_perm( sizeof(*obj) );
    }
    else
    {
	obj		= obj_free;
	obj_free	= obj_free->next;
    }

    *obj		= obj_zero;
    obj->pIndexData	= pObjIndex;
    obj->in_room	= NULL;
    obj->level		= level;
    obj->wear_loc	= -1;

    obj->name           = str_dup( pObjIndex->name );           /* OLC */
    obj->short_descr    = str_dup( pObjIndex->short_descr );    /* OLC */
    obj->description    = str_dup( pObjIndex->description );    /* OLC */
 
    if (pObjIndex->chpoweron != NULL)
    {
	obj->chpoweron    = pObjIndex->chpoweron;
	obj->chpoweroff   = pObjIndex->chpoweroff;
	obj->chpoweruse   = pObjIndex->chpoweruse;
	obj->victpoweron  = pObjIndex->victpoweron;
	obj->victpoweroff = pObjIndex->victpoweroff;
	obj->victpoweruse = pObjIndex->victpoweruse;
	obj->spectype     = pObjIndex->spectype;
	obj->specpower    = pObjIndex->specpower;
    }
    else
    {
	obj->chpoweron    = str_dup( "(null)" );
	obj->chpoweroff   = str_dup( "(null)" );
	obj->chpoweruse   = str_dup( "(null)" );
	obj->victpoweron  = str_dup( "(null)" );
	obj->victpoweroff = str_dup( "(null)" );
	obj->victpoweruse = str_dup( "(null)" );
	obj->spectype     = 0;
	obj->specpower    = 0;
    }
    obj->questmaker     = str_dup( "" );
    obj->questowner     = str_dup( "" );

    obj->chobj		= NULL;

    obj->quest		= 0;
    obj->points		= 0;

    obj->item_type	= pObjIndex->item_type;
    obj->extra_flags	= pObjIndex->extra_flags;
    obj->wear_flags	= pObjIndex->wear_flags;
    obj->value[0]	= pObjIndex->value[0];
    obj->value[1]	= pObjIndex->value[1];
    obj->value[2]	= pObjIndex->value[2];
    obj->value[3]	= pObjIndex->value[3];
    obj->weight		= pObjIndex->weight;
    obj->cost		= number_fuzzy( 10 )
			* number_fuzzy( level ) * number_fuzzy( level );
  
  if (obj->pIndexData->vnum == 30039)  /* Questcards */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    SET_BIT(obj->quest,QUEST_RELIC);
  }
  if (obj->pIndexData->vnum == 30000)
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    SET_BIT(obj->quest,QUEST_RELIC);
  }

  else if (obj->pIndexData->vnum == 420009)
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->timer = 100;
    SET_BIT(obj->quest,QUEST_RELIC);
  }

	
  else if (obj->pIndexData->vnum >= 2205 && obj->pIndexData->vnum <= 2222)
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    SET_BIT(obj->quest,QUEST_NEWBIE);
  }

  else if (obj->pIndexData->vnum == 33342) /* Paladin Mastery */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    SET_BIT(obj->quest,QUEST_RELIC);
  }
  else if (obj->pIndexData->vnum == 33341 || obj->pIndexData->vnum == 33340) /* Inquisitor Mastery */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    SET_BIT(obj->quest,QUEST_RELIC);
  }


  else if (obj->pIndexData->vnum == 420000 || obj->pIndexData->vnum == 420002 || obj->pIndexData->vnum == 420004) /* Guardian of the Portal */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }
  else if (obj->pIndexData->vnum == 100928) /* King Lot */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }
  else if (obj->pIndexData->vnum == 41046) /* Guardian of Immortality*/
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }

  else if (obj->pIndexData->vnum == 101400) /* Guardian Ettin*/
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }
  else if (obj->pIndexData->vnum == 41048) /* Banker Earring*/
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RELIC);
  }
  else if (obj->pIndexData->vnum == 420006) /* Temp Tattoo (exp) */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    obj->timer	    =  number_range(30,60);
    SET_BIT(obj->quest,QUEST_RELIC);
  }
  else if (obj->pIndexData->vnum == 420007) /* Temp Tattoo (+500 damcap)*/
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    obj->timer      =  60;
    SET_BIT(obj->quest,QUEST_RELIC);
  }
  else if (obj->pIndexData->vnum == 420008) /* Temp Tattoo (+1000 damcap)*/
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    obj->timer      =  60;
    SET_BIT(obj->quest,QUEST_RELIC);
  }



  else if (obj->pIndexData->vnum == 101404) /* Ettin Wizard */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }
    else if (obj->pIndexData->vnum == 101505) /* Guardian Whore */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }

  else if (obj->pIndexData->vnum == 101503) /* Guardian Beast */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }



  else if (obj->pIndexData->vnum == 101401) /* Ettin Shaman */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }
  else if (obj->pIndexData->vnum == 101403) /* Ettin Commander */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }


  else if (obj->pIndexData->vnum == 220004) /* Guardian of the Ways */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }
  else if ((obj->pIndexData->vnum == 101500) || (obj->pIndexData->vnum == 101502) ||
(obj->pIndexData->vnum == 101501))
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }

  else if (obj->pIndexData->vnum == 100100 || obj->pIndexData->vnum == 100101 || 
obj->pIndexData->vnum == 100102 || obj->pIndexData->vnum == 100103) /* Tower Guardian EQ*/
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RUNE);
  }

  else if (obj->pIndexData->vnum >=  33960 && obj->pIndexData->vnum <= 33999)  /* artifacts 
*/
  {
    SET_BIT(obj->quest, QUEST_ARTIFACT);
    obj->condition  =  100;
    obj->toughness  =  100;
    obj->resistance =  1;
    obj->level      =  60;
    obj->cost       =  1000000;
  }
  else if (obj->pIndexData->vnum >= 33180 && obj->pIndexData->vnum <= 33199)  /* Angels */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RELIC);
  }
  else if (obj->pIndexData->vnum >= 33200 && obj->pIndexData->vnum <= 33219)  /* Tanar'ri */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RELIC);
  }
  else if (obj->pIndexData->vnum >= 33220 && obj->pIndexData->vnum <= 33240)  /* Lich */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->quest,QUEST_RELIC);
  }
  else if (obj->pIndexData->vnum >= 33060 && obj->pIndexData->vnum <= 33079)  /* drow */
  {
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
    SET_BIT(obj->spectype,SITEM_DROW);
    SET_BIT(obj->quest,QUEST_RELIC);
  }
  else if( obj->pIndexData->vnum >= 33000 && obj->pIndexData->vnum <= 33019)  /* mage */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    SET_BIT(obj->spectype, SITEM_MAGE);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if( obj->pIndexData->vnum >= 33343 && obj->pIndexData->vnum <= 33356)  /* psionist */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }

  else if( obj->pIndexData->vnum >= 33160 && obj->pIndexData->vnum <= 33179)  /* Shapeshifter */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if( obj->pIndexData->vnum >= 29975 && obj->pIndexData->vnum <= 29991)  /* Death Knights */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if( obj->pIndexData->vnum >= 33140 && obj->pIndexData->vnum <= 33159)  /* Spider Droid */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if (obj->pIndexData->vnum >= 33120  && obj->pIndexData->vnum <= 33139)  /* demon */
  {
    SET_BIT(obj->spectype, SITEM_DEMONIC);
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition  = 100;
    obj->toughness  = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if( obj->pIndexData->vnum >= 33040 && obj->pIndexData->vnum <= 33059)  /* vampire */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if((obj->pIndexData->vnum >= 780 && obj->pIndexData->vnum <= 808)) /* Holy EQ */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if(obj->pIndexData->vnum >= 869 && obj->pIndexData->vnum <= 894) /* Mithril EQ */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if((obj->pIndexData->vnum >= 842 && obj->pIndexData->vnum <= 867)) /* Chaos EQ */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if((obj->pIndexData->vnum >= 814 && obj->pIndexData->vnum <= 825)) /* ADAMANTIUM EQ */
  {
    SET_BIT(obj->quest, QUEST_ADAMANTIUM);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }

  else if( obj->pIndexData->vnum >= 33020 && obj->pIndexData->vnum <= 33039)  /* monk */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if( obj->pIndexData->vnum >= 33080 && obj->pIndexData-> vnum <= 33099)  /* ninja */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if( obj->pIndexData->vnum >= 33100 && obj->pIndexData-> vnum <= 33119)  /* ww */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }
  else if( obj->pIndexData->vnum >= 27 && obj->pIndexData-> vnum <= 42)  /* class eq */
  {
    SET_BIT(obj->quest, QUEST_RELIC);
    obj->condition = 100;
    obj->toughness = 100;
    obj->resistance = 1;
    obj->level      =  60;
  }

  else
  {
    obj->condition  =  100;
    obj->toughness  =  5;
    obj->resistance =  25;
  }

    /*
     * Mess with object properties.
     */
    switch ( obj->item_type )
    {
    default:
	bug( "Read_object: vnum %d bad type.", pObjIndex->vnum );
	break;

    case ITEM_LIGHT:
    case ITEM_TREASURE:
    case ITEM_INSTRUMENT:
    case ITEM_FURNITURE:
    case ITEM_TRASH:
    case ITEM_CONTAINER:
    case ITEM_DRINK_CON:
    case ITEM_KEY:
    case ITEM_FOOD:
    case ITEM_BOAT:
    case ITEM_CORPSE_NPC:
    case ITEM_CORPSE_PC:
    case ITEM_FOUNTAIN:
    case ITEM_PORTAL:
    case ITEM_EGG:
    case ITEM_VOODOO:
    case ITEM_STAKE:
    case ITEM_MISSILE:
    case ITEM_AMMO:
    case ITEM_QUEST:
    case ITEM_QUESTCARD:
    case ITEM_QUESTMACHINE:
    case ITEM_SYMBOL:
    case ITEM_BOOK:
    case ITEM_PAGE:
    case ITEM_TOOL:
    case ITEM_COPPER:
    case ITEM_IRON:
    case ITEM_STEEL:
    case ITEM_ADAMANTITE:
    case ITEM_GEMSTONE:
    case ITEM_ELECTRUM:
    case ITEM_HILT:
    case ITEM_SCROLL:
    case ITEM_WAND:
    case ITEM_STAFF:
    case ITEM_BANK:
    case ITEM_WELL:
    case ITEM_STATUE:
    case ITEM_TELEPORTER:
    case ITEM_IOU:
    case ITEM_MACHINE:
    case ITEM_EXP_TOKEN:
	break;


    case ITEM_WEAPON:
	if (!IS_SET(obj->quest, QUEST_ARTIFACT)
	    && !IS_SET(obj->quest, QUEST_RELIC))
	{
	    obj->value[1]	= number_range( obj->value[1], obj->value[2] );
	    obj->value[2]	= number_range((obj->value[1]+1),(obj->value[1]*2));
	}
	break;

    case ITEM_ARMOR:
	if (!IS_SET(obj->quest, QUEST_ARTIFACT)
	    && !IS_SET(obj->quest, QUEST_RELIC))
	    obj->value[0]	= number_range( 10, obj->value[0] );
	break;

    case ITEM_POTION:
    case ITEM_PILL:
	obj->value[0]	= number_fuzzy( number_fuzzy( obj->value[0] ) );
	break;

    case ITEM_MONEY:
	obj->cost = obj->value[0];
	break;
    }

    obj->next		= object_list;
    object_list		= obj;
    pObjIndex->count++;

    return obj;
}



/*
 * Clear a new character.
 */
void clear_char( CHAR_DATA *ch )
{
    static CHAR_DATA ch_zero;

    *ch				= ch_zero;
    ch->name			= &str_empty[0];
    ch->short_descr		= &str_empty[0];
    ch->long_descr		= &str_empty[0];
    ch->description		= &str_empty[0];
    ch->lord			= &str_empty[0];
    ch->morph			= &str_empty[0];
    ch->createtime		= &str_empty[0];
    ch->lasthost		= &str_empty[0];
    ch->lasttime		= &str_empty[0];
    ch->powertype		= &str_empty[0];
    ch->poweraction		= &str_empty[0];
    ch->pload			= &str_empty[0];
    ch->prompt			= &str_empty[0];
    ch->cprompt			= &str_empty[0];
    ch->hunting			= &str_empty[0];

    ch->logon			= current_time;
    ch->armor			= 100;
    ch->position		= POS_STANDING;
    ch->practice		= 0;
    ch->hit			= 1000;
    ch->max_hit			= 1000;
    ch->mana			= 1500;
    ch->max_mana		= 1500;
    ch->move			= 1500;
    ch->max_move		= 1500;
    ch->master			= NULL;
    ch->leader			= NULL;
    ch->fighting		= NULL;
    ch->mount			= NULL;
    ch->wizard			= NULL;
    ch->paradox[0]              = 0;        
    ch->paradox[1]              = 0;        
    ch->paradox[2]              = 0;        
    ch->damcap[0]		= 1000;
    ch->damcap[1]		= 0;
    ch->last_cmd		= NULL;
    ch->substate		= 0;
    return;
}



/*
 * Free a character.
 */
void free_char( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    ALIAS_DATA *ali;
    ALIAS_DATA *ali_next;

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	extract_obj( obj );
    }

    while ( (ch->last_affect) )
        affect_remove( ch, ch->last_affect );

    free_string( ch->name		);
    free_string( ch->short_descr	);
    free_string( ch->long_descr		);
    free_string( ch->description	);
    free_string( ch->lord 		);
    free_string( ch->morph 		);
    free_string( ch->createtime 	);
    free_string( ch->lasttime 		);
    free_string( ch->lasthost 		);
    free_string( ch->powertype 		);
    free_string( ch->poweraction 	);
    free_string( ch->pload 		);
    free_string( ch->prompt 		);
    free_string( ch->cprompt 		);
    free_string( ch->hunting		);
        
    if ( ch->pcdata != NULL )
    {
      for (ali = ch->pcdata->first_alias; ali; ali = ali_next)
      {
        ali_next = ali->next;
        alias_remove(ch, ali);
      }
	free_string( ch->pcdata->pwd		);
	free_string( ch->pcdata->bamfin		);
	free_string( ch->pcdata->bamfout	);
	free_string( ch->pcdata->title		);
	free_string( ch->pcdata->conception	);
	free_string( ch->pcdata->parents	);
	free_string( ch->pcdata->cparents	);
	free_string( ch->pcdata->marriage	);
	free_string( ch->pcdata->email		);
	ch->pcdata->last_change = 0;
	DISPOSE( ch->pcdata );

    }

    ch->next	     = char_free;
    char_free	     = ch;
    return;
}



/*
 * Get an extra description from a list.
 */
char *get_extra_descr( char *name, EXTRA_DESCR_DATA *ed )
{
    for ( ; ed != NULL; ed = ed->next )
    {
	if ( is_name( name, ed->keyword ) )
	    return ed->description;
    }
    return NULL;
}



/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index( int vnum )
{
    MOB_INDEX_DATA *pMobIndex;

    for ( pMobIndex  = mob_index_hash[vnum % MAX_KEY_HASH];
	  pMobIndex != NULL;
	  pMobIndex  = pMobIndex->next )
    {
	if ( pMobIndex->vnum == vnum )
	    return pMobIndex;
    }

    if ( fBootDb )
    {
	bug( "Get_mob_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index( int vnum )
{
    OBJ_INDEX_DATA *pObjIndex;

    for ( pObjIndex  = obj_index_hash[vnum % MAX_KEY_HASH];
	  pObjIndex != NULL;
	  pObjIndex  = pObjIndex->next )
    {
	if ( pObjIndex->vnum == vnum )
	    return pObjIndex;
    }

    if ( fBootDb )
    {
	bug( "Get_obj_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( int vnum )
{
    ROOM_INDEX_DATA *pRoomIndex;

    for ( pRoomIndex  = room_index_hash[vnum % MAX_KEY_HASH];
	  pRoomIndex != NULL;
	  pRoomIndex  = pRoomIndex->next )
    {
	if ( pRoomIndex->vnum == vnum )
	    return pRoomIndex;
    }

    if ( fBootDb )
    {
	bug( "Get_room_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Read a letter from a file.
 */
char fread_letter( FILE *fp )
{
    char c;

    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    return c;
}

/*
 * Read a number from a file.
 */
int fread_number( FILE *fp )
{
    int number;
    bool sign;
    char c;

    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    number = 0;

    sign   = FALSE;

    if ( c == '+' )
    {
	c = getc( fp );
    }
    else if ( c == '-' )
    {
	sign = TRUE;
	c = getc( fp );
    }

    if ( !isdigit(c) )
    {
	bug( "Fread_number: bad format, %c, db.c", c );
	exit( 1 );
    }

    while ( isdigit(c) )
    {
	number = number * 10 + c - '0';
	c      = getc( fp );
    }

    if ( sign )
	number = 0 - number;

    if ( c == '|' )
	number += fread_number( fp );
    else if ( c != ' ' )
	ungetc( c, fp );

    return number;
}



/*
 * Read and allocate space for a string from a file.
  * These strings are read-only and shared.
 * Strings are hashed:
 *   each string prepended with hash pointer to prev string,
 *   hash code is simply the string length.
 * This function takes 40% to 50% of boot-up time.
 */
char *fread_string( FILE *fp )
{
    char *plast;
    char c;

    plast = top_string + sizeof(char *);
    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )
    {
	bug( "Fread_string: MAX_STRING %d exceeded.", MAX_STRING );
	exit( 1 );
    }

    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    if ( ( *plast++ = c ) == '~' )
	return &str_empty[0];
    for ( ;; )
    {
	/*
	 * Back off the char type lookup,
	 *   it was too dirty for portability.
	 *   -- Furey
	 */
	switch ( *plast = getc( fp ) )
	{
	default:
	    plast++;
	    break;

	case EOF:
	    bug( "Fread_string: EOF", 0 );
	    exit( 1 );
	    break;

	case '\n':
	    plast++;
	    *plast++ = '\r';
	    break;

	case '\r':
	    break;

	case '~':
	    plast++;
	    {
		union
		{
		    char *	pc;
		    char	rgc[sizeof(char *)];
		} u1;
		int ic;
		int iHash;
		char *pHash;
		char *pHashPrev;
		char *pString;

		plast[-1] = '\0';
		iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );
		for ( pHash = db_string_hash[iHash]; pHash; pHash = pHashPrev )
		{
		    for ( ic = 0; ic < sizeof(char *); ic++ )
			u1.rgc[ic] = pHash[ic];
		    pHashPrev = u1.pc;
		    pHash    += sizeof(char *);

		    if ( top_string[sizeof(char *)] == pHash[0]
		    &&   !strcmp( top_string+sizeof(char *)+1, pHash+1 ) )
			return pHash;
		}

		if ( fBootDb )
		{
		    pString		= top_string;
		    top_string		= plast;
		    u1.pc		= db_string_hash[iHash];
		    for ( ic = 0; ic < sizeof(char *); ic++ )
			pString[ic] = u1.rgc[ic];
		    db_string_hash[iHash]	= pString;

		    nAllocString += 1;
		    sAllocString += top_string - pString;
		    return pString + sizeof(char *);
		}
		else
		{
		    return str_dup( top_string + sizeof(char *) );
		}
	    }
	}
    }
}



// Legacy fread_to_eol function removed - replaced with safer version below

void fread_to_eol( FILE *fp )
{
    char c;

    do
    {
	if ( feof(fp) )
	{
	    bug("fread_to_eol: EOF encountered on read.\n\r");
	    if ( fBootDb )
		exit(1);
	    return;
	}
	c = getc( fp );
    }
    while ( c != '\n' && c != '\r' );

    do
    {
	c = getc( fp );
    }
    while ( c == '\n' || c == '\r' );

    ungetc( c, fp );
    return;
}


// Legacy fread_word function removed - replaced with safer EOF-checking version below

/*
 * Read one word (into static buffer).
 */
char *fread_word( FILE *fp )
{
    static char word[MAX_INPUT_LENGTH];
    char *pword;
    char cEnd;

    do
    {
	if ( feof(fp) )
	{
	    bug("fread_word: EOF encountered on read.\n\r");
	    if ( fBootDb )
		exit(1);
	    word[0] = '\0';
	    return word;
	}
	cEnd = getc( fp );
    }
    while ( isspace( cEnd ) );

    if ( cEnd == '\'' || cEnd == '"' )
    {
	pword   = word;
    }
    else
    {
	word[0] = cEnd;
	pword   = word+1;
	cEnd    = ' ';
    }

    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )
    {
	if ( feof(fp) )
	{
	    bug("fread_word: EOF encountered on read.\n\r");
	    if ( fBootDb )
		exit(1);
	    *pword = '\0';
	    return word;
	}
	*pword = getc( fp );
	if ( cEnd == ' ' ? isspace(*pword) : *pword == cEnd )
	{
	    if ( cEnd == ' ' )
		ungetc( *pword, fp );
	    *pword = '\0';
	    return word;
	}
    }

    bug( "Fread_word: word too long" );
    exit( 1 );
    return NULL;
}



/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *alloc_mem( int sMem )
{
    void *pMem;
    int iList;

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
	if ( sMem <= rgSizeList[iList] )
	    break;
    }

    if ( iList == MAX_MEM_LIST )
    {
	bug( "Alloc_mem: size %d too large.", sMem );
	exit( 1 );
    }

    if ( rgFreeList[iList] == NULL )
    {
	pMem		  = alloc_perm( rgSizeList[iList] );
    }
    else
    {
	pMem              = rgFreeList[iList];
	rgFreeList[iList] = * ((void **) rgFreeList[iList]);
    }

    return pMem;
}



/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void free_mem( void *pMem, int sMem )
{
    int iList;

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
	if ( sMem <= rgSizeList[iList] )
	    break;
    }

    if ( iList == MAX_MEM_LIST )
    {
	bug( "Free_mem: size %d too large.", sMem );
	exit( 1 );
    }

    * ((void **) pMem) = rgFreeList[iList];
    rgFreeList[iList]  = pMem;

    return;
}



/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *alloc_perm( int sMem )
{
    static char *pMemPerm;
    static int iMemPerm;
    void *pMem;

    while ( sMem % sizeof(long) != 0 )
	sMem++;
    if ( sMem > MAX_PERM_BLOCK )
    {
	bug( "Alloc_perm: %d too large.", sMem );
	exit( 1 );
    }

    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK )
    {
	iMemPerm = 0;
	if ( ( pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) == NULL )
	{
	    perror( "Alloc_perm" );
	    exit( 1 );
	}
    }

    pMem        = pMemPerm + iMemPerm;
    iMemPerm   += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;
    return pMem;
}



/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
char *str_dup( char const *str )
{
    static char *str_new;

    if (!str)
    {
       bug("str_dup lacking a str", 0);
       return "ERROR";
    }

    if ( str[0] == '\0' )
	return &str_empty[0];

    CREATE(str_new, char, strlen(str)+1);
    strcpy( str_new, str );
    return str_new;
}

char *tst_str_dup( char const *str )
{
    static char *ret;
    int len;

    if ( !str )
        return NULL;

    len = strlen(str)+1;

    CREATE( ret, char, len );
    strcpy( ret, str );
    return ret;
}


/*
 * Free a string.
 * Null is legal here to simplify callers.
 * Read-only shared strings are not touched.
 */
void free_string( char *pstr )
{
    if ( pstr == NULL
    ||   pstr == &str_empty[0]
    || ( pstr >= string_space && pstr < top_string ) )
	return;

    free_mem( pstr, strlen(pstr) + 1 );
    return;
}





void do_areas( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    AREA_DATA *pArea1;
    AREA_DATA *pArea2;
    int iArea;
    int iAreaHalf;
    WAIT_STATE(ch,10);
    iAreaHalf = (top_area + 1) / 2;
    pArea1    = first_area;
    pArea2    = first_area;
    for ( iArea = 0; iArea < iAreaHalf; iArea++ )
	pArea2 = pArea2->next;

    
   for ( iArea = 0; iArea < iAreaHalf; iArea++ )
    {
	sprintf( buf, "%-39s%-39s\n\r",
	    pArea1->name, (pArea2 != NULL) ? pArea2->name : "" );
	send_to_char( buf, ch );
	pArea1 = pArea1->next;
	if ( pArea2 != NULL )
	    pArea2 = pArea2->next;
    }

    return;
}



void do_memory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    sprintf( buf, "Affects %5d\n\r", top_affect    ); send_to_char( buf, ch );
    sprintf( buf, "Areas   %5d\n\r", top_area      ); send_to_char( buf, ch );
    sprintf( buf, "RmTxt   %5d\n\r", top_rt        ); send_to_char( buf, ch );
    sprintf( buf, "ExDes   %5d\n\r", top_ed        ); send_to_char( buf, ch );
    sprintf( buf, "Exits   %5d\n\r", top_exit      ); send_to_char( buf, ch );
    sprintf( buf, "Helps   %5d\n\r", top_help      ); send_to_char( buf, ch );
    sprintf( buf, "Mobs    %5d\n\r", top_mob_index ); send_to_char( buf, ch );
    sprintf( buf, "Objs    %5d\n\r", top_obj_index ); send_to_char( buf, ch );
    sprintf( buf, "Resets  %5d\n\r", top_reset     ); send_to_char( buf, ch );
    sprintf( buf, "Rooms   %5d\n\r", top_room      ); send_to_char( buf, ch );
    sprintf( buf, "Shops   %5d\n\r", top_shop      ); send_to_char( buf, ch );

    sprintf( buf, "Strings %5d strings of %7d bytes (max %d).\n\r",
	nAllocString, sAllocString, MAX_STRING );
    send_to_char( buf, ch );

    sprintf( buf, "Perms   %5d blocks  of %7d bytes.\n\r",
	nAllocPerm, sAllocPerm );
    send_to_char( buf, ch );

    return;
}



/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy( int number )
{
    switch ( number_bits( 2 ) )
    {
    case 0:  number -= 1; break;
    case 3:  number += 1; break;
    }

    return UMAX( 1, number );
}



/*
 * Generate a random number.
 */
int number_range( int from, int to )
{
    int power;
    int number;

    if ( ( to = to - from + 1 ) <= 1 )
	return from;

    for ( power = 2; power < to; power <<= 1 )
	;

    while ( ( number = number_mm( ) & (power - 1) ) >= to )
	;

    return from + number;
}



/*
 * Generate a percentile roll.
 */
int number_percent( void )
{
    int percent;

    while ( ( percent = number_mm( ) & (128-1) ) > 99 )
	;

    return 1 + percent;
}



/*
 * Generate a random door.
 */
int number_door( void )
{
    int door;

    while ( ( door = number_mm( ) & (8-1) ) > 5 )
	;

    return door;
}



int number_bits( int width )
{
    return number_mm( ) & ( ( 1 << width ) - 1 );
}



/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */
static	int	rgiState[2+55];

void init_mm( )
{
    int *piState;
    int iState;

    piState	= &rgiState[2];

    piState[-2]	= 55 - 55;
    piState[-1]	= 55 - 24;

    piState[0]	= ((int) current_time) & ((1 << 30) - 1);
    piState[1]	= 1;
    for ( iState = 2; iState < 55; iState++ )
    {
	piState[iState] = (piState[iState-1] + piState[iState-2])
			& ((1 << 30) - 1);
    }
    return;
}



int number_mm( void )
{
    int *piState;
    int iState1;
    int iState2;
    int iRand;

    piState		= &rgiState[2];
    iState1	 	= piState[-2];
    iState2	 	= piState[-1];
    iRand	 	= (piState[iState1] + piState[iState2])
			& ((1 << 30) - 1);
    piState[iState1]	= iRand;
    if ( ++iState1 == 55 )
	iState1 = 0;
    if ( ++iState2 == 55 )
	iState2 = 0;
    piState[-2]		= iState1;
    piState[-1]		= iState2;
    return iRand >> 6;
}



/*
 * Roll some dice.
 */
int dice( int number, int size )
{
    int idice;
    int sum;

    switch ( size )
    {
    case 0: return 0;
    case 1: return number;
    }

    for ( idice = 0, sum = 0; idice < number; idice++ )
	sum += number_range( 1, size );

    return sum;
}



/*
 * Simple linear interpolation.
 */
int interpolate( int level, int value_00, int value_32 )
{
    return value_00 + level * (value_32 - value_00) / 32;
}



/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde( char *str )
{
    for ( ; *str != '\0'; str++ )
    {
	if ( *str == '~' )
	    *str = '-';
    }

    return;
}

void remove_tilde( char *str )
{
    for ( ; *str != '\0'; str++ )
    {
        if ( *str == '~' )
            *str = '\0';
    }

    return;
}


/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
       if (bstr == NULL)
	  bug( "Str_cmp: null astr and bstr. (bool str_cmp)", 0 );
       else
	  bug( "Strr_cmp: null astr, bstr = %s (bool str_cmp)", bstr );
	return TRUE;
    }

    if ( bstr == NULL )
    {
	bug( "Str_cmp: null bstr, astr = %s (bool str_cmp)", bstr );
	return TRUE;
    }

    for ( ; *astr || *bstr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
	bug( "Strn_cmp: null astr. (bool str_prefix)", 0 );
	return TRUE;
    }

    if ( bstr == NULL )
    {
	bug( "Strn_cmp: null bstr. (bool str_prefix)", 0 );
	return TRUE;
    }

    for ( ; *astr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;
    int ichar;
    char c0;

    if ( ( c0 = LOWER(astr[0]) ) == '\0' )
	return FALSE;

    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);

    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
    {
	if ( c0 == LOWER(bstr[ichar]) && !str_prefix( astr, bstr + ichar ) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;

    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);
    if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
	return FALSE;
    else
	return TRUE;
}



/*
 * Returns an initial-capped string.
 */
char *capitalize( const char *str )
{
    static char strcap[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
	strcap[i] = LOWER(str[i]);
    strcap[i] = '\0';
    strcap[0] = UPPER(strcap[0]);
    return strcap;
}



/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA *ch, char *file, char *str )
{
    FILE *fp;

    if ( IS_NPC(ch) || str[0] == '\0' )
	return;

    fclose( fpReserve );
    if ( ( fp = fopen( file, "a" ) ) == NULL )
    {
	perror( file );
	send_to_char( "Could not open the file!\n\r", ch );
    }
    else
    {
	fprintf( fp, "[%5d] %s: %s\n",
	    ch->in_room ? ch->in_room->vnum : 0, ch->name, str );
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}




/*
 * Reports a bug.
 */
void bug( const char *str, ... )
{
    char buf[MAX_STRING_LENGTH];
    FILE *fp;

    if ( fpArea != NULL )
    {
        int iLine;
        int iChar;

        if ( fpArea == stdin )
        {
            iLine = 0;
        }
        else
        {
            iChar = ftell( fpArea );
            fseek( fpArea, 0, 0 );
            for ( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
            {
                while ( getc( fpArea ) != '\n' )
                    ;
            }
            fseek( fpArea, iChar, 0 );
        }

        if ( ( fp = fopen( "shutdown.txt", "a" ) ) != NULL )
        {
            fprintf( fp, "[*****] %s\n", buf );
            fclose( fp );
            fp=NULL;
        }
    }

    strcpy( buf, "[*****] BUG: " );
    {
        va_list param;

        va_start(param, str);
        vsprintf( buf + strlen(buf), str, param);
        va_end(param);
    }
    log_string(buf );
    if (last_command)
    log_string("[*****] CMD: %s", last_command);
    fclose( fpReserve );
    fpReserve=NULL;

    if ( ( fp = fopen( BUG_FILE, "a" ) ) != NULL )
    {
        fprintf( fp, "%s\n", buf );
      if (last_command)
	fprintf( fp, "[*****] CMD: %s", last_command);
        fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );

    return;
}




// System logging function - works like printf
void log_string( const char *cpcFormat, ...)
{
  va_list     tArgList;
  time_t        ct;
  char          *tmstr;
  static char buf[MSL+1];

  va_start(tArgList, cpcFormat);
  ct = time(0);
  tmstr = asctime(localtime(&ct));
  *(tmstr + strlen(tmstr) - 1) = '\0';

  fprintf(stderr, "%-19.19s :: ", tmstr);
  vsprintf( buf, cpcFormat, tArgList );
  fprintf(stderr, buf);
  fprintf(stderr, "\n");
  logchan(buf);
  va_end(tArgList);

}





/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain( void )
{
    return;
}


