/***************************************************************************
 *  Shadow Wars: the Beginning v1.5                                        *
 ---------------------------------------------------------------------------
 *  You are allowd to change and modify this code, but you must comply     *
 *  with all the licenses by LAW. Check the doc directory for more info.   *
 *  Shadow Wars: the Beginning created by Xkilla                           *
 *  (c)1999-2001 Xkilla 'Ryan Waliany' (ryan@scv.net)                      *
 ---------------------------------------------------------------------------
 *  Changes snippet by: Xkilla. This is a snippet, you may modify it, but  *
 *  you must leave in Xkilla's credit.                                     *
 * Special Thanks to: Dreimas for fixing a memory leak                     *.
 ***************************************************************************/
/* Based on Xkilla's Social Code */ 
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>

#include "merc.h"

//Local Functions
int                     MAX_CHANGES;
struct  changes_data * changes_table;
void 	do_changesinfo args ( ( CHAR_DATA *ch, char * argument ) ); 

char *short_date( time_t time )
{
    static char buf[20];
    char        tmp[20];
    char        *date;

    if ( time < 0 )
    {
        time = current_time;
 }
 
 date = ctime( &time );

    tmp[0] = date[4];
    tmp[1] = date[5];
    tmp[2] = date[6];
    tmp[3] = '\0';
 if          ( !str_cmp( tmp, "jan" ) ) { buf[0] = '0'; buf[1] = '1'; }
    else if     ( !str_cmp( tmp, "feb" ) ) { buf[0] = '0'; buf[1] = '2'; }
    else if     ( !str_cmp( tmp, "mar" ) ) { buf[0] = '0'; buf[1] = '3'; }
    else if     ( !str_cmp( tmp, "apr" ) ) { buf[0] = '0'; buf[1] = '4'; }
    else if     ( !str_cmp( tmp, "may" ) ) { buf[0] = '0'; buf[1] = '5'; }
    else if     ( !str_cmp( tmp, "jun" ) ) { buf[0] = '0'; buf[1] = '6'; }
    else if     ( !str_cmp( tmp, "jul" ) ) { buf[0] = '0'; buf[1] = '7'; }
    else if     ( !str_cmp( tmp, "aug" ) ) { buf[0] = '0'; buf[1] = '8'; }
    else if     ( !str_cmp( tmp, "sep" ) ) { buf[0] = '0'; buf[1] = '9'; }
    else if     ( !str_cmp( tmp, "oct" ) ) { buf[0] = '1'; buf[1] = '0'; }
    else if     ( !str_cmp( tmp, "nov" ) ) { buf[0] = '1'; buf[1] = '1'; }
    else if     ( !str_cmp( tmp, "dec" ) ) { buf[0] = '1'; buf[1] = '2'; }
    else        { buf[3] = '9'; buf[4] = '9'; }

    buf[2] = '/';

    if ( date[8] == ' ' )
        buf[3] = '0';
    else
        buf[3] = date[8];

    buf[4] = date[9];

    buf[5] = '/';

    buf[6] = date[22];
     buf[7] = date[23];

    return buf;
}
void load_changes(void)
{
  	FILE *fp;
        int i;
        char filename [ MAX_STRING_LENGTH ];

    sprintf( filename, "../changes/%s", CHANGES_FILE );
    
    if ( !(fp = fopen( filename, "r")) )
    {
        bug( "Could not open filename for reading.", 0 );
        perror( filename );
        return;
    }

    fscanf( fp, "%d\n", &MAX_CHANGES );

    changes_table = malloc( sizeof( CHANGES_DATA) * (MAX_CHANGES+1) );

    for (i = 0; i < MAX_CHANGES; i++)
    {
        char letter;

        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );

        if ( str_cmp( fread_word( fp ), "Change" ) )
            break;
        changes_table[i].change = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "Coder" ) )
            break;
        changes_table[i].coder = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "Date" ) )
            break;
        changes_table[i].date = fread_string( fp );
       
    }
    fclose(fp);
    return; /* just return */
}
void do_addchange(CHAR_DATA *ch, char *argument )
{
    CHANGES_DATA * new_table;
    char buf[MSL];
    
    if ( IS_NPC( ch ) )
        return;
    
    if ( argument[0] == '\0' )
    {
        send_to_char( "Syntax: Addchange: $Change\n\r", ch );
        send_to_char( "#RType #n'changes'#R to view the list.#n\n\r", ch );
        return;
    }

    MAX_CHANGES++;
    new_table = realloc( changes_table, sizeof( CHANGES_DATA ) *(MAX_CHANGES+1) );

    if (!new_table) /* realloc failed */
    {
        send_to_char ("Memory allocation failed. Brace for impact.\n\r",ch);
        return;
    }

    changes_table = new_table;
    
    changes_table[MAX_CHANGES-1].change  = str_dup( argument );
    changes_table[MAX_CHANGES-1].coder   = str_dup( ch->name );
    changes_table[MAX_CHANGES-1].date    = short_date(current_time);
    
    send_to_char("Changes Created.\n\r",ch);
    send_to_char("Type 'changes' to see the changes.\n\r",ch);
    save_changes();
    sysdata.curr_change = current_time;
    ch->pcdata->last_change = current_time;
    save_sysdata(sysdata);
  sprintf(buf, "#GThere has been a #ychange#G in the #RMUD#y! #Gtype: #yhelp changes#G or #ychanges#G!");
	do_changesinfo(NULL,buf); 
    return;
}

