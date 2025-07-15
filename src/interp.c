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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if !defined(WIN32)
#include <unistd.h> /* unlink() */
#endif
#include "merc.h"
#include "garou.h"


bool	check_social	args( ( CHAR_DATA *ch, char *command,
			    char *argument ) );
bool	check_xsocial	args( ( CHAR_DATA *ch, char *command,
			    char *argument ) );
void	make_preg	args( ( CHAR_DATA *mother, CHAR_DATA *father ) );


int can_interpret args( (CHAR_DATA *ch, int cmd) );

int can_interpret (CHAR_DATA *ch, int cmd)
{
    bool cando = FALSE;

    if (ch->level == 12)
	cando = TRUE;

    if (cmd_table[cmd].level > get_trust(ch)) return 0;

    if (cmd_table[cmd].race == 0
        && cmd_table[cmd].discipline == 0)
	cando = TRUE;

    if (cmd_table[cmd].race > 0
         && cmd_table[cmd].discipline == 0
         && ch->class == cmd_table[cmd].race)
	cando = TRUE;

    if (cmd_table[cmd].discipline > 0
         && ch->power [ cmd_table[cmd].discipline ] >= cmd_table[cmd].disclevel)
 	cando = TRUE;

    if (!cando) return 0;


    if ( ch->position < cmd_table[cmd].position )
	return -1; 

    return 1;
}

void do_racecommands( CHAR_DATA *ch, char *argument )
{
    int i, cmd, foundpower = 0;
    char buf[MAX_STRING_LENGTH];
    bool displayed;

    if ( (ch->class == 0 && !IS_IMMORTAL(ch)) || IS_NPC(ch) )
    {
	send_to_char("You don't have any special powers.\n\r", ch);
	return;
    }

    banner_to_char("Powers", ch);

    for (i = 1 ; i < MAX_DISCIPLINES ; i++)
    {
	displayed = FALSE;
	if (ch->power[i] > 0 && strlen(discipline[i]) > 0
		&& (ch->level < 7 || argument[0] == '\0' 
		|| !str_prefix(argument, discipline[i])))
	{
	    foundpower = 1;

		for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
		{
		    if (cmd_table[cmd].discipline == i
			&& ch->power[i] >= cmd_table[cmd].disclevel
			&& IS_CLASS(ch, cmd_table[cmd].race) )
		    {
			if (!displayed)
			{
			    displayed = TRUE;
	    		    sprintf(buf, " %15s : ", discipline[i]);
			    send_to_char(buf, ch);
			}			    
			send_to_char(cmd_table[cmd].name, ch);
			send_to_char(" ", ch);
		    }
	        }
                
		if (displayed) send_to_char("\n\r", ch);
	}
    }
/* OBEAH CRAP HAVE TO ADD MANUAL......ANYONE HAVE ANOTHER WAY.....PLEASE BY ALL MEANS CHANGE IT */

if (IS_CLASS(ch, CLASS_VAMPIRE))
{
if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 1)
                            {
                            sprintf(buf, "           obeah :");
                            send_to_char(buf, ch);
                            }
if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 6)
{
                            sprintf(buf, " (#Gpurification#n)");
                            send_to_char(buf, ch);
}
if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 8)
{
                            sprintf(buf, " (#rBeast control#n)");
                            send_to_char(buf, ch);
}

if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 9)
{
			    sprintf(buf, " (#7Movement of pure magic#n)");
                            send_to_char(buf, ch);
}


send_to_char("\n\r", ch);
}
    i = 0;
    displayed = FALSE;
    for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++)
    {
	if ( IS_CLASS(ch, cmd_table[cmd].race)
	    && cmd_table[cmd].discipline == 0)
	{
	    strcpy(buf, "");
	    if (i == 0)
	    {
		switch (ch->class)
		{
		    case 1 : sprintf(buf, " %15s : ", "Demon Kind");break;
                    case 2 : sprintf(buf, " %15s : ", "Magi");break;
                    case 4 : sprintf(buf, " %15s : ", "Innate"); break;
                    case 8 : sprintf(buf, " %15s : ", "Innate");break;
                    case 16: sprintf(buf, " %15s : ", "Weaponsmaster");break;
                    case 32: sprintf(buf, " %15s : ", "Drow Abilities");break;
                    case 64: sprintf(buf, " %15s : ", "Monk Abilities");break;
                    case 128:sprintf(buf, " %15s : ", "Ninja Abilities");break;
		    case 256:sprintf(buf, " %15s : ", "Lich Abilities");break;
                    case 512:sprintf(buf, " %15s : ", "Shapeshifter");break;
                    case 1024:sprintf(buf, " %15s : ", "Tanar'ri Powers");break;
                    case 2048:sprintf(buf, " %15s : ", "Angel Abilities");break;
                    case 4096:sprintf(buf, " %15s : ", "Knight Abilities");break;
                    case 8192:sprintf(buf, " %15s : ", "Drider Abilities");break;
		    case 16384:sprintf(buf, " %15s : ", "#yP#7aladin Abilities#n");break;
		    case 32768:sprintf(buf, " %15s : ", "#nPsionist Abilities#n");break;
      		    case 65536:sprintf(buf, " %15s : ", "#0Inquisitor Abilities#n");break;
		}
	    }
	    else if (i % 4 == 0)
	    {
		sprintf(buf, "\n\r                   ");
	    }
	    i++;

	    send_to_char(buf, ch);
	    send_to_char(cmd_table[cmd].name,ch);
	    send_to_char(" ",ch);
	    displayed = TRUE;
	    foundpower++;
	}
    }

    if (displayed) send_to_char("\n\r",ch);
  
    if (foundpower == 0)
    {
        send_to_char("\n\r You do not yet possess any powers.\n\r", ch);
	divide4_to_char(ch);
    }
    else divide4_to_char(ch);

}
	
void do_klaive( CHAR_DATA *ch, char *argument )
{
OBJ_DATA *obj;
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch, CLASS_WEREWOLF))
{
send_to_char("Huh?\n\r",ch);
return;
}
if ( 60 > ch->practice)
{
send_to_char("It costs 60 points of primal to create a Lesser Klaive.\n\r",ch);
return;
}
ch->practice   -= 60;
obj = create_object(get_obj_index(OBJ_VNUM_LKLAIVE)  ,0 );
SET_BIT(obj->quest, QUEST_RELIC);
obj->value[1] = 30;
obj->value[2] = 60;
obj->questowner = str_dup(ch->pcdata->switchname);
obj_to_char(obj, ch);
act("A Lesser Klaive appears in your hands in a flash of light.",ch,NULL,NULL,TO_CHAR, 
FALSE);
act("A Lesser Klaive appears in $n's hands in a flash of light.",ch,NULL,NULL,TO_ROOM, 
FALSE);
return;
}


void do_katana( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_HIGHLANDER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( 250 > ch->practice)
  {
    send_to_char("It costs 250 points of primal to create a Katana.\n\r",ch);
    return;
  }
  ch->practice   -= 250;
  obj = create_object(get_obj_index(33176) ,50 );
  SET_BIT(obj->quest, QUEST_RELIC);
  obj->condition = 100;
  obj->toughness = 100;
  obj->resistance = 1;
  obj->value[1] = 65;
  obj->value[2] = 115;
  obj->questowner = str_dup(ch->pcdata->switchname);
  obj_to_char(obj, ch);
  act("A katana flashes into existance.",ch,NULL,NULL,TO_CHAR, FALSE);
  act("A katana suddenly apear in $n's hands.",ch,NULL,NULL,TO_ROOM, FALSE);
  return;
}

/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2



/*
 * Log-all switch.
 */
bool	fLogAll		 = FALSE;
bool	global_status	 = FALSE;
bool    extra_log	 = FALSE;
int     players_logged   = 0;
int     players_decap    = 0;
int     players_gstolen  = 0;

bool  check_disabled (const struct cmd_type *command);
DISABLED_DATA *disabled_first;
BAN_DATA *first_ban;
BAN_DATA *last_ban;

#define END_MARKER    "END" /* for load_disabled() and save_disabled()*/


