#ifndef __maze_maker_h__
#define __maze_maker_h__

#include "common_typedefs.h"
#include "imager.h"
#include "queue.h"

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
    byte walls[4];
    bool visited;
} maze_cell_t;

typedef struct maze_s {
    double shortcut_chance;
    uint width;
    uint height;
    uint wall_width;
    uint corridor_width;
    uint seed;
    enum build_mode mode;
    char* output;
    bool video;
    maze_cell_t* cells;
} maze_t;

typedef struct array_check_s {
    uint size;
    uint* index;
} array_check_t;

void build_maze(maze_t*, image_t*);
void build_border(maze_t*, image_t*);
void destroy_maze(maze_t*);
void draw_cell(image_t*, maze_t*, uint);
void draw_maze(image_t*, maze_t*);
uint get_cell_in_dir(maze_t*, uint, enum directions);
enum directions random_dir_with_cell(maze_t*, uint, bool);
bool is_empty_array(void*);
bool is_empty_queue(void*);
int main(int, char**);
int remove_cell_from_queue(queue_t*, uint);
uint* shuffled_array(uint);
int start_build(maze_t*);

#endif
