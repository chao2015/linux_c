#include "shmat.h"

void lock(int *p){
	while(1){
		if(*p==0){
			*p=1;
			break;
		}
	}
}

void unlock(int *p){
	*p=0;
}

int main(){
	int shmid;
	shmid=shmget((key_t)1234,1<<10,IPC_CREAT|0600);
	if(-1==shmid){
		perror("shmget");
		return -1;
	}

	pchat p;
	p=(pchat)shmat(shmid,NULL,0);
	if((pchat)-1==p){
		perror("shmat");
		return -1;
	}

	char buf[1024];
	memset(p,0,sizeof(chat));
	
	fd_set rdset;
	struct timeval tv;
	tv.tv_sec=0;
	tv.tv_usec=100;
	int ret;
	while(1){
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		ret=select(1,&rdset,NULL,NULL,&tv);
		if(ret>0){
			memset(buf,0,sizeof(buf));
			read(0,buf,sizeof(buf));
			lock(&p->lock);
			strcpy(p->buf,buf);
			p->noread=1;
			unlock(&p->lock);
		}
		lock(&p->lock);
		if(p->noread==2){
			printf("%s",p->buf);
			p->noread=0;
			memset(p->buf,0,sizeof(p->buf));
		}
		unlock(&p->lock);
		usleep(5);
	}
	return 0;	
}

