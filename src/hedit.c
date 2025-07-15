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
#include <unistd.h>
#include <stdarg.h>
#include <assert.h>
#include "merc.h"
#include "garou.h"
#include "olc.h"

#define HEDIT( fun )        bool fun( CHAR_DATA *ch, char *argument )
void show_olc_cmds( CHAR_DATA *ch, const struct olc_cmd_type *olc_table );

void do_hedit(CHAR_DATA * ch, char *argument){
HELP_DATA *tHelp;
char arg1[MAX_INPUT_LENGTH + 20];  
argument = one_argument(argument, arg1);  

    switch( ch->substate )
    {
	default:
	  break;

	case SUB_HELP_EDIT:
	  if ( !ch->dest_buf )
	  {
		send_to_char( "Fatal error: report to Snard.\n\r", ch );
		bug( "do_hedit: sub_help_edit: NULL ch->dest_buf", 0 );
		ch->substate = SUB_NONE;
		return;
	  }

	  tHelp = ch->dest_buf;
	  tHelp->text = copy_buffer(ch);
	  stop_editing(ch);
	  ch->substate = SUB_NONE;
	  return;
     }

if (ch->level < (MAX_LEVEL - 5))  {
send_to_char("huh?\n\r", ch);
return;
}
if (!str_cmp(arg1, "create"))  {
if (hedit_create(ch, argument))
ch->desc->editor = ED_HELP;
return;
}

else if (!str_cmp(arg1, "?"))  {
stc("Doesn't do anything at the moment.\n\r", ch);
return;
}

else if (!str_cmp(arg1, "change"))  {
if (hedit_change(ch, argument))
ch->desc->editor = ED_HELP;
return;
}
else if (!str_cmp(arg1, "index"))  {
if (hedit_index(ch, argument))
ch->desc->editor = ED_HELP;
return;
}
else if (!str_cmp(arg1, "delete"))  {
if (hedit_delete(ch, argument))
ch->desc->editor = ED_HELP;
return;
}
/*
* This is needed :)
* Whoever wrote this version of HEDIT was drunk, I swear.
*   
* -Fand  
*/
else if ( !str_cmp(arg1, "edit"))  {
HELP_DATA *tHelp, *pHelp;
if ( argument[0] == '\0' )   { 
stc("You didn't specify a keyword to edit. Use 'index' to see a listing.\n\r",ch);
return;
}
for (tHelp = first_help; tHelp != NULL; tHelp = tHelp->next)   {
if (is_name(argument, tHelp->keyword))
{
ch->desc->editor = ED_HELP;
ch->desc->pEdit = (void *) tHelp;   
EDIT_HELP(ch, pHelp);    
sprintf(log_buf, "Okay, entering HEDIT mode for: %s.\n\r",tHelp->keyword);      
stc(log_buf,ch);     
show_olc_cmds(ch, hedit_table);  
return;  
}  
}  
/* if we've made it here, we didn't find the keyword. */  
stc("Could not find the keyword. Use 'index' to see the listing.\n\r",ch); 
} 
else {  
send_to_char("Hedit mode NOT entered, #7no default command or help to edit#n..\n\r", ch);  
show_olc_cmds(ch, hedit_table);
return; 
} 
return;
}


HELP_DATA *new_help(void)
{ 
HELP_DATA *pHelp;
pHelp = calloc(1, sizeof(*pHelp));
pHelp->level = 0;
return pHelp;
}
/*help editor by blade, mods by fand */

HEDIT(hedit_level)
{
  HELP_DATA *pHelp;
  EDIT_HELP(ch, pHelp);
  if ( !pHelp )  {
      stc("You need to use hedit create first.\n\r",ch);
      return FALSE;
  }
  if (argument[0] == '\0' || !is_number(argument))  {
    send_to_char("Syntax:  level [number]\n\r", ch);
    return FALSE;
  }
  pHelp->level = atoi(argument);
  send_to_char("Level set.\n\r", ch);
  return TRUE;
}

