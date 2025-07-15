 /***************************************************************************
#
#
#  newclans.c and all contents are sole property of Jason White, who can do
#
#  as he sees fit with it.. copywrite April 19, 2001
#
#
#

***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> /* unlink() */
#include <stdarg.h>
#include <assert.h>
#include <sys/quota.h>
#include <pwd.h>
#include <grp.h>
#include <mntent.h>
#include "merc.h"
#include "olc.h"
void fix_exits( void );
void unlink_mob_index( MOB_INDEX_DATA *pMob );
void save_area( AREA_DATA *pArea );
void    new_load_rooms  args( ( FILE *fp ) );   /* OLC 1.1b */
void    load_mobiles    args( ( FILE *fp ) );    
void    load_objects    args( ( FILE *fp ) );
void    load_resets     args( ( FILE *fp ) );
void    load_rooms      args( ( FILE *fp ) );
void    load_shops      args( ( FILE *fp ) );
void    load_specials   args( ( FILE *fp ) );
void    new_load_area   args( ( FILE *fp ) );   /* OLC */
int find_clan_vnum( );
void allow(CHAR_DATA *ch, char *argument, CLAN_DATA *clan);
void clan_reject( CHAR_DATA *ch, char *argument, CLAN_DATA *clan);
void clan_accept( CHAR_DATA *ch, char *argument, CLAN_DATA *clan);
extern bool fBootDb;
int num_clans;
extern char *munch_colors(char *word);
CLAN_DATA * first_clan; /* first clan in the linked list */
CLAN_DATA * last_clan;  /* last clan in above list */
extern char strArea[MAX_INPUT_LENGTH];
FILE *		fpClan;
void new_clan_area( CLAN_DATA *clan );
void save_clans( );
void clan_syntax( CHAR_DATA *ch );
void bestow(CHAR_DATA *ch, char *argument, CLAN_DATA *clan);
void clan_prereq( CHAR_DATA *ch, char *argument, CLAN_DATA *clan);
void clan_mobile( CHAR_DATA *ch, char *argument, CLAN_DATA *clan);
char * const c_reqs [] = {
  "hp", "mana", "move", "legend", "status"
};

char * const c_flags [] = {
  "leader", "induct", "outcast", "bank", "finance", "bestow",
  "secret", "knowledge", "allow", "Applicant", "Rejected-Applicant", "Accepted-Applicant", "r13", "r14",
  "r15", "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", "r24",
  "r25", "r26", "r27", "r28", "r29", "r30", "r31", "r32"
};

int get_clan_req( char *type )
{
    int x;

    for ( x = 0; x < REQ_MAX; x++ )
      if ( !str_cmp(type, c_reqs[x]) )
        return x;
    return -1;
}

int get_clan_flag( char *type )
{
    int x;

    for ( x = 0; x < (sizeof(c_flags) / sizeof(c_flags[0])); x++ )
      if ( !str_cmp(type, c_flags[x]) )
        return x;
    return -1;
}

CLAN_DATA *get_plr_clan( char *name )
{
CLAN_DATA *clan;
CLAN_MEMBER *member;

  for ( clan = first_clan; clan; clan = clan->next )
  {
     for ( member = clan->first_member; member; member = member->next)
     {
        if (!str_cmp(member->name, capitalize(name)))
	   return clan;
     }
  }
  return NULL;
}
CLAN_MEMBER *get_member(char *name, CLAN_DATA *clan) {
        CLAN_MEMBER *member;

        for (member = clan->first_member; member; member = member->next)
                if (!str_cmp(member->name, capitalize(name)))
                        return member;

        return NULL;
}

CLAN_DATA *get_clan(CHAR_DATA *ch, char *name) {
        CLAN_DATA *clan;

        for (clan = first_clan; clan; clan = clan->next)
                if (!str_cmp(clan->name, name))
                        return clan;

        return NULL;
}

/* get a clan by its name as above, but pay no attention to what char is asking */
CLAN_DATA *clan_name(char *name) {
        CLAN_DATA *clan;

        for (clan = first_clan; clan; clan = clan->next) {
                if (!strcmp(strlower(clan->name), name))
                        return clan;
        }

        return NULL;
}
void clan_apply( CHAR_DATA *ch, CLAN_DATA *clan )
{
	char buf[MSL];
	CLAN_MEMBER *member;

	if ( ch->gold < clan->app_fee )
	{
	   chprintf( ch, "You can't afford the %2.0f gold application fee.\n\r", clan->app_fee );
	   return;
	}

        if ( ch->max_hit  < clan->prereq[REQ_HP]
	  || ch->max_mana < clan->prereq[REQ_MANA]
	  || ch->max_move < clan->prereq[REQ_MOVE]
	  || ch->lstatus  < clan->prereq[REQ_LEGEND]
	  || ch->race     < clan->prereq[REQ_STATUS] )
        {
	   stc( "You don't meet the prerequisites to join that clan yet.\n\r", ch);
	   return;
	}
	CREATE( member, CLAN_MEMBER, 1);

        strcpy(buf, ch->name);
        member->name = STRALLOC(buf);
        member->title = STRALLOC("Inductee");
	xSET_BIT(member->flags, CL_INDUCTEE);
        LINK(member, clan->first_member, clan->last_member, next, prev);
	member->bank = clan->app_fee;
	ch->gold -= clan->app_fee;
//        save_clans();
}
void clan_add(CHAR_DATA *ch, CLAN_DATA *clan) {
        char buf[MAX_STRING_LENGTH];
        CLAN_MEMBER *member;

        CREATE(member, CLAN_MEMBER, 1);

        strcpy(buf, ch->name);
        member->name = STRALLOC(buf);
        member->title = STRALLOC("Inductee");

        LINK(member, clan->first_member, clan->last_member, next, prev);


//        save_clans();
}
bool clan_remove(char *name, CLAN_DATA *clan) {
        CLAN_MEMBER *member;

        if ((member = get_member(name, clan)) == NULL)
                return FALSE;

        UNLINK(member, clan->first_member, clan->last_member, next, prev);
        STRFREE(member->name);
        STRFREE(member->title);
        DISPOSE(member);

        save_clans();
        return TRUE;
}

