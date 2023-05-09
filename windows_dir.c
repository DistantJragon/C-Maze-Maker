#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/*
 * This function ensures that the directory exists.
 */

int ensure_dir(const char* path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        return mkdir(path);
    }
    return 0;
} /* ensure_dir() */

/*
 * This function constructs the name of the video directory.
 */

char* video_dir_name(const char* str, int i) {
    // Determine the length of the resulting string
    size_t str_len = strlen(str);
    size_t int_len = snprintf(NULL, 0, "%d", i);
    size_t total_len = str_len + int_len + 2;  // Add 2 for the / and NUL

    // Allocate memory for the resulting string
    char* result = (char*) malloc(total_len * sizeof(char));
    assert(result);

    // Copy the string and integer into the resulting string
    snprintf(result, total_len, "%s/%d", str, i);

    return result;
} /* concat_str_int() */

