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


/**********************************
 * All kinds of new stuff by Jobo *
 **********************************/

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
#include <unistd.h>
#endif
#include "merc.h"

int	get_ratio	args( ( CHAR_DATA *ch ) );
char *flag_string( const struct flag_type *flag_table, int bits );

void load_kingshit()
{
  FILE *fp;
  int i;

  king_desc_list[0].princes = "";
  king_desc_list[0].members = "";
  king_desc_list[0].reqs = "";
  if ((fp = fopen("../area/kingshit.txt", "r")) == NULL)
  {
    log_string("Error: kingshit.txt not found!");
    exit(1);
  }
  for (i = 1; i <= MAX_KINGDOM; i++)
  {
    king_desc_list[i].princes = fread_string(fp);
    king_desc_list[i].members = fread_string(fp);
    king_desc_list[i].reqs = fread_string(fp);
  }
  fclose(fp);
}

void load_leaderboard()
{
  FILE *fp;

  if ((fp = fopen("../area/leader.txt", "r")) == NULL)
  {   
    log_string("Error: leader.txt not found!");
    return;
//    exit(1);
  }
  leader_board.bestpk_name = fread_string(fp);  
  leader_board.bestpk_number = fread_number(fp);
  leader_board.pk_name = fread_string(fp);
  leader_board.pk_number = fread_number(fp);
  leader_board.pd_name = fread_string(fp);
  leader_board.pd_number = fread_number(fp);
  leader_board.mk_name = fread_string(fp);
  leader_board.mk_number = fread_number(fp);
  leader_board.md_name = fread_string(fp);
  leader_board.md_number = fread_number(fp);
  leader_board.tt_name = fread_string(fp);
  leader_board.tt_number = fread_number(fp);
  leader_board.qc_name = fread_string(fp);
  leader_board.qc_number = fread_number(fp);
  leader_board.aw_name = fread_string(fp);
  leader_board.aw_number = fread_number(fp);
  leader_board.al_name = fread_string(fp);
  leader_board.al_number = fread_number(fp);
  leader_board.primal_name = fread_string(fp);
  leader_board.primal_number = fread_number(fp);
  leader_board.status_name = fread_string(fp);
  leader_board.status_number = fread_number(fp);
  fclose(fp);
}

void save_kingshit()
{
  FILE *fp;
  int i;

  if ((fp = fopen("../area/kingshit.txt","w")) == NULL)
  {
    log_string("Error writing to kingshit.txt");
  }
  for (i = 1; i <= MAX_KINGDOM; i++)
  {
    fprintf(fp, "%s~\n", king_desc_list[i].princes);
    fprintf(fp, "%s~\n", king_desc_list[i].members);
    fprintf(fp, "%s~\n", king_desc_list[i].reqs);
  }
  fclose (fp);
}

void save_leaderboard()
{
  FILE *fp;

  if ((fp = fopen("../area/leader.txt","w")) == NULL)
  {
    log_string("Error writing to leader.txt");
    return;
  }
  fprintf(fp, "%s~\n", leader_board.bestpk_name); 
  fprintf(fp, "%d\n", leader_board.bestpk_number);
  fprintf(fp, "%s~\n", leader_board.pk_name);
  fprintf(fp, "%d\n", leader_board.pk_number);
  fprintf(fp, "%s~\n", leader_board.pd_name);     
  fprintf(fp, "%d\n", leader_board.pd_number);
  fprintf(fp, "%s~\n", leader_board.mk_name);
  fprintf(fp, "%d\n", leader_board.mk_number);
  fprintf(fp, "%s~\n", leader_board.md_name);
  fprintf(fp, "%d\n", leader_board.md_number);
  fprintf(fp, "%s~\n", leader_board.tt_name);
  fprintf(fp, "%d\n", leader_board.tt_number);
  fprintf(fp, "%s~\n", leader_board.qc_name);
  fprintf(fp, "%d\n", leader_board.qc_number);
  fprintf(fp, "%s~\n", leader_board.aw_name);
  fprintf(fp, "%d\n",  leader_board.aw_number);
  fprintf(fp, "%s~\n", leader_board.al_name);
  fprintf(fp, "%d\n",  leader_board.al_number);
  fprintf(fp, "%s~\n", leader_board.primal_name);
  fprintf(fp, "%d\n",  leader_board.primal_number);
  fprintf(fp, "%s~\n", leader_board.status_name);
  fprintf(fp, "%d\n",  leader_board.status_number);
  fclose (fp);
}


