/***************************************************************************
#                                                                          
#
#  jason.c and all contents are sole property of Jason White, who can do   
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
#include <sys/quota.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> /* unlink() */
#include <stdarg.h>
#include <assert.h>
#include <pwd.h>
#include <grp.h>
#include <mntent.h>
#include "merc.h"
#include "garou.h"
#include "olc.h"
struct log_list loglist[999];
int num_of_logs;
extern int fBootDb;
extern int port;

void cleanup_arena();
int gid2group(gid_t id, char *buf);
int uid2user(uid_t id, char *buf);
int id2name(int id, int qtype, char *buf);
uid_t user2uid(char *name);
void sstrncpy(char *d, const char *s, int len);
bool check_speedwalk( CHAR_DATA *ch, char *argument);

int maxHint;
struct hint_type *hint_table;          /* hint table */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )    if ( !str_cmp( word, literal ) ){field  = value;fMatch = TRUE;break;}

#define FKEY( literal, field, value )    if ( !str_cmp( word, literal ) ){strcpy( field, value);fMatch = TRUE;break;}
#define MAXMNTPOINTS 256

#define CORRECT_FSTYPE(type) \
((!strcmp(type, MNTTYPE_EXT2)) || \
(!strcmp(type, MNTTYPE_EXT3)) || \
(!strcmp(type, MNTTYPE_MINIX)) || \
(!strcmp(type, MNTTYPE_UFS)) || \
(!strcmp(type, MNTTYPE_UDF)) || \
(!strcmp(type, MNTTYPE_REISER)) || \
(!strcmp(type, MNTTYPE_XFS)) || \
(!strcmp(type, MNTTYPE_NFS)))
CHAR_DATA *supersnard;
void load_logs()
{
  FILE *fp;
  char *i;

  fp=fopen(LOG_FILE,"r");
  if(!fp)
  {
    bug("NO LOG_FILE",0);
    return;
  }
  
  for(;;)
  {
    i=fread_string(fp);
    if (!str_cmp (i, "NULL")) break;
  
  }


  fclose(fp);
}

void save_loglist()
{
  FILE *fp;
  int f= 0;

  if((fp=fopen(LOG_FILE,"w"))==NULL)
  {
    bug("Cannot open Log file for writing",0);
    return;
  }

  if (num_of_logs >= 0)
     for(f = 0;f <= num_of_logs;f++)
     {
        fprintf(fp,"%s~\n",loglist[f].cmd );
     }

  fprintf(fp,"NULL~\n");
  fclose(fp);

}

void read_loglist()
{
  FILE *fp;
  int j;
  char *cmd;

  if((fp=fopen(LOG_FILE,"r"))==NULL)
  {
    bug("Cannot open Loglist file for reading",0);
    return;
  }

	num_of_logs = -1;

  for(j=0;;j++)
  {
      cmd = fread_string(fp);
      if(!str_cmp(cmd,"NULL"))
      {
	break;  
      }

      strcpy (loglist[j].cmd, cmd);
      num_of_logs = j;

   }  
  fclose(fp);

return;
 }



void load_jason()
{
read_loglist();
sort_commands();
}


void do_logcmd(CHAR_DATA *ch, char *argument)
{
  char buf2[MSL];
  int i;
  int f;
  int x;
  int sex;
  int cmd;

   if (argument[0] == '\0')
    {

        stc("Commands that are logged\n\r", ch);
	stc("#g-----------------------------#n\n\r\n\r", ch);
      if (num_of_logs >= 0)
      {
	for(x=0; x <= num_of_logs; x++)
	{
	  sprintf(buf2, "%s\n\r", loglist[x].cmd);
	  stc(buf2, ch);


	}
        sprintf( buf2, "%d logged commands\n\r", num_of_logs+1);
         stc(buf2, ch);
       }
       else
	stc("\n\rNone...yet.\n\r", ch);
	return;
     }

   if ((argument[0] != '+') && (argument[0] != '-'))
   {
     stc("syntax: Log +/-command (no space)\n\r", ch);
     return;
   }

    if(argument[0] == '-')
    {
       argument++;
       for(i= 0;i <= num_of_logs;i++)
       {
          if(!str_cmp( loglist[i].cmd,argument))
          {
	     sprintf(buf2, "%s command unlogged\n\r", loglist[i].cmd);
             stc(buf2, ch); 

             for(f=i;f <= num_of_logs;f++)
             {
                loglist[f] = loglist[f+1];
	     }
             num_of_logs--;
	     save_loglist();
	     return;
          }
       }

       stc("No such command logged.\n\r", ch);
       return; 

    }

    else if(argument[0] == '+')

    {
      argument++;
      for(sex = 0; sex <= num_of_logs; sex++)
      {
        if(!str_cmp(loglist[sex].cmd, argument))
	{
	  stc("That command is already logged..\n\r", ch);
	  return;
	}
      }

      for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
      {
         if(!str_cmp(cmd_table[cmd].name, argument))
         {
            strcpy (loglist[num_of_logs+1].cmd, cmd_table[cmd].name);
            num_of_logs++;
            save_loglist();
            return;
         }
      }
            stc("That isn't a valid command.", ch);
            return;
    }
             stc("Bad Syntax or error.\n\r", ch);
    }
 

bool multicheck(CHAR_DATA *ch)
{
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];

  for (gch = char_list; gch; gch = gch->next)
  {
    if (IS_NPC(gch)) continue;
    if (gch == ch) continue;
    if (strlen(gch->lasthost) > 2)
    {
        if (!str_cmp(gch->lasthost, ch->lasthost))
        {
          if (check_multilist(ch, gch))
          {
            sprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name );
            log_string(buf);
            chprintf( ch, "You must be given special permission to multiplay. Ask an imm if you require it.\n\r." );
            if(gch->desc)
            chprintf( gch, "You must be given special permission to multiplay. Ask an imm if you require it.\n\r" );
            stop_fighting(ch, TRUE);
	    cleartimer(ch);
            do_quit( ch, "" );
	    return FALSE;
          }
          return TRUE;
        }
    }
  }
  return FALSE;
}  

void do_boot( CHAR_DATA *ch, char *argument)
{
int to_room;
CHAR_DATA *victim;
CHAR_DATA *mount;
ROOM_INDEX_DATA *pRoomIndex;
char buf[MSL];

   if (argument[0] == '\0')
   {
      stc("boot whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_world(ch, argument)) == NULL)
   {
      stc("That person doesn't exist, try again.\n\r", ch);
      return;
   }

    for ( ; ; )
    {
        to_room = number_range( 0, 65535 );
        pRoomIndex = get_room_index( to_room );
        if ( pRoomIndex != NULL )
        if ( !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
        &&   !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY)
       /* &&   !IS_SET(pRoomIndex->room_flags, ROOM_NO_TELEPORT)*/)
            break;
    }
    sprintf(buf, "You boot %s across the realm.\n\r", victim->name);
    stc(buf, ch);
    sprintf(buf, "$n boots %s across the realm", victim->name);
    act( buf, ch, NULL, NULL, TO_ROOM, FALSE);
    send_to_char("An immortal size 10 boots you across the realm\n\r", victim);
    char_from_room( victim );
    char_to_room( victim, pRoomIndex );
    act( "$n falls from the sky with a thud, a big bootprint on his ass.", victim, NULL, NULL, TO_NOTVICT, FALSE );
    do_look( victim, "auto" );
    if ( (mount = ch->mount) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    do_look( mount, "auto" );
    return;
}

void insta_object( OBJ_DATA *list, CHAR_DATA *ch)
{
  ROOM_INDEX_DATA       *pRoom;
  OBJ_INDEX_DATA        *pObjIndex;
  OBJ_DATA              *obj, *inobj;
  RESET_DATA            *pReset, *cReset;
  char                  output[MAX_STRING_LENGTH];

  if (ch->in_room != NULL)
  {
    EDIT_ROOM(ch, pRoom);
    for (obj = list; obj != NULL; obj = obj->next_content )
    {
      if ( !( pObjIndex = get_obj_index( obj->pIndexData->vnum ) ) )
      {
        send_to_char( "REdit: No object has that vnum.\n\r", ch );
        return;
      }

      if ( pObjIndex->area != pRoom->area )
      {
        send_to_char( "REdit: No such object in this area.\n\r", ch );
        return;
      }

      if (pObjIndex->vnum > -1)
      {
        pReset          = new_reset_data();
        pReset->command = 'O';
        pReset->arg1    = pObjIndex->vnum;
        pReset->arg2    = 0;
        pReset->arg3    = pRoom->vnum;
        add_reset( pRoom, pReset, 0 );
        sprintf( output, "%s (%d) has been loaded and added to resets (Object).\n\r",
          capitalize( pObjIndex->short_descr), pObjIndex->vnum );
        send_to_char( output, ch);

        if (obj->item_type == ITEM_CONTAINER)
        {
          for ( inobj = obj->contains; inobj != NULL; inobj = inobj->next_content)
          {
            cReset              = new_reset_data();
            cReset->command     = 'P';
            cReset->arg1        = inobj->pIndexData->vnum;
            cReset->arg3        = obj->pIndexData->vnum;
            add_reset( pRoom, cReset, 0);
            sprintf( output, "%s (%d) will be loaded inside of %s (%d).\n\r",
              capitalize( inobj->short_descr), inobj->pIndexData->vnum, pObjIndex->short_descr, pObjIndex->vnum);
            send_to_char( output, ch);
          }
        }
      }
    }
  }
  return;
}

void insta_monster( CHAR_DATA *list, CHAR_DATA *ch )
{
  ROOM_INDEX_DATA       *pRoom;
  CHAR_DATA             *rch;
  RESET_DATA            *pReset, *gReset, *eReset, *cReset;
  MOB_INDEX_DATA        *pMobIndex;
  OBJ_DATA              *obj, *inobj;
  char                  output[MAX_STRING_LENGTH];

  if (ch->in_room != NULL)
  {

    for (rch = list; rch != NULL; rch = rch->next_in_room )
    {
      EDIT_ROOM(ch, pRoom);

      if ( rch == ch )
        continue;

      if ( !IS_NPC(rch))
        continue;

      if ( !( pMobIndex = get_mob_index( rch->pIndexData->vnum ) ) )
      {
        send_to_char( "REdit: No mobile has that vnum.\n\r", ch );
        break;
      }

      if ( pMobIndex->area != pRoom->area )
      {
        send_to_char( "REdit: No such mobile in this area.\n\r", ch );
        break;
      }

      if ( pMobIndex->vnum > -1)
      {
        pReset = new_reset_data();
        pReset->command     = 'M';
        pReset->arg1        = pMobIndex->vnum;
        pReset->arg2        = rch->pIndexData->count;
        pReset->arg3        = pRoom->vnum;
        add_reset( pRoom, pReset, 0 );

        for ( obj = rch->carrying; obj; obj = obj->next_content )
        {
          if ( obj->wear_loc == WEAR_NONE )
          {
            gReset = new_reset_data();
            gReset->command     = 'G';
            gReset->arg1        = obj->pIndexData->vnum;
//            gReset->arg2      = ;
//            gReset->arg3      = ;
            add_reset( pRoom, gReset, 0);
            sprintf( output, "%s (%d) will be loaded on the mobile %s (%d).\n\r",
              capitalize( obj->pIndexData->short_descr), obj->pIndexData->vnum, rch->short_descr, rch->pIndexData->vnum);
            send_to_char( output, ch);

            if (obj->item_type == ITEM_CONTAINER)
            {
              for ( inobj = obj->contains; inobj != NULL; inobj = inobj->next_content)
              {
                cReset              = new_reset_data();
                cReset->command     = 'P';
                cReset->arg1        = inobj->pIndexData->vnum;
                cReset->arg3        = obj->pIndexData->vnum;
                add_reset( pRoom, cReset, 0);
                sprintf( output, "%s (%d) will be loaded inside of %s (%d) on the mobile %s (%d).\n\r",
                  capitalize( inobj->short_descr), inobj->pIndexData->vnum, obj->pIndexData->short_descr, obj->pIndexData->vnum, rch->short_descr, rch->pIndexData->vnum);
                send_to_char( output, ch);
              }
            }
          }
          else
          {
            eReset = new_reset_data();
            eReset->command     = 'E';
            eReset->arg1        = obj->pIndexData->vnum;
//            eReset->arg2      = ;
            eReset->arg3        = obj->wear_loc;
            add_reset( pRoom, eReset, 0);
            sprintf( output, "%s (%d) will be loaded on the mobile %s (%d) in the %d wear slot.\n\r",
              capitalize( obj->pIndexData->short_descr), obj->pIndexData->vnum, rch->short_descr,  rch->pIndexData->vnum, obj->wear_flags);
            send_to_char( output, ch);

            if (obj->item_type == ITEM_CONTAINER)
            {
              for ( inobj = obj->contains; inobj != NULL; inobj = inobj->next_content)
              {
                cReset              = new_reset_data();
                cReset->command     = 'P';
                cReset->arg1        = inobj->pIndexData->vnum;
                cReset->arg3        = obj->pIndexData->vnum;
                add_reset( pRoom, cReset, 0);
                sprintf( output, "%s (%d) will be loaded inside of %s (%d) on the mobile %s (%d).\n\r",
                  capitalize( inobj->short_descr), inobj->pIndexData->vnum, obj->pIndexData->short_descr,  obj->pIndexData->vnum, rch->short_descr, rch->pIndexData->vnum);
                send_to_char( output, ch);
              }
            }
          }
        }
        sprintf( output, "%s (%d) has been loaded and added to resets.\n\r"
          "There will be a maximum of %d loaded to this area.\n\r",
          capitalize( pMobIndex->short_descr ), pMobIndex->vnum, pReset->arg2 );
        send_to_char( output, ch );
      }
    }
  }
  return;
}


void do_instaroom( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch))
    return;

  if (!IS_IMMORTAL(ch))
  {
    do_huh(ch, "");
    return;
  }

  if (ch->in_room != NULL)
  {
    insta_object(ch->in_room->contents, ch);
    insta_monster(ch->in_room->people, ch);
  }
  return;
}
struct swear_list swearlist[999];
int swearlist_num;

