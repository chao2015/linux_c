#include "factory.h"

void send_msg(int sockfd,char* filename){
	data d;
	bzero(&d,sizeof(d));
	int fd;
	fd=open(filename,O_RDONLY);
	if(-1==fd){
		perror("send_file open");
		exit(-1);
	}
	
	int ret;
	while(bzero(&d,sizeof(d)),(d.len=read(fd,d.buf,sizeof(d.buf)))>0){
		ret=send_n(sockfd,(char*)&d,4+d.len);
		if(-1==ret){
			printf("send_n failed\n");
			exit(-1);
		}
	}
	int flag=0;
	send_n(sockfd,(char*)&flag,sizeof(int));
	close(fd);
}