void do_clan(CHAR_DATA *ch, char *argument) {
	CLAN_DATA *clan;
	CLAN_DATA *cl;
	CLAN_MEMBER *member;
	CLAN_MEMBER *chm;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
        char arg_tmp[MIL];
	int i=0, x;
        AREA_DATA *pArea;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (!arg1 || arg1[0] == '\0') {
		clan_syntax(ch);
		return;
	}

	   if (!arg2 || arg2[0] == '\0')
              clan = get_plr_clan( ch->name );
	   else
	      clan = get_clan( ch, arg2 );

           if (strcmp( arg1, "list" )
	   && strcmp( arg1, "create" )
           &&  strcmp( arg1, "show" ))
           {
 	      if ( clan == NULL)
	      {
		 send_to_char("No clan goes by that abbreviation.\n\r", ch);
		 return;
	      }
	   }
	if (clan == NULL)
        {
		if (strcmp(arg1, "create")
                &&  strcmp(arg1, "save")
		&&  strcmp(arg1, "list")) 
		{
                   for ( ; ; )
                   {
                        argument = one_argument( argument, arg_tmp );
			strcat( arg2, " ");
			strcat( arg2, arg_tmp);
			if ((clan = get_clan(ch, arg2)) != NULL)
			   break;
			if (!argument || argument[0] == '\0')
 			   break;
		   }
                        if (clan == NULL
			&& strcmp(arg1, "create"))
                        {
			   send_to_char("No clan goes by that abbreviation.\n\r", ch);
  	 	  	   return;
			}

		}
	}
	if (clan != NULL && !strcmp(arg1, "create")) 
	{
		send_to_char("There is already a clan by that abbreviation.\n\r", ch);
		return;
	}

        if (!strcmp(arg1, "save")) {
                stc("Saving clans.\n\r", ch);
                save_clans();
                return;
        }

	else if (!strcmp(arg1, "list")) {
		send_to_char("#cClans that you know about:#w\r\n", ch);
		for (clan = first_clan; clan; clan = clan->next) {
			i++;
			if (get_member(ch->name, clan) != NULL)
				chprintf(ch, "#y%d#o: #w%s#w #g(#g%s#g)\r\n", i, clan->title, clan->name);
			else if (clan->ext_sec < 6 )
				chprintf(ch, "#y%d#o: #w%s#w #y(#o%s#y)\r\n", i, clan->title, clan->name);
		}
		return;
	} 
        else if (!strcmp(arg1, "accept"))
        {
                  sprintf( arg_tmp, "%s %s", arg2, argument);
               if (!IS_IMMORTAL( ch ))
                  clan_accept(ch, arg_tmp, clan);
               else
                  clan_accept(ch, argument, clan);
               return;

        }

	else if (!strcmp(arg1, "reject"))
	{
                  sprintf( arg_tmp, "%s %s", arg2, argument);
               if (!IS_IMMORTAL( ch ))
                  clan_reject(ch, arg_tmp, clan);
               else
                  clan_reject(ch, argument, clan);
               return;

	}
	else if (!strcmp(arg1, "apply")) 
	{
           if ( IS_IMMORTAL( ch ))
	   {
              stc("You can't apply to join a clan.\n\r", ch );
	      return;
	   }
	   if (get_plr_clan( ch->name ))
	   {
	      if (clan != get_plr_clan(ch->name))
	      {
	         stc("You are already applying to another clan, to stop applying, type 'Clan Apply <clan>'\n\r", ch);
	         return;
	      }
	      else if (clan == get_plr_clan(ch->name))
	      {
	         if ((chm = get_member(ch->name, clan)) != NULL)
		    clan_remove( ch->name, clan );
		 else
		    log_string( "Error in Do_Clan : Apply : no member");
	         return;
	      }
	   }
	   else
	   {
	      clan_apply( ch, clan );
	   }
	      save_clans();
	      return;
	} 

	else if (!strcmp(arg1, "create")) 
	{
		if (!arg2 || arg2[0] == '\0') {
	                send_to_char("What name do you wish to give your new clan?\r\n", ch);
        	        return;
	        }

	        CREATE(clan, CLAN_DATA, 1);
	        LINK(clan, first_clan, last_clan, next, prev);

		if (clan->name)
			STRFREE(clan->name);
		clan->name = STRALLOC(munch_colors(arg2));
		if (clan->title)
			STRFREE(clan->title);
		clan->title = STRALLOC(arg2);

		clan->int_sec = 5;
		clan->ext_sec = 5;

		clan_add(ch, clan);
		chm = clan->first_member;
		STRFREE(chm->title);
		chm->title = STRALLOC("Leader");
		xSET_BIT(chm->flags, CL_LEADER);
		xSET_BIT(chm->flags, CL_ALLOW);
		xSET_BIT(chm->flags, CL_INDUCT);
		xSET_BIT(chm->flags, CL_OUTCAST);
		xSET_BIT(chm->flags, CL_BESTOW);
		xSET_BIT(chm->flags, CL_BANK);
		xSET_BIT(chm->flags, CL_KNOWLEDGE);
		xSET_BIT(chm->flags, CL_FINANCE);

		new_clan_area( clan );

		chprintf(ch, "Clan '%s' created.\r\n", clan->name);
		save_clans();
		return;
	} 
        	if (clan != NULL)
	           chm = get_member(ch->name, clan);

	if (!strcmp(arg1, "info")) {
		if (!IS_IMMORTAL(ch)
		 && (!chm || !xIS_SET(chm->flags, CL_LEADER))) {
			send_to_char("You are not the leader of that clan.\r\n", ch);
			return;
		}

		STRFREE(clan->info);
		clan->info = STRALLOC(argument);

		save_clans();

		send_to_char("You have changed the clan's info.\n\r", ch);
		return;


	} else if (!strcmp( arg1, "bestow" )) {
	       sprintf( arg_tmp, "%s %s", arg2, argument );
	       if (!IS_IMMORTAL( ch ))
                  bestow(ch, arg_tmp, clan);
               else
                  bestow(ch, argument, clan);
               return;

	} else if (!strcmp( arg1, "guard")) {
	        if (!IS_IMMORTAL(ch)
                && (!chm || !xIS_SET(chm->flags, CL_LEADER))) {
                        send_to_char("You are not the leader of that clan.\r\n", ch);
                        return;
                }
                sprintf( arg_tmp, "%s %s", arg2, argument);
               if (!IS_IMMORTAL( ch ))
                  clan_mobile(ch, arg_tmp, clan);
               else
                  clan_mobile(ch, argument, clan);
               return;

	} else if (!strcmp( arg1, "prereq")) {
		if (!IS_IMMORTAL(ch)
                && (!chm || !xIS_SET(chm->flags, CL_LEADER))) {
                        send_to_char("You are not the leader of that clan.\r\n", ch);
                        return;
		}
		sprintf( arg_tmp, "%s %s", arg2, argument);
               if (!IS_IMMORTAL( ch ))
                  clan_prereq(ch, arg_tmp, clan);
               else
                  clan_prereq(ch, argument, clan);
               return;

        } else if (!strcmp( arg1, "allow" )) {
	       sprintf( arg_tmp, "%s %s", arg2, argument);
	       if (!IS_IMMORTAL( ch ))
                  allow(ch, arg_tmp, clan);
	       else
	          allow(ch, argument, clan);
	       return;
	} else if (!strcmp(arg1, "setfee")) {
                i = atoi(argument);
	   	if (!IS_IMMORTAL(ch)
                && (!chm || !xIS_SET(chm->flags, CL_LEADER))) {
                        send_to_char("You are not the leader of that clan.\r\n", ch);
                        return;
                }

		clan->app_fee = i;
		save_clans();
		return;
	} else if (!strcmp(arg1, "name")) {
		if (!IS_IMMORTAL(ch)
		&& (!chm || !xIS_SET(chm->flags, CL_LEADER))) {
			send_to_char("You are not the leader of that clan.\r\n", ch);
			return;
		}

		STRFREE(clan->title);
		clan->title = STRALLOC(argument);

		save_clans();

		send_to_char("You have changed the clan's name.\n\r", ch);
		return;
	} else if (!strcmp(arg1, "abbr")) {
		argument = munch_colors(argument);

		for (cl = first_clan;cl;cl = cl->next) {
			if (!str_cmp(cl->name, argument)) {
				send_to_char("There is already a clan with that abbreviation. Please choose another.\n\r", ch);
				return;
			}
		}

		if (!IS_IMMORTAL(ch)
		&& (!chm || !xIS_SET(chm->flags, CL_LEADER))) {
			send_to_char("You are not the leader of that clan.\r\n", ch);
			return;
		}

		STRFREE(clan->name);
		clan->name = STRALLOC(argument);

		save_clans();

		send_to_char("You have changed the clan's abbreviation.\n\r", ch);
		return;
	} else if (!strcmp(arg1, "disband")) {
		if (!IS_IMMORTAL(ch)
		&& (!chm || !xIS_SET(chm->flags, CL_LEADER))) {
			send_to_char("You are not the leader of that clan.\r\n", ch);
			return;
		}

		send_to_char("You disband the clan.\r\n", ch);
		pArea = clan->area;	
		UNLINK(clan, first_clan, last_clan, next, prev);
		STRFREE(clan->name);
		for (member = clan->first_member; member; member = member->next) {
			STRFREE(member->name);
			STRFREE(member->title);
			if (member->prev)
				DISPOSE(member->prev);
		}
		DISPOSE(clan->last_member);

		DISPOSE(clan);
		free_area( pArea );		
		save_clans();
		return;
	} else if (!strcmp(arg1, "intsec")) {
		i = atoi(argument);
                if (!IS_IMMORTAL(ch)
                && (!chm || !xIS_SET(chm->flags, CL_LEADER))) {
                        send_to_char("You are not the leader of that clan.\r\n", ch);
                        return;
                }
		clan->int_sec = URANGE(1, i, 6);
		chprintf(ch, "You change the internal secrecy level of %s#w to %d.\n\r", clan->title, clan->int_sec);
		save_clans();
		return;
	} else if (!strcmp(arg1, "extsec")) {
		i = atoi(argument);
                if (!IS_IMMORTAL(ch)
                && (!chm || !xIS_SET(chm->flags, CL_LEADER))) {
                        send_to_char("You are not the leader of that clan.\r\n", ch);
                        return;
                }

		clan->ext_sec = URANGE(1, i, 6);
		chprintf(ch, "You change the external secrecy level of %s#w to %d.\n\r", clan->title, clan->ext_sec);
		save_clans();
		return;
	} else if (!strcmp(arg1, "deposit")) {
		OBJ_DATA *atm;
		bool bank = FALSE;
		int amt = atoi(argument);

		if (!chm) {
			send_to_char("You aren't even a member of that clan!\n\r", ch);
			return;
		}

		for (atm = ch->in_room->contents; atm; atm = atm->next_content) {
        	        if (atm->item_type == ITEM_BANK) {
                	        bank = TRUE;
                        	break;
                	}
        	}

        	if (!bank) {
                	send_to_char("There is no bank here.\r\n", ch);
                	return;
        	}

		if (ch->gold < amt ) {
			send_to_char("You don't have that many #cgold coins.\n\r", ch);
			return;
		}

		if (amt <= 0) {
			send_to_char("Err, how do you plan to do that?\n\r", ch);
			return;
		}

		ch->gold -= amt;
		chm->bank += amt;
		clan->bank += amt;
		save_clans();

		chprintf(ch, "You deposit #c%d gold coin%s #winto the %s#w clan bank.\n\r", 
			amt, amt > 1 ? "s" : "", clan->title);
		return;
	} else if (!strcmp(arg1, "withdraw")) {
		OBJ_DATA *atm;
		bool bank = FALSE;
		int amt = atoi(argument);

		if (!chm && !IS_IMMORTAL(ch)) {
			send_to_char("You aren't even a member of that clan!\n\r", ch);
			return;
		}

		if (!xIS_SET(chm->flags, CL_BANK) && !IS_IMMORTAL(ch) ) {
			send_to_char("You don't have permission to withdraw from the clan bank.\n\r", ch);
			return;
		}

		if (!ch->in_room->contents) {
			send_to_char("There is no bank here.\n\r", ch);
			return;
		}

		for (atm = ch->in_room->contents; atm; atm = atm->next_content) {
        	        if (atm && atm->item_type == ITEM_BANK) {
                	        bank = TRUE;
                        	break;
                	}
        	}

        	if (!bank) {
                	send_to_char("There is no bank here.\r\n", ch);
                	return;
        	}

		if (chm->bank < amt && !xIS_SET(chm->flags, CL_FINANCE)) {
			send_to_char("You don't have that many #cgold coins in your bank account.\n\r", ch);
			return;
		}

		if (clan->bank < amt) {
			send_to_char("There aren't that many #cgold coins#w in that clan's bank.\n\r", ch);
			return;
		}

		if (amt <= 0) {
			send_to_char("Err, how do you plan to do that?\n\r", ch);
			return;
		}

		if (ch->gold > 1000000000) {
			send_to_char("You are carrying too much #cgold#w already.\n\r", ch);
			return;
		}

		ch->gold += amt;
		chm->bank -= amt;
		clan->bank -= amt;
		save_clans();

		chprintf(ch, "You withdraw #c%d gold coin%s#w from the %s#w clan bank.\n\r", 
			amt, amt > 1 ? "s" : "", clan->title);
		return;
	} else if (!strcmp(arg1, "show")) {
	    if (!chm && clan->ext_sec >= 5
                && !IS_IMMORTAL(ch)) {
		send_to_char("You cannot view info on that clan.\n\r", ch);
		return;
	    }

		chprintf(ch, "#cName: #c%s#c   Abbreviation: #c%s\n\r",
			clan->title, clan->name);
		chprintf(ch, "#cApplication Fee: %2.0f \n\r", clan->app_fee);
		chprintf(ch, "#cRequirements: ");
		for ( x = 0; x < REQ_MAX; x++ )
		  chprintf( ch, "#c%s: %d, ", c_reqs[x], clan->prereq[x]);
		chprintf(ch, "\n\r#cInfo: #c%s\n\r", clan->info);
	    if  ((chm && xIS_SET(chm->flags, CL_KNOWLEDGE))
	      || IS_IMMORTAL(ch))
		chprintf(ch, "#cSecrecy - Internal: #c%d#c   External: #c%d#w\n\r",
			clan->int_sec, clan->ext_sec);
            if (( chm && xIS_SET(chm->flags, CL_FINANCE))
            ||    IS_IMMORTAL(ch))
		chprintf(ch, "#cBank: #c%2.0f gold\n\r", clan->bank);
	    if ((!chm && clan->ext_sec <= 3)
	    ||  ( chm && clan->int_sec <= 3)
	    ||  ( chm && xIS_SET(chm->flags, CL_KNOWLEDGE))
	    ||    IS_IMMORTAL(ch)) {
		chprintf(ch, "#cMembers:#w\r\n");
		for (member = clan->first_member; member; member = member->next) {
		    if (!xIS_SET(member->flags, CL_SECRET)
		    ||    IS_IMMORTAL(ch)
		    || (  chm && xIS_SET(chm->flags, CL_KNOWLEDGE))) {
			chprintf(ch, "%s", member->name);
        	    if ((!chm && clan->ext_sec <= 2)
        	    ||  ( chm && clan->int_sec <= 2)
        	    ||  ( chm && xIS_SET(chm->flags, CL_KNOWLEDGE))
		    ||    IS_IMMORTAL(ch))
			chprintf(ch, " - %s", member->title);
        	    if ((!chm && clan->ext_sec <= 1)
        	    ||  ( chm && clan->int_sec <= 1)
        	    ||  ( chm && xIS_SET(chm->flags, CL_KNOWLEDGE))
		    ||    IS_IMMORTAL(ch))
			chprintf(ch, " #g(#g%s#g)", ext_flag_string(&member->flags, c_flags));
			chprintf(ch, "\n\r");
		    if (  chm == member
		    ||  ( chm && xIS_SET(chm->flags, CL_FINANCE))
		    ||    IS_IMMORTAL(ch))
			chprintf(ch, "         #c- Donated #c%2.0f gold#w\n\r",
				member->bank);
		    }
		}
	    }
		return;
	}
	clan_syntax(ch);
	return;
}

