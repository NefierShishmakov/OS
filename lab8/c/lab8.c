#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#define STEPS_NUM 698377680
#define DEFAULT_ARGC_NUM 2
#define PTHREADS_NUM_ARGC_INDEX 1
#define MAX_PTHREADS_NUM 256

#define SUCCESS 0
#define SUCCESS_CONVERTION_STATUS 0

#define PTHREADS_NUM_ERROR -3
#define CONVERTION_ERROR -2
#define ARGS_NUMBER_ERROR -1

typedef struct info {
    int start_point;
    int steps_num;
    double partial_sum;
} info;

void init_pthreads_args(info *pthreads_args, const int pthreads_num) {
    int pthread_steps_num = STEPS_NUM / pthreads_num;
    int remainder = STEPS_NUM % pthreads_num;

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

int args_analyzer(int argc, char **argv, int *result_pthreads_num) {
    if (argc > DEFAULT_ARGC_NUM) {
        return ARGS_NUMBER_ERROR;
    }

    errno = SUCCESS_CONVERTION_STATUS;    
    int pthreads_num = (int)strtol(argv[PTHREADS_NUM_ARGC_INDEX], NULL, 10);

    if (errno != SUCCESS_CONVERTION_STATUS) {
        perror("strtol");
        return CONVERTION_ERROR;
    }
    
    if (pthreads_num < 0 || pthreads_num > MAX_PTHREADS_NUM) {
        return PTHREADS_NUM_ERROR;
    }
    
    *result_pthreads_num = pthreads_num;
    return SUCCESS;
}

void print_error_message(int error_num) {
    switch (error_num) {
        case ARGS_NUMBER_ERROR:
            fprintf(stderr, "Too many arguments!\n");
            break;
        case CONVERTION_ERROR:
            fprintf(stderr, "The second argument (threads number) must be integer!\n");
            break;
        case PTHREADS_NUM_ERROR:
            fprintf(stderr, "Threads number must be bigger than zero and less or equal than %d!\n", MAX_PTHREADS_NUM);
            break;
    }
}

void calculate_pi(double *pi, const int pthreads_num) {
    info pthreads_args[pthreads_num];
    init_pthreads_args(pthreads_args, pthreads_num);
    
    pthread_t pthreads_ids[pthreads_num];

    for (int i = 0; i < pthreads_num; ++i) {
        int create_status = pthread_create(&pthreads_ids[i], NULL, get_partial_sum, &pthreads_args[i]);

        if (create_status != SUCCESS) {
            errno = create_status;
            perror("pthread_create");

            return;
        }
    }

    for (int i = 0; i < pthreads_num; ++i) {
        void *ret_val;

        int join_status = pthread_join(pthreads_ids[i], &ret_val);

        if (join_status != SUCCESS) {
            errno = join_status;
            perror("pthread_join");

            return;
        }

        (*pi) += *(double *)ret_val;
    }

    (*pi) *= 4.0;
}

void print_result(const double *pi) {
    printf("pi=%.9f\n", *pi);
} 

int main(int argc, char **argv) {
    int pthreads_num;

    int args_analyzer_status = args_analyzer(argc, argv, &pthreads_num);

    if (args_analyzer_status != SUCCESS) {
        print_error_message(args_analyzer_status);
        return EXIT_FAILURE;
    }
    
    double pi = 0;
    calculate_pi(&pi, pthreads_num);
    print_result(&pi);

    return EXIT_SUCCESS;
}

