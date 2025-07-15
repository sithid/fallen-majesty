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

#define CHANGES_FILE    "changes.dat"
extern          int                     MAX_CHANGES;
 
typedef struct  changes_data             CHANGES_DATA;


void load_changes args( (void) );
void save_changes args( (void) );
void delete_change args( (int num) );
char *  wordwrap   args( ( char *oldstring ) );
#define  NULLSTR( str )  ( str == NULL || str[0] == '\0' )

struct changes_data
{
    char *         change;
    char *         coder;
    char *         date;
};


DECLARE_DO_FUN( do_changes);
DECLARE_DO_FUN( do_addchange);
DECLARE_DO_FUN( do_chsave);
