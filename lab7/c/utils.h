#ifndef UTILS_H
#define UTILS_H
#include <sys/stat.h>
#include <string.h>
#include "constants.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int try_to_mkdir(const char *dir);
size_t get_length_of_new_path(const char *first_path, const char *second_path);
void prepare_dirs(char *srcdir, char *destdir);

#endif
