#include <stdio.h>
#include <stdlib.h>

#include "maze_maker.h"

int main(int argc, char* argv[]) {
    /* The program needs a path to a directory, image width and height, wall width, corridor width, a seed, and output
     * type. */
    maze_input_t input;
    if (argc != 8) {
        input = get_maze_parameters_from_user();
    } else {
        input = get_maze_parameters_from_args(argc, argv);
    }

    ensure_dir(input.output);
    if (input.video) {
        input.output = video_dir_name(input.output, input.seed);
        ensure_dir(input.output);
    }
    printf("Creating grid with these parameters:\n\tPath: %s\nWidth: %d\n"
           "Height: %d\nWW: %d\nCW: %d\nSeed: %d\nVideo: %s\n",
           input.output, input.width, input.height, input.wall_width, input.corridor_width, input.seed,
           input.video ? "true" : "false");
    return 0;
} /* main() */


