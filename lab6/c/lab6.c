#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STRINGS_NUM 100
#define MIN_STRINGS_NUM 1

int main(int argc, char **argv) {
    if (argc < MIN_STRINGS_NUM || argc > MAX_STRINGS_NUM) {
        fprintf(stderr, "Strings number should be more or equal %d and less or equal %d\n", MIN_STRINGS_NUM, MAX_STRINGS_NUM);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

