#include <ncurses.h>
#include <stdlib.h>
#include "buffer.h"
#include "panic.h"
#include "chargement.h"
#include <menu.h>
#include <string.h>
#include <stdio.h>
#include "sauvegarde.h"

#define WIDTH 30
#define HEIGHT 10

#define HAVE_GETWIN 1
#define HAVE_PUTWIN 1

#ifndef HAVE_PUTWIN
#define HAVE_PUTWIN 0
#endif

#ifndef HAVE_GETWIN
#define HAVE_GETWIN 0
#endif


#define _nc_SPRINTF             NCURSES_VOID sprintf
#define _nc_SLIMIT(n)       /* nothing */


#define _nc_STRCPY(d,s,n)   NCURSES_VOID strcpy((d),(s))
#define _nc_STRNCPY(d,s,n)  NCURSES_VOID strncpy((d),(s),(n))

#define NCURSES_VOID (void)

#define SIZEOF(table)   (sizeof(table)/sizeof(table[0]))


#define AddCh(c)        (void) addch((chtype)(c))
#define WAddCh(w,c)     (void) waddch((w),(chtype)(c))
#define MvAddCh(y,x,c)      (void) mvaddch((y),(x),(chtype)(c))
#define MvWAddCh(w,y,x,c)   (void) mvwaddch((w),(y),(x),(chtype)(c))
#define MvAddStr(y,x,s)     (void) mvaddstr((y),(x),(s))
#define MvWAddStr(w,y,x,s)  (void) mvwaddstr((w),(y),(x),(s))
#define MvWAddChStr(w,y,x,s)    (void) mvwaddchstr((w),(y),(x),(s))
#define MvPrintw        (void) mvprintw
#define MvWPrintw       (void) mvwprintw
#define MvHLine         (void) mvhline
#define MvWHLine        (void) mvwhline
#define MvVLine         (void) mvvline
#define MvWVLine        (void) mvwvline

int startx = 0;
int starty = 0;

static const char *const choices[] = {
            "Chargement de fichier",
            "Editer un nouveau fichier",
            "Exit",
          };
int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);
void print_menu2(WINDOW *menu_win, int highlight);
int debut_menu(int c, WINDOW * win, int highlight, int choice, pbuffer buffer);
static void set_terminal_modes(void);
static int scroller(WINDOW * win, pbuffer buffer, int c, bool b);


#ifndef ExitProgram
#define ExitProgram(code) exit(code)
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef OK
#define OK (0)
#endif

#ifndef ERR
#define ERR (-1)
#endif

#undef CTRL
#define CTRL(x) ((x) & 0x1f)

#define QUIT        CTRL('Q')
#define ESCAPE      CTRL('[')


#define typeCalloc(type,elts) (type *) calloc((size_t)(elts), sizeof(type))

/* Common function to allow ^T to toggle trace-mode in the middle of a test
 * so that trace-files can be made smaller.
 */
static int wGetchar(WINDOW *win)
{
    int c;
#ifdef TRACE
    while ((c = wgetch(win)) == CTRL('T')) {
    if (_nc_tracing) {
        save_trace = _nc_tracing;
        Trace(("TOGGLE-TRACING OFF"));
        _nc_tracing = 0;
    } else {
        _nc_tracing = save_trace;
    }
    trace(_nc_tracing);
    if (_nc_tracing)
        Trace(("TOGGLE-TRACING ON"));
    }
#else
    c = wgetch(win);
#endif
    return c;
}
#define Getchar() wGetchar(stdscr)

static void failed(const char *s) GCC_NORETURN;

/* fermer la fenetre et quitter le programme */
static void
failed(const char *s)
{
    perror(s);
    endwin();
    ExitProgram(EXIT_FAILURE);
}
/* return a boolean  */
static bool
isQuit(int c, bool escape)
{
    return ((c) == QUIT || (escape && ((c) == ESCAPE)));
}
#define case_QUIT   QUIT: case ESCAPE

#define BOTLINES    4   /* number of line stolen from screen bottom */

typedef struct {
    int y, x;
} pair;
/* structure d'un frame  */
#define FRAME struct frame
FRAME
{
    FRAME *next, *last;
    bool do_scroll;
    bool do_keypad;
    WINDOW *wind;
};
/*fenetre en mode ecriture*/
static WINDOW * frame_win(FRAME * curp){
    return (curp != 0) ? curp->wind : stdscr;
}

