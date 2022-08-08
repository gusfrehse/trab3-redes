#ifndef REDE_H
#define REDE_H

#include <arpa/inet.h>

int socket_receiver(in_port_t porta);
int socket_sender(in_port_t porta, char *ip_str);

#endif
