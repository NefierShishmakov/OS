#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>
#include <errno.h>

#define LINES_NUM 10
#define SUCCESS 0
#define ERROR (-1)

#define PTHREADS_NUM 2
#define MUTEXES_NUM (PTHREADS_NUM + 1)

pthread_mutex_t mutexes[MUTEXES_NUM];

bool mutex_is_locked = false;

typedef struct function_args {
    const char *name;
    int num;
} function_args;

void handle_error(int status, const char *error_reason) {
    errno = status;
    perror(error_reason);
}

void destroy_mutexes(int last_index_of_initialised_mutex) {
    for (int i = 0; i <= last_index_of_initialised_mutex; ++i) {
        pthread_mutex_destroy(&mutexes[i]);
    }
}

int init_mutexes() {
    pthread_mutexattr_t attr;

    int status = pthread_mutexattr_init(&attr);

    if (status != SUCCESS) {
        handle_error(status, "pthread_mutexattr_init");
        return ERROR;
    }

    status = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);

    if (status != SUCCESS) {
        handle_error(status, "pthread_mutexattr_settype");
        pthread_mutexattr_destroy(&attr);

        return ERROR;
    }

    for (int i = 0; i < MUTEXES_NUM; ++i) {
        status = pthread_mutex_init(&mutexes[i], &attr);

        if (status != SUCCESS) {
            handle_error(status, "pthread_mutex_init");
            pthread_mutexattr_destroy(&attr);
            destroy_mutexes(i - 1);

            return ERROR;
        }
    }

    pthread_mutexattr_destroy(&attr);

    return SUCCESS;
}

void *start_routine(void *arg) {
    function_args *args = (function_args *)arg;

    for (int i = 1; i <= args->num; ++i) {
        printf("The %s line is printed\n", args->name);
    }

    return NULL;
}

int main(void) {
    pthread_t pthread_id[PTHREADS_NUM - 1];
    int status = init_mutexes();

    if (status == ERROR) {
        return EXIT_FAILURE;
    }

    function_args child = {"child", LINES_NUM};
    function_args parent = {"parent", LINES_NUM};

    pthread_mutex_lock(&mutexes[0]);

    status = pthread_create(&pthread_id, NULL, start_routine, &child);

    if (status != SUCCESS) {
        handle_error(status, "pthread_create");
        destroy_mutexes(MUTEXES_NUM - 1);

        return EXIT_FAILURE;
    }

    while (!mutex_is_locked) {
        sched_yield();
    }

    status = pthread_join(pthread_id, NULL);

    if (status != SUCCESS) {
        handle_error(status, "pthread_join");
        destroy_mutexes(MUTEXES_NUM - 1);

        return EXIT_FAILURE;
    }

    destroy_mutexes(MUTEXES_NUM - 1);
    return EXIT_SUCCESS;
}
