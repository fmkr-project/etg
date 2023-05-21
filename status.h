#ifndef STATUS_H
#define STATUS_H

#include <stdio.h>

#include "frame.h"


void aled()
    {
    printf("h: this\n");
    printf("\n");
    printf("f: fact\n");
    printf("n: node\n");
    printf("r: route\n");
    printf("\n");
    printf("t: tr\n");
    printf("c: cps\n");
    printf("m: mil\n");
    printf("\n");
    printf("0: fin\n");
    }


void save_data(char* savedir, dataframe* df)
    {
    char saveloc[255];
    char savename[255];
    strcpy(saveloc, savedir);

    printf("save name ? ");
    fgets(savename, 255, stdin);
    savename[strcspn(savename, "\n")] = 0;
    
    strcat(saveloc, savename); // Format : ./saves/savename/
    save_dataframe(saveloc, df);
    }

void load_data(char* savedir, dataframe* df)
    {
    char saveloc[255];
    char savename[255];
    strcpy(saveloc, savedir);

    printf("save name ? ");
    fgets(savename, 255, stdin);
    savename[strcspn(savename, "\n")] = 0;
    
    strcat(saveloc, savename); // Format : ./saves/savename/
    strcat(saveloc, "/");
    load_savedata(saveloc, df);
    }


void unroll_nodes(dataframe* df)
    {
    char* buil = "None";
    for (int i = 0; i < (df->nodes-1); i++)
	{
	if (strcmp(df->nodelist[i].name, "INVALID"))
	    printf("%4d|%26s|%17s|%5s|%6d|%5d|%2ds+|%2df+\n",
		i,
		df->nodelist[i].name,
		get_fact_info(df->factlist, df->factions, df->nodelist[i].owner).name,
		df->nodelist[i].type,
		df->nodelist[i].hp,
		df->nodelist[i].curr,
		df->nodelist[i].sgrowth,
		df->nodelist[i].fgrowth);
	}
    printf("\n");
    }

void unroll_fact(dataframe* df)
    {
    for (int i = 0; i < df->factions; i++)
	{
	if (strcmp(df->factlist[i].name, "INVALID")) printf("%s\n", df->factlist[i].name);
	}
    printf("\n");
    }

void unroll_route(dataframe* df)
    {
    for (int i = 0; i < df->routes; i++)
	{
	if (strcmp(df->routelist[i].name, "INVALID"))
	    printf("%4d|%26s|%20s|%20s|%5d\n",
		i,
		df->routelist[i].name,
		df->nodelist[df->routelist[i].from].name,
		df->nodelist[df->routelist[i].to].name,
		df->routelist[i].len);
	}
    printf("\n");
    }

void unroll_armies(dataframe* df)
    {
    char id[4]; char nd[4]; char rt[4];
    for (int i = 0; i < df->factlist[1].narmies; i++)
	{
	corps *hl = &(df->factlist[1].armies[i]);
	sprintf(id, "%d", hl->id);
	sprintf(nd, "%d", hl->cur_node);
	sprintf(rt, "%d", hl->cur_route);
	printf("%3s|%4s|%4s|%5dp %5dl %5ds|%s\n",
	    id,
	    !strcmp(nd, "-1") ? "None" : nd,
	    !strcmp(rt, "-1") ? "None" : rt,
	    hl->det.pnum,
	    hl->det.lnum,
	    hl->det.snum,
	    c_components(*hl));
	}
    }



#endif
