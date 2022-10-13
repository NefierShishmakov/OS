#ifndef LAB17_NODE_H
#define LAB17_NODE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    char *line;
    struct Node *next;
} Node;

Node *create_node(char *line);
void free_list(Node *head);
void print_list(Node *head);
void push(Node **head, char *line);

#endif