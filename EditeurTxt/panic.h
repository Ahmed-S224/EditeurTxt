/***************** panic.h ********************/
#ifndef PANIC_H
#define PANIC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define PANIC( fin )  do{			\
   fprintf(stderr,\
     "\n error in file %s in line %d : %s\n",\
     __FILE__,__LINE__,strerror(errno)); \
   if (fin > 0) exit(fin);		 \
  } while(0)
#endif