/* induct <person> <clan> */
void do_induct(CHAR_DATA *ch, char *argument) {
	CHAR_DATA *victim;
	CLAN_DATA *clan;
	CLAN_MEMBER *chm;
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_INPUT_LENGTH];

	argument = one_argument(argument, arg1);

	if ((victim=get_char_room(ch, arg1))==NULL) {
		send_to_char("They're not here.\r\n", ch);
		return;
	}

	if (IS_NPC(victim)) {
		send_to_char("They don't look interested in your offer.\r\n", ch);
		return;
	}

	if ((clan=get_clan(ch, argument)) == NULL) {
		send_to_char("There is no such clan.\r\n", ch);
		return;
	}

	if ((chm = get_member(ch->name, clan)) == NULL
	&&  !IS_IMMORTAL(ch)) {
		send_to_char("You are not in that clan.\n\r", ch);
		return;
	}

	if (!IS_IMMORTAL(ch) 
	&& !xIS_SET(chm->flags, CL_INDUCT)) {
		send_to_char("You are permitted to induct new members into that clan.\r\n", ch);
		return;
	}

	if (get_member(victim->name, clan)!=NULL) {
		send_to_char("They are already a member of that clan.\r\n", ch);
		return;
	}

	if (!IS_IMMORTAL(ch)
	/* &&  !IS_CONSENTING(victim, ch) */) {
                send_to_char("You may not induct them without their consent.\n\r", ch);
                return;
        }

	clan_add(victim, clan);
        save_clans();        
	sprintf(buf, "You add $N to %s#n.", clan->title);
	act( buf, ch, NULL, victim, TO_CHAR, FALSE);
	sprintf(buf, "$n adds you to %s#n.", clan->title);
	act( buf, ch, NULL, victim, TO_VICT, FALSE);
}

