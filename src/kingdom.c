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


#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"

void do_savecmd( CHAR_DATA *ch, char *argument)
{
  load_kingdoms();
  save_kingdoms();
}


void load_war(void)
{
    FILE *fp;
    int i;

	if ((fp = fopen("../area/war.dat", "r")) == NULL) {
	log_string("Error: war.dat not found!");
	exit(1);}

    for(i=0; i < MAX_WAR; i++ ){
	war_table[i].one = fread_number(fp);
	war_table[i].two = fread_number(fp);
    }
    return;
}

/*void save_war(void)
{
    FILE *fp;
    int i;

	if ((fp = fopen("../area/war.dat", "w")) == NULL) {
	log_string("Error: kingdom.dat not found!");
	exit(1);}
                
        for (i = 0; i < MAX_WAR; i++) {
        fprintf(fp,"%d\n", war_table[i].one);
	fprintf(fp,"%d\n", war_table[i].two);
        }
        
        fclose(fp);
        return;
}*/

void load_kingdoms(void) {

	FILE *fp;
/*	ROOM_INDEX_DATA *room;
	OBJ_INDEX_DATA *obj;*/
	int i;
	int vnum;

	king_table[0].name = "";
	king_table[0].who_name = "";
	king_table[0].leader_name = "";
	king_table[0].pk = 0;
	king_table[0].pd = 0;
	king_table[0].num = 0;
	king_table[0].recall = 0;
	king_table[0].object = NULL;
//	king_table[0].altar = 0;

	if ((fp = fopen("../area/kingdom.dat", "r")) == NULL) {
	log_string("Error: kingdom.dat not found!");
	exit(1);}

	for (i = 1; i <= MAX_KINGDOM; i++) {

	vnum = 90000 + i*100;

	king_table[i].name = fread_string(fp);
	king_table[i].who_name = fread_string(fp);
	king_table[i].leader_name = fread_string(fp);
	king_table[i].pk = fread_number(fp);
	king_table[i].pd = fread_number(fp);
	king_table[i].recall = fread_number(fp);
	king_table[i].altar = fread_number(fp);
//	king_table[i].num = i;
/*	obj = get_obj_index(vnum);
	if (obj != NULL)
	king_table[i].object =  create_object(obj,0);
	if (king_table[i].object != NULL && (room = get_room_index(king_table[i].altar)) != NULL)
		obj_to_room(king_table[i].object,room);
*/	}	

	fclose(fp);
	return;
}


void save_kingdoms(void) {

	FILE *fp;
	int i;

	if ((fp = fopen("../area/kingdom.dat","w")) == NULL) {
	log_string("Error writing to kingdom.dat");
	}

	for (i = 1; i <= MAX_KINGDOM; i++) {
	fprintf(fp,"%s~\n", king_table[i].name);
	fprintf(fp,"%s~\n", king_table[i].who_name);
	fprintf(fp,"%s~\n", king_table[i].leader_name);
	fprintf(fp, "%d\n%d\n",king_table[i].pk, king_table[i].pd);
	fprintf(fp, "%d\n%d\n",king_table[i].recall,king_table[i].altar);

	}

	fclose(fp);
	return;
}

void do_clanlist(CHAR_DATA *ch, char *argument) {

	char buf[MAX_STRING_LENGTH];
        char pkratio[MAX_STRING_LENGTH];
	int i;

	if (IS_NPC(ch)) return;

	sprintf(buf, "#RClan               Leader       PKs     PDs     Ratio#n\n\r" );
	send_to_char(buf,ch);
	sprintf(buf, "#0----               ------       ---     ---     -----#n\n\r" );
	send_to_char(buf,ch);
	for (i = 1; i <= MAX_KINGDOM; i++) {
          if (!str_cmp(king_table[i].name,"<null>")) continue;
          if (king_table[i].pk > 0)
          {
            if (100*king_table[i].pk/(king_table[i].pk + king_table[i].pd) < 100)
              sprintf(pkratio,"0.%-2d", (100*king_table[i].pk/(king_table[i].pk + king_table[i].pd)));
            else
              sprintf(pkratio,"1.00");
          }
          else sprintf(pkratio,"0.00");  
	  sprintf(buf, "#G%-18s %-12s %-7d %-7d %-4s#n\n\r", 
  	  king_table[i].name,
  	  king_table[i].leader_name,
  	  king_table[i].pk,
  	  king_table[i].pd,
          pkratio);
	  send_to_char(buf,ch);
        }
	send_to_char("\n\r", ch );

	return;
}