struct quote_list quotelist[999];
int quotelist_num;

struct multiplay_list multiplaylist[999];
int multiplaylist_num;

void read_swearlist()
{
  FILE *fp;
  int j;
  char word[MSL];

  if((fp=fopen(SWEARLIST_FILE,"r"))==NULL)
  {
    bug("Cannot open Swearlist file for reading",0);
    return;
  }

        swearlist_num = -1;

  for(j=0;;j++)
  {
    fscanf(fp,"%s\n",word);
      if(!str_cmp(word,"NULL"))
      {
        break;
      }

      strcpy (swearlist[j].word, word);
      swearlist_num = j;
   }
  fclose(fp);
return;
 }

void save_swearlist()
{
  FILE *fp;
  int f= 0;

  if((fp=fopen(SWEARLIST_FILE,"w"))==NULL)
  {
    bug("Cannot open Swearlist file for writing",0);
    return;
  }
  if (swearlist_num >= 0)
  for(f = 0;f <=swearlist_num;f++)
  {
    fprintf(fp,"%s\n",swearlist[f].word);
  }
  fprintf(fp,"NULL NULL NULL~\n");
  fclose(fp);

}
void do_swearlist(CHAR_DATA *ch, char *argument)
{
  char buf[MSL];
  char buf2[MSL];
  char arg[MSL];
  char arg2[MSL];
  int i;
  int f;
  int x;
  int sex;

   if (argument[0] == '\0')
   {
     stc("syntax: Swearlist add/remove <word>\n\r", ch);
     return;
   }
   argument=one_argument(argument,arg);
   argument=one_argument(argument,arg2);
   if(arg[0]=='\0')
   {
     stc("syntax: Swearlist add/remove <word>\n\r", ch);
     stc("syntax: Swearlist list\n\r", ch);
     return;
   }

    if(!str_cmp(arg,"add"))
    {
      if(arg2[0]=='\0')
      {
        stc("syntax: Swearlist add|remove <word>\n\r", ch);
        return;
      }

      arg2[0] = UPPER(arg2[0]);

      for(sex = 0; sex <= swearlist_num; sex++)
      {
        if(!str_cmp(swearlist[sex].word, arg2))
        {
          stc("That word is already filtered.\n\r", ch);
          return;
        }
      }
      strcpy (swearlist[swearlist_num + 1].word, arg2);
      swearlist_num += 1;
      save_swearlist();
      return;
    }

     else if(!str_cmp(arg,"remove"))
    {
       if(arg2[0]=='\0')
       {
         stc("syntax: Swearlist add|remove <word>\n\r", ch);
         return;
       }

    arg2[0] = UPPER(arg2[0]);


    for(i=0;i<=swearlist_num;i++)
    {
      if(!str_cmp(swearlist[i].word,arg2))
      {
        for(f=i;f<=swearlist_num;f++)
        {
          swearlist[f]=swearlist[f+1];
        }
        swearlist_num--;
        save_swearlist();
        return;
      }
    }
    }

     else if(!str_cmp(arg, "list"))
    {
      if (swearlist_num >= 0)
      {
	buf[0] = '\0';
        stc("Filtered Words:\n\r", ch);
        stc("#g----------------------------------------------------------#n\n\r\n\r", ch);
        for(x=0; x<=swearlist_num; x++)
        {
	  sprintf(buf2, "%10s", swearlist[x].word);
	  stc(buf2, ch);
	  if ((x+1) % 4 == 0)
          stc("\n\r", ch );
        }
       }
       else
        stc("\n\rNone.\n\r", ch);
        return;
     }
    else
    {
        stc("Choices are Add, Remove or list\n\r", ch);
        return;
    }
}

int check_swearlist(CHAR_DATA *ch, char *argument)
{
int i;
if (IS_IMMORTAL(ch)) return 0;
for(i = 0; i <= swearlist_num; i++)
{

  if(is_contained(swearlist[i].word, argument))
  {
     return 1;
  }
}
return 0;
}

char *colour_strip(char const *str)
{
    static char buf[MAX_STRING_LENGTH];
           char last_char = '\0';
           int  pos       = 0;
    assert(str);
    while(*str != '\0')
    {
        if(last_char == COLOUR_CHAR)
        {
            if(*str == COLOUR_CHAR)
            {
                buf[pos++] = COLOUR_CHAR;
            }
        }
        else
        {
            if(*str != COLOUR_CHAR)
            {
                buf[pos++] = *str;
            }
        }
        last_char = *str++;
    }
    buf[pos] = '\0';
    return buf;
}

void do_multiple( CHAR_DATA *ch, char *argument)
{

char arg1[MSL];
int sex;
int x;

   argument = one_argument(argument, arg1);

   if ( !is_number(arg1))
   {
      stc("You go to hell, you go to hell and die!\n\r", ch);
      return;
   }


   sex = atoi(arg1);
   if (sex > 20000) sex = 20000;

   for ( x = 1 ; x <= sex; x++)
   {
      interpret(ch, argument);
//      process_output(ch->desc, FALSE);
   }

}

void do_page(CHAR_DATA *ch, char *argument)
{
CHAR_DATA *tch;

char arg1[MIL];
char buf[MSL];

   if (argument[0] == '\0')
   {
      stc("page who with what?\n\r", ch);
      return;
   }

   argument = one_argument(argument, arg1);

   if ((tch = get_char_world(ch, arg1)) == NULL)
   {
      stc("That person doesn't exist, try again.\n\r", ch);
      return;
   }

   if (argument[0] == '\0')
   {
      sprintf(buf, "You page %s.\n\r", tch->name);
      stc(buf, ch);
      sprintf(buf, "\007page from %s, beeeeep.\n\r", ch->name);
      stc(buf, tch);
      return;
   }

   sprintf(buf, "\007You page %s with:#C %s.#n\n\r",tch->name,argument);
   stc(buf, ch);
   sprintf(buf, "\007Page from %s:#C %s.#n\n\r", ch->name, argument);
   stc(buf, tch);
   return;
}

void do_buymod( CHAR_DATA *ch, char *argument)
{
   char buf[MSL];

   if( IS_NPC( ch ) )
      return;
   if (!str_cmp(argument, "exp"))
   {
      if (ch->pcdata->quest < 1000 && !IS_IMMORTAL(ch))
      {
         stc("You can't afford the 1000 qps to buy double exps.\n\r", ch);
         return;
      }

      if (!IS_IMMORTAL(ch))
      ch->pcdata->quest -= 1000;

      if (sysdata.exp_timer >= 0)
      {
         sprintf( buf, "%s has bought another hour of double exp for everyone.", ch->name );
         do_info( ch, buf );
         sysdata.exp_timer += 3600;
         return;
      }
 
      sysdata.exp_timer = 3600;
      sprintf( buf, "%s has bought an hour of double exp for everyone.", ch->name );
      do_info( ch, buf );
      return;
   }
   else if (!str_cmp(argument, "qp"))
   {
      stc("Questing has been removed.\n\r",ch);
      return;
   }
   else
   stc("Options: exp\n\r", ch);
}
void do_cls( CHAR_DATA *ch, char *argument )
{
    send_to_char( "\e[2J", ch );
    send_to_char( "\e[0;0f", ch );
}

void do_nuke( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char strsave[MAX_INPUT_LENGTH];
  char arg    [MAX_INPUT_LENGTH];
  char buf    [MAX_STRING_LENGTH];
  const char *cname;

  argument = one_argument( argument, arg );

  if ( IS_NPC(ch))
    return;

  if ( arg[0] == '\0' )
  {
    send_to_char( "Who do you wish to nuke?\n\r", ch);
    return;
  }

  if (( victim = get_char_world( ch, arg )) == NULL)
  {

    send_to_char( "They are not online.\n\r", ch);
    return;
}
else if (IS_NPC(victim))
{
    stc("Not on NPC's!\n\r", ch);
    return;
}
  if (victim->name)
  {
    if(victim->level < ch->level){
    cname = capitalize( victim->name );
    stop_fighting(victim, TRUE);
sprintf( buf,"#C%s#c flies down from the heavens.#n",ch->name);
do_echo(ch,buf);
sprintf( buf,"#C%s#c points a finger at %s#c.#n\n\r",ch->name,victim->name);
do_echo(ch,buf);
sprintf( buf,"#C%s#c says, #y'You've made one too many mistakes, #R%s#g!'#n",ch->name,victim->name);
do_echo(ch,buf);
sprintf( buf,"#cA #7b#8l#yi#Lnd#yi#8n#7g #cblast of energy erupts from #C%s's#c fingertip and #rIN#RCI#rNER#RAT#rES #C%s#n!",ch->name,victim->name);
do_echo(ch,buf);
sprintf( buf,"#cOnly a small puddle of #ogoo#c remains where #C%s#c was standing.#n",victim->name);
do_echo(ch,buf);
sprintf( buf,"#C%s #chas been vaporized!#n",victim->name);
do_echo(ch,buf);
    if (victim->carrying != NULL)
    drop_unowned(victim, victim->carrying); 

    do_disconnect(ch,victim->name);
    stop_fighting(victim,TRUE);
    cleartimer(victim);
    do_quit(victim, "");
    sprintf( strsave, "%s%s", PLAYER_DIR, cname);
    unlink(strsave);
    sprintf( strsave, "%s%s", BACKUP_DIR, cname);
    unlink(strsave);
//    sprintf( strsave, "%s%s", STORE_DIR, cname);
//    unlink(strsave);
    sprintf(buf, "Deleted %s", cname);
    log_string(buf);
}
}
  else if (victim->name != arg)
  ch->pkill += 1;
  return;
}

