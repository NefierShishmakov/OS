#include "pthread_utils.h"

int try_to_create_thread_with_retry(pthread_t *tid, pthread_attr_t *attr, 
        void *(*start_routine)(void *), void *arg) {
    int status;

    do {
        status = pthread_create(tid, attr, start_routine, arg);
        
        if (status != SUCCESS) {
            errno = status;
            switch (errno) {
                case EAGAIN:
                    sleep(WAIT_SEC_FOR_RESOURCES);
                    break;
                default:
                    handle_pthread_error("pthread_create", errno);
                    return ERROR;
            }
        }
    } while (status != SUCCESS);

    return SUCCESS;
}
