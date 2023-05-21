#ifndef NODE_H
#define NODE_H

// rien
#define NONE_CMAX 0
#define NONE_HMAX 1000
#define NONE_FGROWTH 25

// Village
#define V_CMAX 150
#define V_HMAX 2000
#define V_FGROWTH 40

// Village fortifié
#define VF_CMAX 300
#define VF_HMAX 7500
#define VF_FGROWTH 30

// Tour(elle)
#define T_CMAX 400
#define T_HMAX 6000
#define T_FGROWTH 8

// Redoute
#define R_CMAX 750
#define R_HMAX 9000
#define R_FGROWTH 8

// Bastion
#define U_CMAX 2400
#define U_HMAX 11500
#define U_FGROWTH 12

// Fort
#define F_CMAX 1800
#define F_HMAX 20000
#define F_FGROWTH 20

// Fort retranché
#define J_CMAX 1100
#define J_HMAX 38000
#define J_FGROWTH 6

// Fort militaire
#define M_CMAX 3200
#define M_HMAX 13000
#define M_FGROWTH 0

// Barrière
#define B_CMAX 1000
#define B_HMAX 45000
#define B_FGROWTH 0

// Citadelle
#define C_CMAX 5000
#define C_HMAX 100000
#define C_FGROWTH 0



#include "build.h"
#include "debug.h"

typedef struct node
    {
    int id;
    char name[255];
    char desc[1000];

    int owner;

    unsigned int sgrowth;
    unsigned int fgrowth;

    char* type;
    char infra[255];

    unsigned int capa;
    unsigned int curr;
    
    int hp;
      
    } node;

node invalid_node()
    {
    node res;
    res.id = -1;
    strcpy(res.name, "INVALID");
    strcpy(res.desc, "INVALID");
    return res;
    }

typedef struct nl_base* nlist;

typedef struct nl_base
    {
    node top;
    nlist next;
    } nl_base;

void add_node(nlist* nl, node n)   
    {
    nlist res = malloc(sizeof(nl_base));
    res->top = n;
    res->next = *nl;
    *nl = res;
    }

int has_node(node* nl, unsigned int size, unsigned int id)
    {
    for (int i = 0; i < size; i++)
	if (nl[i].id == id) return 1;
    return 0;
    }

node get_node_info(node* nl, unsigned int size, unsigned int id)
    {
    for (int i = 0; i < size; i++)
	if (nl[i].id == id) return nl[i];
    fprintf(stderr, "WARNING: node of id %d was not initialized.", id);
    return invalid_node();
    }

void fill_ownership(nlist* nl)
    {
    nlist* head = nl;
    node hl;
    nlist hl_element = malloc(sizeof(nl_base));
    
    while (*nl != NULL)
	{
	if ((*nl)->top.owner == -1)
	    {
	    hl = (*nl)->top;
	    hl.owner = 0;
	    hl_element->top = hl;
	    hl_element->next = (*nl)->next;
	    (*nl) = hl_element;
	    fprintf(stderr, "NOTICE: switching ownership of node %d to no ownership.\n", hl_element->top.id);
	    }
	*nl = (*nl)->next;
	}
    nl = head;
    }


#endif
