#ifndef SELECTION_H
#define SELECTION_H
#include "buffer.h"
/* cette interface implemente une section dans le pbuffer

/*structure d'une marque de selection */
typedef struct marque{
	char **selection;
	int ligned;
	int colonned;
	int lignef;
	int colonnef;
}marque;


/*selection une region du buffer */
marque* selection(pbuffer bp,int numligne1,int numcol1, int numl2, int numcol2);

void copie(pbuffer,marque m);

void coupe(pbuffer, marque);

void coller(pbuffer,marque,int ,int);

/*efface une selection */
void effacer(pbuffer bp,marque m);
#endif
