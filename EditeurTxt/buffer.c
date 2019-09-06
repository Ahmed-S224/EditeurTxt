#include <ncurses.h>
#include <stdlib.h>
#include "buffer.h"
#include <string.h>
#include "panic.h"

 int size = 1024;
//fonction bas niveau ititialise tailleligne
void initialiseTL(int * tl, int taille, int po){
	int i;
	for(i=po;i<taille;i++){
		tl[i] = 1024;
	}
}
void initislastL(int * tl, int taille, int po, int v){
	int i;
	for(i=po;i<taille;i++){
		tl[i] = v;
	}
}
/*creer un buffer vide d'une taille*/
pbuffer create_buffer(int taille){
  pbuffer b= malloc(sizeof(pbuffer));
  char **tab = (char **)malloc(taille*sizeof(char *));
  int * tl = (int *)malloc(taille*sizeof(int));
   int * ll = (int *)malloc(taille*sizeof(int));
   if (b==NULL ||tab ==NULL ||tl ==NULL || ll==NULL) return NULL;
   b->tailleLigne = tl;
   b->tBuffer = tab;
   b->last =0;
   b->nbLigne = taille;
   b->lastLigne = ll;
  initialiseTL(b->tailleLigne, taille,0);
  initislastL(b->lastLigne,taille, 0,0);

   return b;
}

//fonction bas niveau test si last est en fin de buffer
void reallouBuffer(pbuffer pb){
    if(pb->last >=pb->nbLigne){
       int nbm = pb->nbLigne *2;
      char** tb2 = realloc(pb->tBuffer,nbm *sizeof(char*));
      int * ttl = realloc(pb->tailleLigne,nbm *sizeof(int));
     int * ltl = realloc(pb->lastLigne,nbm *sizeof(int));
      if (tb2==NULL ||ttl ==NULL || ltl==NULL){
	    printw("erreur realloc \n");
	}

	initialiseTL(ttl,nbm,pb->nbLigne);
	pb->tBuffer = tb2;
	pb->tailleLigne = ttl;
	pb->lastLigne = ltl;
	pb->nbLigne = nbm;
     }  
    
}

void ajouteFin(pbuffer pb, char *v){
    //on a atteint la fin du tbuffer
    reallouBuffer(pb);
    int tv = strlen(v);
    int dn = pb->last;
    if(tv>= pb->tailleLigne[dn]){
     pb->tailleLigne[dn] = pb->tailleLigne[dn]*2;
    }
    pb->tBuffer[dn] = malloc(pb->tailleLigne[dn] *sizeof(char));
    if(pb->tBuffer[dn] ==NULL){
     printw("error malloc");
      return ;
      }
     memmove(pb->tBuffer[dn],v,tv*sizeof(char));
     pb->lastLigne[dn] = tv;
     pb->last = pb->last+1;
     
  }

void add_caractere(pbuffer pb, int i, int j, char c){
	pb->tBuffer[i][j] = c;
	
}
void ajoute_car(pbuffer pb, char c){

  if(pb->last ==0 ){
    if(pb->tBuffer[pb->last] == NULL){
      pb->tBuffer[pb->last] = malloc(pb->tailleLigne[pb->last]*sizeof(char));
    }
      int dn = pb->last;
    	int dl = pb->lastLigne[dn];
    	pb->tBuffer[dn][dl] = c;
    	pb->lastLigne[dn] = dl +1;
	  
    
  }else {
    int dn = pb->last=pb->last-1;//car ajout sur la ligne courante 
    
    int dl = pb->lastLigne[dn];
    pb->tBuffer[dn][dl] = c;
    pb->lastLigne[dn] = dl +1;
	    
    
}
}
//fonction bas niveau
void test_position_nbLigne(pbuffer pb, int position){
  if(position>=pb->nbLigne|| pb->last >= pb->nbLigne){
      int nbm= pb->nbLigne *2;
      char** tb2 = realloc(pb->tBuffer,nbm *sizeof(char*));
      int * ttl = realloc(pb->tailleLigne,nbm *sizeof(int));
     int * ltl = realloc(pb->lastLigne,nbm *sizeof(int));
      if (tb2==NULL ||ttl ==NULL || ltl==NULL){
	printw("erreur realloc \n");
	return;
      }
      pb->tBuffer = tb2;
      pb->nbLigne = nbm;
      pb->tailleLigne= ttl;
      pb->lastLigne = ltl;
   }
}

/*insert une nouvelle ligne à une position i dans le buffer*/
void add_ligne(pbuffer pb, char* v,int position){
  int tv= strlen(v) +1;
  //test si position à inserer ou last est superieur à nbLigne
  test_position_nbLigne(pb, position);
      
  if (position<=pb->last){
    //la position à inserer est inferieur à la position courante du buffer
   
    //calcul de la taille entre position et last
    int j=pb->last +1;
    for(;j>position ;j--){
      pb->tBuffer[j] = malloc(pb->tailleLigne[j-1]*sizeof(char));
      pb->tailleLigne[j] = pb->tailleLigne[j-1];
      memmove(pb->tBuffer[j],pb->tBuffer[j-1],pb->tailleLigne[j]*sizeof(char));
      free(pb->tBuffer[j-1]);
    }
    pb->tBuffer[position] = malloc(tv*sizeof(char));
    strcpy(pb->tBuffer[position],v);
    pb->tailleLigne[position]= tv;
    pb->last +=1;
  
  }else{
    for(int i=pb->last+1;i<position;i++){
      pb->tBuffer[i] = malloc(2*sizeof(char));
      pb->tBuffer[i][0] ='\n';
      pb->tailleLigne[i]=2;
    }
     pb->tBuffer[position] = malloc(tv*sizeof(char));
     strcpy(pb->tBuffer[position],v);
    pb->tailleLigne[position]= tv;
    pb->last +=1;
 
  } 
  
}

/*afficher le buffer */
void affiche(pbuffer pb,WINDOW* wind){
  printw("Le contenu du buffer\n");
  printw("last =%d\n",pb->last);
  printw("nbligne =%d\n",pb->nbLigne);
  printw("le contenu du buffer:\n");
  for(int i=0;i<pb->last;i++){
    mvwprintw(wind,i,2,"%s\n",pb->tBuffer[i]);
    wrefresh(wind); 
  }
}
/*afficher le buffer */
void affichescroll(pbuffer pb,WINDOW* wind,int ld, int lf){
  int j=0;
  if (lf < pb ->last )
    {
    for(int i=ld;i<=lf;i++){

      mvwprintw(wind,j,2,"%s\n",pb->tBuffer[i]);
      j++;
      wrefresh(wind);
    }
     
  }
  delwin(wind);
}




 
/*detruire la structure de donnees utilisée pour le buffer*/
void detruire_buffer(pbuffer pb){
  for (int i=0;i<pb->last;i++)
    free(pb->tBuffer[i]);
  free(pb->tBuffer);
 free(pb->tailleLigne);
free(pb->lastLigne);
  free(pb);
       
}
