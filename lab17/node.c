#include "node.h"

Node *create_node(char *line, int chars_num_to_copy) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    strncpy(new_node->line, line, chars_num_to_copy);
    new_node->next = NULL;

    return new_node;
}

void print_list(Node *head) {
    while (head != NULL) {
        printf("%s\n", head->line);
        head = head->next;
    }
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *to_del = head;
        head = head->next;

        free(to_del);
    }
}

void push(Node **head, char *line, int chars_num_to_copy) {
    Node *new_node = create_node(line, chars_num_to_copy);

    if ((*head) == NULL) {
        (*head) = new_node;
    } else {
        Node *prev_head = (*head);
        new_node->next = prev_head;
        (*head) = new_node;
    }
}