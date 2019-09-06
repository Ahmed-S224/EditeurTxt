#ifndef CHARGEMENT_H
#define CHARGEMENT_H
#include "buffer.h"

/*cette fonction permet de lire un fichier et charger son contenu dans le buffer*/

/*compte le nombre de ligne d'un fichier.
 *implementée dans buffer.c
 * utiliseé quand un fichier est charger pour connaitre le nombre d'éléments à ajouter dans le buffer .
 */
int nbligne(FILE *f);

pbuffer charger(char * filename);

#endif
