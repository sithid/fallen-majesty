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
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#if !defined( WIN32 )
#include <sys/time.h>
#endif
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#if !defined(WIN32)
#include <unistd.h>
#endif

#if defined( WIN32 )
#include <winsock.h>
#include <sys/timeb.h> /*for _ftime(), uses _timeb struct*/
#endif
/*
#if defined(__GNUC__) && defined(sun) && defined(unix) && !defined(__svr4__)
 extern void perror(const char *);
 extern double drand48(void);
 extern void srand48(long seedval);
 extern long random(void);
 extern void srandom(int seed);
 #endif
*/

#include "merc.h"


/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif


void check_class args(( CHAR_DATA *ch ));
void save_config args(( void ));

void init_signals();
void sig_handler(int sig);
/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix) || defined(WIN32)
#include <signal.h>
#if !defined(WIN32)
#include <unistd.h>
#include <sys/resource.h>	/* for RLIMIT_NOFILE */
#endif
#endif

#if defined(apollo)
#undef __attribute
#endif

/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const	char	echo_off_str	[] = { '\0' };
const	char	echo_on_str	[] = { '\0' };
const	char 	go_ahead_str	[] = { '\0' };
#endif

#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
void show_string args((DESCRIPTOR_DATA *d, char *input ));

#endif

#if	defined( WIN32 )
const   char echo_off_str	[] = { '\0' };
const   char echo_on_str	[] = { '\0' };
const   char go_ahead_str	[] = { '\0' };
void    gettimeofday    args( ( struct timeval *tp, void *tzp ) );
void show_string args((DESCRIPTOR_DATA *d, char *input ));
#endif

/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(apollo)
#include <unistd.h>
void	bzero		args( ( char *b, int length ) );
#endif

#if	defined(__hpux)
int	accept		args( ( int s, void *addr, int *addrlen ) );
int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname,
 				const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fcntl.h>
#endif

#if	defined(linux)
/* mercpoint: commented out to enable compiling
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
*/
int	close		args( ( int fd ) );
//int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
//int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
//int	listen		args( ( int s, int backlog ) );
/* kavirpoint: put this in for nin site.
int	read		args( ( int fd, char *buf, int nbyte ) );
*/
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
/* kavirpoint: put this in for nin site.
int	write		args( ( int fd, char *buf, int nbyte ) );
*/
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};
#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static	long			theKeys	[4];

int	gettimeofday		args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined(MIPS_OS)
extern	int		errno;
#endif

#if	defined(MSDOS)
int	gettimeofday	args( ( struct timeval *tp, void *tzp ) );
int	kbhit		args( ( void ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * This includes Solaris SYSV as well.
 */
#if defined(sun)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#if defined(SYSV)
int	setsockopt	args( ( int s, int level, int optname, 
                            const char *optval, int optlen ) );
#else 
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * Global variables.
 */
DESCRIPTOR_DATA *   first_descriptor;	/* First descriptor		*/
DESCRIPTOR_DATA *   last_descriptor;	/* Last descriptor		*/

//DESCRIPTOR_DATA *   descriptor_free;	/* Free list for descriptors	*/
//DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
int                 num_descriptors;    /* Number of players online     */
time_t		    current_time;	/* Time of this pulse		*/
bool clan_remove(char *name, CLAN_DATA *clan) ;
char		    str_boot_time[MAX_INPUT_LENGTH];
char		    crypt_pwd[MAX_INPUT_LENGTH];
int		    arena;
int		    doxlimit=0 ;

/* Colour scale char list - Calamar */

char *scale[SCALE_COLS] = {
	L_RED,
	L_BLUE,
	L_GREEN,
	YELLOW
};

/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif

#if defined(unix) || defined( WIN32 )
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	new_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_kickoff		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void	bust_a_prompt		args( ( DESCRIPTOR_DATA *d ) );
void     bust_a_header          args((DESCRIPTOR_DATA *d));

int port,control;

int main( int argc, char **argv )
{
    struct timeval now_time;
    bool fCopyOver = FALSE;

    /*
     * Memory debugging if needed.
     *
#ifndef MEMWATCH
#error "You really, really don't want to run this without memwatch. Trust me."
#endif
*/
//Explanation
/*
What this does is, it makes The port and control global, therefore
when you reexecute the mud, you can send the same parameters. And
if it remembers the control, it will be able to reload the control
and sockets. It passes the argv[3], argv[] is a each parameter
a program is given. Argc is the count of the parameters that are given.
Sorry, this may be hard to follow for some people.
*/

#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

#ifdef RLIMIT_NOFILE
#ifndef min
# define min(a,b)     (((a) < (b)) ? (a) : (b))
#endif
        { 
        struct  rlimit rlp;
        (void)getrlimit(RLIMIT_NOFILE, &rlp);
        rlp.rlim_cur=min(rlp.rlim_max,FD_SETSIZE);
        (void)setrlimit(RLIMIT_NOFILE, &rlp);
	}
#endif

    num_descriptors             = 0;
    first_descriptor            = NULL;
    last_descriptor             = NULL;

    /*
     * Init time and encryption.
     */
    gettimeofday( &now_time, NULL );
    current_time = (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );
    strcpy( crypt_pwd, "Don't bother." );

    /*
     * Macintosh console initialization.
     */
#if defined(macintosh)
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif

    /*
     * Reserve one channel for our use.
     */
    if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
    {
	perror( NULL_FILE );
	exit( 1 );
    }

    /*
     * Get the port number.
     */
    port = 9010;
    if ( argc > 1 )
    {
        if ( !is_number( argv[1] ) )
        {
            fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
            exit( 1 );
        }
        else if ( ( port = atoi( argv[1] ) ) <= 1024 )
        {
            fprintf( stderr, "Port number must be above 1024.\n" );
            exit( 1 );
        }

        /* Are we recovering from a copyover? */
        if (argv[2] && argv[2][0])
        {
                fCopyOver = TRUE;
                control = atoi(argv[3]);
        }
        else
                fCopyOver = FALSE;

}
    /*
     * Run the game.
     */
#if defined(macintosh) || defined(MSDOS)
    boot_db( );
    log_string( "Dystopia is ready to rock." );
    game_loop_mac_msdos( );
#endif

#if defined(unix) || defined(WIN32)
  	if (!fCopyOver) /* We have already the port if copyover'ed */
 		control = init_socket (port);
 	boot_db (fCopyOver);
/*
#ifdef CRASHGUARD    
init_signals();   
#endif
*/
    arena = FIGHT_OPEN;

    load_jason();

    sprintf( log_buf, "Dystopia is ready to rock on port %d.", port );
    log_string( log_buf );

    game_loop_unix( control );
#if !defined( WIN32 )
    close( control );
#else
    closesocket( control );
    WSACleanup();
#endif
#endif

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



#if defined(unix) || defined(WIN32)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x = 1;
    int fd;

#if !defined(WIN32)
    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }
#else
    WORD    wVersionRequested = MAKEWORD( 1, 1 );
    WSADATA wsaData;
    int err = WSAStartup( wVersionRequested, &wsaData ); 
    if ( err != 0 )
    {
	perror( "No useable WINSOCK.DLL" );
	exit( 1 );
    }

    if ( ( fd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        perror( "Init_socket: socket" );
	exit( 1 );
    }
#endif

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close( fd );
	exit( 1 );
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;
#if !defined(WIN32)
    sa.sin_family   = AF_INET;
#else
    sa.sin_family   = PF_INET;
#endif
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror( "Init_socket: bind" );
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	exit( 1 );
    }

    if ( listen( fd, 3 ) < 0 )
    {
	perror( "Init_socket: listen" );
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	exit( 1 );
    }

    return fd;
}
#endif



#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos( void )
{
    struct timeval last_time;
    struct timeval now_time;
    static DESCRIPTOR_DATA dcon;

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /*
     * New_descriptor analogue.
     */
    dcon.descriptor	= 0;
    dcon.connected	= CON_GET_NAME;
    dcon.host		= str_dup( "localhost" );
    dcon.outsize	= 2000;
    dcon.outbuf		= alloc_mem( dcon.outsize );
    dcon.next		= descriptor_list;
    descriptor_list	= &dcon;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( &dcon, help_greeting+1, 0 );
	else
	    write_to_buffer( &dcon, help_greeting  , 0 );
    }

    /* Main loop */
    while ( !merc_down )
    {
	DESCRIPTOR_DATA *d;

	/*
	 * Process input.
	 */
	for ( d = first_descriptor; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

                /* OLC */
                if ( d->showstr_point )
                    show_string( d, d->incomm );
                else
                if ( d->pString )
                    string_add( d->character, d->incomm );
                else
		switch( d->connected )
		{
	     	        default:
 			nanny( d, d->incomm );
			break;
		   case CON_PLAYING:
                        if ( !run_olc_editor( d ) )
                            interpret( d->character, d->incomm );
			break;
		   case CON_EDITING:
			edit_buffer( d->character, d->incomm );
			break;
		  }

                    if ( !d->multi_comm )
		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = first_descriptor; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Busy wait (blargh).
	 */
	now_time = last_time;
	for ( ; ; )
	{
	    int delta;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( dcon.character != NULL )
		    dcon.character->timer = 0;
		if ( !read_from_descriptor( &dcon ) )
		{
		    if ( dcon.character != NULL )
			save_char_obj( d->character );
		    dcon.outtop	= 0;
		    close_socket( &dcon );
		}
#if defined(MSDOS)
		break;
#endif
	    }

	    gettimeofday( &now_time, NULL );
	    delta = ( now_time.tv_sec  - last_time.tv_sec  ) * 1000 * 1000
		  + ( now_time.tv_usec - last_time.tv_usec );
	    if ( delta >= 1000000 / PULSE_PER_SECOND )
		break;
	}
	last_time    = now_time;
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix) || defined(WIN32)

void excessive_cpu(int blx)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next;

	if ( !IS_NPC(vch) )
	{
	    send_to_char("Mud frozen: Autosave and quit.  The mud will reboot in 2 seconds.\n\r",vch);
	    interpret( vch, "quit" );
	}
    }
    exit(1);
}

void game_loop_unix( int control )
{
    static struct timeval null_time;
    struct timeval last_time;

#if !defined(WIN32)
    signal( SIGPIPE, SIG_IGN );
#endif
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
/* kavirpoint
	maxdesc	= control * 2;
*/
	for ( d = first_descriptor; d != NULL; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
            if ( d == last_descriptor )
              break;
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    new_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = first_descriptor; d != NULL; d = d_next )
	{
	    d_next = d->next;   
            d->multi_comm = FALSE;

	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character )
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = first_descriptor; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

                /* OLC */
                if ( d->showstr_point )
                    show_string( d, d->incomm );
                else
                if ( d->pString )
                    string_add( d->character, d->incomm );
                else 
		switch( d->connected )
			  {
			   default:
 				nanny( d, d->incomm );
				break;
			   case CON_PLAYING:
                        if ( !run_olc_editor( d ) )
				interpret( d->character, d->incomm );
				break;
			   case CON_EDITING:
				edit_buffer( d->character, d->incomm );
				break;
			  }

                    if ( !d->multi_comm )
		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = first_descriptor; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	    &&   FD_ISSET(d->descriptor, &out_set) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
#if !defined(WIN32)
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}
#else
	{
	    int times_up;
	    int nappy_time;
	    struct _timeb start_time;
	    struct _timeb end_time;
	    _ftime( &start_time );
	    times_up = 0;

	    while( times_up == 0 )
	    {
		_ftime( &end_time );
		if ( ( nappy_time =
		      (int) ( 1000 *
			     (double) ( ( end_time.time - start_time.time ) +
				       ( (double) ( end_time.millitm -
						   start_time.millitm ) /
					1000.0 ) ) ) ) >=
		    (double)( 1000 / PULSE_PER_SECOND ) )
		  times_up = 1;
		else
		{
		    Sleep( (int) ( (double) ( 1000 / PULSE_PER_SECOND ) -
				  (double) nappy_time ) );
		    times_up = 1;
		}
	    }
	}
#endif

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



 void init_descriptor (DESCRIPTOR_DATA *dnew, int desc)
 {
 	static DESCRIPTOR_DATA d_zero;
 
 	*dnew = d_zero;
 	dnew->descriptor = desc;
 	dnew->character = NULL;
 	dnew->connected = CON_GET_NAME;
 	dnew->showstr_head = str_dup ("");
 	dnew->showstr_point = 0;
 	dnew->pEdit = NULL;			/* OLC */
 	dnew->pString = NULL;		/* OLC */
 	dnew->editor = 0;			/* OLC */
 	dnew->outsize = 2000;
	CREATE( dnew->outbuf, char, dnew->outsize );
 	
 }

#if defined(unix) || defined(WIN32)
void new_descriptor( int control )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    BAN_DATA *pban;
    struct sockaddr_in sock;
    struct hostent *from;
    int desc;
    int size;


    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

#if !defined(WIN32)
    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }
#endif

    /*
     * Cons a new descriptor.
     */
    CREATE( dnew, DESCRIPTOR_DATA, 1 );

    init_descriptor (dnew, desc);

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = str_dup( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
	sprintf( log_buf, "Sock.sinaddr:  %s", buf );
        if (str_cmp("150.113.68.212", buf)) {
        from = gethostbyaddr( (char *) &sock.sin_addr,
            sizeof(sock.sin_addr), AF_INET );
        dnew->host = str_dup( from ? from->h_name : buf );
        }
        else
          dnew->host = str_dup(buf);
    }
	
    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    for ( pban = first_ban; pban != NULL; pban = pban->next )
    {
	if ( !str_suffix( pban->name, dnew->host ) )
	{
	    write_to_descriptor( desc,
		"Your site has been banned from this Mud.\n\r", 0 );
	    close( desc );
	    STRFREE( dnew->host );
	    DISPOSE( dnew->outbuf );
	    DISPOSE( dnew );
	    return;
	}
    }
	log_string( log_buf ); 
/*
	if( !str_suffix( "mars.sunset.net", dnew->host) 
          || !str_suffix( "mix2.Boston.cw.net", dnew->host)
          || !str_suffix( "txol.net", dnew->host)
          || !str_suffix( "xerxes.mudhaven.com", dnew->host)
          || !str_suffix( "cyrus.mudhaven.com", dnew->host)
          || !str_suffix( "localhost", dnew->host)
          || !str_suffix( "gst.aloha.net", dnew->host)
          || !str_suffix( "mudprovider.com", dnew->host)
          || !str_suffix( "cybertours.com", dnew->host)
          || !str_suffix( "powersurfr.com", dnew->host)
          || !str_suffix( "twcny.rr.com", dnew->host)
          || !str_suffix( "blca.blazenet.net", dnew->host)
	  || !str_suffix( "sunset.net", dnew->host))
	{
            write_to_descriptor( desc,
                "Your site has been banned from this Mud.\n\r", 0 );
            close( desc );
            free_string( dnew->host );
	    DISPOSE( dnew->outbuf );
	    DISPOSE( dnew );
            return;
        }
*/

    /*
     * Init descriptor data.
     */
    if ( !last_descriptor && first_descriptor )
    {
        DESCRIPTOR_DATA *d;

        bug( "New_descriptor: last_desc is NULL, but first_desc is not! ...fixing" );
        for ( d = first_descriptor; d; d = d->next )
           if ( !d->next )
                last_descriptor = d;
    }

    LINK( dnew, first_descriptor, last_descriptor, next, prev );

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );
    }

    return;
}
#endif


