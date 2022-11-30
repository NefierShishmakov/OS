#include "../hpp/utils.h"

namespace Utils {
    void print_error(int status, char *error_reason) {
        errno = status;
        perror(error_reason);
    }
}