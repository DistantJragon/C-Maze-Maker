#include "maze_maker.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

maze_input_t get_maze_parameters_from_user() {
    maze_input_t input;
    input.output = malloc(100);
    assert(input.output);
    printf("Enter the path to the directory where the maze will be saved: ");
    scanf("%[^\n]", input.output);
    printf("Enter the width of the maze: ");
    scanf("%d", &input.width);
    printf("Enter the height of the maze: ");
    scanf("%d", &input.height);
    printf("Enter the width of the walls: ");
    scanf("%d", &input.wall_width);
    printf("Enter the width of the corridors: ");
    scanf("%d", &input.corridor_width);
    long temp;
    printf("Enter the seed for the maze: ");
    scanf("%ld", &temp);
    if (temp < 0) {
        input.seed = time(NULL);
    } else {
        input.seed = temp;
    }
    printf("Enter the output type (0 for image, 1 for video): ");
    scanf("%ld", &temp);
    input.video = temp;
    return input;
} /* get_maze_parameters_from_user() */

maze_input_t get_maze_parameters_from_args(int argc, char *argv[]) {
    assert(argc == 8);
    maze_input_t input;
    input.output = malloc(strlen(argv[1]) + 1);
    assert(input.output);
    strcpy(input.output, argv[1]);
    input.width = atoi(argv[2]);
    input.height = atoi(argv[3]);
    input.wall_width = atoi(argv[4]);
    input.corridor_width = atoi(argv[5]);
    long temp = atol(argv[6]);
    if (temp < 0) {
        input.seed = time(NULL);
    } else {
        input.seed = temp;
    }
    input.video = atoi(argv[7]);
    return input;
} /* get_maze_parameters_from_args() */
