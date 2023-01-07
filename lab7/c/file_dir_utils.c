#include "file_dir_utils.h"

int try_to_create_dir(const char* dir_path, mode_t mode) {
    int status = mkdir(dir_path, mode);
    
    if (status == SUCCESS) {
        return SUCCESS;
    }

    if (errno != EEXIST) {
        handle_file_or_dir_error("mkdir", dir_path, errno);
        return ERROR;
    }
        
    status = access(dir_path, W_OK | X_OK);

    if (status != SUCCESS) {
        fprintf(stderr, "Directory %s exists but not accessible\n", dir_path);
        return ERROR;
    }

    chmod(dir_path, mode);

    return SUCCESS;
}

int try_to_open_dir_with_retry(DIR** dir_stream, const char* dir_path) {   
    do {
        (*dir_stream) = opendir(dir_path);
        if ((*dir_stream) == NULL) {
            switch (errno) {
                case EMFILE:
                    sleep(WAIT_SEC_FOR_FD);
                    break;
                default:
                    handle_file_or_dir_error("opendir", dir_path, errno);
                    return ERROR;
            }
        }
    } while ((*dir_stream) == NULL);

    return SUCCESS;
}

int handle_src_and_dest_dirs(paths_t* paths, DIR** new_srcpdir) {
    int status = try_to_create_dir(paths->dest_path, paths->mode);

    if (status == ERROR) {
        return ERROR;
    }

    status = try_to_open_dir_with_retry(new_srcpdir, paths->src_path);
    
    if (status == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

int try_to_open_file_with_retry(const char* file_path, int flags, mode_t mode) {
    int fd;
    
    do {
        fd = open(file_path, flags, mode);
        
        if (fd == ERROR) {
            switch (errno) {
                case EMFILE:
                    sleep(WAIT_SEC_FOR_FD);
                    break;
                default:
                    handle_file_or_dir_error("open", file_path, errno);
                    return ERROR;
            }
        }
    } while (fd == ERROR);

    if (mode != NO_MODE) {
        chmod(file_path, mode);
    }

    return fd;
}

int try_to_write_data_to_file_with_retry(const char* buffer, ssize_t read_bytes_num, int dest_fd) {    
    ssize_t total_bytes_num = 0;
    ssize_t remaining_bytes_num = read_bytes_num;

    while (total_bytes_num < read_bytes_num) {
        ssize_t written_bytes_num = write(dest_fd, &buffer[total_bytes_num], 
                remaining_bytes_num);

        if (written_bytes_num == ERROR) {
            return ERROR;
        }
            
        total_bytes_num += written_bytes_num;
        remaining_bytes_num -= written_bytes_num;
    }

    return SUCCESS;
}

bool is_previous_or_current_dir(const char* element) {
    return !(strcmp(element, CURRENT_DIR) && strcmp(element, PREVIOUS_DIR));
}
