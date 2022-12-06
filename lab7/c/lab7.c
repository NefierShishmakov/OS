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
    pthread_t pid;
    int status;

    do {
        status = pthread_create(&pid, NULL, start_routine, arg);

        if (status == EAGAIN) {
            sleep(WAIT_SEC_FOR_RESOURCES);
        }

    } while (status != SUCCESS);

    pthread_detach(pid);
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
            perror("read");
            break;
        }

        ssize_t written_bytes = write(dest_fd, buffer, read_bytes);

        if (written_bytes == ERROR) {
            perror("write");
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
    
    // TODO
    // Fix exit on returning error num
    if (status == ERROR) {
        exit(EXIT_FAILURE);
    }

    struct dirent *new_src_direntp;
    
    while ((new_src_direntp = readdir(new_srcpdir)) != NULL) {
        if (!(strcmp(new_src_direntp->d_name, CURRENT_DIR) && 
                    strcmp(new_src_direntp->d_name, PREVIOUS_DIR))) {
            continue;
        }
        
        // This is a path of file in src directory
        //strlen(new_src_dir_path) + strlen(new_src_direntp->d_name) + 1
        char path_src[get_length_of_new_path(new_src_dir_path, new_src_direntp->d_name, false)];
        strcat(strcpy(path_src, new_src_dir_path), new_src_direntp->d_name);

        struct stat buf;
        status = stat(path_src, &buf);

        if (status != SUCCESS) {
            perror("stat");
            break;
        }
        //strlen(dirname) + strlen(new_src_direntp->d_name)
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

            status = try_to_mkdir(dest_path);

            if (status != SUCCESS) {
                free(relative_path);
                break;
            }

            try_to_create_thread(copy_dir, (void *)relative_path);
        }
        else {
            free(relative_path);
        }
    }
    
    status = closedir(new_srcpdir);
    if (status != SUCCESS) {
        perror("closedir");
    }

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
    
    if (!strcmp(srcdir_path, destdir_path)) {
        fprintf(stderr, "Can't copy %s dir to itself\n", srcdir_path);
        return EXIT_FAILURE;
    }

    if (try_to_mkdir(destdir_path) == ERROR) {
        return EXIT_FAILURE;
    }

    copy_dir(strdup(INITIAL_VALUE));

    pthread_exit(NULL);
}

