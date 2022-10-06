#include <pthread.h>
#include <stdlib.h>

#define MAX_STRINGS_NUM 100

int main(int argc, char **argv) {
    if (argc > MAX_STRINGS_NUM) {
        fprintf(stderr, "Too many strings ");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