/* outcast <person> <clan> */
void do_outcast(CHAR_DATA *ch, char *argument) {
	CHAR_DATA *victim;
	CLAN_DATA *clan;
	CLAN_MEMBER *chm;
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	victim=get_char_world(ch, arg1);
	
	if ((clan=get_clan(ch, (char *)arg2)) == NULL) {
		send_to_char("There is no such clan.\r\n", ch);
		return;
	}
	
	if (get_member((char *)arg1, clan) == NULL) {
		send_to_char("That would be much easier if they were in that clan to begin with.\r\n", ch);
		return;
	}

        if ((chm = get_member(ch->name, clan)) == NULL
        &&  !IS_IMMORTAL( ch )) {
                send_to_char("You are not in that clan.\n\r", ch);
                return;
        }
	
	if (ch != victim
	&& !IS_IMMORTAL( ch )
	&& !xIS_SET(chm->flags, CL_OUTCAST)) {
		send_to_char("You do not have permission to outcast from clan.\r\n", ch);
		return;
	}

	if (!xIS_SET(chm->flags, CL_LEADER)
	&&  !IS_IMMORTAL( ch )) {
		chm = get_member(victim->name, clan);
		if (xIS_SET(chm->flags, CL_LEADER)) {
			act( "Hey, $N is a leader and you aren't! You can't outcast $M!", ch, NULL, victim, TO_CHAR, FALSE);
			act( "Hey, $n is trying to outcast you but isn't a leader!", ch, NULL, victim, TO_VICT, FALSE);
			return;
		}
	}

	clan_remove(arg1, clan);

	if (ch == victim) {
		sprintf(buf, "You renounce the ways of %s#n.", clan->title);
		act( buf, ch, NULL, NULL, TO_CHAR, FALSE);
		sprintf(buf, "$n renounces the ways of %s#n.", clan->title);
		act( buf, ch, NULL, NULL, TO_ROOM, FALSE);
	} else {
		sprintf(buf, "You remove $T from %s#n.", clan->title);
		act( buf, ch, NULL, capitalize(arg1), TO_CHAR, FALSE);
	    if (victim) {
		sprintf(buf, "$n removes you from %s#n.", clan->title);
		act( buf, ch, NULL, victim, TO_VICT, FALSE);
	    }
	}
}