/* void do_induct(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;
	char arg[MAX_STRING_LENGTH];

	argument = one_argument(argument,arg);

	if (IS_NPC(ch)) return;

	if (ch->pcdata->kingdom == 0) {
	send_to_char("You are not in a clan.\n\r", ch );
	return;}

	if (str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader_name)
	&& !IS_SET(ch->special, SPC_PRINCE)) {
	send_to_char("You cannot induct people.\n\r", ch );
	return;}

	if ((victim = get_char_room(ch, arg)) == NULL) {
	send_to_char("They are not here.\n\r", ch );
	return;}

	if (IS_NPC(victim)) {
	send_to_char("Not on NPCs.\n\r", ch );
	return;}

	if (!IS_SET(victim->newbits, NEW_CLANNABLE)) {
	send_to_char("That person is not CLANNABLE.\n\r", ch );
	return;}

	if (victim->pcdata->kingdom != 0) {
	send_to_char("That person is already clanned.\n\r", ch );
	return;}

	victim->pcdata->kingdom = ch->pcdata->kingdom;
	if (IS_SET(victim->special, SPC_PRINCE)) 
	REMOVE_BIT(victim->special, SPC_PRINCE);
	send_to_char("They are now in your clan.\n\r", ch);
	send_to_char("You are now in a clan.\n\r", victim);
	save_char_obj(victim);
	return;
}

void do_outcast(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;
	char arg[MAX_STRING_LENGTH];
	argument = one_argument(argument, arg);

	if (IS_NPC(ch)) return;

	if (ch->pcdata->kingdom == 0) {
	send_to_char("You are not in a clan.\n\r", ch );
	return;}

	if (str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader_name)) {
	send_to_char("You are not a clan leader.\n\r", ch );
	return;}

	if ((victim = get_char_world(ch, arg)) == NULL ) {
	send_to_char("They are not here.\n\r", ch );
	return;}

	if (IS_NPC(victim)) {
	send_to_char("Not on NPCs.\n\r", ch );
	return;}

	if (victim->pcdata->kingdom != ch->pcdata->kingdom) {
	send_to_char("They are not in your clan.\n\r", ch );
	return;}

	if (ch == victim) {
	send_to_char("You can't outcast yourself.\n\r", ch );
	return;}

	victim->pcdata->kingdom = 0;
	if (IS_SET(victim->special, SPC_PRINCE))
	REMOVE_BIT(victim->special, SPC_PRINCE);
	send_to_char("You remove them from your clan.\n\r", ch );
	send_to_char("You have been outcasted from your clan!\n\r",victim);	

	return;
}
*/
void do_prince(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;
	char arg[MAX_STRING_LENGTH];

	argument = one_argument(argument, arg);

	if (IS_NPC(ch)) return;

	if (ch->pcdata->kingdom == 0) {
	send_to_char("You are not in a clan.\n\r", ch );
	return;}

	if (str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader_name)) {
	send_to_char("You are not the leader of your clan.\n\r", ch );
	return;}

	if ((victim = get_char_room(ch, arg)) == NULL) {
	send_to_char("They are not here.\n\r", ch );
	return;}

	if (IS_NPC(victim)) {
	send_to_char("Not on NPCs.\n\r", ch );
	return;}

	if (victim->pcdata->kingdom != ch->pcdata->kingdom) {
	send_to_char("They are not in your clan.\n\r", ch );
	return;}

	if (ch == victim) {
	send_to_char("You can't do that.\n\r", ch );
	return;}
	if (victim->race < 10)
        {
	send_to_char("To become a prince, they must be stronger.\n\r",ch);
        return;
        }
        if (IS_SET(victim->special, SPC_GENERAL))
        {
         send_to_char("They are already a general!\n\r",ch);
         return;
        }

	if (IS_SET(victim->special, SPC_PRINCE)) {
	REMOVE_BIT(victim->special, SPC_PRINCE);
	send_to_char("You are no longer a prince.\n\r", victim);
	send_to_char("They are no longer a prince.\n\r", ch );
	save_char_obj(victim);
	return;}

	SET_BIT(victim->special, SPC_PRINCE);
	send_to_char("You are now a prince!\n\r", victim);
	send_to_char("You make them a prince.\n\r", ch );
	save_char_obj(victim);
	return;
}

