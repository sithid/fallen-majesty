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

void sort_quote(void);
void save_quote();

typedef struct quote_data QUOTE_DATA;

struct quote_data
{
  long time;
  char *desc;
  char *author;
  QUOTE_DATA *next;
  QUOTE_DATA *prev;
};
QUOTE_DATA *first_quote;
QUOTE_DATA *last_quote;
int max_quote;
QUOTE_DATA *blank_quote()
{
  QUOTE_DATA *quote;
//  QUOTE_DATA *last;
  
  CREATE(quote, QUOTE_DATA, 1);
    if(!quote)return 0;
  quote->time=0;
  quote->desc=str_dup("");
  quote->author=str_dup("");

  LINK( quote, first_quote, last_quote, next, prev);
  return quote;
}
void do_quote(CHAR_DATA *ch,char *argument)
{
  char arg1[MSL];
  char arg2[MSL];
  char buf [MSL];
  QUOTE_DATA *quote;
  int x, i, letters, space, newspace, line;
  char rdesc[MAX_STRING_LENGTH];

  if (ch->level < LEVEL_JUDGE)
  {
    bool listall=0;
    bool header=0;
    int num=0;

    if(!str_cmp(argument,"all")||argument[0]=='\0')
    {
      listall=1;
    }

    for(quote=first_quote;quote;quote=quote->next)
    {
      if(listall)
      {
        if(!header)
        {
          stc("#GQuotes#n\n\r",ch);
          stc("#C-----------------------#n\n\r",ch);
          header=1;
        }


        i=0;
        letters=0;
        space=0;
        newspace=0;
        line=0;


        sprintf(buf, "%s", quote->desc);

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


        sprintf(buf,"#G[#C%3d#G]#7 %-12.12s #4%s\n\r",
          num,quote->author, rdesc);
        stc(buf,ch);
      }
      num++;
    }

  return;
  }
  argument=one_argument(argument,arg1);
  argument=one_argument(argument,arg2);

  if(arg1[0]=='\0')
  {
    stc("Syntax:  quote <add> message author\n\r"
        "         quote <remove> <number>\n\r"
	"         quote list         \n\r"
        "         NOTE: Message must be in quotations only.\n\r", ch);
    return;
  }

  if(!str_cmp(arg1,"add"))
  {
    if(argument[0]=='\0')
    {
      stc("What quote do you wish to post?\n\r",ch);
      return;
    }

    sprintf(buf,"%s",(char *)ctime(&current_time));
    buf[strlen(buf)-1]='\0';


    quote=blank_quote();
    quote->time=current_time;
    quote->author=str_dup(argument);
    quote->desc=str_dup(arg2);
    stc("Ok.\n\r",ch);
    save_quote();
    return;
  }
  if(!str_cmp(arg1,"remove"))
  {
    int i=0;
    int j;
   
    if(!is_number(arg2)){do_quote(ch,"");return;}

    j=atoi(arg2);

    for(quote=first_quote;quote;quote=quote->next)
    {
      if(i++==j)
      {
//	QUOTE_DATA *old;
	  UNLINK( quote, first_quote, last_quote, next, prev );
	  DISPOSE( quote );
	  stc("Ok.\n\r",ch);
	  save_quote();
	  return;
      }
    }
    stc("Not Found.\n\r",ch);
    return;
  }
  if(!str_cmp(arg1,"list"))
  {
    bool listall=0;
    bool header=0;
    int num=0;

    if(!str_cmp(arg2,"all")||arg2[0]=='\0')
    {
      listall=1;
    }

    for(quote=first_quote;quote;quote=quote->next)
    {
      if(listall)
      {
        if(!header)
        {
          stc("#GQuotes#n\n\r",ch);
	  stc("#C-----------------------#n\n\r",ch);
	  header=1;
	}


        i=0;
        letters=0;
        space=0;
        newspace=0;
        line=0;


        sprintf(buf, "%s", quote->desc);

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


	sprintf(buf,"#G[#C%3d#G]#7 %-12.12s #4%s\n\r",
	  num,quote->author, rdesc);
	stc(buf,ch);
      }
      num++;
    }	
  }
}

void load_quote()
{
  FILE *fp;
  QUOTE_DATA *quote;
  long i;
  int count = 0;

  fp=fopen(QUOTE_LIST,"r");
  if(!fp)
  {
    bug("NO QUOTE_LIST",0);
    return;
  }
  
  for(;;)
  {
    i=fread_number(fp);
    if(i==-1)break;
  
    quote=blank_quote();
    quote->time = i;
    quote->author=fread_string(fp);
    quote->desc=fread_string(fp);
    count++;
  }


  fclose(fp);
  max_quote = count-1;
}

void save_quote()
{
  FILE *fp;
  QUOTE_DATA *quote;
  int count = 0;
  
  fp=fopen(QUOTE_LIST,"w");
  if(!fp)
  {
    bug("Could not open QUOTE_LIST for writing.",0);
    return;
  }
  
  for(quote=first_quote;quote;quote=quote->next)
  {
    fprintf(fp,"%ld %s~ %s~\n",
	quote->time,quote->author,quote->desc);
    count++;
  }
  fprintf(fp,"-1\n");
  fclose(fp);
  max_quote = count-1;
}

void sort_quote()
{
  QUOTE_DATA *quote;
  int total=0;
  int i;
  QUOTE_DATA **t_arr;
  QUOTE_DATA *temp;

  for(quote=first_quote;quote;quote=quote->next)total++;

  CREATE(*t_arr, QUOTE_DATA, total);

  i=0;

  for(quote=first_quote;quote;quote=quote->next)
  {
    t_arr[i++]=quote;
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

  first_quote=t_arr[0];

  DISPOSE(t_arr);

}

void quote( CHAR_DATA *ch )
{
int number;
int i = 0;
QUOTE_DATA *quote;

   number = number_range( 0, max_quote);

   for (quote = first_quote;quote;quote=quote->next)
   {
      if(i==number)
      {
         chprintf(ch, "\n\r%s\n\r - %s\n\r",
           quote->desc,
           quote->author);
         return;
      }
   i++;
   }
}
