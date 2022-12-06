#ifndef UTILS_H
#define UTILS_H

#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include "constants.h"

int try_to_mkdir(const char *dir, mode_t mode);
int try_to_open_dir(DIR **dir_stream, const char *dir_path);
int try_to_open_file(const char *pathname, int flags, mode_t mode);
size_t get_length_of_new_path(const char *first_path, const char *second_path, 
        bool is_separator_needed);
void prepare_paths(char *first_path, char *second_path);
void handle_error(int errnum);

#endif
