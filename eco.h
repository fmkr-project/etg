#ifndef ECO_H
#define ECO_H

void ecostatus(dataframe* df)
    {
    printf("%dp %df %dI\n",
	df->factlist[1].money,
	df->factlist[1].food,
	df->factlist[1].ctr);
    }









#endif
