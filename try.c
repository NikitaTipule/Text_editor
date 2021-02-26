#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<fcntl.h> 
#include<unistd.h>
#include <sys/types.h>
#include<errno.h> 

#define LINEMAX 180
typedef struct buffer {
    struct buffer *prev;
    struct buffer *next;

    int cur_line;
    int num_char;
    int cur_X;
    char *line;
}buffer;



void init_buffer(buffer **bf);
int fileexist(char *filename);
void Lineinit(buffer **bf);
void init_buffer(buffer **bf) {
    printf("jdsh");
    (*bf) = (buffer *)malloc(sizeof(buffer));
    (*bf)->prev = NULL;
    (*bf)->next = NULL;
    (*bf)->cur_line = 0;
    (*bf)->num_char = 0;
    (*bf)->cur_X = 0;
    printf("niks");
    Lineinit(bf);
    return;
}
int fileexist(char *filename) {
    int fd;
    fd = open(filename, O_RDWR);
    if(fd == -1)
        return 0;
    close(fd);
    return 1;
}

void Lineinit(buffer **bf) {
    (*bf)->line = (char *)malloc(sizeof(char) * LINEMAX);
    printf("helo");
    (*bf)->line = memset((*bf)->line, '\0', LINEMAX);
    return;
}



int main(int argc, char *argv[]) {

    int fd;
    char filename[255]; // max size of character is 255 characters
    buffer *bf;
    init_buffer(&bf);
    if(argc == 2) {
        strcpy(filename, argv[1]);
        if(fileexist(filename)) {
            fd = open(filename, O_RDWR);
            // buffer_load(fd, bf);
            char ch;
            int re = read(fd, &ch, 1);
            printf("%c\n", ch);
            close(fd);
            // buf_print_stdout(bf);
            // distroy_buffer(bf);
        }
    }
    return 0;
}