#include "imager.h"
#include "maze_maker.h"
#include "queue.h"
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
    maze->cells = calloc(sizeof(maze_cell_t), maze->width * maze->height);
    assert(maze);
    uint cell_width = maze->wall_width + maze->corridor_width;
    image_t* image = image_init(maze->width * cell_width + maze->wall_width,
                                maze->height * cell_width + maze->wall_width,
                                1);
    srand(maze->seed);
    build_border(maze, image);
    build_maze(maze, image);
    draw_maze(image, maze);
    char* filename = output_file_name(maze->output, maze->seed, "F.png");
    image_save(image, filename);
    free(filename);
    image_destroy(image);

    return 0;
} /* start_build() */

void build_maze(maze_t* maze, image_t* image) {
    uint maze_size = maze->width * maze->height;
    uint random_cell = rand() % (maze_size);
    queue_t* q = NULL;
    void* queue = NULL;
    uint queue_index = 0;
    uint iteration = 0;
    bool (*is_empty)(void*) = NULL;
    void* check_empty = NULL;
    if (maze->mode == RANDOM) {
        queue = shuffled_array(maze_size);
        is_empty = is_empty_array;
        array_check_t* array_check = malloc(sizeof(array_check_t));
        array_check->index = &queue_index;
        array_check->size = maze_size;
        check_empty = array_check;
    } else {
        q = queue_init();
        queue = q;
        int* p = malloc(sizeof(int));
        assert(p);
        *p = random_cell;
        queue_push(queue, p);
        is_empty = is_empty_queue;
        check_empty = queue;
    }
    while(!(*is_empty)(check_empty)) {
        uint cell_index = -1;
        switch (maze->mode) {
            case RANDOM_Q:
                queue_index = rand() % q->size;
                cell_index = *((uint*) queue_get(q, queue_index));
                break;
            case LAST:
                queue_index = q->size - 1;
            case FIRST:
                cell_index = *((uint*) queue_get(q, queue_index));
                break;
            case RANDOM:
                cell_index = ((uint*) queue)[queue_index];
                break;
            default:
                return;
        }
        maze->cells[cell_index].visited = true;
        uint r_cell;
        for (enum directions dir = UP; dir <= DOWN; dir++) {
            r_cell = get_cell_in_dir(maze, cell_index, dir);
            if ((r_cell != -1) &&
                (maze->cells[r_cell].visited) &&
                (maze->cells[cell_index].walls[dir] != 1)) {
                maze->cells[cell_index].walls[dir] = 2;
                maze->cells[r_cell].walls[OPP_DIR(dir)] = 2;
            }
        }
        enum directions r_dir = random_dir_with_cell(maze, cell_index, true);
        if ((r_dir != -1) && (maze->mode != RANDOM)) {
            r_cell = get_cell_in_dir(maze, cell_index, r_dir);
            assert(r_cell != -1);
            int* ptr = malloc(sizeof(int));
            assert(ptr);
            *ptr = r_cell;
            queue_push(queue, ptr);
            maze->cells[cell_index].walls[r_dir] = 1;
            maze->cells[r_cell].walls[OPP_DIR(r_dir)] = 1;
            maze->cells[r_cell].visited = true;
        } else if (maze->mode != RANDOM) {
            free(queue_pop(q, queue_index));
        } else if (r_dir == -1) {
            queue_index++;
        }
        if (maze->video) {
            draw_maze(image, maze);
            char* filename = output_file_name(maze->output, iteration++, ".png");
            image_save(image, filename);
            free(filename);
        }
    }
} /* build_maze() */

void build_border(maze_t* maze, image_t* image) {
    uint i = 0;
    for (i = 0; i < maze->width; i++) {
        maze->cells[i].walls[UP] = 2;
        maze->cells[i + (maze->height - 1) * maze->width].walls[DOWN] = 2;
    }
    for (i = 0; i < maze->height; i++) {
        maze->cells[i * maze->width].walls[LEFT] = 2;
        maze->cells[i * maze->width + maze->width - 1].walls[RIGHT] = 2;
    }
    if (maze->width >= maze->height) {
        uint rand_cell_row = rand() % (maze->height / 3) + 1;
        maze->cells[rand_cell_row * maze->width].walls[LEFT] = false;
        rand_cell_row = rand() % (maze->height / 3) + maze->height * 2 / 3;
        maze->cells[rand_cell_row * maze->width + maze->width - 1].walls[RIGHT] = 1;
    } else {
        uint rand_cell_col = rand() % (maze->width / 3) + 1;
        maze->cells[rand_cell_col].walls[UP] = false;
        rand_cell_col = rand() % (maze->width / 3) + maze->width * 2 / 3;
        maze->cells[rand_cell_col + (maze->height - 1) * maze->width].walls[DOWN] = 1;
    }
} /* build_border() */

/*
 * Returns the cell in the given direction from the given cell.
 */