/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */

    /* Name             Function    Min Position     Level  Log      Class,DiscLevel,DiscName*/

    { "north",  do_north, POS_STANDING,  0,  LOG_NORMAL, 0,0,0 },
    { "east",   do_east,  POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "south",  do_south, POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "west",   do_west,  POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "up",     do_up,    POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "down",   do_down,  POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "cast",		do_cast,				POS_FIGHTING,	0,  LOG_NORMAL, 0,0,0},
    { "call",		do_call,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0},
    { "consider",	do_consider,POS_SITTING,	 0,  LOG_NORMAL, 0,0,0},
    { "chi",            do_chi,POS_FIGHTING,3, LOG_NORMAL,64,0,0 },
    { "mesmerise",	do_command,	POS_SITTING,	 3,  LOG_NORMAL, 8, DISC_VAMP_DOMI, 2 },
    { "command",	do_command,	POS_SITTING,	 3, LOG_NORMAL, 8,DISC_VAMP_DOMI,1	},
    { "crack",		do_crack,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "diagnose",	do_diagnose,POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "dismount",	do_dismount,POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "enter",		do_enter,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "exits",		do_exits,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "get",		do_get,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "medit",    do_medit,       POS_DEAD,   7,  LOG_NORMAL, 0, 0,0 },
    { "inventory",	do_inventory,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "kill",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},

    { "buymod",		do_buymod,	POS_STANDING,	 0,  LOG_ALWAYS, 0,0,0	 },
    { "balance",	do_balance,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	 },
    { "deposit",	do_deposit,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	 },
    { "withdraw",	do_withdraw,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	 },
    { "rename",		do_rename,	POS_STANDING,	 11,  LOG_ALWAYS, 0,0,0	 },
    { "flist",		do_flist,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	 },
    { "backup",		do_backup,	POS_DEAD,	 9,  LOG_ALWAYS, 0,0,0	 },
    { "nofollow",	do_nofollow,	POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	 },
    { "cleanup",	do_cleanup,	POS_DEAD,	 9,  LOG_ALWAYS, 0,0,0	 },
    { "page",		do_page,	POS_DEAD,	 9,  LOG_NORMAL, 0,0,0	},
    { "multiple",	do_multiple,	POS_FIGHTING,	 9,  LOG_NORMAL, 0,0,0	},
    { "boot",		do_boot,	POS_DEAD,	 10,  LOG_NORMAL, 0,0,0	},
    { "look",		do_look,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "ls",			do_look,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "ltalk",          do_ftalk,       POS_DEAD,        0,  LOG_NORMAL, 0,0,0  },
    { "meditate",		do_meditate,POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "mount",		do_mount,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "order",		do_order,	POS_SITTING,	 0,  LOG_ALWAYS, 0,0,0	},
    { "rest",		do_rest,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "reimb",           do_reimb,        POS_MEDITATING,  12,  LOG_NORMAL, 0,0,0  },   
    { "sit",		do_sit,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "stand",		do_stand,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "tell",		do_tell,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "whisper",		do_whisper,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wield",		do_wear,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wizhelp",		do_wizhelp,	POS_DEAD,	 	 4,  LOG_NORMAL, 0,0,0	},
    { "version",		do_version,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "for",		do_for,	POS_DEAD,		 10, LOG_NORMAL, 0,0,0 },
    { "linkdead",	do_linkdead, POS_DEAD, 10, LOG_NORMAL, 0,0,0},

    /*
     * Informational commands.
     */
    { "affects",          do_affects,       POS_DEAD,                0,  LOG_NORMAL, 0,0,0},
    { "areas",		do_areas,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "bug",		do_bug,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "commands",		do_commands,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "credits",		do_credits,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "equipment",	do_equipment,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "examine",		do_examine,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0},
    { "help",		do_help,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "idea",		do_idea,	POS_DEAD,	  	 0,  LOG_NORMAL, 0,0,0},
    { "report",		do_report,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0},
    { "reply",          do_reply,       POS_MEDITATING,  0,  LOG_NORMAL, 0,0,0 },
    { "autostance",	do_autostance,	POS_STANDING,	0, LOG_NORMAL,0,0,0},
    { "mastery",        do_mastery,     POS_STANDING,   3, LOG_ALWAYS,0,0,0},
    { "exp",            do_level,  POS_FIGHTING,      0,  LOG_NORMAL,0,0,0},
    { "pkpowers",      do_pkpowers, POS_STANDING,    0,  LOG_NORMAL,0,0,0}, 
    { "gensteal",       do_gensteal, POS_STANDING,    3,  LOG_NORMAL,0,0,0},
    { "setstance",      do_setstance, POS_STANDING,   0,  LOG_NORMAL,0,0,0},
    { "mudstat",	do_mudstat,   POS_DEAD,       2,  LOG_NORMAL,0,0,0},
    { "level",            do_level,  POS_FIGHTING,      0, LOG_NORMAL,0,0,0},
    { "selfclass",     do_classself,  POS_STANDING,      3,  LOG_ALWAYS, 0,0,0},
    { "score",		do_score,	POS_DEAD,	 	 0,LOG_NORMAL, 0,0,0},
    { "spit",           do_spit,    POS_SITTING,       3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,1},
    { "tongue",         do_tongue,  POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_SERP,4},
    { "stat",           do_stat,    POS_DEAD,          3,  LOG_NORMAL, 0,0,0},
    { "skill",		do_skill,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0 },
    { "spells",		do_spell,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0 },
    { "socials",		do_socials,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "time",		do_time,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "typo",		do_typo,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "who",		do_who,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "wizlist",		do_wizlist,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0 },
    { "xemot",		do_huh,	POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0 },
    { "xemote",		do_xemote,	POS_SITTING,	 1,  LOG_NORMAL, 0,0,0 },
    { "xsocial",		do_huh,	POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0 },
    { "xsocials",		do_xsocials,POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0 },
   // { "topxp",		do_topxp, POS_DEAD,		0,  LOG_NORMAL, 0,0,0},
    /*
     * Configuration commands.
     */
    { "purchase",		do_buy, POS_STANDING,	0,  LOG_NORMAL, 0,0,0  },
    { "alignment",	do_alignment,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "channels",		do_channels,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "config",		do_config,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "description",	do_desc,POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "password",		do_password,POS_DEAD,	 	 0,  LOG_NEVER, 0,0,0	},
    { "title",		do_title,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
 //   { "wimpy",		do_wimpy,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "ansi",		do_ansi,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "autoexit",		do_autoexit,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "autoloot",		do_autoloot,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "autosplit",		do_autosplit,POS_DEAD,		 0,  LOG_NORMAL, 0,0,   },
    { "autosac",		do_autosac,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "autogold",		do_autogold, POS_DEAD,		 0,  LOG_NORMAL, 0,0,0  },
    { "autoadamantite",	do_autoadamantite,	POS_DEAD,		 0,  LOG_NORMAL, 0,0,0	},
    { "autoelectrum",   do_autoelectrum, POS_DEAD,               0,  LOG_NORMAL, 0,0,0  },
    { "buyqp",		do_buyqp,	POS_DEAD,		 0,  LOG_NORMAL, 0,0,0  },
    { "blank",		do_blank,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "brief",		do_brief,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "brief2",          do_brief2,       POS_DEAD,                0,  LOG_NORMAL, 0,0,0  },
    { "brief3",		do_brief3,	POS_DEAD,		0, LOG_NORMAL, 0,0,0 },
    { "cprompt",		do_cprompt,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "prompt",		do_prompt,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "map",		do_map,		POS_DEAD,                0,  LOG_NORMAL, 0,0,0  },
    /*
     * Communication commands.
     */
    { "flame",		do_flame,       POS_DEAD,                0,  LOG_NORMAL,0,0,0   },
    { "chat",		do_chat,	POS_DEAD,	 	 0,  LOG_NORMAL,0,0,0	},
    { ".",			do_chat,	POS_DEAD,2,LOG_NORMAL, 0,0,0	},
    { "clan",		do_clan,	POS_DEAD,	 3,	LOG_NORMAL, 0,0,0,},
    { "clandisc",		do_clandisc,POS_SITTING,       3,  LOG_NORMAL, 0,0,0 },
    { "intro",		do_introduce,	POS_DEAD,	 3,LOG_NORMAL, 8,0,0, },
    { "intro",          do_introduce,   POS_DEAD,        3,LOG_NORMAL, 4,0,0, },
    { "emote",		do_xemote,	POS_SITTING,	 0,  LOG_NORMAL,0,0,0 },
    { ",",			do_xemote,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0},
    { "gtell",		do_gtell,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { ";",			do_gtell,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "howl",		do_howl,	POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0	},
    { "telepath",		do_telepath,	POS_DEAD,	 	 1,  LOG_NORMAL, 512,0,0	},
    { "music",		do_music,	POS_SLEEPING,	 2,  LOG_NORMAL,0,0,0 },
    { "note",		do_note,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "board",		do_board,	POS_DEAD,	0,	LOG_NORMAL,0,0,0},
    { "pose",		do_emote,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "quest",		do_quest,	POS_SITTING,	 0,  LOG_ALWAYS,0,0,0	},
 //   { "newbie",		do_newbie,	POS_SLEEPING,	 2,  LOG_NORMAL, 0,0,0 },
    { "say",		do_say,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "'",		do_say,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "shout",		do_shout,	POS_SITTING,	 2,  LOG_NORMAL, 0,0,0 },
    { "yell",		do_yell,	POS_SITTING,	 2,  LOG_NORMAL, 0,0,0 },
    { "ignore",		do_ignore,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0 },
    { "unignore",	do_unignore,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0 },
    { "powers", do_racecommands, POS_FIGHTING, 3, LOG_NORMAL,0,0,0 },

   /*
    * Clan commands
    */

//    { "newclan",          do_newclan,       POS_SITTING,     8,  LOG_NORMAL, 0,0,0  },
//    { "showclan",          do_showclan,       POS_SITTING,     8,  LOG_NORMAL, 0,0,0  },

    /*
     * Object manipulation commands.
     */
    
//    { "activate",		do_activate,POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "brandish",		do_brandish,POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "close",		do_close,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "draw",		do_draw,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "drink",		do_drink,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "drop",		do_drop,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "consume",	do_tokeneat,	POS_STANDING,    0,  LOG_NORMAL, 0,0,0  },
    { "eat",		do_eat,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "empty",		do_empty,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "fill",		do_fill,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "give",		do_give,	POS_SITTING,	 0,  LOG_ALWAYS, 0,0,0	},
    { "gift",           do_gift,        POS_STANDING,    0,  LOG_ALWAYS, 0,0,0  },
    { "hold",		do_wear,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "lock",		do_lock,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "morph",		do_morph,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "open",		do_open,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "pick",		do_pick,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
 //   { "press",		do_press,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
 //   { "pull",		do_pull,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "put",		do_put,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0 },
    { "quaff",		do_quaff,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "recite",		do_recite,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "remove",		do_remove,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "sheath",		do_sheath,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "take",		do_get,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "reload",		do_reload,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "unload",		do_unload,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "shoot",		do_shoot,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "throw",		do_throw,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "turn",		do_turn,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
//    { "twist",		do_twist,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "sacrifice",	do_sacrifice,	POS_SITTING,	 1,  LOG_NORMAL, 0,0,0	},
    { "unlock",		do_unlock,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wear",		do_wear,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "voodoo",		do_voodoo,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "zap",		do_zap,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "junk",           do_sacrifice,POS_STANDING,     3,  LOG_NORMAL, 0,0,0 },


        { "inabikari",  do_inabikari,   POS_FIGHTING,   3,      LOG_NORMAL, 16,0,0 },
        { "safaia",     do_safaia,   POS_FIGHTING,   3,      LOG_NORMAL, 16,0,0 },
        { "strike",     do_strike,   POS_FIGHTING,   3,      LOG_NORMAL, 16,0,0 },
        { "shinku",     do_shinku,   POS_FIGHTING,   3,      LOG_NORMAL, 16,0,0 },
        { "midori",     do_midori,   POS_FIGHTING,   3,      LOG_NORMAL, 16,0,0 },
        { "shiroi",     do_shiroi,   POS_FIGHTING,   3,      LOG_NORMAL, 16,0,0 },
        { "kisetsufuu", do_kisetsufuu,   POS_FIGHTING,   3,      LOG_NORMAL, 16,0,0 },
        { "kaminari",   do_kaminari,   POS_FIGHTING,   3,      LOG_NORMAL, 16,0,0 },
      //  { "ancestralpath",  do_hologramtransfer, POS_STANDING, 3,       LOG_NORMAL, 16,0,0 },
        { "techniques", do_bladespin, POS_STANDING,      3,      LOG_NORMAL, 16,0,0},
        { "web",        do_web, POS_FIGHTING, 3, LOG_NORMAL, 16, 0,0},
        { "wallofswords",   do_wallofswords, POS_STANDING, 3, LOG_NORMAL, 16,0,0},
        { "truesight",  do_truesight,   POS_STANDING,  3, LOG_NORMAL, 16,0,0},

    /*
     *  Inquisitor by Rand
     */
    { "staff",		 do_staff,	POS_STANDING, 3, LOG_NORMAL, 65536,0,0},
    { "enlist",		 do_enlist,	POS_STANDING, 3, LOG_NORMAL, 0,0,0},
    { "inquire",	 do_inquire,	POS_DEAD, 3, LOG_NORMAL, 65536,0,0},
    { "torture",	 do_torture,    POS_FIGHTING, 3, LOG_NORMAL, 65536,0,0},
  //  { "walk",		 do_walk,	POS_STANDING, 3, LOG_NORMAL, 65536,0,0},
    { "unholyfury",	 do_unholyfury,	POS_STANDING, 3, LOG_NORMAL, 65536,0,0},
    { "purify",          do_purify,     POS_FIGHTING, 3, LOG_NORMAL, 65536,0,0},
    { "inquire",         do_scry, 	POS_STANDING, 3, LOG_NORMAL, 65536,0,0},
    { "torment",	 do_torment,    POS_STANDING, 3, LOG_NORMAL, 65536,0,0},
    { "chains",		 do_chains,	POS_STANDING, 3, LOG_NORMAL, 65536,0,0},
    { "unholyritual",    do_unholyritual, POS_STANDING, 3, LOG_NORMAL, 65536,0,0},
    /*
     *  Paladins by Rand
     */
    { "quarter",	do_quarter, POS_STANDING, 3, LOG_NORMAL, 16384,0,0},
  //  { "charge",		do_paladintravel, POS_STANDING, 3, LOG_NORMAL,16384,0,0},
    { "masterofarms",	do_paladinweapons, POS_STANDING, 3, LOG_NORMAL,16384,0,0},
    { "godeyes",	do_truesight, POS_STANDING, 3, LOG_NORMAL,16384,0,0},
    { "holyweb",	do_web, POS_FIGHTING, 3, LOG_NORMAL,16384,0,0},
    { "retribution",	do_paladineye,POS_STANDING, 3, LOG_NORMAL,16384,0,0},
    { "ritual",		do_paladintalk, POS_DEAD, 3, LOG_NORMAL,16384,0,0},
    { "reflect",	do_reflect, POS_FIGHTING, 3, LOG_NORMAL,16384,0,0},
    { "bladeslash",	do_bladeslash,  POS_FIGHTING, 3, LOG_NORMAL,16384,0,0},
    /*
     *  Psionist by Rand
     */ 

    { "scry",		do_scry, POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
    { "truesight",	do_truesight,POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
    { "enhance",	do_enhance, POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
//    { "psionisteq",     do_psi_eq, POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
    { "wrath",          do_wrath, POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
    { "mindblast",	do_mindblast, POS_FIGHTING, 3, LOG_NORMAL, 32768,0,0},
    { "enfeeble",	do_enfeeble, POS_FIGHTING, 3, LOG_NORMAL, 32768,0,0},
    { "meddle",		do_meddle,  POS_FIGHTING,  3, LOG_NORMAL, 32768,0,0},
    { "entrapment",	do_entrapment, POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
    { "command",	do_command, POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
    { "awe",		do_awe,	POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
    { "confuse",	do_confuse, POS_FIGHTING, 3, LOG_NORMAL, 32768,0,0},
  //  { "mindgate",	do_mindgate,POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
    { "massassinate",	do_massassinate,POS_STANDING, 3, LOG_NORMAL, 32768,0,0},
    { "mindtalk",	do_psionisttalk,POS_DEAD, 3, LOG_NORMAL, 32768,0,0},
    /* 
     * Combat commands.
     */
    { "generation",     do_generation,POS_STANDING,   12,  LOG_ALWAYS,0,0,0 },
    { "class",          do_class,   POS_STANDING,     11, LOG_ALWAYS,0,0,0},
    { "backstab",		do_backstab,POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "berserk",		do_berserk,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "spin",	do_spin,	POS_FIGHTING,	0,	LOG_NORMAL, 0,0,0	},
    { "bs",			do_backstab,POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "disarm",		do_disarm,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "flee",		do_flee,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "fightstyle",	do_fightstyle,POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "hurl",		do_hurl,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "kick",		do_kick,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "servant",        do_servant, POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_DAIM,8 },
    { "punch",		do_punch,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "rescue",		do_rescue,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "stance",		do_stance,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},

/* Demon pray */
    { "pray",           do_pray,        POS_MEDITATING,  1,  LOG_NORMAL, 64,0,0 },


    /*
     * Power Lich commands.
     */
    
    { "lore",         do_lore,          POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "studylore",    do_studylore,     POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "lichtalk",     do_lichtalk,      POS_SLEEPING,   3,  LOG_NORMAL, 256,0,0},
    { "objectgate",   do_objectgate,    POS_STANDING,   3,  LOG_NORMAL, 256,0,0}, 
    { "fireball",     do_infernal,      POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "chaosmagic",   do_chaosmagic,    POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0}, 
    { "chaossurge",   do_chaossurge,    POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "chaosshield",  do_chaosshield,   POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "golemsummon",  do_summongolem,   POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
  //  { "planartravel", do_planartravel,  POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "truesight",    do_truesight,     POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "planarstorm",  do_planarstorm,   POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "readaura",     do_readaura,      POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "powertransfer",do_powertransfer, POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "polarity",     do_polarity,      POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "chillhand",    do_chillhand,     POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "creepingdoom", do_creepingdoom,  POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "painwreck",    do_painwreck,     POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "earthswallow", do_earthswallow,  POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
  //  { "licharmor",    do_licharmor,     POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "soulsuck",     do_soulsuck,      POS_FIGHTING,   3,  LOG_NORMAL, 256,0,0},
    { "zombie",       do_zombie,        POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "pentagram",    do_pentagram,     POS_STANDING,   3,  LOG_NORMAL, 256,0,0},
    { "planeshift",   do_planeshift,    POS_STANDING,   3,  LOG_NORMAL, 256,0,0},

    /*
     * Tanar'ri commands.
     */

    { "earthquake",	do_earthquake,  POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "tornado",	do_tornado,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "infernal",	do_infernal,    POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "bloodsacrifice", do_bloodsac,    POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
   // { "enmity",		do_enmity,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "enrage",		do_enrage,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "booming",        do_booming,     POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "truesight",      do_truesight,   POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "web",		do_web,		POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "claws",		do_claws,	POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
 //   { "chaosgate",	do_chaosgate,	POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "fury",		do_fury,	POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "tantalk",	do_tantalk,	POS_DEAD,     3, LOG_NORMAL, 1024,0,0},
//    { "taneq",		do_taneq,	POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "bloodrite",      do_unholyrite,  POS_STANDING, 3, LOG_NORMAL, 1024,0,0},
    { "lavablast",	do_lavablast,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},
    { "chaossurge",	do_chaossurge,	POS_FIGHTING, 3, LOG_NORMAL, 1024,0,0},

    /*
     * Undead Knight commands
     */

    { "knighttalk",     do_knighttalk, POS_DEAD,   3,  LOG_NORMAL, 4096,0,0},
 //   { "knightarmor",  do_knightarmor, POS_STANDING, 3,  LOG_NORMAL,4096,0,0},
    { "gain",		do_gain,	POS_STANDING, 3, LOG_NORMAL,4096,0,0},
    { "weaponpractice", do_weaponpractice, POS_STANDING, 3, LOG_NORMAL, 4096,0,0},
    { "powerword",      do_powerword, POS_FIGHTING, 3,	LOG_NORMAL, 4096,0,0},
    { "aura",		do_aura,	POS_STANDING, 3, LOG_NORMAL, 4096,0,0},
    { "command",	do_command,   POS_STANDING,  3, LOG_NORMAL, 4096,0,0},
    { "unholysight",	do_truesight,  POS_STANDING, 3, LOG_NORMAL, 4096,0,0},
    { "bloodrite",	do_unholyrite, POS_STANDING, 3, LOG_NORMAL, 4096,0,0},
   // { "ride",		do_ride,	POS_STANDING, 3, LOG_NORMAL, 4096,0,0},

    /*
     * Angel Commands.
     */

    { "prayer",        do_angeltalk,     POS_MEDITATING,3, LOG_NORMAL, 2048,0,0 },
    { "gpeace",        do_gpeace,        POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "innerpeace",    do_innerpeace,    POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "houseofgod",    do_houseofgod,    POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "angelicaura",   do_angelicaura,   POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "gbanish",       do_gbanish,       POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "harmony",       do_harmony,       POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "gsenses",       do_gsenses,       POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "gfavor",        do_gfavor,        POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "forgivness",    do_forgivness,    POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "martyr",        do_martyr,        POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
  //  { "swoop",         do_swoop,         POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "awings",        do_awings,        POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "halo",          do_halo,          POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "sinsofthepast", do_sinsofthepast, POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "eyeforaneye",   do_eyeforaneye,   POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
  //  { "angelicarmor",  do_angelicarmor,  POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "touchofgod",    do_touchofgod,    POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    { "spiritform",    do_spiritform,    POS_STANDING,  3, LOG_NORMAL, 2048,0,0 },
    { "repent",	       do_repent,	 POS_FIGHTING,	3, LOG_NORMAL, 2048,0,0 },
    { "scry",	       do_scry,		 POS_STANDING,	3, LOG_NORMAL, 2048,0,0 },
    { "divinewrath",   do_divinewrath,	 POS_FIGHTING,  3, LOG_NORMAL, 2048,0,0 },
    /*
     * Shapeshifter Commands.
     */
    
    { "shift",       do_shift,    POS_FIGHTING,   3,  LOG_NORMAL, 512,0,0 },
    { "formlearn",   do_formlearn, POS_STANDING,  3,  LOG_NORMAL, 512,0,0 },
//    { "camouflage",  do_camouflage, POS_STANDING, 3,  LOG_NORMAL, 512,0,0 },
    { "mask",        do_mask,	POS_STANDING,     3,  LOG_NORMAL, 512,0,0 },
    { "truesight",   do_truesight, POS_STANDING,  3,  LOG_NORMAL, 512,0,0 },
    { "shapeshift",  do_shapeshift, POS_STANDING, 3,  LOG_NORMAL, 512,0,0 },
    { "hatform",     do_hatform,   POS_STANDING,  3,  LOG_NORMAL, 512,0,0 },
  //  { "mistwalk",    do_mistwalk,  POS_STANDING,  3,  LOG_NORMAL, 512,0,0 },
  //  { "shapearmor",  do_shapearmor, POS_STANDING, 3,  LOG_NORMAL, 512,0,0 },
    { "roar",        do_shaperoar, POS_FIGHTING,  3,  LOG_NORMAL, 512,0,0 },
    { "stampede",      do_stampede,   POS_FIGHTING,   3,  LOG_NORMAL, 512,0,0 },
    { "fblink",      do_faerieblink, POS_FIGHTING,3,  LOG_NORMAL, 512,0,0 },
    { "stomp",       do_stomp,  POS_FIGHTING,     3,  LOG_NORMAL, 512,0,0 },
    { "tigercurse", do_tigercurse, POS_FIGHTING,3,  LOG_NORMAL, 512,0,0 },
    { "phase",       do_phase,   POS_FIGHTING,    3,  LOG_NORMAL, 512,0,0 },
    { "breath",      do_breath, POS_FIGHTING,     3,  LOG_NORMAL, 512,0,0 },

    /*
     * Mage Commands.
     */

    { "magics",       do_magics,    POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
 //   { "teleport",     do_teleport,  POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
    { "invoke",       do_invoke,    POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
    { "chant",        do_chant,     POS_FIGHTING, 3,  LOG_NORMAL, 258,0,0 },
 //   { "magearmor",    do_magearmor, POS_MEDITATING, 3,  LOG_NORMAL, 2,0,0 },
    { "objectgate",   do_objectgate,POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
    { "discharge",    do_discharge, POS_FIGHTING,   3,  LOG_NORMAL, 2,0,0 },
    { "scry",	      do_scry,      POS_FIGHTING,   3,  LOG_NORMAL, 2,0,0 },
    { "reveal",       do_reveal,    POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },
    { "chaosmagic",   do_chaosmagic,POS_FIGHTING,   3,  LOG_NORMAL, 2,0,0 },
    { "infuse",       do_infuse,    POS_FIGHTING,   3,  LOG_NORMAL, 2,0,0 },
    { "bomb",         do_bomb,      POS_FIGHTING,   3,  LOG_NORMAL, 2,0,0 },
                    /* PK POWERS */
   // { "resetchar",	do_resetchar,   POS_STANDING,   11, LOG_NORMAL,0,0,0},
    { "eaglesight",       do_pkscry,    POS_STANDING,   3,  LOG_NORMAL,0,0,0},
//    { "silverpath",       do_pkportal,  POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "lifesense",	  do_pkaura,	POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "adamantium",     do_adamantium,POS_STANDING,    3,  LOG_NORMAL, 64,0,0 },

			/*	   */
    			/* My Shit */
 			/*  Rand   */
  			/*	   */
    { "riftwalk",	do_riftwalk,    POS_STANDING, 3, LOG_NORMAL,0,0,0},
    { "blessing",	do_blessing,	POS_FIGHTING, 3, LOG_NORMAL,16384,0,0},
    { "knock",		do_knock,	POS_FIGHTING,   1, LOG_NORMAL, 0,0,0},
    { "oscatter",	do_scatter, POS_STANDING,   11, LOG_NORMAL, 0,0,0},
    { "showtele",	do_showtele, POS_STANDING,   0, LOG_NORMAL, 0,0,0},
    { "policy",		do_policy, POS_STANDING,   0, LOG_NORMAL, 0,0,0},
    { "srecall",	do_srecall,	POS_STANDING,   3, LOG_NORMAL, 0,0,0},
    { "oversee",	     do_pkwhere,  POS_STANDING, 3, LOG_NORMAL, 0,0,0},
    { "rares",       do_rares,    POS_STANDING,    0,  LOG_NORMAL, 0,0,0  },
    { "classleader",	do_classleader,  POS_STANDING,  2,  LOG_NORMAL, 0,0,0},
    { "cl-clear",	do_clear_class_leader, POS_DEAD, 11, LOG_NORMAL, 0,0,0},
    { "newbie",		do_newbie,	POS_DEAD,	0,  LOG_NORMAL, 0,0,0 },
    { "newbiepack",	do_newbiepack,	POS_STANDING,   3, LOG_NORMAL, 0,0,0},
   // { "purchase",	do_purchase,	POS_STANDING,	3,  LOG_NORMAL, 0,0,0 },
    { "mercy",		do_mercy,	POS_STANDING,      3,  LOG_NORMAL, 0,0,0  },
    { "adamantiumeq", 	do_adamantiumeq,  POS_STANDING,   3,  LOG_NORMAL,0,0,0},
  //  { "supremacy",	do_supreme,	POS_STANDING,   3,  LOG_NORMAL,0,0,0},
  //  { "breakfree",	do_breakfree,	POS_DEAD,	3,  LOG_NORMAL,0,0,0},
    { "update",		do_update,	POS_STANDING,	12, LOG_NORMAL,0,0,0},
  //  { "store",		do_legendexp,	POS_STANDING,	3,  LOG_NORMAL,0,0,0},
    { "awareness",	do_pkguardian,	POS_STANDING,	3,  LOG_NORMAL,0,0,0},
    { "ffa",		do_ffa,		POS_STANDING,	3,  LOG_ALWAYS,0,0,0},
  //  { "general",	do_general,	POS_STANDING,	3,  LOG_NORMAL,0,0,0},
 //   { "abuse",		do_pkdamage,	POS_FIGHTING,	3,  LOG_NORMAL,0,0,0},
 //   { "dissolve",	do_pkteleport,  POS_FIGHTING,   3,  LOG_NORMAL,0,0,0},
 //   { "retreat",	do_pkflee,	POS_FIGHTING,	3,  LOG_NORMAL,0,0,0},
    { "pkready",	do_pkready,	POS_STANDING,	3,  LOG_NORMAL,0,0,0},
    { "donate",		do_donate,	POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "toss",		do_toss,	POS_STANDING,	3,	LOG_NORMAL, 0,0,0},
    { "wish",           do_wish,        POS_STANDING,   3,       LOG_NORMAL, 0,0,0},
    { "dice",		do_dice,	POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "clairvoyance",     do_mobius,    POS_STANDING,   3,  LOG_NORMAL,0,0,0},
 //   { "renewal",          do_mobius_heal, POS_STANDING,  3,  LOG_NORMAL,0,0,0},
    { "fixexp",           do_fixexp,    POS_STANDING,   3,  LOG_NORMAL,0,0,0},
 //   { "wormhole",       do_pkportal2,   POS_STANDING,   3,  LOG_NORMAL,0,0,0},
    { "disappear",    do_mobius_bm, POS_STANDING,   3,  LOG_NORMAL, 2,0,0 },

 	//Jobo's Aliases from Rand
   { "alias",           do_alias,       POS_FIGHTING,   0,   LOG_NORMAL, 0,0,0 },
   { "showalias",       do_showalias,   POS_FIGHTING,   0,   LOG_NORMAL, 0,0,0 },
   { "removealias",     do_removealias, POS_FIGHTING,   0,   LOG_NORMAL, 0,0,0 },
/* HUNTERS and things BY HUW */
//   { "ensnare",	do_ensnare,	POS_STANDING,   3,   LOG_NORMAL, 131072,0,0 },
//   { "transmit",	do_transmit,	 POS_FIGHTING,	3,   LOG_NORMAL, 131072,0,0 },
//    { "imbue",         do_imbue,       POS_STANDING,   3,  LOG_NORMAL, 131072,0,0 },
   { "pscore",		do_pscore,	POS_STANDING,	0,   LOG_NORMAL, 0,0,0},



/*                      ****|  V-A-M-P-I-R-E-S  |****          */

// Melpominee
//    { "scream",		do_scream,    POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_MELP, 1 },

// Daimoinon
    { "guardian",	do_guardian,  POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_DAIM, 1 },
    { "fear",		do_fear,      POS_FIGHTING,  3,  LOG_NORMAL, 8, DISC_VAMP_DAIM, 2 },
    { "portal",		do_gate,      POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_DAIM, 3 },
// Lvl 4 - curse ---
//    { "vtwist",		do_vtwist,     POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_DAIM, 5 },
    { "wither", do_wither, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_HAWK,7 },

// Thanatosis
    { "hagswrinkles",	do_hagswrinkles,POS_STANDING, 3, LOG_NORMAL, 8, DISC_VAMP_THAN, 1 },
    { "putrefaction",   do_rot,	      POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_THAN, 2 },
/*    { "ashes",		do_ashes,     POS_STANDING,  3,
LOG_NORMAL, 8, DISC_VAMP_THAN, 3 }, */
    { "withering",	do_withering, POS_FIGHTING,  3,  LOG_NORMAL, 8, DISC_VAMP_THAN, 4 },
    { "drainlife",	do_drain,     POS_FIGHTING,  3,  LOG_NORMAL, 8, DISC_VAMP_THAN, 5 },

// Necromancy
   
    { "preserve",	do_preserve,  POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_NECR, 2 },
//    { "spiritgate",	do_spiritgate,POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_NECR, 3 },
    { "spiritguard",	do_spiritguard,POS_STANDING, 3,  LOG_NORMAL, 8, DISC_VAMP_NECR, 4 },


// Auspex
    { "truesight",      do_truesight, POS_STANDING,  3,  LOG_NORMAL, 4, DISC_WERE_HAWK, 3 },
    { "truesight",	do_truesight, POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 1 },
    { "readaura",	do_readaura,  POS_FIGHTING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 2 },
    { "scry",		do_scry,      POS_FIGHTING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 3 },
  //  { "astralwalk",	do_astralwalk,POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 4 },
    { "unveil",		do_unveil,    POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_AUSP, 5 },

// Obfuscate
    { "vanish",		do_vanish,    POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_OBFU, 1 },
    { "mask",		do_mask,      POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_OBFU, 2 },
    { "shield",		do_shield,    POS_STANDING,  3,  LOG_NORMAL, 8, DISC_VAMP_OBFU, 3 },
    { "shield",         do_shield,    POS_STANDING,  3,  LOG_NORMAL, 4, DISC_WERE_OWL, 2 },

// Chimerstry
    { "mirror",		do_mirror,    POS_STANDING,  3, LOG_NORMAL, 8, DISC_VAMP_CHIM, 1 },
    { "formillusion",	do_formillusion,POS_STANDING, 3, LOG_NORMAL,8, DISC_VAMP_CHIM, 2 },
    { "clone",		do_clone,     POS_STANDING,  3, LOG_NORMAL, 8, DISC_VAMP_CHIM, 3 },
    { "controlclone",	do_control,   POS_STANDING,  3, LOG_NORMAL, 8, DISC_VAMP_CHIM, 4 },
//    { "objectmask",     do_objmask,   POS_STANDING,  3, LOG_NORMAL, 8, DISC_VAMP_CHIM, 5 },

    /*
     * Miscellaneous commands.
     */
    { "accep",		do_huh,		POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "accept",		do_accept,	POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
//    { "agree",          do_agree,       POS_STANDING,    2,  LOG_NORMAL, 0,0,0  },
    { "artifact",	do_artifact,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "birth",		do_birth,	POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "blindfold",	do_blindfold,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "breaku",		do_huh,		POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "breakup",	do_breakup,	POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
  //  { "challenge",      do_challenge,   POS_STANDING,    2,  LOG_NORMAL, 0,0,0  },
    { "claim",		do_claim,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "consen",		do_huh,		POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "consent",	do_consent,	POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    //{ "decline",        do_decline,     POS_STANDING,    2,  LOG_NORMAL, 0,0,0  }, 
    { "whois",		do_see_pfile,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "finger",		do_see_pfile,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "follow",		do_follow,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "gag",		do_gag,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "godsfavor",      do_godsfavor,POS_DEAD,         3,  LOG_NORMAL, 64,0,0 },
    { "group",		do_group,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "hide",		do_hide,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
 // { "home",		do_home,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "hunt",		do_hunt,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "locate",		do_locate,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "practice",		do_practice,POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "propos",		do_huh,	POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "propose",		do_propose,	POS_STANDING,	 2,  LOG_NORMAL, 0,0,0	},
    { "qui",		do_qui,	POS_DEAD,		 0,  LOG_NORMAL, 0,0,0	},
    { "quit",		do_quit,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "recall",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "escape",		do_escape,	POS_DEAD,	 	 3,  LOG_NORMAL, 0,0,0	},
    { "/",			do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "rent",		do_rent,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "safe",		do_safe,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "save",		do_save,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "savecmd",	do_savecmd,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "sleep",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "smother",		do_smother,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "sneak",		do_sneak,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
   // { "split",		do_split,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "scan",		do_scan,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "spy",		do_spy,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "steal",		do_steal,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "notravel",		do_notravel,POS_DEAD,	 	 0,LOG_NORMAL, 0,0,0 },
    { "nosummon",		do_nosummon,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "embrace",        do_embrace, POS_STANDING,      3,  LOG_NORMAL, 8,0,0 },
    { "diablerise",	do_diablerise, POS_STANDING, 3, LOG_NORMAL, 8,0,0 },
    { "tendrils",           do_tendrils,POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_SERP,4 },
    { "lamprey",        do_lamprey, POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,5 },
    { "entrance",       do_entrance,POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_PRES,3 },
    { "fleshcraft",     do_fleshcraft,POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_VICI,2 },
    { "zombie",         do_zombie,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_NECR,5 },
    { "baal",           do_baal,    POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_DOMI,5 },
    { "dragonform",     do_dragonform,POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_VICI,4 },
    { "spew",           do_spew,    POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,6 },
    { "bloodwater",     do_bloodwater,POS_FIGHTING,    3,  LOG_NORMAL, 8,DISC_VAMP_NECR,5 },
    { "gourge",         do_gourge,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,8 },
    { "congregate",	do_congregation,POS_FIGHTING,  3,  LOG_NORMAL, 4,DISC_WERE_CONG,1 },
    { "roar",		do_roar,    POS_FIGHTING,      3,  LOG_NORMAL, 4,DISC_WERE_BEAR,6 },
    { "jawlock",	do_jawlock, POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_RAPT,8 },
    { "perception",	do_perception,POS_STANDING,    3,  LOG_NORMAL, 4,DISC_WERE_RAPT,3 },
    { "skin",		do_skin,    POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_BEAR,7 },
    { "rend",		do_rend,    POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_BOAR,7 },
    { "slam",		do_slam,    POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_BEAR,8 },
    { "shred",		do_shred,   POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_RAPT,7 },
    { "diablerize",	do_diablerise, POS_STANDING,   3,  LOG_NORMAL, 8,0,0},
    { "testarm",	do_testarm,	POS_STANDING, 12,  LOG_NORMAL, 0,0,0 },  
    { "testcmd",	do_testcmd,	POS_STANDING, 12,  LOG_NORMAL, 0,0,0 },  
//    { "pfile",		do_pfile,	POS_STANDING, 12,  LOG_NORMAL, 0,0,0 },  
    { "stackset",	do_setstacker,	POS_STANDING, 3,  LOG_NORMAL, 0,0,0 },  
    { "speedset",	do_setspeeder,	POS_STANDING, 3,  LOG_NORMAL, 0,0,0 },  
    { "noarm",		do_noarm,   POS_DEAD,         8,  LOG_NORMAL, 0, 0, 0 }, 
    { "taste",          do_taste,   POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,1 },
    { "pigeon",         do_pigeon,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,3 },
    { "bloodagony",     do_bloodagony,POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,3 },
    { "tie",		do_tie,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "token",		do_token,	POS_STANDING,	 2,  LOG_ALWAYS, 0,0,0	},
    { "exptoken",	do_exptoken,	POS_STANDING,    3,  LOG_ALWAYS, 0,0,0  },
    { "track",		do_track,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "train",		do_train,   POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "unpolymorph",	do_unpolymorph,POS_STANDING,	 4,  LOG_NORMAL, 0,0,0 },
    { "untie",		do_untie,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "visible",		do_visible,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0 },
    { "wake",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "where",		do_where,	POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
/* need it before forge */


        { "contraception", do_contraception, POS_DEAD,  0,      LOG_NORMAL, 0,0,0},


 /*
  * Monk
  */
    { "chaoshands",	do_chands,	POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
  //	  { "monkarmor",	do_monkarmor,	POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "ghold",		do_ghold,	POS_STANDING,    3,  LOG_NORMAL, 64,0,0 },
    { "godsheal",	do_godsheal,	POS_FIGHTING,	 3,  LOG_NORMAL, 64,0,0 },
    { "mantra",		do_mantra,	POS_STANDING,	 3, LOG_NORMAL, 64,0,0 },
    { "guide",		do_guide,	POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "wrathofgod",  do_wrathofgod, POS_FIGHTING, 3, LOG_NORMAL, 64,0,0},
    { "cloak",		do_cloak,	POS_STANDING,      3,  LOG_NORMAL, 64,0,0 },
    { "sacredinvis",	do_sacredinvis, POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "flaminghands",   do_flaminghands,POS_STANDING,  3,  LOG_NORMAL, 64,0,0 },
    { "darkblaze",	do_darkblaze, POS_FIGHTING,	3, LOG_NORMAL, 64,0,0},
    { "godseye",		do_godseye,	POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
   // { "celestial",	do_celestial,POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "steelskin",	do_steelskin,POS_STANDING,	 3,  LOG_NORMAL, 64,0,0 },
    { "godsbless",	do_godsbless,POS_FIGHTING,	 3,  LOG_NORMAL, 64,0,0 },
    { "thrustkick",	do_thrustkick,POS_FIGHTING,	3, LOG_NORMAL, 64,0,0 },
    { "naturalharmony", do_naturalharmony, POS_STANDING,       3,  LOG_NORMAL, 64,0,0 },
    { "spinkick",	do_spinkick,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "backfist",	do_backfist,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "elbow",		do_elbow,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "sweep",		do_sweep,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "reverse",	do_reverse,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "knee",		do_knee,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "shinkick",	do_shinkick,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "palmstrike",	do_palmstrike,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "spiritpower",	do_spiritpower,POS_FIGHTING,3,LOG_NORMAL,64,0,0},
    { "deathtouch",	do_deathtouch,POS_FIGHTING,3,LOG_NORMAL,64,0,0 },
    { "relax",		do_relax,POS_FIGHTING,3,LOG_NORMAL,64,0,0},
    { "monktalk",	do_monktalk,POS_DEAD,3,LOG_NORMAL,64,0,0 },
    { "godsfire",	do_godsfire,    POS_STANDING, 3,LOG_NORMAL,64,0,0},
    { "disciplines",    do_disciplines,POS_STANDING,   3,  LOG_NORMAL, 0,0,0 },
    { "research",		do_research,POS_STANDING,      3,  LOG_NORMAL, 0,0,0 },
    

/*
 * Garou
 */
// Ahroun
    { "razorclaws",	do_razorclaws, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_WOLF,4 },

// Homid
//  Persuasion, automatic
    { "staredown",	do_staredown, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_OWL,5 },
    { "disquiet",	do_disquiet, POS_FIGHTING, 3 , LOG_NORMAL, 4,DISC_WERE_OWL,6 }, 
//    { "reshape",	do_reshape,  POS_STANDING,  3,  LOG_NORMAL, 4,DISC_WERE_OWL,7 },
    { "cocoon",		do_cocoon,   POS_FIGHTING,  3,  LOG_NORMAL, 4,DISC_WERE_OWL, 8 },

// Metis
    { "quills",	do_quills, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_HAWK,5 },
  //  { "burrow", do_burrow, POS_FIGHTING, 3, LOG_NORMAL, 4,DISC_WERE_HAWK,6 },
    { "nightsight",do_nightsight,POS_FIGHTING,3,LOG_NORMAL,4,DISC_WERE_HAWK,1 },

    { "learn",		do_learn, POS_FIGHTING, 3, LOG_NORMAL, 64,0,0 },

    /*
     * Vampire and werewolf commands.
     */
 //   { "vamparmor",	do_vampirearmor, POS_STANDING, 3, LOG_NORMAL, 8, 0, 0 },
    { "bloodwall",	do_bloodwall,POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_DAIM,2 },
    { "conceal",     do_conceal,  POS_STANDING,  3,  LOG_NORMAL, 8,DISC_VAMP_OBFU,5 },
    { "sharpen",	do_sharpen,  POS_STANDING,     3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,7 },
    { "purification",      do_purification, POS_STANDING, 3, LOG_NORMAL, 0, 0, 0 },

     /* Protean */ /* healing has to go after drow heal */
    /* Obtene */
    { "grab",		do_grab,	POS_STANDING,    	 3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,8 },
//    { "shadowgaze",	do_shadowgaze,POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,10 },

   /* Luna Powers */
    { "flameclaws",	do_flameclaws,  POS_STANDING,  3,  LOG_NORMAL, 4,DISC_WERE_LUNA,1 },
    { "motherstouch",	do_motherstouch,POS_FIGHTING,  3,  LOG_NORMAL, 4,DISC_WERE_LUNA,3 },
    { "gmotherstouch",  do_gmotherstouch,POS_FIGHTING, 3,  LOG_NORMAL, 4,DISC_WERE_LUNA,4 },
    { "sclaws",		do_sclaws,	POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_LUNA,5 },
    { "moonbeam",	do_moonbeam,POS_FIGHTING,      3,  LOG_NORMAL, 4,DISC_WERE_LUNA,8 },
 //   { "moonarmour",	do_moonarmour,POS_STANDING,    3,  LOG_NORMAL, 4,DISC_WERE_LUNA,2 },
 //   { "moongate",	do_moongate,POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_LUNA,6 },
   /* No more luna Powers */

    { "hint",           do_hint,        POS_DEAD,      10,LOG_NORMAL,0,0,0 },
    { "hints",           do_hints,        POS_DEAD,      10,LOG_NORMAL,0,0,0 },
    { "tribe",		do_tribe,	POS_STANDING,	3,LOG_NORMAL,4,0,0},
    { "testform",	do_testform,   POS_DEAD,       12,LOG_NORMAL,0,0,0 },
    { "testrage",	do_werewolf,	POS_STANDING, 12,LOG_NORMAL,0,0,0 },
    { "setdisc", 	do_setdisciplines,POS_DEAD,    10,LOG_ALWAYS, 0,0,0 },
    { "info",		do_info,	POS_DEAD,      10,LOG_NORMAL,0,0,0 },
    { "demongate",	do_dgate,      POS_FIGHTING,   3, LOG_NORMAL, 1,0,0 },
    { "devour",		do_devour,     POS_STANDING,   3, LOG_NORMAL, 4,DISC_WERE_RAPT,5 },
    { "frostbreath",    do_frostbreath,POS_FIGHTING,   3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_GELU,2 },
    { "tick",		do_tick,       POS_DEAD,      12, LOG_ALWAYS, 0,0,0},
    { "distort",	do_form,       POS_DEAD,       10, LOG_ALWAYS, 0,0,0},
    { "resetarea",	do_resetarea,  POS_DEAD,      7, LOG_ALWAYS, 0,0,0},
    { "graft",		do_graft,      POS_STANDING,   3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,5 },
    { "rage",		do_rage,       POS_FIGHTING,   3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,3 },
    { "calm",		do_calm,       POS_STANDING,   3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,4 },
    { "vamptalk",       do_vamptalk,   POS_DEAD,       1, LOG_NORMAL, 8,0,0 },      
    { "obtain",          do_obtain,  POS_STANDING,   3,  LOG_NORMAL, 0,0,0 },
    { "warps",          do_warps,  POS_STANDING,   3,  LOG_NORMAL, 0,0,0 },   
    { "claws",		do_claws,	POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,1 },
    { "claws",          do_claws,       POS_FIGHTING,  3,  LOG_NORMAL, 4,DISC_WERE_WOLF,1 },
    { "fangs",		do_fangs,	POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,2 },
    { "fangs",          do_fangs,       POS_FIGHTING,  3,  LOG_NORMAL, 4,DISC_WERE_WOLF,2 },
    { "horns",		do_horns,	POS_FIGHTING,  3, LOG_NORMAL, 1,DISC_DAEM_ATTA,4 },
    { "blink",		do_blink,	POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_ATTA,7 },
    { "inferno",	do_dinferno,	POS_DEAD,      3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_HELL, 3 },
    { "tail",		do_tail,	POS_FIGHTING,  3,  LOG_NORMAL, 1,0,0 },
    { "web",            do_web, 	POS_FIGHTING,  3,  LOG_NORMAL, 1,DISC_DAEM_CORR,2 },
/* Vamp */
    { "binferno",	do_inferno,	POS_STANDING,  3,  LOG_NORMAL, 1,DISC_VAMP_DAIM, 6 },
/* Vamp ^^^^ */

    { "immolate",	do_immolate,    POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_HELL, 2 },
    { "daemonseed",	do_seed,	POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_HELL, 7 },
    { "hellfire",	do_hellfire,	POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_HELL, 8 },
    { "ban",		do_ban,	POS_DEAD,		 12,  LOG_ALWAYS,0,0,0	},
    { "transfer",       do_transfer,    POS_DEAD,      9, LOG_NORMAL,0,0,0 },
   // { "runeeq",		do_runeeq,	POS_STANDING,    3,  LOG_NORMAL,0,0,0 },

/*holy, chaos, mithril*/
 //   { "holyeq",               	do_holyeq,      POS_STANDING,    3,  LOG_ALWAYS,0,0,0 },
 //   { "mithrileq",              do_mithrileq,   POS_STANDING,    3,  LOG_ALWAYS,0,0,0 },
 //   { "chaoseq",                do_chaoseq,     POS_STANDING,    3,  LOG_ALWAYS,0,0,0 },
      { "aligneq", 	          do_aligneq,     POS_STANDING,    3,  LOG_ALWAYS,0,0,0 },

/*holy, chaos, mithril*/

    { "classeq",		do_classeq,	POS_STANDING,	 3,  LOG_NORMAL,0,0,0 },

   // { "kingeq",		do_kingeq,	POS_STANDING,    3,  LOG_ALWAYS,0,0,0 },
    //{ "kingdomeq",	do_kingdomeq,	POS_STANDING,  3,  LOG_ALWAYS,0,0,0 },
    //{ "increase",		do_meta,	POS_STANDING,  3, LOG_ALWAYS,0,0,0 },  
    { "afk",      	do_afk,         POS_STANDING,    0,  LOG_NORMAL,0,0,0 },
    { "hedit",		do_hedit,	POS_STANDING,  7, LOG_NORMAL,0,0,0 },
    { "freeze",		do_freeze,      POS_DEAD,     10, LOG_ALWAYS,0,0,0 },  
    { "unnerve",	do_unnerve,     POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_DISC,1 },
    { "freezeweapon",	do_wfreeze,     POS_STANDING,  3, LOG_NORMAL, CLASS_DEMON,DISC_DAEM_GELU, 1 },
    { "chaosportal",	do_chaosport,   POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_DISC, 4 },
    { "caust",		do_caust,       POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_CORR, 4 },
    { "gust",		do_gust,	POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_GELU, 7 },
    { "entomb",		do_entomb,      POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_GELU, 6 },
    { "evileye",	do_evileye,     POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_DISC, 2 },
    { "leech",		do_leech,	POS_FIGHTING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_NETH,4 },
    { "deathsense",	do_deathsense,  POS_STANDING,  3,  LOG_NORMAL, CLASS_DEMON,DISC_DAEM_NETH, 2 },
    { "prefix",		do_prefix,	POS_DEAD,      10,  LOG_NORMAL, 0,0,0 },

    /* bugaboo lala Dunkirk Shit Lala mmm POOP Daemon Stuff */


    /* Start of OLC Shit. Hmm */
    { "hset",		do_hset,        POS_DEAD,      12, LOG_ALWAYS, 0,0,0 },
    { "hlist",		do_hlist,	POS_DEAD,      11, LOG_ALWAYS, 0,0,0 },

    { "talons",         do_talons,   POS_FIGHTING,     3,  LOG_NORMAL, 4,DISC_WERE_RAPT,10 },
//    { "run",            do_run,     POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_RAPT,2 },
    { "bonemod",		do_bonemod,	POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_VICI,3 },
    { "vset",		do_vset,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0 },
    { "cauldron",		do_cauldron,POS_FIGHTING,      3, LOG_NORMAL, 8,DISC_VAMP_THAU,2 },
    { "flamehands",	do_flamehands,POS_FIGHTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PROT,5 },
   
    { "summon",		do_summon,	POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_PRES,4 },
    { "shroud",		do_shroud,	POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,1 },
    { "share",		do_share,	POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,4 },
    { "frenzy",		do_frenzy,	POS_FIGHTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,5 },
    { "far",		do_far,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PRES,9 },
    { "awe",            do_awe,     POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_PRES,1 },


    { "forge",		do_forge,	POS_STANDING,  1,LOG_NORMAL,0,0,0 },
    { "forget",         do_forget,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_DOMI,8 },
   // { "vsilence",       do_death,   POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,5 },
    { "flash",          do_flash,   POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,9 },
    { "tide",   	do_tide,    POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,5 },
    { "coil",           do_coil,    POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_SERP,8 },
    { "infirmity",	do_infirmity,POS_FIGHTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_QUIE,2 },
    { "klaive",         do_klaive,  POS_STANDING,      3,  LOG_NORMAL, 4,0,0 },
    { "calm",		do_calm,	POS_STANDING,	 3,  LOG_NORMAL, 4,DISC_WERE_WOLF,3	},
    { "change",		do_change,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PROT,3	},
    { "katana",		do_katana,	POS_RESTING, 3, LOG_NORMAL, CLASS_HIGHLANDER, 0, 0 },
    { "earthshatter",	do_earthshatter,POS_FIGHTING,   3, LOG_NORMAL, 0, 0, 0 },
  //  { "ninjaarmor",          do_ninjaarmor,       POS_STANDING, 3, LOG_NORMAL, CLASS_NINJA, 0, 0 },
  //  { "shadowstep",	do_shadowstep,	POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,4 },
    { "claws",		do_claws,	POS_SITTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PROT,2	},
    { "darkheart",	do_darkheart,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_SERP,1	},
    { "earthmeld",      do_earthmeld,	POS_STANDING,     3,  LOG_NORMAL, 8,DISC_VAMP_PROT,4 },
  //  { "burrow",         do_burrow, 	POS_STANDING,      3,  LOG_NORMAL, 4,DISC_WERE_BOAR,5 },
    { "fangs",		do_fangs,	POS_SITTING,	 3,  LOG_NORMAL, 8,0,0	},
    { "flex",		do_flex,	POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "gcommand",	do_fcommand,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "possession",     do_possession,	POS_STANDING,    3,  LOG_NORMAL, 8,DISC_VAMP_DOMI,3 },
    { "hum",		do_monktalk,	POS_DEAD,		3, LOG_NORMAL, 0, 0, 0},
    { "humanform",	do_humanform,POS_SITTING,	 2,  LOG_NORMAL, 0,0,0 },
//    { "theft",          do_theft,   POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_THAU,4 },   
    { "plasma",         do_plasma,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_VICI,5 },
    { "zuloform",       do_zuloform,POS_FIGHTING,      3,  LOG_NORMAL, 8,DISC_VAMP_VICI,2},
    { "beckon",         do_beckon,  POS_STANDING,      3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,1},
    { "miktalk", do_miktalk, 	POS_SLEEPING, 2, LOG_NORMAL, 128, 0, 0},
    { "principles", do_principles, POS_MEDITATING, 3, LOG_NORMAL, 128, 0, 0 },
    { "michi", do_michi, POS_FIGHTING, 3, LOG_NORMAL, 128, 0, 0 },
    { "harakiri", do_hara_kiri, POS_MEDITATING,3, LOG_NORMAL, 128, 0, 0 },
    { "circle",	do_circle, POS_FIGHTING, 3, LOG_NORMAL, 128, 0, 0},
    { "artteach", do_artteach, POS_STANDING, 3, LOG_NORMAL, 0, 0, 0 },
    { "kakusu", do_kakusu, POS_STANDING, 3, LOG_NORMAL, 128, 0, 0 },
    { "kanzuite", do_kanzuite, POS_MEDITATING, 3, LOG_NORMAL, 128, 0, 0 },
    { "mienaku", do_mienaku, POS_FIGHTING, 3, LOG_NORMAL, 128, 0, 0 },
    { "bomuzite", do_bomuzite, POS_MEDITATING, 3, LOG_NORMAL, 128, 0, 0  },
    { "tsume", do_tsume, POS_FIGHTING, 3, LOG_NORMAL, 128, 0, 0 },
    { "mitsukeru", do_mitsukeru, POS_MEDITATING, 3, LOG_NORMAL, 128, 0, 0 },
    { "koryou", do_koryou, POS_MEDITATING, 3, LOG_NORMAL, 128,0,0},
  	{ "screen", do_screen, POS_STANDING, 3, LOG_NORMAL, 0,0,0},
    { "assassinate",    do_assassinate,POS_STANDING,   3,  LOG_NORMAL, 128, 0, 0 },
    { "web",		do_web,POS_STANDING,	3,	LOG_NORMAL, 128, 0, 0},
      //  { "stalk"       ,do_stalk, POS_STANDING,        3, LOG_NORMAL,128,0,0 },

/*start drow section */
    { "web",		 do_web, POS_FIGHTING,    3,  LOG_NORMAL, 32,0,0 },
    { "sign",		do_sign,	POS_DEAD,	 1,  LOG_NORMAL, 32,0,0 },
    { "relevel",		do_relevel,	POS_DEAD,   	 0,  LOG_ALWAYS, 0,0,0 },
    { "grant",		do_grant,	POS_STANDING,	 3,  LOG_NORMAL, 32,0,0 },
    { "drowsight",	do_drowsight,POS_STANDING,     3,  LOG_NORMAL, 32,0,0 },
    { "drowshield",	do_drowshield,POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "drowfire",		do_drowfire,POS_FIGHTING,      3,  LOG_NORMAL, 32,0,0 },
    { "drowhate",	do_drowhate,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "drowpowers",	do_drowpowers,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "darkness",       do_darkness,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "lloth",		do_lloth,	POS_STANDING,	3,	LOG_NORMAL,32,0,0},
   // { "shadowwalk",	do_shadowwalk,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
//    { "drowcreate",    	do_drowcreate,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "heal",		do_heal,	POS_FIGHTING,    3,  LOG_NORMAL, 32,0,0 },
    { "garotte",	do_garotte,	POS_FIGHTING,    3,  LOG_NORMAL, 32,0,0 },
    { "spiderform",	do_spiderform,	POS_STANDING,    3,  LOG_NORMAL, 32,0,0 },
    { "chaosblast",	do_chaosblast,	POS_FIGHTING,    3,  LOG_NORMAL, 32,0,0 },
  //  { "dgarotte",	do_dark_garotte,POS_STANDING,   3, LOG_NORMAL, 32, 0, 0 },
   // { "glamour",	do_glamour	,POS_RESTING,	3, LOG_NORMAL, 32, 0, 0 },
    { "confuse",	do_confuse	,POS_FIGHTING,  3, LOG_NORMAL, 32, 0, 0 },
    { "darktendrils", do_darktendrils, POS_STANDING,  3, LOG_NORMAL, 32, 0, 0 },
  	{ "fightdance", do_fightdance, POS_STANDING,  3, LOG_NORMAL, 32, 0, 0},
    { "shadowblast",    do_shadowblast,POS_FIGHTING,  3, LOG_NORMAL,  32, 0, 0},
/* end drow */



/*demon section*/

   // { "eyespy",		do_eyespy,      POS_STANDING,    3,  LOG_NORMAL, 1,0,0 },
    { "champions",	do_champions,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
//    { "demonarmour",	do_demonarmour,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "hooves",		do_hooves,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "humanform",	do_humanform,	POS_SITTING,	 2,  LOG_NORMAL, 1,0,0 },
    { "inpart",		do_inpart,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
  //  { "travel",		do_travel,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "weaponform",	do_weaponform,	POS_STANDING,	 2,  LOG_NORMAL, 1,0,0 },
    { "leap",		do_leap,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "wings",		do_wings,	POS_STANDING,	 3,  LOG_NORMAL, 1,0,0 },
    { "demonform",	do_demonform,	POS_STANDING,    3,  LOG_NORMAL, 1,0,0, },

/*end demon section*/



/* vamp protean healing*/
    { "healing",		do_healing, POS_FIGHTING,      3,   LOG_NORMAL, 8,DISC_VAMP_PROT,8},
    { "healingtouch",   do_healingtouch,POS_FIGHTING,3,LOG_NORMAL,64,0,0}, /*monks healing power*/
   // { "inconnu",		do_inconnu,	POS_STANDING,	 3,  LOG_NORMAL, 8,0,0	},
    { "majesty",		do_majesty,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PRES,5	},
    { "nightsight",	do_nightsight,POS_SITTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_PROT,1	},
    { "poison",		do_poison,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_SERP,3	},
    { "rage",		do_rage,	POS_FIGHTING,	 3,  LOG_NORMAL, 4,0,0	},
    { "regenerate",	do_regenerate,POS_SLEEPING,	 3,  LOG_NORMAL, 8,0,0	},
    { "roll",		do_roll,	POS_RESTING,	 2,  LOG_NORMAL, 1,0,0	},
    { "stake",		do_stake,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0 },
    { "serpent",		do_serpent,	POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_SERP,2	},
    { "shadowplane",	do_shadowplane,POS_STANDING,	 3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,3	},
    { "shadowplane",    do_shadowplane,POS_STANDING,     3,  LOG_NORMAL, 4,DISC_WERE_OWL,3     },
    { "shadowsight",	do_shadowsight,POS_SITTING,	 3,  LOG_NORMAL, 8,DISC_VAMP_OBTE,2	},
    { "shadowsight",    do_shadowsight,POS_SITTING,      3,  LOG_NORMAL, 4,DISC_WERE_HAWK,2 },
    { "serenity",       do_serenity,POS_SITTING,       3,  LOG_NORMAL, 8,DISC_VAMP_ANIM,2 },
    { "totems",		do_totems,	POS_SITTING,	 3,  LOG_NORMAL, 4,0,0  },
    { "upkeep",		do_upkeep,	POS_DEAD,	       3, LOG_NORMAL, 0,0,0	},
    { "vanish",		do_vanish,	POS_STANDING,	 3,  LOG_NORMAL, 4,DISC_WERE_OWL,1	},
    { "web",		do_web,	POS_FIGHTING,	 3,  LOG_NORMAL, 4,DISC_WERE_SPID,2	},
/* Tag commands */
    { "healme",		do_healme,	POS_STANDING,   2,       LOG_NORMAL, 0,0,0},
    { "notag", 		do_notag, 	POS_STANDING, 	8,	 LOG_NORMAL, 0, 0, 0 },
    { "bounty", 	do_bounty, 	POS_STANDING, 	0,	 LOG_NORMAL, 0, 0, 0 },
    { "bountylist",	do_bountylist, 	POS_STANDING, 	0,	 LOG_NORMAL, 0, 0, 0 },
    { "ftag",		do_ftag,	POS_SLEEPING,	8,	 LOG_NORMAL, 0,0,0 },
    { "ztag",		do_ztag,	POS_RESTING,	0,	 LOG_NORMAL, 0,0,0 },
    { "tag",            do_ztag,        POS_RESTING,    0,       LOG_NORMAL, 0,0,0 },
    { "red",		do_red,		POS_RESTING,	0,	 LOG_NORMAL, 0,0,0 },
    { "blue",		do_blue,	POS_RESTING,	0,	 LOG_NORMAL, 0,0,0 },
    { "venomspit",	do_stuntubes,	POS_FIGHTING,	3,       LOG_NORMAL, 8192, 0, 0 },
    { "avataroflloth",  do_cubeform,	POS_STANDING,	3,	 LOG_NORMAL, 8192,0,0},
    { "implant",do_implant,POS_STANDING,0,LOG_NORMAL,8192,0,0},
    { "scry",		do_scry,	POS_STANDING,   3,	LOG_NORMAL, 8192,0,0},
    { "readaura",	do_readaura,	POS_STANDING,	3,	LOG_NORMAL, 8192,0,0},
    { "lloth",          do_lloth,       POS_STANDING,   3,       LOG_NORMAL, 8192,0,0},
    { "darkness",       do_darkness,    POS_STANDING,   3,       LOG_NORMAL, 8192,0,0},
    { "llothsight",do_infravision,POS_STANDING,0,LOG_NORMAL,8192,0,0},
//    { "dridereq",   do_dridereq, POS_STANDING, 0,LOG_NORMAL,8192,0,0}, 
    { "web",		do_web,		POS_FIGHTING,	3,	LOG_NORMAL, 8192,0,0},
    { "preach",do_communicate,POS_DEAD,0,LOG_NORMAL,8192,0,0},
//   { "shadowwalk",     do_shadowwalk,  POS_STANDING,    3,  LOG_NORMAL, 8192,0,0 },
/***HUNTER POWERS***/    
//    { "imbue",         do_imbue,       POS_STANDING,    3,  LOG_NORMAL, 65536,0,0},

    /*
     * Immortal commands.
     */
    { "swearlist",      do_swearlist,   POS_DEAD,		  10,  LOG_NORMAL, 0,0,0},
    { "multiplay",      do_multiplay,   POS_DEAD,		  12,  LOG_NORMAL, 0,0,0},
    { "todo",		do_todo,	POS_DEAD,		  9,  LOG_NORMAL, 0,0,0 },
    { "quote",		do_quote,	POS_DEAD,		  0,  LOG_NORMAL, 0,0,0 },
    { "instaroom",	do_instaroom,	POS_DEAD,		  7,  LOG_NORMAL, 0,0,0 },
    { "implag",		do_implag,	POS_DEAD,		 12,  LOG_NORMAL, 0,0,0 },
    { "doublexp",	do_doublexp,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0 },
    { "smite",		do_smite,	POS_DEAD,		 10,  LOG_NORMAL, 0,0,0 },
    { "showchar",	do_showchar,	POS_DEAD,		 12,  LOG_NORMAL, 0,0,0 },
  //  { "airmail",	do_airmail,	POS_DEAD,		  8,  LOG_NORMAL, 0,0,0 },
    { "trust",		do_trust,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0 },
    { "allow",		do_allow,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0	},
    { "secset",		do_secset,	POS_DEAD,		 11,  LOG_NORMAL, 0,0,0	},
    { "addlag",		do_addlag,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0	},
    { "bind",		do_bind,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "clearstats",	do_clearstats,	POS_STANDING,		  0,  LOG_NORMAL, 0,0,0	},
    { "create",		do_create,	POS_STANDING,		  8,  LOG_NORMAL, 0,0,0	},
    { "deny",		do_deny,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "disable",	do_disable,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0 },
    { "disconnect",	do_disconnect,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0 },
    { "divorce",	do_divorce,	POS_DEAD,	 	 9,  LOG_ALWAYS, 0,0,0	},
    { "familiar",	do_familiar,    POS_STANDING,	         12,  LOG_NORMAL, 0,0,0	},
    { "fcommand",	do_fcommand,    POS_STANDING,   	 4,  LOG_NORMAL, 0,0,0	},
    { "marry",		do_marry,	POS_DEAD,	 	 9,  LOG_ALWAYS, 0,0,0	},
    { "paradox",	do_paradox,	POS_DEAD,		 10, LOG_ALWAYS, 0,0,0 },
    { "qset",		do_qset,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "qstat",		do_qstat,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "qtrust",		do_qtrust,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "reboo",		do_reboo,	POS_DEAD,		 11,  LOG_NORMAL, 0,0,0	},
    { "reboot",		do_reboot,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0	},
  /*  { "copyover",	do_hotboot,	POS_DEAD,	         10,  LOG_ALWAYS, 0,0,0	}, */
    { "shutdow",	do_shutdow,	POS_DEAD,	 12,  LOG_NORMAL, 0,0,0	},
    { "shutdown",	do_shutdown,POS_DEAD,		 12, LOG_ALWAYS, 0,0,0	},
    { "users",		do_users,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "omni",		do_omni,	POS_DEAD,		 9,LOG_NORMAL, 0,0,0 },
    { "hreload",	do_hreload,	POS_DEAD,		 10, LOG_NORMAL, 0,0,0 },
    { "wizlock",	do_wizlock,	POS_DEAD,	 11,  LOG_ALWAYS, 0,0,0	},
    { "closemud",	do_closemud,	POS_DEAD,		 12, LOG_ALWAYS,0,0,0, },
    { "watche",		do_huh,	POS_DEAD,	 		 2,  LOG_NORMAL,0,0,0	},
    { "watcher",	do_watcher,	POS_DEAD,	 	 11,  LOG_NORMAL, 
0,0,0	 },
    { "watching",        do_watching,     POS_DEAD,                11,  LOG_NORMAL, 0,0,0    },
    { "showemail",	do_showemail,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0	},
    { "force",		do_force,	POS_DEAD,	 	10,  LOG_ALWAYS, 0,0,0	},
    { "exlist",		do_exlist,	POS_DEAD,		 8, LOG_NORMAL, 0,0,0 },
    { "vlist",		do_vlist,	POS_DEAD,		 8, LOG_NORMAL, 0,0,0 },
    { "mclear",		do_mclear,	POS_DEAD,		 9,  LOG_ALWAYS, 0,0,0	},
    { "clearsuper",         do_clearsuper,      POS_DEAD,                12,  LOG_ALWAYS, 0,0,0  },  
    { "mload",		do_mload,	POS_DEAD,	 	 7,  LOG_ALWAYS, 0,0,0	},
    { "undeny",         do_undeny,  POS_DEAD,          11,  LOG_ALWAYS, 0,0,0 },
   { "resetpassword",	do_resetpassword,	POS_DEAD,	 	 12, LOG_ALWAYS, 0,0,0	},
    { "noemote",		do_noemote,	POS_DEAD,	 	 9,  LOG_NORMAL, 0,0,0	},
    { "notell",		do_notell,	POS_DEAD,	 	 9,  LOG_NORMAL, 0,0,0	},
    { "oclone",		do_oclone,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "oload",		do_oload,	POS_DEAD,	 	 10,  LOG_ALWAYS, 0,0,0	},
    { "oset",		do_oset,	POS_DEAD,	 	 11,LOG_ALWAYS, 0,0,0	},
    { "otransfer",	do_otransfer,POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "pload",		do_pload,	POS_DEAD,		 12,  LOG_ALWAYS, 0,0,0	},
    { "punload",	do_punload,	POS_DEAD,		 12,  LOG_ALWAYS, 0,0,0	},
    { "preturn",		do_preturn,	POS_DEAD,	 	 2,  LOG_NORMAL, 0,0,0	},
    { "pset",		do_pset,	POS_DEAD,		 11,  LOG_ALWAYS, 0,0,0	},
    { "pstat",           do_pstat,        POS_DEAD,                10,  LOG_ALWAYS, 0,0,0 },
    { "purge",		do_purge,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "release",		do_release,	POS_DEAD,	 	 9,  LOG_ALWAYS, 0,0,0	},
    { "restore",		do_restore,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "rset",		do_rset,	POS_DEAD,		 8,  LOG_ALWAYS, 0,0,0	},
    { "silence",		do_silence,	POS_DEAD,	 	 9,  LOG_NORMAL, 0,0,0 },
    { "sla",		do_sla,	POS_DEAD,		 10,  LOG_NORMAL, 0,0,0	},
    { "slay",		do_slay,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "special",		do_special,	POS_DEAD,		 10,  LOG_ALWAYS, 0,0,0	},
    { "decapitate",	do_decapitate,POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "shatter",		do_shatter,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0  },
    { "range",	do_range,	POS_STANDING,		3,   LOG_NORMAL, 0,0,0  },
     { "sset",		do_sset,	POS_DEAD,		 12,  LOG_ALWAYS, 0,0,0 },
   // { "tear",		do_tear,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "gflags",			do_gflags,POS_STANDING, 7, LOG_NORMAL,0,0,0 },
    { "transport",	do_transport,POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "at",			do_at,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "bamfin",		do_bamfin,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "bamfout",		do_bamfout,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "email",		do_email,	POS_DEAD,	0,   LOG_ALWAYS, 0,0,0 },
    { "copyover",       do_hotboot,     POS_DEAD,       11,  LOG_ALWAYS },
    { "echo",		do_echo,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "goto",		do_goto,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "holylight",	do_holylight,POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "incog",          do_incog,   POS_DEAD,          7,  LOG_NORMAL, 0,0,0 },
    { "invis",		do_invis,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "log",		do_log,	POS_DEAD,	 	 12,  LOG_ALWAYS, 0,0,0	},
    { "memory",		do_memory,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "mfind",		do_mfind,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0 },
    { "mstat",		do_mstat,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "mwhere",		do_mwhere,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0 },
    { "ofind",		do_ofind,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0 },
    { "owhere",         do_owhere,      POS_DEAD,                7,  LOG_NORMAL, 0,0,0 },
    { "vshow",		do_vshow,	POS_DEAD,		 7,  LOG_NORMAL, 0,0,0 },
    { "odelete",        do_odelete,     POS_DEAD,              12, LOG_ALWAYS, 0,0,0 },
    { "nohelps",        do_nohelps,     POS_DEAD,                7, LOG_NORMAL, 0,0,0 },
    { "mdelete",        do_mdelete,      POS_DEAD,              12, LOG_ALWAYS, 0,0,0 },
    { "ostat",		do_ostat,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "oswitch",		do_oswitch,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "oreturn",		do_oreturn,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "peace",		do_peace,	POS_DEAD,		 9, LOG_NORMAL, 0,0,0	},
    { "recho",		do_recho,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "immreturn",	do_return,	POS_DEAD,	 	 8, LOG_NORMAL, 0,0,0	},
    { "rstat",		do_rstat,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "rank",           do_rank,        POS_DEAD,               12,  LOG_NORMAL, 0,0,0  },
    { "slookup",		do_slookup,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0 },
    { "snoop",		do_snoop,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "switch",		do_switch,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "gointo",		do_switch,	POS_DEAD,		12,  LOG_NEVER, 0,0,0 },
    { "samtalk",		do_hightalk,POS_DEAD,	 	 1,  LOG_NORMAL, 16,0,0 },
    { "magetalk",		do_magetalk,POS_DEAD,	 	 2,  LOG_NORMAL, 2,0,0	},
    { "vtalk",		do_vamptalk,POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0	},
    { ">",			do_vamptalk,POS_DEAD,	 	 1,  LOG_NORMAL, 0,0,0	},
    { "vampire",		do_vampire,	POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "immune",		do_immune,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "[",			do_fcommand,POS_SITTING,	 3,  LOG_NORMAL, 0,0,0	},
    { "immtalk",		do_immtalk,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { ":",			do_immtalk,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "clannum", 	do_clannum,	POS_DEAD,	10, LOG_NORMAL, 0, 0, 0 },
    { "clanset",	do_clanset,	POS_DEAD,	11, LOG_NORMAL, 0, 0,0 },
  //  { "clanlist",	do_clanlist,	POS_RESTING,	2, LOG_NORMAL, 0, 0,0},
  //  { "clannable",	do_clannable,	POS_RESTING,	2, LOG_NORMAL, 0,0, 0 },
    { "outcast",	do_outcast,	POS_RESTING,	2, LOG_NORMAL, 0,0, 0 },
  //  { "induct",	do_induct,	POS_RESTING,	2, LOG_NORMAL,0,0,0},
  //  { "prince",	  do_prince,	POS_RESTING,  2, LOG_NORMAL, 0,0,0},
  //  { "kingdom",  do_kingdom,	POS_RESTING,  2, LOG_NORMAL, 0,0,0},
  //  { "warlist",  do_warlist,	POS_RESTING,  2, LOG_NORMAL, 0,0,0},
  //  { "decwar",   do_decwar,	POS_RESTING,  2, LOG_NORMAL, 0,0,0},
  //  { "warpeace", do_warpeace,	POS_RESTING,  2, LOG_NORMAL, 0,0,0},
  //  { "ktalk",    do_ktalk,	POS_SLEEPING, 2, LOG_NORMAL, 0,0,0},
  //  { "krecall",  do_krecall,   POS_FIGHTING, 2, LOG_NORMAL, 0,0,0},
  //  { "kwhere",   do_kwhere,    POS_STANDING, 2, LOG_NORMAL, 0,0,0},
    { "thirdeye", do_thirdeye,  POS_STANDING, 3, LOG_NORMAL, 0,0,0},
    { "changes",        do_changes,     POS_STANDING,    0,  LOG_NORMAL  },  
    { "addchange",      do_addchange,   POS_STANDING,    7,  LOG_NORMAL  },
    { "chsave",         do_chsave,      POS_STANDING,    8,  LOG_NORMAL  },
    { "cmdlog",	do_logcmd,		POS_DEAD,	9,	LOG_ALWAYS },    
    { "sysdata",	do_cset,		POS_DEAD,	11, LOG_ALWAYS },    
   // { "makepreg", do_makepreg, 	POS_STANDING,	12, LOG_NORMAL, 0,0,0},
    { "checkbalance", do_checkbalance, POS_DEAD, 9, LOG_NORMAL, 0,0,0},
    { "checkdamcap",	do_checkdamcap, POS_DEAD, 9, LOG_NORMAL, 0,0,0},
    /* New Database shit - JOBO */

    { "showkingdesc", do_showkingdesc, POS_STANDING, 2, LOG_NORMAL,0,0,0},
    { "kingdesc", do_kingdesc,  POS_STANDING, 3, LOG_NORMAL, 0,0,0},
    { "leader",  do_leader,  POS_STANDING,  2,  LOG_NORMAL, 0,0,0},
    { "leaderclear", do_leaderclear, POS_DEAD, 11, LOG_NORMAL, 0,0,0},
    { "cls",	do_cls,	POS_DEAD,	1, LOG_NORMAL, 0, 0, 0},
    { "socedit",do_sedit,	POS_DEAD,	10, LOG_NORMAL, 0, 0, 0},
    { "gsocial",	do_gsocial,	POS_RESTING, 1, LOG_NORMAL, 0, 0, 0},
    { "global",	do_global,	POS_RESTING, 1, LOG_NORMAL, 0, 0, 0},
    { "nuke",	do_nuke,	POS_DEAD,	12, LOG_ALWAYS, 0, 0, 0},
    { "delete",	do_delete,	POS_STANDING, 1, LOG_ALWAYS,0,0,0},
    /*
     * OLC 1.1b
     */
    { "aedit",          do_aedit,       POS_DEAD,    7,  LOG_NORMAL, 0, 0,0 },
    { "redit",          do_redit,       POS_DEAD,    7,  LOG_NORMAL, 0, 0,0 },
    { "oedit",          do_oedit,       POS_DEAD,    7,  LOG_NORMAL, 0, 0,0 },
    { "asave",          do_asave,       POS_DEAD,    7,  LOG_NORMAL, 0, 0,0 },
    { "alist",          do_alist,       POS_DEAD,    7,  LOG_NORMAL, 0, 0,0 },
    { "resets",         do_resets,      POS_DEAD,    7,  LOG_NORMAL, 0, 0,0 },
    { "relearn",         do_relearn,      POS_DEAD,    0,  LOG_NORMAL, 0, 0,0 },

    /*
     * End of list.
     */
    { "",		0,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	}
};




/*
 * The X-social table.
 * Add new X-socials here.
 * Alphabetical order is not required.
 */
const	struct	xsocial_type	xsocial_table [] =
{

    {
	"x-earlobe",
	"On whom do you wish to do this?",
	NULL,
	"You gently tug on $M earlobe with your teeth.",
	"$n gently tugs on $N's earlobe with $s teeth.",
	"$n gently tugs on your earlobe with $s teeth.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

{
"x-kneel",
"Drop to your knees before who?",
NULL,
"You drop to your knees before $M, begging for mercy.",
"$n drops to $s knees before $N, begging for $M mercy.",
"$n drops to $s knees before you, begging for mercy.",
"Nope.",
NULL,
0, 0, 0, 7, 7, FALSE
},
    {
	"x-french",
	"On whom do you wish to do this?",
	NULL,
	"You give $M a deep throbbing kiss, rolling your tongue around $S.",
	"$n gives $N a deep throbbing kiss.",
	"$n gives you a deep throbbing kiss, rolling $s tongue around yours.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

    {
	"x-kissneck",
	"On whom do you wish to do this?",
	NULL,
	"You slowly and softly kiss and nuzzle $M neck.",
	"$n slowly and softly kisses and nuzzles $N's neck.",
	"$n slowly and softly kisses and nuzzles your neck.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

{
"x-wcream",
"On whom do you wish to do this?",
NULL,
"You spray whipped cream all over $M body, licking it up with your tongue.",
"$n coats $N's body in whipped cream and then licks it up with their tongue.",
"$n covers your body in whipped cream, and you moan as it gets licked up with their tongue.",
"No.",
NULL,
0, 0, 0, 5, 5, FALSE
},
{
"x-ice",
"On who?",
NULL,
"You run a cold piece of ice down $M back, following up with your tongue.",
"$n runs a piece of ice over $N's back, following up with their tongue.",
"$n runs a piece of ice over your burning hot flesh, following up with their tongue.",
"No.",
NULL,
0, 0, 0, 6, 8, FALSE
},

{
"x-whip",
"Who's been bad?",
NULL,
"You smirk slightly as you crack a whip over $N's tender skin, making $M whimper.",
"$n grins slightly as he whips $N's tender flesh, making $M whimper.",
"$n grins wickedly as they whip your tender flesh, making you whimper slightly in fear.",
"No.",
NULL,
0, 0, 0, 9, 2, FALSE
},
    {
	"x-moan",
	"On whom do you wish to do this?",
	NULL,
	"You start moaning 'Oh $N...oh yes...don't stop...mmMMmm...'",
	"$n starts moaning 'Oh $N...oh yes...don't stop...mmMMmm...'",
	"$n starts moaning 'Oh $N...oh yes...don't stop...mmMMmm...'",
	"Not on yourself!",
	NULL,
	0, 2, 0, 1, 1, FALSE
    },

    {
	"x-nuttella",
	"On whom do you wish to do this?",
	NULL,
	"You cover $N's naked flesh with a popular choc spread, before licking it off.",
	"$n smears a chocolate spread over $N's body, licking $M clean with relish",
	"$n smears a popular choc spread on your naked flesh, licking it off you",
	"Not on yourself!",
	NULL,
	0, 0, 0, 5, 20, FALSE
    },

    {
	"x-stroke",
	"On whom do you wish to do this?",
	NULL,
	"You lightly run your fingers along the insides of $S thighs.",
	"$n lightly runs $s fingers along the insides of $N's thighs.",
	"$n lightly runs $s fingers along the insides of your thighs.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 5, 10, FALSE
    },

    {
	"x-tender",
	"On whom do you wish to do this?",
	NULL,
	"You run your fingers through $S hair and kiss $M tenderly on the lips.",
	"$n runs $s fingers through $N's hair and kisses $M tenderly on the lips.",
	"$n runs $s fingers through your hair and kisses you tenderly on the lips.",
	"Not on yourself!",
	NULL,
	0, 0, 0, 1, 1, FALSE
    },

    {
	"x-tie",
	"On whom do you wish to do this?",
	NULL,
	"You tie $N to the bed posts!",
	"$n ties $N's body, fixing $M helplessly on the bed.  Oh!! Kinky!",
	"$n ties you to the bed post, leaving you completely at $s mercy",
	"Not on yourself!",
	NULL,
	0, 0, 0, 15, 10, FALSE
    },

	{
	"x-gag",
	"Gag whom?",
	NULL,
	"You tear off a small strip of $N's clothing, and stuff it into their mouth, tying it back behind their head to secure it.",
	"$n reaches down and tears off a small strip of $N's clothing, stuffing it into $S mouth and tying it behind $S head, effectively gagging $S.",
	"$n tears off a strip of your clothing, and as you look up at $s, $s shoves it into your mouth and ties it behind your head, gagging you.",
	"No.",
	NULL,
	0, 0, 0, 5, 2, FALSE
	},

	{
	"x-blindfold",
	"Blindfold who?",
	NULL,
	"You take out a black silk blindfold, and place it over $N's eyes, tying it securely.",
	"$n takes out a black silk blindfold, and places it over $N's eyes, tying it securely behind $S.",
	"$n takes out a black silk blindfold, and grins as $s places it over your eyes, tying it behind your head.",
	"No.",
	NULL,
	0, 0, 0, 7, 2, FALSE
	},

	{
    "x-knife",
	"On whom do you wish to do this?",
    NULL,
    "You lightly run a cool knife over $N's skin, letting it slide across their body and make weak cuts in their flesh.",
    "$n lightly traces a knife over $N's skin, smiling as it makes a few shallow cuts, drawing a bit of blood.",
    "$n lightly runs a knife over your skin, and you shiver a bit as it makes a few shallow cuts, drawing a bit of your blood.",
    "No.",
    NULL,
    0, 0, 0, 15, 3, FALSE
	}, 

    {
	"x-withdraw",
	"On whom do you wish to do this?",
	NULL,
	"You gently pull yourself free of $M.",
	"$n gently pulls $mself free of $N.",
	"$n gently pulls $mself free of you.",
	"Not on yourself!",
	NULL,
	0, 1, 0, 0, 0, FALSE
    },

	{
	"xl-candle",
	"On who?",
	NULL,
	"You produce a thick white candle, and you step behind $N with it, grinning a bit as you run it up and down her pussy lips.  After a moment you push it in and twist it, starting to slowly pump it in and out.",
	"$n produces a thick white candle, and walks behind $N, holding it in her hands.  She grins a bit as she pushes it to the girl's wet pussy lips, and pushes it in, twisting it as she begins to pump it in and out.",
	"$n produces a thick white candle, and steps behind you as you whimper a bit.  After a moment, you feel it running up and down your cunt lips, and you let out a moan as it slips inside you, twisting as it slides in and out.",
	"No.",
	NULL,
	3, 0, 0, 17, 39, FALSE
	},
{
"xl-afinger",
"Use this on who?",
NULL,
"You take your finger to $N's mouth and have her lick it before taking it to her puckered ass and slowly slipping it in.",
"$N sucks lightly on $n's finger a moment before she takes it between $N's ass cheeks and pushes it in to the knuckle, twisting.",
"$n brings her finger to your lips, and you suck on it a bit before she pulls it away.  A moment later, she presses it to your tight little ass and pushes it in, twisting as you whimper softly.",
"No.",
NULL,
3, 0, 0, 20, 25, FALSE
},

	{
    "xl-finger",
	"On whom do you wish to do this?",
    NULL,
    "You spread $N's thighs and slip your finger between them, tracing it over her lips a few times before pushing it past and into her hot and wet pink folds.",
    "$n spreads $N's thighs and plunges her finger into her twat, pumping it out a few times as sweet, sticky juice spills over it.",
    "$n spreads your legs and begins to trace her finger over your moistened lips, and you let out a soft moan as she slips it past them and into your hot snatch, plunging it in and out as it collects your juices.",
    "No.",
    NULL,
    3, 0, 250, 19, 30, FALSE
	},

	{
	"xl-gag",
	"Gag who?",
	NULL,
	"You pick up your soaked panties, and wad them up into a ball, stuffing them into $N's mouth to gag her.",
	"$n picks up her drenched panties, wadding them into a ball and stuffing them into $N's mouth, gagging her.",
	"$n picks up her panties, soaked through with her cunt juice, and wads them into a ball, stuffing them into your mouth to gag you.",
	"No.",
	NULL,
	3, 0, 0, 7, 9, FALSE
	},

	{
    "xl-stroke",
	"On whom do you wish to do this?",
    NULL,
    "You slip your finger between $N's legs and gently stroke her throbbing clit, making her squirm and moan.",
    "$n slips her finger between $N's damp thighs and begins to softly stroke her clit, making the poor girl squirm and moan in pleasure.",
    "$n slips her finger between your thighs and begins to softly stroke your throbbing clit, making you shut your eyes and moan in pleasure.",
    "No.",
    NULL,
    3, 0, 250, 10, 17, FALSE
	},

	{
    "xl-undress",
	"On whom do you wish to do this?",
    NULL,
    "You reach back and unhook your bra, letting it slide to the floor with your panties.",
    "$n reaches back and unhooks her bra, letting it fall to the floor along with her panties.",
    "$n reaches back and unhooks her bra, letting it fall to the floor along with her panties.",
    "Pick a valid target.",
    NULL,
    3, 0, 250, 15, 15, FALSE
	}, 

	{
    "xl-lick",
	"On whom do you wish to do this?",
    NULL,
    "You bury your face between $N's thighs, letting your tongue slide over her swollen pussy lips and gently tease her clit.",
    "$n pushes $N to the floor and buries her face between her thighs, letting her tongue slide over $N's pussy lips and caress her clit.",
    "$n pushes you to the floor, burying her face between your damp thighs as she lets her tongue caress your swollen pussy lips, and you squirm as it lightly runs over your clit.",
    "No.",
    NULL,
    3, 0, 250, 25, 37, FALSE 
	},

	{ 
    "xl-69",
	"On whom do you wish to do this?",
    NULL, 
    "You lie down on your back and motion for $N to straddle your face, moaning as she licks at you while you hungrily devour her wet snatch.", 
    "$N lowers her pussy over $n's face and places her face between her thighs, both of them gasping as their tongues dart in and out of each other.",
    "$n pulls you on top of her, placing your hot, wet pussy right over her face, and you do the same, both of you panting and moaning as you devour each other.", 
    "No.", 
    NULL, 
    3, 1, 250, 40, 40, FALSE 
	},

	{ 
    "xl-breasts",
	"On whom do you wish to do this?",
    NULL, 
    "You lean forward and place your hands over $N's supple breasts, kneading them softly as she moans.",
    "$n places her hands over $N's soft breasts and begins to gently knead them, smiling as she begins to moan.", 
    "$n places her hands over your soft breasts and begins to knead them, a smile playing over her face as you start moaning.", 
    "No.",
    NULL,
    3, 0, 250, 10, 18, FALSE 
	}, 
	
	{
	"xf-oral",
	"On whom do you wish to do this?",
	NULL,
	"You take $N's hot member in your hands, licking his cock from base to tip.",
	"$n takes $N's penis in $s hands, licking $N's cock from base to tip.",
    "You let out a small moan as $n takes your cock into her hands, licking it from your balls to the tip.",
	"On yourself? I'm impressed!",
	NULL,
	2, 0, 0, 5, 15, FALSE
    },

	{
	"xf-spank",
	"Who's been naughty?",
	NULL,
	"You take $N over your knee and spank him hard, making him squirm.",
	"$n bends $N over her knees and spanks him hard on the ass, making him squirm.",
	"You whimper softly as $n takes you over her knee, spanking you hard on the ass, making it sting.",
	"Nah.",
	NULL,
	2, 0, 0, 25, 4, FALSE
	},

	{
	"xf-urine",
	"On whom do you wish to do this?",
	NULL,
	"You maliciously lean $N's head back and stand over him with his mouth open before letting a hard stream of piss shoot down his throat.",
	"$n shoots a golden stream of urine down $N's throat, making him choke and swallow.",
	"You eyes widen as your mistress shoots a stream of piss odown your throat, making you swallow it all.",
	"Nope.",
	NULL,
	2, 0, 0, 37, -10, FALSE
	},
    
	{
	"xf-beg",
	"Beg who, you stupid slut?",
	NULL,
	"You drop to your kneees before $N, begging for him to abuse you and fuck you hard.",
	"$n drops to her knees and begs for $N to take her and use her for his pleasure.",
	"$n drops to her knees before you, begging for you to abuse her worthless slut body.",
	"It doesn't work that way, you stupid whore.",
	NULL,
	2, 0, 0, -10, 12, FALSE
	},

	{
	"xf-blowjob",
	"On whom do you wish to do this?",
	NULL,
	"You take $N's hot member in your mouth, sucking $S shaft.",
	"$n takes $N's throbbing penis in $s mouth, sucking $N's cock.",
	"You gasp as $n takes your penis in $s mouth, licking your head.",
	"On yourself? I'm impressed!",
	NULL,
	2, 1, 250, 10, 25, FALSE
    },

	{
       "xf-tip",
       "On whom do you wish to do this?",
       NULL,
       "You gently run the tip of your tongue along the underside of $N's hot cock.",
       "$n runs the tip of $s tongue along the bottom of $N's pulsing cock.",
       "You let out a low moan as $n runs the tip of her tongue along the underside of your cock.",
       "I think not.",
       NULL,
         2, 0, 0, 5, 59, FALSE
	},

	{
	"xf-handjob",
	"On who?",
	NULL,
	"You take his hot cock in your hand and wrap your fingers around it, slowly pumping up and down.",
	"$n takes $N's hot cock in her hands, wrapping her fingers around it as she slowly pumps up and down.",
	"$n takes your hot cock into her hands, and you moan softly as she wraps her fingers around it, slowly pumping up and down.",
	"No.",
	NULL,
	2, 0, 0, 9, 14, FALSE
	},

	{
	"xf-sitface",
	"Who gets to taste your sweet juice?",
	NULL,
	"You push $N onto his back and crawl up his body, sitting down on his face as he licks at your pussy.",
	"$n pushes $N onto his back and straddles his face, moaning as he licks her hot pussy.",
	"$n pushes you onto your back and straddles your face, moaning and squirming as you eagerly lap at her hot, wet cunt.",
	"No.",
	NULL,
	2, 1, 250, 15, 0, FALSE
	},

	{
	"xf-69",
	"Who?",
	NULL,
	"You crawl on top of $N and takes his hot cock between your lips, moaning as he laps at your cunt.",
	"$n pushes $N onto his back and crawls on top of him, sucking his cock while he wriggles his tongue about in her cunt.",
	"$n pushes you onto your back, crawling on top of you as she sucks on your cock, moaning while you lick her pussy.",
	"No.",
	NULL,
	2, 1, 250, 20, 20, TRUE
	},

    {
	"xf-breasts",
	"On whom do you wish to do this?",
	NULL,
	"You take $S hands and press them to your breasts.",
	"$n takes $N's hands and presses them to $m breasts.",
	"$n takes your hands and presses them to $m breasts.",
	"Not on yourself!",
	NULL,
	2, 0, 0, 5, 10, FALSE
    },

{
"xf-cum",
"Wrong...",
NULL,
"You feel your muscles tighten for a moment, and you let out a loud moan and arch your back as you release, cumming hard as your juices run down your thighs.",
"$n arches her back and lets out a loud moan, gasping and writhing in pleasure as she cums, her juices running down her thighs.",
"$n arches her back and lets out a loud moan, gasping and writhing in pleasure as her juices gush down her thighs.",
"No.",
NULL,
2, 1, 0, 500, 25, FALSE
},
{ 
"xf-cface",
"Wrong...",
NULL,
"You bite down hard on your lip, letting out a loud moan as you climax, gushing your sticky, wet cum all over $N's face, glazing it over.",
"$n bites down on her lip and lets out a loud cry of pleasure, gushing her juices all over $N's face as he sucks and slurps at her dripping wet pussy.",
"$n bites down on her lip and lets out a loud moan as you slurp at her dripping wet cunt, and she shudders before gushing her sweet juices all over your face, glazing it completely over.",
"No.",
NULL,
2, 1, 0, 500, 30, FALSE
},
{
"xf-chands",
"No...",
NULL,
"You keep sliding your hands back and forth over $N's cock as it grows hotter and hotter, not stopping until he lets out a low groan and fills your hand with his hot, sticky cum.",
"$n slides her hands back and forth over $N's cock as he moans, eventually gritting his teeth and shutting his eyes as he fills her hand full of his hot sticky load of cum, most of it oozing out from between her fingertips.", 
"$n slides her hands over your cock as it grows hotter and hotter, and you let out a low moan as you grit your teeth and shoot a hot, sticky load of cum into her closed hands, filling it up as most of your gooey sperm drips out from between her fingertips.",
"No.",
NULL,
2, 1, 0, 30, 500, FALSE
},

	{
"xf-cbreasts",
"No..",
NULL,
"You take $N's twitching cock between your hands, and aim it straight at your heaving tits, jerking him off until you feel stream after stream of hot, white cum splatter against them and trickle over your stiff nipples.",
"$n takes $N's stiff cock in her hands and aims it at her soft tits, giving him a few quick jerks to finish him off as he shoots out a jet of hot, sticky cum all over her ample mounds and hard nipples.",
"$n takes your cock into her hands and aims it straight at her tits, giving you a few quick jerks to finish you off, and you moan as you shoot your sticky white seed all over her soft breasts, your cum oozing down and trickling over her nipples.",
"No.",
NULL,
1, 1, 0, 37, 500, FALSE
},

	{
      "xf-rub",
      "Who gets to feel the magic touch?",
	  NULL,
      "You gently run your fingers over $N's hardening cock.",
      "$n gently runs her fingers over $N's hardening cock.",
      "$n gently runs her fingers over your hardening cock.",
      "You don't have a penis.  At least, I hope not.",
      NULL,
      2, 0, 0, 15, 20, FALSE
	},
    {
	"xf-contract",
	"On whom do you wish to do this?",
	NULL,
	"You contract your vaginal muscles, driving $M wild.",
	"$n contracts $m vaginal muscles, driving $N wild.",
	"$n contracts $m vaginal muscles, driving you wild.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 10, 15, TRUE
    },

{
"xf-afinger",
"Use this on who?",
NULL,
"You lick your finger and press it to $N's ass, pushing lightly until it pops in and he lets out a small moan.",
"$n licks her finger and takes it to $N's ass, popping it in and gently pumping it back and forth, making him moan.",
"$n licks her finger and takes it to your ass, pushing it in and pumping it in and out, making your dick rock-hard as you moan.",
"No.",
NULL,
2, 1, 0, 20, 23, FALSE
},

    {
	"xf-finger",
	"On whom do you wish to do this?",
	NULL,
	"You put your hands between your legs and begin to masterbate for $N.",
	"$n puts $m hands between $m legs and begins to masterbate for $N.",
	"$n puts $m hands between $m legs and begins to masterbate for your viewing pleasure.  What a turn on!.",
	"Not on yourself!",
	NULL,
	2, 0, 0, 20, 10, FALSE
    },
    {
	"xf-floor",
	"On whom do you wish to do this?",
	NULL,
	"You lie on your back, and pull $M between your parted legs.",
	"$n lies on $m back, and pulls $N between $m parted legs.",
	"$n lies on $m back, and pulls you between $m parted legs.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 25, 20, TRUE
    },

    {
	"xf-grind",
	"On whom do you wish to do this?",
	NULL,
	"You grind your hips up to meet $S thrusts.",
	"$n grinds $m hips up to meet $N's thrusts.",
	"$n grinds $m hips up to meet your thrusts.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 15, 10, TRUE
    },

    {
	"xf-mount",
	"On whom do you wish to do this?",
	NULL,
	"You push $M onto $S back, and slowly lower yourself onto $S erection.",
	"$n pushes $N onto $S back, and slowly lowers $mself onto $S erection.",
	"$n pushes you onto your back, and slowly lowers $mself onto your erection.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 25, 20, TRUE
    },

    {
	"xf-nails",
	"On whom do you wish to do this?",
	NULL,
	"You scratch your nails down $S back.",
	"$n scratches $m nails down $N's back.",
	"$n scratches $m nails down your back.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 5, 1, TRUE
    },

    {
	"xf-pull",
	"On whom do you wish to do this?",
	NULL,
	"You wrap your arms and legs around $M and pull $M into you.",
	"$n wraps $m arms and legs around $N and pulls $M into $m.",
	"$n wraps $m arms and legs around you and pulls you into $m.",
	"Not on yourself!",
	NULL,
	2, 1, 250, 15, 10, TRUE
    },

    {
	"xf-squeeze",
	"On whom do you wish to do this?",
	NULL,
	"Your wrap your legs around $M and squeeze tightly.",
	"$n wraps $m legs around $N and squeezes tightly.",
	"$n wraps $m legs around you and squeezes tightly.",
	"Not on yourself!",
	NULL,
	2, 2, 0, 15, 10, TRUE
    },

    {
	"xf-titfuck",
	"Whom do you wish to treat to this?",
	NULL,
	"You take $N's cock and place it between your breasts, as $N gently thrusts.",
	"$n takes $N's penis, places it between $s breasts, and wanks $M off.",
	"$n takes your cock, places it between her breasts, and wanks you gently with them.",
	"Not on yourself!",
	NULL,
	2, 1, 0, 10, 20, FALSE
    },

{
"xf-fondle",
"On whom do you wish to perform this?",
NULL,
"You reach down and gently fondle $N's warm penis.",
"$n takes $N's cock into her hands, fondling it gently.",
"$n takes your warm cock into her hands, fondling you gently.",
"No.",
NULL,
2, 0, 0, 5, 13, FALSE
},

{
"xf-bob",
"On whom do you wish to perform this?",
NULL,
"You wrap your lips tight around $N's cock and bob your head back and forth, forcing him into your mouth.",
"$n wraps her lips over $N's cock and bobs her head back and forth, making him fuck her mouth.",
"$n wraps her lips tight around your cock and bobs her head back and forth, pumping your length in and out of her mouth.",
"No.",
NULL,
2, 1, 0, 20, 25, TRUE
},

    {
	"xf-undress",
	"On whom do you wish to do this?",
	NULL,
	"You push $N onto the floor, a gleam in your eyes, and tear $S clothes off.",
	"$n pushes $N to the floor, tears $S clothes from $S body.",
	"$n pushes you to the floor, grins, and rips your clothes from your body.",
	"Not on yourself!",
	NULL,
	2, 0, 0, 1, 1, FALSE
    },

    {
	"xm-anal",
	"On whom do you wish to do this?",
	NULL,
	"$N screams in extacy as you come from behind and penetrate $M tight ass from behind.",
	"$n comes from behind $N and penetrates $M tight ass from behind.",
	"You scream as $n comes from behind and penetrates your tight ass from behind!",
	"Not on yourself!",
	NULL,
	1, 1, 250, 25, 25, FALSE
    },

    {
	"xm-spank",
	"Who's the naughty whore?",
	NULL,
	"You grab $N by the hair and throw her over your knees, slapping her hard on the ass.",
	"$n visciously throws $N over his knee, spanking her relentlessly on her tight ass.",
	"You cry out softly as $n visciously takes you over his knee, spanking you mercilessly.",
	"Nah.",
	NULL,
	1, 0, 0, 33, -14, FALSE
	},

	{
	"xm-69",
	"Whom do you wish to perform this on?",
	NULL,
	"You pull $N on top of you and spread her legs, burying your face in her pussy as she sucks on your hard cock.",
	"$n pulls $N on top of him and buries his face between her legs, eating her out as she goes down on his cock.",
	"$n pulls you on top of him and spreads your legs, burying his face between your legs and sucking on your clit as you takes his cock into your mouth.",
	"No.",
	NULL,
	1, 1, 0, 24, 24, TRUE
	},

	{
	"xm-urine",
	"Who gets to taste it?",
	NULL,
	"You force open $N's slutty mouth and shoot a hot stream of piss into it, making her swallow.",
	"$n forces open $Ns mouth and shoots a hot stream of golden piss into it, making her swallow.",
	"$n forces open your slutty mouth and shoots a hot, golden stream of his piss into it, making you swallow.",
	"No.",
	NULL,
	1, 0, 0, 40, -19, FALSE
	},

	{
	"xm-beg",
	"Beg who, you worthless bitchboy?",
	NULL,
	"You drop to your knees, tears in your eyes, and beg for $N to take you as hers.",
	"$n drops to his knees before $N, begging for her to take him as hers.",
	"$n drops to his knees before you with tears in his eyes, sobbing as he begs for you to take him as your own personal toy.",
	"You worthless slut, you can't even do this right.",
	NULL,
	1, 0, 0, -15, 30, FALSE
	},

	{
	"xm-facial",
	"Who gets to have their slutty face done over?",
	NULL,
	"You groan as you shoot your load across her delicate features, your hot cum coating her face and dripping from her chin.",
	"$n grits his teeth as he shoots his sticky cum over $N's face, moaning as she licks her lips and lets it drip down.",
	"$n's eyes widen as he shoots his hot, sticky cum over your face, letting it drip from your chin and trickle over your lips.",
	"Nope.",
	NULL,
	1, 1, 0, 300, 20, TRUE
	},
	
	{
	"xm-cum",
	"So close, yet, so far away?",
	NULL,
	"You let out a low moan as you release your load into $N, filling her up with your hot cum.",
	"$n cries out as he shoots his hot, sticky cum all inside of $N, his creamy load shooting home.",
	"$n grits his teeth and shoots his sticky cum inside of yor whorish body, filling you up until it drips out.",
	"Nopers.",
	NULL,
	1, 2, 0, 300, 20, TRUE
	},

{
"xm-chair",
"Cum in whose hair?",
NULL,
"You pull your hard cock out of $N's hole, and force her to her knees before you as you finish yourself off with your hand, shooting your gooey wad straight into her hair and using a handfull of it to wipe off the sticky head.",
"$n pulls his cock out of $N's hole, and forces her to her knees, jerking himself off for a moment before shooting his hot, sticky sperm into her hair, and he takes a handfull of it to wipe off the head of his still-stiff meat.",
"$n pulls himself out of your hole, and forces you to your knees before him, jerking off into your hair as you look up.  A hot, gooey splatter of cum smacks against your scalp, and he takes a handfull of your hair to wipe off the still-sticky head of his cock.",
"No.",
NULL,
1, 1, 0, 500, 47, FALSE
},

{
"xm-canal",
"Cum in whose ass?",
NULL,
"You pump your cock in and out of $N's ass a few more times before grunting and shooting your sticky load of cum right up it, feeling it fill and trickle over your still-hard cock and down her thighs.",
"$n pumps into $N's tight ass a few more times before he grunts, holding his cock there as he shoots her full of cum, and she lets out a tiny gasp as a little bit oozes out of her tight ass and trickles down her thighs.",
"$n shoves his cock into your tight little ass a few more times, and you hear a grunt from behind you as he starts to cum, shooting a sticky white jet of his cream straight up your ass, and you feel a bit of it trickle out and run down your thighs.",
"No.",
NULL,
1, 1, 0, 500, 47, FALSE
},


{
"xm-cbreasts",
"So close, yet, so far away?",
NULL,
"You let out a low moan as you release your load into $N, filling her up with your hot cum.",
"$n cries out as he shoots his hot, sticky cum all inside of $N, his creamy load shooting home.",
"$n grits his teeth and shoots his sticky cum inside of yor whorish body, filling you up until it drips out.",
"Nopers.",
NULL,
1, 1, 0, 300, 20, TRUE
},
	{
	"xm-chand",
	"So close, yet, so far away?",
	NULL,
	"You let out a low moan as you release your load into $N, filling her up with your hot cum.",
	"$n cries out as he shoots his hot, sticky cum all inside of $N, his creamy load shooting home.",
	"$n grits his teeth and shoots his sticky cum inside of yor whorish body, filling you up until it drips out.",
	"Nopers.",
	NULL,
	1, 1, 0, 300, 20, TRUE
	},

	{
	"xm-stomach",
	"Cum on whose stomach?",
	NULL,
	"You pull out of $N and stand up, pushing her to her back.  After a few quick jerks on your stiff cock, you let out a moan and start to cum, spraying your hot, gooey cum all over her smooth body.",
	"$n pulls out of $N and stands up, pushing her to her back.  After a few quick jerks on his stiff cock, he closes his eyes and lets out a moan, a stream of gooey whip cum spraying from the swollen head of his cock and all over $N's smooth skin.",
	"$n pulls out of you and pushes you onto your back.  As you look up, he gives his hard cock a few quick jerks, and he moans as he starts to cum, a spray of hot, gooey white cum splattering all over your body, coating you from head to toe.",
	"No.",
	NULL,
	1, 1, 0, 500, 50, TRUE
	},

	{
	"xm-breasts",
	"On whom do you wish to do this?",
	NULL,
	"You gently run the tip of your tongue across $M naked breasts.",
	"$n gently runs the tip of $s tongue across $N's naked breasts.",
	"$n gently runs the tip of $s tongue across your naked breasts.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

    {
	"xm-cup",
	"On whom do you wish to do this?",
	NULL,
	"You cup $N's breasts in your hands, and caress $S nipples.",
	"$n cups $N's breasts in $s hands and caress' $S nipples.",
	"$n cups your breasts in $s palms and caress' your nipples.",
	"ON YOURSELF??",
	NULL,
	1, 0, 0, 1, 5, FALSE
    },

    {
	"xm-doggy",
	"On whom do you wish to do this?",
	NULL,
	"You roll $M onto all fours and penetrate $M hot flesh from behind.",
	"$n rolls $N onto all fours and penetrates $M hot flesh from behind.",
	"$n rolls you onto all fours and penetrates your hot flesh from behind.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 25, 15, FALSE
    },

    {
	"xm-finger",
	"On whom do you wish to do this?",
	NULL,
	"You slide your fingers between $M legs, gently stroking $M clitoris.",
	"$n slides $s fingers between $N's legs, gently stroking $M clitoris.",
	"$n slides $s fingers between your legs, gently stroking your clitoris.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 0, 10, FALSE
    },

    {
	"xm-fist",
	"On whom do you wish to do this? I hope you asked!",
	NULL,
	"You make a fist and thrust it up $N's spread wide vagina as she screams with pleasure.",
	"$N gasps as $n pushes $s hand up between $S legs into $S vagina.",
	"$n spreads your legs wide, and thrusts $s hand up your vagina making you cry out in pleasure.",
	"C'est non possible.. I hope...",
	NULL,
        1, 1, 0, 5, 15, FALSE
    },

    {
	"xm-floor",
	"On whom do you wish to do this?",
	NULL,
	"You lower $M to the floor, and slide your body between $M parted legs.",
	"$n lowers $N to the floor, and slides $s body between $M parted legs.",
	"$n lowers you to the floor, and slides $s body between your parted legs.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 15, 10, TRUE
    },

    {
	"xm-jerkoff",
	"On whom do you wish to do this?",
	NULL,
	"You grab your penis and begin to jerkoff for $N.",
	"$n grabs your penis and begins to jerkoff for $N.",
	"$n grab his penis and begin to jerkoff for your viewing pleasure.  What a turn on!.",
	"Not on yourself!",
	NULL,
	1, 1, 0, 25, 5, FALSE
    },

    {
	"xm-nipple",
	"On whom do you wish to do this?",
	NULL,
	"You gently twist $M nipple between your thumb and forefinger.",
	"$n gently twists $N's nipple between $s thumb and forefinger.",
	"$n gently twists your nipple between $s thumb and forefinger.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

    {
	"xm-oral",
	"On whom do you wish to do this?",
	NULL,
	"$N squirms in delight as you bend down and run your tongue along and into $S vagina.",
	"$N squirms ecstatically as $n licks and kisses $S loveslit.",
	"$n wriggles $s tongue about between your legs, making you squirm in ecstacy.",
	"Biologically impossible I think you'll find!",
	NULL,
	1, 0, 0, 10, 25, FALSE
    },

    {
	"xm-press",
	"On whom do you wish to do this?",
	NULL,
	"You press $M against the wall, pulling $M legs around your hips.",
	"$n presses $N against the wall, pulling $M legs around $s hips.",
	"$n presses you against the wall, pulling your legs around $s hips.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 25, 20, TRUE
    },

    {
	"xm-pull",
	"On whom do you wish to do this?",
	NULL,
	"You grab $M around the hips and pull $M firmly onto your erection.",
	"$n grabs $N around the hips and pull $M firmly onto $s erection.",
	"$n grabs you around the hips and pulls you firmly onto $s erection.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 10, 10, TRUE
    },

    {
	"xm-spoon",
	"On whom do you wish to do this?",
	NULL,
	"You roll $M onto $S side and penetrate $M hot flesh from behind.",
	"$n rolls $N onto $S side and penetrates $M hot flesh from behind.",
	"$n rolls you onto your side and penetrates your hot flesh from behind.",
	"Not on yourself!",
	NULL,
	1, 1, 250, 20, 20, TRUE
    },

    {
	"xm-suck",
	"On whom do you wish to do this?",
	NULL,
	"You suck slowly on $M nipple, feeling it harden between your lips.",
	"$n suck slowly on $N's nipple.",
	"$n sucks slowly on your nipple, and you feel it harden between $s lips.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

    {
	"xm-thrust",
	"On whom do you wish to do this?",
	NULL,
	"You thrust deeply between $M warm, slippery thighs.",
	"$n thrusts deeply between $N's warm, slippery thighs.",
	"$n thrusts deeply between your warm, slippery thighs.",
	"Not on yourself!",
	NULL,
	1, 2, 0, 15, 10, TRUE
    },
{
"xm-hump",
"On whom do you wish to do this?",
NULL,
"You push $M onto all fours and mount her, madly shoving your hard cock in and out of her tiny hole.",
"$n mounts $N like she was a bitch in heat, madly pumping in and out of her hole.",
"$n pushes you onto all fours like you were a bitch and heat and mounts you as he rapidly shoves his cock in and out of your slutty hole.",
"No!.",
NULL,
1, 2, 0, 35, 65, TRUE
},

{
"xm-mfuck",
"Fuck who?",
NULL,
"You have $N wrap her lips tight around your cock, and you thrust gently, pumping your cock in and out of her warm, wet mouth.",
"$N wraps her lips tight aroudn $n's cock, and he thrusts gently, pumping in and out from between her lips.",
"You wrap your lips tight around $n's cock and he thrusts gently, pumping in and out from between your ruby red lips.",
"No.",
NULL,
1, 1, 0, 12, 20, TRUE
},

{
"xm-feed",
"Feed who?",
NULL,
"You gently stick your fingers in $N's hot cunt and bring them to her lips, making her suck them clean.",
"$n sticks his fingers in $N's twat and then brings them to her lips, making her lick them clean.",
"$n sticks his fingers in your dripping wet pussy and then brings them to your lips, where you suck them clean.",
"No.",
NULL,
1, 1, 0, 5, 10, FALSE
},

{
"xm-gag",
"Who gets to taste it?",
NULL,
"You ram your cock deep into $M mouth, making her gag and try to swallow.",
"$n rams his cock deep into $N's throat, making her gag and try to swallow.",
"$n shoves his cock deep into your throat, making you gag and try to swallow.",
"No.",
NULL,
1, 1, 0, 5, 13, FALSE
},
    {
	"xm-tug",
	"On whom do you wish to do this?",
	NULL,
	"You gently tug $M nipple between your teeth.",
	"$n gently tugs $N's nipple between $s teeth.",
	"$n gently tugs your nipple between $s teeth.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 5, 10, FALSE
    },

{
"xm-nibble",
"On whom do you wish to do this?",
NULL,
"You push $M onto her back and bury your face in her cunt, nibbling gently on her clit.",
"$n pushes $N onto her back and buries his face between her legs, nibbling gently on her clit.",
"$n pushes you onto your back and buries his face in your womanhood, nibbling gently on your clit.",
"Nope.",
NULL,
1, 1, 0, 10, 45, FALSE
},
    {
	"xm-undress",
	"Who do you wish to undress?",
	NULL,
	"You gently tug at $N's garments with your teeth, until $E stands naked before you.",
	"$n gently tugs at $N's clothes with his teeth, until $E stands naked infront of $m.",
	"$n gently tugs at your clothing with his teeth, you stand naked before $m.",
	"Not on yourself!",
	NULL,
	1, 0, 0, 1, 1, FALSE
    },

    {
	"",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, FALSE
    }
};

/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *unveil;
    char arg[MAX_STRING_LENGTH];
    char argu[MAX_STRING_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char command[MAX_STRING_LENGTH];
    char logline[MAX_STRING_LENGTH];
    char lst_cmd[MSL];
    char                new_argument[MAX_STRING_LENGTH];
    char                newer_argument[MAX_STRING_LENGTH];
    int cmd;
    int trust;
    bool found, foundstar = FALSE;
    sh_int col = 0;
    int star = 0;
    int foundlog;
    int snard;

//int string_count = nAllocString ;
//int perm_count = nAllocPerm ;
char cmd_copy[MAX_INPUT_LENGTH] ;
//char buf2[MAX_STRING_LENGTH] ;



    sprintf( lst_cmd, "%s - %s", argument, IS_NPC(ch) ? ch->short_descr : ch->name );
    last_command = str_dup( lst_cmd );
    sprintf(argu,"%s %s",arg,one_argument( argument, arg));

    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

	strcpy(cmd_copy,argument);


    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_FREEZE) )
    {
	send_to_char( "You can't do anything while crying like a bitch!\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) && ch->desc && argument == ch->desc->incomm )
    {
               new_argument[0] = '\0';
               check_multi_cmd ( ch, argument, new_argument );
               argument = new_argument;
	       if (argument[0] == ch->pcdata->speedchar)
	       {
		 newer_argument[0] = '\0';
		 check_speedwalk(ch, argument);
		 return;
	       }
    }

    if ( !IS_NPC(ch) && xIS_SET(ch->extra, FFA_HUNG) )
    {
	if (ch->ffa_timer > 0)
        {
 	stc("#0You are #Rhung#0 about the neck by a #ystrong rope#0, gasping for breath!#n\n\r",ch);
	return;
	}
    }
    if ( !IS_NPC(ch) && xIS_SET(ch->extra, EXTRA_AFK) )
	do_afk(ch,"");

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }


    /*
    *    Do my weird star thingy
    */ 
    if (command[strlen(command) - 1] == '*')
    {
	command[strlen(command) - 1] = '\0';

        for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
        {
	    if ((!str_prefix(command, cmd_table[cmd].name) || strlen(command) == 0)
		&& can_interpret(ch, cmd))
	    {
		foundstar = TRUE;
		star++;
	        sprintf( buf, "%-15s", cmd_table[cmd].name );
	        send_to_char( buf, ch );
	        if ( ++col % 5 == 0 )
		    send_to_char( "\n\r", ch );
	    }
	}
	if (col % 5 != 0 && foundstar)
	    send_to_char("\n\r", ch);

	if (foundstar && star != 0)
  	{
	    sprintf(buf, "\n%d command%s found.\n\r", star,
		(star > 1) ? "s" : "");
	    stc(buf, ch);
	}

	if (!foundstar)
	{
	    stc("No commands found.\n\r", ch);
  	}
	return;
    }
 
  if (!IS_NPC(ch)) // Jobo's Aliases from Rand
  {
    ALIAS_DATA *ali;
    char tempcmd[MSL];
    for (ali = ch->pcdata->first_alias; ali; ali = ali->next)
    {
      if (!str_cmp(command, ali->short_n))
      {
        sprintf(buf, "%s", ali->long_n);
	if(ch->desc->incomm[0] != '\0')
	   sprintf(tempcmd, "%s%c%s", buf, ch->pcdata->stackchar, ch->desc->incomm);
	else
	   sprintf(tempcmd, "%s", buf);
	strcpy( ch->desc->incomm, tempcmd);
        interpret(ch, ch->desc->incomm);
        return;
      }
    }
  }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name )
	&&   cmd_table[cmd].level <= trust )
	{
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immtalk" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, ":"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "emote"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "tell"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "order"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "oreturn" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "roll" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "leap" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "lifespan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "nightsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "truesight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "horns" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "fangs" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "plasma" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "ashes" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "breakfree")) found = TRUE;
                else if (!str_cmp(cmd_table[cmd].name,"obj")&&
                  !IS_NPC(ch) && ch->pcdata->obj_vnum!=0)found=TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "humanform" ) &&
			!IS_NPC(ch) && ch->pcdata->obj_vnum != 0) found = TRUE;
		else {
		    send_to_char( "Not without a body!\n\r", ch ); return; }
	    }
            else if (IS_EXTRA(ch,EXTRA_EARTHMELD) )
	    {
                if      (!str_cmp( cmd_table[cmd].name, "earthmeld"  )) found = TRUE;
      else if (!str_cmp(cmd_table[cmd].name,"burrow")) found= TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits")) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "wake")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "vclan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "upkeep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immune" )) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "clandisc" ))
found = TRUE;
                else {
                    send_to_char( "Not while in the ground.\n\r", ch );
		    return; }
	    }
            else if (ch->embracing!=NULL
                  || ch->embraced!=NULL)
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"  )) found =
TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"    )) found =
TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat" )) found =
TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."    )) found =
TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found =
TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" ))
found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found =
TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found
= TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found =
TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "report" )) found
= TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "goto" )) found =
TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "kill" )) found
= TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "berserk" )) found
= TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "testemb" )) found
= TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "reboot" )) found
= TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save" )) found
= TRUE;
 else if (!str_cmp(cmd_table[cmd].name,"diablerize") && ch->embracing
!=NULL) found=TRUE;
else if(!str_cmp(cmd_table[cmd].name,"restore"))found=TRUE;
		else {
                    send_to_char( "Not while in an embrace.\n\r", ch );
		    return; }
	    }

            else if (IS_EXTRA(ch,TIED_UP) )
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "yell" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shout")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "tell" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "restore" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "order" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "introduce" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "darkness" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "sleep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "wake" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "fangs" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "claws" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "nightsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowplane" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "regenerate" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shield" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "vclan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "upkeep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immune" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "consent" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "report" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "goto" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "flex" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "change" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "drink" )) found = TRUE;
		else {
		    send_to_char( "Not while tied up.\n\r", ch );
		    if (ch->position > POS_STUNNED)
		    	act("$n strains against $s bonds.",ch,NULL,NULL,TO_ROOM, FALSE);
		    return; }
	    }
	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */

    foundlog = FALSE;
    for (snard = 0; snard <= num_of_logs; snard++)
    {
       if (!str_cmp( loglist[snard].cmd, cmd_table[cmd].name))
          foundlog = TRUE;
    }

    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXX" );
    if ( ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   (cmd_table[cmd].log == LOG_ALWAYS && cmd_table[cmd].level <= trust)
    ||   foundlog )
    {
	if (!IS_NPC(ch))
	{
	sprintf( log_buf, "Log %s: %s", ch->pcdata->switchname, logline );
	log_string( log_buf );
	}

    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, "% ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    if ( ch != NULL && ch->unveil != NULL )
    {
	unveil = ch->unveil;
         if ( unveil->in_room != NULL) {
	if ( unveil->in_room->vnum != ch->in_room->vnum )
	{
	  sprintf(buf, "You lose your mental link with %s.\n\r",ch->name);
	  stc(buf,unveil);
	}
	else
	{
	  stc("% ", unveil);
	  stc(logline, unveil);
	  stc("\n\r", unveil);
	}}
	else ch->unveil = NULL;
    }

       if (ch->desc != NULL)
       write_to_buffer(ch->desc,"\n\r",2);	
    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )
	{
	    if ( !check_xsocial( ch, command, argument ) )
		send_to_char( "Huh?\n\r", ch );
	}
/*
	else
	{
	    sprintf(kavirarg,">>>%s",argu);
	    room_text( ch, kavirarg );
	}
*/
	return;
    }
    else /* a normal valid command.. check if it is disabled */
      if (check_disabled (&cmd_table[cmd]))
      {
              send_to_char ("This command has been temporarily disabled.\n\r", ch );
             return;
      }
	 if ( IS_AFF2(ch, PLR_IMPLAG ) )
		 {
		 WAIT_STATE(ch, 20*PULSE_VIOLENCE);
		 }

    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{

	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	{
	if (IS_CLASS(ch, CLASS_VAMPIRE) 
          && (ch->pcdata->powers[VPOWER_PROTEAN] >= 8 )
	  && (cmd_table[cmd].name == "south"
	  || cmd_table[cmd].name == "east"
	  || cmd_table[cmd].name == "west"
	  || cmd_table[cmd].name == "down"
	  || cmd_table[cmd].name == "north"
          || cmd_table[cmd].name == "up"
          || cmd_table[cmd].name == "flex"))
		{
             (*cmd_table[cmd].do_fun) ( ch, argument );
              tail_chain( );
		WAIT_STATE(ch, 24);
                } 
	else  
	 send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;
	}
	case POS_STUNNED:
	{
        if (IS_CLASS(ch, CLASS_VAMPIRE)
          && (ch->pcdata->powers[VPOWER_PROTEAN] >= 8 )
          && (cmd_table[cmd].name == "south"
          || cmd_table[cmd].name == "east"
          || cmd_table[cmd].name == "west"
          || cmd_table[cmd].name == "down"
          || cmd_table[cmd].name == "north"
          || cmd_table[cmd].name == "up"
          || cmd_table[cmd].name == "flex")) 
	    {
	     (*cmd_table[cmd].do_fun) ( ch, argument );
    	      tail_chain( );
		WAIT_STATE(ch, 24); 
	    }
	   else 
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;
	}
	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    break;

	case POS_MEDITATING:
	case POS_SITTING:
	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r", ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */

    ch->last_cmd = cmd_table[cmd].do_fun;
    (*cmd_table[cmd].do_fun) ( ch, argument );
/*
if (string_count < nAllocString)
{
sprintf(buf2,
"Memcheck : Increase in strings :: %s : %s", ch->pcdata->switchname,
cmd_copy) ;
bug(buf2,0);
}

if (perm_count < nAllocPerm)
{
sprintf(buf2,
"Increase in perms :: %s : %s", ch->pcdata->switchname, cmd_copy) ;
bug(buf2,0);
}
*/

    tail_chain( );
/*
    if (!str_cmp(arg,"say"))
    {
    	sprintf(kavirarg,"%s",argu);
    	room_text( ch, strlower(kavirarg) );
    }
    else
    {
    	sprintf(kavirarg,">>>%s",argu);
    	room_text( ch, kavirarg );
    }
*/
    return;
}



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }


    if ( !found )
	return FALSE;

/*
    if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))
    {
	send_to_char( "You cannot socialise in this form!\n\r", ch );
	return TRUE;
    }
*/
    switch ( ch->position )
    {

    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;

    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM , FALSE   );
	act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR , FALSE   );
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM , FALSE   );
	act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR , FALSE   );
    }
    else
    {
	act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT, FALSE );
	act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR   , FALSE );
	act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT   , FALSE );

	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CHARM)
	&&   IS_AWAKE(victim) )
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
		act( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT, FALSE );
		act( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR   , FALSE );
		act( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT   , FALSE );
		break;

	    case 9: case 10: case 11: case 12:
		act( "$n slaps $N.",  victim, NULL, ch, TO_NOTVICT , FALSE);
		act( "You slap $N.",  victim, NULL, ch, TO_CHAR    , FALSE);
		act( "$n slaps you.", victim, NULL, ch, TO_VICT    , FALSE);
		break;
	    }
	}
    }
    return TRUE;
}



