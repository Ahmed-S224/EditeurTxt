#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H
#include "buffer.h"
void ecrire(pbuffer pb,char* filename);
 /*ecris le contenu du buffer dans un fichier dont le nom sera demander par l'utilisateur
tester si le nom du fichier existe alors demander comfirmation avant sauvegarde*/ 
 void sauvegarde(pbuffer pb, char* filename);
#endif