void save_changes(void)
{
    char  filename [ MAX_INPUT_LENGTH ];
    FILE *fp;
    int i;

    sprintf( filename, "../changes/%s", CHANGES_FILE );
    if ( !(fp = fopen( filename,"w")) )
    {
        perror( filename );
        return;
    } 

    fprintf( fp, "%d\n", MAX_CHANGES ); 
        for (i = 0; i < MAX_CHANGES; i++) {
        fprintf (fp, "Change    %s~\n", changes_table[i].change);
  	fprintf (fp, "Coder     %s~\n", changes_table[i].coder);
  	fprintf (fp, "Date      %s~\n", changes_table[i].date);
        }
 
        fclose(fp);
        return;
}

void delete_change(int iChange)
{
    int i,j;
    CHANGES_DATA * new_table;

    new_table = malloc( sizeof( CHANGES_DATA ) * MAX_CHANGES );

    if( !new_table )
    {
       return;
    }
    
    for ( i= 0, j = 0; i < MAX_CHANGES+1; i++)
    {
        if( i != iChange )
        {
            new_table[j] = changes_table[i];
            j++;
        }
    }
    
    free( changes_table );
    changes_table = new_table;

    MAX_CHANGES--;
    save_changes();    
    return;
}
 
void do_chsave( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if (!ch->desc || *arg1 == '\0')
    {
      send_to_char("Syntax: chsave load/save\n\r",ch);
      send_to_char("Syntax: chsave delete (change number)\n\r",ch);
      send_to_char("Syntax: chsave rename (change number) 'New Text'\n\r",ch);
      return;
    }
 
   if ( strcmp(arg1,"load")== 0 )
      {
        load_changes( );
        send_to_char("Changes Loaded.\n\r",ch);
        return;
      }
   else if ( strcmp(arg1,"save")== 0 )
      {
        save_changes( );
        send_to_char("Changes Saved.\n\r",ch);
        return;
      }
   else if ( strcmp(arg1, "rename")== 0)
      {
      int num;
        if ( NULLSTR(arg2) || !is_number( arg2) || NULLSTR(argument)  )
        {
            send_to_char("#yFor chsave rename, you must choose a change number.#n\n\r",ch);
            send_to_char("#1Syntax#2:#4 chsave rename (change number) 'New Change'#n\n\r",ch);
            return;
        }
        num = atoi( arg2 );
        if ( num < 1 || num > MAX_CHANGES )
        {
	char buf[MAX_STRING_LENGTH];
	sprintf(buf,"Valid changes are from 1 to %d.\n\r",MAX_CHANGES);
            send_to_char( buf,ch );
            return;
        }
        changes_table[num-1].change  = str_dup(argument );   
        save_changes( );
        send_to_char("Change renamed.\n\r",ch);
        return;
      }
   else if ( strcmp(arg1, "delete")== 0)
      {
        int num;
        
        if ( NULLSTR(arg2) || !is_number( arg2 ) )
        {
            send_to_char("#yFor chsave delete, you must provide a change number.#n\n\r",ch);
            send_to_char("#1Syntax#2:#4 chsave delete (change number)#n\n\r",ch);
            return;
        }

        num = atoi( arg2 );
        if ( num < 0 || num > MAX_CHANGES )
        {
	char buf[MAX_STRING_LENGTH];
	sprintf(buf,"Valid changes are from 0 to %d.\n\r",MAX_CHANGES);
            send_to_char( buf,ch );
            return;
        }
        delete_change( num );
        send_to_char("Change deleted.\n\r",ch);
        return;
      }
      else
      {
      send_to_char("Syntax: chsave load/save.\n\r",ch);
      send_to_char("Syntax: chsave delete (change number)\n\r",ch);
      return;
      }
}
char *line_indent (char *text, int wBegin, int wMax)
{
   static char buf[MAX_STRING_LENGTH]; 
   char *ptr;
   char *ptr2;
   int count = 0;
   bool stop = FALSE;
   int wEnd;
   
   buf[0] = '\0';
   ptr = text;
   ptr2 = buf;
 
   while (!stop)
   {
      if (count == 0)
      {
         if (*ptr == '\0')
            wEnd = wMax - wBegin;
         else if (strlen (ptr) < (wMax - wBegin))
            wEnd = wMax - wBegin;
         else
         {
            int x = 0;
      
            while (*(ptr + (wMax - wBegin - x)) != ' ')
               x++;
            wEnd = wMax - wBegin - (x - 1);
            if (wEnd < 1)
               wEnd = wMax - wBegin;
         }
      }
      if (count == 0 && *ptr == ' ')
         ptr++;
      else if (++count != wEnd)
      {
         if ((*ptr2++ = *ptr++) == '\0')
            stop = TRUE;
      }
      else if (*ptr == '\0')
      {
         stop = TRUE;
         *ptr2 = '\0';
      }
      else
      {
         int k;
      
         count = 0;
         *ptr2++ = '\n';  
         *ptr2++ = '\r';
         for (k = 0; k < wBegin; k++)
            *ptr2++ = ' ';
      }
   }
   return buf;
}  

