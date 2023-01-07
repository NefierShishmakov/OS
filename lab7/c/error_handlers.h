#ifndef ERROR_HANDLERS_H
#define ERROR_HANDLERS_H

#define ERROR_BUFFER_SIZE 1025

#include <string.h>
#include <stdio.h>
#include "constants.h"

void handle_file_or_dir_error(const char* error_cause, const char* path, int errnum);
void handle_error(const char* error_cause, int errnum);

#endif
