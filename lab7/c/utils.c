#include "utils.h"

int try_to_mkdir(const char *srcdir) {
    int status = mkdir(srcdir, 0700);

    if (status != SUCCESS) {
        if (errno == EEXIST) {
            status = access(srcdir, W_OK | X_OK);

            if (status != SUCCESS) {
                perror("access");
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