void bestow(CHAR_DATA *ch, char *argument, CLAN_DATA *clan) {
	CHAR_DATA *victim;
	CLAN_MEMBER *chm;
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];

	if (IS_NPC(ch)) {
		do_huh(ch, "");
		return;
	}

        argument = one_argument(argument, arg1);

                chm = get_member(ch->name, clan);

	if ( !IS_IMMORTAL( ch )
	&&	!xIS_SET(chm->flags, CL_BESTOW)) {
		send_to_char("You don't have permission to bestow titles for that clan.\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg1);
	if (!victim || !argument || argument[0] == '\0') {
		send_to_char("Bestow what title upon who?\n\r", ch);
		return;
	}

	chm = get_member(victim->name, clan);

	if (!chm) {
		send_to_char("They aren't even a member of that clan.\n\r", ch);
		return;
	}

	smash_tilde(argument);
	STRFREE(chm->title);
	chm->title = STRALLOC(argument);

	save_clans();

        if (ch == victim) {
                sprintf(buf, "You declare yourself to be %s&w of %s#n.",
			chm->title, clan->title);
                act( buf, ch, NULL, NULL, TO_CHAR, FALSE);
                sprintf(buf, "$n declares $mself of %s&w of %s&w.",
			chm->title, clan->title);
                act( buf, ch, NULL, NULL, TO_ROOM, FALSE);
        } else {
                sprintf(buf, "You bestow upon $N the title of %s&w of %s&w.", chm->title, clan->title);
                act( buf, ch, NULL, victim, TO_CHAR, FALSE);
            if (victim) {
                sprintf(buf, "$n bestows upon you the title of %s&w of %s&w.", chm->title, clan->title);
                act( buf, ch, NULL, victim, TO_VICT, FALSE);
            }
        }
}

void clan_mobile( CHAR_DATA *ch, char *argument, CLAN_DATA *clan) {
  char arg1[MIL];
  int i, iHash;
  MOB_INDEX_DATA *pMob;

   argument = one_argument(argument, arg1);

   if (!argument || argument[0] == '\0' || !arg1 || arg1[0] == '\0')
   {
         chprintf( ch, "Syntax: clan guard%s create.\n\r", IS_IMMORTAL(ch) ? " <clan abbr>" : "");
         chprintf( ch, "Syntax: clan guard%s upgrade <stat> <amount>.\n\r", IS_IMMORTAL(ch) ? " <clan abbr>" : "");

      return;
   }

   for (i = clan->area->lvnum; i <= clan->area->uvnum; i++)
   {
      	if ((pMob = get_mob_index(i)) != NULL)
        {
	   if (!strcmp(pMob->player_name, argument))
 	      break;
           else
              pMob = NULL;
	}
   }

   if (!strcmp(arg1, "create"))
   {
      if (pMob != NULL)
      {
         stc("A guard with that abbreviation already exists.\n\r", ch);
	 return;
      }

      for (i = clan->area->lvnum; i <= clan->area->uvnum; i++)
      {
         pMob = get_mob_index(i);
	 if (!pMob)
	    break;
      }

      pMob = new_mob_index();
      pMob->vnum = i;
      pMob->area = clan->area;
      free_string( pMob->player_name );
      pMob->player_name = str_dup( argument );
      pMob->act                   = meb(ACT_IS_NPC);
      iHash                       = i % MAX_KEY_HASH;
      pMob->next                  = mob_index_hash[iHash];
      mob_index_hash[iHash]       = pMob;

      chprintf( ch, "Guard Created with vnum %d.\n\r", pMob->vnum );
		  save_clans();
      return;
   }
   else if (!strcmp(arg1, "show"))
   {
      if (!pMob)
      {
         stc("No guardian exists with that abbr.\n\r", ch);
	 return;
      }
    chprintf( ch, "Name: %s.\n\r",    pMob->player_name );

    chprintf( ch, "Sex: %s.\n\r",
    pMob->sex == SEX_MALE    ? "male"   :
    pMob->sex == SEX_FEMALE  ? "female" : "neutral"    );

    chprintf( ch, "Hp: %d.  Mana: %d.  Move: %d.  Primal: %d.\n\r",
    pMob->level * 100,
    pMob->level * 100,
    pMob->level * 100,
    pMob->level * (number_range(10,20)/10) );

      return;
   }

}


void clan_prereq( CHAR_DATA *ch, char *argument, CLAN_DATA *clan) {
  char arg1[MIL];
  int i, value;
  CLAN_MEMBER *chm;

   argument = one_argument(argument, arg1);

                chm = get_member(ch->name, clan);

        if (!IS_IMMORTAL( ch )
        &&  !xIS_SET(chm->flags, CL_ALLOW)) {
                send_to_char("You don't have permission to alter priviledges for that clan", ch);
		return;
	}
   if ((i = get_clan_req( arg1 )) == -1)
   {
      stc("That isn't a valid prereq.\n\r", ch);
      return;
   }
   value = atoi( argument );

   clan->prereq[i] = value;
   chprintf( ch, "Prerequisite for %s has been changed to %d", c_reqs[i], value);
   
}


void clan_accept( CHAR_DATA *ch, char *argument, CLAN_DATA *clan) {
CHAR_DATA *victim;
CLAN_MEMBER *chm;
char arg1[MSL];

        argument = one_argument(argument, arg1);

                chm = get_member(ch->name, clan);

        if (!IS_IMMORTAL( ch )
        &&  !xIS_SET(chm->flags, CL_INDUCT)) {
                send_to_char("You don't have permission to accept applicants for that clan", ch);
                return;
        }

        victim = get_char_room(ch, arg1);
        if (victim )
        {
           if ((chm = get_member( victim->name, clan)) != NULL)
	   {
              send_to_char("Your clan application has been accepted\n\r\n\r", victim);
  	      xREMOVE_BIT( chm->flags, CL_INDUCTEE );
	      clan->bank += chm->bank;
	      chm->bank = 0;
	      return;
	   }
        }

        if ( (chm = get_member( arg1, clan)) != NULL)
	{
	   xSET_BIT( chm->flags, CL_ACCEPTED );
	   return;
	}
}


void clan_reject( CHAR_DATA *ch, char *argument, CLAN_DATA *clan) {
CHAR_DATA *victim;
CLAN_MEMBER *chm;
char arg1[MSL];

        argument = one_argument(argument, arg1);

                chm = get_member(ch->name, clan);

        if (!IS_IMMORTAL( ch )
        &&  !xIS_SET(chm->flags, CL_INDUCT)) {
                send_to_char("You don't have permission to reject applicants for that clan", ch);
                return;
        }

        victim = get_char_room(ch, arg1);
        if (victim )
        {
           if ((chm = get_member( victim->name, clan)) != NULL)
	   {
              send_to_char("Your clan application has been rejected\n\r\n\r", victim);
	      victim->gold += chm->bank;
   	      clan_remove( arg1, clan);
	      return;
	   }
        }

        if ( (chm = get_member( arg1, clan)) != NULL)
	{
	   xSET_BIT( chm->flags, CL_REJECTED );
	   xREMOVE_BIT( chm->flags, CL_INDUCTEE );
	   return;
	}
}


void allow(CHAR_DATA *ch, char *argument, CLAN_DATA *clan) {
	int i, x;
	CHAR_DATA *victim;
	CLAN_MEMBER *chm;
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch)) {
                do_huh(ch, "");
                return;
        }

        argument = one_argument(argument, arg1);

                chm = get_member(ch->name, clan);

        if (!IS_IMMORTAL( ch )
	&&  !xIS_SET(chm->flags, CL_ALLOW)) {
                send_to_char("You don't have permission to alter priviledges for that clan", ch);
                return;
        }

        victim = get_char_room(ch, arg1);
        if (!victim )
	{
           send_to_char("Bestow title upon who?\n\r\n\r", ch);
                return;
        }

	if ( !argument || argument[0] == '\0') 
	{
	   for (x = 0; x < 9; x++)
              chprintf( ch, "%s%c ", c_flags[x], (x != 8) ? ',' : '.' );
              stc( "\n\r", ch);
	}

	i = get_clan_flag(argument);
	if (i == -1) {
		send_to_char("There is no such priveledge.\n\r", ch);
		return;
	}


	if ((i == CL_LEADER || i == CL_ALLOW)
	&&  !IS_IMMORTAL( ch )) {
	    if (!chm) {
		send_to_char("You aren't even a member of that clan!\n\r", ch);
		return;
	    }
	    if (!xIS_SET(chm->flags, CL_LEADER)) {
		send_to_char("Only a leader can grant that priviledge.\n\r", ch);
		return;
	    }
	}

	if (ch == victim
	&& !IS_IMMORTAL( ch )) {
		send_to_char("Grant yourself priviledges? That would be a trick!\n\r", ch);
		return;
	}

        chm = get_member(victim->name, clan);

        if (!chm) {
                send_to_char("They aren't even a member of that clan.\n\r", ch);
                return;
        }


	xTOGGLE_BIT(chm->flags, i);


        if (xIS_SET(chm->flags, i)) {
                sprintf(buf, "You grant $N %s priviledges in %s&w.", argument, clan->title);
                act( buf, ch, NULL, victim, TO_CHAR, FALSE);
                sprintf(buf, "$n grants you %s priviledges in %s&w.", argument, clan->title);
                act( buf, ch, NULL, victim, TO_VICT, FALSE);
        } else {
                sprintf(buf, "You restrict $N from %s priviledges in %s&w.", argument, clan->title);
                act( buf, ch, NULL, victim, TO_CHAR, FALSE);
                sprintf(buf, "$n restricts you from %s priviledges in %s&w.", argument, clan->title);
                act( buf, ch, NULL, victim, TO_VICT, FALSE);
	}
	save_clans();
}

