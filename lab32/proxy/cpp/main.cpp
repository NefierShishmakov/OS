#include <pthread.h>
#include "../hpp/proxy.h"

int main() {
    ProxySpace::Proxy proxy = ProxySpace::Proxy(Constants::PROXY_PORT_NUMBER, Constants::DEFAULT_PROTOCOL);

    int open_and_listen_fd_status = proxy.open_and_listen_fd();

    if (open_and_listen_fd_status == Constants::ERROR) {
        return EXIT_FAILURE;
    }

    proxy.handle_clients();

    return EXIT_SUCCESS;
}
