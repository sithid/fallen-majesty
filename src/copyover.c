/***************************************************************************
 *  Dawn of Hope                                                           *
 ---------------------------------------------------------------------------
 *  Copyover ported to GodWars and Merc, with a copyover timer, done by    *
 *  Ryan Waliany.                                                          *
 *  Dawn of Hope (c)1999-2001 Xkilla 'Ryan Waliany' (ryan@scv.net)         *
 ---------------------------------------------------------------------------
 * You may not remove the following section, for this has all the credits  *
 * of the creators of this snippet.                                        *
 *  Copyover created by: Erwin S. Andreasen    Snippet by: Ryan Waliany    *
 *  Ported to GodWars/Merc by Ryan Waliany, and also the creator of the    *
 *  timer for copyover.                                                    *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "merc.h"


bool     write_to_descriptor     args( ( int desc, char *txt, int length ) );
#define DD DESCRIPTOR_DATA
DD      *copyover_descriptor    args( (void) );

extern int num_descriptors;

DESCRIPTOR_DATA *copyover_descriptor(void)
{
    DESCRIPTOR_DATA *d;

    CREATE( d, DESCRIPTOR_DATA, 1 );

    d->connected        = CON_GET_NAME;
    d->outsize          = 2000;
    CREATE(d->outbuf, char, d->outsize);
 
    return d;
}

#define CH(descriptor)  ((descriptor)->original ? \
(descriptor)->original : (descriptor)->character)
 
/* This file holds the copyover data */
#define COPYOVER_FILE "copyover.data"

/* This is the executable file */
#define EXE_FILE          "../src/dystopia"
// CHANGE this later - Snard
 
/*  Copyover - Original idea: Fusion of MUD++
 *  Adapted to Diku by Erwin S. Andreasen, <erwin@pip.dknet.dk>
 *  Adapted to GodWars by Xkilla, Ryan Waliany <ryan@scv.net>
 */
void do_copyover (CHAR_DATA *ch, char * argument)
{
        FILE *fp;
        DESCRIPTOR_DATA *d, *d_next;
        char buf [100], buf2[100];
        extern int port,control; /* db.c */
 
        save_sysdata( sysdata );
        fp = fopen (COPYOVER_FILE, "w");
        if (!fp)
        {
                send_to_char ("Copyover file not writeable, aborted.\n\r",ch);
                flog ("Could not write to copyover file: %s", COPYOVER_FILE);
                perror ("do_copyover:fopen");
                return;
        }
//People with olc enable the next line
        do_asave (NULL, "changed");
	do_forceall("call all");
	do_forceall("save");

        sprintf (buf, "\n\r Rebooting -%s\n\r",copyover_info.name);
        do_info(ch, buf);

        /* For each playing descriptor, save its state */
        for (d = first_descriptor; d ; d = d_next)
        {
                CHAR_DATA * och = CH (d);
                d_next = d->next; /* We delete from the list , so need to save this */
 
                if (!d->character || (d->connected != CON_PLAYING && d->connected != 
CON_EDITING)) /* drop those logging on */
                {
                        write_to_descriptor (d->descriptor, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r",
0);
                        close_socket (d); /* throw'em out */
                }
                else
                {
                        fprintf (fp, "%d %s %d %s\n", d->descriptor, och->pcdata->switchname, och->fight_timer, d->host);
 
                        if (och->level == 1)
                        {
                                write_to_descriptor (d->descriptor, "Since you are level one, and level one characters do not save, you gain a free level!\n\r", 0);
                                och->level = 2;
                        do_forceauto(och,"save");    
                        }
//Basic char saving......
                        do_call(och,"all");
                        do_forceauto(och,"save");
			if (och->fight_timer == 0)
   			   do_restore(och,"self");
 			if( IS_SET(och->in_room->room_flags,ROOM_ARENA) )
			{
			char_from_room(och);
			char_to_room(och,get_room_index(ROOM_VNUM_ALTAR));
   			och->fight_timer = 0;
                        do_restore(och,"self");
			}
                        save_char_obj (och);
 
                        write_to_descriptor (d->descriptor, buf, 0);
                }
        }
 
        fprintf (fp, "-1\n");
        fclose (fp);
 
        /* Close reserve and other always-open files and release other resources */
 
        fclose (fpReserve);
 
        /* exec - descriptors are inherited */
 
        sprintf (buf, "%d", port);
        sprintf (buf2, "%d", control);
        execl (EXE_FILE, "Fallen Majesty on port", buf, "Control:", buf2, (char *) NULL);
 
        /* Failed - sucessful exec will not return */
 
        perror ("do_copyover: execl");
        send_to_char ("Copyover FAILED!\n\r",ch);
 
        /* Here you might want to reopen fpReserve */
        fpReserve = fopen (NULL_FILE, "r");
}
 
