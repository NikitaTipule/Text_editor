#include"buffer.h"
#include"gui.h"
#include<ncurses.h>

void init_window() {
    initscr();
    raw();
    curs_set(2);
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
}


void loadwin(buffer *bf, int y) {
    clear();
    refresh();
    attron(COLOR_PAIR(3));
    int x = 0;
    while(bf != NULL) {
        mvprintw(y,x, "%s", bf->line);
        y++;
        bf = bf->next;
    }
    refresh();
}