void do_gsocial(CHAR_DATA *ch, char *argument)
{
    char command[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    int counter;
    int count;
    char buf2[MAX_STRING_LENGTH];

    argument = one_argument(argument,command);
    if (IS_NPC(ch)) return;
    if (command[0] == '\0')
    {
        send_to_char("What do you wish to gocial?\n\r",ch);
        return;
    }
    if (IS_NPC(ch)) return;
     if (!xIS_SET(ch->extra, EXTRA_CHAT))
        {
          stc("You have not yet been allowed to communicate.\n\r",ch);
          return;
        }

    if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_SILENCE) )
    {
        send_to_char( "You're silenced!\n\r", ch );
        return;
    }

    found = FALSE;
    for (cmd = 0; cmd < maxSocial; cmd++)
    {
        if (command[0] == social_table[cmd].name[0]
        && !str_prefix( command,social_table[cmd].name ) )
        {
            found = TRUE;
            break;
        }
    }

    if (!found)
    {
        send_to_char("What kind of social is that?!?!\n\r",ch);
        return;
    }


    if ( !IS_NPC(ch) && xIS_SET(ch->deaf, CHANNEL_GSOCIAL))
    {
        send_to_char("But you have the gsocial channel turned off!\n\r",ch);
        return;
    }

    if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_SILENCE))
    {
        send_to_char("You are silenced!.\n\r",ch);
        return;
    }

    switch (ch->position)
    {
    case POS_DEAD:
        send_to_char("Lie still; you are DEAD!\n\r",ch);
        return;
    case POS_INCAP:
    case POS_MORTAL:
        send_to_char("You are hurt far too bad for that.\n\r",ch);
        return;
    case POS_STUNNED:
        send_to_char("You are too stunned for that.\n\r",ch);
        return;
    }

    one_argument(argument,arg);
    victim = NULL;
    if (arg[0] == '\0')
    {
        sprintf(buf, "#r/#R=#r\\#cG#CS#7O#0C#7I#CA#cL#r/#R=#r\\#7     %s#n", social_table[cmd].char_no_arg );
        act(buf,ch,NULL,NULL,TO_CHAR, FALSE);
        sprintf(buf, "#r/#R=#r\\#cG#CS#7O#0C#7I#CA#cL#r/#R=#r\\#7     %s#n", social_table[cmd].others_no_arg );
        for (d = first_descriptor; d != NULL; d = d->next)
        {
            CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
            if (d->connected == CON_PLAYING &&
                d->character != ch &&
                !xIS_SET(vch->deaf, CHANNEL_GSOCIAL))
            {
                act(buf,ch,NULL,vch,TO_VICT, FALSE);
            }
        }
    }
    else if ((victim = get_char_world(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    else if (victim == ch)
    {
        sprintf(buf, "#r/#R=#r\\#cG#CS#7O#0C#7I#CA#cL#r/#R=#r\\#7     %s#n",  social_table[cmd].char_auto);
        act(buf,ch,NULL,NULL,TO_CHAR, FALSE);
        sprintf(buf, "#r/#R=#r\\#cG#CS#7O#0C#7I#CA#cL#r/#R=#r\\#7     %s#n",  social_table[cmd].others_auto);
        for (d = first_descriptor; d != NULL; d = d->next)
        {
            CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
            if (d->connected == CON_PLAYING &&
                d->character != ch &&
                !xIS_SET(vch->deaf, CHANNEL_GSOCIAL))
            {
                act(buf,ch,NULL,vch,TO_VICT, FALSE);
            }
        }
    }
    else
    {
        sprintf(buf, "#r/#R=#r\\#cG#CS#7O#0C#7I#CA#cL#r/#R=#r\\#7     %s#n",  social_table[cmd].char_found);
        act(buf,ch,NULL,victim,TO_CHAR, FALSE);
        sprintf(buf, "#r/#R=#r\\#cG#CS#7O#0C#7I#CA#cL#r/#R=#r\\#7     %s#n", social_table[cmd].vict_found);
        act(buf,ch,NULL,victim,TO_VICT, FALSE);
        sprintf(buf, "#r/#R=#r\\#cG#CS#7O#0C#7I#CA#cL#r/#R=#r\\#7     %s#n",  social_table[cmd].others_found);
        for (counter = 0; buf[counter+1] != '\0'; counter++)
        {
            if (buf[counter] == '$' && buf[counter + 1] == 'N')
            {
                strcpy(buf2,buf);
                buf2[counter] = '\0';
                strcat(buf2,victim->name);
                for (count = 0; buf[count] != '\0'; count++)
                {
                    buf[count] = buf[count+counter+2];
                }
                strcat(buf2,buf);
                strcpy(buf,buf2);

            }
            else if (buf[counter] == '$' && buf[counter + 1] == 'E')
            {
                switch (victim->sex)
                {
                default:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"it");
                    for (count = 0; buf[count] != '\0'; count ++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 1:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"it");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 2:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"it");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                }
            }
            else if (buf[counter] == '$' && buf[counter + 1] == 'M')
            {
                buf[counter] = '%';
                buf[counter + 1] = 's';
                switch (victim->sex)
                {
                default:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"it");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 1:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"him");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 2:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"her");
                    for (count = 0; buf[count] != '\0'; count++);
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                }
            }
            else if (buf[counter] == '$' && buf[counter + 1] == 'S')
            {
                switch (victim->sex)
                {
                default:
                strcpy(buf2,buf);
                buf2[counter] = '\0';
                strcat(buf2,"its");
                for (count = 0;buf[count] != '\0'; count++)
                {
                    buf[count] = buf[count+counter+2];
                }
                strcat(buf2,buf);
                strcpy(buf,buf2);
                break;
                case 1:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"his");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                case 2:
                    strcpy(buf2,buf);
                    buf2[counter] = '\0';
                    strcat(buf2,"hers");
                    for (count = 0; buf[count] != '\0'; count++)
                    {
                        buf[count] = buf[count+counter+2];
                    }
                    strcat(buf2,buf);
                    strcpy(buf,buf2);
                    break;
                }
            }

        }
        for (d=first_descriptor; d != NULL; d = d->next)
        {
            CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
            if (d->connected == CON_PLAYING &&
                d->character != ch &&
                d->character != victim &&
                !xIS_SET(vch->deaf, CHANNEL_GSOCIAL))
            {
                act(buf,ch,NULL,vch,TO_VICT, FALSE);
            }
        }
    }
    return;
}

