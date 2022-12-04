#include "utils.h"

int try_to_mkdir(const char *destdir) {
    int status = mkdir(destdir, 0700);

    if (status != SUCCESS) {
        if (errno == EEXIST) {
            status = access(destdir, W_OK | X_OK);

            if (status != SUCCESS) {
                fprintf(stderr, "%s is exists but not accessible\n", destdir);
                return ERROR;
            }
        }
        else {
            perror("mkdir");
            return ERROR;
        }
    }

    return SUCCESS;
}

void prepare_dirs(char *srcdir, char *destdir) {
    size_t srcdir_len = strlen(srcdir);
    size_t destdir_len = strlen(destdir);
    
    if (srcdir[srcdir_len - 1] == '/') {
        srcdir[srcdir_len - 1] = '\0';
    }

    if (destdir[destdir_len - 1] == '/') {
        destdir[destdir_len - 1] = '\0';
    }
}

size_t get_size_of_new_dir_path(char *prev_dir_path, char *dir_to_add) {
    return (strlen(prev_dir_path) + strlen(dir_to_add) + 1);
}
