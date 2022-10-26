#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINES_NUM 100
#define MAX_LINE_LENGTH 501

#define SORT_COEFFICIENT 20000

#define ERROR (-1)
#define SUCCESS 0

void handle_error(int status, char *error_reason) {
    errno = status;
    perror(error_reason);
}

void *routine(void *arg) {
    int line_length = strlen((char *)arg);

    int usleep_status = usleep(line_length * SORT_COEFFICIENT);

    if (usleep_status != SUCCESS) {
        handle_error(usleep_status, "usleep");
        return NULL;
    }

    printf("%s", (char *)arg);

    pthread_exit(NULL);
}

int read_lines(char lines[MAX_LINES_NUM][MAX_LINE_LENGTH], int *result_lines_num) {
    *result_lines_num = 0;

    while ((*result_lines_num) <= MAX_LINES_NUM) {
        printf("Enter the new line or press \'Enter\' to end input: ");
        
        char *fgets_status = fgets(&lines[(*result_lines_num)][0], MAX_LINE_LENGTH, stdin);

        if (fgets_status == NULL) {
            return ERROR;
        }

        if (strlen(lines[(*result_lines_num)]) == 1 && lines[(*result_lines_num)][0] == '\n') {
            if ((*result_lines_num) == 0) {
                printf("You need to enter at least one line\n");
                continue;
            }
            
            break;
        }

        ++(*result_lines_num);
    }
    
    return SUCCESS;
}

int join_created_pthreads(pthread_t *pthreads_ids, int last_created_pthread_idx) {
    int result_status = SUCCESS;

    for (int i = 0; i <= last_created_pthread_idx; ++i) {
        int join_status = pthread_join(pthreads_ids[i], NULL);

        if (join_status != SUCCESS) {
            handle_error(join_status, "pthread_join");
            result_status = ERROR;
        }
    }

    return result_status;
}

int create_pthreads(pthread_t *pthreads_ids, char lines[MAX_LINES_NUM][MAX_LINE_LENGTH], int result_lines_num) {
    for (int i = 0; i < result_lines_num; ++i) {
        int create_status = pthread_create(&pthreads_ids[i], NULL, routine, lines[i]);

        if (create_status != SUCCESS) {
            handle_error(create_status, "pthread_create");
            join_created_pthreads(pthreads_ids, i - 1);

            return ERROR;
        }
    }

    return SUCCESS;
}

int main(void) {
    char lines[MAX_LINES_NUM][MAX_LINE_LENGTH];
    int result_lines_num;

    int status = read_lines(lines, &result_lines_num);

    if (status == ERROR) {
        return EXIT_FAILURE;
    }
    
    pthread_t pthreads_ids[result_lines_num];

    status = create_pthreads(pthreads_ids, lines, result_lines_num);

    if (status == ERROR) {
        return EXIT_FAILURE;
    }

    status = join_created_pthreads(pthreads_ids, result_lines_num - 1);

    if (status == ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

