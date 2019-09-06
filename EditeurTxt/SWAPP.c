#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#define TAILLE_MAX 100000

/*int Direction(char* a)
{
    if ((strchr(a,'.'))==NULL) // verifie si le caractère passé en second paramètre est présent dans la chaîne de caractère
        return 1;
    else
        return 0;
}
 
void lire(char* s)
{
    char *a = NULL;
    char t[TAILLE_MAX] = ""; // Chaîne de caractères temporaire contenant la saisie de l'utilisateur. 
    fgets(t,TAILLE_MAX,stdin);
    a=strchr(t, '\n'); // verifie si le caractère passé en second paramètre est présent dans le t
    if(Direction(t)==1)
        strcat(s,t); //On ajoute à la suite le nom du dossier.Concatener
    }
*/

int existanceFichier(char* nom_fichier)
{
  struct stat fstat;                 
  return lstat(nom_fichier, &fstat);
}

int main()
{    char nom[TAILLE_MAX];
     printf("\n -- Quel voulez-vous ouvrir -- ?\n");
          
     char* f= scanf("%s", nom);//le choix de l'ouverture
     if(existanceFIchier(f));
     	charger(f);
     printf("Le fichier a été ouvert avec succés");
    
  }
       
}


/*
void parcourirRepertoire(DIR* repertoire, char* chemin)
{
     lire(chemin); // Appel de la fonction lire pour li	re le nouveau chemin; 
     system(CLEAR); //On efface l'écran. 
     if (!Direction(chemin)) { // On verifie si c'est le chemin est ecrit correctement 
        printf("%s ce n'est pas un dossier", chemin);
        exit(-1);
     }
       repertoire= opendir(chemin);
     if (rep == NULL)
     {
        printf("Le dossier ne s'est pas ouvert", chemin);
        exit(-1);
      }
 
    lireRepertoire(chemin, rep); // Lecture... 
    closedir(repertoire); // Fermeture du répertoire. 
    parcourirRepertoire(repertoire, chemin); // récursivité 
}*/
 