bool check_xsocial( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *partner = NULL;
    int cmd;
    int stage;
    int amount;
    bool is_ok = FALSE;
    bool found = FALSE;
    bool one = FALSE;
    bool two = FALSE;

    if (IS_NPC(ch)) return FALSE;

    for ( cmd = 0; xsocial_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == xsocial_table[cmd].name[0]
	&&   !str_prefix( command, xsocial_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    switch ( ch->position )
    {

    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return TRUE;
    }
    if (IS_NPC(victim))
    {
	send_to_char("You can only perform xsocials on players.\n\r",ch);
	return TRUE;
    }

	if( !str_cmp(ch->pcdata->switchname, "") )
	{
		victim->pcdata->partner = ch;
	}
  else if (xIS_SET(victim->extra, TIED_UP))
         { victim->pcdata->partner = ch;
         }

    
	if (IS_EXTRA(ch, TIED_UP))
    {
	send_to_char("You wiggle and strain but the ropes only tighten.\n\r",ch);
        act("$n strains helplessly against $m bonds.",ch,NULL,NULL,TO_ROOM, FALSE);
	return FALSE;
    }
    else if ( arg[0] == '\0' )
    {
	act( xsocial_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM , FALSE   );
	act( xsocial_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR , FALSE   );
    }
    else if ( victim == ch )
    {
	act( xsocial_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM , FALSE);
	act( xsocial_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR , FALSE);
    }
/*    else if (ch->sex != SEX_MALE && ch->sex != SEX_FEMALE)
    {
	send_to_char("You must be either male or female to use these socials.\n\r",ch);
    }
    else if (victim->sex != SEX_MALE && victim->sex != SEX_FEMALE)
    {
	send_to_char("They must be either male or female for these socials.\n\r",ch);
    }
    else if (ch->sex == victim->sex)
    {
	send_to_char("Please stick to people of the opposite gender.\n\r",ch);
    }
*/    else
    {
	if (xsocial_table[cmd].gender == SEX_MALE && ch->sex != SEX_MALE)
	{
	    send_to_char("Only men can perform this type of social.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == SEX_FEMALE && ch->sex != SEX_FEMALE)
	{
	    send_to_char("Only women can perform this type of social.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == SEX_MALE && victim->sex != SEX_FEMALE)
	{
	    send_to_char("You can only perform this social on a woman.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == SEX_FEMALE && victim->sex != SEX_MALE)
	{
	    send_to_char("You can only perform this social on a man.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == 3 && ch->sex != SEX_FEMALE)
	{
		stc( "Only females may preform this command.\n\r",ch);
	}
	else if (xsocial_table[cmd].gender == 3 && victim->sex != SEX_FEMALE)
	{
		stc( "You can only preform this command on a female.\n\r",ch);
	}
	else if (((partner = victim->pcdata->partner) == NULL || partner != ch) && (!IS_SET (victim->in_room->room_flags, ROOM_SEX)))
  	{
         	stc("You cannot perform an xsocial on someone without their consent.\n\r",ch);
	 }
	else if (xsocial_table[cmd].stage == 0 && ch->pcdata->stage[0] < 1
	    && ch->pcdata->stage[2] > 0 && ch->sex == 5)
	    send_to_char("You have not yet recovered from last time!\n\r",ch);
	else if (xsocial_table[cmd].stage == 0 && victim->pcdata->stage[0] < 1
	    && victim->pcdata->stage[2] > 0 && victim->sex == 5)
	    send_to_char("They have not yet recovered from last time!\n\r",ch);
	else if (xsocial_table[cmd].stage > 0 && ch->pcdata->stage[0] < 100)
	    send_to_char("You are not sufficiently aroused.\n\r",ch);
	else if (xsocial_table[cmd].stage > 0 && victim->pcdata->stage[0] < 100)
	    send_to_char("They are not sufficiently aroused.\n\r",ch);
	else if (xsocial_table[cmd].stage > 1 && ch->pcdata->stage[1] < 1)
	    send_to_char("You are not in the right position.\n\r",ch);
	else if (xsocial_table[cmd].stage > 1 && victim->pcdata->stage[1] < 1)
	    send_to_char("They are not in the right position.\n\r",ch);
	else
	{
	    act(xsocial_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT, FALSE);
	    act(xsocial_table[cmd].char_found,    ch, NULL, victim, TO_CHAR   , FALSE);
	    act(xsocial_table[cmd].vict_found,    ch, NULL, victim, TO_VICT   , FALSE);
	    /*if (xsocial_table[cmd].chance)
	    {
		if (ch->sex == SEX_FEMALE && 
		    !IS_EXTRA(ch, EXTRA_PREGNANT) && number_range(1,3) == 1) 
		make_preg(ch,victim);
		else if (victim->sex == SEX_FEMALE && 
		    !IS_EXTRA(victim, EXTRA_PREGNANT) && 
		    number_range(1,3) == 1) 
		make_preg(victim,ch);
	    }*/
	    if (!str_prefix(xsocial_table[cmd].name,"x-tie"))
	    {
//		xSET_BIT(victim->extra, TIED_UP);
	    }
	    if (!str_prefix(xsocial_table[cmd].name,"x-gag"))
	    {
//		xSET_BIT(victim->extra, GAGGED);
	    }
	    if (!str_prefix(xsocial_table[cmd].name,"x-blindfold"))
	    {
//		xSET_BIT(victim->extra, BLINDFOLDED);
	    }
	    if (xsocial_table[cmd].stage == 1)
	    {
		ch->pcdata->stage[1] = xsocial_table[cmd].position;
		victim->pcdata->stage[1] = xsocial_table[cmd].position;
		if (!xIS_SET(ch->extra, EXTRA_DONE))
		{
		    xSET_BIT(ch->extra, EXTRA_DONE);
		    if (ch->sex == SEX_FEMALE)
		    {
			act("You feel $n bleed as you enter $m.",ch,NULL,victim,TO_VICT, FALSE);
			act("You feel yourself bleed as $N enters you.",ch,NULL,victim,TO_CHAR, FALSE);
			ch->in_room->blood += 1;
		    }
		}
		if (!xIS_SET(victim->extra, EXTRA_DONE))
		{
		    xSET_BIT(victim->extra, EXTRA_DONE);
		    if (victim->sex == SEX_FEMALE)
		    {
			act("You feel $N bleed as you enter $M.",ch,NULL,victim,TO_CHAR, FALSE);
			act("You feel yourself bleed as $n enters you.",ch,NULL,victim,TO_VICT, FALSE);
			ch->in_room->blood += 1;
		    }
		}
		stage = 2;
	    }
	    else stage = xsocial_table[cmd].stage;
	    if (stage == 2) amount = ch->pcdata->stage[1];
		else amount = 100;
	    if (xsocial_table[cmd].self > 0)
	    {
		is_ok = FALSE;
		if (ch->pcdata->stage[stage] >= amount) is_ok = TRUE;
		ch->pcdata->stage[stage] += xsocial_table[cmd].self;
		if (!is_ok && ch->pcdata->stage[stage] >= amount) 
		{
		    stage_update(ch,victim,stage,xsocial_table[cmd].name);
		    one = TRUE;
		}
	    }
	    if (xsocial_table[cmd].other > 0)
	    {
		is_ok = FALSE;
		if (victim->pcdata->stage[stage] >= amount) is_ok = TRUE;
		victim->pcdata->stage[stage] += xsocial_table[cmd].other;
		if (!is_ok && victim->pcdata->stage[stage] >= amount) 
		{
		    stage_update(victim,ch,stage,xsocial_table[cmd].name);
		    two = TRUE;
		}
	    }
	    if ( one && two )
	    {
		    ch->pcdata->stage[0] = 0;
		    victim->pcdata->stage[0] = 0;
		if (!IS_EXTRA(ch, EXTRA_EXP))
		{
		    send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",ch);
		    xSET_BIT(ch->extra, EXTRA_EXP);
		    ch->exp += 100000;
		}
		if (!IS_EXTRA(victim, EXTRA_EXP))
		{
		    send_to_char("Congratulations on achieving a simultanious orgasm!  Recieve 100000 exp!\n\r",victim);
		    xSET_BIT(victim->extra, EXTRA_EXP);
		    victim->exp += 100000;
		}
	    }
	}
    }
    return TRUE;
}

void stage_update( CHAR_DATA *ch, CHAR_DATA *victim, int stage,char *argument )
{
    if (IS_NPC(ch) || IS_NPC(victim)) return;
    if (stage == 0)
    {
	if (ch->sex == SEX_MALE)
	{
	    send_to_char("You get a boner.\n\r",ch);
	    act("You feel $n get a boner.",ch,NULL,victim,TO_VICT, FALSE);
	    return;
	}
	else if (ch->sex == SEX_FEMALE)
	{
	    send_to_char("You get wet.\n\r",ch);
	    act("You feel $n get wet.",ch,NULL,victim,TO_VICT, FALSE);
	    return;
	}
    }
    else if (stage == 2)
    {
	if (ch->sex == SEX_MALE)
	{
	    if( str_cmp(argument,"xm-cum")   && str_cmp(argument,"xm-facial") && str_cmp(argument,"xm-canal") &&
		str_cmp(argument,"xm-canal") && str_cmp(argument,"xm-cbreasts") && str_cmp(argument,"xm-chair") &&
		str_cmp(argument,"xm-chand") && str_cmp(argument,"xm-cstomach") && str_cmp(argument,"xf-chands") &&
		str_cmp(argument,"xf-cbreasts") )
	    {
		act("You grit your teeth as you shoot your creamy load inside of $M.",ch,NULL,victim,TO_CHAR, FALSE);
		act("$n grits his teeth as he shoots his load inside of you.",ch,NULL,victim,TO_VICT, FALSE);
		act("$n grits his teeth as he shoots a load of cum inside of $N.",ch,NULL,victim,TO_NOTVICT, FALSE);
	    }
	    ch->pcdata->genes[8] += 1;
	    victim->pcdata->genes[8] += 1;
	    save_char_obj(ch);
	    save_char_obj(victim);
	    if (victim->pcdata->stage[2] < 1 || victim->pcdata->stage[2] >= 250)
	    {
		ch->pcdata->stage[2] = 0;
		if (ch->pcdata->stage[0] >= 200) ch->pcdata->stage[0] -= 100;
	    }
	    else ch->pcdata->stage[2] = 200;
	    /*if (victim->sex == SEX_FEMALE && 
		!IS_EXTRA(victim, EXTRA_PREGNANT) && number_percent() <= 8) 
	    make_preg(victim,ch);*/
	    return;
	}
	else if (ch->sex == SEX_FEMALE)
	{
	    if( str_cmp(argument,"xf-cum") && str_cmp(argument,"xf-cface") )
	    {
		act("You whimper as you cum.",ch,NULL,victim,TO_CHAR, FALSE);
		act("$n whimpers as $e cums.",ch,NULL,victim,TO_ROOM, FALSE);
	    }
	    if (victim->pcdata->stage[2] < 1 || victim->pcdata->stage[2] >= 250)
	    {
		ch->pcdata->stage[2] = 0;
		if (ch->pcdata->stage[0] >= 200) ch->pcdata->stage[0] -= 100;
	    }
	    else ch->pcdata->stage[2] = 200;
	    return;
	}
    }
    return;
}


/*
 * Return true if an argument is completely numeric.
 */
bool is_number( char *arg )
{
    if ( *arg == '\0' )
	return FALSE;

    for ( ; *arg != '\0'; arg++ )
    {
	if ( !isdigit(*arg) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;
    
    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

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
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

/* Syntax is:
disable - shows disabled commands
disable <command> - toggles disable status of command
*/

void do_disable (CHAR_DATA *ch, char *argument)
{
	int i;
	DISABLED_DATA *p,*q;
	char buf[100];
	
	if (IS_NPC(ch))
	{
		send_to_char ("RETURN first.\n\r",ch);
		return;
	}
	
	if (!argument[0]) /* Nothing specified. Show disabled commands. */
	{
		if (!disabled_first) /* Any disabled at all ? */
		{
			send_to_char ("There are no commands disabled.\n\r",ch);
			return;
		}

		send_to_char ("Disabled commands:\n\r"
		              "Command      Level   Disabled by\n\r",ch);
		                
		for (p = disabled_first; p; p = p->next)
		{
			sprintf (buf, "%-12s %5d   %-12s\n\r",p->command->name, p->level, p->disabled_by);
			send_to_char (buf,ch);
		}
		return;
	}
	
	/* command given */

	/* First check if it is one of the disabled commands */
	for (p = disabled_first; p ; p = p->next)
		if (!str_cmp(argument, p->command->name))
			break;
			
	if (p) /* this command is disabled */
	{
	/* Optional: The level of the imm to enable the command must equal or exceed level
	   of the one that disabled it */
	
		if (get_trust(ch) < p->level)
		{
			send_to_char ("This command was disabled by a higher power.\n\r",ch);
			return;
		}
		
		/* Remove */
		
		if (disabled_first == p) /* node to be removed == head ? */
			disabled_first = p->next;
		else /* Find the node before this one */
		{
			for (q = disabled_first; q->next != p; q = q->next); /* empty for */
			q->next = p->next;
		}
		
		free_string (p->disabled_by); /* free name of disabler */
		free_mem (p,sizeof(DISABLED_DATA)); /* free node */
		
		save_disabled(); /* save to disk */
		send_to_char ("Command enabled.\n\r",ch);
	}
	else /* not a disabled command, check if that command exists */
	{
		/* IQ test */
		if (!str_cmp(argument,"disable"))
		{
			send_to_char ("You cannot disable the disable command.\n\r",ch);
			return;
		}

		/* Search for the command */
		for (i = 0; cmd_table[i].name[0] != '\0'; i++)
			if (!str_cmp(cmd_table[i].name, argument))
				break;

		/* Found? */				
		if (cmd_table[i].name[0] == '\0')
		{
			send_to_char ("No such command.\n\r",ch);
			return;
		}

		/* Can the imm use this command at all ? */				
		if (cmd_table[i].level > get_trust(ch))
		{
			send_to_char ("You dot have access to that command; you cannot disable it.\n\r",ch);
			return;
		}
		
		/* Disable the command */
		
		CREATE(p, DISABLED_DATA, 1);

		p->command = &cmd_table[i];
		p->disabled_by = str_dup (ch->pcdata->switchname); /* save name of disabler */
		p->level = get_trust(ch); /* save trust */
		p->next = disabled_first;
		disabled_first = p; /* add before the current first element */
		
		send_to_char ("Command disabled.\n\r",ch);
		save_disabled(); /* save to disk */
	}
}

/* Check if that command is disabled 
   Note that we check for equivalence of the do_fun pointers; this means
   that disabling 'chat' will also disable the '.' command
*/   
bool check_disabled (const struct cmd_type *command)
{
	DISABLED_DATA *p;
	
	for (p = disabled_first; p ; p = p->next)
		if (p->command->do_fun == command->do_fun)
			return TRUE;

	return FALSE;
}

/* Load disabled commands */
void load_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;
	char *name;
	int i;
	
	disabled_first = NULL;
	
	fp = fopen (DISABLED_FILE, "r");
	
	if (!fp) /* No disabled file.. no disabled commands : */
		return;
		
	name = fread_word (fp);
	
	while (str_cmp(name, END_MARKER)) /* as long as name is NOT END_MARKER :) */
	{
		/* Find the command in the table */
		for (i = 0; cmd_table[i].name[0] ; i++)
			if (!str_cmp(cmd_table[i].name, name))
				break;
				
		if (!cmd_table[i].name[0]) /* command does not exist? */
		{
			bug ("Skipping uknown command in " DISABLED_FILE " file.",0);
			fread_number(fp); /* level */
			fread_word(fp); /* disabled_by */
		}
		else /* add new disabled command */
		{
			CREATE(p, DISABLED_DATA, 1);
			p->command = &cmd_table[i];
			p->level = fread_number(fp);
			p->disabled_by = str_dup(fread_word(fp)); 
			p->next = disabled_first;
			
			disabled_first = p;

		}
		
		name = fread_word(fp);
	}

	fclose (fp);		
}

/* Save disabled commands */
void save_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;
	
	if (!disabled_first) /* delete file if no commands are disabled */
	{
		unlink (DISABLED_FILE);
		return;
	}
	
	fp = fopen (DISABLED_FILE, "w");
	
	if (!fp)
	{
		bug ("Could not open " DISABLED_FILE " for writing",0);
		return;
	}
	
	for (p = disabled_first; p ; p = p->next)
		fprintf (fp, "%s %d %s\n", p->command->name, p->level, p->disabled_by);
		
	fprintf (fp, "%s\n",END_MARKER);
		
	fclose (fp);
}

void load_bans() 
        {
        FILE *fp;       
        BAN_DATA *p;
        char *name;

        first_ban = NULL;
        last_ban = NULL;
        fp = fopen (BAN_LIST, "r");
        
        if (!fp) return;
        
        name = fread_word(fp);
  
        while (str_cmp(name, END_MARKER)) {
                CREATE(p, BAN_DATA, 1);
                p->name = str_dup(name);
		LINK(p, first_ban, last_ban, next, prev );
                name = fread_word(fp);}
                
        fclose(fp);
}        


void save_bans()
{
        FILE *fp;
        BAN_DATA *p;  

        if (!first_ban)       {
                unlink (BAN_LIST);
                return;
        }

        fp = fopen (BAN_LIST, "w");

        if (!fp) {bug("could not open ban.txt",0);
                return;}
       for (p = first_ban; p; p = p->next)
        fprintf(fp, "%s\n", p->name);

        
        fprintf (fp, "%s\n",END_MARKER);
        
        fclose (fp);
}




