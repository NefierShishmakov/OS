#include "utils.h"

int try_to_mkdir(const char *dir) {
    int status = mkdir(dir, 0700);

    if (status != SUCCESS) {
        if (errno == EEXIST) {
            status = access(dir, W_OK | X_OK);

            if (status != SUCCESS) {
                fprintf(stderr, "%s is exists but not accessible\n", dir);
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

size_t get_length_of_new_path(const char *first_path, const char *second_path) {
    return (strlen(first_path) + strlen(second_path) + 1);
}
