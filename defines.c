#include "defines.h"

connection	*g_connections,
		*g_cend,
		g_main;

retval g_retvals[]=
{
	{100,"Continue"},		{200,"OK"},			{204,"No Content"},	//	1
	{206,"Partial content"},	{301,"Moved Permanently"},	{302,"Found"},		//	2
	{304,"Not Modified"},		{400,"Bad Request"}, 		{403,"Forbidden"},	//	3
	{404,"Not Found"},		{408,"Request Timeout"}, 	{501,"Not Implemented"},//	4
	{505,"HTTP Version Not Supported"},	{416, "Requested Range Not Satisfiable"}
};

const char *g_head[]=
{
	"Date",			"Server",		"Last-Modified",	//	1
	"Accept-Ranges",	"Content-Length",	"Content-Range",	//	2
	"Content-Type",		"ETag",			"Cache-Control",	//	3
	0,			"Expires",		0
};

const char *g_sizeunits[]=	//For range specifier
//	Name		Size (bytes)	index
{	"bytes",	//1		[0]
	"kilobytes",	//2^10		[1]
	"megabytes",	//2^20		[2]
	"gigabytes",	//2^30		[3]
	"terabytes",	//2^40		[4]
	"exabytes",	//2^50		[5]
	"bits",		//2^-3		[6]
	"baud",		//2^-3		[7]
	"words",	//2		[8]
	(char*)EOD	//		[9]
};

namevalue g_chead[]=
{ 
	{"Host:",0},			{"User-Agent:",0}, 	{"Accept:",0},		//	1
	{"Accept-Language:",0}, 	{"Accept-Encoding:",0}, {"Accept-Charset:",0},	//	2
	{"If-Modified-Since:",0},	{"Keep-Alive:",0}, 	{"Connection:",0}, 	//	3
	{"Range",0}, 			{0,0}
};


char	g_maxcon = 0,	g_dowait,
		*g_buf, 	*g_sbuf,
		*g_default,	*g_hptr,
		*g_docroot, 	*g_error,
		*g_fmime = ".apac.mime",
		*g_mimetype, 	**g_tok,
		**tp;

const char	*g_http[] =
			{
				"HTTP/0.9", 
				"HTTP/1.0",
				"HTTP/1.1"
			},

		*g_defaultconfig =
			"port 4400\ndocroot ~/web\ndefault index.html\nshowdirs yes\nerror /error.html\nuselog no\nlogfile doesntmatter.txt\nmaxcon 10\nusleep 100\nmimes ~/.apac.mime\ndowait yes",

		*g_tabs = "\t\t\t",
		*g_units = "BKMGTPEY";//For directory listings


unsigned short	g_port, //Port to run on
		g_maxtimeout = 30;

vhost	*vhostdb,	// Virtual host database
	*curvhost;	// Current virtual host

int		g_vhostsize,	// Size of vhost structure
		g_vlast;	// The last define data structure

struct timeval	g_stmp;

unsigned int	g_sleep,
		g_sleeptime,
		g_sig,
		g_itmp,
		g_addrlen;

signed int	g_pends;

int64_t		g_size,
		g_etag;

struct sockaddr g_addr;

mystat_s	g_dp;