void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;
    DESCRIPTOR_DATA *d;
    bool DoNotUnlink = FALSE;

    if ( dclose->outtop > 0 )
        process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
       write_to_buffer( dclose->snoop_by,
            "Your victim has left the game.\n\r", 0 );
    }

    if ( dclose->character != NULL &&
        (dclose->connected == CON_PLAYING || dclose->connected == 
        CON_EDITING) &&
        IS_NPC(dclose->character) ) do_return(dclose->character,"");

        for ( d = first_descriptor; d != NULL; d = d->next )
        {
            if ( d->snoop_by == dclose )
                d->snoop_by = NULL;
        }
   if ( dclose->original )
    {
        if ( ( ch = dclose->character ) != NULL )
          do_return(ch, "");
        else
        {
          bug( "Close_socket: dclose->original without character %s",
                (dclose->original->name ? dclose->original->name : "unknown") );
          dclose->character = dclose->original;
          dclose->original = NULL;
        }
    }

    ch = dclose->character;

    /* sanity check :( */
    if ( !dclose->prev && dclose != first_descriptor )
    {
        DESCRIPTOR_DATA *dp, *dn;
        bug( "Close_socket: %s desc:%p != first_desc:%p and desc->prev = NULL!",
                ch ? ch->name : dclose->host, dclose, first_descriptor );
        dp = NULL;
        for ( d = first_descriptor; d; d = dn )
        {
           dn = d->next;
           if ( d == dclose )
           {
                bug( "Close_socket: %s desc:%p found, prev should be:%p, fixing.",
                    ch ? ch->name : d->host, dclose, dp );
                dclose->prev = dp;
                break;
           }
           dp = d;
        }
        if ( !dclose->prev )
        {
            bug( "Close_socket: %s desc:%p could not be found!.",
                    ch ? ch->name : dclose->host, dclose );
           DoNotUnlink = TRUE;
        }
    }
    if ( !dclose->next && dclose != last_descriptor )
    {
        DESCRIPTOR_DATA *dp, *dn;
        bug( "Close_socket: %s desc:%p != last_desc:%p and desc->next = NULL!",
                ch ? ch->name : d->host, dclose, last_descriptor );
        dn = NULL;
        for ( d = last_descriptor; d; d = dp )
        {
           dp = d->prev;
           if ( d == dclose )
          {
                bug( "Close_socket: %s desc:%p found, next should be:%p, fixing.",                    ch ? ch->name : d->host, dclose, dn );
                dclose->next = dn;
                break;
           }
           dn = d;
        }
        if ( !dclose->next )
        {
            bug( "Close_socket: %s desc:%p could not be found!.",
                    ch ? ch->name : dclose->host, dclose );
            DoNotUnlink = TRUE;
        }
    }

    if ( dclose->character ) {
        do_channels(dclose->character, "quiet");
        sprintf( log_buf, "Closing link to %s.", ch->name );
        log_string( log_buf );
        if ( dclose->connected == CON_PLAYING
        ||   dclose->connected == CON_EDITING )
        {
            act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM, FALSE );
            if ( xIS_SET(ch->act,PLR_CHALLENGER) && arena == FIGHT_START )
            {
               char buf[MAX_STRING_LENGTH];

               xREMOVE_BIT(ch->act,PLR_CHALLENGER);
               xREMOVE_BIT(ch->challenged->act,PLR_CHALLENGED);
               ch->challenged->challenger = NULL;
               ch->challenged = NULL;
               arena = FIGHT_OPEN;
	       sprintf(buf, "[Arena] %s has lost $s link. Arena is OPEN.", ch->name);
            }
            ch->desc = NULL;
            
        }
        else
        {
            /* clear descriptor pointer to get rid of bug message in log 
*/
            dclose->character->desc = NULL;
            free_char( dclose->character );
        }
    }


    if ( !DoNotUnlink )
    {
        /* make sure loop doesn't get messed up */
        if ( d_next == dclose )
          d_next = d_next->next;
        UNLINK( dclose, first_descriptor, last_descriptor, next, prev );
    }

    close( dclose->descriptor );
    STRFREE( dclose->host );
    DISPOSE( dclose->outbuf );
    DISPOSE( dclose );

    --num_descriptors;
    return;
}


void old_close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;
    DESCRIPTOR_DATA *dt;
    bool DoNotUnlink = FALSE;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    if ( dclose->character != NULL && 
	(dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING) &&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");
/*
    if ( dclose->character != NULL 
	&& dclose->connected == CON_PLAYING
	&& !IS_NPC(dclose->character)
	&& dclose->character->pcdata != NULL
	&& dclose->character->pcdata->obj_vnum != 0
	&& dclose->character->pcdata->chobj != NULL)
	    extract_obj(dclose->character->pcdata->chobj);
*/
    {
	DESCRIPTOR_DATA *d;

	for ( d = first_descriptor; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
 	/* If ch is writing note or playing, just lose link otherwise clear char */
 	if ( (dclose->connected == CON_PLAYING) || 
 	  ((dclose->connected >= CON_NOTE_TO) && 
 	   (dclose->connected <= CON_NOTE_FINISH)))
	{
            if (xIS_SET(ch->extra, EXTRA_OSWITCH)) do_humanform(ch,"");
	    if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
		    	act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM, FALSE );
            if ( xIS_SET(ch->act,PLR_CHALLENGER) && arena == FIGHT_START )
            {
               char buf[MAX_STRING_LENGTH];

               xREMOVE_BIT(ch->act,PLR_CHALLENGER);
               xREMOVE_BIT(ch->challenged->act,PLR_CHALLENGED);
               ch->challenged->challenger = NULL;
               ch->challenged = NULL;
               arena = FIGHT_OPEN;
     sprintf(buf, "[Arena] %s has lost $s link. Arena is OPEN.", ch->name);
 {
 DESCRIPTOR_DATA *d;
 for ( d = first_descriptor; d; d = d->next )
  
     if( d->connected == CON_PLAYING || d->connected == CON_EDITING)
     {
        send_to_char( buf, d->character );
     }
  }
            }
	    ch->desc = NULL;
	}
	else
	{
           free_char( dclose->original ? dclose->original : dclose->character );
	}
    }

    /* sanity check :( */
    if ( !dclose->prev && dclose != first_descriptor )
    {
        DESCRIPTOR_DATA *dp, *dn;
        bug( "Close_socket: %s desc:%p != first_desc:%p and desc->prev = NULL!",
                ch ? ch->name : dclose->host, dclose, first_descriptor );
        dp = NULL;
        for ( dt = first_descriptor; dt; dt = dn )
        {
           dn = dt->next;
           if ( dt == dclose )
           {
                bug( "Close_socket: %s desc:%p found, prev should be:%p, fixing.",
                    ch ? ch->name : dt->host, dclose, dp );
                dclose->prev = dp;
                break;
           }
           dp = dt;
        }
        if ( !dclose->prev )
        {
            bug( "Close_socket: %s desc:%p could not be found!.",
                    ch ? ch->name : dclose->host, dclose );
            DoNotUnlink = TRUE;
        }
    }
    if ( !dclose->next && dclose != last_descriptor )
    {
        DESCRIPTOR_DATA *dp, *dn;
        bug( "Close_socket: %s desc:%p != last_desc:%p and desc->next = NULL!",
                ch ? ch->name : dt->host, dclose, last_descriptor );
        dn = NULL;
        for ( dt = last_descriptor; dt; dt = dp )
        {
           dp = dt->prev;
           if ( dt == dclose )
           {
                bug( "Close_socket: %s desc:%p found, next should be:%p, fixing.",
                    ch ? ch->name : dt->host, dclose, dn );
                dclose->next = dn;
                break;
           }
           dn = dt;
        }
        if ( !dclose->next )
        {
            bug( "Close_socket: %s desc:%p could not be found!.",
                    ch ? ch->name : dclose->host, dclose );
            DoNotUnlink = TRUE;
        }
    }

    if ( dclose->character ) {
        do_channels(dclose->character, "quiet");
        sprintf( log_buf, "Closing link to %s.", ch->name );
        log_string( log_buf );
        if ( dclose->connected == CON_PLAYING
        ||   dclose->connected == CON_EDITING )
        {
            act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM, FALSE );
            ch->desc = NULL;
        }
        else
        {
            /* clear descriptor pointer to get rid of bug message in log */
            dclose->character->desc = NULL;
            free_char( dclose->character );
        }
    }


    if ( !DoNotUnlink )
    {
        /* make sure loop doesn't get messed up */
    if ( d_next == dclose )
	d_next = d_next->next;   
        UNLINK( dclose, first_descriptor, last_descriptor, next, prev );
    }

#if !defined( WIN32 )
    close( dclose->descriptor );
#else
    closesocket( dclose->descriptor );
#endif
    free_string( dclose->host );

    /* RT socket leak fix */
      DISPOSE( dclose->outbuf );

      DISPOSE( dclose );
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
  --num_descriptors;
    return;
}

