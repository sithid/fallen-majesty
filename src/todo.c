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
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>

void sort_todo(void);
void save_todo();

typedef struct todo_data TODO_DATA;

struct todo_data
{
  long  time;
  char *date;
  char *desc;
  char *owner;
  TODO_DATA *next;
  TODO_DATA *prev;
};
TODO_DATA *first_todo;
TODO_DATA *last_todo;

TODO_DATA *blank_todo()
{
  TODO_DATA *todo;
  
  CREATE(todo, TODO_DATA, 1);
    if(!todo)return 0;
  todo->time=0;  
  todo->date=str_dup("");
  todo->desc=str_dup("");
  todo->owner=str_dup("");

  LINK( todo, first_todo, last_todo, next, prev);
  return todo;
}
void do_todo(CHAR_DATA *ch,char *argument)
{
  char arg1[MIL];
  char buf [MSL];
  TODO_DATA *todo;
  int x, i, letters, space, newspace, line;
  char rdesc[MAX_STRING_LENGTH];
  argument=one_argument(argument,arg1);

  if(arg1[0]=='\0')
  {
    stc("Syntax:  todo <add/remove> <desc/number>\n\r"
	"         todo claim        <number>\n\r",ch);
    if(get_trust(ch)>=12)
    stc("         todo assign       <number> <person>\n\r",ch);
    stc("\n\r"
	"         todo list         <person>\n\r"
	"         todo list         all\n\r",ch);
    return;
  }

  if(!str_cmp(arg1,"add"))
  {
    if (strcasecmp(ch->name, "Jank") 
       && strcasecmp(ch->name, "Rand")
       && strcasecmp(ch->name, "Tei")
       && strcasecmp(ch->name, "Snard")
       && strcasecmp(ch->name, "Huw"))
    {
        stc("You can't add to todo, use the idea board\n\rif we like it we will add it to todo.", ch);
        return;
    }

    if(argument[0]=='\0')
    {
      stc("What project do you wish to post?\n\r",ch);
      return;
    }

    sprintf(buf,"%s",(char *)ctime(&current_time));
    buf[strlen(buf)-1]='\0';


    todo=blank_todo();
    todo->desc=str_dup(argument);
    todo->owner=str_dup("None");
    todo->time=current_time;
    todo->date=str_dup(buf);
    stc("Ok.\n\r",ch);
    save_todo();
    return;
  }
  if(!str_cmp(arg1,"remove"))
  {
    int i=0;
    int j;
   
    if(!is_number(argument)){do_todo(ch,"");return;}

    j=atoi(argument);

    for(todo=first_todo;todo;todo=todo->next)
    {
      if(i++==j)
      {
	TODO_DATA *old;
	if(todo==first_todo)
	{
     if (strcasecmp(ch->name, "Jank") 
       && strcasecmp(ch->name, "Rand")
       && strcasecmp(ch->name, "Huw")
       && strcasecmp(ch->name, "Snard"))
       if( strcasecmp(todo->owner, ch->name) )
    {
        stc("You aren't authorized to remove from the todo list.\n\r", ch);
        return;
    }

	  first_todo=first_todo->next;
	  DISPOSE(todo);
	  stc("Ok.\n\r",ch);
	  save_todo();
	  return;
	}
        for(old=first_todo;old->next!=todo;old=old->next);
	{
     if (strcasecmp(ch->name, "Jank") 
       && strcasecmp(ch->name, "Rand")
       && strcasecmp(ch->name, "Huw")
       && strcasecmp(ch->name, "Snard"))
       if( strcasecmp(todo->owner, ch->name) )
    {
        stc("You aren't authorized to remove from the todo list.\n\r", ch);
        return;
    }
	  old->next=todo->next;
          DISPOSE(todo);
	  stc("Ok.\n\r",ch);
          save_todo();
	  return;
	}
      }
    }
    stc("Not Found.\n\r",ch);
    return;
  }
  if(!str_cmp(arg1,"claim"))
  {
    int i=0;
    int j=0;

    if(!is_number(argument)){do_todo(ch,"");return;}

    j=atoi(argument);
    
    for(todo=first_todo;todo;todo=todo->next)
    {
      if(i++==j)
      {
        todo->owner=str_dup(ch->name);
	stc("Ok.\n\r",ch);
        save_todo();
	return;
      }
    }
    stc("Not Found.\n\r",ch);
    return;
  }
  if(!str_cmp(arg1,"assign"))
  {
    char arg3[MIL];
    int i=0;
    int j=0;

    if (strcasecmp(ch->name, "Jank") 
       && strcasecmp(ch->name, "Rand")
       && strcasecmp(ch->name, "Huw")
       && strcasecmp(ch->name, "Snard"))
    {
        stc("You can't assign to others, use claim instead.", ch);
        return;
    }
    argument=one_argument(argument,arg3);

    if(!is_number(arg3)){do_todo(ch,"");return;}

    j=atoi(arg3);

    for(todo=first_todo;todo;todo=todo->next)
    {
      if(i++==j)
      {
	todo->owner=str_dup(argument);
        stc("Ok.\n\r",ch);
        save_todo();
	return;
      }
    }
    stc("Not Found.\n\r",ch);return;
  }
  if(!str_cmp(arg1,"list"))
  {
    bool listall=0;
    bool header=0;
    int num=0;

    if(!str_cmp(argument,"all")||argument[0]=='\0')
    {
      listall=1;
    }

    for(todo=first_todo;todo;todo=todo->next)
    {
      if(listall||!str_cmp(argument,todo->owner))
      {
        if(!header)
        {
          stc("#GTo Do#n\n\r",ch);
	  stc("#C-----------------------#n\n\r",ch);
	  header=1;
	}


        i=0;
        letters=0;
        space=0;
        newspace=0;
        line=0;


        sprintf(buf, "%s", todo->desc);

        for (i=0; i<strlen(buf); i++)
        {
                if (buf[i]==' ')
                {
                        space=i;
                        newspace=letters;
                }

                if (line > 55)
                {
                        i=space;
                        letters=newspace;
                        rdesc[letters++]='\r';
                        rdesc[letters++]='\n';
                        line=0;
                        for (x=0;x<22;x++)
                           rdesc[letters++]= ' ';
                }
                else if (!(buf[i]==' ' && buf[i+1]==' '))
                {
                        rdesc[letters]=buf[i];
                        letters++; /* Index for rdesc; i is the index for buf */
                        line++; /* Counts number of characters on this line */
                }
                rdesc[letters+1]='\0';
        }


	sprintf(buf,"#G[#C%3d#G]#7 %-12.12s #4%s\n\r                      #G%s#n\n\r",
	  num,todo->owner,todo->date, rdesc);
	stc(buf,ch);
      }
      num++;
    }	
  }
}

