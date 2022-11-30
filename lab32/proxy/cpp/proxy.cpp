#include <iostream>
#include <pthread.h>
#include "../hpp/proxy_utils.h"
#include "../hpp/utils.h"



int main(int argc, char **argv) {
    if ((argc - 1) != Constants::PROXY_ARGC_NUM) {
        std::cerr << "Usage: arg1 - proxy port number" << std::endl;
        return EXIT_FAILURE;
    }

    int proxy_port_number = std::stoi(argv[Constants::PROXY_ARGC_INDEX]);
    int proxy_fd = ProxyUtils::open_and_listen_proxy_fd(proxy_port_number);

    if (proxy_fd == Constants::ERROR) {
        return EXIT_FAILURE;
    }

    int close_proxy_fd_status = close(proxy_fd);

    if (close_proxy_fd_status == Constants::ERROR) {
        perror("close");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
