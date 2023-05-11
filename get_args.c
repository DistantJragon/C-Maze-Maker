#include "get_args.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * This function gets the arguments from the user and returns them in a struct.
 */

maze_t* get_maze_parameters_from_user() {
    maze_t* input = malloc(sizeof(maze_t));
    input->output = malloc(100);
    assert(input->output);
    printf("Enter the path to the directory where the maze will be saved: ");
    scanf("%99[^\n]", input->output);
    printf("Enter the width of the maze: ");
    scanf("%d", &input->width);
    printf("Enter the height of the maze: ");
    scanf("%d", &input->height);
    printf("Enter the width of the walls: ");
    scanf("%d", &input->wall_width);
    printf("Enter the width of the corridors: ");
    scanf("%d", &input->corridor_width);
    long temp;
    printf("Enter the seed for the maze: ");
    scanf("%ld", &temp);
    if (temp < 0) {
        input->seed = time(NULL);
    } else {
        input->seed = temp;
    }
    printf("Enter the build mode (0 for first, 1 for random queue, 2 for random, 3 for last): ");
    scanf("%ld", &temp);
    input->mode = temp;
    printf("Enter the output type (0 for image, 1 for video): ");
    scanf("%ld", &temp);
    input->video = temp ? true : false;
    printf("Enter the chance to place a shortcut (0-1): ");
    scanf("%lf", &input->shortcut_chance);
    input->cells = NULL;
    return input;
} /* get_maze_parameters_from_user() */

/*
 * Get the maze parameters from the command line arguments.
 */

maze_t* get_maze_parameters_from_args(int argc, char *argv[]) {
    assert(argc == 11);
    maze_t* input = malloc(sizeof(maze_t));
    input->output = malloc(strlen(argv[1]) + 1);
    assert(input->output);
    strcpy(input->output, argv[1]);
    input->width = atoi(argv[2]);
    input->height = atoi(argv[3]);
    input->wall_width = atoi(argv[4]);
    input->corridor_width = atoi(argv[5]);
    long temp = atol(argv[6]);
    if (temp < 0) {
        input->seed = time(NULL);
    } else {
        input->seed = temp;
    }
    input->mode = atoi(argv[7]);
    input->video = atoi(argv[7]);
    input->shortcut_chance = atof(argv[8]);
    temp = atol(argv[9]);
    assert(temp >= 0 && temp <= 3);
    input->mode = temp;
    input->cells = NULL;
    return input;
} /* get_maze_parameters_from_args() */

/*
 * Get the maze parameters from the input file.
 */

maze_t* get_maze_parameters_from_file(FILE *fp) {
    maze_t* input = malloc(sizeof(maze_t));
    assert(input);
    fscanf(fp, "Path=");

    /* Store the rest of the line in the output path. Need to count the number of characters until the end of the line
     * so we can allocate the correct amount of memory. */

    int count = 0;
    while ((fgetc(fp)) != '\n') {
        count++;
    }
    input->output = malloc(count + 1);
    assert(input->output);
    fseek(fp, -count - 2, SEEK_CUR);
    fgets(input->output, count + 1, fp);

    long temp;
    if (fscanf(fp, "\nWidth=%d\nHeight=%d\nWall width=%d\nCorridor width=%d\nSeed=%ld\n",
               &input->width, &input->height, &input->wall_width, &input->corridor_width, &temp) != 5) {
        return NULL;
    }
    if (temp < 0) {
        input->seed = time(NULL);
    } else {
        input->seed = temp;
    }
    if ((fscanf(fp, "Build Mode=%ld\n", &temp) != 1) || (temp < 0) || (temp > 3)) {
        return NULL;
    }
    input->mode = temp;
    if (fscanf(fp, "Video=%ld\nShortcut chance=%lf", &temp, &input->shortcut_chance) != 2) {
        return NULL;
    }
    input->video = temp ? true : false;
    input->cells = NULL;
    return input;
} /* get_maze_parameters_from_file() */