void do_changes(CHAR_DATA *ch, char *argument)
{
    char  arg[MIL], arg2[MIL], buf[MSL], *test;
    int   i, page=0, maxpage=0, today;
    
    argument = one_argument(argument, arg);
    argument = one_argument(argument, arg2);
        
    if (IS_NPC(ch)) return;
    if(MAX_CHANGES < 1) return;
         
    i = 0;
    test = short_date(current_time);
    today = 0;
           
        
    for ( i = 0; i < MAX_CHANGES; i++)
    {
        if (!str_cmp(test,changes_table[i].date)) today++;
    }

    if (is_number(arg)) page = atoi(arg);
        maxpage = (MAX_CHANGES+9)/10;
        
    if (page> 0){
        if (page > maxpage){
            sprintf(buf, "Please pick a page 1 thru %d\n\r", maxpage);
            send_to_char(buf, ch);
            return;
        }
        page *= 10;
    }
 
    send_to_char("#y[NUM ] Coder        Date\n\r",ch );
    send_to_char("#0--------------------------------------------------------------------------------\n\r",ch);
    if (!str_cmp(arg, "search")){
        int dsp=0;
    
        if (arg2[0] == '\0'){
            send_to_char("If you're going to search at least search for something.\n\r", ch);
            return;
        }
    
        for (i=0; i< MAX_CHANGES; i++)
        {
           if (!str_infix(arg2, changes_table[i].change)|| !str_infix(arg2, changes_table[i].coder)){
        sprintf(buf,"#G[#C%4d#G] #7%-12s#w #G*#L%-6s\n\r                    #G%-62s#n\n\r",
                     (++dsp),
                     changes_table[i].coder ,
                     changes_table[i].date,
		     line_indent (changes_table[i].change, 20, 79));
                send_to_char(buf, ch);   
            }
        }
        if (dsp == 0)
            send_to_char("There are no changes matching what you entered.\n\r", ch);
        
        return;
    }
            
    for (i = 0; i < MAX_CHANGES; i++)
    {
      if (page == 0 &&
        str_prefix(test, changes_table[i].date) &&
        i < (MAX_CHANGES-10)) continue;
         
      if (page > 0 &&
        (page > 0 && (i < (page-10) || i >= page))) continue;

        sprintf(buf,"#G[#C%4d#G] #7%-12s#w #G*#L%-6s\n\r                    #G%-62s#n\n\r",
                     (i+1),
                     changes_table[i].coder ,
                     changes_table[i].date,
		     line_indent (changes_table[i].change, 20, 79));
        send_to_char(buf, ch);
    }
    ch->pcdata->last_change = current_time;
    send_to_char("#0--------------------------------------------------------------------------------\n\r",ch);
    sprintf(buf, "#7There are a total of [#R%d#7] changes in the database.", MAX_CHANGES);
    cent_to_char( buf, ch );
    sprintf(buf, "#7There have been a total of [#R%d#7] new change%s that have been added today.", today, today> 1? "s": "");
    cent_to_char( buf, ch );
    sprintf(buf, "#7Also see: 'changes <#R1#0-#r%d#7>' to list each page.",(MAX_CHANGES+9)/10);
    cent_to_char(buf, ch);
    cent_to_char("#7To search all changes use: 'changes search <word>'", ch);
    send_to_char("#0--------------------------------------------------------------------------------\n\r",ch);
}
      

