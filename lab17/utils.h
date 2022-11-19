#ifndef LAB17_UTILS_H
#define LAB17_UTILS_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "line_constants.h"

int get_result_line_length(size_t entered_line_length);
void handle_error(int status, char *error_reason);

#endif