/* clavier accessible */
static bool HaveKeypad(FRAME * curp){
    WINDOW *win = frame_win(curp);
    (void) win;
    return is_keypad(win);
}
/* va permettre de scroller dans notre fenetre*/
static bool HaveScroll(FRAME * curp){
    WINDOW *win = frame_win(curp);
    (void) win;
    return is_scrollok(win);
}
/* raccourcis clavier et scroll de la fenetre  */
static void newwin_legend(FRAME * curp){
#define DATA(num, name) { name, num }
    static const struct {
    const char *msg;
    int code;
    }
    legend[] = {
        DATA(0, "^C = cree fenetre"),
        DATA(0, "^N = fenetre suivante"),
        DATA(0, "^P = fenetre precedente"),
        DATA(0, "^F = scroll Haut"),
        DATA(0, "^B = scroll Bas"),
        DATA(1, "^K = keypad(%s)"),
        DATA(2, "^S = scrollok(%s)"),
        DATA(0, "^W = save fenetre"),
        DATA(0, "^R = restaurer fenetre"),
#if HAVE_WRESIZE
        DATA(0, "^X = redimension"),
#endif
        DATA(3, "^Q%s = exit")
    };
#undef DATA
    size_t n;
    int x;
    bool do_keypad = HaveKeypad(curp);
    bool do_scroll = HaveScroll(curp);
    char buf[BUFSIZ];

    move(LINES - 4, 0);
    for (n = 0; n < SIZEOF(legend); n++) {
    switch (legend[n].code) {
    default:
        _nc_STRCPY(buf, legend[n].msg, sizeof(buf));
        break;
    case 1:
        _nc_SPRINTF(buf, _nc_SLIMIT(sizeof(buf))legend[n].msg, do_keypad ? "yes" : "no");
        break;
    case 2:
        _nc_SPRINTF(buf, _nc_SLIMIT(sizeof(buf))legend[n].msg, do_scroll ? "yes" : "no");
        break;
    case 3:
        _nc_SPRINTF(buf, _nc_SLIMIT(sizeof(buf))legend[n].msg, do_keypad ? "/ESC" : "");
        break;
    }
    x = getcurx(stdscr);
    addstr((COLS < (x + 3 + (int) strlen(buf))) ? "\n" : (n ? ", " : ""));
    addstr(buf);
    }
    clrtoeol();
}

/* position et ajout du message */
static void
transient(FRAME * curp, NCURSES_CONST char *msg)
{
    newwin_legend(curp);
    if (msg) {
    MvAddStr(LINES - 1, 0, msg);
    refresh();
    napms(1000);
    }

    move(LINES - 1, 0);
    printw("%s characters are echoed, window should %sscroll.",
       HaveKeypad(curp) ? "Non-arrow" : "All other",
       HaveScroll(curp) ? "" : "not ");
    clrtoeol();
}

static void newwin_report(FRAME * curp)
/* report on the cursor's current position, then restore it */
{
    WINDOW *win = frame_win(curp);
    int y, x;

    if (win != stdscr)
    transient(curp, (char *) 0);
    getyx(win, y, x);
    move(LINES - 1, COLS - 17);
    printw("Y = %2d X = %2d", y, x);
    if (win != stdscr)
    refresh();
    else
    wmove(win, y, x);
}

static pair * selectcell(int uli, int ulj, int lri, int lrj)
/* arrows keys move cursor, return location at current on non-arrow key */
{
    static pair res;        /* result cell */
    int si = lri - uli + 1; /* depth of the select area */
    int sj = lrj - ulj + 1; /* width of the select area */
    int i = 0, j = 0;       /* offsets into the select area */

    res.y = uli;
    res.x = ulj;
    for (;;) {
    move(uli + i, ulj + j);
    newwin_report((FRAME *) 0);

    switch (Getchar()) {
    case KEY_UP:
        i += si - 1;
        break;
    case KEY_DOWN:
        i++;
        break;
    case KEY_LEFT:
        j += sj - 1;
        break;
    case KEY_RIGHT:
        j++;
        break;
    case case_QUIT:
        return ((pair *) 0);
#ifdef NCURSES_MOUSE_VERSION
    case KEY_MOUSE:
        {
        MEVENT event;

        getmouse(&event);
        if (event.y > uli && event.x > ulj) {
            i = event.y - uli;
            j = event.x - ulj;
        } else {
            beep();
            break;
        }
        }
#endif
        /* FALLTHRU */
    default:
        res.y = uli + i;
        res.x = ulj + j;
        return (&res);
    }
    i %= si;
    j %= sj;
    }
}


