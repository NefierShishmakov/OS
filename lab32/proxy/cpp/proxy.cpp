#include <iostream>
#include <pthread.h>
#include "../hpp/proxy_utils.h"
#include "../hpp/utils.h"
#include <arpa/inet.h>



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

    while (true) {
        struct sockaddr_in client_addr;
        int clilen = sizeof(client_addr);

        int client_fd = accept(proxy_fd, (struct sockaddr *)&client_addr, (socklen_t *)&clilen);

        if (client_fd == Constants::ERROR) {
            perror("accept");
            int close_status = close(client_fd);

            if (close_status == Constants::ERROR) {
                perror("\'close\' method of socket client");
            }
        }

        char *client_ip = inet_ntoa(client_addr.sin_addr);

        std::cout << "New client with ip - " << "(" << client_ip << ")" << "has been connected" << std::endl;

        char buffer[257] = {0};

        std::cout << "Server started receiving data from the client" << std::endl;
        int read_status = read(client_fd, buffer, 256);
        std::cout << "Server ended receiving data from the client" << std::endl;

        if (read_status == Constants::ERROR) {
            perror("read");
        }

        std::cout << "The data received from the client" << std::endl;
        std::cout << buffer << std::endl;

        close(client_fd);
    }

    int close_proxy_fd_status = close(proxy_fd);

    if (close_proxy_fd_status == Constants::ERROR) {
        perror("close");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
