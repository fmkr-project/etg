#ifndef FRAME_H
#define FRAME_H

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "node.h"
#include "pers.h"
#include "route.h"
#include "fact.h"

#include "debug.h"

typedef struct dataframe
    {
    node nodelist[1000]; int nodes;
    //person perslist[1000]; int people;
    route routelist[1000]; int routes;
    fact factlist[255]; int factions;
    } dataframe;



void save_dataframe(char* path, dataframe* df)
    {
    char ans[255];
    char nodepath[255];
    int status;

    strcpy(nodepath, path);
    strcat(nodepath, "/node/"); // Format : ./saves/savename/node/

    // Check for save directory existence
    DIR* saves = opendir("./saves");
    if (saves == NULL)
	{
	printf("NOTICE: creating saves directory.\n");
	mkdir("./saves", 0755);
	}
    closedir(saves);

    // Check for save existence
    DIR* savedir = opendir(path);
    if (savedir == NULL)
	{
        printf("NOTICE: save with such name does not exist\n");
	printf("Do you want to create a new save? (y/n) ");
	fgets(ans, 255, stdin);
	ans[strcspn(ans, "\n")] = 0;

	if (!strcmp(ans, "y")) // Create new save directory
	    {
	    printf("NOTICE: creating new save...\n");
	    printf(path);
	    status = mkdir(path, S_IRWXU);
	    if (status != 0)
		{
		perror("mkdir");
		return;
		}
	    closedir(savedir);
	    savedir = opendir(path);
	    }

	}

    // Save node state
    // Open node directory
    DIR* snodes = opendir(nodepath);
    if (snodes == NULL)
	{
	fprintf(stderr, "NOTICE: creating node directory...\n");
	status = mkdir(nodepath, S_IRWXU);
	if (status != 0)
	    {
	    perror("mkdir");
	    return;
	    }
	closedir(snodes);
	snodes = opendir(nodepath);
	}
    
    // Save state for each node
    for (int i = 0; i < df->nodes; i++)
	{
	char cur_node_path[255];
	sprintf(cur_node_path, "%s%04d", nodepath, i);
	FILE* cur_node_file = fopen(cur_node_path, "w");
	fprintf(cur_node_file, "%s\n", df->nodelist[i].name);
	fprintf(cur_node_file, "%s\n", df->nodelist[i].desc);
	fprintf(cur_node_file, "%s\n", df->nodelist[i].type);
	fprintf(cur_node_file, "%03d\n", df->nodelist[i].owner);

	fclose(cur_node_file);
	}
    
    closedir(savedir);
    printf("Saved.\n");
    }



