#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define DX 7
#define DY 3

int main(int argc, char *argv[]) {
    if (argc < 2)
        return 1;

    FILE *fd = fopen(argv[1], "r");

    if (!fd) 
        return 1;

    WINDOW *frame, *win;
    int c = 0;

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    refresh();

    frame = newwin(LINES - 2*DY, COLS - 2*DX, DY, DX);
    box(frame, 0, 0);
    mvwaddstr(frame, 0, (int)((COLS - 2*DX - 5) / 2), argv[1]);
    wrefresh(frame);

    win = newwin(LINES - 2*DY - 2, COLS - 2*DX-2, DY+1, DX+1);
    keypad(win, TRUE);
    scrollok(win, TRUE);

    int lin = LINES - 2*DY;
    int col = COLS -2*DX;
    int i = 0;
    char s[9000];

    while (i < lin && fgets(s, 9000, fd)) {
        s[col-1] = '\0';
        mvwaddnstr(win, i, 0, s, col);
        ++i;
    }
    wrefresh(win);
    while((c = wgetch(win)) != 27) {
        if (c != ' ') 
            continue;

        if (fgets(s, 9000, fd)) {
            s[col-1] = '\0';

            wscrl(win, 1);

            mvwaddnstr(win, lin-4, 0, s, col);
        }
    }
    delwin(win);
    delwin(frame);
    endwin();
    return 0;
}
