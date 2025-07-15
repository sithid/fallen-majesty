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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"

DECLARE_DO_FUN(do_look     );
int	arena;
int     arena_timer;

/*=======================================================================*
 * function: do_challenge                                                *
 * purpose: sends initial arena match query                              *
 * written by: Doug Araya (whiplash@tft.nacs.net) 6-10-96                *
 *=======================================================================*/
void do_challenge(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim; 
  char buf[MAX_STRING_LENGTH]; 
  DESCRIPTOR_DATA *d;


/* == First make all invalid checks == */
 if(IS_NPC(ch))
  return; 

 if( arena == FIGHT_START )
  {
  if (IS_SET(ch->act, PLR_CHALLENGER))
     stc("You're already challenging someone.\n\r", ch);
  else
     send_to_char("Sorry, some one else has already started a challenge, please try later.\n\r",ch);
  return;
  }

 if( arena == FIGHT_BUSY )
  {
  send_to_char("Sorry, there is a fight in progress, please wait a few moments.\n\r",ch);
  return; 
  }

 if( arena == FIGHT_LOCK )
  {
  send_to_char("Sorry, the arena is currently locked from use.\n\r",ch);
  return;
  }

 if(IS_SET(ch->act,PLR_CHALLENGED))
  {
  send_to_char("You have already been challenged, either AGREE or DECLINE first.\n\r",ch);
  return; 
  }

 if(ch->hit < ch->max_hit)
  {
  send_to_char("You must be fully healed to fight in the arena.\n\r",ch);
  return;
  }


 if(argument[0] == '\0')
  {
  send_to_char("You must specify whom you wish to challenge.\n\r",ch);
  return;
  }

 if((victim = get_char_world(ch,argument)) == NULL)
  {
  send_to_char("They are not playing.\n\r",ch);
  return;
  }

 if (!IS_IMMORTAL(ch))
 {
 if (is_safe(victim,ch))
 {
  send_to_char("You cannot kill them.\n\r",ch);
  return;
 }
 if (is_safe(ch,victim))
 {
  send_to_char("You cannot kill them.\n\r",ch);
  return;
 }
 }
 if (ch->race < 1)
 {
  send_to_char("You must have 1 status to bet.\n\r",ch);
  return;
 }
 if(IS_NPC(victim) || victim == ch )
  {
  send_to_char("You cannot challenge NPC's or yourself.\n\r",ch);
  return;
   }


 if(IS_SET(victim->act,PLR_CHALLENGER))
  {
  send_to_char("They have already challenged someone else.\n\r",ch);
  return;
  }

 if(victim->fighting != NULL )
  {
  send_to_char("That person is engaged in battle right now.\n\r",ch); 
  return; 
  }

 if(victim->hit < victim->max_hit)
  {
  send_to_char("That player is not healthy enough to fight right now.\n\r",ch);
  return;
  }
 if ( victim->desc == NULL )
  {
  send_to_char("That player is linkdead at the moment, try them later.\n\r",ch);
  return;
  }


/* == Now for the challenge == */
 if (arena == FIGHT_OPEN)
 {
 arena_timer = 3;
 ch->challenged = victim;
 SET_BIT(ch->act,PLR_CHALLENGER);
 victim->challenger = ch;
 SET_BIT(victim->act,PLR_CHALLENGED);
 arena = FIGHT_START;
 send_to_char("Challenge has been sent\n\r",ch);
 act("$n has challenged you to a death match.",ch,NULL,victim,TO_VICT);
 sprintf(buf,"#0[#7Arena#0]#n#L %s #7has challenged#R %s #7to a deathmatch.#n\n\r",
         ch->name, victim->name);
 do_info(ch, buf);
 sprintf(buf,"type: AGREE %s to meet the challenge.\n\r",ch->name);
 send_to_char(buf,victim);
 sprintf(buf,"type: DECLINE %s to chicken out.\n\r",ch->name); 
 send_to_char(buf,victim);
 return;
 }
}

/*=======================================================================*
 * function: do_accept                                                   *
 * purpose: to accept the arena match, and move the players to the arena *
 * written by: Doug Araya (whiplash@tft.nacs.net) 6-10-96                *
 *=======================================================================*/
