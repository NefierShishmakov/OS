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

char *srcdir;
char *destdir;

void print_success_status() {
    printf("Copy dirs from %s to %s dir finished successfully\n", srcdir, destdir);
}

void *copy_file(void *arg) {
    return NULL;
}

void *copy_dir(void *arg) {
    return NULL;
}

int main(int argc, char **argv) {
    if ((argc - 1) != DEFAULT_ARGC_NUM) {
        fprintf(stderr, "Usage: first arg - source tree full path, second arg - target tree full path\n");
        return EXIT_FAILURE;
    }

    srcdir = argv[SOURCE_TREE_FULL_PATH_ARGC_IDX];
    destdir = argv[TARGET_TREE_FULL_PATH_ARGC_IDX];

    if (try_to_mkdir(srcdir) == ERROR) {
        return EXIT_FAILURE;
    }

    int status = atexit(&print_success_status);

    if (status != SUCCESS) {
        fprintf(stderr, "atexit function error\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

