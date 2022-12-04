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
    return NULL;
}

void *copy_dir(void *arg) {
    char *dirname = (char *)arg;

    char new_src_dir_path[get_size_of_new_dir_path(srcdir_path, dirname)];
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
        char path[strlen(new_src_dir_path) + strlen(new_src_direntp->d_name) + 1];
        strcat(strcpy(path, new_src_dir_path), new_src_direntp->d_name);

        struct stat buf;
        int status = stat(path, &buf);

        if (status != SUCCESS) {
            perror("stat");
            break;
        }

        char relative_path[strlen(dirname) + strlen(new_src_direntp->d_name) + 2];
        strcat(strcat(strcpy(relative_path, dirname), "/"), new_src_direntp->d_name);

        switch ((buf.st_mode & S_IFMT)) {
            case S_IFREG:
                try_to_create_thread(copy_file, (void *)relative_path);
                break;
            case S_IFDIR:
                
                break;
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

    char *initial_dir = "";
    prepare_dirs(srcdir_path, destdir_path); 

    copy_dir((void *)initial_dir);

    return EXIT_SUCCESS;
}