void do_clannable(CHAR_DATA *ch, char *argument) {

	if (IS_NPC(ch)) return;

	if (ch->pcdata->kingdom != 0) {
	send_to_char("You already are in a clan.\n\r", ch );
	return;}

	if (IS_SET(ch->newbits, NEW_CLANNABLE)) {
	REMOVE_BIT(ch->newbits, NEW_CLANNABLE);
	send_to_char("You can no longer be clanned.\n\r", ch );
	return;}

	SET_BIT(ch->newbits, NEW_CLANNABLE);
	send_to_char("You can now be clanned.\n\r", ch );

	return;
}

void do_kingdom(CHAR_DATA *ch, char *argument)
{

    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
   char rankname[9];

    if (IS_NPC(ch)) return;

	if (ch->pcdata->kingdom == 0) {
	send_to_char("You are not in a kingdom.\n\r", ch );
	return;}

	sprintf(buf, "%s's Kingdom of %s:\n\r",
	king_table[ch->pcdata->kingdom].leader_name,
	king_table[ch->pcdata->kingdom].name);

    send_to_char( buf, ch );
    send_to_char("#2[    Name    ] [ Hits ] [ HPmax ] [ Mana ] [ Move ] [ Status ] [  Rank  ]#n\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( gch->pcdata->kingdom != ch->pcdata->kingdom) 
	    continue;
	if (!str_cmp(king_table[ch->pcdata->kingdom].leader_name,gch->pcdata->switchname)) 
	sprintf(rankname,"King");
	else if (IS_SET(gch->special, SPC_PRINCE)) 
	sprintf(rankname, "Prince");
	else
	 sprintf(rankname,"Member");
	    sprintf( buf,
	    "#2[#n%-12s#2] [#n%6d#2] [#n%7d#2] [#n%6d#2] [#n%6d#2] [#n   %-2d   #2] [ #n%-6s #2]#n\n\r",
		gch->pcdata->switchname ,
		gch->hit,gch->max_hit,gch->mana,gch->move,
		gch->race, rankname);
		send_to_char( buf, ch );
    }
    return;
}

/* new kingdom commands by Jobo */
void do_kwhere(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  DESCRIPTOR_DATA *d;
  bool found = FALSE;
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  if (ch->pcdata->kingdom == 0)
  {
    send_to_char("You are not in a kingdom.\n\r", ch );
    return;
  }
  send_to_char("Players in your kingdom:\n\r", ch);
  for (d = first_descriptor; d != NULL; d = d->next)
  {
    if ((d->connected == CON_PLAYING || d->connected == CON_EDITING)
       && ((victim = d->character) != NULL) && victim->in_room != NULL && victim->pcdata->chobj == NULL
       && victim->pcdata->kingdom == ch->pcdata->kingdom)
    {
      found = TRUE;
      sprintf(buf, "%-28s %s\n\r", victim->name, victim->in_room->name);
      send_to_char(buf, ch);
    }
  }
  if (!found) send_to_char( "None\n\r", ch);
  return;
}

void do_krecall(CHAR_DATA *ch, char *argument) {

	CHAR_DATA *victim;
	CHAR_DATA *mount;
	ROOM_INDEX_DATA *location;

	if (IS_NPC(ch)) return;

	if (ch->pcdata->kingdom == 0) {
	send_to_char("Huh?\n\r", ch );
	return;}

	act("$n's body flickers with green energy.",ch,NULL,NULL,TO_ROOM, FALSE);
	act("Your body flickers with green energy.",ch,NULL,NULL,TO_CHAR, FALSE);

        if (ch->fight_timer>0)
	{
	send_to_char("Not with a fighttimer.\n\r",ch);
	return;
	}

	if ((location = get_room_index(king_table[ch->pcdata->kingdom].recall)) == NULL) {
	send_to_char("You are completely lost.\n\r", ch );
	return;}

	if (ch->in_room == location) return;

	if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) || IS_AFFECTED(ch, AFF_CURSE)) {
	send_to_char("You are unable to recall.\n\r", ch );
	return;}

	if ((victim = ch->fighting) != NULL) {
	if (number_bits(1) == 0) {
		WAIT_STATE(ch,4);
		send_to_char("You failed!\n\r", ch );
		return;}
	send_to_char("You recall from combat!\n\r", ch );
	stop_fighting(ch,TRUE);
	}

	act("$n dissapears.",ch,NULL,NULL,TO_ROOM, FALSE);
	char_from_room(ch);
	char_to_room(ch,location);
	act("$n appears in the room.",ch,NULL,NULL,TO_ROOM, FALSE);
	do_look(ch,"auto");
	if ((mount = ch->mount) == NULL) return;
	char_from_room(mount);
	char_to_room(mount,ch->in_room);
	
	return;
}


