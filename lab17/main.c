#include <unistd.h>
#include "node.h"
#include "utils.h"

#define SEC_TO_SLEEP 5

#define CONTINUE 1
#define END_OF_DATA 2

#define END_INPUT_SYM 'q'

enum operations {
    PRINT_LIST = 1,
    PUSH_LINES = 2,
    END_INPUT = 3
};

void *sort_list(void *arg) {
    Node **head = (Node **)arg;

    while (true) {
        pthread_testcancel();
        sleep(SEC_TO_SLEEP);
        bubbleSort(head);
    }
}

int get_operation(char *line, size_t len) {
    if (line[0] == '\n') {
        return PRINT_LIST;
    } else if (len == 2 && line[0] == END_INPUT_SYM) {
        return END_INPUT;
    }

    return PUSH_LINES;
}

void push_lines(Node **head, char *line) {
    line[strcspn(line, "\r\n")] = '\0';

    int line_length = get_result_line_length(strlen(line));
    size_t start = 0;

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
    printf("Enter a new line or enter \'q\' to finish entering data or press \'enter\' "
               "to print all lines:\n");

    while (read_status != END_OF_DATA) {
        if (fgets(line, MAX_ENTERED_LINE_LENGTH, stdin) == NULL) {
            break;
        }

        read_status = handle_new_line(head, line, strlen(line));

        memset(line, 0, MAX_ENTERED_LINE_LENGTH);
    }
}

int try_to_cancel_pthread(pthread_t pthread_id) {
    int status = pthread_cancel(pthread_id);

    if (status != SUCCESS) {
        handle_error(status, "pthread_cancel");
        return FAILURE;
    }

    void *retval;

    status = pthread_join(pthread_id, &retval);

    if (status != SUCCESS) {
        handle_error(status, "pthread_join");
        return FAILURE;
    }

    if (retval != PTHREAD_CANCELED) {
        fprintf(stderr, "Error: thread was not cancelled");
        return FAILURE;
    }

    return SUCCESS;
}

int main(void) {
    int status;
    pthread_t pthread_id;
    Node *head;

    status = init_resources(&head);

    if (status != SUCCESS) {
        return EXIT_FAILURE;
    }

    status = pthread_create(&pthread_id, NULL, sort_list, &head);

    if (status != SUCCESS) {
        handle_error(status, "pthread_create");
        free_resources(&head);
        return EXIT_FAILURE;
    }

    read_lines(&head);

    status = try_to_cancel_pthread(pthread_id);

    if (status != SUCCESS) {
        free_resources(&head);
        return EXIT_FAILURE;
    }

    status = free_resources(&head);

    if (status != SUCCESS) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

