#ifndef LAB17_NODE_H
#define LAB17_NODE_H

#define LINE_LENGTH 501

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node {
    char line[LINE_LENGTH];
    struct Node *next;
    int nodes_num;
} Node;

Node *create_node(char *line, int nodes_num);
void free_list(Node *head);
void print_list(Node *head);
void push(Node **head, char *line, int nodes_num);

#endif