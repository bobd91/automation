#ifndef _SERVER_H
#define _SERVER_H

#include "lwip/tcp.h"

err_t server_connect(char *server_ip, char *server_port);

#endif