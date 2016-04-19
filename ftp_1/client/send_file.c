#include "client.h"

void send_file(int sockfd,char* filename){
	data d;
	bzero(&d,sizeof(d));
	d.len=strlen(filename);
	strcpy(d.buf,filename);
	send_n(sockfd,(char*)&d,4+d.len);//send filename
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
	send_n(sockfd,(char*)&d.len,sizeof(int));//len=0的终止信号
	close(fd);
}
