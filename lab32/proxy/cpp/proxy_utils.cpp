#include "../hpp/proxy_utils.h"

namespace ProxyUtils {
    int open_and_listen_proxy_fd(int proxy_port_number) {
        int proxy_fd = socket(AF_INET, SOCK_STREAM, Constants::DEFAULT_PROTOCOL);

        if (proxy_fd == Constants::ERROR) {
            perror("socket");
            return Constants::ERROR;
        }

        struct sockaddr_in proxy_addr;

        bzero((char *)&proxy_addr, sizeof(proxy_addr));

        proxy_addr.sin_family = AF_INET;
        proxy_addr.sin_addr.s_addr = INADDR_ANY;
        proxy_addr.sin_port = htons(proxy_port_number);

        int bind_status = bind(proxy_fd, (struct sockaddr *)&proxy_addr, sizeof(proxy_addr));

        if (bind_status == Constants::ERROR) {
            perror("bind");
            int close_status = close(proxy_fd);

            if (close_status == Constants::ERROR) {
                perror("close");
            }
            return Constants::ERROR;
        }

        int listen_status = listen(proxy_fd, Constants::DEFAULT_BACKLOG_SIZE);

        if (listen_status == Constants::ERROR) {
            perror("listen");
            int close_status = close(proxy_fd);

            if (close_status == Constants::ERROR) {
                perror("close");
            }

            return Constants::ERROR;
        }

        return proxy_fd;
    }
}