/* For a better kickoff message :) KaVir */
void close_socket2( DESCRIPTOR_DATA *dclose, bool kickoff )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    if ( dclose->character != NULL && 
       (dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING )&&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");
/*
    if ( dclose->character != NULL 
	&& ( dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING)
	&& !IS_NPC(dclose->character)
	&& dclose->character->pcdata != NULL
	&& dclose->character->pcdata->obj_vnum != 0
	&& dclose->character->pcdata->chobj != NULL)
	    extract_obj(dclose->character->pcdata->chobj);
*/
    {
	DESCRIPTOR_DATA *d;

	for ( d = first_descriptor; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	if ( dclose->connected == CON_PLAYING
		|| dclose->connected == CON_EDITING )
	{
	    if (kickoff)
		act( "$n doubles over in agony and $s eyes roll up into $s head.", ch, NULL, NULL, TO_ROOM, FALSE );
 	    save_char_obj( ch );
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    UNLINK( dclose, first_descriptor, last_descriptor, next, prev );

#if !defined(WIN32)
    close( dclose->descriptor );
#else
    closesocket( dclose->descriptor );
#endif
    free_string( dclose->host );
    DISPOSE( dclose );
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
  --num_descriptors;
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
	if (d != NULL && d->character != NULL)
	    sprintf( log_buf, "%s input overflow!", d->character->lasthost );
	else
	    sprintf( log_buf, "%s input overflow!", d->host );
	log_string( log_buf );

	write_to_descriptor( d->descriptor,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    /* Snarf input. */
#if defined(macintosh)
    for ( ; ; )
    {
	int c;
	c = getc( stdin );
	if ( c == '\0' || c == EOF )
	    break;
	putc( c, stdout );
	if ( c == '\r' )
	    putc( '\n', stdout );
	d->inbuf[iStart++] = c;
	if ( iStart > sizeof(d->inbuf) - 10 )
	    break;
    }
#endif

#if defined(MSDOS) || defined(unix) || defined(WIN32)
    for ( ; ; )
    {
	int nRead;

#if !defined(WIN32)
	nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
#else
	nRead = recv( d->descriptor, d->inbuf + iStart,
		     sizeof( d->inbuf ) - 10 - iStart, 0 );
#endif
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string( "EOF encountered on read." );
	    return FALSE;
	}
#if !defined(WIN32)
	else if ( errno == EWOULDBLOCK )
	    break;
#endif
#if defined( WIN32 )
        else if ( WSAGetLastError() == WSAEWOULDBLOCK || errno == EAGAIN )
	    break;
#endif
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i, j, k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */
    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    if ( ++d->repeat >= 40 )
	    {
		if (d != NULL && d->character != NULL)
		    sprintf( log_buf, "%s input spamming!", d->character->lasthost );
		else
		    sprintf( log_buf, "%s input spamming!", d->host );
		log_string( log_buf );
		write_to_descriptor( d->descriptor,
		    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
		close_socket(d);
	    }
	}
    }

    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */
/*
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;
    
    if ( fPrompt && !merc_down && d->connected == CON_PLAYING )
    {    if ( d->showstr_point )
            write_to_buffer( d,
  "[Please type (c)ontinue, (r)efresh, (b)ack, (h)elp, (q)uit, or RETURN]:  ",
                            0 );
        else if ( d->pString )
            write_to_buffer( d, "> ", 2 );
        else
   {
        char buf[200]; //more than plenty
        CHAR_DATA *ch;
        CHAR_DATA *victim;

	ch = d->original ? d->original : d->character;
	if ( xIS_SET(ch->act, PLR_BLANK) )
	    write_to_buffer( d, "\n\r", 2 );
		

	if (xIS_SET(ch->act, PLR_PROMPT) && IS_EXTRA(ch, EXTRA_PROMPT))
	    bust_a_prompt( d );
	else if ( xIS_SET(ch->act, PLR_PROMPT) )
	{
	    char buf[MAX_STRING_LENGTH];
	    char cond[MAX_INPUT_LENGTH];
	    char hit_str[MAX_INPUT_LENGTH];
	    char mana_str[MAX_INPUT_LENGTH];
	    char move_str[MAX_INPUT_LENGTH];
	    char exp_str[MAX_INPUT_LENGTH];
	    int per;

	    ch = d->character;
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))
	    {
		sprintf(exp_str, "%2.0f", ch->exp);
//		COL_SCALE(exp_str, ch, ch->exp, 1000);

	        sprintf( buf, "<[%sX] [?H ?M ?V]> ",exp_str );
	    }
	    else if (ch->position == POS_FIGHTING)
	    {
	        victim = ch->fighting;
                if (victim == NULL || victim->max_hit < 1)
                {
                    strcpy(cond, "UNKNOWN");
                    ADD_COLOUR(ch, cond, L_RED);
                }
		else if ((victim->hit*100/victim->max_hit) < 25)
		{
		    strcpy(cond, "Awful");
		    ADD_COLOUR(ch, cond, L_RED);
		}
		else if ((victim->hit*100/victim->max_hit) < 50)
		{
		    strcpy(cond, "Poor");
		    ADD_COLOUR(ch, cond, L_BLUE);
		}
		else if ((victim->hit*100/victim->max_hit) < 75)
		{
		    strcpy(cond, "Fair");
		    ADD_COLOUR(ch, cond, L_GREEN);
		}
		else if ((victim->hit*100/victim->max_hit) < 100)
		{
		    strcpy(cond, "Good");
		    ADD_COLOUR(ch, cond, YELLOW);
		}
		else if ((victim->hit*100/victim->max_hit) >= 100)
		{
		    strcpy(cond, "Perfect");
		    ADD_COLOUR(ch, cond, L_CYAN);
		}
		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
  		  sprintf( buf, "<%s [%sH %sM %sV]> ", cond, hit_str, mana_str, move_str );

	    }
	    else
	    {
		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf(exp_str, "2.0f", ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, 1000);
	        sprintf( buf, "<[%s] [%sH %sM %sV]> ",exp_str, hit_str, mana_str, move_str );
	    }
	    write_to_buffer( d, buf, 0 );
	}
    }
	if ( xIS_SET(ch->act, PLR_TELNET_GA) )
	    write_to_buffer( d, go_ahead_str, 0 );
    }

    if ( d->outtop == 0 )
	return TRUE;

    if ( d->snoop_by != NULL )
    {
	write_to_buffer( d->snoop_by, "% ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }

}
*/

bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;
    /*
     * Bust a prompt.
     */
    if ( fPrompt && !merc_down && d->connected == CON_PLAYING )
    {
	CHAR_DATA *ch;
	CHAR_DATA *victim;
        char buf[MSL];
        int percent;
        int wish = 0;
        int ft = 0;
        int quit = 0;

//	ch = d->original ? d->original : d->character;
	ch = d->character;
	if ( xIS_SET(ch->act, PLR_BLANK) )
	    write_to_buffer( d, "\n\r", 2 );

        if (xIS_SET(ch->act, PLR_BRIEF3) && ch->desc != NULL)
        {
          if (ch->amount_attacks_dealt > 0)
          {
sprintf(buf, "#0You#7'#0ve #7dealt %d #7attack#0s #7causin#0g #R%d #7damage#0! (#R%d #7average#0)#n\n\r",
              ch->amount_attacks_dealt, ch->amount_damage_dealt,
              ch->amount_damage_dealt/ch->amount_attacks_dealt);
            send_to_char(buf, ch);
          }
          if (ch->amount_attacks_recieved > 0)
          {
sprintf(buf, "#0You#7'#0ve #7bee#0n #Rstruck#0 by #R%d#7 attack#0s#7, causin#0g #R%d #7damage#0! (#R%d #7average#0)#n\n\r",
              ch->amount_attacks_recieved, ch->amount_damage_recieved,
              ch->amount_damage_recieved/ch->amount_attacks_recieved);
            send_to_char(buf, ch);
          }
          ch->amount_damage_dealt = 0;
          ch->amount_damage_recieved = 0;
          ch->amount_attacks_dealt = 0;
          ch->amount_attacks_recieved = 0;
        }
	if (xIS_SET(ch->act, PLR_PROMPT) && IS_EXTRA(ch, EXTRA_PROMPT))
	    bust_a_prompt( d );
	else if ( xIS_SET(ch->act, PLR_PROMPT) && ch->desc != NULL)
	{
	    char vict[MSL];
	    char cond[MAX_INPUT_LENGTH];
	    char hit_str[MAX_INPUT_LENGTH];
	    char mana_str[MAX_INPUT_LENGTH];
	    char move_str[MAX_INPUT_LENGTH];
	    char exp_str[MAX_INPUT_LENGTH];
	    char ft_str[MSL];
	    char wish_str[MSL];
	    char quit_str[MSL];
	    ch = d->character;
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))
	    {
		sprintf(exp_str, "#C%2.0f#n", ch->exp);
	        sprintf( buf, "#7<[#4%sX#7] [#3?#1H #3?#6M #3?#2V#7]>#n ",exp_str );
	    }
	    else if (ch->position == POS_FIGHTING)
	    {
		victim = ch->fighting;
		if (IS_NPC(victim))
		  sprintf(vict, victim->short_descr);
		else if (!IS_NPC(victim))
		  sprintf(vict, victim->pcdata->switchname);
               if (victim->max_hit > 0)
                    percent = victim->hit *100 / victim->max_hit;      
	       else                    percent = -1;                
	        if (percent >= 100)  sprintf (cond, "#7%s: #C|||||#0+#C|||||#n",vict); 
		else if (percent >= 90)   sprintf (cond, "#7%s: #y|||||#0+#y||||#0-#n",vict);
		else if (percent >= 80)   sprintf (cond, "#7%s: #y|||||#0+#y|||#0--#n",vict);
		else if (percent >= 70)   sprintf (cond, "#7%s: #G|||||#0+#G||#0---#n",vict);  
		else if (percent >= 60)   sprintf (cond, "#7%s: #G|||||#0+#G|#0----#n",vict);
		else if (percent >= 50)   sprintf (cond, "#7%s: #G|||||#0+-----#n",vict);
		else if (percent >= 40)   sprintf (cond, "#7%s: #L||||#0-+-----#n",vict);
		else if (percent >= 30)   sprintf (cond, "#7%s: #L|||#0--+-----#n",vict);
		else if (percent >= 20)   sprintf (cond, "#7%s: #R||#0---+-----#n",vict);
		else if (percent >= 10)   sprintf (cond, "#7%s: #R|#0----+-----#n",vict);
		else   sprintf (cond, "#7%s: #R-----+-----#n",vict);

		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf(ft_str,	"#R%d#n", ch->fight_timer);
		sprintf(wish_str, "#y%d#n", ch->wish_timer);
		sprintf(quit_str, "#G%d#n", ch->quit_timer);

                ft = ch->fight_timer;
                wish = ch->wish_timer;
		quit = ch->quit_timer;

                if (ft > 0 && wish <= 0 && quit <= 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#R%s #nFT#7]> ",
                        cond,hit_str, mana_str, move_str, ft_str );
                }
                else if (wish > 0 && ft <= 0 && quit <= 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#y%s #nWT#7]> ",
                        cond,hit_str, mana_str, move_str, wish_str );
                }
                else if (wish > 0 && ft > 0 && quit <= 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#R%s #nFT#7][#y%s #nWT#7]> ",
                        cond,hit_str, mana_str, move_str, ft_str, wish_str );
                }
		else if (wish <= 0 && ft <= 0 && quit > 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#G%s #nQUIT#7]> ",
                        cond,hit_str, mana_str, move_str, quit_str );
                }
		else if (wish <= 0 && ft > 0 && quit > 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#R%s #nFT#7][#G%s #nQUIT#7]> ",
                        cond,hit_str, mana_str, move_str, ft_str, quit_str );
                }
		else if (wish > 0 && ft <= 0 && quit > 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#y%s #nWT#7][#G%s #nQUIT#7]> ",
                        cond,hit_str, mana_str, move_str, wish_str, quit_str );
                }
		else if (wish > 0 && ft > 0 && quit > 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sM#7] [#R%s #nFT#7][#y%s #nWT#7][#G%s #nQUIT#7]> ",
                        cond,hit_str, mana_str, move_str, ft_str, wish_str, quit_str );
                }
		else
		  sprintf( buf, "#7<[%s#7] [%sH %sM %sM#7]> ", 
			cond,hit_str, mana_str, move_str );


	    }
	    else
	    {
		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf(exp_str, "#C%2.0f#n", ch->exp);
                sprintf(ft_str, "#R%d#n", ch->fight_timer);
                sprintf(wish_str, "#y%d#n", ch->wish_timer);
		sprintf(quit_str, "#G%d#n", ch->quit_timer); 
		ft = ch->fight_timer;
		wish = ch->wish_timer;
		quit = ch->quit_timer;
                if (ft > 0 && wish <= 0 && quit <= 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#R%s #nFT#7]> ",
                        exp_str,hit_str, mana_str, move_str, ft_str );
                }
                else if (wish > 0 && ft <= 0 && quit <= 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#y%s #nWT#7]> ",
                        exp_str,hit_str, mana_str, move_str, wish_str );
                }
                else if (wish > 0 && ft > 0 && quit <= 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#R%s #nFT#7][#y%s #nWT#7]> ",
                        exp_str,hit_str, mana_str, move_str, ft_str, wish_str );
                }
                else if (wish <= 0 && ft <= 0 && quit > 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#G%s #nQUIT#7]> ",
                        exp_str,hit_str, mana_str, move_str, quit_str );
                }
                else if (wish <= 0 && ft > 0 && quit > 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#R%s #nFT#7][#G%s #nQUIT#7]> ",
                        exp_str,hit_str, mana_str, move_str, ft_str, quit_str );
                }
                else if (wish > 0 && ft <= 0 && quit > 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#y%s #nWT#7][#G%s #nQUIT#7]> ",
                        exp_str,hit_str, mana_str, move_str, wish_str, quit_str );
                }
                else if (wish > 0 && ft > 0 && quit > 0)
                {
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7] [#R%s #nFT#7][#y%s #nWT#7][#G%s #nQUIT]>",
                        exp_str,hit_str, mana_str, move_str, ft_str, wish_str, quit_str );
                }
                else
                  sprintf( buf, "#7<[%s#7] [%sH %sM %sV#7]> ",
                        exp_str,hit_str, mana_str, move_str );
	    }
	    write_to_buffer( d, buf, 0 );
	}

	if ( xIS_SET(ch->act, PLR_TELNET_GA) )
	    write_to_buffer( d, go_ahead_str, 0 );
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by != NULL )
    {
	write_to_buffer( d->snoop_by, "% ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}



 // Append onto an output buffer.
 
/*void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    char tb[MAX_STRING_LENGTH], ccode;
    int i,j;
    bool ansi, grfx = FALSE;


    if (d != NULL && d->as != NULL) d = d->as;


    
     // Find length in case caller didn't.
     
    if ( length <= 0 )
	length = strlen(txt);

    if ( length >= MAX_STRING_LENGTH )
    {
	bug( "Write_to_buffer: Way too big. Closing.", 0 );
	return;
    }

    if (d->character == NULL ||
	(d->connected != CON_PLAYING && d->connected != CON_EDITING))
	ansi = TRUE;
    else ansi = (xIS_SET(d->character->act, PLR_ANSI)) ? TRUE : FALSE;

    
     // Initial \n\r if needed.
     
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]    = '\n';
	d->outbuf[1]    = '\r';
	d->outtop       = 2;
    }

    
     // Expand d->outbuf for ansi info
     

    j = 0;
    ccode = '3';
    tb[0] = '\0';
    for (i = 0; i < length ; i++)
    {
	if (txt[i] == '\n')
	{
// added to fix stupid PC telnet incompatibility with the rest of the world
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
// end.
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm';
	}
	if (txt[i] == '#' && txt[i+1] != 'I' && txt[i+1] != 'N')
	{
	    if (txt[++i] == 'B')
	    {
		ccode = '3';
		i++;
	    }
	    else ccode = '3';

	    switch (txt[i])
	    {
		default : break;
		case '#': tb[j++] = '#'; break;
		case '-': tb[j++] = '~'; break;
		case '+': tb[j++] = '%'; break;
		case 'I':
		case 'i': tb[j++] = 27; tb[j++] = '['; tb[j++] = '7';
			  tb[j++] = 'm'; break;
		case 'b': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '5'; tb[j++] = 'm'; 
// bold just selects colour white for now 
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = '3'; tb[j++] = '7'; tb[j++] = 'm';
		    break;
		case 'G': if (!ansi) continue;
		    tb[j++] = 27; tb[j++]='('; tb[j++]='0'; grfx = TRUE;
		    break;
		case 'g': if (!ansi) continue;
		    tb[j++] = 27; tb[j++]='('; tb[j++]='B'; grfx = TRUE;
		    break;
		case 'N':
		case 'n':
// added to fix stupid PC telnet incompatibility with the rest of the world
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
// end.
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm';
		    break;
		case '0': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
		    break;
		case 'Q': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
		    break;
		case '1': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
		    break;
		case 'W': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
		    break;
		case '2': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
		    break;
		case 'E': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
		    break;
		case '3': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
		    break;
		case 'R': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
		    break;
		case '4': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
		    break;
		case 'T': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
		    break;
		case '5': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
		    break;
		case 'Y': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
		    break;
		case '6': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
		    break;
		case '7': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
		    break;
	    }
	}
	else tb[j++] = txt[i];
    }

    {
    	tb[j++] = 27;
    	tb[j++] = '[';
    	tb[j++] = '0';
    	tb[j++] = 'm';
	if (grfx)
	{
		tb[j++] = 27;
		tb[j++]='('; 
		tb[j++]='B';
	}
    }
    tb[j]   = '\0';

    length = j;

    
     // Expand the buffer as needed.
     
    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;

	if (d->outsize >= 32000)
	{
	    bug("Buffer overflow. Closing.\n\r",0);
	    close_socket(d);
	    return;
	}
	CREATE(outbuf, char, 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	free_mem( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }

    
     //Copy.
     
    strncpy( d->outbuf + d->outtop, tb, length);
    d->outtop += length;
    return;
}*/
char *format_buffer(DESCRIPTOR_DATA *d, char *txt)
{
  char show[32000];
  char *p = '\0';
  int cnt = 0;
        
  if (strlen(txt)< 30000){
    bug("Format_buffer: Invalid size %d", strlen(txt));
    return txt;
  }
    
  p = show;
   
  for (;;)
  {
    if (cnt++< 30000) *p++ = *txt++;
    else{
      if (*txt != ' ') p--, txt--;
      else break;
    } 
  }
       
  p = '\0';
  cnt = strlen(show); 
        
  write_to_descriptor(d->descriptor, show, cnt);
  return txt;
}
        
#define  COLOR_TAG              '#'
#define  ANSI_STRING            "0123456789bBcCdDfgGiIlLnNopPrRsuwWyY"
#define  REPLACE_STRING         "-#+"

bool is_color(char code)
{
  int i = 0, j = 0;
  
  while (ANSI_STRING[i] != '\0')
  {
    if (ANSI_STRING[i] == code) return TRUE;
    i++;
  }

  while (REPLACE_STRING[j]  != '\0')
  {
    if (REPLACE_STRING[j] == code) return TRUE;
    j++;
  }
    
  return FALSE;
}
void write_to_buffer(DESCRIPTOR_DATA *d, const char *txt, int length)
{      
  char tb[32000], ccode[20];
  int i, j, cnt;
  bool ansi;
  
  if (length <= 0) length = strlen(txt);
  if (d->character == NULL)
        ansi = TRUE;
    else if (d->connected != CON_PLAYING)
        ansi = TRUE;
    else ansi = (xIS_SET(d->character->act, PLR_ANSI)) ? TRUE : FALSE;

  if (d->outtop == 0 && !d->fcommand){
    d->outbuf[0]    = '\n';
    d->outbuf[1]    = '\r';
    d->outtop       = 2;
  }
  
  j = 0;
  tb[0] = '\0';
   
  for (i = 0; i < length ; i++)
  {
    ccode[0] = '\0';

    if (txt[i] == COLOR_TAG && is_color(txt[i+1])){
      switch (txt[++i])
      {
        default: sprintf(ccode, "#%c", txt[i]);
          break;
        case '-': sprintf(ccode, "%c", '~');
          break;
        case '+': sprintf(ccode, "%c", '%');
          break;
        case '#': sprintf(ccode, "%c", '#');
          break;
        case 'I':
        case 'i': strcpy(ccode, INVERTED);
          break;
        case 'u': strcpy(ccode, UNDERLINE);
          break;
        case 'f': strcpy(ccode, FLASHING);
          break;
        case 'N':
        case 'n': strcpy(ccode, NO_COLOUR);
          break;
        case '0': if (!ansi) continue; strcpy(ccode, GREY);
          break;
        case '1': if (!ansi) continue; strcpy(ccode, L_RED);
          break;
        case '2': if (!ansi) continue; strcpy(ccode, L_GREEN);
          break;
        case '3': if (!ansi) continue; strcpy(ccode, YELLOW);
          break;
        case '4': if (!ansi) continue; strcpy(ccode, L_BLUE);
          break;
        case '5': if (!ansi) continue; strcpy(ccode, PINK);
          break;
        case '6': if (!ansi) continue; strcpy(ccode, L_CYAN);
          break;
        case '7': if (!ansi) continue; strcpy(ccode, WHITE);
          break;
        case '8': if (!ansi) continue; strcpy(ccode, D_WHITE);
          break;
        case '9': if (!ansi) continue; strcpy(ccode, INVERTED);
          break;
        case 'b': if (!ansi) continue; strcpy(ccode, D_BLUE);
          break;
        case 'B': if (!ansi) continue; strcpy(ccode, L_BLUE);
          break;
        case 'c': if (!ansi) continue; strcpy(ccode, D_CYAN);
          break;
        case 'C': if (!ansi) continue; strcpy(ccode, L_CYAN);
          break;
        case 'D': if (!ansi) continue; strcpy(ccode, GREY);
          break; 
        case 'g': if (!ansi) continue; strcpy(ccode, D_GREEN);
          break;
        case 'G': if (!ansi) continue; strcpy(ccode, L_GREEN);
          break;
        case 'l': if (!ansi) continue; strcpy(ccode, D_BLUE);
          break;
        case 'L': if (!ansi) continue; strcpy(ccode, L_BLUE); 
          break;
        case 'o': if (!ansi) continue; strcpy(ccode, BROWN); 
          break;
        case 'p': if (!ansi) continue; strcpy(ccode, MAGENTA);
          break;
        case 'P': if (!ansi) continue; strcpy(ccode, PINK);
          break;
        case 'r': if (!ansi) continue; strcpy(ccode, D_RED); 
          break;
        case 'R': if (!ansi) continue; strcpy(ccode, L_RED);
          break;
        case 's':
          if (!ansi) continue;
        
          switch(number_range(1,15))
          {
            case  1:  strcpy(ccode, L_RED);   break;
            case  2:  strcpy(ccode, L_CYAN);  break;
            case  3:  strcpy(ccode, L_BLUE);  break;
            case  4:  strcpy(ccode, L_GREEN); break;
            case  5:  strcpy(ccode, PINK);    break;
            case  6:  strcpy(ccode, YELLOW);  break;
            case  7:  strcpy(ccode, WHITE);   break;
            case  8:  strcpy(ccode, D_RED);   break;
            case  9:  strcpy(ccode, D_CYAN);  break;
            case  10: strcpy(ccode, D_BLUE);  break;
            case  11: strcpy(ccode, D_GREEN); break;
            case  12: strcpy(ccode, MAGENTA); break;
            case  13: strcpy(ccode, BROWN);   break;
            case  14: strcpy(ccode, D_WHITE); break;
            case  15: strcpy(ccode, GREY);    break;
            default:  strcpy(ccode, GREY);    break;
          }
          break;
        case 'w': if (!ansi) continue; strcpy(ccode, D_WHITE);
          break;
        case 'W': if (!ansi) continue; strcpy(ccode, WHITE);
          break;
        case 'Y': if (!ansi) continue; strcpy(ccode, BROWN);
          break;
        case 'y': if (!ansi) continue; strcpy(ccode, YELLOW);
          break;
      }
        
      if (ccode[0] != '\0'){  
        ccode[strlen(ccode)+1] = '\0';
        for (cnt=0; cnt< strlen(ccode); cnt++)
          tb[j++] = ccode[cnt];
      }
    }
    else if (txt[i] == '{' && txt[i+1] != '#'){
      switch (txt[++i])
      {
        default:
          sprintf(ccode, "{%c", txt[i]);
          break;
        case '{':
          sprintf(ccode, "%c", '{');
          break;
        case 'N':
        case 'n': strcpy(ccode, BG_CLEAR);
          break;
        case '1': if (!ansi) continue; strcpy(ccode, BG_RED);
          break;
        case '2': if (!ansi) continue; strcpy(ccode, BG_GREEN);
          break;
        case '3': if (!ansi) continue; strcpy(ccode, BG_BROWN);
          break;
        case '4': if (!ansi) continue; strcpy(ccode, BG_BLUE);
          break;
        case '5': if (!ansi) continue; strcpy(ccode, BG_PURPLE);
          break;
        case '6': if (!ansi) continue; strcpy(ccode, BG_CYAN);
          break;
        case '7': if (!ansi) continue; strcpy(ccode, BG_WHITE);
          break;
      }
      if (ccode[0] != '\0'){
        for (cnt=0; cnt< strlen(ccode); cnt++)
          tb[j++] = ccode[cnt];
      }
    }
    else tb[j++] = txt[i];
  }
        
   if (ansi)
   {
         
      tb[j++] = 27;
            
      tb[j++] = '[';
            
      tb[j++] = '0';
         
      tb[j++] = 'm';
   }        
            
  tb[j]   = '\0';
  length = strlen(tb);
        
  if ((d->outtop + length)>= 32000){
    write_to_descriptor(d->descriptor, d->outbuf, d->outtop);
    d->connected        = CON_PLAYING;
    d->outtop           = 0;
  }
        
  while (length>= 30000)
  {
    char tmp[MSL];
        
    sprintf(tmp, "%s", format_buffer(d, tb));
    tb[0] = '\0';
    strcat(tb, tmp);
    length = strlen(tb);
  }
  while (d->outtop + length >= d->outsize)
  {
//    char *outbuf;

//      outbuf = alloc_mem (2 * d->outsize);
//      strncpy (outbuf, d->outbuf, d->outtop);
//      free_mem (d->outbuf, d->outsize);
//      d->outbuf = outbuf;
      d->outsize *= 2;
RECREATE( d->outbuf, char, d->outsize );
  }  
    
  /* Copy. */
  strncpy(d->outbuf + d->outtop, tb, length);
  d->outtop += length;
  return;
}     

void owrite_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length)
{
    char tb[32000], ccode;
    int i,j;
    bool ansi, grfx = FALSE;


    /*
     * Find length in case caller didn't.
     */                                                                        
    if ( length <= 0 )
        length = strlen(txt);

    if ( length >= MAX_STRING_LENGTH )
    {
            d->connected        = CON_PLAYING;
            d->outsize          = 2000;
 	    RECREATE( d->outbuf, char, d->outsize );
            d->outtop       = 2;
	    send_to_char("#1Too much text to show, clearing...#n",d->character);
            flog("Too much text to show, clearing..... (%s)",d->character->name);
        return;
    }

    if (d->character == NULL)
        ansi = TRUE;
    else if (d->connected != CON_PLAYING)
        ansi = TRUE;
    else ansi = (xIS_SET(d->character->act, PLR_ANSI)) ? TRUE : FALSE;

    /*
     * Initial \n\r if needed.
     */                                                                        
    if ( d->outtop == 0 && !d->fcommand )
    {
        d->outbuf[0]    = '\n';
        d->outbuf[1]    = '\r';
        d->outtop       = 2;
    }

    /*
     * Expand d->outbuf for ansi info
     */

    j = 0;
    ccode = '3';
    tb[0] = '\0';
    for (i = 0; i < length ; i++)
    {
        if (txt[i] == '#' && txt[i+1] != 'I' && txt[i+1] != 'N')               
        {
            i++;
            ccode = '3';

            switch (txt[i])
            {
                default : tb[j++] = '#'; tb[j++] = txt[i]; break;
                case '-': tb[j++] = '~'; break;
                case '+': tb[j++] = '%'; break;
		case '#': tb[j++] = '#'; break;
                case 'I':
                case 'i': tb[j++] = 27; tb[j++] = '['; tb[j++] = '7'; tb[j++] = 'm'; break;                                
                case 'b': tb[j++] = 27; tb[j++] = '['; tb[j++] = '5'; tb[j++] ='m'; break;
                case 'u': tb[j++] = 27; tb[j++] = '['; tb[j++] = '4'; tb[j++] ='m'; break;
                case 't': if (!ansi) continue;
                  tb[j++] = '['; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case 'T': if (!ansi) continue;
                  tb[j++] = 27; tb[j++] = '['; tb[j++] = '4'; tb[j++] = '1';
                    break;
                case 'd': if (!ansi) continue;
                  tb[j++] = 27; tb[j++] = '['; tb[j++] = '5'; tb[j++] = '1';
                    break;
                case 'D': if (!ansi) continue;
                  tb[j++] = 27; tb[j++] = '['; tb[j++] = '6'; tb[j++] = '1';
                    break;
                case 'N':
                case 'n':
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm';
                    break;
                case 'm': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '7'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
                    break;
                case '0': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
                    break;
                case '1': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case '2': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                case '3': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case '4': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case '5': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case '6': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case '7': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';             
                    break;
                case 'w': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';             
                    break;
                case '8': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';             
                    break;
                case '9': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '7'; tb[j++] = ';';
                    tb[j++] = '4'; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case 'r': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case 'R': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case 'g': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                case 'G': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;                                                     
                case 'o': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case 'y': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case 'l': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case 'L': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case 'p': if (!ansi) continue;                                 
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case 'P': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case 'c': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case 'C': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case 's': if (!ansi) continue;
                     switch(number_range(1,15))                                
                     {
                        case  1:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                        case  2:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                        case  3:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
                    break;
                        case  4:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
                    break;                                                     
                        case  5:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                        case  6:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                        case  7:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                        case  8:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                        case  9:                                               
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                        case  10:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                        case  11:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                        case  12:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                        case  13:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                        case  14:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                        case  15:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;

                     }
                        break;


            }
        }   
        else if (txt[i] == '{')               
        {
            i++;
            ccode = '4';

            switch (txt[i])
            {
                default : tb[j++] = '{'; tb[j++] = txt[i]; break;                       
                case 'N':
                case 'n':
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm';
                    break;
                case '0': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
                    break;
                case '1': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case '2': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                case '3': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case '4': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case '5': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case '6': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case '7': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';             
                    break;
            }
        }                                                                          
        else tb[j++] = txt[i];
    }

    {
        tb[j++] = 27;
        tb[j++] = '[';
        tb[j++] = '0';
        tb[j++] = 'm';
        if (grfx)
        {
                tb[j++] = 27;
                tb[j++]='(';
                tb[j++]='B';
        }
    }
    tb[j]   = '\0';                                                            

    length = j;

    if ( length >= MAX_STRING_LENGTH )
    {
            d->connected        = CON_PLAYING;
            d->outsize          = 2000;
            RECREATE(d->outbuf, char, d->outsize );
            d->outtop       = 2;
            sprintf(tb,"Too much text to show, clearing....");
            length=strlen(tb);
            flog("Too much text to show, clearing..... (%s)",d->character->name);
    }

        if (d->outtop + length >= 32000)
        {
            d->connected        = CON_PLAYING;
            d->outsize          = 2000;
   	    RECREATE( d->outbuf, char, d->outsize );
            d->outtop       = 2;
	    sprintf(tb,"Too much text to show, clearing....");
	    length=strlen(tb);
	    flog("Too much text to show, clearing..... (%s)",d->character->name);
        }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
        if (d->outsize > 32000)
	{
	   /* empty buffer */
	    d->outtop = 0;
	    close_socket(d);
	    bug("Buffer overflow. Closing (%s).", d->character ? d->character->name : "???" );
	    return;
 	}
	d->outsize *= 2;
	RECREATE( d->outbuf, char, d->outsize );
    }

    /*
     * Copy.
     */
    strncpy( d->outbuf + d->outtop, tb, length);
    d->outtop += length;
    d->outbuf[d->outtop] = '\0';
    return;
}

