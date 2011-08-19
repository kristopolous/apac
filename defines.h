/*
 *  See http://qaa.ath.cx/dos.html
 *
 *  (c) Copyright 2005-2008, Christopher J. McKenzie under the terms of the
 *      GNU Public License, incorporated herein by reference
 */
#ifndef _DEFINES_
#define _DEFINES_
#define __USE_LARGEFILE64
// This block is in a specific order
#include <glob.h>
#define __USE_LARGEFILE64
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
// End of specific order

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <langinfo.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <time.h>

#define VERSION		/*BEGIN_VERSIONATE*/"0.2.1305"/*END_VERSIONATE*/
#define BIG		65536
#define MEDLAR		16384
#define MEDIUM		4096
#define MEDSMAL		1024
#define PAGE_SIZE	4096
#define SMALL		256
#define SLEEP		10	//usleep this amount
#define EOD		1	//End of a data structure

#define ACTIVE		0x0001
#define IDLE		0x0002
#define WRITING		0x0004
#define READING		0x0008
#define UNKNOWN		0x1000

#ifdef LINUX
#ifdef __USE_LARGEFILE64
#define mystat_s	struct stat64
#define mystat(a, b)	stat64(a, b)
#define myopen(a, b)	open64(a, b)
#else
#define mystat_s	struct stat
#define mystat(a, b)	stat(a, b)
#define myopen(a, b)	open(a, b)
#endif
#else
#define mystat_s	struct stat
#define mystat(a, b)	stat(a, b)
#define myopen(a, b)	open(a, b)
#endif

#ifdef _DEBUG
#define mmalloc(bytes) malloc(bytes);printf("malloc: %d\t<%s@%d>\n", bytes, __FILE__, __LINE__);
#define mrealloc(pointer, bytes) realloc(pointer, bytes);printf("realloc: %d\t<%s@%d>\n", bytes, __FILE__, __LINE__);
#else //_DEBUG
#define mmalloc(bytes) malloc(bytes)
#define mrealloc(pointer, bytes) realloc(pointer, bytes)
#endif //_DEBUG

enum
{	
	FALSE,	
	TRUE
};

enum
{	
	HTTP09,	
	HTTP10,	
	HTTP11
};

/* RFC 2616 subset 
   Tuple mapping (see above)*/
enum
{ 
	CONTINUE, 	OK, 		NO_CONTENT,	//	1
	PARTIAL,	MOVED,		REDIRECT,	//	2
	NOT_MODIFIED,	BAD_REQUEST, 	FORBIDDEN,	//	3
	NOT_FOUND,	REQ_TIMEOUT,	NOT_IMPLEMENTED,// 	4
	NO_SUPPORT,	REQ_NORANG
};

enum
{
	DATE,	SERV,	LMOD,	//	1
	ACCE,	CONL,	RANG,	//	2
	CONT,	ETAG,	CACH,	//	3
	EXPR
};

enum
{
	C_HOST,	C_USER,	C_ACPT,	//	1
	C_ALNG,	C_AENC,	C_ACHR,	//	2
	C_IFMD,	C_KEEP,	C_CONN,	//	3
	C_RANG
};

typedef struct namevalue_struct
{
	char	*name,
		*value;
}namevalue;

typedef struct retvals_s
{
	short	error;
	char*	message;
} retval;

typedef struct connection_struct
{
	unsigned int	last;		//Last time connection was active
	unsigned short	timeout;	//Timeout before connection closed

	char		version,	//HTTP version
			*buffer,	//Buffer to send
			*bp,	//Buffer pointer
			*be;	//End of buffer	so be-buffer=size

	int		fd,	//The socket descriptor
			ofd,	//The file (being sent) descriptor
			rstart,	//Range start - for continuation
			rend;	//Range end

	size_t	bsize;	// buffer size

	unsigned long	etag;	//unique tag for the request

	unsigned short	state;	//Connection state

	struct sockaddr addr;
} connection;

typedef struct vhost_struct
{	
	char	*name,
		*docroot,
		*error,
		*logfile,
		*dfault;

	char	showdirs,	//Show directory listings for this virtual host
		uselog;		//Use a log?

	FILE*	flog;	//Log file
} vhost;

extern char	g_maxcon,
       		g_dowait,
		*g_buf,
		*g_sbuf,
		*g_default,
		*g_hptr,
		*g_docroot,
		*g_error,
	 	*g_fmime,
		*g_mimetype,
		**g_tok,
		**tp; 		

extern const char
		*g_head[],
		*g_sizeunits[],
		*g_http[],
		*g_defaultconfig,
		*g_tabs,
		*g_units;

extern unsigned short	
		g_port, //Port to run on
		g_maxtimeout;


extern int	g_vhostsize,	// Size of vhost structure
       		g_pends,
		g_vlast,
		errno;	// The last define data structure

extern unsigned int
		g_sleep,
		g_sleeptime,
		g_sig,
		g_itmp,
		g_addrlen;

extern int64_t	g_size,
       		g_etag;

extern vhost		*vhostdb,	// Virtual host database
			*curvhost;	// Current virtual host

extern connection	*g_connections,
			*g_cend,
			g_main;

extern struct timeval  g_stmp;
extern struct sockaddr g_addr;
extern mystat_s	g_dp;
extern retval g_retvals[];
extern namevalue g_chead[];

void buildmime();
void close_connection(connection*);
char *getmime(char*);
char isa(char*);
void parse(char**, char***);
char poll_server();
void poll_servers();
int  process(connection*, char*);
char safewrite(connection*, char*, unsigned int);
char serverstart();
int  setvhost(char*host);
char *timetochar(time_t m_time);

#endif
