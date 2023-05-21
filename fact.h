#ifndef FACT_H
#define FACT_H

#include "forma.h"
#include "corps.h"

typedef struct fact
    {
    unsigned int id;
    char name[255];
    char desc[1000];

    unsigned int nforma;
    forma formas[1000];
    unsigned int narmies;
    corps armies[100];

    int money;
    int food;
    int ctr;
    
    } fact;

fact invalid_fact()
    {
    fact res;
    res.id = -1;
    strcpy(res.name, "INVALID");
    strcpy(res.desc, "INVALID");
    return res;
    }


typedef struct fl_base* flist;

typedef struct fl_base
    {
    fact top;
    flist next;
    } fl_base;

void add_fact(flist* fl, fact n)   
    {
    flist res = malloc(sizeof(fl_base));
    res->top = n;
    res->next = *fl;
    *fl = res;
    }

int has_fact(fact* fl, unsigned int size, unsigned int id)
    {
    for (int i = 0; i < size; i++)
	if (fl[i].id == id) return 1;
    return 0;
    }

fact get_fact_info(fact* fl, unsigned int size, unsigned int id)
    {
    for (int i = 0; i < size; i++)
	if (fl[i].id == id) return fl[i];
    fprintf(stderr, "WARNING: faction of id %d was not initialized.", id);
    return invalid_fact();
    }





#endif
