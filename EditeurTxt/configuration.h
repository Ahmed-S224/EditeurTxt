/*regle de configuration: 
* cle:valeur ou cle est une chaine et valeur un caractere
*cut:char ->pour couper 
* paste:char ->coller
* copy:char -> copier
* save:char-> sauvegarde de fichier 
* open:char ->ouvrir un fichier
* new:char ->creer un nouveau fihier
* exit:char -> quitter
*/
#ifndef CONFIGURATION_H
#define CONFIGURATION_H


struct tableconfig {
  char valeur;
  char *cle;
  int indice;
};
typedef struct tableconfig tableconfig;
typedef struct tableconfig **hachtable;
//fonction de hachage qui renvoi indice%SIZE du tableau de hachage
int hachCode(int indice);
//recherche element dans le tableau de hachage
tableconfig * cherche(hachtable hachtab,int indice, char* cl);

//insert un element dan le tableau 
void insert(hachtable hachtab,int indice,char* c, char data);
//supprime un element
tableconfig * supprime(hachtable hachtab, tableconfig *item);

//modifie une valeur d'une cle;
void modifie(hachtable hachtab, char v,int indice,char * cle);
char * recupValeur(hachtable hachtab);
void afficheValeur(char *valeur,int t);
int sizeHachtab(hachtable hachtab);
//li le fichier de conficuration et cree le hachtab
hachtable lireConfig(char * filename);
//affiche la table de hachage
void affiche(hachtable hachtab);
/*creer une nouvelle configuration
void remplace(char*);
*/

#endif