void do_showkingdesc( CHAR_DATA *ch, char *argument )
{
  int i;

  if (IS_NPC(ch)) return;

  if (argument[0] == '\0')
  {
    send_to_char("Valid kingdoms are :\n\r",ch);
    for (i = 1; i <= MAX_KINGDOM; i++)
    {
      if (!king_table[i].name) continue;
      if (!str_cmp(king_table[i].name,"<null>")) continue;

      send_to_char(king_table[i].name,ch);
      send_to_char("\n\r",ch);
    }
    return;
  }
  for (i = 1; i <= MAX_KINGDOM; i++)
  {
    if (!str_cmp(king_table[i].name,"<null>")) continue;
    if (str_cmp(king_table[i].name,argument)) continue;
    send_to_char("\n\r#GKingdom#0:#n ",ch);
    send_to_char(king_table[i].who_name,ch);
    send_to_char("\n\r#GKing#0:#n ",ch);
    send_to_char(king_table[i].leader_name,ch);
    send_to_char("\n\r#GPrinces#0:#n\n\r",ch);
    send_to_char(king_desc_list[i].princes,ch);
    send_to_char("\n\r",ch);
    send_to_char("#GMembers#0:#n\n\r",ch);
    send_to_char(king_desc_list[i].members,ch);
    send_to_char("\n\r",ch);
    send_to_char("#GReqs#0:#n\n\r",ch);
    send_to_char(king_desc_list[i].reqs,ch);
    send_to_char("\n\r",ch);
    send_to_char("\n\r",ch);
  }
  return;
}


void do_kingdesc(CHAR_DATA *ch, char *argument )
{
  char arg[MAX_STRING_LENGTH];

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
  if (ch->pcdata->kingdom == 0)
  {
    send_to_char("You are not in a clan.\n\r", ch );
    return;
  }
  if (str_cmp(ch->pcdata->switchname,king_table[ch->pcdata->kingdom].leader_name))
  {
    send_to_char("You are not the leader of your clan.\n\r", ch );
    return;
  }
  if (!str_cmp(arg, "princes"))
  {
    if (argument[0] == '\0')
    {
      send_to_char("You need to submit a description.\n\r",ch);
      return;
    }
    if (has_bad_chars(ch,argument))
    {
      send_to_char("Illegal chars, please retry.\n\r",ch);
      return;
    }
    if (strlen(argument) > 300) // should be plenty
    {
      send_to_char("String to long.\n\r",ch);
      return;
    }
    free_string(king_desc_list[ch->pcdata->kingdom].princes);
    king_desc_list[ch->pcdata->kingdom].princes = str_dup(argument);
  }
  else if (!str_cmp(arg, "members"))
  {
    if (argument[0] == '\0')
    {
      send_to_char("You need to submit a description.\n\r",ch);
      return;
    }
    if (has_bad_chars(ch,argument))
    {
      send_to_char("Illegal chars, please retry.\n\r",ch);
      return;
    }
    if (strlen(argument) > 300) // should be plenty
    {
      send_to_char("String to long.\n\r",ch);
      return;
    }
    free_string(king_desc_list[ch->pcdata->kingdom].members);
    king_desc_list[ch->pcdata->kingdom].members = str_dup(argument);
  }
  else if (!str_cmp(arg, "reqs"))
  {
    if (argument[0] == '\0')
    {
      send_to_char("You need to submit a description.\n\r",ch);
      return;
    }
    if (has_bad_chars(ch,argument))
    {
      send_to_char("Illegal chars, please retry.\n\r",ch);
      return;
    }
    if (strlen(argument) > 300) // should be plenty
    {
      send_to_char("String to long.\n\r",ch);
      return;
    }
    free_string(king_desc_list[ch->pcdata->kingdom].reqs);
    king_desc_list[ch->pcdata->kingdom].reqs = str_dup(argument);
  }
  else
  {
    send_to_char("kingdesc <princes/members/reqs> <desc>\n\r",ch);
    return;
  }
  send_to_char("Ok.\n\r",ch);
  save_kingshit();
  return;
}

