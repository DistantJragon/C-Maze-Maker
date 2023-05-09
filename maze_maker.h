#ifndef __maze_maker_h__
#define __maze_maker_h__

#include <stdbool.h>
#include <stdio.h>

#define OPP_DIR(dir) (~(dir) & 0x03)

enum directions {
    UP,
    RIGHT,
    LEFT,
    DOWN
};

enum build_mode {
    FIRST,    /* cringe */
    RANDOM_Q, /* based  */
    RANDOM,   /* cringe */
    LAST      /* based  */
};

typedef struct maze_input_s {
    double shortcut_chance;
    int width;
    int height;
    int wall_width;
    int corridor_width;
    unsigned int seed;
    enum build_mode mode;
    char* output;
    bool video;
} maze_input_t;

typedef struct maze_cell_s {
    bool visited;
    bool walls[4];
} maze_cell_t;

int           ensure_dir(const char*);
maze_input_t* get_maze_parameters_from_args(int, char**);
maze_input_t* get_maze_parameters_from_file(FILE*);
maze_input_t* get_maze_parameters_from_user();
int           main(int, char**);
char*         video_dir_name(const char*, int);


#endif
