#include <errno.h>
#include <stdlib.h>


void cp_status_tostring(unsigned int cp_id, dataframe* df)
    {
    if (cp_id >= df->factlist[1].narmies)
	{
        printf("invalid id\n");
	return;
	}
    
    corps cp = df->factlist[1].armies[cp_id];

    char res[255];
    // Sur un point
    if (cp.cur_route == -1)
	sprintf(res, "Stationné : %s (%d)",
	    df->nodelist[cp.cur_node].name,
	    cp.cur_node);
    
    // Sur une route
    if (cp.cur_node == -1)
	{
	sprintf(res, "En route %s (%d) > %s (%d)",
	    df->nodelist[cp.cur_dep].name,
	    df->nodelist[cp.cur_dep].id,
	    df->nodelist[cp.cur_dest].name,
	    df->nodelist[cp.cur_dest].id);
	}
    printf("%3d: %s\n", cp_id, res);
    }


void move_cp(dataframe* df, unsigned int cp_id)
    {
    errno = 0;
    char dest[100]; char* dest_tr;
    int corr_nd[100]; int corr_rt[100]; int corr = 0; // Table des correspondances
    int res;
    
    if (cp_id >= df->factlist[1].narmies)
	{
	printf("non\n");
	return;
	}
    corps cp = df->factlist[1].armies[cp_id];
    if (cp.cur_route != -1)
	{
	printf("peux pas\n");
	return;
	}
    
    for (int rt = 0; rt < df->routes; rt++)
	{
	if (df->routelist[rt].from == cp.cur_node)
	    {
	    printf("%4d (dist %d)\n", df->routelist[rt].to, df->routelist[rt].len);
	    corr_nd[corr] = df->routelist[rt].to;
	    corr_rt[corr] = rt;
	    corr++;
	    }
	if (df->routelist[rt].to == cp.cur_node)
	    printf("%4d (dist %d)\n", df->routelist[rt].from, df->routelist[rt].len);
	    corr_nd[corr] = df->routelist[rt].from;
	    corr_rt[corr] = rt;
	    corr++;
	}
    
    while (1)
	{
	printf("où ? ");
	fgets(dest, 100, stdin);
	dest[strcspn(dest, "\n")] = 0;
	printf("\n");
	res = strtol(dest, &dest_tr, 0);
	if (res == -1) return;
	if (strcmp(dest_tr, ""))
	    {
	    printf("invalid input\n");
	    continue;
	    }
        break;
	}

    if (!is_in(get_neighbors(df->routelist, df->routes, cp.cur_node), res))
	{
	printf("inaccessible\n");
	return;
	}

    // Recherche de l'ID de la route dest.
    for (int c = 0; c < corr; c++)
	{
	if (corr_nd[c] == res)
	    {
	    int reqf = get_capa(df->factlist[1].armies[cp_id]) * df->routelist[corr_rt[c]].len / 500;
	    printf("need %d\n", reqf);
	    if(df->factlist[1].food < reqf)
		{
		printf("tg\n");
		return;
		}
	     
	    df->factlist[1].armies[cp_id].cur_node = -1;
	    df->factlist[1].armies[cp_id].cur_route = corr_rt[c];
	    df->factlist[1].armies[cp_id].cur_dest = corr_nd[c];
	    if (df->routelist[corr_rt[c]].from == corr_nd[c])
		df->factlist[1].armies[cp_id].cur_dep = df->routelist[corr_rt[c]].to;
	    else df->factlist[1].armies[cp_id].cur_dep = df->routelist[corr_rt[c]].from;
	    df->factlist[1].armies[cp_id].rem = df->routelist[corr_rt[c]].len;
	    df->factlist[1].food -= reqf;
	    break;
	    }
	}
    }


void siege(dataframe* df)
    {
    char cp[10]; char* cp_tr;
    int res;
    
    while (1)
	{
	printf("qui ? ");
	fgets(cp, 10, stdin);
	cp[strcspn(cp, "\n")] = 0;
	printf("\n");
	res = strtol(cp, &cp_tr, 0);
	if (res == -1) return;
	if (strcmp(cp_tr, ""))
	    {
	    printf("invalid input\n");
	    continue;
	    }
        break;
	}

    if (res >= df->factlist[1].narmies)
	{
	printf("c qui ?\n");
	return;
	}
    if (df->factlist[1].armies[res].cur_node == -1)
	{
	printf("oqp\n");
	return;
	}
    if (df->nodelist[df->factlist[1].armies[res].cur_node].owner == 1)
	{
	printf("maison\n");
	return;
	}
    if (df->factlist[1].armies[res].F_siege == 1)
	{
	printf("tout cramer\n");
	return;
	}

    df->factlist[1].armies[res].F_siege = 1;
    }


void sdamage(dataframe* df, int cp_id)
    {
    int pos = df->factlist[1].armies[cp_id].cur_node;
    df->nodelist[pos].hp -= calc_spow(df->factlist[1].armies[cp_id]);
    printf("bobo\n");

    if (df->nodelist[pos].hp < 0)
	{
	printf("cap\n");
	df->nodelist[pos].hp = 1;
	df->nodelist[pos].owner = PLAYERFAC;
	df->factlist[1].armies[cp_id].F_siege = 0;
	}
    }



void milit_menu(dataframe* df)
    {
    char action[100];
    char arg2[100];
    char* arg2_tr;
    int res;
    
    printf("c: mov cp\n");
    printf("m: strview cp\n");
    printf("s: tout cramer\n");

    while (1)
	{
	printf("action ? ");
	fgets(action, 100, stdin);
	action[strcspn(action, "\n")] = 0;

	if (!strcmp(action, "c"))
	    {
	    printf("qui ? ");
	    fgets(arg2, 100, stdin);
	    arg2[strcspn(arg2, "\n")] = 0;
	    printf("\n");
	    res = strtol(arg2, &arg2_tr, 0);
	    if (!strcmp(arg2_tr, "")) move_cp(df, res);
	    else printf("invalid input\n");
	    }

	if (!strcmp(action, "m"))
	    {
	    for (int i = 0; i < df->factlist[1].narmies; i++)
		{
		cp_status_tostring(i, df);
		printf("\n");
		}
	    }

	if (!strcmp(action, "s"))
	    {
	    siege(df);
	    }

	if (!strcmp(action, "q")) return;
	}
    }
