/*
 *  (c) Copyright 2005, 2007, 2008 Christopher J. McKenzie under the terms of the
 *      GNU Public License, incorporated herein by reference
 */
#ifndef _SERVER_
#define _SERVER_

#include "defines.h"

#define errout(in)	exit(printf("ERROR:%s()!\n",in))

char serverstart()
{ 
	int ret;
	int val = 1;
	struct 	sockaddr_in 	name;
	struct	hostent 	*gethostbyaddr();
	g_addr.sa_family = AF_INET;
	strcpy(g_addr.sa_data, "somename");

	name.sin_family = AF_INET;
	name.sin_port = htons(g_port);
	name.sin_addr.s_addr = INADDR_ANY;

	if((g_main.fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		errout("socket");
	}

	ret = setsockopt(g_main.fd, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(int));

	if(bind(g_main.fd, (struct sockaddr*)&name, sizeof(name)) < 0)
	{
		printf("Could not bind to port: ");
		while(bind(g_main.fd, (struct sockaddr*)&name, sizeof(name)) < 0)
		{	
			if(g_dowait)
			{
				printf(".");
				usleep(990000);
				fflush(stdout);
			}
			else
			{
				printf("%d ", ntohs(name.sin_port));
				name.sin_port += htons(1);
			}
		}
		printf("\n");
	}
	printf("Running on port:\t%d\n", ntohs(name.sin_port));
	g_addrlen = sizeof(g_main.addr);

	if(getsockname(g_main.fd, &g_main.addr, &g_addrlen) < 0)
	{
		errout("getsockname");
	}

	if(listen(g_main.fd, 10) < 0)
	{
		errout("listen");
	}

	fcntl(g_main.fd, F_SETFL, O_NONBLOCK);

	if(curvhost->uselog)	//Should be default
	{
		fprintf(curvhost->flog, "%s|APAC Started\n", timetochar(time(NULL)));
	}

	g_etag = 0;	//Start with etag=0 - this is a dumb idea actually
	return 1;
}

char poll_server()
{	
	int c = 0,
	    p = 0;

	if((c = accept(g_main.fd, 0, 0)) == -1)
	{
		return 0;
	}

	fcntl(c, F_SETFL, O_NONBLOCK);

	for(	p = 0;
		(p<g_maxcon)&&g_connections[p].state&ACTIVE;
		p++);

	if(p == g_maxcon)
	{	
		close(c);
		return 0;
	}

	getpeername(c,&g_connections[p].addr, &g_addrlen);

//Start a new connction
	g_connections[p].state		= ACTIVE;
	g_connections[p].fd		= c;

	g_connections[p].last 		= time(NULL);
	g_connections[p].timeout 	= 5;

	if(g_connections[p].bsize == 0)	
	{	
		g_connections[p].bsize = MEDSMAL;
		g_connections[p].buffer = (char*)malloc(g_connections[p].bsize);
	}
	memset(g_connections[p].buffer, 0, g_connections[p].bsize);

	return 1;
}

void close_connection(connection *toclose)
{ 	
	char*buffer = toclose->buffer;
	int bsize = toclose->bsize;	

	shutdown(toclose->fd, SHUT_RDWR);

	if(toclose->ofd >0)
	{	
		close(toclose->ofd); 
	}
	close(toclose->fd);
	memset(toclose, 0, sizeof(connection));
//Restore buffer
	toclose->buffer = buffer;
	toclose->bsize = bsize;
	return;
}

void poll_connection(connection*topoll)
{	
	int res;	

	if(topoll->state & WRITING)/*need to send data*/
	{
	 	safewrite(topoll, 0, 0);	
	}
	else 
	{	
		res = read(topoll->fd, g_buf, MEDIUM);
	
		if(res > 0)
		{
			g_buf[res] = 0;
			topoll->last = time(NULL);
			process(topoll, g_buf); 
		}
		else if(time(NULL) > (topoll->last + topoll->timeout))
		{
			close_connection(topoll);
		}
	}

	return;
}

void poll_servers()
{
	int i;
	char iswrite = 0;

	while(poll_server());

	for(i = 0;i < g_maxcon;i++)
	{
		if(g_connections[i].state & ACTIVE)
		{
			poll_connection(&g_connections[i]);
		}

		iswrite += (g_connections[i].state & WRITING);
	}
		
	if(!iswrite)
	{
		usleep(400000);
	}

	return;
}

int setvhost(char*host)
{
	int 	ix;

	if(!host)	//This should not be valid
	{	
		curvhost = &vhostdb[0];
		return(0);
	}

	for(ix = 0;ix <= g_vlast;ix++)
	{
		if(!vhostdb[ix].name)
		{
			continue;
		}
		if(!strncmp(host, vhostdb[ix].name, strlen(vhostdb[ix].name)))
		{	
			curvhost = &vhostdb[ix];
			return(1);
		}
	}

	curvhost = &vhostdb[0];	//Couldn't find it, fall back!
	return(1);
}
#endif
