#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "copy.h"

int main(int argc, char** argv) {
    if ((argc - 1) != DEFAULT_ARGC_NUM) {
        fprintf(stderr, "Usage: first arg - source tree full path, second arg - target tree full path\n");
        return EXIT_FAILURE;
    }
    
    char* srcdir_path = argv[SOURCE_TREE_FULL_PATH_ARGC_IDX];
    char* destdir_path = argv[TARGET_TREE_FULL_PATH_ARGC_IDX];

    prepare_paths(srcdir_path, destdir_path);

    if (strstr(srcdir_path, destdir_path) != NULL) {
        fprintf(stderr, "Can\'t copy %s dir to itself\n", srcdir_path);
        return EXIT_FAILURE;
    }

    paths_t* paths = NULL;
    int status = init_paths_t(&paths, srcdir_path, destdir_path, INITIAL_VALUE);

    if (status != SUCCESS) {
        return EXIT_FAILURE;
    }

    set_mode(paths, DEFAULT_FOLDER_MODE);
    
    copy_dir(paths);
    
    pthread_exit(NULL);
}
