#include "utils.h"

int get_result_line_length(size_t entered_line_length) {
    return (entered_line_length > LINE_LENGTH ? SLICED_LINE_LENGTH : LINE_LENGTH);
}

void handle_error(int status, char *error_reason) {
    errno = status;
    perror(error_reason);
}
