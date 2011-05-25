/* Apac MIME database
 *
 *      http://qaa.ath.cx/apac.html
 *
 *  (c) Copyright 2005, 2008 Christopher J. McKenzie under 
 *      the terms of the GNU Public License, incorporated 
 *      herein by reference
 *
 */

#ifndef _MIME_
#define _MIME_

#include "defines.h"

#define MVAL(x)	((g_positions[(int)*x])<<3)
const char g_positions[]=
{
       /*0   1   2   3     4   5   6   7     8   9   A   B    C   D   E   F */
	 0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,   0,  0,  0,  0,	// 0
	 0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,   0,  0,  0,  0,	// 1
	 0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,   0,  4,  0,  0,	// 2
	 1,  1,  1,  2,    2,  2,  3,  3,    3,  4,  0,  0,   0,  0,  0,  4,	// 3

	 0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,   0,  0,  0,  0,	// 4
	 0,  0,  0,  0,    0,  0,  0,  0,    0,  0,  0,  0,   0,  0,  0,  4, 	// 5
	 0,  5,  6,  7,    8,  9, 10, 11,   12, 13, 14, 15,  16, 17, 18, 19,	// 6
	20, 21, 22, 23,   24, 25, 26, 27,   28, 29, 30,  0,   0,  0,  0,  0 	// 7
};

const char g_table_lowercase[256] = 
{
	 /*   0     1     2     3     4     5     6     7      8     9     A     B     C     D     E     F     
	0 */0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,/* 0
	1 */0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,/* 1
	2 */0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,/* 2
	3 */0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,/* 3

	4 */0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,  0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,/* 4
	5 */0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,  0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,/* 5
	6 */0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,  0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,/* 6
	7 */0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,  0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,/* 7

	8 */0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,  0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,/* 8
	9 */0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,  0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,/* 9
	A */0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,  0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,/* A
	B */0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,  0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,/* B

	C */0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,  0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,/* C
	D */0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,  0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,/* D
	E */0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,  0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,/* E
	F */0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,  0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF /* F
	     0     1     2     3     4     5     6     7      8     9     A     B     C     D     E     F    */
};

struct extmime
{
	char	*mimetype,
		*extension,
		size;
};
struct extmime**g_exts = 0;

void dumpmime()
{
	int ix;
	for(ix = 0; ix < SMALL; ix++)
	{
		printf("%4d ",ix);

		if(g_exts[ix])
		{
			printf("%8s - %s\n", g_exts[ix]->extension, g_exts[ix]->mimetype);
		}
		else
		{
			printf("empty\n");
		}
	}
}

void buildmime()
{
	int mimes = 0;

	short ptr = 0,
	      tmp = 0;

	char	*pmime = 0,
		*bpt = 0;

	struct extmime
		*eptr = 0;

	if(mystat(g_fmime, &g_dp))
	{	
		printf("Can't open extensions file %s!\n",g_fmime);
		return;
	}

	mimes = open (g_fmime, 0);
	read (mimes, g_buf, g_dp.st_size);
	close (mimes);
	parse (&g_buf, &g_tok);

	tp = g_tok;

	g_exts = malloc(SMALL * sizeof(void*));

	for(tmp = 0;tmp < SMALL;tmp++)
	{
		g_exts[tmp] = malloc(sizeof(struct extmime));
		g_exts[tmp]->mimetype = 0;
	}
	
	eptr = *g_exts;

	while(tp && *tp)
	{	
		pmime = *tp++;	
		while(*tp && **tp <= ' ')
		{
			tp++;	
		}
		if(!*tp)
		{
			break;
		}
		for(bpt = *tp; *bpt; bpt++)
		{
			*bpt = g_table_lowercase[(int)*bpt];
		}
			
		ptr = MVAL(*tp);

		while(g_exts[ptr]->mimetype)
		{
			do
			{
				ptr++;
			}while(g_exts[ptr]->mimetype);

			if(ptr == (SMALL-1))
			{
				ptr = 0;
			}
		}

		eptr = g_exts[ptr];
		eptr->mimetype = malloc(strlen(pmime));
		strcpy(eptr->mimetype,pmime);
		eptr->size = strlen(*tp);
		eptr->extension = malloc(eptr->size);
		strcpy(eptr->extension,*tp);
		tp++;

		while(*tp && **tp <= ' ')
		{
			tp++;	
		}
		if(!*tp)
		{
			break;
		}
	}
	return;
}

// We get filename.ext
char*getmime(char*path)
{
	static struct extmime*eptr;

	static char	*p_ptr = 0,
			*p_mov = 0,
			ext[SMALL] = {0},
			*ex_ptr = 0;

	static short 	ptr,
			sz,
			start;

	// No mime database, assume text/plain
	if (g_exts == 0)
	{
		return ("text/plain");
	}
	// p_ptr is goes to the end
	p_ptr = path + strlen(path);

	// Search for a "." going backwards
	while (p_ptr != path && *p_ptr != '.')
	{
		p_ptr--;
	}

	// If we went all the way back, it's text/plain
	if (p_ptr == path)
	{
		return ("text/plain");
	}
	// Otherwise, go past the dot
	p_ptr++;

	// ext is our buffer
	ex_ptr = ext;
	p_mov = p_ptr;
	// This makes things lower case
	// Cut off ex_ptr if it goes past SMALL (our allocated buffer)
	for(p_mov = p_ptr; *p_mov && (p_ptr - p_mov) < SMALL; p_mov++, ex_ptr++)
	{
		*ex_ptr = g_table_lowercase[(int)*p_mov];
	}
	// null terminate the ext to search
	*ex_ptr = 0;

	start = ptr = MVAL(ext);
	eptr = g_exts[ptr];

	sz = strlen(ext);
	while(eptr && eptr->mimetype)
	{
		if(sz == eptr->size)	
		{
			if(!strcmp(ext, eptr->extension))
			{
				// This swaps around hash collisions
				// based on popularity in order to
				// speed things up
				if(start + 3 < ptr)
				{
					g_exts[ptr] = g_exts[start];
					g_exts[start] = eptr;
				}
				return(eptr->mimetype);
			}
		}
		// This tests for wrap around (a.k.a exhausted hash)
		if(++ptr == SMALL)
		{
			ptr = 0;
		}
		eptr = g_exts[ptr];
	}

	return("text/plain");
}
#endif
