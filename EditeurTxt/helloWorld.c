#include <ncurses.h>

int main(){
  initscr(); //initialise la structure de données window et autre parametre
  printw("Hello World"); //affiche le mgs à l'endroit ou ce trouve le curseur logique
  refresh();//rafraichi la fenetre courante afin de voir le msg apparitre
  getch();//on attend que l'utilisateur apuis sur une touche pour quiter
  endwin();//restaure les parametre par defaut du terminal
  return 0;
  
  
}
