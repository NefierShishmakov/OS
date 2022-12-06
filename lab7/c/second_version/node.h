#ifndef NODE_H
#define NODE_H

#include <pthread.h>
#include <stdlib.h>
#include "paths.h"
#include "utils.h"

typedef struct Node {
    pthread_t tid;
    paths_t *paths;
    struct Node *next;
} Node;

Node *create_node(pthread_t tid, paths_t *paths);
void free_resources(Node *head);
void push(Node **head, pthread_t tid, paths_t *paths);

#endif
