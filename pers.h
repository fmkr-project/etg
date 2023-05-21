#ifndef PERS_H
#define PERS_H

#include "equip.h"

typedef struct person
    {
    char name[255];
    char sname[255];
    
    int pist;
    int lgun;
    int spdr;
    int agil;

    equip thing;
    
    } person;

typedef struct pl_base* plist;

typedef struct pl_base
    {
    person top;
    plist next;
    } pl_base;



#endif
