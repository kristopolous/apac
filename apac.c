/*
 * Apac webserver
 *
 *	Pronounced as if you were going to say "Apache"
 *	but then stopped midway
 *
 *	http://qaa.ath.cx/apac.html
 *
<<<<<<< apac.c
 *  (c) Copyright 2005,2007 Christopher J. McKenzie under
=======
 *  (c) Copyright 2005-2007, Christopher J. McKenzie under
>>>>>>> 1.121
 *	the terms of the GNU Public License, incorporated
 *	herein by reference
 */

#ifndef _APAC_
#define _APAC_

// Converts a hexidecimal character, uppercase or lowercase, to its int equivalent
#define HEX2DEC(in)	(	(in >= 'a') ?\
					(in - 'a') + 10 :\
	       				(in >= 'A') ?\
						(in - 'A') + 10 :\
						in - '0'\
			)

// Returns 1 if the parameter is "true" or "yes"
#define ATOB(in)	(!strcmp(*++in, "true") || !strcmp(*in, "yes"))

// Mallocs space for var
#define ISACODE(var,val)			\
{\
	var=malloc(strlen(*++tp)+10);	\
	strcpy(var,val);		\
}

#define ISANUM(val)((val<='9')&&(val>='0'))

#include "defines.h"
connection * lasttowrite;	//Prevents CPU overloading, see safewrite
unsigned short msg;

