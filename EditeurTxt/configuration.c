#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configuration.h"
int SIZE =10;



struct tableconfig* dummyitem;


int hachCode(int indice){
  return indice % SIZE;
}

 tableconfig * cherche(hachtable hachtab,int indice, char* cl){
  // renvoi hachcode
  int hi = hachCode(indice);
  while(hachtab[hi] !=NULL){
    if(strcmp(hachtab[hi]->cle,cl)==0)
      return hachtab[hi];
    //aller au suivant
    ++hi;
    hi = hi%SIZE;
  }
  return NULL;
}
void insert(hachtable hachtab,int indice,char* c, char data){
   tableconfig * item = (tableconfig *)malloc(sizeof(tableconfig));
  
  if(item==NULL){
  
  perror("erreur malloc item");
  }
  else{
    item->cle = malloc(strlen(c)*sizeof(char));
    if(item->cle ==NULL) perror("erreur malloc itm->valeur");
    else{
      strcpy(item->cle, c);
      item->valeur = data;
      item->indice = indice;
      //recup hachcode
      int hi = hachCode(indice);
      while(hachtab[hi]!=NULL && strcmp(hachtab[hi]->cle,c) !=0){
	++hi;
	hi= hi%SIZE;
      }
      hachtab[hi] = item;
    }
    
  }
  
}

 tableconfig * supprime(hachtable  hachtab,  tableconfig *item){
  int indice = item->indice;
  char* c = item->cle;

  //hachcode
  int hi = hachCode(indice);
  //move in array until empty
  while(hachtab[hi] !=NULL){
    if(strcmp(hachtab[hi]->cle,c) ==0){
      struct tableconfig * temp = hachtab[hi];
      hachtab[hi] = dummyitem;
      return temp;
    } ++hi;
    hi = hi%SIZE;
    
  }
  return NULL;
}

void affiche(hachtable  hachtab){
  int i=0;
  for(i=0;i<SIZE;i++){
    if(hachtab[i] !=NULL)
      printf("%s->%c\n",hachtab[i]->cle, hachtab[i]->valeur);
      else printf("~~\n");
    
  }
}

void modifie(hachtable hachtab, char v,int indice,char * cle){
	int hi = hachCode(indice);
        while(hachtab[hi]!=NULL ){
		if(strcmp(hachtab[hi]->cle,cle)==0){
	 		hachtab[hi]->valeur = v;
			return ;

		}
		++hi;
		hi= hi%SIZE;

	}
}
  
hachtable  lireConfig(char * filename){
   FILE *f;
   f=fopen(filename,"r");
  hachtable hachtab = (hachtable)malloc(SIZE * sizeof(hachtable));

   if(f==NULL || hachtab ==NULL)
    perror("fopen r ou malloc");
 	else{
	 int i =0;
    int taille=1024;
    char *s;
    char buf[taille];
 
     while((s=fgets(buf,taille,f))!=NULL){
	char * token = strtok(buf,":");
     char *t1 =strtok(NULL,":");
	insert(hachtab,i,token,t1[0]);
	 i++;
	}
    }
 return hachtab;

}
 int sizeHachtab(hachtable hachtab){
  int i = hachCode(0);
  int s=0;
 
    while(hachtab[i]!=NULL){
   if(strcmp(hachtab[i]->cle ,"-1")!=0)
       s =s+1;
   i=i+1;
   i = hachCode(i);
}
 return s;
}
char * recupValeur(hachtable hachtab){
  int s = sizeHachtab(hachtab);
   char * res = malloc(s*sizeof(char));
 if(res==NULL) return NULL;
   int i =0;
 int hi = hachCode(0);
 
  while(hachtab[i]!=NULL){
   if(strcmp(hachtab[i]->cle ,"-1")!=0)
       res[i]=hachtab[i]->valeur;
   i=i+1;
  hi = hi+1;
   hi= hachCode(hi);
}
 return res;
}
void afficheValeur(char *valeur,int t){
   int i;
printf("[");
 for(i=0;i< t; i++)
  printf("%c,",valeur[i]);
printf("]\n");




}
void detruireHachtab(hachtable hachtab){
 int hi = hachCode(0);
 while(hachtab[hi]!=NULL){
 	free(hachtab[hi]->cle);
 	++hi;
 	hi = hachCode(hi);
 } free(hachtab);
}

hachtable remplace(hachtable hachtab){
  detruireHachtab(hachtab);
  return lireConfig(".config");
}
int main(void){
  /*struct tableconfig** hachtab = (struct tableconfig **)malloc(SIZE * sizeof(struct tableconfig**));
  dummyitem = (struct tableconfig *)malloc(sizeof(struct tableconfig));
  dummyitem->valeur ='-';
  dummyitem->cle="-1";
  insert(hachtab,1,"copier",'C');
  insert(hachtab,0,"coller",'V');
  insert(hachtab,2,"quiter",'Q');
  affiche(hachtab);
  struct tableconfig* item = cherche(hachtab,0,"coller");
  if(item!=NULL)printf("element ok %c\n", item->valeur);
  else printf("pas de valeur\n");
  item= supprime(hachtab,item);
    if(item!=NULL) printf("\nDelete\nelement ok %c\n", item->valeur);
  else printf("pas de valeur\n");
  
   affiche(hachtab);*/
 hachtable hachtab= lireConfig(".config");
  affiche(hachtab);
  
 char * valeur = recupValeur(hachtab);
 printf("%s\n", valeur);
  char controle[] ={'C','N','F','B','K','S','W','R'};
			printf("%c",controle[0]);
/*printf("pass1\n");
 char *s =malloc(10*sizeof(char));
 strcpy(s,"aertfd:eze");
 printf("pass %s\n",s);
  char *delim =":";
   char *token ;
  token=strtok(s,delim);
printf("%s %s\n",token ,strtok(NULL,delim));
 */
 
    return 0;
}
 
