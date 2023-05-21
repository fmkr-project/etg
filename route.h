#ifndef ROUTE_H
#define ROUTE_H

#include "node.h"
#include "bases.h"

typedef struct route
    {
    unsigned int id;
    char name[255];
    char desc[1000];
    unsigned int len;

    int from;
    int to;
    
    } route;

route invalid_route()
    {
    route res;
    res.id = -1;
    strcpy(res.name, "INVALID");
    strcpy(res.desc, "INVALID");
    res.from = -1;
    res.to = -1;
    return res;
    }

int has_route(route* rl, unsigned int size, unsigned int id)
    {
    for (int i = 0; i < size; i++)
	if (rl[i].id == id) return 1;
    return 0;
    }

route get_route_info(route* rl, unsigned int size, unsigned int id)
    {
    for (int i = 0; i < size; i++)
	if (rl[i].id == id) return rl[i];
    fprintf(stderr, "WARNING: route of id %d was not initialized.", id);
    //return invalid_fact();
    }

intlist get_neighbors(route* rl, unsigned int size, unsigned int id)
    {
    intlist res = NULL;
    for (int i = 0; i < size; i++)
	{
        if (rl[i].from == id) chain (&res, rl[i].to);
	if (rl[i].to == id) chain (&res, rl[i].from);
	}
    return res;
    }


#endif
