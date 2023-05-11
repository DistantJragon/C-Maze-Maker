#include "windows_dir.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
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
 * This function concatenates two strings, allocating memory for the result.
 */

char* string_safe_concat(const char* str1, const char* str2) {
    char* result = malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));
    assert(result);
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

/*
 * This function constructs the name of the video directory.
 */

char* video_dir_name(const char* str, uint i) {
    // Determine the length of the resulting string
    size_t str_len = strlen(str);
    size_t int_len = snprintf(NULL, 0, "%d", i);
    size_t total_len = str_len + int_len + 2;  // Add 2 for the \ and NUL

    // Allocate memory for the resulting string
    char* result = (char*) malloc(total_len * sizeof(char));
    assert(result);

    // Copy the string and integer into the resulting string
    snprintf(result, total_len, "%s\\%d", str, i);

    return result;
} /* concat_str_int() */

/*
 * This function constructs the name of the video file.
 */

char* output_file_name(const char* str, uint i, const char* ext) {
    // Determine the length of the resulting string
    size_t str_len = strlen(str);
    size_t int_len = snprintf(NULL, 0, "%d", i);
    size_t ext_len = strlen(ext);
    size_t total_len = str_len + int_len + ext_len + 2;  // Add 2 for the \ and NUL

    // Allocate memory for the resulting string
    char* result = (char*) malloc(total_len * sizeof(char));
    assert(result);

    // Copy the string and integer into the resulting string
    snprintf(result, total_len, "%s\\%d%s", str, i, ext);

    return result;
} /* concat_str_int() */
