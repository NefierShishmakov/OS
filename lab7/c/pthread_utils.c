#include "pthread_utils.h"
#include "error_handlers.h"
#include <pthread.h>

int try_to_create_thread(pthread_t *tid, void *(*start_routine)(void *), void *arg) {
    pthread_attr_t attr;

    int status = pthread_attr_init(&attr);

    if (status != SUCCESS) {
        errno = status;
        handle_error("pthread_attr_init", errno);
        return ERROR;
    }

    status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (status != SUCCESS) {
        errno = status;
        handle_error("pthread_attr_setdetachstate", errno);
        pthread_attr_destroy(&attr);
        return ERROR;
    }

    status = pthread_create(tid, &attr, start_routine, arg);

    if (status != SUCCESS) {
        errno = status;
        handle_error("pthread_create", errno);
        pthread_attr_destroy(&attr);
        return ERROR;
    }

    pthread_attr_destroy(&attr);

    return SUCCESS;
}
