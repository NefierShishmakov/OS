#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>

#define LINES_NUM 10
#define SUCCESS 0
#define ERROR (-1)

#define PARENT_ID 1
#define CHILD_ID 0

#define DO_NO_EXECUTE 0

#define NOT_PSHARED_SEM 0

#define FIRST_SEM_START_VALUE 0
#define SECOND_SEM_START_VALUE 1

#define SEM_NUM 2

sem_t semaphores[SEM_NUM];

typedef struct function_args {
    const char *name;
    int iter_num;
    int id;
    pthread_t other_pthread_id;
} function_args;

void handle_error(int status, const char *error_reason) {
    errno = status;
    perror(error_reason);
}

int init_semaphore(int sem_num, int sem_start_value) {
    int status = sem_init(&semaphores[sem_num], NOT_PSHARED_SEM, sem_start_value);

    if (status != SUCCESS) {
        handle_error(status, "sem_init");
        return ERROR;
    }

    return SUCCESS;
}

int destroy_semaphore(int sem_num) {
    int status = sem_destroy(&semaphores[sem_num]);

    if (status != SUCCESS) {
        handle_error(status, "sem_destroy");
        return ERROR;
    }

    return SUCCESS;
}

int destroy_semaphores(int last_index_of_initialised_semaphore) {
    int status;

    for (int i = 0; i <= last_index_of_initialised_semaphore; ++i) {
        status = destroy_semaphore(i);
    }

    return status;
}

int init_semaphores(int *sem_values) {
    int status;
    
    for (int i = 0; i < SEM_NUM; ++i) {
        status = init_semaphore(i, sem_values[i]);

        if (status != SUCCESS) {
            destroy_semaphores(i - 1);
            return ERROR;
        }
    }

    return SUCCESS;
}

void *start_routine(void *arg) {
    function_args *args = (function_args *)arg;
    
    int first = args->id;
    int second = !first;

    for (int i = 1; i <= args->iter_num; ++i) {
        int wait_status = sem_wait(&semaphores[first]);

        if (wait_status != SUCCESS) {
            perror("sem_wait");
            pthread_cancel(args->other_pthread_id);
            return NULL;
        }

        printf("The %s %d line is printed\n", args->name, i);
        int post_status = sem_post(&semaphores[second]);

        if (post_status != SUCCESS) {
            perror("sem_post");
            pthread_cancel(args->other_pthread_id);
            return NULL;
        }
    }

    return NULL;
}

void destroy_semaphores_on_cancel() {
    destroy_semaphores(SEM_NUM - 1);
}

int main(void) {
    pthread_cleanup_push(&destroy_semaphores_on_cancel, NULL);
    pthread_t pthread_id;
    int sem_values[SEM_NUM] = {FIRST_SEM_START_VALUE, SECOND_SEM_START_VALUE};

    int status = init_semaphores(sem_values);

    if (status == ERROR) {
        return EXIT_FAILURE;
    }

    pthread_t main_pthread_id = pthread_self();

    function_args child = {"child", LINES_NUM, CHILD_ID, main_pthread_id};
    function_args parent = {"parent", LINES_NUM, PARENT_ID, pthread_id};

    status = pthread_create(&pthread_id, NULL, start_routine, &child);

    if (status != SUCCESS) {
        handle_error(status, "pthread_create");
        destroy_semaphores(SEM_NUM - 1);
        return EXIT_FAILURE;
    }

    start_routine(&parent);

    status = pthread_join(pthread_id, NULL);

    if (status != SUCCESS) {
        handle_error(status, "pthread_join");
        destroy_semaphores(SEM_NUM - 1);
        return EXIT_FAILURE;
    }
    
    status = destroy_semaphores(SEM_NUM - 1);

    if (status != SUCCESS) {
        return EXIT_FAILURE;
    }
    
    pthread_cleanup_pop(DO_NO_EXECUTE);
    return EXIT_SUCCESS;
}

