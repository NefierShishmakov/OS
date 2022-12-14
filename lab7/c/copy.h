#ifndef COPY_H
#define COPY_H

#include <pthread.h>
#include "paths.h"
#include "file_dir_utils.h"
#include "pthread_utils.h"

void* copy_file(void* arg);
void* copy_dir(void* arg);
void set_pthread_function(void** result_function, int file_type);

#endif
