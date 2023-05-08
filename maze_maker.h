#ifndef __maze_maker_h__
#define __maze_maker_h__

typedef struct maze_input_s {
    int width;
    int height;
    int wall_width;
    int corridor_width;
    unsigned int seed;
    char* output;
    int video : 1;
} maze_input_t;

void         ensure_dir(const char*);
maze_input_t get_maze_parameters_from_user();
maze_input_t get_maze_parameters_from_args(int, char**);
int          main(int, char**);
char*        video_dir_name(const char*, int);


#endif
