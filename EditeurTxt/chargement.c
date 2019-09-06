#include <stdlib.h>
#include "chargement.h"
#include "buffer.h"
int nbligne(FILE *f){
 char *s;
  int size=1024;
 char buf[size];
 int nl =0;
   while((s=fgets(buf,size,f))!=NULL){
     nl ++;
  }
  rewind(f);
  return nl; 
}

pbuffer charger(char * filename){
   FILE *f;
   f=fopen(filename,"r");
   if(f==NULL){
    perror("fopen r");
    exit(1);
   }
   int taille = nbligne(f);
   pbuffer pb =create_buffer(taille);
    if(pb==NULL){
     perror("create_buffer");
     exit(1);
    }
   //int i =0;
    int size=1024;
    char *s;
    char buf[size];
     while((s=fgets(buf,size,f))!=NULL){
      ajouteFin(pb,buf);
      //i++; 
     }
    return pb;

}