uint get_cell_in_dir(maze_t* maze, uint cell, enum directions dir) {
    uint width = maze->width;
    uint height = maze->height;
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
 * Returns a random direction of which cell has a (unvisited) neighbour.
 */

uint random_dir_with_cell(maze_t* maze, uint cell, bool filter_unvisited) {
    /* Not every direction is valid for every cell. */

    byte dirs[DOWN - UP + 1];
    byte num_dirs = 0;
    for (enum directions dir = UP; dir <= DOWN; dir++) {
        uint cell_in_dir = get_cell_in_dir(maze, cell, dir);
        if ((cell_in_dir != -1) &&
            (!(filter_unvisited) || 
             !(maze->cells[cell_in_dir].visited))) {
            dirs[num_dirs++] = dir;
        }
    }

    if (num_dirs == 0) {
        return -1;
    }

    return dirs[rand() % num_dirs];
} /* get_cell_in_random_dir() */

/*
 * Draws a cell of the maze onto the image.
 */

void draw_cell(image_t* image, maze_t* maze, uint cell) {
    maze_cell_t* maze_cell = &maze->cells[cell];
    uint ww = maze->wall_width;
    uint cw = maze->corridor_width;
    uint clw = cw + ww;
    uint x = CELL_X(cell, maze->width) * clw;
    uint y = CELL_Y(cell, maze->width) * clw;
    byte* walls = maze_cell->walls;
    uint up = walls[UP] == 0 ? 0x9F : (walls[UP] == 2 ? 0x00 : 0xFF);
    uint down = walls[DOWN] == 0 ? 0x9F : (walls[DOWN] == 2 ? 0x00 : 0xFF);
    uint left = walls[LEFT] == 0 ? 0x9F : (walls[LEFT] == 2 ? 0x00 : 0xFF);
    uint right = walls[RIGHT] == 0 ? 0x9F : (walls[RIGHT] == 2 ? 0x00 : 0xFF);
    image_rect_t rects[9] = {
        {x, y, ww, ww, 0}, {x + ww, y, cw, ww, up}, {x + clw, y, ww, ww, 0},
        {x, y + ww, ww, cw, left}, {x + ww, y + ww, cw, cw, 0xFF}, {x + clw, y + ww, ww, cw, right},
        {x, y + clw, ww, ww, 0}, {x + ww, y + clw, cw, ww, down}, {x + clw, y + clw, ww, ww, 0}
    };
    image_set_rects(image, rects, 9);
} /* draw_cell() */

void draw_cell_up_left(image_t* image, maze_t* maze, uint cell) {
    maze_cell_t* maze_cell = &maze->cells[cell];
    uint ww = maze->wall_width;
    uint cw = maze->corridor_width;
    uint clw = cw + ww;
    uint x = CELL_X(cell, maze->width) * clw;
    uint y = CELL_Y(cell, maze->width) * clw;
    byte* walls = maze_cell->walls;
    uint up = walls[UP] == 0 ? 0x9F : (walls[UP] == 2 ? 0x00 : 0xFF);
    uint left = walls[LEFT] == 0 ? 0x9F : (walls[LEFT] == 2 ? 0x00 : 0xFF);
    image_rect_t rects[4] = {
        {x, y, ww, ww, 0}, {x + ww, y, cw, ww, up},
        {x, y + ww, ww, cw, left}, {x + ww, y + ww, cw, cw, 0xFF}
    };
    image_set_rects(image, rects, 4);
} /* draw_cell_up_left() */

/*
 * Draws the maze onto the image.
 */

void draw_maze(image_t* image, maze_t* maze) {
    /* for (int i = 0; i < maze->width * maze->height; i++) { */
    /*     draw_cell_up_left(image, maze, i); */
    /* } */
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
    free(maze->cells);
    free(maze->output);
    free(maze);
} /* destroy_maze() */

/*
 * This function creates a shuffled array of integers from 0 to max_num - 1.
 */

uint* shuffled_array(uint max_num) {
    uint* arr = malloc(sizeof(uint) * (max_num));
    uint i, j, temp;
    for (i = 0; i < max_num; i++) {
        arr[i] = i;
    }
    for (i = max_num - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    return arr;
} /* random_array() */

bool is_empty_queue(void* data) {
    return ((queue_t*) data)->size == 0;
} /* is_empty_queue() */

bool is_empty_array(void* data) {
    array_check_t array_check = *((array_check_t*) data);
    return *array_check.index >= array_check.size;
} /* is_empty_array() */

int remove_cell_from_queue(queue_t* queue, uint cell) {
    uint i;
    queue_node_t* node = queue->head;
    for (i = 0; i < queue->size; i++) {
        if (*((uint*) node->data) == cell) {
            queue_pop(queue, i);
            return 0;
        }
        node = node->next;
    }
    return 1;
} /* remove_cell_from_queue() */