void 
save_clan_list( )
{
FILE *fp;
CLAN_DATA *pClan;

    if ( ( fp = fopen( "./clans/clan.lst", "w" ) ) == NULL )
    {
        bug( "Save_clan_list: fopen", 0 );
        perror( "clan.lst" );
    }
    else
    {
        for( pClan = first_clan; pClan; pClan = pClan->next )
        {
            fprintf( fp, "%s\n", pClan->area->filename );
        }
        fprintf( fp, "$\n" );
        fclose( fp );
    }

    return;
}


void load_clanlist( )
{
    FILE *fpList;
    char buf[MSL];
//    char to_open[MSL];

    first_clan = NULL;
    last_clan = NULL;
    if ( ( fpList = fopen( CLAN_LIST, "r" ) ) == NULL )
    {
        bug( "Missing clanlist file %s", CLAN_LIST );
        exit( 1 );
    }
            for ( ; ; )
        {
            strcpy( strArea, fread_word( fpList ) );
            if ( strArea[0] == '$' )
                break;

            if ( strArea[0] == '-' )
            {
                fpClan = stdin;
            }
            else
            {
                sprintf(buf,"loading clan: %s",strArea);
                log_string(buf);
                if ( ( fpClan = fopen( strArea, "r" ) ) == NULL )
                {
                    perror( strArea );
                    exit( 1 );
                }
            }

            for ( ; ; )
            {
                char *word;

                if ( fread_letter( fpClan ) != '#' )
                {
                    bug( "Boot_db: # not found.", 0 );
                    exit( 1 );
                }

                word = fread_word( fpClan );

                     if ( word[0] == '$'               )                 break;
                else if ( !str_cmp( word, "CLANDATA"     ) ) load_clan(fpClan);
                else if ( !str_cmp( word, "AREADATA" ) )        /* OLC */
		{
                    new_load_area( fpClan );
		    SET_BIT(last_area->area_flags, AREA_CLAN);
		}
                else if ( !str_cmp( word, "MOBILES"  ) ) load_mobiles (fpClan);
                else if ( !str_cmp( word, "OBJECTS"  ) ) load_objects (fpClan);
                else if ( !str_cmp( word, "RESETS"   ) ) load_resets  (fpClan);
                else if ( !str_cmp( word, "SHOPS"    ) ) load_shops   (fpClan);
                else if ( !str_cmp( word, "SPECIALS" ) ) load_specials(fpClan);
                else if ( !str_cmp( word, "ROOMDATA" ) )        /* OLC 1.1b */
                    new_load_rooms( fpClan );

                else
		{
                    bug( "load_clanlist: bad section name: %s", word );
                    exit( 1 );
                }
            }

            if ( fpClan != stdin )
                fclose( fpClan );
            fpClan = NULL;
        }
        fclose( fpList );
return;
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )    if ( !str_cmp( word, literal ) ){field  = value;fMatch = TRUE;break;}
#define FKEY( literal, field, value )    if ( !str_cmp( word, literal ) ){strcpy( field,value);fMatch = TRUE;break;}
void load_clan( FILE *fp )
{
CLAN_DATA *pClan;
CLAN_MEMBER *member;
char *word;
bool fMatch;
//int counter = 0;
int x;
int mode = 0;

CREATE(pClan, CLAN_DATA, 1);

pClan->prev = NULL;
pClan->next = NULL;
pClan->area = last_area;
SET_BIT (pClan->area->area_flags, AREA_CLAN);
pClan->type = 1;
pClan->name = strdup( "New Clan" );
pClan->bank = 0;
pClan->app_fee = 0;
for(x = 0; x < REQ_MAX; x++)
pClan->prereq[x] = 0;
pClan->top_member = 0;
    for ( ; ; )
    {
       word   = feof( fp ) ? "End" : fread_word( fp );
       fMatch = FALSE;

     if (mode == 0) {
       switch ( UPPER(word[0]) )
       {

                case '#':
                        if ( !str_cmp(word, "#MEMBER")) {
                                mode = 1;
                                CREATE(member, CLAN_MEMBER, 1);
                                LINK(member, pClan->first_member, pClan->last_member, next, prev);
                        }
                        break;
	   case 'A':
	    KEY( "Appfee", pClan->app_fee, fread_double( fp ) );
	    break;
	   case 'B':
	    KEY( "Bank", pClan->bank, fread_double( fp ) );
            break;
	   case 'E':
	    KEY( "Ext_Sec", pClan->ext_sec, fread_number( fp ) );
            if (!str_cmp( word, "End") )
	    {
	       log_string("Clan %s loaded", pClan->name);
	       LINK( pClan, first_clan, last_clan, next, prev );
               return;
            }
            break;
	   case 'I':
	    KEY( "Info", pClan->info, fread_string( fp ) );
	    KEY( "Int_sec", pClan->int_sec, fread_number( fp ) );
	    break;
           case 'N':
            FKEY( "Name", pClan->name, fread_string( fp ) );
            break;
	    case 'T':
             KEY( "Type", pClan->type, fread_number( fp ) );
	     KEY( "Title", pClan->title, fread_string( fp ) );
             break;
	}
        }
        else if (mode == 1) {
                switch (UPPER(word[0])) {
                case '*':
                        fMatch = TRUE;
                        fread_to_eol(fp);
                        break;

		case 'B':
		   KEY("Bank", member->bank, fread_double( fp ) );
		  break;
                case 'E':
                        if (!strcmp(word, "End")) {
                                fMatch = TRUE;
                                mode = 0;
                                member = NULL;
                        }
                        break;

                case 'F':
                        KEY("Flags", member->flags, fread_bitvector(fp));
                        break;

                case 'N':
                        KEY("Name", member->name, fread_string(fp));
                        break;

                case 'T':
                        KEY("Title", member->title, fread_string(fp));
                        break;
                }
        }

/*        if ( !fMatch )
        {
            bug( "Fread_clan: no match: %s", word );
            fread_to_eol( fp );
        } */
    }

}

