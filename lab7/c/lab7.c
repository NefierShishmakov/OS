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

typedef struct dirs_t {
    char *srcdir;
    char *destdir;
} dirs_t;

void *copy_file(void *arg) {
    return NULL;
}

void *copy_dir(void *arg) {
    dirs_t *dirs = (dirs_t *)&arg;
    
    
    
    return NULL;
}

int main(int argc, char **argv) {
    if ((argc - 1) != DEFAULT_ARGC_NUM) {
        fprintf(stderr, "Usage: first arg - source tree full path, second arg - target tree full path\n");
        return EXIT_FAILURE;
    }
    
    dirs_t dirs = {argv[SOURCE_TREE_FULL_PATH_ARGC_IDX], argv[TARGET_TREE_FULL_PATH_ARGC_IDX]};

    if (try_to_mkdir(dirs.destdir) == ERROR) {
        return EXIT_FAILURE;
    }
    
    copy_dir((void *)&dirs);

    return EXIT_SUCCESS;
}

