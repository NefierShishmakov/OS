#include <pthread.h>
#include <errno.h>
#include <stdio.h>

#define LINES_NUM 10
#define SUCCESS 0
#define FAILURE 1

typedef struct function_args {
    const char *name;
    int num;
} function_args;

void *print_lines(void *arg) {
    function_args *args = (function_args *)arg;

    for (int i = 1; i <= args->num; ++i) {
        printf("The %s line %d is printed\n", args->name, i);
    }

    return NULL;
}

int main(void) {
    pthread_t pthread_id;

    function_args parent = {"parent", LINES_NUM};
    function_args child = {"child", LINES_NUM};

    int create_status = pthread_create(&pthread_id, NULL, print_lines, &child);

    if (create_status != SUCCESS) {
        errno = create_status;
        perror("pthread_create");

        return FAILURE;
    }

    int join_status = pthread_join(pthread_id, NULL);

    if (join_status != SUCCESS) {
        errno = join_status;
        perror("pthread_join");

        return FAILURE;
    }

    print_lines(&parent);
    
    pthread_exit(NULL);
}
