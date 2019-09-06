#include <ncurses.h>
#include <stdlib.h>
#include "buffer.h"
#include <string.h>
#include "panic.h"
#include "selection.h"

marque* selection(pbuffer bp,int nl1,int ncl1, int nl2, int ncl2){
  marque *m = malloc(sizeof(marque));
  if (m==NULL) return NULL;
  int nbl= nl2-nl1+1;
  char** select = malloc(nbl *sizeof(char *));
  if (select ==NULL) return NULL;
  int i=nl1;
  int j=0;
  int tcol= pb->tailleLigne[nl1]-ncl1;
  while(i<nl2){
    select[j] =malloc(tcol *sizeof(char));
    memmove(select[j],pb->tBuffer[i],tcol*sizeof(char));
    j++;
    i++;
    tcol =pb->tailleLigne[i];
  }
  tcol =ncl2;
   select[j] =malloc(tcol *sizeof(char));
   memmove(select[j],pb->tBuffer[i],tcol*sizeof(char));
   m->selection = select;
   m->ligned=nl1;
   m->colonned = ncl1;
   m->lignef = nl2;
   m->colonnef = ncl2;
   return m;
}


void copie(pbuffer pb,marque *m){
  
  
}

void coupe(pbuffer pb, marque* m){
  int i= m->ligned;
  int j= m->colonned;
  
  while(i<m->lignef){
    for(;j<pb->tailleLigne[i];j++){
      pb->tBuffer[j] ='\0';
    }
    j=0;
    i++;
  }
  for(j=0;j<=m->colonnef;j++)
    pb->tBuffer[j] ='\0';
  
}

void coller(pbuffer pb,marque* m, int nl, int ncl){
  int ts0 = strlen(m->selection[0]);
  int tcol = ts0+pb->tailleLigne[nl] ;
  
  realloc(pb->tBuffer[nl],pb->tBuffer[nl],sizeof(char)*tcol);
  int tfin=pb->tailleLigne[nl]-ncl;
  memmove(&pb->tBuffer[nl][ts0],&pb->tBuffer[nl][ncl],tfin*sizeof(char));
  memmove(&pb->tBuffer[nl][ncl],m->selection[0],ts0*sizeof(char));
  pb->tailleLigne[nl]=tcol;
  int j=1;
  //coller les lignes entre ligned et lignef
  int i=m->ligned+1
  for(;i<m->lignef;i++){
    add_ligne(pb,m->selection[j],i);
    j++;
  }
  //coller la fin de ligne
  ts0= strlen(m->selection[j]);
  tcol = ts0+m->colonnef;
   realloc(pb->tBuffer[i],pb->tBuffer[i],sizeof(char)*tcol);
   tfin=

  memmove(&pb->tBuffer[nl][ts0],&pb->tBuffer[nl][ncl],tfin*sizeof(char));
  memmove(&pb->tBuffer[nl][ncl],m->selection[0],ts0*sizeof(char));
  
  }

/*efface une selection */
void effacer(pbuffer bp,marque m*){}
