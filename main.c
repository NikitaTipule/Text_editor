#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h> 
#include<errno.h> 
#include"buffer.h"
#include<sys/types.h>
#include<unistd.h>
#include<ncurses.h>
//#include "gui.h"


int main(int argc, char *argv[]) {

    int fd, newfile = 0;
    char filename[255]; // max size of character is 255 characters
    buffer *bf;
    init_buffer(&bf);
    if(argc == 2) {
        strcpy(filename, argv[1]);
        if(fileexist(filename)) {
            fd = open(filename, O_RDWR);
            buffer_load(fd, bf);
            close(fd);
            buf_print_stdout(bf);
            distroy_buffer(bf);
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

    // initscr();
    // cbreak();
    // noecho();
    // keypad(stdscr, TRUE);
    // WINDOW * win = newwin(10, 10, 0, 0);
    // int h, w;
    // getmaxyx(stdscr, h, w);
    // wrefresh(win);
    // while(1) {
    //     int i = 0;
    // }
    
    return 0;
}