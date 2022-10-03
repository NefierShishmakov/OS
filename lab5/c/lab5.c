#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define SUCCESS 0

#define SEC_TO_SLEEP 2
#define TRUE 1

#define EXECUTE 1

typedef unsigned long long ull;

void print_goodbye() {
    printf("Message from the clean-up handler\n");
}

void *print_lines() {
    ull i = 1;

    pthread_cleanup_push(print_goodbye, NULL);

    while (TRUE) {
        printf("Hello, I\'m your %lld friend\n", i);
        ++i;
        pthread_testcancel();
    }

    pthread_cleanup_pop(EXECUTE);
}

int main(void) {
    pthread_t pthread_id;

    int create_status = pthread_create(&pthread_id, NULL, print_lines, NULL);

    if (create_status != SUCCESS) {
        errno = create_status;
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    sleep(SEC_TO_SLEEP);

    int cancel_status = pthread_cancel(pthread_id);

    if (cancel_status != SUCCESS) {
        errno = cancel_status;
        perror("pthread_cancel");
        return EXIT_FAILURE;
    }

    void *res;

    int join_status = pthread_join(pthread_id, &res);

    if (join_status != SUCCESS) {
        errno = join_status;
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    if (res != PTHREAD_CANCELED) {
        fprintf(stderr, "The child pthread was not cancelled");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