//Step 2- A color strip function, add this to where ever.
char *ColorStrip(char *string)
{
int length=strlen(string);
int i=0;
int j=0;

for (i=0;i<length;i++)
{

 if (string[i] == '#' || string[i] == '{')
 {
 length -= 2;
  for (j=i;j<length;j++)
  {
  string[j] = string[j+2];
  }
  string[j++] = '\0';
 }

}

if (string[i-1] == '#')
    string[i-1] = '\0';

return(str_dup(string));
}

bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined(macintosh) || defined(MSDOS)
    if ( desc == 0 )
	desc = 1;
#endif

    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
#if !defined(WIN32)
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
#else
	if ( ( nWrite = send( desc, txt + iStart, nBlock , 0) ) < 0 )
#endif
	    { perror( "Write_to_descriptor" ); return
FALSE; }
    } 

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    OBJ_DATA *obj;
    DESCRIPTOR_DATA *dlist;
    CLAN_DATA *pClan;
    CLAN_MEMBER *cMember;
    char *pwdnew;
    char *p;
    char *strtime;
    int char_age = 17;
    bool fOld; 
    int i;
    int sn;


	if (d->connected != CON_NOTE_TEXT)
    while ( isspace(*argument) )
	argument++;

    ch = d->character;


    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected %d.", d->connected );
	close_socket( d );
	return;

    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}

	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) && str_cmp(argument, "Snard" ))
	{
	    write_to_buffer( d, "Illegal name, try another.\n\rEnter thy name brave traveler: ", 0 );
	    return;
	}

        for (dlist = first_descriptor; dlist != NULL; dlist = dlist->next)
        {
          if (!dlist->character)
             continue;

          if(dlist->character->level == 0)
          {
            if (!str_cmp(argument, dlist->character->name))
            {
               write_to_buffer(d, "Someone is already creating a char with that name.\n\rEnter thy name brave traveler: ", 0);
               return;
            }
          }
        }
	sprintf(kav,"%s trying to connect.", argument);
	log_string( kav );
	fOld = load_char_short( d, argument );
	ch   = d->character;

        char_age = years_old(ch);
	if ( xIS_SET(ch->act, PLR_DENY))	
	{
            if (ch->denied <= current_time)
	    {
	       log_string( "ReAllowing access to %s@%s.", argument, ch->lasthost );
	       xREMOVE_BIT(ch->act, PLR_DENY);
 	       ch->denied = 0;
            }
	    else
	    {
	       sprintf( log_buf, "Denying access to %s@%s.", argument, ch->lasthost );
	       log_string( log_buf );
	       write_to_buffer( d, "You are denied access.\n\r", 0 );
	       close_socket( d );
	       return;
	    }
	}
	else if ( IS_EXTRA(ch, EXTRA_BORN) && char_age < 15 )
	{
	    char agebuf [MAX_INPUT_LENGTH];
	    if (char_age == 14)
		sprintf(agebuf, "You cannot play for another year.\n\r" );
	    else
		sprintf(agebuf, "You cannot play for another %d years.\n\r", 
		(15 - years_old(ch)) );
	    write_to_buffer(d, agebuf, 0);
	    close_socket( d );
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{
	    /* Check max number of players - KaVir */

	    DESCRIPTOR_DATA *dcheck;
	    DESCRIPTOR_DATA *dcheck_next;
	    int countdesc = 0;
	    int max_players = 150;

	    for (dcheck = first_descriptor; dcheck != NULL; dcheck = dcheck_next)
	    {
		dcheck_next = dcheck->next;
		countdesc++;
	    }

	    if ( countdesc > max_players && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "Too many players connected, please try again in a couple of minutes.\n\r", 0 );
		close_socket( d );
		return;
	    }

	    if ( wizlock && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "\n\rThe game is temporarily closed to find a bug.\n\r", 0 );
		close_socket( d );
		return;
	    }
	}

	if ( fOld )
	{
	    /* Old player */
	    write_to_buffer( d, "Please enter password: ", 0 );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    /* New player */
	    sprintf( buf, "\n\rYou want %s engraved on your tombstone (Y/N)?\n\r", argument );
 	     write_to_buffer( d, buf, 0 );
	    sprintf( buf, "NOTE:  Names that are oddly capitalized will not be allowed, aka 'JeStEr'\n\r");
             write_to_buffer( d, buf, 0 );
	    sprintf( buf, "NOTE:  Names that are compound words will not be allowed, aka 'Lightbringer'\n\r");
             write_to_buffer( d, buf, 0 );
	    sprintf( buf, "NOTE:  Names that are professions will not be allowed, aka 'Butcher'\n\r");
             write_to_buffer( d, buf, 0 );
	    sprintf( buf, "NOTE:  Names that are close in characters to immortal's names will not be allowed.\n\r");
             write_to_buffer( d, buf, 0 );
	    sprintf( buf, "NOTE:  Names that are deemed inapparopriate will not be allowed.\n\r");
             write_to_buffer( d, buf, 0 );
	    sprintf( buf, "NOTE:  Choose wisely, you will be renamed if you do not.\n\r");
             write_to_buffer( d, buf, 0 );
            sprintf( buf, "You want %s engraved on your tombstone (Y/N)?\n\r", argument );
            write_to_buffer( d, buf, 0 );
	    d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	}
	break;

    case CON_GET_OLD_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif


	if ( ch == NULL || (!IS_EXTRA(ch,EXTRA_NEWPASS) &&
	    strcmp( argument, ch->pcdata->pwd ) 
&&	    strcmp( crypt ( argument, ch->pcdata->pwd ),ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}
	else if ( ch == NULL || (IS_EXTRA(ch,EXTRA_NEWPASS)
  &&	    strcmp( crypt ( argument, ch->pcdata->pwd ), ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
	if ( check_reconnect( d, GET_PC_NAME(ch), TRUE ) )
	    return;

	if ( check_playing( d, GET_PC_NAME(ch) ) )
	    return;

	if (ch->level > 1)
	{
	    sprintf(kav,ch->pcdata->switchname);
            free_char(d->character);
            d->character = NULL;
            fOld = load_char_obj( d, kav );
            ch   = d->character;
	}

//	if ( !IS_EXTRA(ch,EXTRA_NEWPASS) && strlen(argument) > 1) {
//		sprintf(kav,"%s %s",argument,argument);
//		do_password(ch,kav);}

	if (ch->lasthost != NULL) free_string(ch->lasthost);
	if (ch->desc != NULL && ch->desc->host != NULL)
	{
        ch->lasthost = str_dup(ch->desc->host);
	}
	else 
	{
	    ch->lasthost = str_dup("(unknown)");
	}
	strtime = ctime( &current_time );
	strtime[strlen(strtime)-1] = '\0';
	free_string(ch->lasttime);
	ch->lasttime = str_dup( strtime );
	sprintf( log_buf, "%s@%s has connected.",ch->name, ch->lasthost );
	log_string( log_buf );

        /* Marlow claims this is leaking memory */

	if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 &&
		ch->lasttime != NULL && strlen(ch->lasttime) > 1 )
	{
	    sprintf(kav,"Last connected from %s on %s\n\r",ch->lasthost,ch->lasttime);
	    write_to_buffer( d, kav, 0 );
	}
	else if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 )
	{
	    sprintf(kav,"Last connected from %s.\n\r",ch->lasthost);
	    write_to_buffer( d, kav, 0 );
	}

        /* Marlows claim ends here */

	/* In case we have level 4+ players from another merc mud, or 
	 * players who have somehow got file access and changed their pfiles.
	 */

	if (IS_SET(ch->newbits, NEW_CLANNABLE)) 
	REMOVE_BIT(ch->newbits, NEW_CLANNABLE);
	if (ch->level < 4 && ch->trust < 4)
	ch->trust = 0;
	if ( ch->level > 3 && ch->trust == 0)
	    ch->level = 3;
//	if (ch->level == 3 && ch->trust < 6)
//	    ch->level = 2;
//	if (ch->gold != 0) ch->gold = 0;
	else
	{

	    if ( ch->level > MAX_LEVEL )
		ch->level = MAX_LEVEL;
	    if ( ch->trust > MAX_LEVEL)
		ch->trust = MAX_LEVEL;
	    /* To temporarily grant higher powers...
	    if ( ch->trust > ch->level)
		ch->trust = ch->level;
	    */
	}
	if ( IS_HERO(ch) )
	do_help( ch, "motd" );
	
write_to_buffer( d,     "\n\r#3**#6Welcome to #RFallen Majesty #3**\n\r#3**#6Where #1Death#6 is a part of #7Life#3**\n\r", 0 );
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;
	
    if ( !IS_NPC(ch) && !IS_SET(ch->flag2, AFF_CDONE) )
    {
        if ( IS_CLASS(ch, CLASS_VAMPIRE) )
	  config_info->vampires++;
	else if ( IS_CLASS(ch, CLASS_MAGE) )
	  config_info->mages++;   
	else if ( IS_CLASS(ch, CLASS_DEMON) )
	  config_info->demons++;
	else if ( IS_CLASS(ch, CLASS_WEREWOLF) )
	  config_info->werewolves++;
	else if ( IS_CLASS(ch, CLASS_DROW) )
	  config_info->drow++;
	else if ( IS_CLASS(ch, CLASS_MONK) )
	  config_info->monks++;
	save_config();
    }

    if( IS_CLASS(ch, CLASS_WEREWOLF))
    {
	ch->gnosis[GCURRENT] = ch->gnosis[GMAXIMUM] - 5;
	if( ch->gnosis[GCURRENT] < 0 ) ch->gnosis[GCURRENT] = 0;
    }
	if (ch->mounted == IS_RIDING) do_dismount(ch,"");

	if ( !xIS_SET(ch->extra, EXTRA_TRUSTED) )
		xSET_BIT(ch->extra, EXTRA_TRUSTED);

	if ( ch->level == 0 )
	{
	    ch->pcdata->plr_wager    = 0; /* arena betting amount */
            ch->pcdata->awins        = 0; /* arena wins           */
            ch->pcdata->alosses      = 0; /* arena losses         */
		ch->pcdata->board = &boards[0];
            ch->gladiator       = NULL; /* set player to bet on to NULL */
	    ch->challenger = NULL;
	    ch->challenged = NULL;
	    ch->level	= 1;
	    ch->exp	= 0;
	    ch->hit	= ch->max_hit;
	    ch->mana	= ch->max_mana;
	    ch->move	= ch->max_move;
            ch->special = 0;
	    set_switchname(ch, ch->name);
	    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    do_look( ch, "auto" );
	}
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	{
	    if (ch->in_room != NULL) char_to_room( ch, ch->in_room );
	    else char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    bind_char(ch);
	    break;
	}
	else if ( ch->in_room != NULL )
	{
	    if (xIS_SET(ch->extra, EXTRA_FIXSTATUS))
	    char_to_room( ch, ch->in_room );
	    else if (!xIS_SET(ch->extra, EXTRA_FIXSTATUS))
	    char_to_room(ch, get_room_index(ROOM_VNUM_QUIT));
	    do_look( ch, "auto" );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_ALTAR ) );
	    do_look( ch, "auto" );
	}
        do_board(ch,"");
        players_logged++;
	ch->home = 41001;

	if (!xIS_SET(ch->extra, EXTRA_FIXSTATUS)) fixstatus(ch);

        if (!IS_IMMORTAL(ch))
        ch->quit_timer += 20 + ch->race;
        for ( d = first_descriptor; d != NULL; d = d->next )
	{
	    if ( d->character != NULL )
    	    {
		  if (!d->connected == CON_PLAYING) continue;
		  if (d->character->level > 6) continue;
	          if (!IS_HERO(d->character)) continue;
       		  if (d->character->pcdata->chobj != NULL) continue;
		  if (is_range(ch,d->character)) continue;
		  if (ch->level > 6) continue;
		  d->character->quit_timer += (20 + ch->race);
		  sprintf(buf, "#7Because #C%s #7is in your range, your quit timer is now: #C%d#7.#n\n\r",ch->pcdata->switchname,d->character->quit_timer);
		  stc(buf,d->character);
	    }	    
  	}

	if (!xIS_SET(ch->extra, EXTRA_SPELL_RESET))
	{
          xSET_BIT(ch->extra, EXTRA_SPELL_RESET);
          for ( sn = 0; sn < MAX_SKILL; sn++ )
          {
            if ((skill_table[sn].name != NULL) && (ch->level >= skill_table[sn].skill_level))
                ch->pcdata->learned[sn]     = SKILL_ADEPT;
          }
	}

	if ( ch->class == 0 )	
	sprintf(buf,"#2%s#7 has entered the #RFallen Majesty.#n",ch->pcdata->switchname);
	else 
	{
	 switch( ch->class )
	  { // Thanks Alstor for these Class Messages
  	    case CLASS_DEMON: sprintf(buf,"#r%s#R bows to the whim of Lord Satan and enters the frey.#n",ch->name);
                                    break;
	    case CLASS_MONK: sprintf(buf,"#7God sends #R%s#7 to fight the good fight.#n",ch->name);
                                   break;
	    case CLASS_WEREWOLF: sprintf(buf,"#R%s#o stands and howls at the moon before charging into the frey.#n",ch->name);
                                       break;
	    case CLASS_CYBORG: sprintf(buf,"#R%s#p skitters forth with a metallic clicking.#n",ch->name);
                                     break;
	    case CLASS_UNDEAD_KNIGHT: sprintf(buf,"#R%s#0 charges forth on a skeletal steed!#n",ch->name);
                                      break;
	    case CLASS_ANGEL: sprintf(buf,"#7%s#n #yslowly floats down from Heaven.#n",ch->name);
                                    break;
            case CLASS_MAGE: sprintf(buf,"#C%s#C appears in a #7blinding flash.#n",ch->name);
                                   break;
	    case CLASS_VAMPIRE: sprintf(buf,"#R%s#0 steps out of %s crypt.#n",ch->name,ch->sex ==SEX_MALE         ? "his" : ch->sex == SEX_FEMALE       ? "her"      : "its");
                                      break;
	    case CLASS_NINJA: sprintf(buf,"#R%s#0 steps out of the shadows.#n",ch->name);
                                    break;
	    case CLASS_TANARRI: sprintf(buf,"#r%s#R yells loudly as the #r B#Rlood#r W#Rars begin.#n",ch->name);
                                       break;
	    case CLASS_DROW: sprintf(buf,"#R%s#P steps out of the Temple of Lloth.#n",ch->name);
                                   break;
	    case CLASS_HIGHLANDER: sprintf(buf,"#R%s#C raises %s katana in salute.#n",ch->name,ch->sex ==SEX_MALE         ? "his" : ch->sex == SEX_FEMALE       ? "her"      : "its");
                                         break;
	    case CLASS_POWER_LICH: sprintf(buf,"#R%s#0 calls upon the dark magics.#n",ch->name);
                                     break;
	    case CLASS_SHAPESHIFTER: sprintf(buf,"#R%s#P coalesces from the mist.#n",ch->name);
                                          break;
	    case CLASS_PALADIN:	sprintf(buf,"#y%s#7 walks the path of the #yP#7aladin.#n",ch->name);
			break;
	    case CLASS_PSIONIST: sprintf(buf,"#P%s#C makes %s appear, as any true #PPsionist#C would.#n",ch->name,ch->sex == SEX_MALE ? "himself" : ch->sex == SEX_FEMALE ? "herself" : "itself");
			break;
	    case CLASS_INQUISITOR: sprintf(buf, "#7%s#0 walks amongst you, sinners beware.#n",ch->name);
			break;
	    default: sprintf(buf,"#2%s#7 enters the #RFallen Majesty.,n",ch->name);
                           break;
               }
            }



	if (!IS_IMMORTAL(ch)) do_info(ch,buf);

    if ( ++num_descriptors > sysdata.maxplayers )
        sysdata.maxplayers = num_descriptors;
/*    if ( sysdata.maxplayers > sysdata.alltimemax )
    {
        if ( sysdata.time_of_max )
          DISPOSE(sysdata.time_of_max);
        sprintf(buf, "%24.24s", ctime(&current_time));
        sysdata.time_of_max = str_dup(buf);
        sysdata.alltimemax = sysdata.maxplayers;
        log_string( "Broke all-time maximum player record: %d", sysdata.alltimemax );
        sprintf(buf,  "Broke all-time maximum player record: %d", sysdata.alltimemax );
        do_info(ch, buf);
        save_sysdata( sysdata );
    }
*/
        sysdata.exp_timer += 60;
        save_sysdata( sysdata );

	ch->fight_timer = 0;
/*	if (ch->expgained == 0)
	{
	  SET_BIT(ch->special, SPC_NEWBIE);
	  ch->expgained = 1;
	  stc("You have been reverted to a newbie, please train pk.\n\r",ch);
	}
*/

	act( "$n has entered the game.", ch, NULL, NULL, TO_NOTVICT, FALSE );
	do_save(ch,"");       
	if ( !IS_NPC(ch) )
       {
       for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
       {
	int value = 0;
	if( obj->item_type == ITEM_WEAPON )
	{
	    value = obj->value[0];
	    if( value >= 1000 )
	    {
		int temp = value / 1000;
		temp = temp * 1000;
		value -= temp;
	    }
	    if( value == 81 )
	    { do_remove(ch,obj->name);extract_obj(obj);}
	}
    }
  }

	if( !IS_NPC(ch) && ch->pcdata->bounty > 0 ){
	    sprintf(buf,"%s has a %d QPs bounty on %s head.",ch->name,ch->pcdata->bounty,ch->sex ==SEX_MALE         ? "his" : ch->sex == SEX_FEMALE       ? "her"      : "its");
	    do_info(ch,buf);
	}
	room_text(ch,">ENTER<");

	if (!IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_HIGHLANDER) )
		do_noarm(ch,"self");


        if (IS_CLASS(ch, CLASS_DEMON))
   	    set_form(ch, get_normal_form(ch));

	for ( i = 0 ; i < MAX_DISCIPLINES ; i++)
        {                           
	    if (ch->power[i] > 10 && !xIS_SET(ch->extra, EXTRA_FLASH) && !xIS_SET(ch->extra, EXTRA_BAAL))
		ch->power[i] = 10;
	        
        }

	if( !IS_CLASS(ch, CLASS_DEMON) && !IS_IMMORTAL(ch) && ch->level > 2)
	{
	    ch->power[DISC_DAEM_ATTA] = -2;
	    ch->power[DISC_DAEM_IMMU] = -2;
	    ch->power[DISC_DAEM_TEMP] = -2;
	    ch->power[DISC_DAEM_CORR] = -2;
	    ch->power[DISC_DAEM_HELL] = -2;
	    ch->power[DISC_DAEM_GELU] = -2;
	    ch->power[DISC_DAEM_MORP] = -2;
	    ch->power[DISC_DAEM_NETH] = -2;
	    ch->power[DISC_DAEM_DISC] = -2;
	}
	if( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_IMMORTAL(ch) && ch->level > 2)
	{
	    ch->power[DISC_WERE_BEAR] = -2;
	    ch->power[DISC_WERE_LYNX] = -2;
	    ch->power[DISC_WERE_BOAR] = -2;
	    ch->power[DISC_WERE_OWL]  = -2;
	    ch->power[DISC_WERE_SPID] = -2;
	    ch->power[DISC_WERE_WOLF] = -2;
	    ch->power[DISC_WERE_HAWK] = -2;
	    ch->power[DISC_WERE_MANT] = -2;
	    ch->power[DISC_WERE_RAPT] = -2;
	    ch->power[DISC_WERE_LUNA] = -2;
	    ch->power[DISC_WERE_PAIN] = -2;
	    ch->power[DISC_WERE_CONG] = -2;
	}
	if( !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_IMMORTAL(ch) && ch->level > 2)
	{
	    ch->power[DISC_VAMP_FORT] = -2;
	    ch->power[DISC_VAMP_CELE] = -2;
	    ch->power[DISC_VAMP_OBTE] = -2;
	    ch->power[DISC_VAMP_PRES] = -2;
	    ch->power[DISC_VAMP_QUIE] = -2;
	    ch->power[DISC_VAMP_THAU] = -2;
	    ch->power[DISC_VAMP_AUSP] = -2;
	    ch->power[DISC_VAMP_DOMI] = -2;
	    ch->power[DISC_VAMP_OBFU] = -2;
	    ch->power[DISC_VAMP_POTE] = -2;
	    ch->power[DISC_VAMP_PROT] = -2;
	    ch->power[DISC_VAMP_SERP] = -2;
	    ch->power[DISC_VAMP_VICI] = -2;
	    ch->power[DISC_VAMP_DAIM] = -2;
	    ch->power[DISC_VAMP_ANIM] = -2;
	    ch->power[DISC_VAMP_CHIM] = -2;
	    ch->power[DISC_VAMP_MELP] = -2;
	    ch->power[DISC_VAMP_NECR] = -2;
	    ch->power[DISC_VAMP_THAN] = -2;
	    ch->power[DISC_VAMP_OBEA] = -2;
	}

        if (is_demonic_form(ch->cur_form) && ch->cur_form != get_normal_form(ch))
	    set_form(ch, get_normal_form(ch)); 

        if (ch->cur_form > 0)
        {
             int form;
	     form = ch->cur_form;
	     set_form(ch,form);
	} 

	ch->embraced=NULL; 
        ch->embracing=NULL;
                if ( ch->position <= POS_STANDING)
        {
        ch->position = POS_STANDING;
        }
