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

void do_paradox( CHAR_DATA *ch, char *argument)  {
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
        DESCRIPTOR_DATA *d;        
        argument = one_argument(argument, arg);        

	if (arg[0] == '\0') {
		send_to_char("Paradox whom?\n\r", ch );
		return;}

    if ( get_trust(ch) >=  MAX_LEVEL - 2 && !str_cmp(arg,"all"))
    {
        for (d = first_descriptor; d != NULL; d = d->next)
        {
	    victim = d->character;

	    if (victim == NULL || IS_NPC(victim))
		continue;
	    if (IS_IMMORTAL(ch)) continue;                
          
            { paradox(victim); }
        }
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
	paradox(victim);
	send_to_char("Done!\n\r", ch );
}


void paradox( CHAR_DATA *ch )
{
    char            buf[MAX_STRING_LENGTH]; 

    send_to_char("The sins of your past strike back!\n\r",  ch);
    send_to_char("The paradox has come for your soul, spamkiller!!\n\r",  ch);
    sprintf( buf, "#P%s#0 has been struck down by a #Phumiliating paradox#n.", ch->name);
    do_update(NULL, buf);
    ch->paradox[1] = 0;
    ch->paradox[2] = 0;


    ch->hit     =  -10;
    ch->mana    =  -10;
    ch->move    =  -10;
    update_pos( ch );
    do_escape(ch, "@PARADOX@");    
    xSET_BIT(ch->extra, GAGGED);
    xSET_BIT(ch->extra, BLINDFOLDED);
    if (!xIS_SET(ch->extra,TIED_UP))
     xSET_BIT(ch->extra, TIED_UP);
    WAIT_STATE(ch, 100);    
    return;
}

