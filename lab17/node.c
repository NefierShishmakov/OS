#include "node.h"

pthread_mutex_t mutex;

Node *create_node(char *line, int chars_num_to_copy) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    strncpy(new_node->line, line, chars_num_to_copy);
    new_node->next = NULL;

    return new_node;
}

void print_list(Node *head) {
    pthread_mutex_lock(&mutex);
    while (head != NULL) {
        printf("%s\n", head->line);
        head = head->next;
    }
    pthread_mutex_unlock(&mutex);
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *to_del = head;
        head = head->next;

        free(to_del);
    }
}

void push(Node **head, char *line, int chars_num_to_copy) {
    pthread_mutex_lock(&mutex);
    Node *new_node = create_node(line, chars_num_to_copy);

    if ((*head) == NULL) {
        (*head) = new_node;
    } else {
        Node *prev_head = (*head);
        new_node->next = prev_head;
        (*head) = new_node;
    }
    pthread_mutex_unlock(&mutex);
}

void bubbleSort(Node **head) {
    pthread_mutex_lock(&mutex);
    if ((*head) != NULL) {
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
    pthread_mutex_unlock(&mutex);
}

int init_resources(Node **head) {
    *head = NULL;

    int status = pthread_mutex_init(&mutex, NULL);

    if (status != SUCCESS) {
        handle_error(status, "pthread_mutex_init");
        return FAILURE;
    }

    return SUCCESS;
}

int free_resources(Node **head) {
    free_list(*head);

    int status = pthread_mutex_destroy(&mutex);

    if (status != SUCCESS) {
        handle_error(status, "pthread_mutex_destroy");
        return FAILURE;
    }

    return SUCCESS;
}