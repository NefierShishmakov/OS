#include <pthread.h>
#include <errno.h>
#include <stdio.h>

#define LINES_NUM 10
#define SUCCESS 0
#define FAILURE 1

typedef struct function_args {
    const char *name;
    int num;
} function_args;

void *print_lines(void *arg) {
    function_args *args = (function_args *)arg;

    for (int i = 1; i <= args->num; ++i) {
        printf("The %s line %d is printed\n", args->name, i);
    }

    pthread_exit(NULL);
}

// Я использовал здесь static над этими переменными, для того чтобы передостеречься от одного исхода
// Может случиться такое, что главная нить завершится до того, как дочерняя нить начнёт своё выполнение с параметрами.
// Т.е. возникнет висячая ссылка, и чтобы этого избежать я использую ключевое слово static
// Все объяснения, как что работает я расположил в файле explanation.txt


static function_args child = {"child", LINES_NUM};
static function_args parent = {"parent", LINES_NUM};

int main(void) {
    pthread_t pthread_id;

    int create_status = pthread_create(&pthread_id, NULL, print_lines, &child);

    if (create_status != SUCCESS) {
        errno = create_status;
        perror("pthread_create");

        return FAILURE;
    }

    print_lines(&parent);
    
    pthread_exit(NULL);
}