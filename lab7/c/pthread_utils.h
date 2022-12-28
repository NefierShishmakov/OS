#ifndef PTHREAD_UTILS_H
#define PTHREAD_UTILS_H

#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "constants.h"
#include "error_handlers.h"

int try_to_create_thread(pthread_t* tid, void *(*start_routine)(void*), void* arg);

#endif
