#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#define BACKLOG 5

#include <arpa/inet.h>

int initTCPServerSocket(int port);
int createServerSocket();
void setSocketOptions(int server_socket);
void bindServerSocket(int server_socket, int port);
void listenOnSocket(int server_socket);

#endif
