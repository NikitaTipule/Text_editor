#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>  // for open close of file
#include<errno.h> 
#include<sys/types.h> // for system calls
#include"buffer.h"
#include"gui.h"
#include <sys/stat.h>   
#include<unistd.h>
#include<ncurses.h>


int main(int argc, char *argv[]) {

    int fd, newfile = 0, ht, wd, x , y, i = 0, ch;
    char filename[255]; // max size of character is 255 characters
    buffer *bf, *head, *st;
    init_buffer(&bf);
    if(argc == 2) {  // when filename is provided 
        strcpy(filename, argv[1]);
        if(fileexist(filename)) {
            fd = open(filename, O_RDWR);
            buffer_load(fd, bf);
            close(fd);
            buf_print_stdout(bf);
            // distroy_buffer(bf);
        }
    }
    else if(argc == 1) { // when filename is not provided then start an empty buffer and then insert character into that buffer
        newfile = 1;
    }
    else {
        printf("USAGE : ./project <filename> or ./project");
        distroy_buffer(bf);
    }

    init_window();


    getmaxyx(stdscr, ht, wd); // ht = heigth , wd = width
    keypad(stdscr, TRUE);
    mvprintw(ht/2 - 12, wd/2 - 19, "-------------------------------------");
    mvprintw(ht/2 - 11, wd/2 - 20, "|                                     |");
    attron(COLOR_PAIR(1));
	mvprintw(ht/2 - 10, wd/2 - 20, "|   WELCOME TO NIKITA's TEXT EDITOR   |");
	attroff(COLOR_PAIR(1));
    mvprintw(ht/2 - 9, wd/2 - 20,  "|                                     |");
    mvprintw(ht/2 - 8, wd/2 - 19,  "-------------------------------------");
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
	mvprintw(ht - 1, 0, "| filename: %s | row : %3d | col: %3d |", filename, y, x );
	move(y, x);
	attroff(COLOR_PAIR(1));
	refresh();
	move(0, 0);

    head = bf, st = bf;
    while(ch = getch()) {


        switch(ch) {
            
			case KEY_LEFT:  // left arrow
				if(x > 0){
					move(y, --x);
				}
				break;

            case KEY_RIGHT: // right arrow
                if(x < LINEMAX) {
                    move(y, ++x);
                }
                break;

            case KEY_UP:
                if(y != 0) {
                    y--;
                    bf = bf->prev;
                    if(x >= bf->num_chars) {
                        x = bf->num_chars - 1;
                    }
                    move(y, x);
                }
                else {
                    move(y, x);
                }
                break;

            case KEY_DOWN:
                // if(bf->next != NULL) {
                //     bf = bf->next;
                //     move(bf->cur_line, bf->cur_X);
                //     loadwin(head, 0);
                // } 
                if(y < ht - 2 && bf != NULL){
					if(bf->next != NULL){
						bf = bf->next;
						if(x >= bf->num_chars){
							if(bf->num_chars  > 0){
								x = bf->num_chars - 1;								
							}
							else{
								x = 0;
							}
							move(++y, x);
						}
						else{
							move(++y, x);
						}
					}
				}
				// else if( y == ht - 2 && bf->next != NULL && st->next != NULL){
				// 	st = st->next;
				// 	bf = bf->next;
				// 	loadwin(st, 0);
				// 	move(y, x);
				// }
				break;


            case '3':
                clear();
                distroy_buffer(bf);
                refresh();
                endwin();
                exit(0);
                

            case '\n' :
                buf_create_next(bf); // created next buffer
                charInsert(bf, ch, x);
                i = 0;
                bf = bf->next;
                charInsert(bf, ch, 0);
                x = 0;
                move(++y, x);
                loadwin(head, 0);
                break;
            
            default :
                if(i < LINEMAX) {
                    charInsert(bf, ch, x++);
                    move(bf->cur_line, x);
                    loadwin(head, 0);
                }
                else {
                    
                    buf_create_next(bf); // created next buffer
                    i = 0;
                    bf->next->prev = bf;
                    bf = bf->next;
                    bf->cur_line = bf->prev->cur_line + 1;
                    bf->line[i] = ch;
                    bf->num_chars = i;
                    bf->cur_X = ++i;
                    move(bf->cur_line, x);
                    loadwin(head, 0);
                }
                break;


        }
        attron(COLOR_PAIR(1));
        mvprintw(ht - 1, 0, "| filename: %s | row : %3d | col: %3d |", filename, y, x );
        move(y, x);
        attroff(COLOR_PAIR(1));
        refresh();
    }

    clear();
  	distroy_buffer(bf);
    refresh();
    endwin();
    
    return 0;
}