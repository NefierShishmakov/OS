#include "error_handlers.h"

void handle_file_error(const char *error_cause, const char *file_path, int errnum) {
    char error_buffer[ERROR_BUFSIZE];
    strerror_r(errnum, error_buffer, ERROR_BUFSIZE);
    fprintf(stderr, "%s: \'%s\': %s\n", error_cause, file_path, error_buffer);
}

void handle_dir_error(const char *error_cause, const char *dir_path, int errnum) {
    char error_buffer[ERROR_BUFSIZE];
    strerror_r(errnum, error_buffer, ERROR_BUFSIZE);
    fprintf(stderr, "%s: \'%s\': %s\n", error_cause, dir_path, error_buffer);
}

void handle_pthread_error(const char *error_cause, int errnum) {
    char error_buffer[ERROR_BUFSIZE];
    strerror_r(errnum, error_buffer, ERROR_BUFSIZE);
    fprintf(stderr, "%s: %s\n", error_cause, error_buffer);
}
