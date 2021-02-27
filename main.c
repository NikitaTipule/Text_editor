#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h> 
#include<errno.h> 
#include<sys/types.h>
#include"buffer.h"
#include"gui.h"
#include<unistd.h>
#include<ncurses.h>


int main(int argc, char *argv[]) {

    int fd, newfile = 0, ht, wd, x , y;
    char filename[255], copybuf[LINEMAX]; // max size of character is 255 characters
    buffer *bf;
    init_buffer(&bf);
    if(argc == 2) {
        strcpy(filename, argv[1]);
        if(fileexist(filename)) {
            fd = open(filename, O_RDWR);
            buffer_load(fd, bf);
            close(fd);
            buf_print_stdout(bf);
            // distroy_buffer(bf);
        }
    }
    else if(argc == 1) {
        newfile = 1;
        // bf->line[0] = '\n';
        // bf->num_chars = 1;
    }
    else {
        printf("USAGE : ./project <filename> or ./project");
        distroy_buffer(bf);
    }

    init_window();


    getmaxyx(stdscr, ht, wd); // ht = heigth , wd = width
    keypad(stdscr, TRUE);
    attron(COLOR_PAIR(1));
    mvprintw(ht/2 - 12, wd/2 - 19, "-------------------------------------");
    mvprintw(ht/2 - 11, wd/2 - 20, "|                                     |");
	mvprintw(ht/2 - 10, wd/2 - 20, "|   WELCOME TO NIKITA's TEXT EDITOR   |");
    mvprintw(ht/2 - 9, wd/2 - 20,  "|                                     |");
    mvprintw(ht/2 - 8, wd/2 - 19,  "-------------------------------------");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	mvprintw(ht/2 - 5, wd/2 - 17, "CTRL+S OR F2 :  SAVE");
	mvprintw(ht/2 - 4, wd/2 - 17, "CTRL+A OR F3 :  SAVE AND QUIT");
	mvprintw(ht/2 - 3, wd/2 - 17, "CTRL+Q OR F4 :  QUIT WITHOUT SAVE");
	mvprintw(ht/2 - 2, wd/2 - 17, "CTRL+F OR F5 :  SEARCH");
	mvprintw(ht/2 - 1, wd/2 - 17, "CTRL+R OR F6 :  SEARCH AND REPLACE");
	mvprintw(ht/2 - 0, wd/2 - 17, "CTRL+X OR F7 :  CUT");
	mvprintw(ht/2 + 1, wd/2 - 17, "CTRL+C OR F8 :  COPY");
	mvprintw(ht/2 + 2, wd/2 - 17, "CTRL+V OR F9 :  PASTE");
	mvprintw(ht/2 + 3, wd/2 - 17, "CTRL+H OR F10 : SHOW HELP WINDOW");
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));
	mvprintw(ht/2 + 6, wd/2 - 17, "WINDOW SIZE: %3d X %3d", wd, ht); 
	mvprintw(ht/2 + 8, wd/2 - 17, "PRESS ANY KEY TO CONTINUE");
	attroff(COLOR_PAIR(1));
	getch();
	clear();
    refresh();
    y = 0, x = 0;
	move(y, x);
	refresh();

    loadwin(bf, 0);
    attron(COLOR_PAIR(1));
	mvprintw(ht - 1, 0, "| filename: %s | row : %3d | col: %3d | copy: \"%s\" |", filename, y, x, copybuf );
	move(y, x);
	attroff(COLOR_PAIR(1));
	refresh();
	move(0, 0);
    getch();
    clear();
    endwin();
  	distroy_buffer(bf);
    refresh();
    
    return 0;
}