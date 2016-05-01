#include <stdio.h>
#include <sys/stat.h>//open
#include <sys/types.h>//open,socket
#include <fcntl.h>//open
#include <sys/socket.h>//socket
#include <sys/epoll.h>//epoll
#include <netinet/in.h>//inet_addr
#include <arpa/inet.h>//inet_addr,htons
#include <stdlib.h>//malloc
#include <strings.h>//bzero
#include <string.h>//strlen

typedef struct{
	pid_t pid;
	int fd;
	int busy;
}child,*pchild;

typedef struct{
	int len;//代表后面buf的实际发送长度
	char buf[1000];
}data;

#define DOWNLOAD "the_holy_bible.txt"
void make_child(pchild,int);
void child_handle(int);
void send_file(int);
void send_file(int);
int recv_n(int,char*,int);
int send_n(int,char*,int);
void send_fd(int,int);
void recv_fd(int,int*);
