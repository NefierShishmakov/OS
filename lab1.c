#include <pthread.h>
#include <errno.h>
#include <stdio.h>

#define LINES_NUM 10

typedef struct function_args {
    const char *str;
    int num;
} function_args;

void *print_lines(void *arg) {
    function_args *args = (function_args*)arg;

    for (int i = 1; i <= args->num; ++i) {
        printf("The %s line %d is printed\n", args->str, i);
    }

    pthread_exit(NULL);
}

int main(void) {
    pthread_t pthread_id;

    function_args child = {"child", LINES_NUM};
    function_args parent = {"parent", LINES_NUM};

    int status = pthread_create(&pthread_id, NULL, print_lines, &child);

    if (status != 0) {
        errno = status;
        perror("pthread_create");

        return 1;
    }

    print_lines(&parent);
    
    return 0;
}
