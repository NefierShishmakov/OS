#include "node.h"

Node *create_node(pthread_t tid, paths_t *paths) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->tid = tid;
    new_node->paths = paths;
    new_node->next = NULL;

    return new_node;
}

void push(Node **head, pthread_t tid, paths_t *paths) {
    Node *new_node = create_node(tid, paths);

    if ((*head) == NULL) {
        (*head) = new_node;
    }
    else {
        Node *prev_head = (*head);
        new_node->next = prev_head;
        (*head) = new_node;
    }
}

void free_resources(Node *head) {
    while (head != NULL) {
        Node *to_del = head;
        head = head->next;
        
        int status = pthread_join(to_del->tid, NULL);

        if (status != SUCCESS) {
            handle_pthread_error("pthread_join", status);
        }

        free_paths_t(to_del->paths);
        free(to_del);
    }
}
