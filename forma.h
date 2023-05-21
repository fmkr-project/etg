#ifndef FORMA_H
#define FORMA_H

typedef struct forma
    {
    int id;
    int cur_co;
    int cur_node;
    int cur_route;
      
    unsigned int capa;
    unsigned int curr;
    char type[10];

    } forma;

int indep_forma(forma f)
    {
    return (f.cur_co == -1);
    }

#endif
