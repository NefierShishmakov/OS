#include <pthread.h>
#include <errno.h>
#include <stdio.h>

void *print_ten_lines() {
    for (int i = 0; i < 10; ++i) {
        printf("The line %d is printed\n", i);
    }

    pthread_exit(NULL);
}

int main(void) {
    pthread_t pthread_id;

    int status = pthread_create(&pthread_id, NULL, print_ten_lines, NULL);

    if (status != 0) {
        errno = status;
        perror("pthread_create");

        return 1;
    }

    print_ten_lines();
    
    return 0;
}