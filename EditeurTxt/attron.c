#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
  int ch;
  initscr();
  raw();
  keypad(stdscr,TRUE);
  noecho();

  printw("Type any character to see it in bold\n");
  ch = getch();

  if(ch==KEY_F(3))
    printw("F3 Key pressed");

  else{
    printw("The pressed key is ");
    attron(A_BOLD|A_UNDERLINE);
    printw("%c",ch);
    attroff(A_NORMAL);
  }
  refresh();
  getch();
  endwin();

  return 0;
}
