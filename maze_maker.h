#ifndef __maze_maker_h__
#define __maze_maker_h__

#include "imager.h"

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

typedef struct maze_cell_s {
    bool walls[4];
    bool visited;
} maze_cell_t;

typedef struct maze_s {
    double shortcut_chance;
    int width;
    int height;
    int wall_width;
    int corridor_width;
    unsigned int seed;
    enum build_mode mode;
    char* output;
    bool video;
    maze_cell_t* maze;
} maze_t;

void build_maze(maze_t*, image_t*);
void build_border(maze_t*, image_t*);
void destroy_maze(maze_t*);
void draw_cell(image_t*, maze_t*, int);
void draw_maze(image_t*, maze_t*);
int get_cell_in_dir(int, enum directions, int, int);
int get_cell_in_random_dir(int, int, int);
int get_unvisited_cell_in_random_dir(maze_cell_t*, int, int, int);
int main(int, char**);
int start_build(maze_t*);

#endif
