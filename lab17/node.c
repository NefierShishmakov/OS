#include "node.h"

Node *create_node(char *line, int nodes_num) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    strcpy(new_node->line, line);
    new_node->nodes_num = nodes_num;
    new_node->next = NULL;

    return new_node;
}

void print_list(Node *head) {
    while (head != NULL) {
        printf("%s", head->line);
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

void push(Node **head, char *line, int nodes_num) {
    Node *new_node = create_node(line, nodes_num);

    if ((*head) == NULL) {
        (*head) = new_node;
    } else {
        Node *prev_head = (*head);
        new_node->next = prev_head;
        (*head) = new_node;
    }
}