/*
	if (multicheck(ch))
        {
          send_to_char("\n\r#R====#0[#y**#0]#R====  #GWARNING  #R====#0[#y**#0]#R====#n\n\r",ch);
          send_to_char("You have connected from the same\n\r",ch);
          send_to_char("IP as someone who is already on the\n\r",ch);
          send_to_char("mud. Multiplaying is illegal, and\n\r",ch);
          send_to_char("your actions will be monitored.\n\r",ch);
          send_to_char("#R====#0[#y**#0]#R====  #GWARNING  #R====#0[#y**#0]#R====#n\n\r",ch);
        }
*/
	if (( pClan = get_plr_clan( ch->name )) != NULL)
	{
	   cMember = get_member( ch->name, pClan );
	   if (xIS_SET(cMember->flags, CL_REJECTED))
	   {
	      chprintf(ch, "\n\r\n\r\n\rYour application for %s has been rejected.\n\r", pClan->title);
              ch->gold += cMember->bank;
	      clan_remove( ch->name, pClan );
	   }
	}

        break;
    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':

	    sprintf( buf, "\n\rNew character.\n\rGive me a password for %s: %s\n\r",
		ch->name, echo_off_str );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	 
	   break;
	case 'n': case 'N':
	    write_to_buffer( d, "Ok, what IS it, then?\n\r", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "Please type Yes or No?\n\r", 0 );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword:\n\r",
		0 );
	    return;
	}
	pwdnew = crypt ( argument, ch->name );

	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
   0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );

	write_to_buffer( d, "Please retype password:\n\r", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

  if ( strcmp( argument, ch->pcdata->pwd) && 
 strcmp( crypt ( argument, ch->pcdata->pwd ), ch->pcdata->pwd ))
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password:\n\r",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "What is your sex (M/F)?\n\r", 0 );
	d->connected = CON_GET_NEW_SEX;
	break;

      case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;  break;
	default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
	    return;
	}
	write_to_buffer( d, echo_on_str, 0 );
        write_to_buffer( d, "\n\rDo you wish to use ANSI color (y/n)?\n\r", 0);
	d->connected = CON_GET_NEW_ANSI;
	break;

