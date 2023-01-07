#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "copy.h"
#include "paths.h"

#define ARGC_NUM 2
#define SOURCE_DIR_PATH_ARGC_INDEX 1
#define DESTINATION_DIR_PATH_ARGC_INDEX 2

int main(int argc, char** argv) {
    if ((argc - 1) != ARGC_NUM) {
        fprintf(stderr, "Usage: first arg - source tree full path, second arg - target tree full path\n");
        return EXIT_FAILURE;
    }
    
    char* srcdir_path = argv[SOURCE_DIR_PATH_ARGC_INDEX];
    char* destdir_path = argv[DESTINATION_DIR_PATH_ARGC_INDEX];

    prepare_paths(srcdir_path, destdir_path);

    if (is_dest_path_in_src_path(srcdir_path, destdir_path)) {
        fprintf(stderr, "Can\'t copy %s dir to itself\n", srcdir_path);
        return EXIT_FAILURE;
    }

    paths_t* paths;
    int status = init_paths_t(&paths, srcdir_path, destdir_path, INITIAL_VALUE);

    if (status != SUCCESS) {
        return EXIT_FAILURE;
    }

    set_mode(paths, DEFAULT_FOLDER_MODE);
    
    copy_dir(paths);
    
    pthread_exit(NULL);
}
