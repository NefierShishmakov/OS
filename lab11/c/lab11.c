#include <pthread.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <stdio.h>

#define LINES_NUM 10
#define SUCCESS 0

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;

pthread_mutexattr_t attr;

int pthread_locked_mutex = 0;

typedef struct function_args {
    const char *name;
    int num;
} function_args;

void *print_lines(void *arg) {
    function_args *args = (function_args *)arg;

    pthread_mutex_lock(&mutex2);

    pthread_locked_mutex = 1;

    for (int i = 1; i <= args->num; ++i) {
        pthread_mutex_lock(&mutex1);
        printf("The %s line %d is printed\n", args->name, i);
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_lock(&mutex3);
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_lock(&mutex2);
        pthread_mutex_unlock(&mutex3);
    }

    pthread_mutex_unlock(&mutex2);

    pthread_exit(NULL);
}

int main(void) {
    int status;

    status = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_mutexattr_settype");

        return EXIT_FAILURE;
    }

    pthread_mutex_init(&mutex1, &attr);
    pthread_mutex_init(&mutex2, &attr);
    pthread_mutex_init(&mutex3, &attr);

    pthread_t pthread_id;

    static function_args child = {"child", LINES_NUM};
    static function_args parent = {"parent", LINES_NUM};

    pthread_mutex_lock(&mutex1);
    
    status = pthread_create(&pthread_id, NULL, print_lines, &child);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_create");

        return EXIT_FAILURE;
    }

    while (!pthread_locked_mutex) {
        sched_yield();
    }

    for (int i = 1; i <= parent.num; ++i) {
        printf("The %s line %d is printed\n", parent.name, i);
        pthread_mutex_lock(&mutex3);
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_lock(&mutex2);
        pthread_mutex_unlock(&mutex3);
        pthread_mutex_lock(&mutex1);
        pthread_mutex_unlock(&mutex2);
    }

    pthread_mutex_unlock(&mutex1);

    status = pthread_join(pthread_id, NULL);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_join");

        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

