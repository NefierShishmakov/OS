#include "paths.h"
#include "constants.h"
#include "utils.h"

void init_paths_t(paths_t *paths, const char *src_path, 
        const char *dest_path, const char *new_path_el) {
    size_t src_path_len = get_length_of_new_path(src_path, new_path_el);
    size_t dest_path_len = get_length_of_new_path(dest_path, new_path_el);

    paths->src_path = (char *)malloc(src_path_len * sizeof(char));
    paths->dest_path = (char *)malloc(dest_path_len * sizeof(char));
    
    char *separation_sym = (!strcmp(new_path_el, INITIAL_VALUE) ? INITIAL_VALUE: SEPARATOR);

    strcat(strcat(strcpy(paths->src_path, src_path), separation_sym), new_path_el);
    strcat(strcat(strcpy(paths->dest_path, dest_path), separation_sym), new_path_el);
}

void free_paths_t(paths_t *paths) {
    free(paths->dest_path);
    free(paths->src_path);
    free(paths);
}

void set_mode(paths_t *paths, mode_t mode) {
    paths->mode = mode;
}
