#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"
#include "sauvegarde.h"

void ecrire(pbuffer pb,char* filename){

  FILE* f;
  f=fopen(filename, "w");
  if (f==NULL){
    perror("fopen w");
   exit(1);
}
  for(int i=0;i<=pb->last;i++){
      fwrite(pb->tBuffer[i],sizeof(char),pb->lastLigne[i],f);
  }
 fclose(f);
}
void sauvegarde(pbuffer pb, char*filename){
   if(fopen(filename,"r")!=NULL) {
   	printw("voulez vous ecrayer le fichier Y ou N");
   	char c = getch();
   	if(c=='Y' || c=='y'){
   		ecrire(pb,filename);
   	}
   }
   else {
   	ecrire(pb,filename);
   }
}

   

