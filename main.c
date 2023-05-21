#define PLAYERFAC 1

#include <stdio.h>

#include "frame.h"
#include "status.h"
#include "eco.h"
#include "training.h"
#include "milit.h"
#include "turn.h"

int main(int argc, char** argv)
    {
    char res_location[50]; strcpy(res_location, "./res/");
    char save_location[50]; strcpy(save_location, "./saves/");
    int running = 1;
    
    dataframe* data = load_dataframe(res_location, 0);

    unsigned int turns = 0;
    char action[100];

    while (running)
	{
	turns++;
	printf("\n----- TOUR %d -----\n", turns);

	while (strcmp(action, "0"))
	    {	
	    printf("action ? (h: help) ");
	    fgets(action, 100, stdin);
	    action[strcspn(action, "\n")] = 0;

	    if (!strcmp(action, "h")) aled();
	    
	    if (!strcmp(action, "n")) unroll_nodes(data);
	    if (!strcmp(action, "f")) unroll_fact(data);
	    if (!strcmp(action, "r")) unroll_route(data);

	    if (!strcmp(action, "e")) ecostatus(data);
	    if (!strcmp(action, "t")) training_menu(data);
	    if (!strcmp(action, "c")) corps_menu(data);
	    if (!strcmp(action, "m")) milit_menu(data);
	    
	    if (!strcmp(action, "l")) load_data(save_location, data);
	    if (!strcmp(action, "s")) save_data(save_location, data);
	
	    }
	
	strcpy(action, "");
	end_turn(data);
	}
    free(data);
    return 1;
    }
