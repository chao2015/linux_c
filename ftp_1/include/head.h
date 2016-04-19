#ifndef __HEAD_H__
#define __HEAD_H__
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <time.h>

typedef struct{
	int len;
	char buf[1000];
}data;

int send_n(int,char*,int);
#endif
