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

    int fd, newfile = 0, ht, wd, x , y, i = 0, ch, offY = 0, total_lines;
    char filename[255]; // max size of character is 255 in linux
    buffer *bf, *head, *st, *winStart;
    buffer *temp;
    char search[LINEMAX];
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
    mvprintw(ht/2 + 4, wd/2 - 17, "CTRL+G OR F11 : GO TO LINE NUMBER");
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
	mvprintw(ht - 1, 0, "| filename: %s | row : %d | col: %d | num_chars : %d", filename, bf->cur_line + 1, x+1, bf->num_chars );
    mvprintw(ht - 1, wd - 35, "| help : Press ctrl + h or F(10) |");
	move(y, x);
	attroff(COLOR_PAIR(1));
	refresh();
	move(0, 0);

    head = bf, st = bf, winStart = bf;
    while(ch = getch()) {


        switch(ch) {
            
			case KEY_LEFT:  // left arrow
				if(x > 0){
					move(y, --x);
				}
                else if(x == 0 && y > 0) {
                    bf = bf->prev;
                    x = bf->num_chars-1;
                    move(--y, x);
                }
                else if(x == 0 && y == 0 && winStart->prev == NULL) {
                    move(y, x);
                }
                else if(x == 0 && y == 0 && winStart->prev != NULL) {
                    bf = bf->prev;
                    winStart = winStart->prev;
                    x = bf->num_chars - 1;
                    move(y, x);
                    loadwin(winStart, 0);
                }
				break;


            case KEY_RIGHT: // right arrow
                if(bf->next == NULL && x == bf->num_chars-1) {
                    move(y, x);
                }
                else if(bf->next != NULL && x == bf->num_chars-1 && y < ht-2) {
                    bf = bf->next;
                    x = 0;
                    move(++y, x);
                } 
                else if(bf->next != NULL && x == bf->num_chars -1 && y == ht- 2){
                    winStart = winStart -> next;
                    bf = bf->next;
                    x = 0;
                    move(y, x);
                    loadwin(winStart, 0);
                }
                else if(bf->next == NULL && bf->num_chars == 1){
                    move(y, x);
                }
                else {
                    move(y, ++x);
                }
                break;


            case KEY_UP:  // up arrow
                if(winStart->prev != NULL && y == 0 && bf->prev != NULL) {
                    
                    bf = bf->prev;
                    winStart = winStart->prev;
                    if(x >= bf->num_chars) {
                        x = bf->num_chars - 1;
                    }
                    move(y, x);
                    loadwin(winStart, 0);
                }
                else if(y > 0) {
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
                // loadwin(winStart, 0); // just added to see bf->num_chars is works fine or not
                break;


            case KEY_DOWN:  // down arrow
                
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
                else if( y == ht -2 && bf->next != NULL && winStart->next != NULL) {
                    
                    winStart = winStart->next;
                    bf = bf->next;
                    loadwin(winStart, 0);
                    move(y, x);
                }
				//loadwin(winStart, 0); just added to see bf->num_chars works fine or not
				break;

            case KEY_BACKSPACE:  // backspace
                
                if(x == 0 && y == 0 && bf->prev == NULL) {
                    move(y, x);
                }
                // else if(x == (bf->num_chars - 1)) {
                //     bf->line[x] = '\0';
                //     bf->num_chars--;
                //     move(y, --x);
                //     loadwin(winStart, 0);
                // }
                else if((x>0) && x <= bf->num_chars) {
                    memmove(bf->line + x - 1, bf->line + x, bf->num_chars - x + 1);
                    bf->num_chars--;
                    bf->cur_X = x;
                    loadwin(winStart, 0);
                    move(y, --x);


                }
                else if(x == 0 && y > 0 && (bf->prev->num_chars + bf->num_chars - 1) > LINEMAX) {
                    memmove(bf->prev->line + bf->num_chars, bf->line, LINEMAX - bf->prev->num_chars);
                    bf = bf->prev;
                    x = bf->num_chars;
                    y--;
                    move(y, x);
                    loadwin(winStart, 0);
                }
                else if(x == 0 && bf->prev != NULL && (bf->prev->num_chars + bf->num_chars - 1) < LINEMAX) {
                    if(bf == winStart) {
                        winStart = winStart->prev;
                    }
                    if(bf->prev->num_chars == 0) {
                        bf->prev->line[0] = '\n';
                        bf->prev->num_chars = 1;
                    }
                    memmove(bf->prev->line + bf->prev->num_chars - 1, bf->line, bf->num_chars);
                    bf->prev->next = bf->next;
                    if(bf->next != NULL) {
                        bf->next->prev = bf;
                    }
                    x = bf->num_chars - 1;
                    bf->prev->num_chars = bf->prev->num_chars + bf->num_chars - 1;
                    buffer *temp = bf->prev;
                    free(bf->line);
                    free(bf);
                    bf = temp;
                    if(bf->next != NULL) {
                        buf_Decr_lineno(bf->next, 1);
                    }
                    if(y == 0) {
                        loadwin(winStart, 1);
                        move(y, bf->cur_X = x = bf->num_chars-x-1);
                    }
                    else {
                        y--;
                        move(y, bf->cur_X = x = bf->num_chars - x - 1);
                        loadwin(winStart, 0);
                    }

                }
                else if(x == 0 && y == 0 && bf->prev == NULL && bf->num_chars == 0) {
                    bf->line[0] = '\n';
                    bf->num_chars = 1;
                    move(y, x);
                    loadwin(winStart, 0);
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
                mvprintw(ht/2 + 4, wd/2 - 17, "CTRL+G OR F11 : GO TO LINE NUMBER");
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

            // case  : //ctrl + G
            //     move(ht-1, 0);
            //     clrtoeol();
            //     attron(COLOR_PAIR(1));
            //     mvprintw(ht-1, 0, "Enter Line number : ");
            //     mvscanw(ht-1, strlen("Enter file name : "), "%d", filename);
            //     attroff(COLOR_PAIR(1));
            //     refresh();
            //         if((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
            //             printf("Error :(");
            //             return 0;
            //         }
            //         newfile = 0;
            //         loadwin(head, 0);
            //         move(y, x);
                
               
                

            case '\n' :
                // if(x == bf->num_chars && bf->next == NULL) {
                    // buf_create_next(bf); // created next buffer
                    // charInsert(bf, ch, x);
                    // i = 0;
                    // bf = bf->next;
                    // // charInsert(bf, ch, 0);
                    // x = 0;
                    // move(++y, x);
                    // loadwin(head, 0);
                // }
                // else if(bf -> next == NULL) {
                //     buf_create_next(bf);
                //     charInsert(bf, ch, x);
                //     i = 0;
                //     memmove(bf->next->line + x, bf->line + x, bf->num_chars - x);
                //     memset(bf->line + x, '\0', bf->num_chars - x);
                //     bf->num_chars = bf->num_chars - x;
                //     bf = bf -> next;
                //     x = 0;
                //     y++;
                //     move(y, x);
                //     loadwin(head, 0);
                // }
                // else {
                //     buffer *temp;
                //     temp = bf->next;
                //     buf_create_next(bf);
                //     temp->prev = bf->next;
                //     memmove(bf->next->line + x, bf->line + x, bf->num_chars - x);
                //     memset(bf->line, '\0', bf->num_chars - x);
                //     bf = bf->next;
                //     while(temp) {
                //         temp->cur_line + 1;
                //         temp = temp->next;
                //     }
                //     x = 0;
                //     y++;
                //     move(y, x);
                //     loadwin(head, 0);
                // }
                // break;
                //*******************************
                // bf->cur_X = x;
				// charInsert(bf, bf->cur_X, ch);
				// if(x < bf->num_chars -1){
				// 	bufInsert_betw(bf);
				// 	memmove(bf->next->line, (bf->line + x + 1), bf->num_chars - x + 1);
				// 	memset((bf->line + x + 1), '\0', bf->num_chars - x);
				// 	bf->next->num_chars = bf->num_chars - x - 1;
				// 	bf->num_chars = x + 1;
				// 	bf = bf->next;
				// 	if(y < ht - 2)
				// 		move(++y, x = 0);
				// 	else{
				// 		if(winStart->next != NULL){
				// 			winStart = winStart->next;
				// 			offY++;
				// 		}

				// 		move(y = ht - 2, x = 0);
				// 	}
				// 	loadwin(winStart, 0);


				// }
				// else if(x == bf->num_chars || x == bf->num_chars - 1){
				// 	bufInsert_betw(bf);
				// 	clear();
				// 	loadwin(winStart, 0);					
				// 	bf = bf->next;
				// 	if(y < ht - 2)
				// 		move(++y, x = 0);
				// 	else{
				// 		if(winStart->next != NULL){
				// 			winStart = winStart->next;
				// 			offY++;
				// 		}

				// 		move(y = ht - 2, x = 0);
				// 	}
				// 	loadwin(winStart, 0);
				// }
                if(x == bf->num_chars-1 || x == bf->num_chars) {
                    bufInsert_betw(bf);
                    bf->line[x] = '\n';
                    bf->num_chars++;
                    bf = bf->next;
                    if(bf->next != NULL && bf->num_chars == 0) {
                        bf->line[0] = '\n';
                        bf->num_chars++;
                    }
                    y++;
                    x = 0;
                    move(y, x);
                    if(y < ht - 2) {
                        loadwin(winStart, 0);
                    }
                    else {
                        winStart = winStart -> next;
                        loadwin(winStart, 0);
                    }
                }
                else if(x < bf->num_chars) {
                    charInsert(bf, x, ch);
                    // bf->num_chars++;
                    bufInsert_betw(bf);
                    memmove(bf->next->line, (bf->line + x + 1), bf->num_chars - x);
                    memset(bf->line + x + 1, '\0', bf->num_chars - x);
                    bf->next->num_chars = bf->num_chars - x; 
                    bf->num_chars = x + 1;
                    bf = bf -> next;
                    if(y < ht - 2) {
                        move(++y, x = 0);
                    }
                    else {
                        winStart = winStart -> next;
                        move(y = ht-2, x = 0);
                    }
                    move(y, x);
                    loadwin(winStart, 0);
                }
				break;


            case 6: //ascii of ctrl + F  ----- (search)
            case KEY_F(5): 
                temp = head;
                move(ht-1, 0);
                clrtoeol();
                attron(COLOR_PAIR(1));
                mvprintw(ht-1, 0, "Enter Word to Search : ");
                mvscanw(ht-1, strlen("Enter Word to Search : "), "%s", search);
                refresh();
                noecho();
                char *pt = NULL;
                while(temp->next != NULL) {
                    int flag = 0;
                    x = 0;
                    pt = strstr(temp->line, search);
                    if(pt != NULL) {
                        flag = 1;
                        x = pt - temp->line;
                        temp->cur_X = x;
                        bf = temp;
                        attroff(COLOR_PAIR(1));
                        if(temp->cur_line - winStart->cur_line >= 0 && temp->cur_line - winStart->cur_line < ht - 1) {
                            y = temp->cur_line - winStart->cur_line;
                            move(y, x);
                            loadwin(winStart, 0);
                        }
                        else {
                            y = 0;
                            winStart = temp;
                            loadwin(winStart, 0);
                        }
                        break;
                    }
                    temp = temp->next;
                }
                break;
                
            
            default :
                // if(i < LINEMAX) {
                //     charInsert(bf, ch, x++);
                //     move(bf->cur_line, x);
                //     loadwin(winStart, 0);
                // }
                // else {
                    
                //     buf_create_next(bf); // created next buffer
                //     i = 0;
                //     bf->next->prev = bf;
                //     bf = bf->next;
                //     bf->cur_line = bf->prev->cur_line + 1;
                //     bf->line[i] = ch;
                //     bf->num_chars = i;
                //     bf->cur_X = ++i;
                //     move(bf->cur_line, x);
                //     loadwin(winStart, 0);
                // }
                // total_lines++;
                // break;
                if(x < LINEMAX) {
                    charInsert(bf, ch, x++);
                    move(y, x);
                    loadwin(winStart, 0);
                }
                else {
                    buf_create_next(bf);
                    bf= bf->next;
                    x = 0;
                    charInsert(bf, ch, x++);
                    move(++y, x);
                    loadwin(winStart, 0);
                }
                total_lines++;


        }
        attron(COLOR_PAIR(1));
        mvprintw(ht - 1, 0, "| filename: %s | row : %3d | col: %3d | num_chars = %d | x : %d", filename, bf->cur_line + 1, x+1, bf->num_chars, x );
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