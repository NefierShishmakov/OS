#include "proxy.h"

namespace ProxySpace
{
    Proxy::Proxy(int proxy_port, int backlog_size)
    {
        this->proxy_port = proxy_port;
        this->backlog_size = backlog_size;
        this->init_proxy_addr();
    }

    void Proxy::init_proxy_addr()
    {
        bzero((char *)&this->proxy_addr, sizeof(this->proxy_addr));
        proxy_addr.sin_family = AF_INET;
        proxy_addr.sin_addr.s_addr = INADDR_ANY;
        proxy_addr.sin_port = htons(this->proxy_port);
    }

    int Proxy::open_and_listen_fd()
    {
        this->proxy_fd = socket(AF_INET, SOCK_STREAM, Constants::DEFAULT_PROTOCOL);

        if (this->proxy_fd == Constants::ERROR) {
            perror("socket");
            return Constants::ERROR;
        }

        int optval = 1;

        int setsockopt_status = setsockopt(proxy_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(optval));

        if (setsockopt_status == Constants::ERROR) {
            perror("setsockopt");
            return Constants::ERROR;
        }

        int bind_status = bind(this->proxy_fd, (struct sockaddr *)&this->proxy_addr,
                sizeof(this->proxy_addr));

        if (bind_status == Constants::ERROR) {
            perror("bind");
            return Constants::ERROR;
        }

        int listen_status = listen(this->proxy_fd, backlog_size);

        if (listen_status == Constants::ERROR) {
            perror("listen");
            return Constants::ERROR;
        }

        return Constants::SUCCESS;
    }

    void Proxy::handle_clients()
    {
        while (true)
        {
            struct sockaddr_in client_addr;
            int clilen = sizeof(client_addr);

            std::cout << "Server is waiting for new connections" << std::endl;

            int client_fd = accept(this->proxy_fd, (struct sockaddr *) &client_addr, (socklen_t *) &clilen);

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
    }

    Proxy::~Proxy()
    {
        int close_status = close(this->proxy_fd);

        if (close_status == Constants::ERROR) {
            perror("close");
        }
    }
}
