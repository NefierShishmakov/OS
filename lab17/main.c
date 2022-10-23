#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "node.h"

#define SUCCESS 0
#define SEC_TO_SLEEP 5

#define MAX_ENTERED_LINE_LENGTH 503

#define CONTINUE 1
#define END_OF_DATA 2

pthread_mutex_t mutex;

enum operations {
    PRINT_LIST,
    PUSH_LINES,
    END_INPUT
};

int get_result_line_length(size_t entered_line_length) {
    return (entered_line_length > (LINE_LENGTH - 1) ? (SLICED_LINE_LENGTH - 1) : (LINE_LENGTH - 1));
}

void swap(char *first_line, char *second_line) {
    char temp[LINE_LENGTH] = {0};
    strcpy(temp, first_line);

    memset(first_line, 0, LINE_LENGTH);
    strcpy(first_line, second_line);
    memset(second_line, 0, LINE_LENGTH);
    strcpy(second_line, temp);
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

void bubbleSort(Node **head) {
    if ((*head) == NULL) {
        return;
    }

    Node *left_ptr;
    Node *right_ptr = NULL;

    bool is_swapped = true;

    do {
        is_swapped = false;
        left_ptr = *head;

        while (left_ptr->next != right_ptr) {
            if (strcmp(left_ptr->line, left_ptr->next->line) > 0) {
                swap(left_ptr->line, left_ptr->next->line);
                is_swapped = true;
            }

            left_ptr = left_ptr->next;
        }

        right_ptr = left_ptr;
    } while (is_swapped);
}

void *sort_list(void *arg) {
    Node **head = (Node **)arg;

    while (true) {
        pthread_testcancel();
        sleep(SEC_TO_SLEEP);

        pthread_mutex_lock(&mutex);
        bubbleSort(head);
        pthread_mutex_unlock(&mutex);
    }
}

int get_operation(char *line, size_t len) {
    if (line[0] == '\n') {
        return PRINT_LIST;
    } else if (len == 2 && line[0] == 'q') {
        return END_INPUT;
    }

    return PUSH_LINES;
}

void push_lines(Node **head, char *line) {
    line[strcspn(line, "\r\n")] = '\0';

    int line_length = get_result_line_length(strlen(line));
    int start = 0;

    while (start < strlen(line)) {
        push(head, &line[start], line_length);
        start += line_length;
    }
}

int handle_new_line(Node **head, char *line, size_t len) {
    int operation = get_operation(line, len);

    switch (operation) {
        case PRINT_LIST:
            print_list(*head);
            break;
        case PUSH_LINES:
            push_lines(head, line);
            break;
        case END_INPUT:
           return END_OF_DATA;
    }

    return CONTINUE;
}

void read_lines(Node **head) {
    char line[MAX_ENTERED_LINE_LENGTH] = {0};

    int read_status = CONTINUE;

    while (read_status != END_OF_DATA) {
        printf("Enter a new line or enter \'q\' to finish entering data or press \'enter\' "
               "to print all lines:  ");

        fgets(line, MAX_ENTERED_LINE_LENGTH, stdin);

        pthread_mutex_lock(&mutex);
        read_status = handle_new_line(head, line, strlen(line));

        memset(line, 0, MAX_ENTERED_LINE_LENGTH);
        pthread_mutex_unlock(&mutex);
    }
}

int main(void) {
    int status;
    pthread_t pthread_id;
    Node *head = NULL;

    status = pthread_mutex_init(&mutex, NULL);

    if (status != SUCCESS) {
        handle_error(status, "pthread_mutex_init");
        return EXIT_FAILURE;
    }

    status = pthread_create(&pthread_id, NULL, sort_list, &head);

    if (status != SUCCESS) {
        handle_error(status, "pthread_create");
        mutex_destroy_handler();
        return EXIT_FAILURE;
    }

    read_lines(&head);

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
