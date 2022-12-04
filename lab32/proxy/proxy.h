#ifndef LAB32_PROXY_H
#define LAB32_PROXY_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "constants.h"

namespace ProxySpace
{
    class Proxy
    {
        public:
            Proxy(int proxy_port, int backlog_size);
            ~Proxy();
            int open_and_listen_fd();
            void handle_clients();
        private:
            struct sockaddr_in proxy_addr;
            int proxy_fd;
            int proxy_port;
            int backlog_size;
        private:
            void init_proxy_addr();
    };
}

#endif