static void outerbox(pair ul, pair lr, bool onoff)
/* draw or erase a box *outside* the given pair of corners */
{
    MvAddCh(ul.y - 1, lr.x - 1, onoff ? ACS_ULCORNER : ' ');
    MvAddCh(ul.y - 1, lr.x + 1, onoff ? ACS_URCORNER : ' ');
    MvAddCh(lr.y + 1, lr.x + 1, onoff ? ACS_LRCORNER : ' ');
    MvAddCh(lr.y + 1, ul.x - 1, onoff ? ACS_LLCORNER : ' ');
    move(ul.y - 1, ul.x);
    hline(onoff ? ACS_HLINE : ' ', lr.x - ul.x + 1);
    move(ul.y, ul.x - 1);
    vline(onoff ? ACS_VLINE : ' ', lr.y - ul.y + 1);
    move(lr.y + 1, ul.x);
    hline(onoff ? ACS_HLINE : ' ', lr.x - ul.x + 1);
    move(ul.y, lr.x + 1);
    vline(onoff ? ACS_VLINE : ' ', lr.y - ul.y + 1);
}
// creation de window
static WINDOW * getwindow(pbuffer buffer)
/* Ask user for a window definition */
{
    WINDOW *rwindow;
    pair ul, lr, *tmp;
    int j = 0;



    move(0, 0);
    clrtoeol();
    addstr("Utilisez les fleches pour bouger votre curseur et une lettre pour marquer l'angle 1");
    refresh();
    if ((tmp = selectcell(2, 1, LINES - BOTLINES - 2, COLS - 2)) == (pair *) 0)
    return ((WINDOW *) 0);
    memcpy(&ul, tmp, sizeof(pair));
    MvAddCh(ul.y - 1, ul.x - 1, ACS_ULCORNER);
    move(0, 0);
    clrtoeol();
    addstr("Utilisez les fleches pour bouger votre curseur et une lettre pour marquer l'angle 2");
    refresh();
    if ((tmp = selectcell(ul.y, ul.x, LINES - BOTLINES - 2, COLS - 2)) ==
    (pair *) 0)
    return ((WINDOW *) 0);
    memcpy(&lr, tmp, sizeof(pair));


    rwindow = subwin(stdscr, lr.y - ul.y + 1, lr.x - ul.x + 1, ul.y, ul.x);
   
    outerbox(ul, lr, TRUE);

   j++;
    refresh();

    if (rwindow != 0)
    wrefresh(rwindow);

    move(0, 0);
    clrtoeol();
    return (rwindow);
}

static void newwin_move(WINDOW *win, int dy, int dx)
{
    //WINDOW *win = frame_win(curp);
    int cur_y, cur_x;
    int max_y, max_x;

    getyx(win, cur_y, cur_x);
    getmaxyx(win, max_y, max_x);
    if ((cur_x += dx) < 0)
    cur_x = 0;
    else if (cur_x >= max_x)
    cur_x = max_x - 1;
    if ((cur_y += dy) < 0)
    cur_y = 0;
    else if (cur_y >= max_y)
    cur_y = max_y - 1;
    wmove(win, cur_y, cur_x);
}


