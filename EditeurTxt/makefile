cc = gcc
CFLAGS = -g  -Wall -std=c11 
LDFLAGS= -lncurses -lmenu
OBJET= buffer.o fenetre.o chargement.o sauvegarde.o

main: $(OBJET)
	$(cc) $(OBJET)  -o main $(LDFLAGS)


buffer.o: buffer.c buffer.h
chargement.o: chargement.c chargement.h buffer.h buffer.o
sauvegarde.o: sauvegarde.c sauvegarde.h buffer.h buffer.o
fenetre.o: fenetre.c buffer.o chargement.o sauvegarde.o



clean:
	rm -f $(OBJET) main
