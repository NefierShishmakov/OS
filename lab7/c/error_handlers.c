#include "error_handlers.h"

void handle_file_or_dir_error(const char* error_cause, const char* path, int errnum) {
    char error_buffer[ERROR_BUFFER_SIZE];
    strerror_r(errnum, error_buffer, ERROR_BUFFER_SIZE);
    fprintf(stderr, "%s: \'%s\': %s\n", error_cause, path, error_buffer);
}

void handle_error(const char* error_cause, int errnum) {
    char error_buffer[ERROR_BUFFER_SIZE];
    strerror_r(errnum, error_buffer, ERROR_BUFFER_SIZE);
    fprintf(stderr, "%s: %s\n", error_cause, error_buffer);
}
