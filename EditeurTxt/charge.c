#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TAILLE_MAX 100000

charger(char* filename){
  FILE * fichier;
  fichier=fopen(filename,"r");
  int curseur=0;
  int i=0;
  char buffer[TAILLE_MAX]="";char buffer[TAILLE_MAX]="";
  if(fichier==NULL){
       exit(1);
  }else{
     char** curslu=malloc(TAILLE_MAX*sizeof(char*));
    while((curseur=fgets(buffer,TAILLE_MAX,fichier)!=Null)){
       ligne=strlen(curseur)+1;
       curslu[i]=malloc(ligne*sizeof(char**));
       i++;
      }
    }
}
