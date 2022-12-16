#ifndef FILE_DIR_UTILS_H
#define FILE_DIR_UTILS_H

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
#include "error_handlers.h"

int try_to_mkdir(const char *dir_path, mode_t mode);
int try_to_open_dir_with_retry(DIR **dir_stream, const char *dir_path);
int try_to_open_file_with_retry(const char *file_path, int flags, mode_t mode);
bool is_wrong_element(const char *el);

#endif