/*    case CON_GET_NEW_CLASS:
        write_to_buffer( d, echo_on_str, 0 );
        write_to_buffer( d, "\n\rDo you wish to use ANSI color (y/n)?\n\r", 0);
        d->connected = CON_GET_NEW_ANSI;
        break;
*/
    case CON_GET_NEW_ANSI:
	switch ( argument[0] )
	{
	case 'y': case 'Y': xSET_BIT(ch->act,PLR_ANSI);    break;
	case 'n': case 'N': break;
	default:
	    write_to_buffer( d, "\n\rDo you wish to use ANSI (y/n)?\n\r", 0 );
	    return;
	}

	ch->pcdata->perm_str=number_range(10,16);
        ch->pcdata->perm_int=number_range(10,16);
        ch->pcdata->perm_wis=number_range(10,16);
        ch->pcdata->perm_dex=number_range(10,16);
	ch->pcdata->perm_con=number_range(10,16);
	ch->class = 0;
	set_learnable_disciplines(ch);
	sprintf( log_buf, "%s@%s new player.",ch->name, d->host );
	log_string( log_buf );
	write_to_buffer( d, "\n\r", 2 );
        ch->lasthost = d->host;
	do_help( ch, "motd" );
	xSET_BIT(ch->extra, EXTRA_FIXRESET);
	xSET_BIT(ch->extra, EXTRA_FIXSTATUS);
	xSET_BIT(ch->extra, EXTRA_SPELL_RESET);
	d->connected = CON_READ_MOTD;

	break;

    case CON_READ_MOTD:
	write_to_buffer( d,     "\n\r#3**#6Welcome to #RFallen Majesty #3**\n\r#3**#6Where #1Death#6 is a part of #7Life#3**\n\r", 0 );
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;

    if ( !IS_NPC(ch) && !IS_SET(ch->flag2, AFF_CDONE) )
    {
        if ( IS_CLASS(ch, CLASS_VAMPIRE) )
	  config_info->vampires++;
	else if ( IS_CLASS(ch, CLASS_MAGE) )
	  config_info->mages++;   
	else if ( IS_CLASS(ch, CLASS_DEMON) )
	  config_info->demons++;
	else if ( IS_CLASS(ch, CLASS_WEREWOLF) )
	  config_info->werewolves++;
	else if ( IS_CLASS(ch, CLASS_DROW) )
	  config_info->drow++;
	else if ( IS_CLASS(ch, CLASS_MONK) )
	  config_info->monks++;
	save_config();
    }

    if( IS_CLASS(ch, CLASS_WEREWOLF))
    {
	ch->gnosis[GCURRENT] = ch->gnosis[GMAXIMUM] - 5;
	if( ch->gnosis[GCURRENT] < 0 ) ch->gnosis[GCURRENT] = 0;
    }


	if (ch->mounted == IS_RIDING) do_dismount(ch,"");

	if ( !xIS_SET(ch->extra, EXTRA_TRUSTED) )
		xSET_BIT(ch->extra, EXTRA_TRUSTED);

	if ( ch->level == 0 )
	{
	    ch->pcdata->plr_wager    = 0; /* arena betting amount */
            ch->pcdata->awins        = 0; /* arena wins           */
            ch->pcdata->alosses      = 0; /* arena losses         */
		ch->pcdata->board = &boards[0];
            ch->gladiator       = NULL; /* set player to bet on to NULL */
	    ch->challenger = NULL;
	    ch->challenged = NULL;
	    ch->level	= 1;
            ch->generation = 6;
            ch->stance[19] = -1;
            ch->stance[20] = -1;
            ch->stance[21] = -1;
            ch->stance[22] = -1;
            ch->stance[23] = -1;
	    ch->exp	= 0;
	    ch->hit	= ch->max_hit;
	    ch->mana	= ch->max_mana;
	    ch->move	= ch->max_move;
            ch->special = 0;
	    set_switchname(ch, ch->name);
	    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    do_look( ch, "auto" );
            do_newbiepack(ch,"");
	}
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	{
	    if (ch->in_room != NULL) char_to_room( ch, ch->in_room );
	    else char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    bind_char(ch);
	    break;
	}
	else if ( ch->in_room != NULL )
	{
	    char_to_room( ch, ch->in_room );
	    do_look( ch, "auto" );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
	    do_look( ch, "auto" );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
	    do_look( ch, "auto" );
	}
	do_board(ch,"");
	sprintf(buf,"#7A #Rnew player#7 named #2%s #7enters #RFallen Majesty.#n", ch->name );
	do_info(ch,buf);
	ch->fight_timer = 0;
	ch->pcdata->wolf = 1;
	SET_BIT(ch->special, SPC_NEWBIE);
	act( "$n has entered the game.", ch, NULL, NULL, TO_NOTVICT, FALSE );
        clear_stats(ch);
        do_wear(ch, "all");
  if ( !IS_NPC(ch) )
  {
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	int value = 0;
        if ( IS_SET(obj->quest, QUEST_ARTIFACT) )
	{
		extract_obj( obj );
	}
	if( obj->item_type == ITEM_WEAPON )
	{
	    value = obj->value[0];
	    if( value >= 1000 )
	    {
		int temp = value / 1000;
		temp = temp * 1000;
		value -= temp;
	    }
	    if( value == 81 )
	    { do_remove(ch,obj->name);extract_obj(obj);}
	}
    }
  }

	if( !IS_NPC(ch) && ch->pcdata->bounty > 0 ){
	    sprintf(buf,"%s has a %d QPs bounty on %s head.",ch->name,ch->pcdata->bounty,ch->sex ==SEX_MALE         ? "his" : ch->sex == SEX_FEMALE       ? "her"      : "its");
	    do_info(ch,buf);
	}
	room_text(ch,">ENTER<");

        if (strlen(ch->lasthost) > 2)
/*
        if (multicheck(ch))
        {
          send_to_char("\n\r#R====#0[#y**#0]#R====  #GWARNING  #R====#0[#y**#0]#R====#n\n\r",ch);
          send_to_char("You have connected from the same\n\r",ch);
          send_to_char("IP as someone who is already on the\n\r",ch);
          send_to_char("mud. Multiplaying is illegal, and\n\r",ch);
          send_to_char("your actions will be monitored.\n\r",ch);
          send_to_char("#R====#0[#y**#0]#R====  #GWARNING  #R====#0[#y**#0]#R====#n\n\r",ch);
        }
*/
	if (!IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_HIGHLANDER) )
		do_noarm(ch,"self");
