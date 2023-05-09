#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maze_maker.h"

int main(int argc, char* argv[]) {
    /* The program needs a path to a directory, image width and height, wall width, corridor width, a seed, and output
     * type. */
    maze_input_t* input;
    if ((argc == 3) && (strcmp(argv[1], "-i") == 0)) {
        FILE* fp = fopen(argv[2], "r");
        if (!fp) {
            printf("Could not open file %s\n", argv[2]);
            return 1;
        }
        input = get_maze_parameters_from_file(fp);
        fclose(fp);
    } else if (argc == 11) {
        input = get_maze_parameters_from_args(argc, argv);
    } else {
        input = get_maze_parameters_from_user();
    }
    if (!input) {
        printf("Could not get maze parameters\n");
        return 1;
    }

    if (ensure_dir(input->output) != 0) {
        printf("Could not create directory %s\n", input->output);
        return 1;
    }
    if (input->video) {
        input->output = video_dir_name(input->output, input->seed);
        if (ensure_dir(input->output) != 0) {
            printf("Could not create directory %s\n", input->output);
            return 1;
        }
    }
    printf("Creating grid with these parameters:\n\tPath: %s\nWidth: %d\n"
           "Height: %d\nWW: %d\nCW: %d\nSeed: %d\nVideo: %s\nSC%%: %lf\n",
           input->output, input->width, input->height, input->wall_width, input->corridor_width, input->seed,
           input->video ? "true" : "false", input->shortcut_chance);
    return 0;
} /* main() */
