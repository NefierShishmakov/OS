#ifndef LAB17_NODE_H
#define LAB17_NODE_H

#define LINE_LENGTH 81
#define SLICED_LINE_LENGTH 41

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node {
    char line[LINE_LENGTH];
    struct Node *next;
} Node;

Node *create_node(char *line, int chars_num_to_copy);
void free_list(Node *head);
void print_list(Node *head);
void push(Node **head, char *line, int chars_num_to_copy);

#endif