static int scroller(WINDOW * win, pbuffer buffer, int c, bool b){
    //keypad(stdscr,TRUE);
/* Initialize curses */


    int x = 36;
    int y =0;

    win = subwin(stdscr,LINES -3, COLS -2,1,0);
    box(win,0,0);
    clear();
    //newwin_legend(win);
    refresh();

    FRAME *current = (FRAME *) 0, *neww;
    WINDOW *usescr = win;
#if HAVE_PUTWIN && HAVE_GETWIN
    FILE *fp;
#endif

#define FICERR    "fichier_err"

#ifdef NCURSES_MOUSE_VERSION
    mousemask(BUTTON1_CLICKED, (mmask_t *) 0);
#endif
    c = CTRL('C');
    raw();
    do {
    transient((FRAME *) 0, (char *) 0);
    switch (c) {
    case CTRL('C'):
        if (!b)
           {
            if ((neww = typeCalloc(FRAME, (size_t) 1)) == 0) {
            failed("scroll_test");
           // goto breakout;
            }
            buffer = create_buffer(LINES);
            neww->wind = getwindow(buffer);
            
           }else if (b){
                affichescroll(buffer,usescr,y, x);
                neww -> wind = usescr;

           }


        if (current == 0) { /* First element,  */
        neww->next = neww;  /*   so point it at itself */
        neww->last = neww;
        } else {
        neww->next = current->next;
        neww->last = current;
        neww->last->next = neww;
        neww->next->last = neww;
        }
        current = neww;
        keypad(current->wind, TRUE);
        current->do_keypad = HaveKeypad(current);
        current->do_scroll = HaveScroll(current);
        break;

    case CTRL('N'): /* go to next window */
        if (current)
        current = current->next;
        break;

    case CTRL('P'): /* go to previous window */
        if (current)
        current = current->last;
        break;

    case CTRL('F'): /* scroll current window forward */
        if (current)
        wscrl(frame_win(current), 1);
        break;

    case CTRL('B'): /* scroll current window backwards */
        if (current)
        wscrl(frame_win(current), -1);
        break;

    case CTRL('K'): /* toggle keypad mode for current */
        if (current) {
        current->do_keypad = !current->do_keypad;
        keypad(current->wind, current->do_keypad);
        }
        break;

    case CTRL('S'):
        if (current) {
        current->do_scroll = !current->do_scroll;
        scrollok(current->wind, current->do_scroll);
        }
        break;

#if HAVE_PUTWIN && HAVE_GETWIN
    case CTRL('W'): /* save and delete window */
        initscr();
        
        clear();
        echo();
        printw("donnez le nom de votre fichier .....");

        char file_name[80];
        getstr(file_name);
        sauvegarde(buffer, file_name);

        break;

    case CTRL('R'): /* restore window */
        if ((fp = fopen(FICERR, "r")) == (FILE *) 0) {
        transient(current, "Erreur on ne peut ouvrir le fichier ....");
        } else {
        if ((neww = typeCalloc(FRAME, (size_t) 1)) != 0) {

            neww->next = current ? current->next : 0;
            neww->last = current;
            if (neww->last != 0)
            neww->last->next = neww;
            if (neww->next != 0)
            neww->next->last = neww;

            neww->wind = getwin(fp);

            wrefresh(neww->wind);
        } else {
            failed("scroll_test");
        }
        (void) fclose(fp);
        }
        break;
#endif


    case KEY_UP:
        newwin_move(usescr, -1, 0);
        if (y >0)
        {
          move(1,0);
          affichescroll(buffer,win,y, x);
          y--;
          x--;
        }
        break;
    case KEY_DOWN:
        newwin_move(usescr, 1, 0);
        if (x<buffer->last)
        {
          move(1,0);
          affichescroll(buffer,win,y, x);
          y++;
          x++;
        }
        break;
    case KEY_LEFT:
        newwin_move(usescr, 0, -1);
        break;
    case KEY_RIGHT:
        newwin_move(usescr, 0, 1);
        break;

    case KEY_BACKSPACE:
        /* FALLTHROUGH */
    case KEY_DC:
        {
        int y, x;
        getyx(frame_win(current), y, x);
        if (--x < 0) {
            if (--y < 0)
            break;
            x = getmaxx(frame_win(current)) - 1;
        }
        (void) mvwdelch(frame_win(current), y, x);
        }
        break;

    case '\r':
        c = '\n';
        /* FALLTHROUGH */

    default:
        if (current){
         waddch(current->wind, (chtype) c);
         ajoute_car(buffer,(char)c);
        }else
        beep();
        break;
    }
    newwin_report(current);
    usescr = frame_win(current);
    wrefresh(usescr);
    } while(!isQuit(c = wGetchar(usescr), TRUE) && (c != ERR));

    noraw();
    erase();
    endwin();
    delwin(current -> wind);
    //detruire_buffer(buffer);
    return OK;

}


int main()
{   WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c =0;

    initscr();
    clear();
    //noecho();
    //cbreak();   /* Line buffering disabled. pass on everything */

    pbuffer buffer = NULL;
    startx = (40 - WIDTH) / 2;
    starty = (14 - HEIGHT) / 2;

    menu_win = subwin(stdscr,LINES -3, COLS -2,1,0);
    debut_menu(c, menu_win,highlight,choice, buffer);
    endwin();
    set_terminal_modes();
    delwin(menu_win);
    endwin();
    return 0;
}

int debut_menu(int c, WINDOW * win, int highlight, int choice, pbuffer buffer){
    bool b =TRUE;
    keypad(win, TRUE);
    mvprintw(0, 0, "Utilisez les flèches Haut et Bas, Appuyer sur entrer pour votre choix");
    refresh();
    print_menu(win, highlight);

    while(1)
    {   print_menu(win, highlight);
        c = wgetch(win);
        switch(c)
        {   case KEY_UP:
                if(highlight == 1){
                    highlight = n_choices;
                }else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
               // choice = highlight;
                if (highlight == 1)
                {
                    initscr();
                    clear();
                    endwin();
                    delwin(win);
                    printw("donnez le nom de votre fichier à ouvrir.....");

                    char file_name[80];
                    getstr(file_name);

                    buffer = charger(file_name);

                    scroller(win,buffer,c,b);
                    goto breakout;
                }else if (highlight == 2)
                {
                    b=FALSE;
                    clear();
                    endwin();
                    set_terminal_modes();
                    scroller(win,buffer,c,b);

                    goto breakout;
                }else
                {
                    choice = highlight;
                }
    }

        if(choice == 3) /* User did a choice come out of the infinite loop */
            break;
    }
    breakout:
       refresh();
    return 0; 
    
}

void print_menu(WINDOW *menu_win, int highlight)
{
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i)
    {   if(highlight == i + 1) /* High light the present choice */
        {   wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

static void set_terminal_modes(void){
    noraw();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    idlok(stdscr, TRUE);
    keypad(stdscr, TRUE);
}
