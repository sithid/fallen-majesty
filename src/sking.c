/***************************************************************************
#
#
#  sking.c and all contents are sole property of Jason White, who can do
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
#include "merc.h"
#include "garou.h"
#include "olc.h"



