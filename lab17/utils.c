#include "utils.h"

int get_result_line_length(size_t entered_line_length) {
    return (entered_line_length > (LINE_LENGTH - 1) ? (SLICED_LINE_LENGTH - 1) : (LINE_LENGTH - 1));
}

void swap(char *first_line, char *second_line) {
    char temp[LINE_LENGTH] = {0};
    strcpy(temp, first_line);

    memset(first_line, 0, LINE_LENGTH);
    strcpy(first_line, second_line);
    memset(second_line, 0, LINE_LENGTH);
    strcpy(second_line, temp);
}

void handle_error(int status, char *error_reason) {
    errno = status;
    perror(error_reason);
}
