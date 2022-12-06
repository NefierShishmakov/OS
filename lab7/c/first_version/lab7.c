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
#include "utils.h"

char *srcdir_path;
char *destdir_path;

void try_to_create_thread(void *(*start_routine)(void *), void *arg) {
    pthread_t tid;
    int status;

    do {
        status = pthread_create(&tid, NULL, start_routine, arg);

        if (status == EAGAIN) {
            sleep(WAIT_SEC_FOR_RESOURCES);
        }

    } while (status != SUCCESS);

    pthread_detach(tid);
}

void *copy_file(void *arg) {
    char src_filepath[get_length_of_new_path(srcdir_path, arg, true)];
    char dest_filepath[get_length_of_new_path(destdir_path, arg, true)];

    strcat(strcat(strcpy(src_filepath, srcdir_path), SEPARATOR), arg);
    strcat(strcat(strcpy(dest_filepath, destdir_path), SEPARATOR), arg);
    free(arg);

    int src_fd;
    int dest_fd;
    src_fd = try_to_open_file(src_filepath, O_RDONLY, 0);

    if (src_fd == ERROR) {
        return NULL;
    }
    
    struct stat buf;
    stat(src_filepath, &buf);

    dest_fd = try_to_open_file(dest_filepath, O_CREAT | O_WRONLY | O_TRUNC, buf.st_mode);

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
            handle_error(errno);
            break;
        }

        ssize_t written_bytes = write(dest_fd, buffer, read_bytes);

        if (written_bytes == ERROR) {
            handle_error(errno);
            break;
        }
    }
    
    close(src_fd);
    close(dest_fd);

    return NULL;
}

void *copy_dir(void *arg) {
    char dirname[strlen(arg) + 1];
    strcpy(dirname, arg);

    char new_src_dir_path[get_length_of_new_path(srcdir_path, dirname, true)];
    strcat(strcat(strcpy(new_src_dir_path, srcdir_path), SEPARATOR), dirname);
    free(arg);
    
    DIR *new_srcpdir = NULL;
    int status = try_to_open_dir(&new_srcpdir, new_src_dir_path);
    
    if (status == ERROR) {
        if (!strcmp(dirname, INITIAL_VALUE)) {
            exit(EXIT_FAILURE);
        }
        return NULL;
    }

    struct dirent *new_src_direntp;

    while ((new_src_direntp = readdir(new_srcpdir)) != NULL) {
        if (!(strcmp(new_src_direntp->d_name, CURRENT_DIR) && 
                    strcmp(new_src_direntp->d_name, PREVIOUS_DIR))) {
            continue;
        }
        
        char path_src[get_length_of_new_path(new_src_dir_path, new_src_direntp->d_name, false)];
        strcat(strcpy(path_src, new_src_dir_path), new_src_direntp->d_name);

        struct stat buf;
        status = stat(path_src, &buf);

        if (status != SUCCESS) {
            handle_error(errno);
            break;
        }

        char *relative_path = (char *)malloc((get_length_of_new_path(dirname, 
                        new_src_direntp->d_name, true)) * sizeof(char));

        strcat(strcpy(relative_path, dirname), new_src_direntp->d_name);

        if ((buf.st_mode & S_IFMT) == S_IFREG) {
            try_to_create_thread(copy_file, (void *)relative_path);
        }
        else if ((buf.st_mode & S_IFMT) == S_IFDIR) {
            strcat(relative_path, SEPARATOR);
            char dest_path[get_length_of_new_path(destdir_path, relative_path, true)];
            strcat(strcat(strcpy(dest_path, destdir_path), SEPARATOR), relative_path);

            status = try_to_mkdir(dest_path, buf.st_mode);

            if (status != SUCCESS) {
                free(relative_path);
                continue;
            }

            try_to_create_thread(copy_dir, (void *)relative_path);
        }
        else {
            free(relative_path);
        }
    }
    
    closedir(new_srcpdir);
    return NULL;
}

int main(int argc, char **argv) {
    if ((argc - 1) != DEFAULT_ARGC_NUM) {
        fprintf(stderr, "Usage: first arg - source tree full path, second arg - target tree full path\n");
        return EXIT_FAILURE;
    }
    
    srcdir_path = argv[SOURCE_TREE_FULL_PATH_ARGC_IDX];
    destdir_path = argv[TARGET_TREE_FULL_PATH_ARGC_IDX];

    prepare_paths(srcdir_path, destdir_path);
       
    if (strstr(destdir_path, srcdir_path) != NULL) {
        fprintf(stderr, "Can\'t copy %s dir to itself\n", srcdir_path);
        return EXIT_FAILURE;
    }

    if (try_to_mkdir(destdir_path, DEFAULT_FOLDER_MODE) == ERROR) {
        return EXIT_FAILURE;
    }

    copy_dir(strdup(INITIAL_VALUE));

    pthread_exit(NULL);
}

