#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h> 
#include<sys/types.h>
#include<errno.h> 
#include"buffer.h"
#include<ncurses.h>
#include<unistd.h>
#include"gui.h"


// Function to initialize a buffer
void init_buffer(buffer **bf) {
    (*bf) = (buffer *)malloc(sizeof(buffer));
    (*bf)->prev = NULL;
    (*bf)->next = NULL;
    (*bf)->cur_line = 0;
    (*bf)->num_chars = 0;
    (*bf)->cur_X = 0;
    Lineinit(*bf);
    return;
}


// Function to check the provided file is exist or not.
int fileexist(char *filename) {
    int fd;
    fd = open(filename, O_RDWR);
    if(fd == -1)
        return 0;
    close(fd);
    return 1;
}

// Function to init line
void Lineinit(buffer *bf) {
    bf->line = (char *)malloc(sizeof(char) * LINEMAX);
    bf->line = memset(bf->line, '\0', LINEMAX);
    return;
}

// Function to create next buffer
void buf_create_next(buffer *bf) {
    bf->next = (buffer *)malloc(sizeof(buffer));
    bf->next->prev = bf;
    bf->next->cur_line = bf->cur_line + 1;
    bf->next->num_chars = 0;
    bf->next->cur_X = 0;
    bf = bf->next;
    Lineinit(bf);
}

void buffer_load(int fd, buffer *bf) {
    char ch;
    int err, linenumber = 0, i = 0;

    while(err = read(fd, &ch, 1)) {
        if(err == -1) {
            printf("Read error :( ");
            exit(0);
        }
        if(i < LINEMAX) {
            if(ch == '\n') {
                buf_create_next(bf);
                bf->line[i] = ch;
                bf->cur_line = linenumber;
                bf->num_chars = i + 1;
                bf = bf->next;
                linenumber++;
                i = 0;
                continue;
            }
            else {
                bf->line[i] = ch;
                bf->num_chars = i; 
            }
        }
        else {
            buf_create_next(bf);
            bf->line[i] = ch;
            bf->cur_line = linenumber;
            bf->num_chars = i + 1;
            bf = bf->next;
            linenumber++;
            i = 0;
            continue; 
        }
        i++;
    }
}

void buf_print_stdout(buffer *bf) {
    while(1) {
        if(bf == NULL) 
            return;
        else {
            fputs(bf->line, stdout);
            bf = bf->next;
        }
    }
}

// void distroy_buffer(buffer *bf) {
//     while(bf != NULL) {
//         buffer *b = bf;
//         bf = bf->next;
//         bf->prev = NULL;
//         b->next = NULL;
//         free(b->line);
//         free(b);
//     }
// }

/*destroys (clears all data in them) all the buffers after and including current buffer. */
void distroy_buffer(buffer *bf){
	buffer *temp;

	while(bf != NULL){
		temp = bf;
		bf = bf->next;
		free(temp->line);
		free(temp);
	}

}