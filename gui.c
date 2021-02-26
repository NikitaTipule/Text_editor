#include <ncurses.h>
#include "gui.h"

void init_window() {
    initscr();
    noecho();
}