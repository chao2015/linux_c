#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//struct sockaddr_in
#include <arpa/inet.h>//inet_
#include <strings.h>//bzero
#include <unistd.h>//read
#include <string.h>

#include <sys/epoll.h>

#define DEFAULT_SVR_PORT 2000
#define ADDRLEN sizeof(struct sockaddr)
