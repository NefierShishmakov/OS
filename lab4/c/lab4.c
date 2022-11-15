#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define SUCCESS 0

#define SEC_TO_SLEEP 10
#define TRUE 1

typedef unsigned long long ull;

void *print_lines() {
    ull i = 1;

    while (TRUE) {
        pthread_testcancel();
        printf("Hello, I\'m your %lld friend\n", i++);
    }
}

int main(void) {
    pthread_t pthread_id;
    
    int status = pthread_create(&pthread_id, NULL, print_lines, NULL);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_create");
        return EXIT_FAILURE;
    }
    
    sleep(SEC_TO_SLEEP);
    
    status = pthread_cancel(pthread_id);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_cancel");
        return EXIT_FAILURE;
    }

    void *res;

    status = pthread_join(pthread_id, &res);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    if (res != PTHREAD_CANCELED) {
        fprintf(stderr, "The child pthread was not cancelled\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

