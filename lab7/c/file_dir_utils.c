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
                default:
                    handle_dir_error("opendir", dir_path, errno);
                    return ERROR;
            }
        }
    } while ((*dir_stream) == NULL);

    return SUCCESS;
}

int handle_src_and_dest_dirs(paths_t *paths, DIR **new_srcpdir) {
    int status = try_to_mkdir(paths->dest_path, paths->mode);

    if (status == ERROR) {
        return ERROR;
    }

    status = try_to_open_dir_with_retry(new_srcpdir, paths->src_path);
    
    if (status == ERROR) {
        return ERROR;
    }

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

int try_to_write_data_to_file_with_retry(const char *buffer, ssize_t read_bytes, int dest_fd) {    
    ssize_t total_bytes = 0;
    ssize_t remaining_bytes = read_bytes;

    while (total_bytes < read_bytes) {
        ssize_t written_bytes = write(dest_fd, &buffer[total_bytes], remaining_bytes);

        if (written_bytes == ERROR) {
            return ERROR;
        }
            
        total_bytes += written_bytes;
        remaining_bytes -= written_bytes;
    }

    return SUCCESS;
}

bool is_wrong_element(const char *el) {
    return !(strcmp(el, CURRENT_DIR) && strcmp(el, PREVIOUS_DIR));
}