void do_agree(CHAR_DATA *ch, char *argument)
{

 float odd1, odd2;
 float lvl1, lvl2;
 DESCRIPTOR_DATA *d;
 CHAR_DATA *victim; 
 char buf1[MAX_STRING_LENGTH]; 
 char buf2[MAX_STRING_LENGTH];
 char buf3[MAX_STRING_LENGTH];
 int char_room; 
 int vict_room; 

 /*== the room VNUM's for our arena.are ==*/
 /* we use 1051 thru 1066 for a 4x4 arena */
 char_room = 500005;
 vict_room = 500000;

/* == first make all invalid checks == */
 if(IS_NPC(ch))
 return;

 if( !IS_SET(ch->act,PLR_CHALLENGED) )
  {
  send_to_char("You have not been challenged.\n\r",ch);
  return;
  }

 if( arena == FIGHT_BUSY )
  {
  send_to_char("Sorry, there is a fight in progress, please wait a few moments.\n\r",ch);
  return;
  }

 if( arena == FIGHT_LOCK )
  {
  send_to_char("Sorry, the arena is currently locked from use.\n\r",ch);
  return;
  }

 if(argument[0]=='\0')
  {
  send_to_char("You must specify whose challenge you wish to accept.\n\r",ch);
  return;
  }

 if((victim = get_char_world(ch,argument)) == NULL)
  {
  send_to_char("They aren't logged in!\n\r",ch);
  return;
  }

 if(victim == ch)
  {
  send_to_char("You haven't challenged yourself!\n\r",ch);
  return;
  }
 if (is_safe(victim,ch))
 {
  send_to_char("You cannot kill them.\n\r",ch);
  return;
 }
 if (is_safe(ch,victim))
 {
  send_to_char("You cannot kill them.\n\r",ch);
  return;
 }
 if (ch->race < 1)
 {
  send_to_char("You must have 1 status to bet.\n\r",ch);
  return;
 }

 if(!IS_SET(victim->act,PLR_CHALLENGER) || victim != ch->challenger )
  {
  send_to_char("That player hasn't challenged you!\n\r",ch); 
  return; 
  }

/* == now get to business == */
if (arena == FIGHT_START)
{
send_to_char("You have accepted the challenge!\n\r",ch);
act("$n accepts your challenge!",ch,NULL,victim,TO_VICT);
/* == announce the upcoming event == */

sprintf(buf1,"#0[#7Arena#0]#n #L%s #7(%d wins) (%d losses)#n\n\r",victim->name,victim->pcdata->awins,victim->pcdata->alosses);
sprintf(buf2,"#0[#7Arena#0]#n #R%s #7(%d wins) (%d losses)#n\n\r",ch->name,ch->pcdata->awins,ch->pcdata->alosses); 
        do_info(ch, buf1 );
        do_info(ch,buf2 );
/* == now move them both to an arena for the fun == */
send_to_char("You make your way into the arena.\n\r",ch);
char_from_room(ch); 
char_to_room(ch,get_room_index(char_room));
do_look(ch, "auto");
send_to_char("You make your way to the arena.\n\r",victim); 
char_from_room(victim); 
char_to_room(victim,get_room_index(vict_room)); 
do_look(victim, "auto");
arena = FIGHT_BUSY; 
return; 
}
}

/*=======================================================================*
 * function: do_decline                                                  *
 * purpose: to chicken out from a sent arena challenge                   *
 * written by: Doug Araya (whiplash@tft.nacs.net) 6-10-96                *
 *=======================================================================*/
void do_decline(CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim; 
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;


/*== make all invalid checks == */
  if(IS_NPC(ch))
   return;

 if( !IS_SET(ch->act,PLR_CHALLENGED) )
  {
  send_to_char("You have not been challenged.\n\r",ch);
  return;
  }

 if(argument[0]=='\0')
  {
  send_to_char("You must specify whose challenge you wish to decline.\n\r",ch);
  return;
  }
  
 if((victim = get_char_world(ch,argument)) == NULL)
  {
  send_to_char("They aren't logged in!\n\r",ch);
  return;
  }

 if(!IS_SET(victim->act,PLR_CHALLENGER) || victim != ch->challenger )
  {
  send_to_char("That player hasn't challenged you.\n\r",ch);
  return;
  }

 if(victim == ch)
 return; 

/*== now actually decline == */
victim->challenged = NULL;
REMOVE_BIT(victim->act,PLR_CHALLENGER); 
ch->challenger = NULL;
REMOVE_BIT(ch->act,PLR_CHALLENGED);
arena = FIGHT_OPEN;
send_to_char("Challenge declined!\n\r",ch);
act("$n has declined your challenge.",ch,NULL,victim,TO_VICT);
sprintf(buf,"#0[#7Arena#0] #L%s #7has declined #R%s#7's challenge.#n\n\r",
        ch->name, victim->name);
        do_info(ch,buf );

return;
}