void load_todo()
{
  FILE *fp;
  TODO_DATA *todo;
  long i;

  fp=fopen(TODO_LIST,"r");
  if(!fp)
  {
    bug("NO TODO_LIST",0);
    return;
  }
  
  for(;;)
  {
    i=fread_number(fp);
    if(i==-1)break;
  
    todo=blank_todo();
    todo->time=i;
    todo->date=fread_string(fp);
    todo->owner=fread_string(fp);
    fread_string(fp);
    todo->desc=fread_string(fp);
  }


  fclose(fp);
}

void save_todo()
{
  FILE *fp;
  TODO_DATA *todo;
  
  fp=fopen(TODO_LIST,"w");
  if(!fp)
  {
    bug("Could not open TODO_LIST for writing.",0);
    return;
  }
  
  for(todo=first_todo;todo;todo=todo->next)
  {
    fprintf(fp,"%ld %s~ %s~ %s~ %s~\n",
	todo->time,todo->date,todo->owner," ",todo->desc);
  }
  fprintf(fp,"-1\n");
  fclose(fp);
}

void sort_todo()
{
  TODO_DATA *todo;
  int total=0;
  int i;
  TODO_DATA **t_arr;
  TODO_DATA *temp;

  for(todo=first_todo;todo;todo=todo->next)total++;

  CREATE(*t_arr, TODO_DATA, total);

  i=0;

  for(todo=first_todo;todo;todo=todo->next)
  {
    t_arr[i++]=todo;
  }

  {
    int k,j;
    for(j=1;j<i;++j)
    {
      for(k=i-1;k>=j;--k)
      {
        if(t_arr[k-1]->time>t_arr[k]->time)
        {
	  temp=t_arr[k-1];
  	  t_arr[k-1]=t_arr[k];
	  t_arr[k]=temp;
        }
      }
    }
  }

  for(i=0;i<total;i++)
  {
    if(i+1==total)
      t_arr[i]->next=0;
    else
      t_arr[i]->next=t_arr[i+1];
  }

  first_todo=t_arr[0];

  DISPOSE(t_arr);

}

