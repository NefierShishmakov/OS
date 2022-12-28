#include "copy.h"
#include "file_dir_utils.h"

void set_pthread_function(void** result_function, int file_type) {
    switch (file_type) {
        case S_IFREG:
            *result_function = copy_file;
            break;
        case S_IFDIR:
            *result_function = copy_dir;
            break;
        default:
            *result_function = NULL;
            break;
    }
}

void* copy_file(void* arg) {
    paths_t* paths = (paths_t*)arg;

    int src_fd;
    int dest_fd;
    src_fd = try_to_open_file_with_retry(paths->src_path, O_RDONLY, NO_MODE);

    if (src_fd == ERROR) {
        free_paths_t(paths);
        return NULL;
    }

    dest_fd = try_to_open_file_with_retry(paths->dest_path, O_CREAT | O_WRONLY | O_TRUNC, 
            paths->mode);

    if (dest_fd == ERROR) {
        close(src_fd);
        free_paths_t(paths);
        return NULL;
    }

    char buffer[BUFSIZ];
    ssize_t read_bytes;

    while (true) {
        read_bytes = read(src_fd, buffer, BUFSIZ);
        
        if (read_bytes == END_OF_FILE) {
            break;
        }
        if (read_bytes == ERROR) {
            handle_file_error("read", paths->src_path, errno);
            break;
        }

        int status = try_to_write_data_to_file_with_retry(buffer, read_bytes, dest_fd);

        if (status == ERROR) {
            handle_file_error("write", paths->dest_path, errno);
            break;
        }
    }
    
    close(src_fd);
    close(dest_fd);
    free_paths_t(paths);
    pthread_exit(NULL);
}

void* copy_dir(void* arg) {
    paths_t* paths = (paths_t*)arg;
    DIR* new_srcpdir = NULL;
    
    int status = handle_src_and_dest_dirs(paths, &new_srcpdir);

    if (status == ERROR) {
        free_paths_t(paths);
        return NULL;
    }

    struct dirent* new_src_direntp;

    while ((new_src_direntp = readdir(new_srcpdir)) != NULL) {
        if (is_wrong_element(new_src_direntp->d_name)) {
            continue;
        }

        paths_t* new_paths = NULL;
        status = init_paths_t(&new_paths, paths->src_path, paths->dest_path, 
                new_src_direntp->d_name);

        if (status != SUCCESS) {
            break;
        }

        struct stat buf;
        status = lstat(new_paths->src_path, &buf);

        if (status != SUCCESS) {
            handle_error("stat", errno);
            free_paths_t(new_paths);
            break;
        }

        set_mode(new_paths, buf.st_mode);

        void* result_function = NULL;

        set_pthread_function(&result_function, (buf.st_mode & S_IFMT));
        
        if (result_function == NULL) {
            fprintf(stdout, "\'%s\' is not a regular file or directory\n", new_paths->src_path);
            free_paths_t(new_paths);
            continue;
        }

        pthread_t tid;
        status = try_to_create_thread(&tid, result_function, new_paths);

        if (status != SUCCESS) {
            free_paths_t(new_paths);
            break;
        }
    }
    
    closedir(new_srcpdir);
    free_paths_t(paths);
    return NULL;
}