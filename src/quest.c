//=============================================================\\
//							       ||	
//	Quest System					       ||
//							       ||
//		For Fallen Majesty			       ||
//							       ||
//				By Ryan Wilcox		       ||
//							       ||
//=============================================================//

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

void do_crusade( CHAR_DATA *ch, char *argument )
{
	char arg1 [MAX_STRING_LENGTH];
	char arg2 [MAX_STRING_LENGTH];
	char arg3 [MAX_STRING_LENGTH];
	char buf  [MAX_STRING_LENGTH];
	char endchar[3];

	if (IS_NPC(ch))
	{
		send_to_char("Not while switched.\n\r",ch);
		return;
	}
	if arg1
