#include "eco.h"

void end_turn(dataframe* df)
    {
    for (int nd = 0; nd < df->nodes; nd++)
	{
	df->nodelist[nd].curr += df->nodelist[nd].sgrowth;
	if (df->nodelist[nd].curr > df->nodelist[nd].capa)
	    df->nodelist[nd].curr = df->nodelist[nd].capa;

	int node_owner = df->nodelist[nd].owner;
	df->factlist[node_owner].food += df->nodelist[nd].fgrowth;
	}
    
    for (int cp = 0; cp < df->factlist[1].narmies; cp++)
	{
	if (refresh_pos(&(df->factlist[1].armies[cp])) == 1)
	    {
	    printf("%d arrive > %s (%d)",
		df->factlist[1].armies[cp].id,
		df->nodelist[df->factlist[1].armies[cp].cur_node].name,
		df->factlist[1].armies[cp].cur_node);
	    }
	if (df->factlist[1].armies[cp].F_siege == 1)
	    {
	    sdamage(df, cp);
	    }
	}

    }
