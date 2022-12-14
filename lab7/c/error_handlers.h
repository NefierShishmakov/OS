#ifndef ERROR_HANDLERS_H
#define ERROR_HANDLERS_H

#include <string.h>
#include <stdio.h>
#include "constants.h"

void handle_file_error(const char *error_cause, const char *file_path, int errnum);
void handle_dir_error(const char *error_cause, const char *dir_path, int errnum);
void handle_error(const char *error_cause, int errnum);

#endif
