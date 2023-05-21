#ifndef BUILD_H
#define BUILD_H



typedef struct build
    {
    char name[255];
    char desc[1000];

    int node_id;
    char* type;

    unsigned int capa;
    unsigned int curr;
    
    int hp;


    } build;

build invalid_build()
    {
    build res;
    res.node_id = -1;
    strcpy(res.name, "INVALID");
    strcpy(res.desc, "INVALID");
    return res;
    }

typedef struct bl_base* blist;

typedef struct bl_base
    {
    build top;
    blist next;
    } bl_base;

void add_build(blist* bl, build n)   
    {
    blist res = malloc(sizeof(bl_base));
    res->top = n;
    res->next = *bl;
    *bl = res;
    }

int has_build(build* bl, unsigned int size, unsigned int id)
    {
    for (int i = 0; i < size; i++)
	if (bl[i].node_id == id) return 1;
    return 0;
    }

build get_build_info(build* bl, unsigned int size, unsigned int id)
    {
    for (int i = 0; i < size; i++)
	if (bl[i].node_id == id) return bl[i];
    fprintf(stderr, "WARNING: building of id %d was not initialized.", id);
    //return invalid_fact();
    }



#endif
