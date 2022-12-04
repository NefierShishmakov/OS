#ifndef UTILS_H
#define UTILS_H
#include <sys/stat.h>
#include <string.h>
#include "constants.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int try_to_mkdir(const char *srcdir);
size_t get_size_of_new_dir_path(char *prev_dir_path, char *dir_to_add);
void prepare_dirs(char *srcdir, char *destdir);

#endif
