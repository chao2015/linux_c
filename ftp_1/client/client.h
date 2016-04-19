#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct{
	int len;
	char buf[1000];
}data;

typedef struct com{
	int key;
	char comd[10];
	struct com* next;
}command,*pcommand;

int recv_n(int,char*,int);
int send_n(int,char*,int);
void send_file(int,char*);
void recv_file(int,int);

