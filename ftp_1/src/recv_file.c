#include "factory.h"

void recv_file(int sockfd){
	int len;
	char buf[1024];
	recv_n(sockfd,(char*)&len,sizeof(int));
	recv_n(sockfd,buf,len);
	int fd;
	fd=open(buf,O_CREAT|O_RDWR,0666);
	if(-1==fd){
		perror("open or create");
		exit(-1);
	}
	while(1){
		recv_n(sockfd,(char*)&len,sizeof(int));
		if(len==0) break;
		bzero(buf,sizeof(buf));
		recv_n(sockfd,buf,len);
		write(fd,buf,len);
	}
	printf("success recv!\n");
	close(fd);
}
