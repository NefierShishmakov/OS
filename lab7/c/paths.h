#ifndef PATHS_H
#define PATHS_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "constants.h"

typedef struct paths_t {
    char *src_path;
    char *dest_path;
    mode_t mode;
} paths_t;

void init_paths_t(paths_t *paths, const char *src_path, 
        const char *dest_path, const char *new_path_el);
void free_paths_t(paths_t *paths);
void set_mode(paths_t *paths, mode_t mode);

#endif