void do_newclan( CHAR_DATA *ch, char *argument )
{
CLAN_DATA *pClan;
int x;

if (IS_NPC(ch)) return;

CREATE( pClan, CLAN_DATA, 1);

pClan->type = 1;
pClan->name = strdup( "New Clan" );
pClan->bank = 0;
for(x = 0; x < REQ_MAX; x++)
pClan->prereq[x] = 0;

LINK(pClan, first_clan, last_clan, next, prev);

}

void clan_list( CHAR_DATA *ch, char *argument )
{
CLAN_DATA *pClan;
int col = 0;

  if (!argument || argument[0] == '\0')
  {
     stc( "-------------------------------=[Clans]=------------------------------------\n\r", ch);
     for (pClan = first_clan; pClan; pClan = pClan->next)
     {
         chprintf( ch, "%s", pClan->name);
         if (pClan->next != NULL)
             stc( ", ", ch); 
            if ( ++col % 5 == 0 )
                send_to_char( "\n\r", ch );
    }
    if ( col % 6 != 0 )
        send_to_char( "\n\r", ch );
    return;
  }      
  else
  {
     for (pClan = first_clan; pClan; pClan = pClan->next)
     {
        if ( !str_cmp( pClan->name, argument ) )
	{
           chprintf( ch, "Name    : %s\n\r", pClan->name );
	   return;
	}

     }
     stc("That clan doesn't exist.\n\r", ch);
  }

}
void do_showclan( CHAR_DATA *ch, char *argument )
{
CLAN_DATA *pClan;
CLAN_MEMBER *member;

  if (!argument || argument[0] == '\0')
  {
     stc( "-------------------------------=[Clans]=------------------------------------\n\r", ch);
     for (pClan = first_clan; pClan; pClan = pClan->next)
     {
         chprintf( ch, "%s", pClan->name);
         chprintf(ch, "Members:\r\n");
                for (member = pClan->first_member; member; member = member->next)
                        chprintf(ch, "  %s\n\r", member->name);

     }
        return;
  }      
  else
  {
     for (pClan = first_clan; pClan; pClan = pClan->next)
     {
        if ( !str_cmp( pClan->name, argument ) )
	{
	   chprintf( ch, "Filename: %s\n\r", pClan->area->filename);
           chprintf( ch, "Name    : %s\n\r", pClan->name );
 	   stc( "        : ", ch );
	   chprintf( ch, "Bank    : %2.0f\n\r", pClan->bank );
	   return;
	}

     }
     stc("That clan doesn't exist.\n\r", ch);
  }
}

void save_clans( )
{
CLAN_DATA *pClan, *pClan_prev;
MOB_INDEX_DATA *pMob;
int i;
save_clan_list( );

        for( pClan = last_clan; pClan != NULL; pClan = pClan_prev )    
	{
	   pClan_prev = pClan->prev;
	   save_area( pClan->area );
	   for (i = pClan->area->lvnum; i <= pClan->area->uvnum; i++)
  	   {
	      if ((pMob = get_mob_index(i)) != NULL)
	      {
	          unlink_mob_index( pMob );
	          pMob->area = NULL;
	          pMob->vnum = 0;
	          free_mob_index( pMob );
	      }
	   }
	   free_area( pClan->area );
           UNLINK( pClan, first_clan, last_clan, next, prev );
	   DISPOSE ( pClan );
	}
        fclose( fpReserve );
	load_clanlist();
        fpReserve = fopen( NULL_FILE, "r" );
	fBootDb = TRUE;
        log_string("Fixing Exits");
        fix_exits();
        fBootDb = FALSE;
        log_string("Updating Areas");
        area_update(TRUE);

        return;
}

