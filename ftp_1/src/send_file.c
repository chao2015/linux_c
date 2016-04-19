#include "factory.h"

void send_file(int sockfd,char* filename){
	data d;
	bzero(&d,sizeof(d));
	d.len=0-strlen(filename);
	strcpy(d.buf,filename);
	send_n(sockfd,(char*)&d,4+(0-d.len));//send filename
	
	int fd;
	fd=open(filename,O_RDONLY);
	if(-1==fd){
		perror("send_file open");
		exit(-1);
	}
	
	int ret;
	while(bzero(&d,sizeof(d)),(d.len=read(fd,d.buf,sizeof(d.buf)))>0){
		d.len=0-d.len;
		ret=send_n(sockfd,(char*)&d,4+(0-d.len));
		if(-1==ret){
			printf("send_n failed\n");
			exit(-1);
		}
	}
	send_n(sockfd,(char*)&d.len,sizeof(int));
	close(fd);
}
