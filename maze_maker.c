#include "imager.h"
#include "maze_maker.h"
#include "queue.h"
#include "stb_image_write.h"
#include "windows_dir.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#define CELL_ID(x, y, width) ((y) * (width) + (x))
#define CELL_X(id, width) ((id) % (width))
#define CELL_Y(id, width) ((id) / (width))

/*
 * Starts the maze building process.
 */

int start_build(maze_t* maze) {
    maze->maze = calloc(sizeof(maze_cell_t), maze->width * maze->height);
    assert(maze);
    int cell_width = maze->wall_width + maze->corridor_width;
    image_t* image = image_init(maze->width * cell_width + maze->wall_width,
                                maze->height * cell_width + maze->wall_width,
                                1);
    srand(maze->seed);
    build_border(maze, image);
    draw_maze(image, maze);
    char* filename = string_safe_concat(maze->output, "\\test.png");
    image_save(image, filename);
    free(filename);
    image_destroy(image);

    return 0;
} /* start_build() */

void build_maze(maze_t* maze, image_t* image) {
    int random_cell = rand() % (maze->width * maze->height);
    queue_t* queue = queue_init();
    if (maze->mode == RANDOM) {
        fprintf(stderr, "Random mode not implemented (May never, may use a lot of memory\n");
    } else {
        queue_push(queue, &random_cell);
    }
    queue_push(queue, &random_cell);
    
    
} /* build_maze() */

void build_border(maze_t* maze, image_t* image) {
    for (int i = 0; i < maze->width; i++) {
        maze->maze[i].walls[UP] = true;
        maze->maze[i + (maze->height - 1) * maze->width].walls[DOWN] = true;
    }
    for (int i = 0; i < maze->height; i++) {
        maze->maze[i * maze->width].walls[LEFT] = true;
        maze->maze[i * maze->width + maze->width - 1].walls[RIGHT] = true;
    }
    if (maze->width >= maze->height) {
        int rand_cell_row = rand() % (maze->height / 3) + 1;
        maze->maze[rand_cell_row * maze->width].walls[LEFT] = false;
        rand_cell_row = rand() % (maze->height / 3) + maze->height * 2 / 3;
        maze->maze[rand_cell_row * maze->width + maze->width - 1].walls[RIGHT] = false;
    } else {
        int rand_cell_col = rand() % (maze->width / 3) + 1;
        maze->maze[rand_cell_col].walls[UP] = false;
        rand_cell_col = rand() % (maze->width / 3) + maze->width * 2 / 3;
        maze->maze[rand_cell_col + (maze->height - 1) * maze->width].walls[DOWN] = false;
    }
} /* build_border() */

/*
 * Returns the cell in the given direction from the given cell.
 */

int get_cell_in_dir(int cell, enum directions dir, int width, int height) {
    switch (dir) {
        case UP:
            return cell < width ? -1 : cell - width;
        case DOWN:
            return cell >= (width * (height - 1)) ? -1 : cell + width;
        case LEFT:
            return cell % width == 0 ? -1 : cell - 1;
        case RIGHT:
            return cell % width == width - 1 ? -1 : cell + 1;
        default:
            return -1;
    }
} /* get_cell_in_dir() */

/*
 * Returns a random cell in the given direction from the given cell.
 */

int get_cell_in_random_dir(int cell, int width, int height) {
    /* Not every direction is valid for every cell. */

    int dirs[4];
    int num_dirs = 0;
    for (int i = 0; i < 4; i++) {
        int dir = get_cell_in_dir(cell, i, width, height);
        if (dir != -1) {
            dirs[num_dirs++] = dir;
        }
    }

    if (num_dirs == 0) {
        return -1;
    }

    int dir = dirs[rand() % num_dirs];
    return get_cell_in_dir(cell, dir, width, height);
} /* get_cell_in_random_dir() */

/*
 * Draws a cell of the maze onto the image.
 */

void draw_cell(image_t* image, maze_t* maze, int cell) {
    maze_cell_t* maze_cell = &maze->maze[cell];
    const int WHITE = maze_cell->visited ? 0xFF : 0x79;
    int ww = maze->wall_width;
    int cw = maze->corridor_width;
    int clw = cw + ww;
    int x = CELL_X(cell, maze->width) * clw;
    int y = CELL_Y(cell, maze->width) * clw;
    bool* walls = maze_cell->walls;
    int up = walls[UP] ? 0x00 : WHITE;
    int left = walls[LEFT] ? 0x00 : WHITE;
    int right = walls[RIGHT] ? 0x00 : WHITE;
    int down = walls[DOWN] ? 0x00 : WHITE;
    image_rect_t rects[9] = {
        {x, y, ww, ww, 0}, {x + ww, y, cw, ww, up}, {x + clw, y, ww, ww, 0},
        {x, y + ww, ww, cw, left}, {x + ww, y + ww, cw, cw, WHITE}, {x + clw, y + ww, ww, cw, right},
        {x, y + clw, ww, ww, 0}, {x + ww, y + clw, cw, ww, down}, {x + clw, y + clw, ww, ww, 0}
    };
    image_set_rects(image, rects, 9);
} /* draw_cell() */

void draw_cell_up_left(image_t* image, maze_t* maze, int cell) {
    maze_cell_t* maze_cell = &maze->maze[cell];
    const int WHITE = maze_cell->visited ? 0xFF : 0x79;
    int ww = maze->wall_width;
    int cw = maze->corridor_width;
    int clw = cw + ww;
    int x = CELL_X(cell, maze->width) * clw;
    int y = CELL_Y(cell, maze->width) * clw;
    bool* walls = maze_cell->walls;
    int up = walls[UP] ? 0x00 : WHITE;
    int left = walls[LEFT] ? 0x00 : WHITE;
    image_rect_t rects[4] = {
        {x, y, ww, ww, 0}, {x + ww, y, cw, ww, up},
        {x, y + ww, ww, cw, left}, {x + ww, y + ww, cw, cw, WHITE}
    };
    image_set_rects(image, rects, 4);

} /* draw_cell_up_left() */

/*
 * Draws the maze onto the image.
 */

void draw_maze(image_t* image, maze_t* maze) {
    for (int i = 0; i < (maze->width - 1) * (maze->height - 1); i++) {
        draw_cell_up_left(image, maze, i + i / (maze->width - 1));
    }
    for (int i = 0; i < maze->width - 1; i++) {
        draw_cell(image, maze, i + (maze->height - 1) * maze->width);
    }
    for (int i = 0; i < maze->height; i++) {
        draw_cell(image, maze, (maze->width - 1) + i * maze->width);
    }
} /* draw_maze() */

/*
 * Free the memory used by the maze.
 */

void destroy_maze(maze_t* maze) {
    free(maze->maze);
    free(maze->output);
    free(maze);
} /* destroy_maze() */
