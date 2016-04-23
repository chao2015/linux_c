#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

typedef struct{
	int lock;
	int noread;
	char buf[1024];
}chat,*pchat;

