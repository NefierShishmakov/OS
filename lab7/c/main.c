#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include "constants.h"
#include "paths.h"
#include "file_dir_utils.h"
#include "pthread_utils.h"

void *copy_file(void *arg) {
    paths_t *paths = (paths_t *)arg;

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
        else if (read_bytes == ERROR) {
            handle_file_error("read", paths->src_path, errno);
            break;
        }

        ssize_t written_bytes = write(dest_fd, buffer, read_bytes);

        if (written_bytes == ERROR) {
            handle_file_error("write", paths->dest_path, errno);
            break;
        }
    }
    
    close(src_fd);
    close(dest_fd);
    free_paths_t(paths);
    pthread_exit(NULL);
}

void *copy_dir(void *arg) {
    paths_t *paths = (paths_t *)arg;
    
    int status = try_to_mkdir(paths->dest_path, paths->mode);

    if (status == ERROR) {
        free_paths_t(paths);
        return NULL;
    }

    DIR *new_srcpdir = NULL;
    status = try_to_open_dir_with_retry(&new_srcpdir, paths->src_path);
    
    if (status == ERROR) {
        free_paths_t(paths);
        return NULL;
    }

    struct dirent *new_src_direntp;

    while ((new_src_direntp = readdir(new_srcpdir)) != NULL) {
        if (is_wrong_element(new_src_direntp->d_name)) {
            continue;
        }

        paths_t *new_paths = NULL;
        status = init_paths_t(&new_paths, paths->src_path, paths->dest_path, 
                new_src_direntp->d_name);

        if (status != SUCCESS) {
            break;
        }

        struct stat buf;
        status = stat(new_paths->src_path, &buf);

        if (status != SUCCESS) {
            handle_error("stat", errno);
            free_paths_t(new_paths);
            break;
        }

        set_mode(new_paths, buf.st_mode);
        
        pthread_t tid;
        if ((buf.st_mode & S_IFMT) == S_IFREG) {
            status = try_to_create_thread(&tid, copy_file, new_paths);

            if (status != SUCCESS) {
                free_paths_t(new_paths);
                break;
            }
        }
        else if ((buf.st_mode & S_IFMT) == S_IFDIR) {
            status = try_to_create_thread(&tid, copy_dir, new_paths);

            if (status != SUCCESS) {
                free_paths_t(new_paths);
                break;
            }
        }
        else {
            fprintf(stdout, "\'%s\' is not a regular file or directory\n", new_paths->src_path);
            free_paths_t(new_paths);
        }
    }
    
    closedir(new_srcpdir);
    free_paths_t(paths);
    return NULL;
}

int main(int argc, char **argv) {
    if ((argc - 1) != DEFAULT_ARGC_NUM) {
        fprintf(stderr, "Usage: first arg - source tree full path, second arg - target tree full path\n");
        return EXIT_FAILURE;
    }
    
    char *srcdir_path = argv[SOURCE_TREE_FULL_PATH_ARGC_IDX];
    char *destdir_path = argv[TARGET_TREE_FULL_PATH_ARGC_IDX];

    prepare_paths(srcdir_path, destdir_path);

    if (strstr(destdir_path, srcdir_path) != NULL) {
        fprintf(stderr, "Can\'t copy %s dir to itself\n", srcdir_path);
        return EXIT_FAILURE;
    }

    paths_t *paths = NULL;
    int status = init_paths_t(&paths, srcdir_path, destdir_path, INITIAL_VALUE);

    if (status != SUCCESS) {
        return EXIT_FAILURE;
    }

    set_mode(paths, DEFAULT_FOLDER_MODE);
    
    copy_dir(paths);
    
    pthread_exit(NULL);
}
