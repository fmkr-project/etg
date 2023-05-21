#ifndef BASES_H
#define BASES_H

// Int lists
typedef struct ilbase* intlist;

typedef struct ilbase
    {
    int top;
    intlist next;
    } ilbase;

void print_intlist(intlist il)
    {
    while (il != NULL)
	{
	printf("[%d] -> ", il->top);
	il = il->next;
	}
    printf("\n");
    }

void chain(intlist* il, int el)
    {
    intlist res = malloc(sizeof(ilbase));
    res->top = el;
    res->next = *il;
    *il = res;
    }

int is_in(intlist il, int el)
    {
    while (il != NULL)
	{
	if (il->top == el) return 1;
	il = il->next;
	}
    return 0;
    }




#endif
