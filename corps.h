#ifndef CORPS_H
#define CORPS_H

#include "bases.h"
#include "forma.h"
#include "pers.h"
#include "route.h"

typedef struct compo
    {
    int pnum;
    int lnum;
    int snum;
    } compo;


typedef struct corps
    {
    int id;
    
    int leader;
    unsigned int nforma;
    forma formas[255];
    compo det;

    int spd; // /30'

    int cur_node;
    int cur_route;
    int cur_dep;
    int cur_dest;
    int rem;

    unsigned char F_siege;

    } corps;



int forma_check(corps cp, unsigned int nf)
    {
    for (int i=0; i < cp.nforma; i++)
	{
	if (cp.formas[i].id == nf) return 1;
	}
    return 0;
    }


void add_forma(corps* cp, forma f)
    {
    if (!forma_check(*cp, f.id))
	{
	cp->formas[cp->nforma] = f;
	cp->nforma++;
	}
    else
        printf("déjà");
    }


char* c_components(corps cp)
    {
    char* res = malloc(1000 * sizeof(char));
    memset(res, 0, sizeof(char));
    char buf[10];
    for (int i=0; i < cp.nforma; i++)
	{
	sprintf(buf, "%d", cp.formas[i].id);
	strcat(res, buf);
	strcat(res, " ");
	}
    return res;
    }


int refresh_pos(corps* cp)
    {
    for (int i=0; i < cp->nforma; i++)
	{
	cp->formas[i].cur_node = cp->cur_node;
	cp->formas[i].cur_route = cp->cur_route;
	}
    
    if (cp->cur_route != -1)
	{
	cp->rem -= cp->spd;
	if (cp->rem < 0)
	    {
	    cp->cur_node = cp->cur_dest;
	    cp->cur_route = -1;
	    cp->cur_dep = -1;
	    cp->cur_dest = -1;
	    return 1;
	    }
	}

    return 0;
    }


void init_flags(corps* cp)
    {
    cp->F_siege = 0;
    }


void refresh_compo(corps* cp)
    {
    cp->det.pnum = 0;
    cp->det.lnum = 0;
    cp->det.snum = 0;
    
    for (int i=0; i < cp->nforma; i++)
	{
	int type = cp->formas[i].type[0];
	switch (type)
	    {
	    case 'p':
		cp->det.pnum += cp->formas[i].curr; break;
	    case 'l':
		cp->det.lnum += cp->formas[i].curr; break;
	    case 's':
		cp->det.snum += cp->formas[i].curr; break;
	    }
	}
    }

int get_capa(corps cp)
    {
    return cp.det.pnum + cp.det.lnum + cp.det.snum;
    }


int calc_spow(corps cp)
    {
    int res = 0;
    res += 2 * cp.det.pnum;
    res += cp.det.lnum;
    res += 0.5 * cp.det.snum;
    return res;
    }


int calc_apow(corps cp, corps adv)
    {
    int res = 0;


    return res;
    }



#endif
