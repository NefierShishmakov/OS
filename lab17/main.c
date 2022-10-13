#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "node.h"

#define SUCCESS 0
#define SEC_TO_SLEEP 5

pthread_mutex_t mutex;

void *sort_list(void *arg) {
    Node *head = (Node *)arg;

    while (true) {
        sleep(SEC_TO_SLEEP);
        pthread_mutex_lock(&mutex);
        if (head != NULL) {

        }
        pthread_mutex_unlock(&mutex);
    }
}

int main(void) {
    int status;
    pthread_t pthread_id;

    status = pthread_mutex_init(&mutex, NULL);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_mutex_init");

        return EXIT_FAILURE;
    }

    Node *head = NULL;

    status = pthread_create(&pthread_id, NULL, sort_list, head);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_create");

        return EXIT_FAILURE;
    }


    status = pthread_mutex_destroy(&mutex);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_mutex_destroy");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
