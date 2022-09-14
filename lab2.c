#include <pthread.h>
#include <errno.h>
#include <stdio.h>

void *print_ten_lines() {
    for (int i = 1; i <= 10; ++i) {
        printf("The line %d is printed\n", i);
    }

    pthread_exit(NULL);
}

int main(void) {
    pthread_t pthread_id;

    int create_status = pthread_create(&pthread_id, NULL, print_ten_lines, NULL);

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
    print_ten_lines();
    
    return 0;
}