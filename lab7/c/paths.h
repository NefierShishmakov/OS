#ifndef PATHS_H
#define PATHS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"

#define SEPARATOR "/"
#define INITIAL_VALUE ""
#define END_OF_STRING '\0'

typedef struct paths {
    char* src_path;
    char* dest_path;
    mode_t mode;
} paths_t;

int init_paths_t(paths_t** paths, const char* src_path, const char* dest_path, 
        const char* new_path_element);
void free_paths_t(paths_t* paths);
void set_mode(paths_t* paths, mode_t mode);
size_t get_length_of_new_path(const char* first_path, const char* second_path);
void prepare_paths(char* first_path, char* second_path);
bool is_dest_path_in_src_path(const char* src_path, const char* dest_path);

#endif
