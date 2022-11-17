#ifndef LAB17_NODE_H
#define LAB17_NODE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "line_constants.h"
#include "utils.h"

#define SUCCESS 0
#define FAILURE (-1)

typedef struct Node {
    char line[LINE_LENGTH];
    struct Node *next;
} Node;

Node *create_node(char *line, int chars_num_to_copy);
void free_list(Node *head);
void print_list(Node *head);
void push(Node **head, char *line, int chars_num_to_copy);
void bubbleSort(Node **head);

int init_resources(Node **head);
int free_resources(Node **head);

#endif