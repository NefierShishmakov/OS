#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "node.h"

#define SUCCESS 0
#define SEC_TO_SLEEP 5

#define CONTINUE 1
#define END_OF_DATA 2

pthread_mutex_t mutex;

Node *head = NULL;

void swap(Node *a, Node *b) {
    char temp[LINE_LENGTH] = {0};
    strcpy(temp, a->line);

    memset(a->line, 0, LINE_LENGTH);
    strcpy(a->line, b->line);
    memset(b->line, 0, LINE_LENGTH);
    strcpy(b->line, temp);
}

void handle_error(int status, char *error_reason) {
    errno = status;
    perror(error_reason);
}

void mutex_destroy_handler() {
    int status = pthread_mutex_destroy(&mutex);

    if (status != SUCCESS) {
        handle_error(status, "pthread_mutex_destroy");
    }
}

bool isBigger(char *first, char *second) {
    for (int i = 0; i < first[i] != '\n' && second[i] != '\n'; ++i) {
        if ((first[i] > second[i]) || (first[i] == second[i] && strlen(first) > strlen(second))) {
            return true;
        }
    }

    return false;
}

void bubbleSort() {
    if (head == NULL) {
        return;
    }

    int nodes_num = head->nodes_num;

    for (int i = 0; i < nodes_num - 1; ++i) {
        Node *first = head;
        Node *second = head->next;

        for (int j = 0; j < nodes_num - i - 1; ++j) {
            if (isBigger(first->line, second->line)) {
                swap(first, second);
            }

            first = second;
            second = second->next;
        }
    }
}

void *sort_list(void *arg) {
    while (true) {
        pthread_testcancel();
        sleep(SEC_TO_SLEEP);

        pthread_mutex_lock(&mutex);
        bubbleSort();
        pthread_mutex_unlock(&mutex);
    }
}

int handle_new_line(char *line, size_t len, int *nodes_num) {
    int handle_status = CONTINUE;

    if (len == 1 && line[0] == '\n') {
        print_list(head);
    } else if (len == 2 && line[0] == 'q') {
        handle_status = END_OF_DATA;
    } else {
        push(&head, line, ++(*nodes_num));
    }

    return handle_status;
}

int main(void) {
    int status;
    pthread_t pthread_id;

    status = pthread_mutex_init(&mutex, NULL);

    if (status != SUCCESS) {
        handle_error(status, "pthread_mutex_init");
        return EXIT_FAILURE;
    }

    status = pthread_create(&pthread_id, NULL, sort_list, NULL);

    if (status != SUCCESS) {
        handle_error(status, "pthread_create");
        mutex_destroy_handler();
        return EXIT_FAILURE;
    }

    char line[LINE_LENGTH] = {0};

    int nodes_num = 0;

    while (fgets(line, LINE_LENGTH, stdin)) {
        int handle_status;

        pthread_mutex_lock(&mutex);
        handle_status = handle_new_line(line, strlen(line), &nodes_num);

        memset(line, 0, LINE_LENGTH);
        pthread_mutex_unlock(&mutex);

        if (handle_status == END_OF_DATA) {
            break;
        }
    }

    status = pthread_cancel(pthread_id);

    if (status != SUCCESS) {
        handle_error(status, "pthread_cancel");
        mutex_destroy_handler();
        free_list(head);
        return EXIT_FAILURE;
    }

    void *retval;

    status = pthread_join(pthread_id, &retval);

    if (status != SUCCESS) {
        handle_error(status, "pthread_join");
        mutex_destroy_handler();
        free_list(head);
        return EXIT_FAILURE;
    }

    if (retval != PTHREAD_CANCELED) {
        fprintf(stderr, "Error: thread was not cancelled");
        mutex_destroy_handler();
        free_list(head);
        return EXIT_FAILURE;
    }

    status = pthread_mutex_destroy(&mutex);

    if (status != SUCCESS) {
        handle_error(status, "pthread_mutex_destroy");
        free_list(head);
        return EXIT_FAILURE;
    }

    free_list(head);
    return EXIT_SUCCESS;
}