dataframe* load_dataframe(char* path, unsigned char existing_save)
    {
    FILE* node_dump = fopen("./node.dump", "w");
    fprintf(node_dump, "--- Node Information ---\n\n");

    
    DIR* resourcedir = opendir(path);
    if (resourcedir == NULL)
	{
	fprintf(stderr, "CRITICAL: failed to open res directory.\n");
	exit(401);
	}
    
    char nodepath[255];
    char routepath[255];
    char factpath[255];
    strcpy(nodepath, path);
    strcpy(routepath, path);
    strcpy(factpath, path);
    strcat(nodepath, "node/");
    strcat(routepath, "route/");
    strcat(factpath, "fact/");

    DIR* nodedir = opendir(nodepath);
    if (nodedir == NULL)
	{
	fprintf(stderr, "CRITICAL: failed to open res/node directory.\n");
	exit(401);
	}

    DIR* routedir = opendir(routepath);
    if (routedir == NULL)
	{
	fprintf(stderr, "CRITICAL: failed to open res/route directory.\n");
	exit(401);
	}
    
    DIR* factdir = opendir(factpath);
    if (factdir == NULL)
	{
	fprintf(stderr, "CRITICAL: failed to open res/fact directory.\n");
	exit(401);
	}
    
    struct dirent* resfile;
    FILE* entryfile;
    char buffer[1000];

    
    dataframe* res = malloc(sizeof(dataframe));

// Dataframe initialization
    for (int h = 0; h < 255; h++) res->factlist[h] = invalid_fact();
    for (int i = 0; i < 1000; i++)
	{
	res->nodelist[i] = invalid_node();
	res->routelist[i] = invalid_route();
	//res->personlist[i] = invalid_person();
	}


    
// Faction list processing    
    fact res_f;
    char fp[1000];
    char name[1000];

    res->factions = 0;
    
    while (resfile = readdir(factdir))
	{
	// support for . and ..
	if ((!strcmp(resfile->d_name, ".")) || (!strcmp(resfile->d_name, ".."))) continue;
	// duplicate guarding
	if (has_fact(res->factlist, res->factions, atoi(resfile->d_name)))
	    {
	    fprintf(stderr, "NOTICE: faction of id %s already initialized.\n", resfile->d_name);
	    continue;
	    }
 
	strcpy(fp, factpath);
	strcat(fp, resfile->d_name);
	entryfile = fopen(fp, "r");
	if (entryfile == NULL)
	    {
	    fprintf(stderr, "ERROR: on opening file for faction of id %s.\n", resfile->d_name);
	    continue;
	    }

	res_f.id = atoi(resfile->d_name);

	// Faction name
	fgets(name, 1001, entryfile);
	name[strcspn(name, "\n")] = 0;
	strcpy(res_f.name, name);
	// Faction desc
	fgets(buffer, 1001, entryfile);
	buffer[strcspn(buffer, "\n")] = 0;
	strcpy(res_f.desc, buffer);

	res_f.nforma = 0;

	res_f.money = 1000;
	res_f.food = 2500;
	res_f.ctr = 10;


	printf("Loading faction: %s, of id %d\n", res_f.name, res_f.id);
	res->factlist[res_f.id] = res_f;
	res->factions++;
	
	fclose(entryfile);
	}


    
// Node list processing
    node res_n;
    char np[1000];
    char namebuf[1000];
    char btbuf[100];
    char buf2[1000];

    res->nodes = 0;
    
    while (resfile = readdir(nodedir))
	{
	// support for . and ..
	if ((!strcmp(resfile->d_name, ".")) || (!strcmp(resfile->d_name, ".."))) continue;
	// duplicate guarding
	if (has_node(res->nodelist, res->nodes, atoi(resfile->d_name))) continue;

	strcpy(np, nodepath);
	strcat(np, resfile->d_name);
	entryfile = fopen(np, "r");
	if (entryfile == NULL)
	    {
	    fprintf(stderr, "ERROR: on opening file for node of id %s.\n", resfile->d_name);
	    continue;
	    }

	res_n.id = atoi(resfile->d_name);
	// Node name
	fgets(name, 1000, entryfile);
	name[strcspn(name, "\n")] = 0; // removing ending \n
	strcpy(res_n.name, name);
	// Node desc
	fgets(buffer, 1001, entryfile);
	buffer[strcspn(buffer, "\n")] = 0;
	strcpy(res_n.desc, buffer);
	// Building type
	fgets(btbuf, 1001, entryfile);
	btbuf[strcspn(btbuf, "\n")] = 0;
	if (!strcmp(btbuf, "None")) // no build
	    {
	    res_n.type = "None";
	    res_n.capa = NONE_CMAX;
	    res_n.curr = NONE_CMAX;
	    res_n.hp = NONE_HMAX;
	    res_n.fgrowth = NONE_FGROWTH;
	    }	
	if (!strcmp(btbuf, "v")) // type v
	    {
	    res_n.type = "v";
	    res_n.capa = V_CMAX;
	    res_n.curr = V_CMAX;
	    res_n.hp = V_HMAX;
	    res_n.fgrowth = V_FGROWTH;
	    }
	if (!strcmp(btbuf, "vf")) // type vf
	    {
	    res_n.type = "vf";
	    res_n.capa = VF_CMAX;
	    res_n.curr = VF_CMAX;
	    res_n.hp = VF_HMAX;
	    res_n.fgrowth = VF_FGROWTH;
	    }
	if (!strcmp(btbuf, "t")) // type t
	    {
	    res_n.type = "t";
	    res_n.capa = T_CMAX;
	    res_n.curr = T_CMAX;
	    res_n.hp = T_HMAX;
	    res_n.fgrowth = T_FGROWTH;
	    }
	if (!strcmp(btbuf, "r")) // type r
	    {
	    res_n.type = "r";
	    res_n.capa = R_CMAX;
	    res_n.curr = R_CMAX;
	    res_n.hp = R_HMAX;
	    res_n.fgrowth = R_FGROWTH;
	    }
	if (!strcmp(btbuf, "u")) // type u
	    {
	    res_n.type = "u";
	    res_n.capa = U_CMAX;
	    res_n.curr = U_CMAX;
	    res_n.hp = U_HMAX;
	    res_n.fgrowth = U_FGROWTH;
	    }
	if (!strcmp(btbuf, "f")) // type f
	    {
	    res_n.type = "f";
	    res_n.capa = F_CMAX;
	    res_n.curr = F_CMAX;
	    res_n.hp = F_HMAX;
	    res_n.fgrowth = F_FGROWTH;
	    }
	if (!strcmp(btbuf, "j")) // type j
	    {
	    res_n.type = "j";
	    res_n.capa = J_CMAX;
	    res_n.curr = J_CMAX;
	    res_n.hp = J_HMAX;
	    res_n.fgrowth = J_FGROWTH;
	    }
	if (!strcmp(btbuf, "m")) // type m
	    {
	    res_n.type = "m";
	    res_n.capa = M_CMAX;
	    res_n.curr = M_CMAX;
	    res_n.hp = M_HMAX;
	    res_n.fgrowth = M_FGROWTH;
	    }
	if (!strcmp(btbuf, "b")) // type b
	    {
	    res_n.type = "b";
	    res_n.capa = B_CMAX;
	    res_n.curr = B_CMAX;
	    res_n.hp = B_HMAX;
	    res_n.fgrowth = B_FGROWTH;
	    }	
	if (!strcmp(btbuf, "c")) // type c
	    {
	    res_n.type = "c";
	    res_n.capa = C_CMAX;
	    res_n.curr = C_CMAX;
	    res_n.hp = C_HMAX;
	    res_n.fgrowth = C_FGROWTH;
	    }

	// SGrowth rate
	if (res_n.type != "None") res_n.sgrowth = 0.003 * res_n.capa + 1;
	    else res_n.sgrowth = 0;
	
	// Node faction (if applicable)
	if (fgets(buf2, 1000, entryfile) != NULL)
	    {
	    buf2[strcspn(buf2, "\n")] = 0;
	    res_n.owner = atoi(buf2);
	    }
	else res_n.owner = 0;
      
	printf("Loading node: %s owned by %d, with %s, of id %d\n", res_n.name, res_n.owner, res_n.type, res_n.id);
	res->nodelist[res_n.id] = res_n;
	res->nodes++;
	fclose(entryfile);
	}

    res->nodes++;

    
// Route list processing
    route res_r;
    char rp[1000];

    res->routes = 0;
    
    while (resfile = readdir(routedir))
	{
	// support for . and ..
	if ((!strcmp(resfile->d_name, ".")) || (!strcmp(resfile->d_name, ".."))) continue;
	// duplicate guarding
	if (has_route(res->routelist, res->routes, atoi(resfile->d_name)))
	    {
	    fprintf(stderr, "NOTICE: route of id %s already initialized.\n", resfile->d_name);
	    continue;
	    }
 
	strcpy(rp, routepath);
	strcat(rp, resfile->d_name);
	entryfile = fopen(rp, "r");
	if (entryfile == NULL)
	    {
	    fprintf(stderr, "ERROR: on opening file for route of id %s.\n", resfile->d_name);
	    continue;
	    }

	res_r.id = atoi(resfile->d_name);
	// Route endpoint A
	fgets(buffer, 1001, entryfile);
	buffer[strcspn(buffer, "\n")] = 0;
	res_r.from = atoi(buffer);
	// Route endpoint B
	fgets(buffer, 1001, entryfile);
	buffer[strcspn(buffer, "\n")] = 0;
	res_r.to = atoi(buffer);
	// Route name
	fgets(name, 1001, entryfile);
	name[strcspn(name, "\n")] = 0;
	strcpy(res_r.name, name);

	// Test if endpoints are existing nodes
	if (!has_node(res->nodelist, res->nodes, res_r.from) || !has_node(res->nodelist, res->nodes, res_r.to))
	    {
	    fprintf(stderr, "ERROR: route of id %d has invalid endpoint(s)\n", res_r.id);
	    continue;
	    }
	
	// Route length
	fgets(buffer, 1001, entryfile);
	buffer[strcspn(buffer, "\n")] = 0;
	res_r.len = atoi(buffer);
	if (res_r.len == 0) fprintf(stderr, "WARNING: route of id %d has length 0, consider adding line with route length\n", res_r.id);
	
	printf("Loading route: %s, from %d to %d (dist %d), of id %d\n", res_r.name, res_r.from, res_r.to, res_r.len, res_r.id);
	res->routelist[res_r.id] = res_r;
	res->routes++;
	
	fclose(entryfile);
	}



/*----------DUMPS----------*/
    
// Node information dump
    for (int i = 0; i < res->nodes; i++)
	{
	fprintf(node_dump, "Node %d: %s, with neighbors:\n", i, res->nodelist[i].name);
	for (int r = 0; r < res->routes; r++)
	    {
	    if (res->routelist[r].from == i)
		{
		int dest = res->routelist[r].to;
		fprintf(node_dump, "    %d: %s, at %d\n",
		    dest,
		    res->nodelist[dest].name,
		    res->routelist[r].len);
		}
	    if (res->routelist[r].to == i)
		{
		int dest = res->routelist[r].from;
		fprintf(node_dump, "    %d: %s, at %d\n",
		    dest,
		    res->nodelist[dest].name,
		    res->routelist[r].len);
		}
	    }
	fprintf(node_dump, "\n");
	}
    
    closedir(nodedir);
    closedir(factdir);
    closedir(routedir);

    fclose(node_dump);
    
    return res;
    }




void load_savedata(char* path, dataframe* df)
    {
    DIR* save_dir = opendir(path);
    if (save_dir == NULL)
	{
	fprintf(stderr, "ERROR: failed to open save directory.\n");
	return;
	}
    printf("Loading save...\n");
    
    df = load_dataframe(path, 1);
    }



#endif
