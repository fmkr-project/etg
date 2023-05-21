#include "forma.h"
#include "corps.h"
#include "debug.h"
#include "status.h"

void new_forma(dataframe* df, unsigned int node_id)
    {
    char type[10];
    char s_capa[10];
    char* s_capa_tr;
    int capa;
    
    // ownership test
    if (df->factlist[1].nforma >= 1000)
	{
	printf("overflow\n");
	return;
        }	
    if (df->nodelist[node_id].owner != 1) printf("tg\n");
    else
	{
	// facility test
	if (df->nodelist[node_id].type == "None") printf("non lol\n");
	else
	    {
	    while (1)
		{
		printf("combien ? ");
		fgets(s_capa, 10, stdin);
		s_capa[strcspn(s_capa, "\n")] = 0;
		printf("\n");
		capa = strtol(s_capa, &s_capa_tr, 0);
		if (strcmp(s_capa_tr, ""))
		    {
		    printf("invalid input\n");
		    continue;
		    }
		if (capa > df->nodelist[node_id].curr)
		    {
		    printf("manque\n");
		    continue;
		    }
		if (capa == -1)
		    {
		    printf("annule\n");
		    return;
		    }
		if (capa == 0) continue;
		break;
		}
	    while (strcmp(type, "p") && strcmp(type, "l") && strcmp(type, "s"))
		{
		printf("type ? ");
		fgets(type, 10, stdin);
		type[strcspn(type, "\n")] = 0;
		printf("\n");
		}

	    forma res;
	    res.id = df->factlist[1].nforma;
	    res.capa = capa;
	    res.curr = res.capa;
	    res.cur_node = node_id;
	    res.cur_route = -1;
	    res.cur_co = -1;
	    strcpy(res.type, type);

	    df->factlist[1].formas[df->factlist[1].nforma] = res;
	    df->factlist[1].nforma++;
	    df->nodelist[node_id].curr -= res.capa;
	    
	    printf("Created formation (%s) of capacity %d.\n", type, capa);
	    }
	}
    }

void new_corps(dataframe* df, unsigned int node_id)
    {
    char s_nf[100];
    char* s_nf_tr;
    int nf;
    
    corps cp;
    cp.id = df->factlist[1].narmies;
    cp.leader = -1;
    cp.nforma = 0;
    cp.cur_node = node_id;
    cp.cur_route = -1;
    cp.cur_dep = -1;
    cp.cur_dest = -1;
    cp.spd = 1200;
    init_flags(&cp);

    while (1)
	{
	for (int i=0; i < df->factlist[1].nforma; i++)
	    {
	    forma* res = &(df->factlist[1].formas[i]);
	    if (res->cur_node == node_id
		&& !forma_check(cp, res->id)
		&& indep_forma(*res))
		printf("%4d|%5d of %s\n",
		    res->id,
		    res->curr,
		    res->type);
	    }
	
	printf("add ? ");
	fgets(s_nf, 100, stdin);
	s_nf[strcspn(s_nf, "\n")] = 0;
	printf("\n");
	if (!strcmp(s_nf, "q")) break;
	nf = strtol(s_nf, &s_nf_tr, 0);
	if (strcmp(s_nf_tr, "")) printf("invalid input\n");
	if (nf < 0) continue;
	
	if (df->factlist[1].formas[nf].cur_node == node_id
	    && !forma_check(cp, df->factlist[1].formas[nf].id)
	    && indep_forma(df->factlist[1].formas[nf]))
	    {
	    forma new = df->factlist[1].formas[nf];
	    new.cur_co = cp.id;
	    
	    add_forma(&cp, df->factlist[1].formas[nf]);
	    df->factlist[1].formas[nf] = new;
	    }
	}
    if (cp.nforma != 0)
	{
	refresh_compo(&cp);
	df->factlist[1].armies[df->factlist[1].narmies] = cp;
	df->factlist[1].narmies++;
	}
    }


void training_menu(dataframe* df)
    {
    char action[100];
    char s_arg2[100];
    char* s_arg2_tr;
    int arg2;
    
    printf("f: forma\n");
    printf("action ? ");
    fgets(action, 100, stdin);
    action[strcspn(action, "\n")] = 0;
    
    if (!strcmp(action, "f"))
	{
	printf("où ? ");
	fgets(s_arg2, 100, stdin);
	s_arg2[strcspn(s_arg2, "\n")] = 0;
	printf("\n");
	arg2 = strtol(s_arg2, &s_arg2_tr, 0);
	if (strcmp(s_arg2_tr, ""))
	    {
	    printf("invalid input\n");
	    }
	else if (arg2 >= 0) new_forma(df, arg2);
	}
	
    return;
    }

void corps_menu(dataframe* df)
    {
    char action[100];
    char s_arg2[100];
    char* s_arg2_tr;
    int arg2;

    printf("a: ass\n");
    printf("i: all\n");

    printf("action ?");
    fgets(action, 100, stdin);
    action[strcspn(action, "\n")] = 0;

    if (!strcmp(action, "a"))
	{
	printf("où ? ");
	fgets(s_arg2, 100, stdin);
	s_arg2[strcspn(s_arg2, "\n")] = 0;
	printf("\n");
	arg2 = strtol(s_arg2, &s_arg2_tr, 0);
	if (strcmp(s_arg2_tr, ""))
	    {
	    printf("invalid input\n");
	    }
	else if (arg2 >= 0) new_corps(df, arg2);
	}
    
    if (!strcmp(action, "i")) unroll_armies(df);
    
    return;
    }