/* reset keyword */
HEDIT(hedit_keyword)
{
  HELP_DATA *pHelp;
  HELP_DATA *tHelp;
  EDIT_HELP(ch, pHelp);
  if ( argument[0] == '\0' ) return FALSE;
  if ( !pHelp )  {
      stc("You need to use hedit create first.\n\r",ch);
      return FALSE;
  }
  if (!is_number(argument))  {
    for (tHelp = first_help;
 tHelp != NULL;
 tHelp = tHelp->next)    {
      if (is_name(argument, tHelp->keyword))      {
        send_to_char("That keyword already exits.\n\r", ch);
        return FALSE;
      }
    }
   if ( !pHelp ) return FALSE;
   if ( pHelp->keyword != NULL )	free_string(pHelp->keyword);
    pHelp->keyword = str_dup(argument);
    return TRUE;
  }
  send_to_char("Syntax: keyword [word(s)]\n\r", ch);
  return FALSE;
}
/* mod current help text */
HEDIT(hedit_text)
{
  HELP_DATA *pHelp;
  EDIT_HELP(ch, pHelp);
  if (argument[0] == '\0')  {
    if ( !pHelp )    {
      stc("You need to use hedit create first.\n\r",ch);
      return FALSE;
    }
    string_append(ch, &pHelp->text);
    return TRUE;
  }
  else  {
    send_to_char("Syntax: text [no arguments!]\n\r", ch);
    return FALSE;
  }
}
/* kill a help */
HEDIT(hedit_delete)
{
  extern int top_help;
  HELP_DATA *target;
  HELP_DATA *previous = NULL;
  if (argument[0] == '\0')  {
    send_to_char("Syntax: Hedit delete 'keyword'\n\r", ch);
    return FALSE;
  }
  for (target = first_help;
 target != NULL;
 target = target->next)  {
    if (is_name(argument, target->keyword))    {
      /*       
* Okay, why don't we actually *erase* the contents of the help? Sound good? Yes.       
* While we're at it, we might as well free the memory. mmmHmmmm..       
*       
* -Fand      
*/
        UNLINK(target, first_help, last_help, next, prev);
        DISPOSE(target->keyword);
        DISPOSE(target->text);
        DISPOSE(target);
        top_help--;
        send_to_char("Help removed.\n\r", ch);
        do_asave(ch, "helps");
        edit_done(ch);
        return TRUE;
    }
    previous = target;
          /* set previous to last target */
  }
  send_to_char("No Help with that keyword found to delete!\n\r", ch);
  return FALSE;
}
/* mod an existing help - throw into text editor */
HEDIT(hedit_change){
  HELP_DATA *tHelp;
  if (argument[0] == '\0')  {
    send_to_char(" Syntax: Hedit change 'keyword'\n\r", ch);
    return FALSE;
  }
  for (tHelp = first_help;
 tHelp != NULL;
 tHelp = tHelp->next)  {
    if (is_name(argument, tHelp->keyword))    {
      send_to_char(" Help found, Entering String editor\n\r", ch);
      ch->desc->pEdit = (void *) tHelp;
      send_to_char(tHelp->keyword, ch);
      send_to_char("\n\r", ch);
      ch->substate = SUB_HELP_EDIT;
      ch->dest_buf = tHelp;
	start_editing(ch, tHelp->text);
//      string_append(ch, &tHelp->text);
      return TRUE;
    }
  }
  send_to_char("No Help by that keyword available,\n\r  Try 'hedit index' for a listing of current helps.\n\r Remember, keywords must\n\rmatch exactly!\n\r", ch);
  return FALSE;
}
/* list all helps by keyword */
HEDIT(hedit_index){
  HELP_DATA *pHelp;
  int level;
  char output[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int bob;
  buf[0] = '\0';
  sprintf(output, "Help By Index.\n\r");
  for (pHelp = first_help;
 pHelp;
 pHelp = pHelp->next)  {
    level = (pHelp->level < 0) ? -1 * pHelp->level - 1 : pHelp->level;
    if (level > get_trust(ch))      continue;
    sprintf(buf, "%-17.16s  ", pHelp->keyword);
    strcat(output, buf);
    if (++bob % 3 == 0)         /* column check */
      strcat(output, "\n\r");
  }
  send_to_char(output, ch);
  send_to_char("\n\r", ch);
  return TRUE;
}
/* create a new help */
HEDIT(hedit_create){
  extern int top_help;
  HELP_DATA *pHelp;
  HELP_DATA *tHelp;
  if (argument[0] == '\0')  {
    send_to_char("Syntax: Hedit create 'keyword'\n\r", ch);
    return FALSE;
  }
  for (tHelp = first_help;
 tHelp != NULL;
 tHelp = tHelp->next)  {
    if (is_name(argument, tHelp->keyword))    {
      send_to_char("That Help already exits.\n\r", ch);
      return FALSE;
    }
  }
  /* make the help in memory and initialize */
  pHelp = new_help();
  pHelp->level = 0;
  pHelp->keyword = str_dup(argument);
  pHelp->text = str_dup("");
  pHelp->next = first_help;
  first_help = pHelp;
  top_help++;
  ch->desc->pEdit = (void *) pHelp;
  send_to_char(pHelp->keyword, ch);
  send_to_char("\n\r", ch);
  ch->substate = SUB_HELP_EDIT;
  ch->dest_buf = pHelp;
  start_editing(ch, pHelp->text);
//  string_append(ch, &pHelp->text);
  return TRUE;
}

HEDIT(hedit_edit)
{
   HELP_DATA *tHelp, *pHelp;
 
   if ( argument[0] == '\0' )
   {
      stc("You didn't specify a keyword to edit. Use 'index' to see a listing.\n\r",ch);
      return FALSE;
   }
 
   for (tHelp = first_help; tHelp != NULL; tHelp = tHelp->next)
  {
      if (is_name(argument, tHelp->keyword))
      {
        ch->desc->editor = ED_HELP;
        ch->desc->pEdit = (void *) tHelp;
        EDIT_HELP(ch, pHelp);
  
        sprintf(log_buf, "Okay, entering HEDIT mode for: %s.\n\r",
        tHelp->keyword);
        stc(log_buf,ch);
        show_olc_cmds(ch, hedit_table);
        return FALSE;
      }
   }
   /* if we've made it here, we didn't find the keyword. */
   stc("Could not find the keyword. Use 'index' to see the listing.\n\r",ch);
   
   return FALSE;
 
}
