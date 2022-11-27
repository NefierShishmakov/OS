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
    static function_args child = {"child", LINES_NUM};
    static function_args parent = {"parent", LINES_NUM};
    
    int create_status = pthread_create(&pthread_id, NULL, print_lines, &child);

    if (create_status != SUCCESS) {
        errno = create_status;
        perror("pthread_create");

        return FAILURE;
    }

    int detach_status = pthread_detach(pthread_id);

    if (detach_status != SUCCESS) {
        errno = detach_status;
        perror("pthread_detach");
    }

    print_lines(&parent);
   
    pthread_exit(NULL);
}

