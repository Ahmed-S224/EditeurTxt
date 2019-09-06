#ifndef BUFFER_H
#define BUFFER_H
#include <ncurses.h>
/* structutre de données utiliser dans tout le programme.
 *composée d'un tableau de chaines de caracteres et de la taille du tableau.
 */
struct buffer {
  char ** tBuffer;
  int last;//derniere position inserer dans le buffer
  int nbLigne;
 int* tailleLigne;
//la taille d'une ligne i dans la case lastLigne[i]
int *lastLigne;
};

/*pointeur vers la stucture son type de données */
typedef struct buffer *pbuffer;


void initialiseTL(int * tl, int taille, int po);

/*creer un buffer vide d'une taille*/
pbuffer create_buffer(int taille);
void reallouBuffer(pbuffer pb);
void ajouteFin(pbuffer pb, char *v);
/*ajoute une nouvelle ligne
*si i< last alors insertion
i>= last alors insertion
i>nlLigne alors on fais un realloc de deux * le nombre de ligne
  */
void add_ligne(pbuffer pb ,char* v, int i );
void ajoute_car(pbuffer pb, char c);

/*afficher le buffer */
void affiche(pbuffer pb,WINDOW*);
/*compte le nombre de ligne d'un fichier.
 *implementée dans buffer.c
 * utiliseé quand un fichier est charger pour connaitre le nombre d'éléments à ajouter dans le buffer .
 */
void affichescroll(pbuffer pb,WINDOW* wind,int ld, int lf);
 
/*detruire la structure de donnees utilisée pour le buffer*/
void detruire_buffer(pbuffer pb);

void add_caractere(pbuffer pb, int i, int j, char c);


#endif

