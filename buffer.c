#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>  
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

// Function to initialise line
void Lineinit(buffer *bf) {
    bf->line = (char *)malloc(sizeof(char) * LINEMAX);
    bf->line = memset(bf->line, '\0', LINEMAX); 
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

// implimented to check that is the buffer loads all the characters into it from an existing file . for testing purpose only.
// this function prints the buffer content to the stdout
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

// It will insert a character at a given location means at cursor position.
void charInsert(buffer *bf, char ch, int loc) {
    if(loc == LINEMAX) {
        return;
    }
    else if(loc == bf->num_chars) {
        bf->line[loc] = ch;
        bf->num_chars++;
        bf->cur_X = loc+1;
    }
    else if(bf->num_chars == 0) {
        bf->line[0] = ch;
        bf->num_chars++;
        bf->cur_X++;
    }
    else {
        memmove(bf->line + loc + 1, bf->line + loc, bf->num_chars - loc);
        bf->line[loc] = ch;
        bf->num_chars++;
        bf->cur_X++;
    }
}


void bufSave(int fd, buffer *bf) {
    if(lseek(fd, 0, SEEK_SET) == -1){
		
		perror("lseek error: ");
	}
    while(bf != NULL) {
        if(write(fd, bf->line, bf->num_chars) == -1) {
            printf("Error while writing :(");
        }
        bf = bf->next;
    }
}