/* Recover from a copyover - load players */
void copyover_recover ()
{
        DESCRIPTOR_DATA *d;
        FILE *fp;
        char name [100];
        char host[MAX_STRING_LENGTH];
        int desc;
        int ftimer;
        bool fOld;

        flog ("Copyover recovery initiated");
 
        fp = fopen (COPYOVER_FILE, "r");
        if (!fp) /* there are some descriptors open which will hang forever then ? */
        {
                perror ("copyover_recover:fopen");
                flog ("Copyover file not found. Exitting.\n\r");
                exit (1);
        }
 
        unlink (COPYOVER_FILE); /* In case something crashes - doesn't prevent leading  */
 
        for (;;)
        {
                fscanf (fp, "%d %s %d %s\n", &desc, name, &ftimer, host);
                if (desc == -1)
                        break;
 
                /* Write something, and check if it goes error-free */
                if (!write_to_descriptor (desc, "\n\rLoading.....\n\r",0))
                {
                        close (desc); /* nope */
                        continue;
                }
 
                d = copyover_descriptor();
                d->descriptor = desc;
 
                d->host = str_dup (host);
		LINK( d, first_descriptor, last_descriptor, next, prev );
                d->connected = CON_COPYOVER_RECOVER; /* -15, so close_socket frees the char */
 
 
                /* Now, find the pfile */
 
                fOld = load_char_obj (d, name);
 
                if (!fOld) /* Player file not found?! */
                {
                        write_to_descriptor (desc, "\n\rSomehow, your character was lost in the copyover. Sorry.\n\r", 0);
                        close_socket (d);
                }
                else /* ok! */
                {
                        write_to_descriptor (desc, "\n\rDone.\n\r",0);
 
                        /* Just In Case */
                        if (!d->character->in_room)
                                d->character->in_room = get_room_index (ROOM_VNUM_TEMPLE);
 
                        /* Insert in the char_list */
                        d->character->next = char_list;
                        char_list = d->character;
    if ( ++num_descriptors > sysdata.maxplayers )
        sysdata.maxplayers = num_descriptors;
 /*   if ( sysdata.maxplayers > sysdata.alltimemax )
    {
        if ( sysdata.time_of_max )
          DISPOSE(sysdata.time_of_max);
        sprintf(buf, "%24.24s", ctime(&current_time));
        sysdata.time_of_max = str_dup(buf);
        sysdata.alltimemax = sysdata.maxplayers;
        log_string( "Broke all-time maximum player record: %d", sysdata.alltimemax );
        sprintf(buf,  "Broke all-time maximum player record: %d", sysdata.alltimemax);
        do_info(NULL, buf);
        save_sysdata( sysdata );
    }
*/
    if( IS_CLASS(d->character, CLASS_WEREWOLF))
    {
        d->character->gnosis[GCURRENT] = d->character->gnosis[GMAXIMUM] - 5;
        if( d->character->gnosis[GCURRENT] < 0 ) d->character->gnosis[GCURRENT] = 0;
    }
    if (xIS_SET(d->character->act,PLR_CHALLENGER)) xREMOVE_BIT(d->character->act,PLR_CHALLENGER);
    if (xIS_SET(d->character->act,PLR_CHALLENGED)) xREMOVE_BIT(d->character->act,PLR_CHALLENGED);
                        char_to_room (d->character, d->character->in_room);
                        do_look (d->character, "auto");
                        act ("$n materializes!", d->character, NULL, NULL, TO_ROOM, FALSE);
			d->character->fight_timer = ftimer;
                        d->connected = CON_PLAYING;


                }
		 
        }
  fclose (fp);
 
 
}

void do_hotboot( CHAR_DATA *ch, char *argument )
{
        char  buf[MAX_STRING_LENGTH];
 
if (!ch->desc || *argument == '\0')
{
  send_to_char("Syntax: copyover  on/off/now!\n\r",ch);
  return;
}
 
   if ( strcmp(argument,"on")== 0 )
      {
        copyover_info.time = 3;
        copyover_info.name = str_dup(ch->name);
        sprintf(buf, "Copyover Started by %s. 3 Ticks left.\n\r", ch->name);
        do_info(ch, buf);
        return;
      }
 
   else if ( strcmp(argument,"off")== 0 )
      {
        sprintf(buf,"Reboot Cancelled.\n\r");
        do_info(ch, buf);
        copyover_info.time = 0;
        copyover_info.name = "No one";
        return;
      }
   else if ( strcmp(argument,"now")== 0 )
      {
        save_sysdata( sysdata );
        copyover_info.time = 0;
        if ( xIS_SET(ch->act, PLR_WIZINVIS) ) copyover_info.name = "Someone";
        if (!xIS_SET(ch->act, PLR_WIZINVIS))  copyover_info.name = str_dup(ch->name);
        sprintf(buf, "No warning reboot by -%s.\n\r", ch->name);
        do_info(ch, buf);
        do_copyover(NULL,"");
      }
   else
      {
        send_to_char("The Commands are On and Off",ch);
        return;
      }
}

void flog (char * fmt, ...)
{
	char buf [2*MAX_STRING_LENGTH];
	va_list args;
	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);
	
	log_string (buf);
}
          
