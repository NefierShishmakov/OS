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
#include "node.h"
#include "paths.h"
#include "utils.h"

void try_to_create_thread(pthread_t *tid, pthread_attr_t *attr, 
        void *(*start_routine)(void *), void *arg) {
    int status;

    do {
        status = pthread_create(tid, attr, start_routine, arg);

        if (status == EAGAIN) {
            sleep(WAIT_SEC_FOR_RESOURCES);
        }

    } while (status != SUCCESS);
}

void *copy_file(void *arg) {
    paths_t *paths = (paths_t *)arg;

    int src_fd;
    int dest_fd;
    src_fd = try_to_open_file(paths->src_path, O_RDONLY, 0);

    if (src_fd == ERROR) {
        return NULL;
    }

    dest_fd = try_to_open_file(paths->dest_path, O_CREAT | O_WRONLY | O_TRUNC, paths->mode);

    if (dest_fd == ERROR) {
        close(src_fd);
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

    pthread_exit(NULL);
}

void *copy_dir(void *arg) {
    paths_t *paths = (paths_t *)arg;
    
    int status = try_to_mkdir(paths->dest_path, paths->mode);

    if (status == ERROR) {
        return NULL;
    }

    DIR *new_srcpdir = NULL;
    status = try_to_open_dir(&new_srcpdir, paths->src_path);
    
    if (status == ERROR) {
        return NULL;
    }

    struct dirent *new_src_direntp;

    Node *head = NULL;

    while ((new_src_direntp = readdir(new_srcpdir)) != NULL) {
        if (is_wrong_element(new_src_direntp->d_name)) {
            continue;
        }

        paths_t *new_paths = (paths_t *)malloc(sizeof(paths_t));
        init_paths_t(new_paths, paths->src_path, paths->dest_path, new_src_direntp->d_name);

        struct stat buf;
        stat(new_paths->src_path, &buf);

        set_mode(new_paths, buf.st_mode);
        
        pthread_t tid;
        if ((buf.st_mode & S_IFMT) == S_IFREG) {
            try_to_create_thread(&tid, NULL, copy_file, new_paths);
            push(&head, tid, new_paths);
        }
        else if ((buf.st_mode & S_IFMT) == S_IFDIR) {
            try_to_create_thread(&tid, NULL, copy_dir, new_paths);
            push(&head, tid, new_paths);
        }
        else {
            free_paths_t(new_paths);
        }
    }
    
    closedir(new_srcpdir);
    free_resources(head);
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

    paths_t *paths = (paths_t *)malloc(sizeof(paths_t));
    init_paths_t(paths, srcdir_path, destdir_path, INITIAL_VALUE);
    set_mode(paths, DEFAULT_FOLDER_MODE);
    
    copy_dir(paths);
    
    free_paths_t(paths);
    pthread_exit(NULL);
}
