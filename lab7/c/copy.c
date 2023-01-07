#include "copy.h"
#include "file_dir_utils.h"

void set_pthread_function(void** pthread_function, int file_type) {
    switch (file_type) {
        case S_IFREG:
            *pthread_function = copy_file;
            break;
        case S_IFDIR:
            *pthread_function = copy_dir;
            break;
        default:
            *pthread_function = NULL;
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
    ssize_t read_bytes_num;

    while (true) {
        read_bytes_num = read(src_fd, buffer, BUFSIZ);
        
        if (read_bytes_num == END_OF_FILE) {
            break;
        }

        if (read_bytes_num == ERROR) {
            handle_file_or_dir_error("read", paths->src_path, errno);
            break;
        }

        int status = try_to_write_data_to_file_with_retry(buffer, read_bytes_num, dest_fd);

        if (status == ERROR) {
            handle_file_or_dir_error("write", paths->dest_path, errno);
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

    while ((new_src_direntp = readdir(new_srcpdir)) != END_OF_CATALOG) {
        if (is_previous_or_current_dir(new_src_direntp->d_name)) {
            continue;
        }

        paths_t* new_paths;
        status = init_paths_t(&new_paths, paths->src_path, paths->dest_path, 
                new_src_direntp->d_name);

        if (status != SUCCESS) {
            break;
        }

        struct stat statbuf;
        status = lstat(new_paths->src_path, &statbuf);

        if (status != SUCCESS) {
            handle_error("stat", errno);
            free_paths_t(new_paths);
            break;
        }

        set_mode(new_paths, statbuf.st_mode);

        void* pthread_function;
        int file_type = statbuf.st_mode & S_IFMT;

        set_pthread_function(&pthread_function, file_type);
        
        if (pthread_function == NULL) {
            fprintf(stdout, "\'%s\' is not a regular file or directory\n", new_paths->src_path);
            free_paths_t(new_paths);
            continue;
        }

        pthread_t tid;
        status = try_to_create_thread(&tid, pthread_function, new_paths);

        if (status != SUCCESS) {
            free_paths_t(new_paths);
            break;
        }
    }
    
    closedir(new_srcpdir);
    free_paths_t(paths);
    return NULL;
}
