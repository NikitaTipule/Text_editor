#define LINEMAX 180


typedef struct buffer {
    int cur_line;
    int num_chars;
    int cur_X;
    char *line;
    struct buffer *prev;
    struct buffer *next;
}buffer;

void init_buffer(buffer **bf);
int fileexist(char *filename);
void Lineinit(buffer *bf);
void buf_create_next(buffer *bf);
void buffer_load(int fd, buffer *bf);
void buf_print_stdout(buffer *bf);
void distroy_buffer(buffer *bf);
void charInsert(buffer *bf, char ch, int loc);
void bufSave(int fd, buffer *bf);
void bufInsert_betw(buffer *bf);
void buf_Decr_lineno(buffer *bf, int no);
void buf_Incr_lineno(buffer *bf, int no);
