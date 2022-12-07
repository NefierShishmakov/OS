#include "file_dir_utils.h"

int try_to_mkdir(const char *dir_path, mode_t mode) {
    int status = mkdir(dir_path, mode);
    
    if (status != SUCCESS) {
        if (errno == EEXIST) {
            status = access(dir_path, W_OK | X_OK);

            if (status != SUCCESS) {
                fprintf(stderr, "Directory %s exists but not accessible\n", dir_path);
                return ERROR;
            }
            chmod(dir_path, mode);
        }
        else {
            handle_dir_error("mkdir", dir_path, errno);
            return ERROR;
        }
    }

    return SUCCESS;
}

int try_to_open_dir_with_retry(DIR **dir_stream, const char *dir_path) {   
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

int try_to_open_file_with_retry(const char *file_path, int flags, mode_t mode) {
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

    if (mode != NO_MODE) {
        chmod(file_path, mode);
    }

    return fd;
}

bool is_wrong_element(const char *el) {
    return !(strcmp(el, CURRENT_DIR) && strcmp(el, PREVIOUS_DIR));
}