/*	do_help(ch,config_info->autohelp);*/
       /* do_sset(ch,"self all 100");*/
      /*  if (xIS_SET(ch->act,PLR_VT102))
        {d->vt102=TRUE;}
        if (d->vt102==TRUE)
        do_refresh(ch,"");*/


        if (IS_CLASS(ch, CLASS_DEMON))
	    set_form(ch, get_normal_form(ch));  

	for ( i = 0 ; i < MAX_DISCIPLINES ; i++)
        {                           
	    if (ch->power[i] > 10)
		ch->power[i] = 10;
/*	    if ( IS_CLASS(ch, CLASS_DEMON) )
		ch->power[i] = -1;*/
	        
        }

	if( !IS_CLASS(ch, CLASS_DEMON) && !IS_IMMORTAL(ch))
	{
	    ch->power[DISC_DAEM_ATTA] = -2;
	    ch->power[DISC_DAEM_IMMU] = -2;
	    ch->power[DISC_DAEM_TEMP] = -2;
	    ch->power[DISC_DAEM_CORR] = -2;
	    ch->power[DISC_DAEM_HELL] = -2;
	    ch->power[DISC_DAEM_GELU] = -2;
	    ch->power[DISC_DAEM_MORP] = -2;
	    ch->power[DISC_DAEM_NETH] = -2;
	    ch->power[DISC_DAEM_DISC] = -2;
	}
	if( !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_IMMORTAL(ch))
	{
	    ch->power[DISC_WERE_BEAR] = -2;
	    ch->power[DISC_WERE_LYNX] = -2;
	    ch->power[DISC_WERE_BOAR] = -2;
	    ch->power[DISC_WERE_OWL]  = -2;
	    ch->power[DISC_WERE_SPID] = -2;
	    ch->power[DISC_WERE_WOLF] = -2;
	    ch->power[DISC_WERE_HAWK] = -2;
	    ch->power[DISC_WERE_MANT] = -2;
	    ch->power[DISC_WERE_RAPT] = -2;
	    ch->power[DISC_WERE_LUNA] = -2;
	    ch->power[DISC_WERE_PAIN] = -2;
	    ch->power[DISC_WERE_CONG] = -2;
	}
	if( !IS_CLASS(ch, CLASS_VAMPIRE) && !IS_IMMORTAL(ch))
	{
	    ch->power[DISC_VAMP_FORT] = -2;
	    ch->power[DISC_VAMP_CELE] = -2;
	    ch->power[DISC_VAMP_OBTE] = -2;
	    ch->power[DISC_VAMP_PRES] = -2;
	    ch->power[DISC_VAMP_QUIE] = -2;
	    ch->power[DISC_VAMP_THAU] = -2;
	    ch->power[DISC_VAMP_AUSP] = -2;
	    ch->power[DISC_VAMP_DOMI] = -2;
	    ch->power[DISC_VAMP_OBFU] = -2;
	    ch->power[DISC_VAMP_POTE] = -2;
	    ch->power[DISC_VAMP_PROT] = -2;
	    ch->power[DISC_VAMP_SERP] = -2;
	    ch->power[DISC_VAMP_VICI] = -2;
	    ch->power[DISC_VAMP_DAIM] = -2;
	    ch->power[DISC_VAMP_ANIM] = -2;
	    ch->power[DISC_VAMP_CHIM] = -2;
	    ch->power[DISC_VAMP_MELP] = -2;
	    ch->power[DISC_VAMP_NECR] = -2;
	    ch->power[DISC_VAMP_THAN] = -2;
	    ch->power[DISC_VAMP_OBEA] = -2;
	}

        if (is_demonic_form(ch->cur_form) && ch->cur_form != get_normal_form(ch))
	    set_form(ch, get_normal_form(ch));    

        if (ch->cur_form > 0)
        {
             int form;
	     form = ch->cur_form;
	     set_form(ch,form);  
	}

	ch->embraced=NULL; 
        ch->embracing=NULL;
	if ( ch->position <= POS_STANDING)
	{
	ch->position = POS_STANDING;
	}
/*
        if (multicheck(ch))
        {
          send_to_char("\n\r#R====#0[#y**#0]#R====  #GWARNING  #R====#0[#y**#0]#R====#n\n\r",ch);
          send_to_char("You have connected from the same\n\r",ch);
          send_to_char("IP as someone who is already on the\n\r",ch);
          send_to_char("mud. Multiplaying is illegal, and\n\r",ch);
          send_to_char("your actions will be monitored.\n\r",ch);
          send_to_char("#R====#0[#y**#0]#R====  #GWARNING  #R====#0[#y**#0]#R====#n\n\r",ch);
        }
*/
	break;
	/* states for new note system, (c)1995-96 erwin@pip.dknet.dk */
	/* ch MUST be PC here; have nwrite check for PC status! */
		
	case CON_NOTE_TO:
		handle_con_note_to (d, argument);
		break;
		
	case CON_NOTE_SUBJECT:
		handle_con_note_subject (d, argument);
		break; /* subject */
	
	case CON_NOTE_EXPIRE:
		handle_con_note_expire (d, argument);
		break;

	case CON_NOTE_TEXT:
		handle_con_note_text (d, argument);
		break;
		
	case CON_NOTE_FINISH:
		handle_con_note_finish (d, argument);
		break;

    }

    return;
}



/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
    if ( is_name( name, "all auto immortal self dorky someone gaia none save extasy fuck shit bitch pussy marlboro inspired insipid method truculent" ) )
	return FALSE;

    /*
     * Length restrictions.
     */
    if ( strlen(name) <  3 )
	return FALSE;

#if defined(MSDOS)
    if ( strlen(name) >  8 )
	return FALSE;
#endif

#if defined(macintosh) || defined(unix) || defined(WIN32)
    if ( strlen(name) > 12 )
	return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll;

	fIll = TRUE;
	if( !str_cmp(name,"al'Lan") ) return TRUE;

	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) )
		return FALSE;
	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;
    }
    if( !str_cmp(name,"kip") ) return TRUE;

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->player_name ))
		    return FALSE;
	    }
	}
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)  && !IS_EXTRA(ch, EXTRA_SWITCH)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( GET_PC_NAME(d->character), GET_PC_NAME(ch) ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "Reconnecting.\n\r", ch );
	    	if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
		    act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM, FALSE );
		sprintf( log_buf, "%s@%s reconnected.",ch->name, ch->lasthost );
		log_string( log_buf );
		d->connected = CON_PLAYING;

 		/* Inform the character of a note in progress and the possbility of continuation! */		
 		if (ch->pcdata->in_progress)
 			send_to_char ("You have a note in progress. Type NWRITE to continue it.\n\r",ch);
/*
        if (multicheck(ch))
        {
          send_to_char("\n\r#R====#0[#y**#0]#R====  #GWARNING  #R====#0[#y**#0]#R====#n\n\r",ch);
          send_to_char("You have connected from the same\n\r",ch);
          send_to_char("IP as someone who is already on the\n\r",ch);
          send_to_char("mud. Multiplaying is illegal, and\n\r",ch);
          send_to_char("your actions will be monitored.\n\r",ch);
          send_to_char("#R====#0[#y**#0]#R====  #GWARNING  #R====#0[#y**#0]#R====#n\n\r",ch);
        }
*/
	    }
	    return TRUE;
	}
    }

    return FALSE;
}

/*
 * Kick off old connection.  KaVir.
 */
bool check_kickoff( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( GET_PC_NAME(d->character),GET_PC_NAME(ch) ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "You take over your body, which was already in use.\n\r", ch );		
		act( "...$n's body has been taken over by another spirit!", ch, NULL, NULL, TO_NOTVICT, FALSE );
		sprintf( log_buf, "%s@%s kicking off old link.", ch->name, ch->lasthost );
		log_string( log_buf );
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing - KaVir.
 * Using kickoff code from Malice, as mine is v. dodgy.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = first_descriptor; dold != NULL; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? GET_PC_NAME(dold->original) : GET_PC_NAME(dold->character) ) )
	{
	    char	buf [MAX_STRING_LENGTH];
	    if ( d->character != NULL )
	    {
		free_char( d->character );
		d->character = NULL;
	    }
	    send_to_char("This body has been taken over!\n\r",dold->character);
	    d->connected = CON_PLAYING;
	    d->character = dold->character;
	    d->character->desc = d;
	    send_to_char( "You take over your body, which was already in use.\n\r", d->character );
	    act( "$n doubles over in agony and $s eyes roll up into $s head.", d->character, NULL, NULL, TO_NOTVICT, FALSE );
	    act( "...$n's body has been taken over by another spirit!", d->character, NULL, NULL, TO_ROOM, FALSE);
	    dold->character=NULL;
	    
	    sprintf(buf,"Kicking off old connection %s@%s",d->character->name,d->host);
	    log_string(buf);
	    close_socket(dold);	/*Slam the old connection into the ether*/
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   (ch->desc->connected != CON_PLAYING && ch->desc->connected != CON_EDITING)
    ||   ch->was_in_room == NULL 
    ||   ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_NOTVICT, FALSE );
    return;
}


void stc( const char *txt, CHAR_DATA *ch )
{
    if ( txt != NULL && ch->desc != NULL )
	write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}

int col_str_len(char *txt)
{
    int pos, len;

    len = 0;
    for (pos = 0; txt[pos] != '\0'; pos++)
    {
	if (txt[pos] != '#')
	{
	    len++;
	    continue;
	}

	pos++;
	if (txt[pos] == '\0') return len;
	if (txt[pos] == '#' || txt[pos] == '-') len++;
    }

    return len;
}

void cent_to_char(char *txt, CHAR_DATA *ch)
{
    int len,pos;
    char buf[MAX_STRING_LENGTH];

    len = (80 - col_str_len(txt)) / 2;
    for (pos = 0; pos < len; pos++)
    {
	buf[pos] = ' ';
    }
    buf[pos]= '\0';
    send_to_char(buf, ch);
    send_to_char(txt, ch);
    send_to_char("\n\r",ch);
}

void divide_to_char(CHAR_DATA *ch)
{
send_to_char("-------------------------------------------------------------------------------\r\n",
ch); }

void divide2_to_char(CHAR_DATA *ch)
{
send_to_char("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n",
ch); }

void divide3_to_char(CHAR_DATA *ch)
{
send_to_char("===============================================================================\r\n",ch);
}

void divide4_to_char(CHAR_DATA *ch)
{
send_to_char("#4-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6***#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-\r\n",ch);
}

void divide5_to_char(CHAR_DATA *ch)
{
cent_to_char("#4-=[#6***********#4]=-------------=[#6***********#4]=-#n",ch);
}

void divide6_to_char(CHAR_DATA *ch)
{
cent_to_char("#4-    -   -  - - -#6- ---====#7*#6====--- -#4- - -  -   - -",ch); }
/*
void rand_to_char2( CHAR_DATA *ch)
{
cent_to_char("#0-    #n-   #7-  #n- #0- #n-#7- #n-#0-#n-#7=#n=#0=#n=#7*
#n=#0=#n=#7=#n-#0-#n- #7-#n- #0- #n-  #7-   #n-    #0-",ch);
}

void rand_to_char( CHAR_DATA *ch)
{
cent_to_char("#0=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0
=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=
#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n=#0=#n
=#7=#n=#0=#n=#7=#n=#0=#n=#7=#n",ch);
}
*/
void banner_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "#4-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[                   ]#4=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-#n"); 
	wdth = (17 - ln) / 2 + 20;
}
else
{
	sprintf(buf,
"#4-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[              ] #L=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-#n");
	wdth = (11 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 22] = txt[loop];
cent_to_char(buf,ch);
}

void banner2_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "#4    -   -  - - -#6- ---===#7                               #6===--- -#4- - -  -   -\r\n"); 
	wdth = (31 - ln) / 2 + 24;
}
else
{
	sprintf(buf, "#4     -    -   -  - - -#6- ---====#7                #6====--- -#4- - -  -   -    -\r\n");
	wdth = (16 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 6] = txt[loop];
cent_to_char(buf,ch);
}


/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    CHAR_DATA *wizard;
    CHAR_DATA *familiar;

/*    if (txt[0] == '\0')
    {
       bug("Error STC, NULL txt");
       return;
    }
*/
    if ( ch->desc == NULL && IS_NPC(ch) && (wizard = ch->wizard) != NULL )
    {
	if (!IS_NPC(wizard) && (familiar = wizard->pcdata->familiar) != NULL 
	    && familiar == ch && ch->in_room != wizard->in_room)
	{
	    send_to_char("[ ",wizard);
	    if ( txt != NULL && wizard->desc != NULL )
		write_to_buffer( wizard->desc, txt, strlen(txt) );
	}
    }

    if ( txt != NULL && ch->desc != NULL )
	write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}



/*
 * The primary output interface for formatted output.
 */
void act( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type, 
bool brief )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;

    CHAR_DATA *to_old;

    CHAR_DATA *vch = (CHAR_DATA *) arg2;

    CHAR_DATA *familiar = NULL;
    CHAR_DATA *wizard = NULL;

    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;

    bool is_fam;

    bool is_ok;
    /*
     * Discard null and zero-length messages.
     */
    if ( !format )
	return;

    if ( format[0] == '\0' )
       return;

/*	Skummel crash bug, tror dette check spiser buggen, vi faar se */
	if ( ch == NULL || ch->in_room == NULL)
	{
	    return;
	}

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL || vch->in_room == NULL)
	{
/*	    bug( "Act: null vch with TO_VICT.", 0 );*/
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {

	is_fam = FALSE;
	to_old = to;

	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT)
          {
	  if (to == ch || to == vch) continue;
	  if (brief)          
	  if (xIS_SET(to->act, PLR_BRIEF2)) continue;
          }

	if ( to->desc == NULL && IS_NPC(to) && (wizard = to->wizard) != NULL )
	{
	    if (!IS_NPC(wizard) && ((familiar=wizard->pcdata->familiar) != NULL)
		&& familiar == to)
	    {
		if (to->in_room == ch->in_room && 
		    wizard->in_room != to->in_room)
		{
		    to = wizard;
		    is_fam = TRUE;
		}
	    }
	}

	if ( to->desc == NULL || !IS_AWAKE(to) )
	{

	    if (is_fam) to = to_old;

	    continue;
	}

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok)
	    {

		if (is_fam) to = to_old;

		continue;
	    }
    	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {
/*		bug( "Act: missing arg2 for code %d.", *str );*/
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "Act: bad code %d.", *str );
			  i = " <@@@> ";				break;
		/* Thx alex for 't' idea */
		case 't': i = (char *) arg1;				break;
		case 'T': i = (char *) arg2;          			break;
		case 'n': i = PERS( ch,  to  );				break;
		case 'N': i = PERS( vch, to  );				break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'M': i = him_her [URANGE(0, vch ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'S': i = his_her [URANGE(0, vch ->sex, 2)];	break;

		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'P':
		    i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    break;

		case 'd':
		    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
		    {
			i = "door";
		    }
		    else
		    {
			one_argument( (char *) arg2, fname );
			i = fname;
		    }
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';

	if (is_fam)
	{
	    if (to->in_room != ch->in_room && familiar != NULL &&
		familiar->in_room == ch->in_room)
		send_to_char("[ ", to);
	    else
	    {
		to = to_old;
		continue;
	    }
	}

	buf[0]   = UPPER(buf[0]);
	if (to->desc && (to->desc->connected == CON_PLAYING))
	write_to_buffer( to->desc, buf, point - buf );

	if (is_fam) to = to_old;

    }
    return;
}

