#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "node.h"

#define SUCCESS 0
#define SEC_TO_SLEEP 5

pthread_mutex_t mutex;


void swap(Node *a, Node *b) {
    char *temp = a->line;
    /* this is not right, think how to copy strings correctly
    a->line = b->line;
    b->line = temp;
    */
}

void bubbleSort(Node *head) {
    int swapped, i;
    struct Node *ptr1;
    struct Node *lptr = NULL;

    if (head == NULL) {
        return;
    }

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->line > ptr1->next->line) {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }

        lptr = ptr1;
    } while (swapped);
}

void *sort_list(void *arg) {
    Node *head = (Node *)arg;

    while (true) {
        pthread_testcancel();
        sleep(SEC_TO_SLEEP);

        pthread_mutex_lock(&mutex);
        bubbleSort(head);
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

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, stdin)) {

    }

    status = pthread_cancel(pthread_id);

    if (status != SUCCESS) {
        errno = status;
        perror("pthread_cancel");


        // Maybe I should return sth else????
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