void do_leader( CHAR_DATA *ch, char *argument )
{    
  char buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  stc("#r-==--==#L**#r==--==#L**#r==--==#L**#r== #GLEADER BOARD #r==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==#n\n\r\n\r",ch);

  send_to_char("   #oBest PK'er        #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.bestpk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith a #G%d #oPK score#n\n\r",leader_board.bestpk_number);
  send_to_char(buf,ch);

  send_to_char("   #oMost Deadly       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.pk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #opkills#n\n\r",leader_board.pk_number);
  send_to_char(buf,ch);

  send_to_char("   #oMost Pathetic     #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.pd_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #opdeaths#n\n\r",leader_board.pd_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oBest Slayer       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.mk_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #omkills#n\n\r",leader_board.mk_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oMost Wimpy        #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.md_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #omdeaths#n\n\r",leader_board.md_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oHas No life       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.tt_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oHours played#n\n\r",leader_board.tt_number);
  send_to_char(buf,ch);
  
  send_to_char("   #oBest Questor      #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.qc_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oQuests completed#n\n\r",leader_board.qc_number);
  send_to_char(buf,ch);

  send_to_char("   #oBest Arena        #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.aw_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oArena Wins#n\n\r",leader_board.aw_number);
  send_to_char(buf,ch);

  send_to_char("   #oWorst Arena       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.al_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oArena Losses#n\n\r",leader_board.al_number);
  send_to_char(buf,ch);

  send_to_char("   #oMost Primal       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.primal_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oPrimal Energy#n\n\r",leader_board.primal_number);
  send_to_char(buf,ch);

  send_to_char("   #oMost Wanted       #C--->    ",ch);
  sprintf(buf, "#G%-13s",leader_board.status_name);
  send_to_char(buf,ch);
  sprintf(buf, " #owith #G%d #oStatus#n\n\r",leader_board.status_number);
  send_to_char(buf,ch);

  stc("\n\r#r-==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==--==#L**#r==#n\n\r",ch);
  return;
}

void check_leaderboard( CHAR_DATA *ch, char *argument )
{
  bool changed = FALSE;

  if (IS_NPC(ch)) return;
  if (ch->level > 6) return;
  if (ch->pkill - ch->pdeath > 0)
  {
  if ((ch->pkill - ch->pdeath) > leader_board.bestpk_number)
  {
    leader_board.bestpk_number = (ch->pkill - ch->pdeath);
    free_string(leader_board.bestpk_name);
    leader_board.bestpk_name = str_dup(ch->name);
    changed = TRUE;
  }
  }
  if (ch->mdeath > leader_board.md_number)
  {
    leader_board.md_number = ch->mdeath;
    free_string(leader_board.md_name);
    leader_board.md_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->mkill > leader_board.mk_number)
  {
    leader_board.mk_number = ch->mkill;
    free_string(leader_board.mk_name);
    leader_board.mk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pkill > leader_board.pk_number)
  {
    leader_board.pk_number = ch->pkill;
    free_string(leader_board.pk_name);
    leader_board.pk_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pdeath > leader_board.pd_number)
  {
    leader_board.pd_number = ch->pdeath;
    free_string(leader_board.pd_name);
    leader_board.pd_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pcdata->questsrun > leader_board.qc_number)
  {
    leader_board.qc_number = ch->pcdata->questsrun;
    free_string(leader_board.qc_name);
    leader_board.qc_name = str_dup(ch->name);
    changed = TRUE;
  }
  if ((get_age(ch)-17)*2 > leader_board.tt_number)
  {
    leader_board.tt_number = (get_age(ch)-17)*2;
    free_string(leader_board.tt_name);
    leader_board.tt_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pcdata->awins > leader_board.aw_number)
  {
    leader_board.aw_number = ch->pcdata->awins;
    free_string(leader_board.aw_name);
    leader_board.aw_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->pcdata->alosses > leader_board.al_number)
  {
    leader_board.al_number = ch->pcdata->alosses;
    free_string(leader_board.al_name);
    leader_board.al_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->practice > leader_board.primal_number)
  {
    leader_board.primal_number = ch->practice;
    free_string(leader_board.primal_name);
    leader_board.primal_name = str_dup(ch->name);
    changed = TRUE;
  }
  if (ch->race > leader_board.status_number)
  {
    leader_board.status_number = ch->race;
    free_string(leader_board.status_name);
    leader_board.status_name = str_dup(ch->name);
    changed = TRUE;
  }

  if (changed) save_leaderboard();
  return;
}

int get_ratio(CHAR_DATA *ch)
{
  int ratio;

  if (IS_NPC(ch)) return 0;
  if (ch->pkill == 0) ratio = 0;
  else ratio = 100 * (ch->pkill * ch->pkill)/((ch->pkill + ch->pdeath) * (ch->pkill + ch->pdeath));
  return ratio;
}

void do_leaderclear(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (ch->level < 7) return;
  free_string(leader_board.bestpk_name);
  leader_board.bestpk_name = str_dup("Noone");
  leader_board.bestpk_number = 0;
  free_string(leader_board.pk_name);
  leader_board.pk_name = str_dup("Noone");
  leader_board.pk_number = 0;
  free_string(leader_board.pd_name);
  leader_board.pd_name = str_dup("Noone");
  leader_board.pd_number = 0;
  free_string(leader_board.md_name);
  leader_board.md_name = str_dup("Noone");
  leader_board.md_number = 0;
  free_string(leader_board.mk_name);
  leader_board.mk_name = str_dup("Noone");
  leader_board.mk_number = 0;
  free_string(leader_board.qc_name);
  leader_board.qc_name = str_dup("Noone");
  leader_board.qc_number = 0;
  free_string(leader_board.tt_name);
  leader_board.tt_name = str_dup("Noone");
  leader_board.tt_number = 0;
  free_string(leader_board.aw_name);
  leader_board.aw_name = str_dup("Noone");
  leader_board.aw_number = 0;
  free_string(leader_board.al_name);
  leader_board.al_name = str_dup("Noone");
  leader_board.al_number = 0;
  free_string(leader_board.primal_name);
  leader_board.primal_name = str_dup("Noone");
  leader_board.primal_number = 0;
  free_string(leader_board.status_name);
  leader_board.status_name = str_dup("Noone");
  leader_board.status_number = 0;
   
  save_leaderboard();
  send_to_char("Leader board cleared.\n\r",ch);
  return;
}
