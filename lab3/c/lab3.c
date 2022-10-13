#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR -1
#define TOTAL_LINES_NUM 24
#define PTHREADS_NUM 4

typedef struct lines_t {
    char **strings;
    int pthread_num;
    int strings_num;
} lines_t;

void init_lines_t(lines_t *arg, char **start_point, const int pthread_num, const int strings_num) {
    arg->strings = start_point;
    arg->pthread_num = pthread_num;
    arg->strings_num = strings_num;
}

void *print_lines(void *arg) {
    lines_t *lines = (lines_t *)arg;

    for (int i = 0; i < lines->strings_num; ++i) {
        printf("Messsage from pthread with number - %d: %s\n", lines->pthread_num, lines->strings[i]);
    }

    pthread_exit(NULL);
}

int pthreads_join(pthread_t *pthreads_ids) {
    int status = SUCCESS;

    for (int i = 0; i < PTHREADS_NUM; ++i) {
        int join_status = pthread_join(pthreads_ids[i], NULL);

        if (join_status != SUCCESS) {
            errno = join_status;
            perror("pthread_join");
            status = ERROR;
        }
    }

    return status;
}

void init_lines_distribution(int *lines_distribution) {
    int lines_per_pthread = TOTAL_LINES_NUM / PTHREADS_NUM;
    int remainder = TOTAL_LINES_NUM % PTHREADS_NUM;
    
    for (int i = 0; i < PTHREADS_NUM; ++i) {
        lines_distribution[i] = lines_per_pthread;
    }

    lines_distribution[PTHREADS_NUM - 1] += remainder;
} 

int main(void) {
    pthread_t pthreads_ids[PTHREADS_NUM];
    
    char *lines[TOTAL_LINES_NUM] = {
        "HELLO",
        "LALALAL",
        "PRIVET",
        "RUSSIA",
        "TODAY",
        "APPLE",
        "BANANA",
        "RESERVED",
        "SUCCESS",
        "ADMIRAL",
        "ORANGE",
        "RPH",
        "NOBODY",
        "WEEk",
        "DAY",
        "CIRCLE",
        "FUTURE",
        "PPPPPP",
        "RRRRR",
        "MAN",
        "WOMAN",
        "RESOURCE",
        "TRYING",
        "POP"
    };

    int lines_distribution[PTHREADS_NUM] = {0};

    init_lines_distribution(lines_distribution);

    lines_t pthreads_lines[PTHREADS_NUM];

    for (int i = 0, idx = 0; i < PTHREADS_NUM; idx += lines_distribution[i], ++i) {
        init_lines_t(&pthreads_lines[i], &lines[idx], i + 1, lines_distribution[i]);

        int create_status = pthread_create(&pthreads_ids[i], NULL, print_lines, &pthreads_lines[i]);

        if (create_status != SUCCESS) {
            errno = create_status;
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    } 
    
    int pthreads_join_status = pthreads_join(pthreads_ids);

    if (pthreads_join_status == ERROR) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