char*timetochar(time_t m_time)
{	
	struct tm*gmt=localtime(&m_time);
	static char toret[SMALL];

	sprintf(toret,"%s %02d %02d:%02d:%02d"
		,nl_langinfo(gmt->tm_mon+ABMON_1)
		,gmt->tm_mday,gmt->tm_hour
		,gmt->tm_min,gmt->tm_sec);

	return toret;
}
// Signal handler
void handle_bp(int in)
<<<<<<< apac.c
{
	g_sig=TRUE;
=======
{	
	g_sig=TRUE;
>>>>>>> 1.121
}

void shutitdown(int in)
<<<<<<< apac.c
{
	int i;	
=======
{	
	int i;	
<<<<<<< apac.c
>>>>>>> 1.121
//Shuts down all connections
	for(i=0;i<g_maxcon;i++)
<<<<<<< apac.c
        {
	       if(g_connections[i].state&ACTIVE)
                {       close_connection(&g_connections[i]);
=======
=======

	//Shuts down all connections
	for(i = 0;i < g_maxcon;i++)
>>>>>>> 1.128
        {
	 	if(g_connections[i].state & ACTIVE)
                { 
		  	close_connection(&g_connections[i]);
>>>>>>> 1.121
                }
        }
	if(vhostdb[0].uselog)
<<<<<<< apac.c
	{
		fprintf(vhostdb[0].flog,"%s|APAC Quit\n",
=======
	{	
<<<<<<< apac.c
		fprintf(vhostdb[0].flog,"%s|APAC Quit\n",
>>>>>>> 1.121
=======
		fprintf(vhostdb[0].flog, "%s|APAC Quit\n",
>>>>>>> 1.133
			timetochar(time(NULL)));

		fflush(vhostdb[0].flog);
		fclose(vhostdb[0].flog);
	}

	exit(0);
}

void transfertobuffer(connection *c)
{	
	int r = 0;

	if(c->ofd > 0)	// Is there a file descriptor?
	{
		r=read(c->ofd, c->buffer, c->bsize);
		if(r != -1)
		{
			c->be = c->buffer+r;
			c->bp = c->buffer;// Make sure the pointer is good
		}
		if(r < c->bsize)	//The file should be done
		{
			c->ofd = 0;
		}
	}
}
//
// I need to incorporate the new state part of the struct
// Which should maintain whether it is reading/writing/idle/active
// etc.  It is a better method than testing for nulls
//
//This needs to be rewritten
char safewrite(connection*c,char*data,unsigned int size)
{	
	signed int r = -2;

	//If this is the case, then we are sending buffer data.
	if(data)	// We are not reading from a file
	{
		if(size > c->bsize)
		{
			unsigned int 	oldbsize=c->bsize;

			while(size>c->bsize)	//Increase buffer size appropriately
			{
				c->bsize = c->bsize << 1;
			}

			c->buffer = mrealloc(c->buffer, c->bsize);
			memset(c->buffer + oldbsize, 0, c->bsize-oldbsize);	//redo memory
		}
		memcpy(c->buffer, data, size);
		c->bp = c->buffer;	
		c->be = c->buffer+size;		
		c->state |= WRITING;
	}

	if(c->be == c->bp)
	{
		transfertobuffer(c);
	}
	if((c->be - c->bp) > MEDSMAL)
	{
		r = MEDSMAL;
	}
	else
	{
		r=c->be-c->bp;	// Try to send the rest of the buffer
	}

	if(r == 0)	//there is nothing else to write
	{
		c->state &= ~WRITING;	
		return 1;
	}

	r = write(c->fd, c->bp, r);

	if(g_sig == TRUE)	// EPIPE, close connection
	{
		close_connection(c);
		g_sig = FALSE;
		return 0;
	}
	if(r > -1)	// Write was successful
	{
		c->bp += r;
		lasttowrite = c;
	}//else if(c==lasttowrite);	// We looped
		//usleep(5);
	return 1;
}

// Converts %ab to ascii in a string
// in - string to convert
char*hex2ascii(char *in)
{
	char	*out = 0,	
		*ptr_in = 0,
		*ptr_out = 0,
		*end = 0;

	size_t size_in = strlen(in);

	if(size_in > 0)
	{
		end = in + size_in;
	}
	else
	{
		return in;
	}

	// This is the new string without the escape codes ... it needs certainly less then sz
	out = (char*)mmalloc(size_in * sizeof(char));
	memset(out, 0, size_in * sizeof(char));
		
	ptr_in = in;
	ptr_out = out;
	while(ptr_in < end)
	{
		if(ptr_in[0] == '%')
		{	ptr_in++;
			ptr_out[0] = (HEX2DEC(ptr_in[0]) << 4) + HEX2DEC(ptr_in[1]);
			ptr_in += 2;
		}
		else
		{
			ptr_out[0] = ptr_in[0];
			ptr_in++;
		}
		ptr_out++;
	}
	// null terminate the ptr_out string
	ptr_out[0] = 0;
	memcpy(in, out, 1 + ptr_out - out);
	free(out);
	return in;
}
// Converts a request, such as one with % hex codes or "../" to something that C can understand.
// This also has heuristic checks to make sure that we can't get arbitrary files via a "../" hack

char*unhtmlit(char *in,short sz,short *rt)
{	
	static char
		*res = 0;

	static short 	
		size = SMALL,
		count = 0;

	short	cd = 0;

	char	*ptr = 0,
		*rptr = 0,
		*end = 0;

	count = 0;

	if(sz > 0)
	{
		end = in + sz;
	}
	else
	{
		rt = &sz;
		return in;
	}

	if(!res)
	{
		res = mmalloc(SMALL);
	}
	if(sz > size)
	{	
		size = sz + 2;
		res = mrealloc(res, size);
	}
		
	ptr = in;
	rptr = res;
	while(ptr < end)
	{
// Some things
//	"/..file" is ok
//	"directory/../file" is ok
//	"domain/.." is _not_ ok
		if(ptr[0] == '/')	//Are we cd'ing down or up?
		{
			if(ptr[1] == '.' && ptr[2] == '.')
			{
				if(ptr[3] == '/' || ptr[3] == 0)
				{
					cd--;
				}
				else
				{
					cd++;
				}
			}
			else if(ptr[1] != '/')
			{
				cd++;
			}
			if(cd == -1)	//We went up more times than allowed
			{
				cd++;
				ptr += 3;
			}
			else
			{
				rptr[0] = ptr[0];
				rptr++;
				ptr++;
			}
		}
		else 
		{
			rptr[0] = ptr[0];
			rptr++;
			ptr++;
		}
	}

	*rptr = 0;
	count = rptr-res;
	(*rt) = count;
	return res;
}
	
char isa(char*totest)
{	
	while(*tp&&**tp<' ')
	{
		tp++;
	}
	if(*tp)
	{
		return !strncmp(*tp,totest,strlen(totest));
	}
	else
	{
		return 0;
	}
}

char*globit(char*toglob)
{
	char 	*out,
			*pout,
			tilda=0,
			*ptr=toglob;

	pout=out=mmalloc(strlen(toglob)+strlen(getenv("HOME"))+2);

	while(*ptr)
	{
		if(*ptr=='~'&&(tilda==0))
		{
			strcat(out,getenv("HOME"));
			while(*pout)
			{
				pout++;
			}

			ptr++;
			tilda = 1;
		}
		else
		{
			*(pout++)=*(ptr++);
		}
	}
	return out;
}

struct tm*chrtotm(char*toconvert)
{	
	static struct tm ret;

	static char 
		mon[5] = {0},
	     	day[5] = {0},
	     	n = 0;

	strncpy(day, toconvert,3);
	strncpy(mon, toconvert + 8,3);
	ret.tm_mday = atoi(toconvert + 5);
	ret.tm_year = atoi(toconvert + 12);
	ret.tm_hour = atoi(toconvert + 17);
	ret.tm_min = atoi(toconvert + 20);
	ret.tm_sec = atoi(toconvert + 23);
	ret.tm_isdst = 0;

	for(n = 0; n < 7; n++)
	{
		if(!strcmp(day, nl_langinfo(ABDAY_1 + n)))
		{
			ret.tm_wday = n;
		}
	}

	for(n = 0; n < 12; n++)
	{
		if(!strcmp(mon,nl_langinfo(ABMON_1 + n)))
		{
			ret.tm_mon = n;
		}
	}

	ret.tm_year -= 1900;

	return &ret;
}

char*getdata(int flag, mystat_s dp, char *data, connection *c)
{	
	static time_t m_time;
	static struct tm *gmt;

	switch(flag)
	{
		case DATE:
			time(&m_time);
			gmt=gmtime(&m_time);
			sprintf(g_buf,"%s, %02d %s %d %02d:%02d:%02d GMT"
				,nl_langinfo(gmt->tm_wday + ABDAY_1)
				,gmt->tm_mday 
				,nl_langinfo(gmt->tm_mon + ABMON_1)
				,gmt->tm_year+1900,gmt->tm_hour
				,gmt->tm_min,gmt->tm_sec);
			break;

		case SERV:
			sprintf(g_buf,"Apac %s",VERSION);
			break;	

		case LMOD:
			m_time=dp.st_mtime;
			gmt=gmtime(&m_time);
			sprintf(g_buf,"%s, %02d %s %d %02d:%02d:%02d GMT"
				,nl_langinfo(gmt->tm_wday + ABDAY_1)
				,gmt->tm_mday 
				,nl_langinfo(gmt->tm_mon + ABMON_1)
				,gmt->tm_year+1900,gmt->tm_hour
				,gmt->tm_min,gmt->tm_sec);
			break;

		case ACCE:
			return "bytes";
			break;

		case CONL:
			sprintf(g_buf, "%lld", (int64_t)(g_size?(g_size-c->rstart):strlen(data)));
			break;

		case CONT:
			return g_mimetype;
			break;

		case RANG:
			if(c->rstart)
			{
				sprintf(g_buf,"bytes %d-%d/%d",c->rstart,(int)dp.st_size,(int)dp.st_size);
			}
			else
			{
				sprintf(g_buf,"-%d",strlen(data));
			}
			break;

		case ETAG:
			sprintf(g_buf,"%016lX",c->etag);
			break;

		case CACH:
			sprintf(g_buf,"private");
			break;

		default:
			break;
	}

	return g_buf;
}

#ifdef USECGI
int docgi(char *in, char*args, char **outbuf, unsigned int *outsize)
{	
	int 	inlen = strlen(in),
		argslen = 0,
		torunlen;

	char	*buf = *outbuf,
		*bufend = buf;
	FILE	*cgi;

	char	*torun = 0;

	if(args)
	{
		argslen = strlen(args);
		torunlen = inlen + argslen + 2;
	}
	else
	{
		torunlen = inlen + 1;
	}

	torun=(char*)mmalloc(torunlen);
	memset(torun,' ',torunlen);
	torun[torunlen]=0;
	memcpy(torun,in,inlen);

	if(args)
	{
		memcpy(torun+inlen+1,args,argslen);
	}

	cgi = popen(torun,"r");
	if(cgi == 0 || cgi==(FILE*)-1)
	{
		return(FALSE);	//Couldn't execute for some reason
	}
	while(fgets(bufend, *outsize-(bufend-buf)+1, cgi))
	{
		bufend += strlen(bufend);	//Put c->be at the end

		if(	((bufend-buf))>=	//calculated size
			*outsize)
		{
			*outsize = (*outsize) << 1;
			buf = mrealloc(buf, *outsize);
			memset(buf + ((*outsize) >> 1), 0, (*outsize) >> 1);
			*outbuf = buf;
			bufend = buf+strlen(buf);
		}
	}
	pclose(cgi);
	free(torun);

	if(bufend==buf)	//No output, assume failure
	{
		return(FALSE);
	}
	return(TRUE);
}
#endif

int double_it(void**toResize, int*size)
{
	(*size) *= 2;

	(*toResize) = (void*)mrealloc(*toResize, *size);

	return 1;
}

short get(char **args, connection*c)
{	
	struct tm 	*tm2,	*tm1;

	struct dirent	**alltoshow,	**t;

	DIR	*dir=0;

	int	fin = 0,
		ptp = 0,
		itmp = 0,
		tlen = 0;

	int64_t	sz = 0;

	short	place = 0,
		ret,	rs=MEDIUM;
	
	static unsigned int	dsize;

	static char	*full,	*root,
			*temp,	flag = 0,
			*bpt,	header[MEDIUM],
			*data,	*def = "text/html",
			*query;

	static char iscgi = 0;
	struct tm*gmt;

	if(!root)
	{
		full = mmalloc(SMALL);
		root = mmalloc(MEDIUM);
		data = mmalloc(MEDIUM);
		dsize = MEDIUM;
	}
	memset(data, 0, MEDIUM);
// TODO: Replace with a "WTF ARE YOU DOING?" page
	// Check the request
	if(!args[1] || (args[2] - args[1]) < 0 || (args[2] - args[1]) >> 11)
	{
		return OK;
	}

	// Do hex translation if necessary
	if(*args[3] > 31)	//request with spaces
	{
		for(fin = 3;*args[fin] > 31 && *args[fin];fin++)
		{
			*(args[fin - 1] - 1) = 32;
		}
		args[1] = unhtmlit(args[1], args[fin] - args[1], &ret);
	}
	else 
	{
		args[1] = unhtmlit(args[1], args[2] - args[1], &ret);
	}

	// Increase size of the buffer if necessary
	if(ret > rs)
	{
		rs = ret;
		root = mrealloc(root, rs + strlen(curvhost->docroot) + 10);
	}
	memset(root, 0, rs);
	g_mimetype = def;
	g_size = 0;

	chdir(curvhost->docroot);
	strcpy(root, curvhost->docroot);
	strcat(root, args[1]);

	query = 0;
	temp = root;

	while(*temp)
	{
		if(*temp == '?')
		{
			*temp = 0;
			query = temp + 1;
		}
		if(query)
		{
			if(*temp == '&')
			{
				*temp = 32;
			}
		}
		temp++;
	}
	if(query)
	{	
		query = hex2ascii(query);
	}
	if(mystat(root, &g_dp))//stat fails
	{
		root = hex2ascii(root);	//Dump the hex codes
	}

	// stat is successful, no message has been set
	if(!msg && mystat(root, &g_dp)==0)
	{
		//If we are dealing with a directory	
		if(g_dp.st_mode & S_IFDIR)
		{ 
			temp = root + strlen(root) - 1;	//Go to the end of the string and if it's not a '/' make it one
			if(*temp!='/')
			{	
				*++temp='/';
				msg=MOVED;
				sprintf(data,"<H1>Moved Permanently</H1><a href=%s/>Over here actually</a><br>Apac %s",args[1],VERSION);
			}
			else
			{
				temp++;
//Put on the default page
			}
			strcat(root,curvhost->dfault);
//If the default page isn't there, take it back off
			if(mystat(root, &g_dp)<0)
			{
			 	*temp=0;	
			}
		}
iscgi=0;
#ifdef USECGI
//If the file is owned by me and the permissions are like
//	0007	then this will fail
		if(	g_dp.st_mode&(0x0049)	&&	//Test for execution
			g_dp.st_mode&(0x0024)	&&	//Test for read
			!(g_dp.st_mode&S_IFDIR)
			)	//This should be right
		{	
			if(docgi(root,query,&data,&dsize))
			{
				iscgi=1;
				msg=OK;
			}
		}
#endif
//This does an if-modified-since
		if(g_chead[C_IFMD].value && (query == 0))
		{ 
			tm2	=chrtotm(g_chead[C_IFMD].value);
			tm1	=gmtime(&g_dp.st_mtime);

			if(mktime(tm2) >= mktime(tm1))
			{
				msg = NOT_MODIFIED;
			}
		}
	}
	if(!msg)
	{
		if(mystat(root, &g_dp) == 0)	//If the file exists
		{
			msg = OK;	
			if(!(g_dp.st_mode & S_IFDIR))	//If it's not a directory
			{
				c->ofd = myopen(root, 0);	//Open it
				if(c->ofd == -1)	//Couldn't open it
				{
					msg = FORBIDDEN;
				}
				else
				{	g_mimetype=getmime(root);	//Get the mime type
					g_size=g_dp.st_size;	//Set the global size

					if(g_size==0)
					{
						msg=NO_CONTENT;
//BUG BUG, if a file is zeroed out and partial content is asked
					}
					else			//Look for partial content
					{
						if(c->rstart)
						{
							if(c->rstart>=g_dp.st_size)	//The range cannot be satisfied
							{
								msg=REQ_NORANG;
								close(c->ofd);
								c->ofd=-1;
							}
							else
							{
								lseek(c->ofd,c->rstart,SEEK_SET);
								msg=PARTIAL;
							}
						}
						if(c->ofd>0)
						{
							read(c->ofd,data,MEDSMAL);
						}
					}
				}
			}
			else if(curvhost->showdirs)
			{	
				dir = opendir((const char*)root);

				if(dir)
				{	
					place = scandir(root,&alltoshow,0,alphasort);
					itmp = place;

					t = alltoshow;

					strncpy(full, root, SMALL);
					ptp = strlen(full);
//Resizes data structure for large directories
					if((place * (256+60)) > dsize)
					{	
						while( (place * (256+60)) > dsize)
						{
							dsize = dsize << 1;	
						}
						data = mrealloc(data, dsize);
						memset(data, 0, dsize);
					}
					sprintf(data,"<H1>%s</H1>",args[1]);
					gmt = localtime(&g_dp.st_mtime);
					sprintf(data,"%s<pre>Name\t\t\tLast Modified\tSize<hr color=black height=1><a href=..>Parent Directory</a>\t%02d/%02d %02d:%02d:%02d\t-\n",data,gmt->tm_mon,gmt->tm_mday,gmt->tm_hour,gmt->tm_min,gmt->tm_sec);

					chdir(full);
					flag = 1;
					while(place--)
					{	
						if(flag) 	
						{	
							if(*(*t)->d_name == '.')
							{
								if((*((*t)->d_name + 1)) == 0)
								{ 
									t++;
									continue;
								}
								else if((*((*t)->d_name + 1)) == '.')
								{
									if(*((*t)->d_name + 2) == 0)
									{
										t++;
										flag=0;
										continue;
									}
								}
							}
						}

						full[ptp] = 0;
						bpt = (char*)g_units;
						mystat((*t)->d_name, &g_dp);

						sz = g_dp.st_size;

						while(sz>>10)
						{
							bpt++;
							sz=sz>>10;
						}

						gmt = localtime(&g_dp.st_mtime);

						if(g_dp.st_mode & S_IFDIR)
						{	
							sprintf(data,"%s<a href=\"%s/\">",
								data, (*t)->d_name);

							if((tlen=strlen((*t)->d_name)) > 23)
							{
								strcpy((*t)->d_name+21,"..");
								tlen = 23;
							}

							sprintf(data + strlen(data),"<b>%s</b></a>%s%02d/%02d %02d:%02d:%02d\t[DIR]\n",
								(*t)->d_name, g_tabs + (tlen >> 3)
								,gmt->tm_mon + 1 
								,gmt->tm_mday
								,gmt->tm_hour
								,gmt->tm_min
								,gmt->tm_sec);
						}
						else
						{
							sprintf(data,"%s<a href=\"%s\">",
								data, (*t)->d_name);

							if((tlen=strlen((*t)->d_name)) > 23)
							{
								strcpy((*t)->d_name+21, "..");
								tlen = 23;
							}

							sprintf(data + strlen(data),"%s</a>%s%02d/%02d %02d:%02d:%02d\t%d%c\n",
								(*t)->d_name, g_tabs + (tlen>>3)
								,gmt->tm_mon + 1 
								,gmt->tm_mday
								,gmt->tm_hour
								,gmt->tm_min
								,gmt->tm_sec
								,(int) sz
								,*bpt);
						}
						t++;
					}

					sprintf(data,"%s</pre><a href=http://qaa.ath.cx/apac.html>Apac</a> Server %s",data,VERSION);
					closedir(dir);
					while(itmp--)
					{
						free(alltoshow[itmp]);
					}
					free(alltoshow);
				}
				else
				{	
					sprintf(data,"<body><font size=+4><B>403</B></font><hr color=black><blockquote><font size=+3>Forbidden</font><br><font size=+2>Unable to access %s</font><br><br><a href=http://qaa.ath.cx/apac.html>Apac</a> Server %s</blockquote>",args[1],VERSION);
					msg = FORBIDDEN;
				}
			}
			else msg=0;
		}
		if(!msg)
		{	
			msg = NOT_FOUND;
			strcpy(root, curvhost->docroot);
			strcat(root, curvhost->error);

			if(!mystat(root, &g_dp))
			{
				c->ofd = myopen(root,0);
				g_size = g_dp.st_size;
				read(c->ofd, data, MEDSMAL);
			}
			else
			{
				sprintf(data,"<body><font size=+4><B>404</B></font><hr color=black><blockquote><font size=+3>Document not found</font><br><font size=+2>Unable to find %s</font><br><br><a href=http://qaa.ath.cx/apac.html>Apac</a> Server %s</blockquote>",args[1],VERSION);
			}
		}
	}
	if(!g_size&&!data[0]&&msg!=NOT_MODIFIED)	//Nothing has been written
	{
		sprintf(data,
			"<body><font size=+4><B>%d</B></font><hr color=black><blockquote><font size=+3>%s</font><br><br><a href=http://qaa.ath.cx/apac.html>Apac</a> Server %s</blockquote>", 
			g_retvals[msg].error, 
			g_retvals[msg].message,
			VERSION);
	}
	
// Start of header
	sprintf(header,
		"%s %d %s",
		g_hptr,
		g_retvals[msg].error,
		g_retvals[msg].message);

	for(place = 0; g_head[place]; place++)
	{	
		if(iscgi)
		{	
			if(place==LMOD || place==CONT)
			{
				continue;
			}
		}
		else
		{
			if(place==CACH)
			{
				continue;
			}
		}
		if(!c->ofd)
		{
			if(place==LMOD)
			{
				continue;
			}
		}
		sprintf(header,
			"%s\r\n%s: %s",
			header,
			g_head[place], 
			getdata(place,g_dp,data,c));
	}
	if(msg==MOVED)
	{
		sprintf(header,"%s\r\nLocation: %s/",header,args[1]);
	}

	strcat(header,"\r\n");
	if(!iscgi)
	{
		strcat(header,"\r\n");
	}
// End of header
	if(!safewrite(c,header,strlen(header)))
	{
		return 0;
	}
	c->state|=WRITING;	//Need to write data

	if(g_size)
	{
		if(g_size >= MEDSMAL)
		{
			itmp=MEDSMAL;
		}
		else
		{
		       	itmp=g_size;
		}
//This is needed for directory listings
	}
	else
	{
		itmp=strlen(data);
	}
//This violates the keep-alive default of HTTP/1.1
	safewrite(c,data,itmp);
	return(fin);
}

int process(connection*c, char*in)
{
	int 	chr=0;

	msg=0;
<<<<<<< apac.c
	printf("%s\n",in);
=======

#ifdef _DEBUG
	printf("<%s>",in);
	fflush(0);
#endif // _DEBUG

>>>>>>> 1.121
	parse(&in,&g_tok);

	tp=&g_tok[0];

	if(!isa("GET"))	//This is a problem
	{
		msg=NOT_IMPLEMENTED;
	}

	tp = &g_tok[2];
//	if(isa(g_http[2]))
		c->version=HTTP11;
//	else if(isa(g_http[1]))
//		c->version=HTTP10;
//	else
//		c->version=HTTP09;

	g_hptr = (char*)g_http[(int)c->version];

<<<<<<< apac.c

	for(chr=0;chead[chr].name;chr++)
=======
	for(chr=0;g_chead[chr].name;chr++)
>>>>>>> 1.133
	{
		g_chead[chr].value=0;
	}
	tp = g_tok;


	while(*tp)
	{
		for(chr = 0;g_chead[chr].name;chr++)
		{
			if(!g_chead[chr].value)
			{
				if(isa(g_chead[chr].name))
				{
					g_chead[chr].value = *(tp + 1);
					while(*++tp&&**tp!='\n')
					{
						if(tp[0][0] > ' ') 
						{
							*((*tp)-1) = ' ';
						}
					}
					break;
				}
			}
		}
		if(*tp)
		{
			tp++;
		}
	}
	if(g_chead[C_CONN].value)
	{	
		if(g_chead[C_KEEP].value)
		{	
			if((c->timeout = atoi(g_chead[C_KEEP].value)) > g_maxtimeout)
			{
				c->timeout = g_maxtimeout;
			}
		} 
		else
		{
			c->timeout=10;
		}
	}

// Virtual hosts
	if(g_chead[C_HOST].value)
	{
		setvhost(g_chead[C_HOST].value);
	}

//E-Tag
	c->etag = g_etag;
	g_etag++;

// Continuation 
//	Apparrently the RFC specifies:
//	units=start-<end>
	if(g_chead[C_RANG].value)
	{	
		long bytelength=0; //For the units
		char*ptr = 0;
		ptr=(char*)g_chead[C_RANG].value;	//Beginning of value

		for(chr = 0; g_sizeunits[chr] != (char*)EOD; chr++)
		{	
			if(!strncmp(ptr, g_sizeunits[chr], strlen(g_sizeunits[chr])))
			{
				break;
			}
		}

// Determine the units
		if(g_sizeunits[chr] == (char*)EOD)	//Got to the end
		{	
			bytelength = 1;	//Assume 1
		}
		else
		{	ptr += strlen(g_sizeunits[chr]);
			if(chr<6)
			{
				bytelength = (1 << (10 * chr));
			}
			else
			{
				switch(chr)
				{
					case 7:
					case 8:
						bytelength=0;	//Symbolizing bits
						break;
					case 9:
						bytelength=2;	//word
						break;
					default:
						break;			//Should be unreachable
				}
			}
		}
		if(*ptr!='=')//This is a problem
		{
		}
		else
		{
			ptr++;

			while(ISANUM(*ptr))
			{
				c->rstart*=10;
				c->rstart+=*ptr-'0';
				ptr++;
			}
			c->rstart--;
			if(*ptr!='-')//This is less of a critical error
			{
			}
			else
			{
				ptr++;

				while(ISANUM(*ptr))
				{	
					c->rend*=10;
					c->rend+=*ptr-'0';
					ptr++;
				}
			}
		}
		if(!bytelength)//it's in bits
		{	
			c->rstart=c->rstart<<3;
			c->rend=c->rend<<3;
		}else
		{	
			c->rstart*=bytelength;
			c->rend*=bytelength;
		}

		c->rstart++;
	}
// Writes to the log
	if(curvhost->uselog)
	{ 	
		fprintf(curvhost->flog,"%s|%s|%s|%s|\n",
#ifdef _SunOS
			"N/S",
#else
			inet_ntoa(((struct sockaddr_in*)&c->addr)->sin_addr),
#endif
			timetochar(time(NULL)),
			g_tok[1],
			g_chead[C_USER].value);

		fflush(curvhost->flog);
	}
	return get(g_tok,c);
}

char setup_vhost(char*tosetup)
{	
	//
	// We assume that the memory has yet to be allocated
	//
	int conf = 0;

	char 	*config = 0,
		*localbuf = 0,
		**localtok = 0;

	localbuf=(char*)mmalloc(BIG);
	localtok=(char**)mmalloc(MEDIUM<<1);
	config=(char*)mmalloc(MEDIUM);

	// No bounds check is done here
	sprintf(config,"%s/.apac.conf.%s",getenv("HOME"),tosetup);	//The configuration file.

	if(mystat(config, &g_dp))
	{
		creat(config, 0644);	
		conf = myopen(config, O_WRONLY);
		write(conf, g_defaultconfig, strlen(g_defaultconfig));
		close(conf);
		printf("%s not found, new one written\n", config);
		strcpy(localbuf, g_defaultconfig);
	}
	else
	{
		conf = myopen(config, 0);

		if(g_dp.st_size > BIG)
		{
			localbuf = mrealloc(localbuf, g_dp.st_size + 1);
		}	
		// Load the config into a buffer
		read(conf, localbuf, g_dp.st_size);
		close(conf);
	}	

	// Assign to the current host
	curvhost = &vhostdb[g_vlast];
	g_vlast++;

	// Ran out of space, realloc
	if(g_vlast == g_vhostsize)
	{
		// Double size
		g_vhostsize *= 2;
		vhostdb = mrealloc(vhostdb, g_vhostsize * sizeof(vhost));
		memset(vhostdb + g_vlast * sizeof(vhost), 0, g_vlast * sizeof(vhost));	
	}
	parse(&localbuf, &localtok);
	tp = localtok;

	// This puts the name in the curvhost
	ISACODE(curvhost->name, tosetup);

	tp--;
	while(*tp)
	{
		if(isa("uselog"))
		{
			curvhost->uselog = ATOB(tp);
		}
		if(isa("showdirs"))
		{
			curvhost->showdirs = ATOB(tp);
		}
		if(isa("docroot"))
		{
			ISACODE(curvhost->docroot, globit(*tp));
		}
		if(isa("logfile"))
		{
			ISACODE(curvhost->logfile, globit(*tp));
		}
		if(isa("default"))
		{
			ISACODE(curvhost->dfault, *tp);
		}
		if(isa("error"))
		{
			ISACODE(curvhost->error,*tp);
		}
		if(*tp && **tp == '#')
		{	
			while(tp[0] && tp[0][0] != '\n')
			{
				tp++;
			}
			if(!tp[0])
			{
				break;
			}
		}
		tp++;
	}
	if(!curvhost->docroot || !curvhost->dfault)
	{
		printf("Error! Docroot not defined for virtual host %s\n", tosetup);
		printf("Removing %s from the virtual host lists!\n", tosetup);
		curvhost->name=0;
	}
	if(curvhost->uselog)
	{
		if((curvhost->flog = fopen(curvhost->logfile, "a"))==0)
		{
			printf("Can't open logfile %s, not logging!\n",curvhost->logfile);
			curvhost->uselog=0;
		}
	}

	//Assign to the current host
	curvhost = &vhostdb[g_vlast];	
	g_vlast++;

	//Ran out of space, realloc
	if(g_vlast == g_vhostsize)	
	{		
		// Double size
		g_vhostsize *= 2;
		vhostdb = mrealloc(vhostdb, g_vhostsize * sizeof(vhost));
		memset(vhostdb + g_vlast * sizeof(vhost), 0, g_vlast * sizeof(vhost));
	}

	free(localbuf);
	free(localtok);
	return(1);
}

char read_config()
{	
	int conf;
	char config[SMALL];
	char**stp;

	// Allocate virtual host database
	
	// Up to 10 virtual hosts by default
	g_vlast = 10;

	// No virtual hosts initially
	g_vhostsize = 0;

	vhostdb = (vhost*) mmalloc(sizeof(vhost) * g_vlast);
	memset(vhostdb, 0, sizeof(vhostdb));

	curvhost = &vhostdb[0];
	g_vlast = 1;

	sprintf(config, "%s/.apac.conf", getenv("HOME"));

	if(mystat(config, &g_dp))
	{
		creat(config, 0644);
		conf = myopen(config, O_WRONLY);
		write(conf, g_defaultconfig, strlen(g_defaultconfig));
		close(conf);
		printf(".apac.conf not found, new one written\n");
		strcpy(g_buf, g_defaultconfig);
	}
	else
	{
		conf = myopen(config, 0);
//File truncation possible
		read(conf, g_buf, g_dp.st_size > BIG ? BIG : g_dp.st_size);
		close(conf);
	}

	parse(&g_buf, &g_tok);
	tp = g_tok;
	g_maxcon = 1;

	while(*tp)
	{	
		if(isa("vhost"))
		{
			stp = tp;	//Saves tp
			setup_vhost(*++tp);
			curvhost = &vhostdb[0];	//Reset vhost to top level
			tp = ++stp;
		}
		if(isa("port"))
		{
			g_port = atoi(*++tp);
		}
		if(isa("maxcon"))
		{
			g_maxcon = atoi(*++tp);
		}
		if(isa("uselog"))
		{
			curvhost->uselog = ATOB(tp);
		}
		if(isa("showdirs"))
		{
			curvhost->showdirs = ATOB(tp);
		}
		if(isa("dowait"))
		{
			g_dowait = ATOB(tp);
		}
		if(isa("docroot"))
		{
			ISACODE(curvhost->docroot, globit(*tp));
		}
		if(isa("mimes"))
		{
			ISACODE(g_fmime, globit(*tp));
		}
		if(isa("logfile"))
		{
			ISACODE(curvhost->logfile, globit(*tp));
		}
		if(isa("default"))
		{
			ISACODE(curvhost->dfault, *tp);
		}
		if(isa("error"))
		{
			ISACODE(curvhost->error, *tp);
		}
		if(*tp && **tp == '#')
		{
			while(tp[0] && tp[0][0] != '\n')
			{
				tp++;
			}
			if(!tp[0])
			{
				break;
			}
		}
		tp++;
	}

	g_connections = mmalloc(g_maxcon * sizeof(connection));
	memset(g_connections, 0, g_maxcon * sizeof(connection));
	g_cend = g_connections + g_maxcon * sizeof(connection);

	if(curvhost->uselog)
	{
		if((curvhost->flog = fopen(curvhost->logfile,"a")) == 0)
		{
			printf("Can't open logfile %s, not logging!\n", curvhost->logfile);
			curvhost->uselog = 0;
		}
	}
	g_sleeptime=1;
	return TRUE;
}

#define PUSHARG	argc--;argv++;
int main(int argc,char*argv[])
{
	char*prog = argv[0];
	short pos;

	g_buf=mmalloc(BIG);
	g_sbuf=mmalloc(SMALL<<1);
	g_tok=(char**)mmalloc(MEDIUM<<1);

	read_config();

	PUSHARG
	while(argc>0)
	{
		if(argv[0][1] == '-')
		{
			pos = 2;
		}
		else
		{
			pos = 1;
		}

		switch((argv[0][pos])|0x20)
		{
		case 's':
			g_dowait=0;
			printf("Using current directory as the docroot\n");
			curvhost->docroot=(char*)mmalloc(SMALL);
			getcwd(curvhost->docroot,SMALL);
			break;

		case 'p':
			g_port = atoi(argv[1]);
			PUSHARG
			break;

		case 'v':
			printf("%s ",VERSION);
#ifdef USECGI
			printf("CGI ");
#endif
#ifdef _SunOS
			printf("Solaris ");
#endif
			printf("\n");
			exit(0);

		case 'h':
			printf("%s\n", prog);
			printf("\t-h\tThis help screen\n");
			printf("\t-p num\tUse the port num as the port to listen on\n");
			printf("\t-s\tUse the current directory as the root\n");
			printf("\t-v\tVersion and build information\n");
			exit(0);
			break;
		}
		PUSHARG
	}

	signal(SIGPIPE, handle_bp);
	signal(SIGTERM, shutitdown);
	signal(SIGQUIT, shutitdown);
	signal(SIGINT, shutitdown);
	signal(SIGHUP, shutitdown);
	signal(SIGABRT, shutitdown);

	if(!serverstart())
	{
		return 0;
	}

	buildmime();

	loop:
		poll_servers();
	goto loop;

	return 1;
}
#endif
