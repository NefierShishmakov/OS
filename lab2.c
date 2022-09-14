#include <pthread.h>
#include <errno.h>
#include <stdio.h>

typedef struct function_args {
    const char *name;
    int num;
} function_args;

void *print_lines(void *arg) {
    function_args *args = (function_args*)arg;

    for (int i = 1; i <= args->num; ++i) {
        printf("The %s line %d is printed\n", args->name, i);
    }

    pthread_exit(NULL);
}

int main(void) {
    pthread_t pthread_id;

    function_args parent = {"parent", 10};
    function_args child = {"child", 10};

    int create_status = pthread_create(&pthread_id, NULL, print_lines, &child);

    if (create_status != 0) {
        errno = create_status;
        perror("pthread_create");

        return 1;
    }

    int join_status = pthread_join(pthread_id, NULL);

    if (join_status != 0) {
        errno = join_status;
        perror("pthread_join");

        return 1;
    }

    printf("The main process is started\n");
    print_lines(&parent);
    
    return 0;
}