void olc_save_clans( FILE *fp, AREA_DATA *pArea )
{
CLAN_DATA *pClan;
CLAN_MEMBER *member;

for (pClan = first_clan; pClan; pClan = pClan->next)
{
   if ( pClan->area == pArea ) break;
}

   if (!pClan)
   {
      bug( "save_clans: Area without clan: %s : %s", pArea->name, pArea->filename);
      REMOVE_BIT(pArea->area_flags, AREA_CLAN);
      return;
   }

   fprintf( fp, "#CLANDATA\n" );
   fprintf( fp, "Name           %s~\n", pClan->name );
   fprintf( fp, "Type           %d\n", pClan->type );
   fprintf( fp, "Title          %s~\n", pClan->title);
   fprintf( fp, "Info           %s~\n", pClan->info);
   fprintf( fp, "Ext_Sec        %d\n", pClan->ext_sec);
   fprintf( fp, "Int_Sec        %d\n", pClan->int_sec);
   fprintf( fp, "Bank           %2.0f\n", pClan->bank);
   fprintf( fp, "Appfee         %2.0f\n", pClan->app_fee);

   for(member = pClan->first_member; member; member = member->next)
   {
      fprintf( fp, "#MEMBER\n" );
      fprintf( fp, "Name           %s~\n", member->name);
      fprintf( fp, "Title          %s~\n", member->title);
      fprintf( fp, "Bank           %2.0f\n", member->bank);
		fprintf(fp, "Flags %s~\n",
			print_bitvector(&member->flags));
      fprintf( fp, "End\n\n" );
   }
   fprintf( fp, "End\n\n\n\n" );

}


void clan_syntax( CHAR_DATA *ch )
{
CLAN_DATA *pClan;
CLAN_MEMBER *chm;
EXT_BV flags;
int x;
char buf[MSL];
char spacer[10];

if (!ch) return;

xCLEAR_BITS( flags );
chm = NULL;
if ((pClan = get_plr_clan( ch->name )) != NULL)
     if ((chm = get_member(ch->name, pClan)) != NULL)
     log_string("Yer a member");

  sprintf( buf, "%s", IS_IMMORTAL( ch ) ? "<clan> " : "" );
  sprintf( spacer, "%s", IS_IMMORTAL( ch ) ? "" : "       " );

  if ( chm != NULL)
   for( x = 0; x < 32; x++)
     if (xIS_SET(chm->flags, x))
       xSET_BIT(flags, x);

     stc( "clan list                               - lists all clans\n\r", ch);
     stc( "clan show <abbr>                        - shows info about a clan\n\r", ch);
  if (IS_IMMORTAL( ch ))
     stc( "clan save                               - saves all the clans, and reloads them\n\r", ch);
  if (!chm || IS_IMMORTAL( ch ))
     stc( "clan create <abbr>                      - creates a new clan with the specified abbr\n\r", ch);
  if (!chm && !IS_IMMORTAL( ch ))
     stc( "clan apply <abbr>                       - apply to join a clan\n\r", ch);

     if (CAN_CLAN(ch, flags, CL_INDUCT))
     {
	chprintf(ch, "clan accept %s<applicant> %s         - accepts an applying char into your clan\n\r", buf, spacer);
	chprintf(ch, "clan reject %s<applicant> %s         - turns down an applying char from your clan\n\r", buf, spacer);
     }

     if ( CAN_CLAN(ch, flags, CL_BANK))
        chprintf(ch, "clan withdraw %s<amt> %s             - withdraws from the clan bank\n\r", buf, spacer);

     if ( CAN_CLAN(ch, flags, CL_LEADER))
     {
	chprintf(ch, "clan info %s<clan description> %s    - sets the clans description\n\r", buf, spacer );
	chprintf(ch, "clan name %s<name> %s                - sets the clans name\n\r", buf, spacer );
	chprintf(ch, "clan abbr %s<abbreviation> %s        - sets the clans abbr\n\r", buf, spacer );
	chprintf(ch, "clan disband %s  %s                  - permanently disbands the clan\n\r", buf, spacer);
	chprintf(ch, "clan intsec %s<value>  %s            - sets the internal secrecy\n\r", buf, spacer);
	chprintf(ch, "clan extsec %s<value> %s             - sets the external secrecy\n\r", buf, spacer);
	chprintf(ch, "clan setfee %s<cost> %s              - sets the cost to apply\n\r", buf, spacer);
     }

     if ( CAN_CLAN(ch, flags, CL_BESTOW))
        chprintf(ch, "clan bestow %s<member> <title> %s    - sets your members clan-title\n\r", buf, spacer);

     if ( CAN_CLAN(ch, flags, CL_ALLOW))
        chprintf(ch, "clan allow %s<member> <privilege> %s - toggles a clan members ability to use various clan commands\n\r", buf, spacer);

     if ( chm || IS_IMMORTAL( ch ))
        chprintf(ch, "clan deposit %s<amt> %s              - deposits gold into the clan bank\n\r", buf, spacer);
     return;
}

int find_clan_vnum( );
void new_clan_area( CLAN_DATA *clan )
{
AREA_DATA *cArea;
int vnum;
char buf[MSL];

  if (!clan)
  {
     bug("new_clan_area: NULL clan" );
     return;
  }

  if ((vnum = find_clan_vnum()) == -1)
  {
     log_string( "Error new_clan_area, no free vnums." );
     return;
  }

  cArea = new_area();
  cArea->lvnum = vnum;
  cArea->uvnum = (vnum + 49);
  sprintf( buf, "./clans/%s", cArea->filename);
  cArea->filename = strdup( buf );
  SET_BIT( cArea->area_flags, AREA_ADDED );
  SET_BIT( cArea->area_flags, AREA_CLAN );
  clan->area = cArea;
}

int find_clan_vnum( )
{
AREA_DATA *pArea;
int used[100];
int vnum;
int count;

  for (pArea = first_area; pArea; pArea = pArea->next)
  {
     if (IS_SET(pArea->area_flags, AREA_CLAN))
     {
        vnum = ((pArea->lvnum - 53950) / 50);
        used[vnum-1] = 1;        
     }
  }

  for (count = 0; count < 100; count++)
  {
     if ( used[count] < 1 )
     {
        vnum = ((( count+1 ) * 50 ) + 53950 );
        break;
     }
  }

  if ( count == 100 )
     return -1;
  else
     return vnum;
}
