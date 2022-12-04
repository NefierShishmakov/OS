#include <stdio.h>
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
            sleep(WAIT_SEC_FOR_THREAD_RESOURCES);
        }

    } while (status != SUCCESS);

    pthread_detach(pid);
}

void *copy_file(void *arg) {
    char *relative_filepath = (char *)arg;

    char src_filepath[get_length_of_new_path(srcdir_path, relative_filepath)];
    char dest_filepath[get_length_of_new_path(destdir_path, relative_filepath)];

    strcpy(strcpy(src_filepath, srcdir_path), relative_filepath);
    strcpy(strcpy(dest_filepath, destdir_path), relative_filepath);

    free(relative_filepath);

    char buffer[BUFSIZE];
    int src_fd;
    int dest_fd;

    // TODO
    // Think how to read and write file 
    // Watch the last link in browser



    return NULL;
}

void *copy_dir(void *arg) {
    char *dirpath_arg = (char *)arg;

    char dirname[strlen(dirpath_arg) + 1];
    strcpy(dirname, dirpath_arg);
    free(dirpath_arg);

    char new_src_dir_path[get_length_of_new_path(srcdir_path, dirname)];
    strcat(strcpy(new_src_dir_path, srcdir_path), dirname);
    
    DIR *new_srcpdir;
    struct dirent *new_src_direntp;

    do {
        new_srcpdir = opendir(new_src_dir_path);
        if (new_srcpdir == NULL) {
            switch (errno) {
                case EMFILE:
                    sleep(WAIT_SEC_FOR_FD);
                    break;
                default:
                    perror("opendir");
                    return NULL;
            }
        }
    } while (new_srcpdir == NULL);

    while ((new_src_direntp = readdir(new_srcpdir)) != NULL) {
        if (!(strcmp(new_src_direntp->d_name, ".") && strcmp(new_src_direntp->d_name, ".."))) {
            continue;
        }
        
        // This is a path of file in src directory
        //strlen(new_src_dir_path) + strlen(new_src_direntp->d_name) + 1
        char path_src[get_length_of_new_path(new_src_dir_path, new_src_direntp->d_name)];
        strcat(strcpy(path_src, new_src_dir_path), new_src_direntp->d_name);

        struct stat buf;
        int status = stat(path_src, &buf);

        if (status != SUCCESS) {
            perror("stat");
            break;
        }
        //strlen(dirname) + strlen(new_src_direntp->d_name)
        char *relative_path = (char *)malloc((get_length_of_new_path(dirname, 
                        new_src_direntp->d_name) + 1) * sizeof(char));

        strcat(strcat(strcpy(relative_path, dirname), SEPARATOR), new_src_direntp->d_name);

        if ((buf.st_mode & S_IFMT) == S_IFREG) {
            try_to_create_thread(copy_file, (void *)&relative_path);
        }
        else if ((buf.st_mode & S_IFMT) == S_IFDIR) {
            char dest_path[get_length_of_new_path(destdir_path, relative_path)];
            strcpy(strcpy(dest_path, destdir_path), relative_path);

            status = try_to_mkdir(dest_path);

            if (status != SUCCESS) {
                break;
            }

            try_to_create_thread(copy_dir, (void *)&relative_path);
        }
        else {
            free(relative_path);
        }
    }
    
    int close_dir_status = closedir(new_srcpdir);
    if (close_dir_status != SUCCESS) {
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

    if (try_to_mkdir(destdir_path) == ERROR) {
        return EXIT_FAILURE;
    }

    prepare_dirs(srcdir_path, destdir_path); 

    copy_dir((void *)strdup(""));

    pthread_exit(NULL);
}