void act2( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *to;

    CHAR_DATA *to_old;

    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    CHAR_DATA *familiar = NULL;
    CHAR_DATA *wizard = NULL;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;

    bool is_fam;

    bool is_ok;
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL || vch->in_room == NULL)
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {

	is_fam = FALSE;
	to_old = to;

	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;

	if ( to->desc == NULL && IS_NPC(to) && (wizard = to->wizard) != NULL )
	{
	    if (!IS_NPC(wizard) && ((familiar=wizard->pcdata->familiar) != NULL)
		&& familiar == to)
	    {
		if (to->in_room == ch->in_room && 
		    wizard->in_room != to->in_room)
		{
		    to = wizard;
		    is_fam = TRUE;
		}
	    }
	}

	if ( to->desc == NULL || !IS_AWAKE(to) )
	{

	    if (is_fam) to = to_old;

	    continue;
	}

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok)
	    {

		if (is_fam) to = to_old;

		continue;
	    }
    	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {

/*		bug( "Act: missing arg2 for code %d.", *str );
*/
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  i = " "; break;
		case 'n': if ( ch != NULL ) i = PERS( ch,  to  );
			  else i = " ";
			  break;
		case 'N': if ( vch != NULL ) i = PERS( vch,  to  );
			  else i = " ";
			  break;
		case 'e': if ( ch != NULL ) i=he_she  [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'E': if (vch != NULL ) i=he_she  [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 'm': if ( ch != NULL ) i=him_her [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'M': if (vch != NULL ) i=him_her [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 's': if ( ch != NULL ) i=his_her [URANGE(0, ch  ->sex, 2)];
			  else i = " ";
			  break;
		case 'S': if (vch != NULL ) i=his_her [URANGE(0, vch  ->sex,2)];
			  else i = " ";
			  break;
		case 'p':
		    if (obj1 != NULL)
		    {
			i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    }
		    else i = " ";
		    break;

		case 'P':
		    if (obj2 != NULL)
		    {
			i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    }
		    else i = " ";
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';

	if (is_fam)
	{
	    if (to->in_room != ch->in_room && familiar != NULL &&
		familiar->in_room == ch->in_room)
		send_to_char("[ ", to);
	    else
	    {
		to = to_old;
		continue;
	    }
	}

	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );

	if (is_fam) to = to_old;

    }
    return;
}



void kavitem( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char kav[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    const char *str;
    const char *i;
    char *point;
    bool is_ok;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL )
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {
	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok) continue;
    	}
	if ( type == TO_CHAR && to != ch )
	    continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) )
	    continue;
	if ( type == TO_ROOM && to == ch )
	    continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) )
	    continue;

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
		i = "";
	    else
	    {
		switch ( *str )
		{
		default:  i = "";					break;
		case 'n': i = PERS( ch,  to  );				break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'o':
		    if (obj1 != NULL) sprintf(kav,"%s's",obj1->short_descr);
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "your" : kav)
			    : "something's";
		    break;

		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );
    }

    return;
}
void bust_a_header(DESCRIPTOR_DATA *d)
{
char class[MAX_STRING_LENGTH];
char class2[MAX_STRING_LENGTH];
char header[MAX_STRING_LENGTH];
char header1[MAX_STRING_LENGTH];
char blanklin[MAX_STRING_LENGTH];
CHAR_DATA *ch;
char cls[MAX_STRING_LENGTH];
sprintf(cls," ");

ch=d->character;
if (ch==NULL) return;
ADD_COLOUR(ch,cls,NORMAL);

if (IS_CLASS(ch,CLASS_VAMPIRE)) sprintf(class,"Vampire");
else if (IS_CLASS(ch,CLASS_DEMON)) sprintf(class,"Demon");
else if (IS_CLASS(ch,CLASS_WEREWOLF)) sprintf(class,"Werewolf");
else sprintf(class,"Classless"); 
sprintf(class2,"%s the %s",ch->name,class);
sprintf(blanklin," ");
sprintf(header1,"%-30s Align:%-4d",class2,ch->alignment);
sprintf(header,
"\0337\033[1;1H\033[1;44m\033[1;37m%-79s%s\0338",header1,cls);
send_to_char(header,ch);
return;
}
/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( DESCRIPTOR_DATA *d )
{
         CHAR_DATA *ch;
         CHAR_DATA *victim;
         CHAR_DATA *tank;
   const char      *str;
   const char      *i;
         char      *point;
         char       buf  [ MAX_STRING_LENGTH ];
         char       buf2 [ MAX_STRING_LENGTH ];
	 bool       is_fighting = TRUE;

   if ( ( ch = d->character ) == NULL ) return;
   if ( ch->pcdata == NULL )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }
   if ( ch->position == POS_FIGHTING && ch->cprompt[0] == '\0' )
   {
      if ( ch->prompt[0] == '\0' )
      {
         send_to_char( "\n\r\n\r", ch );
         return;
      }
      is_fighting = FALSE;
   }
   else if ( ch->position != POS_FIGHTING && ch->prompt[0] == '\0' )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }

   point = buf;
   if ( ch->position == POS_FIGHTING && is_fighting )
      str = d->original ? d->original->cprompt : d->character->cprompt;
   else
      str = d->original ? d->original->prompt : d->character->prompt;
   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
         case 'h' :
            sprintf( buf2, "%d", ch->hit );
            COL_SCALE(buf2, ch, ch->hit, ch->max_hit);
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%d", ch->mana                              );
            COL_SCALE(buf2, ch, ch->mana, ch->max_mana);
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana                          );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'v' :
            sprintf( buf2, "%d", ch->move                              ); 
            COL_SCALE(buf2, ch, ch->move, ch->max_move);
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move                          );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'x' :
            sprintf( buf2, "%2.0f", ch->exp                               );
//            COL_SCALE(buf2, ch, ch->exp, 1000);
            i = buf2; break;
         case 'g' :
            sprintf( buf2, "%2.0f", ch->gold                              );
//            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'q' :
            sprintf( buf2, "%d", ch->pcdata->quest                     );
//            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'f' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else
            {
               if ((victim->hit*100/victim->max_hit) < 25)
               {strcpy(buf2, "Awful");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((victim->hit*100/victim->max_hit) < 50)
               {strcpy(buf2, "Poor");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((victim->hit*100/victim->max_hit) < 75)
               {strcpy(buf2, "Fair");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((victim->hit*100/victim->max_hit) < 100)
               {strcpy(buf2, "Good");ADD_COLOUR(ch, buf2, YELLOW);}
               else if ((victim->hit*100/victim->max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'F' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else if ( ( tank = victim->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else
            {
               if ((tank->hit*100/tank->max_hit) < 25)
               {strcpy(buf2, "Awful");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((tank->hit*100/tank->max_hit) < 50)
               {strcpy(buf2, "Poor");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((tank->hit*100/tank->max_hit) < 75)
               {strcpy(buf2, "Fair");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((tank->hit*100/tank->max_hit) < 100)
               {strcpy(buf2, "Good");ADD_COLOUR(ch, buf2, YELLOW);}
               else if ((tank->hit*100/tank->max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'n' :
            if ( ( victim = ch->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else
            {
               if ( IS_AFFECTED(victim, AFF_POLYMORPH) )
                  strcpy(buf2, victim->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, victim->short_descr);
               else
                  strcpy(buf2, victim->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
         case 'N' :
            if ( ( victim = ch->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else if ( ( tank = victim->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else
            {
               if ( ch == tank )
                  strcpy(buf2, "You");
               else if ( IS_AFFECTED(tank, AFF_POLYMORPH) )
                  strcpy(buf2, tank->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, tank->short_descr);
               else
                  strcpy(buf2, tank->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
         case 'a' :
            sprintf( buf2, "%s", IS_GOOD( ch ) ? "good"
		                  : IS_EVIL( ch ) ? "evil" : "neutral" );
//            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'A' :
            sprintf( buf2, "%d", ch->alignment                      );
//            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 't' :
            sprintf(buf2, "%d", ch->fight_timer			  );
//            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'w' :
            sprintf(buf2, "%d", ch->wish_timer                   );
            i = buf2; break;
         case 'k' :
            if (!IS_CLASS(ch, CLASS_SHAPESHIFTER) || IS_NPC(ch))
            {
              sprintf(buf2, " ");
              i = buf2; 
              break;
            }
            else
            {
              sprintf(buf2, "%d", ch->pcdata->powers[SHAPE_COUNTER]);
              ADD_COLOUR(ch, buf2, L_CYAN);
              i = buf2;
              break;
            }
         case 'r' :
            if( ch->in_room )
               sprintf( buf2, "%s", ch->in_room->name                  );
            else
               sprintf( buf2, " "                                      );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'R' :
            if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_WEREWOLF) || IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_NINJA) ) )
            {
               sprintf( buf2, "%d", ch->rage);
               ADD_COLOUR(ch, buf2, D_RED);
            }
            else strcpy( buf2, "0" );
            i = buf2; break;
         case 'b' :
            sprintf( buf2, "%d", ch->beast );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'B' :
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
            {
               sprintf( buf2, "%d", ch->pcdata->condition[COND_THIRST] );
               ADD_COLOUR(ch, buf2, D_RED);
            }
           else if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
			{
			   sprintf( buf2, "%d", ch->pcdata->condition[COND_FULL] );
			   ADD_COLOUR(ch, buf2, D_RED);
			}
            else strcpy( buf2, "0" );
            i = buf2; break;
         case 'c' :
            sprintf( buf2, "%d", char_ac(ch) );
            i = buf2; break;
         case 'p' :
            sprintf( buf2, "%d", char_hitroll(ch) );
            COL_SCALE(buf2, ch, char_hitroll(ch), 200);
            i = buf2; break;
         case 'P' :
            sprintf( buf2, "%d", char_damroll(ch) );
            COL_SCALE(buf2, ch, char_damroll(ch), 200);
            i = buf2; break;
         case 's' :

            if (!IS_NPC(ch) && ch->pcdata->stage[2]+25 >= ch->pcdata->stage[1]
		&& ch->pcdata->stage[1] > 0)
            {
               sprintf( buf2, "yes" );
               ADD_COLOUR(ch, buf2, WHITE);
            }
            else strcpy( buf2, "no" );
            i = buf2; break;
         case 'O' :
            if ( ( victim = ch->pcdata->partner ) == NULL )
	       strcpy( buf2, "no" );
            else if (!IS_NPC(victim) && victim != NULL && victim->pcdata->stage[1] > 0
		&& victim->pcdata->stage[2]+25 >= victim->pcdata->stage[1])
            {
               sprintf( buf2, "yes" );
               ADD_COLOUR(ch, buf2, WHITE);
            }
            else strcpy( buf2, "no" );
            i = buf2; break;
         case 'l' :
            if ( ( victim = ch->pcdata->partner ) == NULL )
	       strcpy( buf2, "Nobody" );
            else
            {
               if ( IS_AFFECTED(victim, AFF_POLYMORPH) )
                  strcpy(buf2, victim->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, victim->short_descr);
               else
                  strcpy(buf2, victim->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
        case '%' :
            sprintf( buf2, "%%"                                        );
            i = buf2; break;
      } 
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;      
   }
   write_to_buffer( d, buf, point - buf );
   return;
}


/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif
/*

int len(char s)
{
	int l = 0;
	int x;

	if (s[0] == '\0') return 0;

	for (x = 0; x < length(s); x++)
	{
		if (s[x] == '#')
		{
			if s[x+1
		}

	}

	return l;
}
*/

/* OLC, new pager for editing long descriptions. */
/* ========================================================================= */
/* - The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom for porting  */
/*   this SillyMud code for MERC 2.0 and laying down the groundwork.         */
/* - Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which the improvements*/
/*   to the pager was modeled from.  - Kahn                                  */
/* - Safer, allows very large pagelen now, and allows to page while switched */
/*   Zavod of jcowan.reslife.okstate.edu 4000.                               */
/* ========================================================================= */

void show_string( DESCRIPTOR_DATA *d, char *input )
{
    char               *start, *end;
    char                arg[MAX_INPUT_LENGTH];
    int                 lines = 0, pagelen;

    /* Set the page length */
    /* ------------------- */

    pagelen = d->original ? d->original->pcdata->pagelen
                          : d->character->pcdata->pagelen;

    /* Check for the command entered */
    /* ----------------------------- */

    one_argument( input, arg );

    switch( UPPER( *arg ) )
    {
        /* Show the next page */

        case '\0':
        case 'C': lines = 0;
                  break;
        
        /* Scroll back a page */

        case 'B': lines = -2 * pagelen;
                  break;

        /* Help for show page */

        case 'H': write_to_buffer( d, "B     - Scroll back one page.\n\r", 0 );
                  write_to_buffer( d, "C     - Continue scrolling.\n\r", 0 );
                  write_to_buffer( d, "H     - This help menu.\n\r", 0 );
                  write_to_buffer( d, "R     - Refresh the current page.\n\r",
                                   0 );
                  write_to_buffer( d, "Enter - Continue Scrolling.\n\r", 0 );
                  return;

        /* refresh the current page */

        case 'R': lines = -1 - pagelen;
                  break;

        /* stop viewing */

        default:  free_string( d->showstr_head );
                  d->showstr_head  = NULL;
                  d->showstr_point = NULL;
                  return;
    }

    /* do any backing up necessary to find the starting point */
    /* ------------------------------------------------------ */

    if ( lines < 0 )
    {
        for( start= d->showstr_point; start > d->showstr_head && lines < 0;
             start-- )
            if ( *start == '\r' )
                lines++;
    }
    else
        start = d->showstr_point;

    /* Find the ending point based on the page length */
    /* ---------------------------------------------- */

    lines  = 0;

    for ( end= start; *end && lines < pagelen; end++ )
        if ( *end == '\r' )
            lines++;

    d->showstr_point = end;

    if ( end - start )
        write_to_buffer( d, start, end - start );

    /* See if this is the end (or near the end) of the string */
    /* ------------------------------------------------------ */

    for ( ; isspace( *end ); end++ );

    if ( !*end )
    {
        free_string( d->showstr_head );
        d->showstr_head  = NULL;
        d->showstr_point = NULL;
    }

    return;
}

#if defined( WIN32 )
void gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif

/*
 * Removed to make room for Xkilla's copyover code.
 */
/*void logf (char * fmt, ...)
{
	char buf [2*MSL];
	va_list args;
	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);
	
	log_string (buf);
}
*/

/* source: EOD, by John Booth <???> */
/* stick this in in comm.c somewhere */
/* Remember to include <stdargs.h> */

void printf_to_char (CHAR_DATA *ch, char *fmt, ...)
{
	char buf [MAX_STRING_LENGTH];
	va_list args;
	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);
	
	send_to_char (buf, ch);
}

/*
#ifdef CRASHGUARD
void init_signals()
{
  struct sigaction setup_action;
  sigset_t block_mask;
    
  sigemptyset (&block_mask);
  sigaddset (&block_mask, SIGINT);
  sigaddset (&block_mask, SIGQUIT);
  setup_action.sa_handler = sig_handler;
  setup_action.sa_mask = block_mask;
  setup_action.sa_flags = 0;
  sigaction (SIGBUS, &setup_action, NULL);
  sigaction (SIGTERM, &setup_action, NULL);
  //sigaction (SIGABRT, &setup_action, NULL);
//  sigaction (SIGSEGV, &setup_action, NULL);
}
void sig_handler(int sig)
{
  
        switch(sig)
  {
    case SIGBUS:
      bug("Sig handler SIGBUS.",0);
      do_crashguard(); 
      break;
    case SIGTERM:
      bug("Sig handler SIGTERM.",0);
      do_crashguard(); 
      break;
    case SIGABRT:
      bug("Sig handler SIGABRT.",0);
     do_crashguard();
      break;
    case SIGSEGV:
      bug("Sig handler SIGSEGV.",0);
      do_crashguard(); 
      break;
  }
}

#endif
*/
