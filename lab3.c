#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR -1
#define TOTAL_LINES 24
#define PTHREADS_NUM 4
#define LINES_PER_THREAD (TOTAL_LINES / PTHREADS_NUM)

typedef struct lines_t {
    char **strings;
    int pthread_num;
} lines_t;

void init_lines_t(lines_t *arg, char **start_point, const int pthread_num)
{
    arg->strings = start_point;
    arg->pthread_num = pthread_num;
}

void *print_lines(void *arg) {
    lines_t *lines = (lines_t *)arg;

    for (int i = 0; i < LINES_PER_THREAD; ++i) {
        printf("%d thread: %s\n", lines->pthread_num, lines->strings[i]);
    }

    pthread_exit(NULL);
}

static char *strings[TOTAL_LINES] = {
    "HELLO",
    "HI",
    "OHHH NOO",
    "PRINT",
    "LOKIK",
    "REST",
    "APPLE",
    "BANANA",
    "HALAYVA",
    "NI",
    "HU",
    "UUUUU",
    "AAAAA",
    "SSSSS",
    "DDDDD",
    "YYYYY",
    "HHHHH",
    "JJJJJ",
    "KKKKK",
    "PRINTED",
    "OHHHH YES",
    "STATIS",
    "STATICAL",
    "TRAGIC",
};

static lines_t lines[PTHREADS_NUM];

int main(void) {
    pthread_t pthreads_ids[PTHREADS_NUM];

    for (int i = 0; i < PTHREADS_NUM; ++i) {
        init_lines_t(&lines[i], &strings[i * LINES_PER_THREAD], i);

        int create_status = pthread_create(&pthreads_ids[i], NULL, print_lines, &lines[i]);

        if (create_status != SUCCESS) {
            errno = create_status;
            perror("pthread_create");

            return EXIT_FAILURE;
        }
    }

    pthread_exit(NULL);
}