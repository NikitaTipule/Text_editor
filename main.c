#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>  // for open close of file
#include<errno.h> 
#include"buffer.h"
#include"gui.h"
#include<unistd.h>
#include<ncurses.h>


int main(int argc, char *argv[]) {

    int fd, newfile = 0, ht, wd, x , y, i = 0, ch;
    char filename[255]; // max size of character is 255 in linux
    buffer *bf, *head, *st;
    init_buffer(&bf);
    if(argc == 2) {  // when filename is provided 
        strcpy(filename, argv[1]);
        if(fileexist(filename)) {
            fd = open(filename, O_RDWR);
            buffer_load(fd, bf);
            close(fd);
            //buf_print_stdout(bf);  // to test only that buffer is loaded or not
            // distroy_buffer(bf);
        }
        else {
            printf("File don't exist :(");
            distroy_buffer(bf);
            return 0;
        }
    }
    else if(argc == 1) { // when filename is not provided then start an empty buffer and then insert character into that buffer
        newfile = 1;  // used as a flag to create a new file
    }
    else {
        printf("USAGE : ./editor <filename> or ./editor");
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
	mvprintw(ht/2 + 6, wd/2 - 17, "WINDOW SIZE: %d X %d", wd, ht); 
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
	mvprintw(ht - 1, 0, "| filename: %s | row : %d | col: %d |", filename, y+1, x+1 );
    mvprintw(ht - 1, wd - 35, "| help : Press ctrl + h or F(10) |");
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
                else if(x == 0 && y > 0) {
                    bf = bf->prev;
                    x = bf->num_chars;
                    move(--y, x);
                }
                else if(x == 0 && y == 0) {
                    move(y, x);
                }
				break;


            case KEY_RIGHT: // right arrow
                if(bf->next == NULL && x == bf->num_chars) {
                    move(y, x);
                }
                else if(bf->next != NULL && x == bf->num_chars) {
                    bf = bf->next;
                    x = 0;
                    move(++y, x);
                } 
                else {
                    move(y, ++x);
                }
                break;


            case KEY_UP:  // up arrow
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


            case KEY_DOWN:  // down arrow
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
				
				break;

            case KEY_BACKSPACE:  // backspace

                if(bf != NULL) {
                    if(x != 0) {
                        memmove(bf->line + x - 1, bf->line + x, bf->num_chars - x);
                        bf->num_chars--;
                        move(y, --x);
                        loadwin(head, 0);
                    }
                    else if(y != 0 && x == 0) {

                        if(bf->num_chars == 1) {
                            buffer *temp2;
                            temp2 = bf;
                            while(temp2) {
                                temp2->num_chars--;
                                temp2 = temp2->next;
                            }
                            bf->next->prev = bf->prev;
                            bf->prev->next = bf->next;
                            temp2 = bf;
                            bf = bf->prev;
                            free(temp2->line);
                            free(temp2);
                            move(--y, x = bf->num_chars);
                            loadwin(head, 0);
                        }
                        else if(bf->num_chars < (LINEMAX - (bf->prev->num_chars))) {
                
                            memmove(bf->prev->line + bf->prev->num_chars - 1, bf->line, bf->num_chars);
                            x = bf->num_chars;
                            bf->prev->num_chars = bf->prev->num_chars + bf->num_chars;
                            buffer *temp2;
                            temp2 = bf;
                            while(temp2) {
                                temp2->num_chars--;
                                temp2 = temp2->next;
                            }
                            bf->next->prev = bf->prev;
                            bf->prev->next = bf->next;
                            temp2 = bf;
                            bf = bf->prev;
                            free(temp2->line);
                            free(temp2);
                            move(--y, x);
                            loadwin(head, 0);
                     
                        }
                    }
                    else if(x == 0, y == 0) {
                        move(y, x);
                        loadwin(head, 0);
                    }
                }
                break;

            case 19: // ascii of ctrl + s
            case KEY_F(2): 
                move(ht-1, 0);
                clrtoeol();
                if(newfile == 1) {
                    attron(COLOR_PAIR(1));
                    mvprintw(ht-1, 0, "Enter file name : ");
                    mvscanw(ht-1, strlen("Enter file name : "), "%s", filename);
                    attroff(COLOR_PAIR(1));
                    refresh();
                    if((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
                        printf("Error :(");
                        return 0;
                    }
                    newfile = 0;
                    loadwin(head, 0);
                    move(y, x);
                }
                else {
                    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
                }
                bufSave(fd, head);
                break;
            
            // case KEY_DC :   // delete a character
            //     if(bf->next == NULL && y == bf->cur_line && x == bf->num_chars - 1) {
            //         move(x, y);
            //         loadwin(head, 0);
            //         break;
            //     }
            //     break;


            case 8: // ascii of ctrl + h
            case KEY_F(10):
                clear();
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
                mvprintw(ht/2 + 6, wd/2 - 17, "WINDOW SIZE: %d X %d", wd, ht); 
                mvprintw(ht/2 + 8, wd/2 - 17, "PRESS ANY KEY TO CONTINUE");
                attroff(COLOR_PAIR(1));
                getch();
                clear();
                loadwin(head, 0);
                break;


            case 1: // ascii of ctrl + A (save and quit)
            case KEY_F(3):
                move(ht-1, 0);
                clrtoeol();
                if(newfile == 1) {
                    attron(COLOR_PAIR(1));
                    mvprintw(ht-1, 0, "Enter file name : ");
                    mvscanw(ht-1, strlen("Enter file name : "), "%s", filename);
                    attroff(COLOR_PAIR(1));
                    refresh();
                    if((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
                        printf("Error :(");
                        return 0;
                    }
                    newfile = 0;
                    loadwin(head, 0);
                    move(y, x);
                }
                else {
                    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
                }
                bufSave(fd, head);
                

            case 17:   // ascii of ctrl + Q (quit)
            case KEY_F(4):
                clear();
                distroy_buffer(head);
                refresh();
                endwin();
                exit(0);
                

            case '\n' :
                buf_create_next(bf); // created next buffer
                charInsert(bf, ch, x);
                i = 0;
                bf = bf->next;
                // charInsert(bf, ch, 0);
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
        mvprintw(ht - 1, 0, "| filename: %s | row : %3d | col: %3d |", filename, y+1, x+1 );
        mvprintw(ht - 1, wd - 35, "| help : Press ctrl + h or F(10) |");
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