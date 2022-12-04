#ifndef UTILS_H
#define UTILS_H
#include <sys/stat.h>
#include "constants.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int try_to_mkdir(const char *srcdir);

#endif
