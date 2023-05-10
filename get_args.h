#ifndef __get_args_h__
#define __get_args_h__

#include "maze_maker.h"

maze_t* get_maze_parameters_from_args(int, char**);
maze_t* get_maze_parameters_from_file(FILE*);
maze_t* get_maze_parameters_from_user();

#endif
