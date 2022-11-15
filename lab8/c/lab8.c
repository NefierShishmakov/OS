#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#define DEFAULT_ARGC_NUM 2
#define PTHREADS_NUM_ARGC_INDEX 1
#define STEPS_NUM_ARGC_INDEX 2

#define MAX_PTHREADS_NUM 256
#define MIN_PTHREADS_NUM 1
#define MIN_STEPS_NUM 1

#define SUCCESS 0
#define SUCCESS_CONVERTION_STATUS 0

#define ERROR -1

typedef struct info {
    int start_point;
    int steps_num;
    double partial_sum;
} info;

void init_pthreads_args(info *pthreads_args, const int pthreads_num, const int steps_num) {
    int pthread_steps_num = steps_num / pthreads_num;
    int remainder = steps_num % pthreads_num;

    for (int i = 0, start_point = 0; i < pthreads_num; start_point += pthread_steps_num, ++i) {
        pthreads_args[i].start_point = start_point;
        pthreads_args[i].steps_num = pthread_steps_num;
        pthreads_args[i].partial_sum = 0.0;
    }

    pthreads_args[pthreads_num - 1].steps_num += remainder;
}

void *get_partial_sum(void *arg) {
    info *pthread_info = (info *)arg;
    
    double number_sign = (pthread_info->start_point % 2 == 0) ? 1 : -1;

    for (int i = pthread_info->start_point; i < pthread_info->start_point + pthread_info->steps_num; ++i) {
        pthread_info->partial_sum += (number_sign * (1.0 / (2 * i + 1)));
        number_sign *= -1;
    }

    pthread_exit(&pthread_info->partial_sum);
}

int args_analyzer(int argc, char **argv, int *result_pthreads_num, int *result_steps_num) {
    if ((argc - 1) != DEFAULT_ARGC_NUM) {
        return ERROR;
    }

    errno = SUCCESS_CONVERTION_STATUS;    
    int pthreads_num = (int)strtol(argv[PTHREADS_NUM_ARGC_INDEX], NULL, 10);

    if (errno != SUCCESS_CONVERTION_STATUS) {
        perror("strtol");
        return ERROR;
    }

    errno = SUCCESS_CONVERTION_STATUS;
    int steps_num = (int)strtol(argv[STEPS_NUM_ARGC_INDEX], NULL, 10);

    if (errno != SUCCESS_CONVERTION_STATUS) {
        perror("strtol");
        return ERROR;
    }
    
    if ((pthreads_num < MIN_PTHREADS_NUM || pthreads_num > MAX_PTHREADS_NUM) 
            || (steps_num < MIN_STEPS_NUM)) {
        return ERROR;
    }
    
    *result_pthreads_num = pthreads_num;
    *result_steps_num = steps_num;

    return SUCCESS;
}

void print_error_message() {
    fprintf(stderr, "Usage: arg1 - number of threads, arg2 - number of steps\n");
}

void join_created_threads(pthread_t *pthreads_ids, const int last_created_thread_idx) {
    for (int i = 0; i <= last_created_thread_idx; ++i) {
        int join_status = pthread_join(pthreads_ids[i], NULL);

        if (join_status != SUCCESS) {
            errno = join_status;
            perror("pthread_join");
        }
    }
}

int calculate_pi(double *pi, const int pthreads_num, const int steps_num) {
    info pthreads_args[pthreads_num];
    init_pthreads_args(pthreads_args, pthreads_num, steps_num);
    
    pthread_t pthreads_ids[pthreads_num];

    for (int i = 0; i < pthreads_num; ++i) {
        int create_status = pthread_create(&pthreads_ids[i], NULL, get_partial_sum, &pthreads_args[i]);

        if (create_status != SUCCESS) {
            errno = create_status;
            perror("pthread_create");
            
            join_created_threads(pthreads_ids, i - 1);

            return ERROR;
        }
    }

    int join_result = SUCCESS;

    for (int i = 0; i < pthreads_num; ++i) {
        void *ret_val;

        int join_status = pthread_join(pthreads_ids[i], &ret_val);

        if (join_status != SUCCESS) {
            errno = join_status;
            perror("pthread_join");

            join_result = ERROR;
        }

        (*pi) += *(double *)ret_val;
    }

    if (join_result != SUCCESS) {
        return ERROR;
    }

    (*pi) *= 4.0;

    return SUCCESS;
}

void print_result(const double *pi) {
    printf("pi=%.18f\n", *pi);
} 

int main(int argc, char **argv) {
    int pthreads_num;
    int steps_num;

    int status = args_analyzer(argc, argv, &pthreads_num, &steps_num);

    if (status != SUCCESS) {
        print_error_message();
        return EXIT_FAILURE;
    }
    
    double pi = 0;
    status = calculate_pi(&pi, pthreads_num, steps_num);

    if (status != SUCCESS) {
        return EXIT_FAILURE;
    }

    print_result(&pi);

    return EXIT_SUCCESS;
}