void do_ochanges(CHAR_DATA *ch, char *argument) 
{
        char arg[MAX_INPUT_LENGTH]; 
        char buf[MAX_STRING_LENGTH];
        char *test;
        int today;
        int i, start, end;
 
        argument = one_argument( argument, arg ); 
        if (IS_NPC(ch)) return;
	i = 0;
        test = short_date(current_time);
        today = 0;
for ( i = 0; i < MAX_CHANGES; i++)
{
if (!str_cmp(test,changes_table[i].date))
today++;
}
if (is_number(arg) && is_number(argument))
{
   if ((start = atoi(arg)) > (end = atoi(argument)))
   {
      start = atoi(argument);
      end = atoi(arg);
   }
   if (start > MAX_CHANGES) start = MAX_CHANGES - 14;
   start = start - 1;
   if (end > MAX_CHANGES+1) end = MAX_CHANGES+1;
   if ((end - start) > 15) end = (start + 15);

        sprintf(buf, "#yNo.     Coder               Date        Change\n\r" );        
send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        for (i = start; i < end; i++) {
//        sprintf(buf, " #4%3d#G- #C%-12s #R*#c%-8s   #o%-45s\n\r",
        sprintf(buf, " #G[#C%3d#G]  #7%-12s     #G*#L%-8s\n\r                               #G%-45s\n\r",
	i+1,changes_table[i].coder ,
        changes_table[i].date,
        changes_table[i].change);
        send_to_char(buf,ch);
        }
sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);

        sprintf(buf, "#4There are %d out of %d changes in the database listed.#n\n\r", end - start, MAX_CHANGES);
        send_to_char(buf,ch);
        send_to_char("#4Also see: Changes 'current' for a list of current changes. #n\n\r",ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#1There is a total of %d new changes that were added today.#n\n\r", today);      
        send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        send_to_char("\n\r", ch );
        ch->pcdata->last_change = current_time;

}
else if (!str_cmp(arg,"new"))
        {
        sprintf(buf, "#yNo.     Coder               Date        Change\n\r" );
        send_to_char(buf,ch);
 sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        for (i = 0; i < MAX_CHANGES; i++) {
	if (str_cmp(test,changes_table[i].date)) continue;
//        sprintf(buf, " #4%3d#G- #C%-12s #R*#c%-8s   #o%-45s\n\r",
        sprintf(buf, " #G[#C%3d#G]  #7%-12s	#G*#L%-8s\n\r                               #G%-45s\n\r",
        i+1,changes_table[i].coder ,        
	changes_table[i].date,
        changes_table[i].change);
        send_to_char(buf,ch);
        }
sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#4There is a total of %d changes in the database.#n\n\r", MAX_CHANGES);
        send_to_char(buf,ch);
        send_to_char("#4Also see: Changes 'current' for a list of current changes. #n\n\r",ch);
sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#1There is a total of %d new changes that were added today.#n\n\r", today);
        send_to_char(buf,ch);
 sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        send_to_char("\n\r", ch );
        ch->pcdata->last_change = current_time;
        }
      else if (!str_cmp(arg,"all"))
	{
        sprintf(buf, "#yNo.     Coder               Date        Change\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        for (i = 0; i < MAX_CHANGES; i++) {
//        sprintf(buf, " #4%3d#G- #C%-12s #R*#c%-8s   #o%-45s\n\r",
        sprintf(buf, " #G[#C%3d#G]  #7%-12s     #G*#L%-8s\n\r                               #G%-45s\n\r",        
i+1,changes_table[i].coder ,        
changes_table[i].date,
        changes_table[i].change);
        send_to_char(buf,ch);
        }
sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);        
sprintf(buf, "#4There is a total of %d changes in the database.#n\n\r", MAX_CHANGES);
        send_to_char(buf,ch);
        send_to_char("#4Also see: Changes 'current' for a list of current changes. #n\n\r",ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#1There is a total of %d new changes that were added today.#n\n\r", today);      
        send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        send_to_char("\n\r", ch );
        ch->pcdata->last_change = current_time;
	}
      else if (!str_cmp(arg,"current"))
        {
	if ( MAX_CHANGES < 15 )
	{
        sprintf(buf, "#yNo.     Coder               Date        Change\n\r" );        
send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        for (i = 0; i < MAX_CHANGES; i++) {
//        sprintf(buf, " #4%3d#G- #C%-12s #R*#c%-8s   #o%-45s\n\r",
        sprintf(buf, " #G[#C%3d#G]  #7%-12s     #G*#L%-8s\n\r                               #G%-45s\n\r",        
i+1,changes_table[i].coder ,        
changes_table[i].date,
        changes_table[i].change);
        send_to_char(buf,ch);
        }
sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#4There is a total of %d changes in the database.#n\n\r", MAX_CHANGES);
        send_to_char(buf,ch);
        send_to_char("#4Also see: Changes 'current' for a list of current changes. #n\n\r",ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#1There is a total of %d new changes that were added today.#n\n\r", today);      
        send_to_char(buf,ch);
        sprintf(buf,"#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        send_to_char("\n\r", ch );
        ch->pcdata->last_change = current_time;
	}
	else
	{
        sprintf(buf, "#yNo.     Coder               Date        Change\n\r" );        
send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        for (i = (MAX_CHANGES - 15); i < MAX_CHANGES; i++) {
//        sprintf(buf, " #4%3d#G- #C%-12s #R*#c%-8s   #o%-45s\n\r",
        sprintf(buf, " #G[#C%3d#G]  #7%-12s     #G*#L%-8s\n\r                               #G%-45s\n\r",
        
i+1,changes_table[i].coder ,        
changes_table[i].date,
        changes_table[i].change);
        send_to_char(buf,ch);
        }
sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#4There are 15 out of %d changes in the database listed.#n\n\r", MAX_CHANGES);
        send_to_char(buf,ch);
        send_to_char("#4Also see: Changes 'all' for a complete list.#n\n\r",ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#1There is a total of %d new changes that were added today.#n\n\r", today);      
        send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        send_to_char("\n\r", ch );
        ch->pcdata->last_change = current_time;
	}
        }
      else
        { 
        if ( MAX_CHANGES < 15 )
        {
        sprintf(buf, "#yNo.     Coder               Date        Change\n\r" );        
send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        for (i = 0; i < MAX_CHANGES; i++) {
//        sprintf(buf, " #4%3d#G- #C%-12s #R*#c%-8s   #o%-45s\n\r",
        sprintf(buf, " #G[#C%3d#G]  #7%-12s     #G*#L%-8s\n\r                               #G%-45s\n\r",
	i+1,changes_table[i].coder ,
        changes_table[i].date,
        changes_table[i].change);
        send_to_char(buf,ch);
        }
sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);

        sprintf(buf, "#4There is a total of %d changes in the database.#n\n\r", MAX_CHANGES);
        send_to_char(buf,ch);
        send_to_char("#4Also see: Changes 'current' for a list of current changes. #n\n\r",ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#1There is a total of %d new changes that were added today.#n\n\r", today);      
        send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        send_to_char("\n\r", ch );
        ch->pcdata->last_change = current_time;
        }
        else
        {
        sprintf(buf, "#yNo.     Coder               Date        Change\n\r" );
        
send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        for (i = (MAX_CHANGES - 15); i < MAX_CHANGES; i++) {
//        sprintf(buf, " #4%3d#G- #C%-12s #R*#c%-8s   #o%-45s\n\r",
        sprintf(buf, " #G[#C%3d#G]  #7%-12s     #G*#L%-8s\n\r                               #G%-45s\n\r",        
i+1,changes_table[i].coder ,        
	changes_table[i].date,
        changes_table[i].change);
        send_to_char(buf,ch);
        }
sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
	sprintf(buf, "#4There are 15 out of %d changes in the database listed.#n\n\r", MAX_CHANGES);
        send_to_char(buf,ch);
	send_to_char("#4Also see: Changes 'all' for a complete list.#n\n\r",ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        sprintf(buf, "#1There is a total of %d new changes that were added today.#n\n\r", today);
        send_to_char(buf,ch);
        sprintf(buf, "#0-----------------------------------------------------------------------#n\n\r" );
        send_to_char(buf,ch);
        send_to_char("\n\r", ch );
        ch->pcdata->last_change = current_time;
	}
	} 
        return;
}

