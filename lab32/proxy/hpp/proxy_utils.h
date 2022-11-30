#ifndef LAB32_PROXY_UTILS_H
#define LAB32_PROXY_UTILS_H

#include <iostream>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include "constants.h"
#include <unistd.h>

namespace ProxyUtils {
    int open_and_listen_proxy_fd(int proxy_port_number);
}

#endif