void do_warlist(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    int i;

    stc( "#2THE WARLIST!!!#n\n\r\n\r",ch);

    for(i=0 ; i < MAX_WAR ; i++){
	if( war_table[i].one != 0
	&& war_table[i].one <= MAX_KINGDOM
	&& war_table[i].two <= MAX_KINGDOM)
	{
	sprintf(buf, "%s's Kingdom of the %s is at war with\n\r%s's Kingdom of the %s\n\r",
	king_table[war_table[i].one].leader_name,
        king_table[war_table[i].one].name,
	king_table[war_table[i].two].leader_name,
        king_table[war_table[i].two].name );
	stc(buf,ch);
	}
    }
    return;
}

void do_decwar(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    int i;
    int target=0, slot = -1;

    one_argument(argument, arg);

	if (ch->pcdata->kingdom == 0) {
	send_to_char("You are not in a clan.\n\r", ch );
	return;}

	if (str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader_name)) {
	send_to_char("You are not the leader of your clan.\n\r", ch );
	return;}

    if( arg[0] == '\0' )
    {
	stc( "You need to type the name of the leader of the kingdom to declare war on.\n\r",ch);
	return;
    }

    for( i = 1 ; i <= MAX_KINGDOM ; i++ ){
	if( !str_prefix(arg,king_table[i].leader_name) ) target = i;
    }

    if( target == 0 )
    {
	stc( "No match found.\n\r",ch);
	return;
    }

    for( i=0 ; i <= MAX_WAR ; i++ ){
	if( (war_table[i].one == ch->pcdata->kingdom && war_table[i].two == target)
	|| (war_table[i].two == ch->pcdata->kingdom && war_table[i].one == target )){
	    stc( "You are already at war with them. Use warpeace to have a peace.\n\r",ch);
	    return;
        }
	if(war_table[i].one == 0 && war_table[i].two == 0 && slot < 0 ) slot = i;
    }

    if( slot < 0 ){
	stc( "No slots left.\n\r",ch);
	return;
    }

    war_table[slot].one = ch->pcdata->kingdom;
    war_table[slot].two = target;
    stc( "You are now at WAR!!!!\n\r",ch);
//	save_war();
    do_info( ch, "THERE IS A NEW WAR!!!!! WARLIST TO SEE!!!!");
    return;
}

void do_warpeace( CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    int i;
    int target=0, slot = -1;

    one_argument(argument, arg);

	if (ch->pcdata->kingdom == 0) {
	send_to_char("You are not in a clan.\n\r", ch );
	return;}

	if (str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader_name)) {
	send_to_char("You are not the leader of your clan.\n\r", ch );
	return;}

    if( arg[0] == '\0' )
    {
	stc( "You need to type the name of the leader of the kingdom to make peace with.\n\r",ch);
	return;
    }

    for( i = 1 ; i <= MAX_KINGDOM ; i++ ){
	if( !str_prefix(arg,king_table[i].leader_name) ) target = i;
    }

    if( target == 0 )
    {
	stc( "No match found.\n\r",ch);
	return;
    }

    for( i=0 ; i <= MAX_WAR ; i++ ){
	if( (war_table[i].one == ch->pcdata->kingdom && war_table[i].two == target) ){
	slot = i;
        }
    }

    if( slot < 0 ){
	stc( "You are not at war with them, or they declared war on you. Get the other leader to make peace.\n\r",ch);
	return;
    }

    war_table[slot].one = 0;
    war_table[slot].two = 0;
    stc( "You are now at PEACE!!!!\n\r",ch);
    do_info(ch,"THERE IS A NEW PEACE BETWEEN KINGDOMS!!");
//	save_war();

    return;
}
