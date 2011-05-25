/*
 *  See http://qaa.ath.cx/apac.html
 *
 *  (c) Copyright 2004, Christopher J. McKenzie under the terms of the
 *      GNU Public License, incorporated herein by reference
 */
#ifndef _PARSE_
#define _PARSE_
#include"defines.h"

char *g_chrs[]=
{       "\000","\001","\002","\003","\004","\005","\006","\007",
	"\010","\011","\012","\013","\014","\015","\016","\017",
	"\020","\021","\022","\023","\024","\025","\026","\027",
	"\030","\031","\032","\033","\034","\035","\036","\037",
	"\040","\041","\042","\043","\044","\045","\046","\047",
	"\050","\051","\052","\053","\054","\055","\056","\057",
	"\060","\061","\062","\063","\064","\065","\066","\067",
	"\070","\071","\072","\073","\074","\075","\076","\077",
	"\100","\101","\102","\103","\104","\105","\106","\107",
	"\110","\111","\112","\113","\114","\115","\116","\117",
	"\120","\121","\122","\123","\124","\125","\126","\127",
	"\130","\131","\132","\133","\134","\135","\136","\137",
	"\140","\141","\142","\143","\144","\145","\146","\147",
	"\150","\151","\152","\153","\154","\155","\156","\157",
	"\160","\161","\162","\163","\164","\165","\166","\167",
	"\170","\171","\172","\173","\174","\175","\176","\177",
};

void parse(char**_in,char***_out)
{	static char	*in,**out,*end;
	in=(*_in);
	out=(*_out);
	end=in+strlen(in);

	if(end==in)
	{	*out=0;
		return;
	}	
	while(*in==32)//if the first character is a space
		++in;//dump it.
	*out=in;

	while(in!=end)
	{	if(*in>32)
		{	in++;
			continue;	
		}
		if(*in!=' ')
		{	if(*out)	
				if(*out[0]>32)
					out++;
			*out=g_chrs[(int)*in];
		}
		*in=0;
		in++;
		if(*in>32)
		{	out++;
			*out=in;
		}	
	}
	out[1]=out[2]=out[3]=0;
	return;
}	

#endif