void do_global(CHAR_DATA *ch, char *argument)
{
char buf[MSL];
DESCRIPTOR_DATA *d;
    if (IS_NPC(ch)) return;
     if (!xIS_SET(ch->extra, EXTRA_CHAT))
        {
          stc("You have not yet been allowed to communicate.\n\r",ch);
          return;
        }

  if (!IS_NPC(ch) && (ch->mkill < 200) && (ch->level < 4))
   {
     send_to_char("Because of abuse of the global channel (spamming, etc) you need 200 mobkills to use gpose.\n\r", ch);
     send_to_char("Getting 200 mob kills is not hard, most mobs in the newbie arena and mud school\n\r", ch);
     send_to_char("dies when you touch them.\n\r",ch);
     return;
   }
    if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_SILENCE) )
    {
        send_to_char( "You're silenced!\n\r", ch );
        return;
    }

    if (!argument)
    {
       stc("What would you like to globally pose?\n\r", ch);
        return;
    }
	    if (IS_SET(ch->special, SPC_NEWBIE) && !IS_NPC(ch))
            if (check_swearlist(ch,colour_strip(argument)))
            {
              stc("Your globla emote was not allowed. (help censorship)\n\r",ch);
              return;
            }
           if (ch->sex == SEX_FEMALE)
           {
                chprintf( ch, "#g----#G,#g-#R[#0GLOBAL#R]#g-#G'#g-#G(#R@ #C%s #c%s#n.\n\r", ch->name, argument );
                sprintf( buf, "#g----#G,#g-#R[#0GLOBAL#R]#g-#G'#g-#G(#R@ #C%s #c$t#n.",ch->name );
            }
            else
            {
                chprintf( ch, "#L>>#c---#7[#LGLOBAL#7]#c---#7|> #C%s #c%s#n.\n\r", ch->name, argument );
                sprintf( buf, "#L>>#c---#7[#LGLOBAL#7]#c---#7|> #C%s #c$t#n.",ch->name );
            }

        for (d = first_descriptor; d != NULL; d = d->next)
        {
            CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
            if (d->connected == CON_PLAYING &&
                d->character != ch &&
                !xIS_SET(vch->deaf, CHANNEL_GSOCIAL))
            {
            act( buf, ch, argument, vch, TO_VICT, FALSE );
            }
        }

  return;
}
void do_delete( CHAR_DATA *ch, char *argument)
{
    char strsave[MAX_INPUT_LENGTH];
    char        arg[MAX_INPUT_LENGTH];
    char        buf[MAX_STRING_LENGTH];
    char *pArg;
    const char *cname;


    if ( IS_NPC( ch ) ) return;
       {
		pArg = arg;
		while (*argument == ' ') argument++;
		while (*argument != '\0') {
			if (*argument == ' ') {
				argument++;
				break;
			}
			*pArg++ = *argument++;
		}
		*pArg = '\0';
	}

    if ( arg[0] == '\0' )
    {
        send_to_char( "Syntax: delete [password]\n\r", ch );
        send_to_char( "   If you self delete, you will #rNOT#n get a reimburse\n\r", ch);
        send_to_char( "   Use this command very carefully.\n\r", ch);
        sprintf(buf, "%s is contemplating deletion.", ch->name);
        log_string(buf);
        return;
    }
    if ( strcmp( crypt( arg, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
        send_to_char("Wrong Password, wait 10 seconds.\n\r", ch );
        WAIT_STATE( ch, 40 );
	sprintf(buf, "%s tried to delete with the wrong pwd.", ch->name);
        log_string(buf);
        return;
    }
    cname = capitalize( ch->name );
    do_call(ch,"all");
    sprintf(buf, "%s has deleted.", ch->name );
    log_string(buf);
    sprintf(buf, "#C%s#7 has turned %s into line noise.#n",ch->pcdata->switchname,ch->sex == SEX_MALE ? "himself" : ch->sex == SEX_FEMALE ? "herself" : "itself");
    do_info(NULL,buf);
    stop_fighting(ch, TRUE);
//    char_from_room( ch );
    cleartimer(ch);
//    do_quit( ch, "" );
    if (ch->carrying != NULL)
    drop_unowned(ch, ch->carrying);
    if (ch->pcdata->chobj != NULL) extract_obj(ch->pcdata->chobj);
    if ( ch->in_room != NULL )
    extract_char( ch, TRUE );
    sprintf( strsave, "%s%s", PLAYER_DIR, cname);
    unlink(strsave);
    sprintf( strsave, "%s%s", BACKUP_DIR, cname);
    unlink(strsave);
    return;
}

void do_new_fread_to_eol( FILE *fp )
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

void fread_and_write_eol( FILE *fp, FILE *fp2 )
{
    char c = ' ';

    do
    {
       c = getc( fp );
       fprintf(fp2, "%c", c);
    }
    while ( c != '\n' );
    
    return;
}

EXT_BV bitv_to_extbv( int bitv )
{
    EXT_BV new_bitv;
    int x;
    int count=0;
       xCLEAR_BITS(new_bitv);

       for ( x = 1;count <32; x = x*2)
       {
          if ( IS_SET( bitv, x) )
                 xSET_BIT(new_bitv, count);
          count +=1;
       }
return new_bitv;
}

void convert_to_extbv( CHAR_DATA *ch, int version, int bitv )
{
    int x;
    int count=0;

    if (version == VERSION_ACT)
       bitv = bitv - 1600;

       for ( x = 1;count <31; x = x*2)
       {
          if ( IS_SET( bitv, x) )
          {
	     switch(version)
	     {
             case VERSION_EXTRA: 
                 xSET_BIT(ch->extra, count);
	         break;
	     case VERSION_ACT:
		 if (!xIS_SET(ch->act, count))
		 xSET_BIT(ch->act, count);
		 break;
	     case VERSION_DEAF:
		 xSET_BIT(ch->deaf, count);
		 break;
             default:
	        bug("Problem in convert_to_extbv.", 0);
		break;
	     }

          }
          count +=1;
      }
}

void fwrite_converted( FILE *fp, FILE *fp2 )
{
    int count = 0;
    int bitv;
    int x;
    EXT_BV new_bitv;

       bitv = fread_number( fp );
       fprintf(fp2, "      ");
       for ( x = 1;count <32; x = x*2)
       {
          count +=1;
          if ( IS_SET( bitv, x) )
          {
	     xSET_BIT(new_bitv, count);
          }
      }
      fprintf( fp2, "%s\n", print_bitvector(&new_bitv));
    return;
}

/* Zeran - procedure to scan for multiple commands */
void check_multi_cmd ( CHAR_DATA * ch, char *orig_cmd, char *final_cmd )
{
     int                 count;
     int                 len;
     char               *tmp_ptr = NULL;
     bool                first_s_quote = FALSE;
     bool                first_d_quote = FALSE;
     char               *tmp_incomm = NULL;
     bool                need_tmp_incomm = FALSE;
     char		 first_word[MIL];

     one_argument( orig_cmd, first_word );
     if (!str_cmp(first_word, "alias"))
     {
        strcpy( final_cmd, orig_cmd );
        return;
     }
     len = strlen ( orig_cmd );
    /* are we parsing an alias, or just parsing incomm? */
     if ( orig_cmd != ch->desc->incomm )
          need_tmp_incomm = TRUE;
     for ( count = 0; count < len; count++ )
     {
          if ( orig_cmd[count] == '"' )
               {
                    if ( !first_s_quote )
                    {
                         if ( !first_d_quote )
                              first_d_quote = TRUE;
                         else
                              first_d_quote = FALSE;
                    }
               }
          else if ( orig_cmd[count] == '\'' )		/* skip if count is 0...its the short say command */
               {
                    if ( count == 0 )
                         break;
                    if ( !first_d_quote )
                    {
                         if ( !first_s_quote )
                              first_s_quote = TRUE;
                         else
                              first_s_quote = FALSE;
                    }
               }
          else if ( orig_cmd[count] == ch->pcdata->stackchar )
               {
                    if ( !tmp_ptr && !first_d_quote && !first_s_quote )
                         tmp_ptr = &( orig_cmd[count] );
               }
          /* if got a separator pointer, break */
          if ( tmp_ptr )
               break;
     }
     /* end for loop */
     if ( tmp_ptr != NULL && !first_s_quote && !first_d_quote )
     {
          ch->desc->multi_comm = TRUE;

          /* copy ch->desc->incomm if needed */
          if ( need_tmp_incomm )
               tmp_incomm = str_dup ( ch->desc->incomm );
          *tmp_ptr = '\0';
          tmp_ptr++;
          while ( isspace ( *tmp_ptr ) )
               tmp_ptr++;
          strcpy ( final_cmd, orig_cmd );
          strcpy ( ch->desc->incomm, tmp_ptr ); // strcpy with a pointer
          if ( need_tmp_incomm )
          {
               if ( ( strlen ( ch->desc->incomm ) + strlen ( ( tmp_ptr + 1 ) ) ) >= ( 
MAX_INPUT_LENGTH - 10 ) )
               {
                    send_to_char ( "Command expansion too large, ignoring last command.\n\r", ch );
                    ch->desc->incomm[0] = '\0';
                    final_cmd[0] = '\0';
                    return;
               }
               strcat ( ch->desc->incomm, "|" );
               strcat ( ch->desc->incomm, tmp_incomm );
          }

          free_string ( tmp_incomm );
          return;
     }
     else
     {
          strcpy ( final_cmd, orig_cmd );
          if ( orig_cmd == ch->desc->incomm )
               ch->desc->incomm[0] = '\0';
     }
     return;
}
void do_desc( CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC( ch ) )
    {
	send_to_char( "Monsters are too dumb to do that!\n\r", ch );
	return;	  
    }

    if ( !ch->desc )
    {
	bug( "do_description: no descriptor", 0 );
	return;
    }

    switch( ch->substate )
    {
	default:
	   bug( "do_description: illegal substate", 0 );
	   return;

	case SUB_RESTRICTED:
	   send_to_char( "You cannot use this command from within another command.\n\r", ch );
	   return;

	case SUB_NONE:
	   ch->substate = SUB_PERSONAL_DESC;
	   ch->dest_buf = ch;
	   start_editing( ch, ch->description );
	   return;

	case SUB_PERSONAL_DESC:
	   STRFREE( ch->description );
	   ch->description = copy_buffer( ch );
	   stop_editing( ch );
	   return;	
    }
}
void do_backup( CHAR_DATA *ch, char *argument)
{
  char buf1[200];
  char buf2[200];
  struct tm *ptr;
  time_t lt;

  lt = time( '\0' );

  ptr = localtime(&lt);

sprintf(buf1, "%d-%d_%d-%d-%d.tgz", ptr->tm_hour, 
ptr->tm_min, ptr->tm_mon+1, 
ptr->tm_mday, ptr->tm_year);

if (!str_cmp( argument, "all" ))
sprintf( buf2, "./backup %s all", buf1);
else if (!str_cmp( argument, "source" ) || !str_cmp( argument, "src" ) )
sprintf( buf2, "./backup %s src", buf1);
else if ( !str_cmp( argument, "players" ) || !str_cmp( argument, "player" ) )
sprintf( buf2, "./backup %s player", buf1);
else if ( !str_cmp( argument, "areas" ) || !str_cmp( argument, "area" ) )
sprintf( buf2, "./backup %s area", buf1);
else
{
   stc( "Options are: all, source, players, areas.\n\r", ch);
   return;
}
system( buf2 );
return;

}
void drop_unowned( CHAR_DATA *ch, OBJ_DATA *obj)
{
   if (IS_NPC(ch))
      return;
   if (!ch)
      return;
   if (obj->next_content != NULL)
      drop_unowned(ch, obj->next_content);

   if (obj->questowner != NULL && strlen(obj->questowner) > 1 )
   {
       if (str_cmp(ch->pcdata->switchname,obj->questowner))
       {
          if ( obj->in_obj != NULL )
             obj_from_obj(obj);
	  else if (obj->carried_by != NULL)
             obj_from_char(obj);
          else
             bug("drop_unowned, %s not on %s or in anything.", obj->short_descr, ch->name);
          obj_to_room( obj, ch->in_room );
       }
   }
   if ( obj->contains != NULL )
      drop_unowned(ch, obj->contains);
}
/*
void do_desc( CHAR_DATA *ch, char *argument)
{
if (IS_NPC(ch))
return;

   if ( argument[0] == '\0' )
    {
        ch->desc->pEdit = (void *)ch;
//        ch->desc->editor = ED_DESC;
        string_append( ch, &ch->description );
    }
}
*/
/*
void do_random( CHAR_DATA *ch, char *argument ) {
        char arg1[MAX_INPUT_LENGTH];
        char arg2[MAX_INPUT_LENGTH];
        char arg3[MAX_INPUT_LENGTH];
        char arg4[MAX_INPUT_LENGTH];
        char buf[MAX_STRING_LENGTH];
        RANDOM_DATA *random;
        RANDOM_DATA *new_random;
        OBJ_INDEX_DATA *pObjIndex;

        smash_tilde(argument);
        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        argument = one_argument(argument, arg3);
        strcpy(arg4, argument);

        send_to_char("#w", ch);
        if (arg1[0] == '\0') {
                send_to_char("Syntax: random create\r\n",ch);
                send_to_char("        random delete <number>\r\n",ch);
                send_to_char("        random list\r\n",ch);
                send_to_char("        random show <number>\r\n", ch);
                send_to_char("        random set <number> <field> <value>\r\n", ch);
                send_to_char("        random save\r\n",ch);
                send_to_char("Where <field> is one of:\r\n",ch);
                send_to_char("  vnum lvnum uvnum\r\n",ch);
                return;
        }

        if (!strcmp(arg1, "list"))
        {
                if (!first_random) {
                        send_to_char("No random items have been defined.\r\n",ch);
                        return;
                }
                random = first_random;
                send_to_char("Currently defined random items:\r\n",ch);
                while (random)
                {
                sprintf(buf, "%d : %s\r\n", random->vnum,
                (pObjIndex = get_obj_index(random->vnum)) ?
                pObjIndex->short_descr : "???");
                        send_to_char(buf, ch);
                        random=random->next;
                }
        return;
        }

        if (!strcmp(arg1, "show"))
        {
                if (!(random=find_random(atoi(arg2))) )
                {
                        send_to_char("There is no random item with that vnum.\r\n", ch);
                        return;
                }
                sprintf(buf, "%d : %s\r\n", random->vnum,
                (pObjIndex = get_obj_index(ingred->vnum)) ?
                pObjIndex->short_descr : "???");
                send_to_char(buf, ch);
                sprintf(buf, "Sector: %d\r\n", ingred->sector);
                send_to_char(buf, ch);
                sprintf(buf, "Rarity: %d percent\r\n", ingred->rarity);
                send_to_char(buf, ch);
                sprintf(buf, "Moisture:    %d   Elevation:     %d\r\n",
                        ingred->moisture, ingred->elevation);
                send_to_char(buf, ch);
                sprintf(buf, "Temperature: %d   Precipitation: %d\r\n",
                        ingred->temp, ingred->precip);
                send_to_char(buf, ch);
                sprintf(buf, "Mana:        %d\n\r",
                        ingred->mana);
                send_to_char(buf, ch);
                return;
        }

        if (!strcmp(arg1, "create"))
        {
                CREATE(new_ingred, INGRED_DATA, 1);

                if (!first_ingred)
                {
                        first_ingred=new_ingred;
                        last_ingred=new_ingred;
                }
                else
                {
                        last_ingred->next=new_ingred;
                        new_ingred->prev=last_ingred;
                        last_ingred=new_ingred;
                }
                ingred=new_ingred;
                if (arg2)
                        ingred->vnum=atoi(arg2);
                else
                        ingred->vnum=1;
                ingred->sector=0;
                ingred->mana=0;
                ingred->moisture=0;
                    ingred->elevation=0;
                    ingred->precip=0;
                    ingred->temp=0;
                    ingred->rarity=0;
                sprintf(buf, "ingred %d created.\r\n",ingred->vnum);
                send_to_char(buf, ch);
                return;
        }

        if (!strcmp(arg1, "delete"))
        {
                ingred=find_ingred(atoi(arg2));
                if (!ingred)
                {
                        send_to_char("Unknown alchemical ingredient.\r\n",ch);
                        return;
                }

                                chprintf(ch, "Ingredient \"%d\" deleted.\r\n", ingred->vnum);
                                if (ingred==first_ingred)
                                        first_ingred=ingred->next;
                                if (ingred==last_ingred)
                                        last_ingred=ingred->prev;
                                if (ingred->prev)
                                        ingred->prev->next=ingred->next;
                                if (ingred->next)
                                        ingred->next->prev=ingred->prev;
                                DISPOSE(ingred);
                                return;
        }

        if (!strcmp(arg1, "set"))
        {
                ingred=find_ingred(atoi(arg2));
                if (!ingred)
                {
                        send_to_char("Unknown alchemical ingredient.\r\n", ch);
                        return;
                }

                if (!strcmp(arg3, "sector"))
                {
                        if (!is_number(arg4))
                        {
                                send_to_char("That is not a number.\r\n",ch);
                                return;
                        }
                        ingred->sector=atoi(arg4);
                        send_to_char("ingred sector set.\r\n",ch);
                        return;
                }

                if (!strcmp(arg3, "vnum"))
                {
                        if (!is_number(arg4))
                        {
                                send_to_char("That is not a number.\r\n",ch);
                                return;
                        }
                        ingred->vnum=atoi(arg4);
                        send_to_char("ingred vnum set.\r\n",ch);
                        return;
                }
                if (!strcmp(arg3, "rarity"))
                {
                        if (!is_number(arg4))
                        {
                                send_to_char("That is not a number.\r\n",ch);
                                return;
                        }
                        ingred->rarity=atoi(arg4);
                        send_to_char("ingred rarity set.\r\n",ch);
                        return;
                }
                if (!strcmp(arg3, "moisture"))
                {
                        if (!is_number(arg4))
                        {
                                send_to_char("That is not a number.\r\n",ch);
                                return;
                        }
                        ingred->moisture=atoi(arg4);
                        send_to_char("ingred moisture set.\r\n",ch);
                        return;
                }
                if (!strcmp(arg3, "elevation"))
                {
                        if (!is_number(arg4))
                        {
                                send_to_char("That is not a number.\r\n",ch);
                                return;
                        }
                        ingred->elevation=atoi(arg4);
                        send_to_char("ingred elevation set.\r\n",ch);
                        return;
                }
                if (!strcmp(arg3, "mana"))
                {
                        ingred->mana=atoi(arg4);
                        send_to_char("ingred mana set.\r\n",ch);
                        return;
                }
                if (!strcmp(arg3, "precip"))
                {
                        ingred->precip=atoi(arg4);
                        send_to_char("ingred precip set.\r\n",ch);
                        return;
                }
                if (!strcmp(arg3, "temp"))
                {
                        ingred->temp=atoi(arg4);
                        send_to_char("ingred temp set.\r\n",ch);
                        return;
                }
        }

        if (!strcmp(arg1, "save"))
        {
                FILE *fp;

                if ((fp=fopen(INGRED_FILE, "w")) == NULL)
                {
                        bug("Cannot open ingredient file!",0);
                        return;
                }

                for (ingred=first_ingred; ingred; ingred=ingred->next)
                {
                        fprintf(fp, "\n#INGRED\n");
                        fprintf(fp, "Vnum        %d\n", ingred->vnum);
                        fprintf(fp, "Sector      %d\n", ingred->sector);
                        fprintf(fp, "Rarity      %d\n", ingred->rarity);
                        fprintf(fp, "Mana        %d\n", ingred->mana);
                        fprintf(fp, "Temp        %d\n", ingred->temp);
                        fprintf(fp, "Moisture    %d\n", ingred->moisture);
                        fprintf(fp, "Elevation   %d\n", ingred->elevation);
                        fprintf(fp, "Precip      %d\n", ingred->precip);
                }
                fprintf(fp, "#END\n");

                fclose(fp);

                send_to_char("ingredients saved.\r\n",ch);
                return;
        }
        send_to_char("Unknown option.\r\n",ch);
}

*/

int
send_to_world (char *argument, bool showprompt)
{
   DESCRIPTOR_DATA *d;

   for (d = first_descriptor; d != NULL; d = d->next)
     {
        if (d->connected == CON_PLAYING)
          {
             send_to_char (argument, d->character);

             if ((d->fcommand || d->outtop > 0))
               {
                  if (!process_output (d, showprompt))
                    {
                       if (d->character != NULL)
                          save_char_obj (d->character);
                       d->outtop = 0;
                       close_socket (d);
                    }
               }
          }
     }
   return 1;
}

int
insta_to_char (CHAR_DATA *ch, char *argument, bool showprompt)
{
   DESCRIPTOR_DATA *d;

     d = ch->desc;
        if (d->connected == CON_PLAYING)
        {
           send_to_char (argument, d->character);

           if ((d->fcommand || d->outtop > 0))
           {
              if (!process_output (d, showprompt))
              {
                 if (d->character != NULL)
                    save_char_obj (d->character);
                    d->outtop = 0;
                    close_socket (d);
              }
           }
        }
   return 1;
}

/*
 * Load the sysdata file
 */
bool load_systemdata( SYSTEM_DATA *sys )
{
    FILE *fp;
    bool found;

    found = FALSE;

    fclose( fpReserve );
    if ( ( fp = fopen( "sysdata.dat", "r" ) ) != NULL )
    {

        found = TRUE;
        for ( ; ; )
        {
            char letter;
            char *word;

            letter = fread_letter( fp );
            if ( letter == '*' )
            {
                fread_to_eol( fp );
                continue;
            }

            if ( letter != '#' )
            {
                bug( "Load_sysdata_file: # not found." );
                break;
            }

            word = fread_word( fp );
	    log_string( "read in %s", word);
            if ( !str_cmp( word, "SYSTEM" ) )
	    {
	        log_string("freading sysdata");
		fread_sysdata( sys, fp );
            }
            else if ( !str_cmp( word, "END"  ) )
	    {
	        log_string("Ending");
		break;
	    }
            else
            {
                bug( "Load_sysdata_file: bad section." );
                break;
            }
        }
        fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}

/*
 * Read a string from file fp using str_dup (ie: no string hashing)
 */
char *fread_string_nohash( FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *plast;
    char c;
    int ln;

    plast = buf;
    buf[0] = '\0';
    ln = 0;

    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
        if ( feof(fp) )
        {
            bug("fread_string_no_hash: EOF encountered on read.\n\r");
            if ( fBootDb )
                exit(1);
            return str_dup("");
        }
        c = getc( fp );
    }
    while ( isspace(c) );

    if ( ( *plast++ = c ) == '~' )
        return str_dup( "" );

    for ( ;; )
    {
        if ( ln >= (MAX_STRING_LENGTH - 1) )
        {
           bug( "fread_string_no_hash: string too long" );
           *plast = '\0';
           return str_dup( buf );
        }
        switch ( *plast = getc( fp ) )
        {
        default:
            plast++; ln++;
            break;

        case EOF:
            bug( "Fread_string_no_hash: EOF" );
            if ( fBootDb )
              exit( 1 );
            *plast = '\0';
            return str_dup(buf);
            break;

        case '\n':
            plast++;  ln++;
            *plast++ = '\r';  ln++;
            break;

        case '\r':
            break;

        case '~':
            *plast = '\0';
            return str_dup( buf );
        }
    }
}

void fread_sysdata( SYSTEM_DATA *sys, FILE *fp )
{
    char *word;
    bool fMatch;

    sys->time_of_max = NULL;
    sys->mud_name = NULL;
    for ( ; ; )
    {
        word   = feof( fp ) ? "End" : fread_word( fp );
        fMatch = FALSE;

	log_string("Loading sysdata: %s", word);

        switch ( UPPER(word[0]) )
        {
        case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

	case 'C':
	    KEY( "CurrChange", sys->curr_change,	fread_number( fp ) );
	    break;

        case 'E':
	    KEY( "Expmod",  sys->exp_timer,	fread_number( fp ) );

            if ( !str_cmp( word, "End" ) )
            {
                if ( !sys->time_of_max )
                    sys->time_of_max = str_dup("(not recorded)");
                if ( !sys->mud_name )
                    sys->mud_name = str_dup("(Name Not Set)");
		log_string( "Returning");
                return;
            }
            break;
	case 'F':
	    KEY( "FreeForAll",	   sys->ffa_timer,	  fread_number(fp) );
	    break;
        case 'H':
            KEY( "Highplayers",    sys->alltimemax,       fread_number( fp ) );
            KEY( "Highplayertime", sys->time_of_max,      fread_string_nohash( fp ) );
            break;

        case 'M':
            KEY( "MudName", sys->mud_name, fread_string_nohash(fp));
            KEY( "MaxTotExp", sys->max_tot_exp, fread_double(fp));
	    KEY( "MaxItem", sys->max_items, fread_number( fp ) );
            break;

	case 'Q':
	    KEY( "Qpmod",  sys->qp_timer,	fread_number( fp ) );
	    break;

        case 'S':
            KEY( "Saveflags",      sys->save_flags,     fread_number( fp ) );
            KEY( "Savefreq",       sys->save_frequency, fread_number( fp ) );
            break;
        }

        if ( !fMatch )
        {
            bug( "Fread_sysdata: no match: %s", word );
        }
    }
}

/*
 * Save system info to data file
 */
void save_sysdata( SYSTEM_DATA sys )
{
    FILE *fp;

    if (port == 9010)
       return;

    fclose( fpReserve );
    if ( ( fp = fopen( "sysdata.dat", "w" ) ) == NULL )
    {
        bug( "save_sysdata: fopen", 0 );
        perror( "save_sysdata" );
    }
    else
    {
        fprintf( fp, "#SYSTEM\n" );
        fprintf( fp, "MudName        %s~\n", sys.mud_name               );
        fprintf( fp, "Highplayers    %d\n", sys.alltimemax              );
        fprintf( fp, "Highplayertime %s~\n", sys.time_of_max            );
        fprintf( fp, "Saveflags      %d\n", sys.save_flags              );
        fprintf( fp, "Savefreq       %d\n", sys.save_frequency          );
        fprintf( fp, "MaxTotExp      %2.0f\n", sys.max_tot_exp          );
        fprintf( fp, "Expmod	     %d\n", sys.exp_timer		);
        fprintf( fp, "Qpmod	     %d\n", sys.qp_timer		);
	fprintf( fp, "FreeForAll     %d\n", sys.ffa_timer		);
	fprintf( fp, "CurrChange     %ld\n", sys.curr_change		);
	fprintf( fp, "MaxItem        %d\n", sys.max_items		);
        fprintf( fp, "End\n\n"                                          );
        fprintf( fp, "#END\n"                                           );
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

void show_saveflags( CHAR_DATA *ch)
{
int x;
   stc("Save flags are: \n\r", ch);
   for (x = 0; x < 32; x++)
   {
      chprintf(ch, "%s, ", save_flag[x]);
   }
}

void do_cset (CHAR_DATA * ch, char *argument)
{
   char arg[MAX_STRING_LENGTH];
   int level;
   double xp;

   if (argument[0] == '\0')
     {
        chprintf (ch, "#wMud_name: %s", sysdata.mud_name);
        chprintf (ch,
                  "\n\r#wAutosave frequency (minutes):  #w%d\n\r",
                  sysdata.save_frequency);
        chprintf (ch, "#wSave flags: #w%s\n\r",
                  string_flag (sysdata.save_flags, save_flag));
        chprintf (ch, "#wMax Total Exp: #w%2.0f\n\r", sysdata.max_tot_exp);
	chprintf (ch, "#wMax Items:  %d\n\r", sysdata.max_items);
     }
   argument = one_argument (argument, arg);
   smash_tilde (argument);

   if (!str_cmp (arg, "help"))
     {
        do_help (ch, "controls");
        return;
     }
   if (!str_cmp (arg, "save"))
     {
        save_sysdata (sysdata);
        send_to_char ("System Data Saved.\n\r", ch);
        return;
     }

   if (!str_cmp (arg, "mudname"))
     {
//        if (sysdata.mud_name)
//           DISPOSE (sysdata.mud_name);
        sysdata.mud_name = str_dup (argument);
        send_to_char ("Name set.\n\r", ch);
        return;
     }

  if (!str_cmp(arg, "saveflag"))
  {
	int x = get_saveflag( argument );

	if ( x == -1 )
	{
	    send_to_char( "Not a save flag.\n\r", ch );
	    show_saveflags(ch);
	}
	else
	{
	    TOGGLE_BIT( sysdata.save_flags, 1 << x );
	    send_to_char( "Ok.\n\r", ch );
	}
	return;
  }


   level = (int) atoi (argument);

   if (!str_cmp (arg, "savefrequency"))
     {
        sysdata.save_frequency = level;
        send_to_char ("Ok.\n\r", ch);
        return;
     }

   if (!str_cmp (arg, "exp"))
     {
        xp = atof (argument);
        sysdata.max_tot_exp = xp;
        stc ("Ok.\n\r", ch);
        return;
     }

   if( !str_cmp(arg, "maxitems"))
   {
      sysdata.max_items = level;
      stc ("Ok.\n\r", ch);
      return;
   }

}

void
do_rename (CHAR_DATA * ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  char new_name[MAX_INPUT_LENGTH];
  DESCRIPTOR_DATA *d;
  CHAR_DATA *victim;
  OBJ_DATA *obj;

  argument = one_argument (argument, arg);
  argument = one_argument (argument, new_name);

  if (IS_NPC (ch))
    return;

  if ((victim = get_char_room (ch, arg)) == NULL)
    {
      send_to_char ("Rename who?\n\r", ch);
      return;
    }

  if (new_name[0] == '\0')
    {
      send_to_char ("Rename them to what?\n\r", ch);
      return;
    }

  if (IS_NPC (victim))
    {
      send_to_char ("Use mset instead.\n\r", ch);
      return;
    }

  if (victim->desc == NULL || victim->desc->connected != CON_PLAYING)
    {
      send_to_char ("They are not available for a rename at the moment.\n\r",
                    ch);
      return;
    }
  if (IS_IMMORTAL (victim) && victim != ch)
    {
      send_to_char ("You are unable to rename Gods!\n\r", ch);
      return;
    }

  if (char_exists (FALSE, new_name))
    {
      send_to_char ("There is already a player saved by that name.\n\r", ch);
      return;
    }

  for (d = first_descriptor; d != NULL; d = d->next)
    {
      if (d->character != NULL && victim->desc != d)
        {
          if (!IS_NPC (d->character)
              && strlen (d->character->name) > 0
              && !str_cmp (d->character->name, new_name))
            {
              send_to_char ("There is already a player with that name.\n\r",
                            ch);
              return;
            }
        }
    }
  if (!check_parse_name (new_name))
    {
      send_to_char ("That is an illegal new name!\n\r", ch);
      return;
    }

  for (obj = object_list; obj != NULL; obj = obj->next)
    {
      if (strlen (obj->questowner) < 3)
        continue;

      if (str_cmp (victim->name, obj->questowner))
        continue;

      if (!str_cmp (victim->name, obj->questowner))
        {
          free_string (obj->questowner);
          obj->questowner = str_dup (capitalize (new_name));
        }
    }

  sprintf (buf, "%s%s", PLAYER_DIR, capitalize (victim->pcdata->switchname));
  free_string (victim->name);
  victim->name = str_dup (capitalize (new_name));
  free_string (victim->pcdata->switchname);
  victim->pcdata->switchname = str_dup (capitalize (new_name));
  unlink (buf);
  do_autosave (victim, "");
  sprintf (buf, "You have been renamed to %s.\n\r", capitalize (new_name));
  send_to_char (buf, victim);
  send_to_char ("Rename complete.\n\r", ch);
  return;
}

bool at_bank(CHAR_DATA *ch)
{
OBJ_DATA *obj;
  for (obj = ch->in_room->contents; obj; obj = obj->next_content)
  {   if(obj->item_type == ITEM_BANK)
        return TRUE;
  }
  for (obj = ch->carrying; obj; obj = obj->next_content)
  {   if(obj->item_type == ITEM_BANK)
        return TRUE;
  }
  return FALSE;
}

void do_balance(CHAR_DATA *ch, char *argument)
{
  char arg1[MSL];
  char arg_amt[MSL];
  char arg_type[MSL];
  char arg_vict[MSL];
  int amount;
  CHAR_DATA *victim, *wch;
  if (IS_NPC(ch))
     return;

  if (!at_bank(ch))
  {
     stc("You can only check your balance at a bank or atm.\n\r", ch);
     return;
  }

  if (!argument || argument[0] == '\0')
  {
     if (ch->pcdata->bank > 0)
        chprintf(ch, "You currently have %2.0f gold in your bank account.\n\r", ch->pcdata->bank);
     else
        stc("You currently have no gold in your account.\n\r", ch);
     if (ch->pcdata->bankqp > 0)
        chprintf(ch, "You currently have %2.0f qps in your bank account.\n\r", ch->pcdata->bankqp);
     else
        stc("You currently have no qps in your account.\n\r", ch);
     return;
  }

  argument = one_argument( argument, arg1);
  argument = one_argument( argument, arg_amt);
  argument = one_argument( argument, arg_type);
  one_argument( argument, arg_vict);

  if (str_cmp(arg1, "transfer"))
  {
     stc("Syntax: balance\n\r", ch);
     stc("        balance transfer <amount> <qp|gold> <person>\n\r", ch);
     return;
  }

  if ((!arg_amt || arg_amt[0] == '\0') || (!arg_type || arg_type[0] == '\0') || (!arg_vict || arg_vict[0] == '\0'))
  {
     stc("Syntax: balance\n\r", ch);
     stc("        balance transfer <amount> <qp|gold> <person>\n\r", ch);
     return;
  }

  if ( str_cmp(arg_type, "qp") && str_cmp(arg_type, "gold") )
  {
     stc("You can only transfer gold or qp.\n\r", ch);
     return;
  }

  if ( !is_number(arg_amt))
  {
     stc("Amount must be a number.\n\r", ch);
     return;
  }
  amount = atoi( arg_amt );

  victim = NULL;

  for ( wch = char_list; wch != NULL; wch = wch->next )
  {
     if(IS_NPC(wch)) continue;
     else if (!is_name( arg_vict, wch->name )) continue;
     else
        victim = wch;
  }
/*
  if (victim == NULL)
  {
     if(!jope_load_char(ch, arg_vict))
     {
        stc("Victim doesn't exist.\n\r", ch);
        return;
     }

     chprintf( ch, "transferring to %s", ch->pcdata->pfile->name);
     jope_free_char(ch);
  }
*/
}

void do_deposit(CHAR_DATA *ch, char *argument)
{
char arg2[MSL];
double amount;
bool depqp = FALSE;

  if (IS_NPC(ch))
     return;

  argument = one_argument(argument, arg2);

  if (!at_bank(ch))
  {
     stc("You can only check your balance at a bank or atm.\n\r", ch);
     return;
  }

  if (!str_cmp(argument, "qp"))
     depqp = TRUE;
  else if (!str_cmp(argument, "gold"))
     depqp = FALSE;
  else
  {
     stc("You can only deposit gold or qps in the bank.\n\r", ch);
     return;
  }
  if (!is_number(arg2) && str_cmp(arg2, "all"))
  {
     stc("Thats not a number.\n\r", ch);
     return;
  }

  if (!str_cmp(arg2, "all"))
  {
     if (!depqp)
        amount = ch->gold;
     else
        amount = ch->pcdata->quest;
  }
  else 
  {
     amount = atoi(arg2);
     if ((!depqp) && (amount > ch->gold))
     {
        stc("You don't have that much gold on you.\n\r", ch);
        return;
     }
     else 
        if ((depqp) && (amount > ch->pcdata->quest))
        {
           stc("You don't have that many qp on you.\n\r", ch);
           return;
        }
  }

  if (amount <= 0)
  {
     stc("You can only deposit a positive number.\n\r", ch);
     return;
  }

  if (!depqp)
  {
     ch->pcdata->bank += amount;
     ch->gold -= amount;
     chprintf(ch, "You deposit %2.0f gold into your account, your balance is now %2.0f.\n\r", amount, ch->pcdata->bank);
     return;
  }
  else
  {
     ch->pcdata->bankqp += amount;
     ch->pcdata->quest -= amount;
     chprintf(ch, "You deposit %2.0f gold into your account, your balance is now %2.0f.\n\r", amount, ch->pcdata->bankqp);
     
  }
}

void do_withdraw(CHAR_DATA *ch, char *argument)
{
double amount;
char arg2[MSL];
bool depqp = FALSE;

  if (IS_NPC(ch))
     return;

  if (!at_bank(ch))
  {
     stc("You can only check your balance at a bank or atm.\n\r", ch);
     return;
  }

  argument = one_argument(argument, arg2);

  if (!str_cmp(argument, "qp"))
     depqp = TRUE;
  else if (!str_cmp(argument, "gold"))
     depqp = FALSE;
  else
  {
     stc("You can only withdraw gold or qps from the bank.\n\r", ch);
     return;
  }

  if (!is_number(arg2))
  {
     stc("Thats not a number.\n\r", ch);
     return;
  }
  amount = atoi(arg2);

  if (amount <= 0)
  {
     stc("You can only withdraw a positive number.\n\r", ch);
     return;
  }

  if ((!depqp) && (amount > ch->pcdata->bank))
  {
     stc("You don't have that much gold in your account.\n\r", ch);
     return;
  }
  else
     if ((depqp) && (amount > ch->pcdata->bankqp))
     {
        stc("You don't have that many qps in your account.\n\r", ch);
        return;
     }

  if (!depqp)
  {
     ch->pcdata->bank -= amount;
     ch->gold += amount;
     chprintf(ch, "You withdraw %2.0f gold from your account, your balance is now %2.0f.\n\r", amount, ch->pcdata->bank);
  }
  else
  {
     ch->pcdata->bankqp -= amount;
     ch->pcdata->quest += amount;
     chprintf(ch, "You withdraw %2.0f qps from your account, your balance is now %2.0f.\n\r", amount, ch->pcdata->bankqp);
  }
}
/*
void arena_update()
{
  if (arena == FIGHT_START)
     if (--arena_timer <= 0)
        cleanup_arena();   
}

void cleanup_arena()
{
CHAR_DATA *challenger;
CHAR_DATA *challenged;
DESCRIPTOR_DATA *dlist;
char buf[MSL];

  for (dlist = first_descriptor; dlist; dlist = dlist->next)
  {
     if (dlist->character == NULL)
        continue;

     if (xIS_SET(dlist->character->act, PLR_CHALLENGER))
     {
        xREMOVE_BIT(dlist->character->act, PLR_CHALLENGER);
        dlist->character->challenged = NULL;
        challenger = dlist->character;
     }        

     if (xIS_SET(dlist->character->act, PLR_CHALLENGED))
     {
        xREMOVE_BIT(dlist->character->act, PLR_CHALLENGED);
        dlist->character->challenger = NULL;
        challenged = dlist->character;
     }        
  }
  if (!challenger || !challenged)
     sprintf(buf, "#0[#7Arena#0] The Arena reopens.\n\r");

  else sprintf(buf,"#0[#7Arena#0] #L%s's #7challenge goes unanswered as #R%s#7 cowers in fear.#n\n\r",challenger->name, challenged->name);
        do_info(NULL,buf );

arena = FIGHT_OPEN;
}
*/

void do_setstacker( CHAR_DATA *ch, char *argument)
{
char stacker;

   if (argument[0] == '\0')
   {
       chprintf(ch, "Stack Character is currently %c.\n\r", ch->pcdata->stackchar);
       return;
   }

   stacker = argument[0];

   if (!ispunct(stacker))
   {
      chprintf(ch, "%c isn't a symbol character\n\r", stacker);
      return;
   }

   if (stacker == '"' || stacker == '\'' )
   {
      stc("You can't use that symbol as your stackchar, as its used in string parsing.\n\r", ch);
      return;
   }

   if (stacker == ch->pcdata->speedchar)
   {
      stc("You're already using that symbol as your speedwalk character.\n\r", ch);
      return;
   }

   ch->pcdata->stackchar = stacker;
   chprintf(ch, "stack character set to %c\n\r", ch->pcdata->stackchar);

}

void do_setspeeder( CHAR_DATA *ch, char *argument)
{
char speeder;

   if (argument[0] == '\0')
   {
       chprintf(ch, "Speedwalk Character is currently %c.\n\r", ch->pcdata->speedchar);
       return;
   }

   speeder = argument[0];

   if (!ispunct(speeder))
   {
      chprintf(ch, "%c isn't a symbol character\n\r", speeder);
      return;
   }

   if (speeder == '"' || speeder == '\'' )
   {
      stc("You can't use that symbol as your speedwalk character, as its used in string parsing.\n\r", ch);
      return;
   }

   if (speeder == ch->pcdata->stackchar)
   {
      stc("You're already using that symbol as your stackchar.\n\r", ch);
      return;
   }

   ch->pcdata->speedchar = speeder;
   chprintf(ch, "speedwalk character set to %c\n\r", ch->pcdata->speedchar);

}

void do_cleanup(CHAR_DATA *ch, char *argument)
{
system("./cleanup");

}

void read_multilist()
{
  FILE *fp;
  int j;
  char word[MSL];

  if((fp=fopen(MULTILIST_FILE,"r"))==NULL)
  {
    bug("Cannot open Multilist file for reading",0);
    return;
  }

       multiplaylist_num = -1;

  for(j=0;;j++)
  {
    fscanf(fp,"%s\n",word);
      if(!str_cmp(word,"NULL"))
      {
        break;
      }

      strcpy (multiplaylist[j].word, word);
      multiplaylist_num = j;
   }
  fclose(fp);
return;
 }

void save_multilist()
{
  FILE *fp;
  int f= 0;

  if((fp=fopen(MULTILIST_FILE,"w"))==NULL)
  {
    bug("Cannot open Multilist file for writing",0);
    return;
  }
  if (multiplaylist_num >= 0)
  for(f = 0;f <=multiplaylist_num;f++)
  {
    fprintf(fp,"%s\n",multiplaylist[f].word);
  }
  fprintf(fp,"NULL NULL NULL~\n");
  fclose(fp);

}

void do_multiplay(CHAR_DATA *ch, char *argument)
{
  char buf[MSL];
  char buf2[MSL];
  char arg[MSL];
  char arg2[MSL];
  int i;
  int f;
  int x;
  int sex;

   if (argument[0] == '\0')
   {
     stc("syntax: multiplay add/remove <name>\n\r", ch);
     return;
   }
   argument=one_argument(argument,arg);
   argument=one_argument(argument,arg2);
   if(arg[0]=='\0')
   {
     stc("syntax: multiplay add/remove <word>\n\r", ch);
     stc("syntax: multiplay list\n\r", ch);
     return;
   }

    if(!str_cmp(arg,"add"))
    {
      if(arg2[0]=='\0')
      {
        stc("syntax: multiplay add|remove <word>\n\r", ch);
        return;
      }

      arg2[0] = UPPER(arg2[0]);

      for(sex = 0; sex <= multiplaylist_num; sex++)
      {
        if(!str_cmp(multiplaylist[sex].word, arg2))
        {
          stc("That name is already on the list.\n\r", ch);
          return;
        }
      }
      strcpy (multiplaylist[multiplaylist_num + 1].word, arg2);
      multiplaylist_num += 1;
      save_multilist();
      stc("Done.\n\r",ch);
      return;
    }

     else if(!str_cmp(arg,"remove"))
    {
       if(arg2[0]=='\0')
       {
         stc("syntax: multiplay add|remove <word>\n\r", ch);
         return;
       }

    arg2[0] = UPPER(arg2[0]);


    for(i=0;i<=multiplaylist_num;i++)
    {
      if(!str_cmp(multiplaylist[i].word,arg2))
      {
        for(f=i;f<=multiplaylist_num;f++)
        {
          multiplaylist[f]=multiplaylist[f+1];
        }
        multiplaylist_num--;
        save_multilist();
        stc("Done.\n\r",ch);
        return;
      }
    }
    }

     else if(!str_cmp(arg, "list"))
    {
      if (multiplaylist_num >= 0)
      {
	buf[0] = '\0';
        stc("Players allowed to multiplay:\n\r", ch);
        stc("#g----------------------------------------------------------#n\n\r\n\r", ch);
        for(x=0; x<=multiplaylist_num; x++)
        {
	  sprintf(buf2, "%10s", multiplaylist[x].word);
	  stc(buf2, ch);
	  if ((x+1) % 4 == 0)
          stc("\n\r", ch );
        }
       }
       else
        stc("\n\rNone.\n\r", ch);
        return;
     }
    else
    {
        stc("Choices are Add, Remove or list\n\r", ch);
        return;
    }
}

int check_multilist(CHAR_DATA *ch, CHAR_DATA *gch)
{
int i;
bool chfound = FALSE;
bool gchfound = FALSE;
if (IS_IMMORTAL(ch)) return 0;
for(i = 0; i <= multiplaylist_num; i++)
{

  if(!str_cmp(multiplaylist[i].word, ch->name))
  chfound = TRUE;

  if(!str_cmp(multiplaylist[i].word, gch->name))
  gchfound = TRUE;
}
  if (chfound && gchfound)
return 0;

  else
     return 1;
}

void add_contents( OBJ_DATA *obj, CHAR_DATA *ch )
{
    ch->carry_number += 1;
if (obj->next_content != NULL)
    add_contents( obj->next_content, ch );

if (obj->contains != NULL)
    add_contents( obj->contains, ch );
}

void subtract_contents( OBJ_DATA *obj, CHAR_DATA *ch )
{
    ch->carry_number -= 1;

   if (obj->next_content != NULL)
      subtract_contents( obj->next_content, ch );

if (obj->contains != NULL)
    subtract_contents( obj->contains, ch );
}

void retake_inventory( CHAR_DATA *ch )
{
   ch->carry_number = 0;
   if (!ch->carrying) return;

   count_item( ch, ch->carrying);
}

void count_item( CHAR_DATA *ch, OBJ_DATA *obj )
{
   ch->carry_number +=1;
   if (obj->next_content != NULL)
      count_item( ch, obj->next_content );

   if (obj->contains != NULL)
      count_item( ch, obj->contains );


}

void do_see_pfile( CHAR_DATA *ch, char *argument )
{
FILE *fp;
bool fMatch;
char arg[MSL];
char strsave[MSL];
char fingerbuf[7][MSL];
int fingernum[16];
int temp;
int x;
EXT_BV extra;

    if ( IS_NPC(ch) ) return;
    one_argument(argument,arg);

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( !char_exists(FALSE,argument) )
    {
        send_to_char( "Cannot find a player with that name.\n\r", ch );
        return;
    }

    for ( x = 0; x < 7; x++ )
    strcpy( fingerbuf[x], "");
    fclose( fpReserve );
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
            char *wordone;

            wordone = fread_word( fp );
            if ( !str_cmp( wordone, "#PLAYERS" ) )
	    {
	    for ( ; ; )
            {
            char *word;
	        word   = feof( fp ) ? "End" : fread_word( fp );
	        fMatch = FALSE;

            if ( !str_cmp( word, "End" ) )
                break;

            switch ( UPPER(word[0]) )
            {
            case '*':
                fMatch = TRUE;
                fread_to_eol( fp );
                break;

            case 'A':
              KEY( "Awin", fingernum[PF_AWINS], fread_number( fp ));
	      KEY( "Alos", fingernum[PF_ALOSSES], fread_number( fp ));
              break;

	    case 'B':
	      KEY( "Bounty", fingernum[PF_BOUNTY], fread_number( fp ));
	      break;

  	    case 'C':
	      FKEY( "Createtime", fingerbuf[PF_CREATETIME], fread_string( fp ));
	      KEY( "Class", fingernum[PF_CLASS], fread_number( fp ));
              break;

            case 'E':
 	      if (!str_cmp(word, "Extra"))
	      {
                  extra = fread_bitvector( fp );
	          fMatch = TRUE;
	      }
  	      break;

	    case 'G':
              KEY( "Generation", fingernum[PF_GENERATION], fread_number( fp ) );
	      break;

	   case 'L':
	     FKEY("Lasthost", fingerbuf[PF_LASTHOST], fread_string( fp ));
	     FKEY("Lasttime", fingerbuf[PF_LASTTIME], fread_string( fp ));
	     KEY("Level", fingernum[PF_LEVEL], fread_number( fp ));
	     KEY("Lstatus", fingernum[PF_LSTATUS], fread_number( fp ));
  	     break;

	   case 'M':
	     FKEY("Marriage", fingerbuf[PF_MARRIAGE], fread_string( fp ));
	     break;

	   case 'N':
             if ( !str_cmp( word, "Name" ) )
             {
	         strcpy( fingerbuf[PF_NAME], fread_string( fp ));
		 fMatch = TRUE;
	     }
	     break;

	   case 'P':
	     if( !str_cmp( word, "Played" ) )
	     {
	         temp = fread_number( fp );
 	         if (temp > 0)
 	             fingernum[PF_PLAYED] = (2*(temp/7200));
	         else
	 	     fingernum[PF_PLAYED] = 0;
	         fMatch = TRUE;
	         break;
             }

             if ( !str_cmp( word, "PkPdMkMd" ) )
             {
                 fingernum[PF_PKILL]       = fread_number( fp );
                 fingernum[PF_PDEATH]      = fread_number( fp );
                 fingernum[PF_MKILL]       = fread_number( fp );
                 fingernum[PF_MDEATH]      = fread_number( fp );
                 fMatch = TRUE;
                 break;
             }
	     KEY("Practice", fingernum[PF_PRACTICE], fread_number( fp ));
             break;

	case 'R':
	  KEY("Race", fingernum[PF_STATUS], fread_number( fp ));
	  KEY("RandMight", fingernum[PF_RANDMIGHT], fread_number( fp ));
	  break;

	case 'S':
	  KEY("Sex", fingernum[PF_SEX], fread_number( fp ));
          break;

        case 'T':
          if ( !str_cmp( word, "Title" ) )
          {
            strcpy( fingerbuf[PF_TITLE], fread_string( fp ));
            fMatch = TRUE;
          }
          break;
 	}

	if (!fMatch )
           do_new_fread_to_eol( fp );
    }
    }
	fflush( fp );
        fclose( fp );
  }
    fpReserve = fopen( NULL_FILE, "r" );

    
stc("----------------------------------------------------------------------------\n\r", ch);
    chprintf(ch, "%s %s\n\r", fingerbuf[PF_NAME], fingerbuf[PF_TITLE]);
stc("----------------------------------------------------------------------------\n\r", ch);
    chprintf(ch, "Last Connected ");
       if (IS_IMPLEMENTOR(ch))
           chprintf(ch, "from %s ", fingerbuf[PF_LASTHOST]);
	chprintf(ch, "at %s.\n\r", fingerbuf[PF_LASTTIME]);
stc("----------------------------------------------------------------------------\n\r", ch);
chprintf(ch, "Sex: %s. ", fingernum[PF_SEX] == SEX_MALE ? "Male" : "Female");
        if (fingernum[PF_SEX] == SEX_FEMALE)
        {
            if (xIS_SET(extra, EXTRA_PREGNANT)) other_age(ch,extra,TRUE,fingerbuf[PF_CONCEP]);
        }
other_age(ch, extra, FALSE, fingerbuf[PF_CREATETIME]);
    switch( fingernum[PF_LEVEL] )
    {
            default:
                chprintf(ch,"They are mortal with #C%d#n status,",fingernum[PF_STATUS]);
                break;
            case LEVEL_AVATAR:
            case LEVEL_APPRENTICE:
            case LEVEL_MAGE:
            case LEVEL_ARCHMAGE:
            {
                if (fingernum[PF_STATUS] < 1)
                    chprintf(ch,"They are a Piss Ant with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 4)
                    chprintf(ch,"They are a Squire with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 7)
                    chprintf(ch,"They are an Apprentice with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 10)
                    chprintf(ch,"They are a Scout with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 13)
                   chprintf(ch,"They are a Soldier with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 16)
                    chprintf(ch,"They are a Lieutenant with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 19)
                    chprintf(ch,"They are a Captain with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 22)
                    chprintf(ch,"They are a Guard with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 25)
                    chprintf(ch,"They are a Knight with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 28)
                    chprintf(ch,"They are a Ranger with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 31)
                    chprintf(ch,"They are a Hunter with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 34)
                    chprintf(ch,"They are an Assassin with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 37)
                    chprintf(ch,"They are a Lord with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 40)
                    chprintf(ch,"They are a Baron with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 43)
                    chprintf(ch,"They are a Duke with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 46)
                    chprintf(ch,"They are a Prince with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 51)
                    chprintf(ch,"They are a King with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 76)
                    chprintf(ch,"They are an Emperor with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 101)
                    chprintf(ch,"They are an Overlord with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 126)
                    chprintf(ch,"They are a Demigod with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 151)
                    chprintf(ch,"They are a Lesser God with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] < 198)
                    chprintf(ch,"They are a Mighty God with #C%d#n status, ",fingernum[PF_STATUS]);
                else if (fingernum[PF_STATUS] >= 199)
                    chprintf(ch,"They are the Reaper with #C%d#n status, ",fingernum[PF_STATUS]);
                break;
            }
            case LEVEL_BUILDER:
                chprintf(ch,"They are a Builder with #C%d#n status, ",fingernum[PF_STATUS]);
                break;
            case LEVEL_QUESTMAKER:
                chprintf(ch,"They are a Quest Maker with #c%d#n status, ",fingernum[PF_STATUS]);
                break;
            case LEVEL_ENFORCER:
                chprintf(ch,"They are an Enforcer with #C%d#n status, ",fingernum[PF_STATUS]);
                break;
            case LEVEL_JUDGE:
                chprintf(ch,"They are a Judge with #C%d#n status, ",fingernum[PF_STATUS]);
                break;
            case LEVEL_HIGHJUDGE:
                chprintf(ch,"They are a High Judge with #C%d#n status, ",fingernum[PF_STATUS]);
                break;
            case LEVEL_IMPLEMENTOR:
                chprintf(ch,"They are an Implementor with #C%d#n status, ",fingernum[PF_STATUS]);
                break;
        }

       chprintf(ch,"and have been playing for %d hours.\n\r",fingernum[PF_PLAYED]);

       if (strlen(fingerbuf[PF_MARRIAGE]) > 2)
        {
            if (xIS_SET(extra, EXTRA_MARRIED))
                chprintf(ch,"They are married to %s.\n\r",fingerbuf[PF_MARRIAGE]);
            else
                chprintf(ch,"They are engaged to %s.\n\r",fingerbuf[PF_MARRIAGE]);
        }
        chprintf(ch,"#RPlayer kills#n: #r%d, #RPlayer Deaths#n: #r%d.#n\n\r",fingernum[PF_PKILL],fingernum[PF_PDEATH]);
        chprintf(ch,"#yArena kills#n: %d, #yArena Deaths#n: %d.#n\n\r",fingernum[PF_AWINS], fingernum[PF_ALOSSES]);
        chprintf(ch,"#PMob Kills:#n #p%d,#P Mob Deaths#n: #p%d.#n\n\r",fingernum[PF_MKILL],fingernum[PF_MDEATH]);
        chprintf(ch,"#GClass#n: %s, #CGeneration#n: %d.\n\r",capitalize(flag_string(class_flags, fingernum[PF_CLASS])), fingernum[PF_GENERATION]);
        chprintf(ch, "#0Primal Energy#n: %d.\n\r", fingernum[PF_PRACTICE]);
        chprintf(ch, "#CLegend#n: %s.\n\r", fingernum[PF_LSTATUS] > 0 ? flag_string(legend_flags, fingernum[PF_LSTATUS]) : "Unknown");
        chprintf(ch, "#LBounty#n: #y%d qp.#n\n\r", fingernum[PF_BOUNTY]);
        chprintf(ch, "#GPower Rating#n: #0%d#n.\n\r",fingernum[PF_RANDMIGHT]);
stc("----------------------------------------------------------------------------\n\r", ch);

 }
bool check_speedwalk( CHAR_DATA *ch, char *argument )
{
int len;
int count;
char buf[MSL];

len = strlen( argument );

if (ch->fight_timer > 0)
{
chprintf(ch, "not with a fight timer!\n\r");
return FALSE;
}
     for ( count = 1; count < len; count++ )
     {
        switch (argument[count])
	{
	case 'n':
	case 'N':
	case 's':
	case 'S':
	case 'e':
	case 'E':
	case 'w':
	case 'W':
	case 'u':
	case 'U':
	case 'd':
	case 'D':
	   break;
	default:
	   chprintf(ch, "%c isn't a valid direction.\n\r", argument[count]);
	   return FALSE;
	   break;
        }
     }

     argument++;
     while ( *argument !=  '\0' )
     {
	sprintf(buf, "%c", *argument );
	interpret( ch, buf );
	argument++;
     }
     return TRUE;
}
	
void do_testcmd( CHAR_DATA *ch, char *argument )
{
int used[100];
int vnum;
int count;
int ignore;

  ignore = atoi( argument );

  for (count = 54000; count <= 59000; count += 50)
  {
        if (count == ignore)
	   continue;

        vnum = ((count - 53950) / 50);
        used[vnum-1] = 1;
  }
  for (count = 0; count < 100; count++)
  {
     if ( used[count] < 1 )
        log_string( "found: %d", ((count+1)*50)+53950);
  }
}

void do_nofollow( CHAR_DATA *ch, char *argument )
{
  if (xIS_SET(ch->extra, EXTRA_NOFOLLOW))
  {
     stc("You once again allow others to follow your lead.\n\r", ch);
     xREMOVE_BIT(ch->extra, EXTRA_NOFOLLOW);
     return;
  }

  else
  {
     stc("You no longer allow others to follow your lead.\n\r", ch);
     act("$n no longer allows anyone to follow in $s lead.\n\r",ch,NULL,NULL,TO_ROOM,FALSE);
     xSET_BIT(ch->extra, EXTRA_NOFOLLOW);
     die_follower( ch );
     return;
  }

}

void do_sochelp( CHAR_DATA *ch, char *argument )
{
int iSocial;
     iSocial = social_lookup (argument);
        stc( "\n\r", ch );
        stc( "------------------------------------------------------------------",ch);
chprintf (ch, "\n\rSocial: %s\n\r", social_table[iSocial].name );
        stc( "------------------------------------------------------------------",ch);
        stc( "\n\r\n\r    No argument given, character sees:\n\r", ch );
               act( social_table[iSocial].char_no_arg, ch, NULL, ch, TO_CHAR , FALSE   );
               stc( "\n\r     No argument given, others see:\n\r", ch );
               act( social_table[iSocial].others_no_arg, ch, NULL, supersnard, TO_CHAR , FALSE   );
               stc( "\n\r     Target found, character sees:\n\r", ch );
               act( social_table[iSocial].char_found, ch, NULL, supersnard, TO_CHAR , FALSE   ); 
               stc( "\n\r     Target found, others see:\n\r", ch );
               act( social_table[iSocial].others_found, ch, NULL, supersnard, TO_CHAR , FALSE   );
               stc( "\n\r     Target found, victim sees:\n\r", ch );
               act( social_table[iSocial].vict_found, ch, NULL, ch, TO_CHAR , FALSE   );
               stc( "\n\r     Target is character himself:\n\r", ch );
               act( social_table[iSocial].char_auto, ch, NULL, ch, TO_CHAR , FALSE   );
               stc( "\n\r     Target is character himself, others see:\n\r", ch );
               act( social_table[iSocial].others_auto, ch, NULL, ch, TO_CHAR , FALSE   );
                return; /* return right away, do not save the table */

}


void init_supersnard()
{
   ROOM_INDEX_DATA *office;

   supersnard = create_mobile(get_mob_index( 29 ));
   office = get_room_index ( 11 );
   char_to_room( supersnard, office );
}

void load_hint (FILE *fp, struct hint_type *hint)
{
hint->text =           fread_string (fp);
}


void load_hint_table ()
{
        FILE *fp;
        int i;


        fp = fopen (HINT_FILE, "r");

        if (!fp)
        {
                bug ("Could not open " HINT_FILE " for reading.",0);
                exit(1);
        }

        fscanf (fp, "%d\n", &maxHint);

        /* IMPORTANT to use malloc so we can realloc later on */

        hint_table = malloc (sizeof(struct hint_type) * (maxHint+1));

        for (i = 0; i < maxHint; i++)
                load_hint (fp,&hint_table[i]);

        /* For backwards compatibility */

        hint_table[maxHint].text = str_dup(""); /* empty! */

        fclose (fp);

}

void do_hints( CHAR_DATA *ch, char *argument )
{
char arg1[MSL];
int x;
argument = one_argument(argument, arg1);

  if (IS_NPC(ch))
     return;

  if (!arg1 || arg1[0] == '\0')
  {
     stc("Syntax: Hints list\n\r        hints add <hint>\n\r        hints delete ##\n\r", ch);
     return;
  }

  if (!str_cmp(arg1, "list"))
  {
     stc( "------- HINTS -------\n\r\n\r", ch);
     for (x = 0; x < maxHint; x++)
     {
        chprintf( ch, "%d: %s\n\r", x+1, hint_table[x].text);
     }
     return;
  }
  else if (!str_cmp(arg1, "save"))
  {
     save_hint_table();
     stc("Hints saved.\n\r", ch);
     return;
  }

  else if (!str_cmp(arg1, "delete"))
  {
     int i,j,togo;
     struct hint_type *new_table = malloc (sizeof(struct hint_type) * maxHint);

     if (!is_number(argument))
     {
        stc("What number do you want to delete?\n\r", ch);
        return;
     }
     togo = atoi(argument);
               if (!new_table)
                {
                        send_to_char ("Memory allocation failed. Brace for impact...\n\r",ch);
                        return;
                }

                /* Copy all elements of old table into new table, except the deleted hint */
                for (i = 0, j = 0; i < maxHint+1; i++)
                        if (i != togo-1) /* copy, increase only if copied */
                        {
                                new_table[j] = hint_table[i];
                                j++;
                        }

                free (hint_table);
                hint_table = new_table;

                maxHint--; /* Important :() */

                send_to_char ("That hint is history now.\n\r",ch);

  }
  else if (!str_cmp(arg1, "add"))
  {
     struct hint_type *new_table;
     if (!argument || argument[0] == '\0')
     {
        stc("syntax: hints add <hint to add>\n\r", ch);
        return;
     }
     maxHint++;
     new_table = realloc (hint_table, sizeof(struct hint_type) * maxHint + 1);
     if (!new_table) /* realloc failed */
     {
        send_to_char ("Memory allocation failed. Brace for impact.\n\r",ch);
        return;
     }

     hint_table = new_table;
     hint_table[maxHint-1].text = str_dup (argument);
     hint_table[maxHint].text = str_dup (""); /* 'terminating' empty string */
     stc( "New hint added.\n\r", ch);
  }

  save_hint_table();
  return;

}

void save_hint (const struct hint_type *h, FILE *fp)
{
fprintf(fp, "%s~\n", h->text);
}


void save_hint_table()
{
        FILE *fp;
        int i;

        fp = fopen (HINT_FILE, "w");

        if (!fp)
        {
                bug ("Could not open " HINT_FILE " for writing.",0);
                return;
        }

       fprintf (fp, "%d\n", maxHint);

        for ( i = 0 ; i < maxHint ; i++)
                save_hint (&hint_table[i], fp);

        fclose (fp);
}

void do_pload( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA d;
  bool isChar = FALSE;
  char name[MAX_INPUT_LENGTH];

  if (argument[0] == '\0')
  {
    send_to_char("Load who?\n\r", ch);
    return;
  }

  argument[0] = UPPER(argument[0]);
  argument = one_argument(argument, name);

  /* Dont want to load a second copy of a player who's allready online! */
  if ( get_char_world( ch, name ) != NULL )
  {
    send_to_char( "That person is allready connected!\n\r", ch );
    return;
  }

  isChar = load_char_obj(&d, name); /* char pfile exists? */

  if (!isChar)
  {
    send_to_char("Load Who? Are you sure? I cant seem to find them.\n\r", 
ch);
    return;
  }

  d.character->desc     = NULL;
  d.character->next     = char_list;
  char_list             = d.character;
  d.connected           = CON_PLAYING;
//  reset_char(d.character);

  /* bring player to imm */
  if ( d.character->in_room != NULL )
  {
    char_to_room( d.character, ch->in_room); /* put in room imm is in */
  }

  act( "$n has pulled $N from the pattern!",
        ch, NULL, d.character, TO_ROOM, FALSE );

}

/** Function: do_punload
  * Descr   : Returns a player, previously 'ploaded' back to the void from
  *           whence they came.  This does not work if the player is 
actually 
  *           connected.
  * Returns : (void)
  * Syntax  : punload (who)
  * Written : v1.0 12/97
  * Author  : Gary McNickle <gary@dharvest.com>
  */
void do_punload( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char who[MAX_INPUT_LENGTH];

  argument = one_argument(argument, who);

  if ( ( victim = get_char_world( ch, who ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }

  /** Person is legitametly logged on... was not ploaded.
   */
  if (victim->desc != NULL)
  {
    send_to_char("I dont think that would be a good idea...\n\r", ch);
    return;
  }

  if (victim->was_in_room != NULL) /* return player and pet to orig room 
*/
  {
    char_to_room(victim, victim->was_in_room);
  }

  save_char_obj(victim);
  do_quit(victim,"");

  act("$n has released $N back to the Pattern.",
       ch, NULL, victim, TO_ROOM, FALSE);
}


bool jope_load_char(CHAR_DATA *ch, char *arg)
{
  DESCRIPTOR_DATA *dummy;

  if (ch->pcdata->pfile != NULL)
  {
    send_to_char("Your already editing a pfile.\n\r", ch);
    return FALSE;
  }

  CREATE( dummy, DESCRIPTOR_DATA, 1 );

  if (load_char_obj(dummy, arg)) ch->pcdata->pfile = dummy->character;
  else send_to_char("No such pfile.\n\r", ch);
  LINK( dummy, first_descriptor, last_descriptor, next, prev );
  if (ch->pcdata->pfile == NULL) return FALSE;
  return TRUE;
}

void jope_free_char(CHAR_DATA *ch)
{
  CHAR_DATA *victim;

  if ((victim = ch->pcdata->pfile) == NULL)
  {
    send_to_char("You are not editing a char.\n\r", ch);
    return;
  }
  save_char_obj(victim);
  free_char(victim);
  ch->pcdata->pfile = NULL;
  return;
}

