#ifndef __windows_dir_h__
#define __windows_dir_h__

#include "common_typedefs.h"

int   ensure_dir(const char*);
char* string_safe_concat(const char* str1, const char* str2);
char* video_dir_name(const char*, uint);
char* output_file_name(const char*, uint, const char*);

#endif
