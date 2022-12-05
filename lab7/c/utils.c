#include "utils.h"

int try_to_mkdir(const char *dir) {
    int status = mkdir(dir, 0700);

    if (status != SUCCESS) {
        if (errno == EEXIST) {
            status = access(dir, W_OK | X_OK);

            if (status != SUCCESS) {
                fprintf(stderr, "%s is exists but not accessible\n", dir);
                return ERROR;
            }
        }
        else {
            perror("mkdir");
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
                default:
                    perror("opendir");
                    return ERROR;
            }
        }
    } while ((*dir_stream) == NULL);

    return SUCCESS;
}

int try_to_open_file(const char *pathname, int flags, mode_t mode) {
    int fd;

    do {
        fd = open(pathname, flags, mode);
        
        if (fd == ERROR) {
            switch (errno) {
                case EMFILE:
                    sleep(WAIT_SEC_FOR_FD);
                    break;
                default:
                    perror("open");
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

size_t get_length_of_new_path(const char *first_path, const char *second_path, 
        bool is_separator_needed) {
    size_t result_len = strlen(first_path) + strlen(second_path) + 1;
    return (is_separator_needed ? (result_len + 1) : result_len);
}

