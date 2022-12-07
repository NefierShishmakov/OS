#include "utils.h"
#include "constants.h"

int try_to_mkdir(const char *dir_path, mode_t mode) {
    int status = mkdir(dir_path, mode);
    
    if (status != SUCCESS) {
        if (errno == EEXIST) {
            status = access(dir_path, W_OK | X_OK);

            if (status != SUCCESS) {
                fprintf(stderr, "Directory %s exists but not accessible\n", dir_path);
                return ERROR;
            }
        }
        else {
            handle_dir_error("mkdir", dir_path, errno);
            return ERROR;
        }
    }

    return SUCCESS;
}

int try_to_open_dir(DIR **dir_stream, const char *dir_path) {   
    do {
        (*dir_stream) = opendir(dir_path);
        if ((*dir_stream) == NULL) {
            switch (errno) {
                case EMFILE:
                    sleep(WAIT_SEC_FOR_FD);
                    break;
                case ENOMEM:
                    sleep(WAIT_SEC_FOR_RESOURCES);
                    break;
                default:
                    handle_dir_error("opendir", dir_path, errno);
                    return ERROR;
            }
        }
    } while ((*dir_stream) == NULL);

    return SUCCESS;
}

int try_to_open_file(const char *file_path, int flags, mode_t mode) {
    int fd;
    
    do {
        fd = open(file_path, flags, mode);
        
        if (fd == ERROR) {
            switch (errno) {
                case EMFILE:
                    sleep(WAIT_SEC_FOR_FD);
                    break;
                default:
                    handle_file_error("open", file_path, errno);
                    return ERROR;
            }
        }
    } while (fd == ERROR);

    return fd;
}

void prepare_paths(char *first_path, char *second_path) {
    size_t first_path_len = strlen(first_path);
    size_t second_path_len = strlen(second_path);

    if (first_path[first_path_len - 1] == '/') {
        first_path[first_path_len - 1] = '\0';
    }

    if (second_path[second_path_len - 1] == '/') {
        second_path[second_path_len - 1] = '\0';
    }
}

size_t get_length_of_new_path(const char *first_path, const char *second_path) {
    return (strlen(first_path) + strlen(second_path) + 2);
}

void handle_file_error(const char *error_cause, const char *file_path, int errnum) {
    char error_buffer[ERROR_BUFSIZE];
    strerror_r(errnum, error_buffer, ERROR_BUFSIZE);
    fprintf(stderr, "%s: \'%s\': %s\n", error_cause, file_path, error_buffer);
}

void handle_dir_error(const char *error_cause, const char *dir_path, int errnum) {
    char error_buffer[ERROR_BUFSIZE];
    strerror_r(errnum, error_buffer, ERROR_BUFSIZE);
    fprintf(stderr, "%s: \'%s\': %s\n", error_cause, dir_path, error_buffer);
}

void handle_pthread_error(const char *error_cause, int errnum) {
    char error_buffer[ERROR_BUFSIZE];
    strerror_r(errnum, error_buffer, ERROR_BUFSIZE);
    fprintf(stderr, "%s: %s\n", error_cause, error_buffer);
}

bool is_wrong_element(const char *el) {
    return !(strcmp(el, CURRENT_DIR) && strcmp(el, PREVIOUS_DIR));
}
