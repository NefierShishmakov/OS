#include "paths.h"
#include "constants.h"

int init_paths_t(paths_t** paths, const char* src_path, 
        const char* dest_path, const char* new_path_element) {
    size_t src_path_len = get_length_of_new_path(src_path, new_path_element);
    size_t dest_path_len = get_length_of_new_path(dest_path, new_path_element);

    (*paths) = (paths_t*)malloc(sizeof(paths_t));

    if ((*paths) == NULL) {
        fprintf(stderr, "Error allocating paths_t structure\n");
        return ERROR;
    }

    (*paths)->src_path = (char*)malloc(src_path_len * sizeof(char));

    if ((*paths)->src_path == NULL) {
        fprintf(stderr, "Error allocating string for %s%s%s\n", src_path, 
                SEPARATOR, new_path_element);
        free(*paths);
        return ERROR;
    }

    (*paths)->dest_path = (char*)malloc(dest_path_len * sizeof(char));

    if ((*paths)->dest_path == NULL) {
        fprintf(stderr, "Error allocating string for %s%s%s\n", dest_path, 
                SEPARATOR, new_path_element);
        free((*paths)->src_path);
        free(*paths);
        return ERROR;
    }
    
    char* separation_sym = (!strcmp(new_path_element, INITIAL_VALUE) ? INITIAL_VALUE: SEPARATOR);

    strcat(strcat(strcpy((*paths)->src_path, src_path), separation_sym), new_path_element);
    strcat(strcat(strcpy((*paths)->dest_path, dest_path), separation_sym), new_path_element);

    return SUCCESS;
}

void free_paths_t(paths_t* paths) {
    free(paths->dest_path);
    free(paths->src_path);
    free(paths);
}

void set_mode(paths_t* paths, mode_t mode) {
    paths->mode = mode;
}

void prepare_paths(char* first_path, char* second_path) {
    size_t first_path_last_sym_index = strlen(first_path) - 1;
    size_t second_path_last_sym_index = strlen(second_path) - 1;

    if (!strcmp(&first_path[first_path_last_sym_index], SEPARATOR)) {
        first_path[first_path_last_sym_index] = END_OF_STRING;
    }

    if (!strcmp(&second_path[second_path_last_sym_index], SEPARATOR)) {
        second_path[second_path_last_sym_index] = END_OF_STRING;
    }
}

size_t get_length_of_new_path(const char* first_path, const char* second_path) {
    return (strlen(first_path) + strlen(second_path) + 2);
}

bool is_dest_path_in_src_path(const char* src_path, const char* dest_path) {
    return strstr(src_path, dest_path) != NULL;
}
