#include "client.h"

void recv_file(int sockfd,int len){
	//int len;
	char buf[1024];
	//recv_n(sockfd,(char*)&len,sizeof(int));
	printf("%d\n",len);
	len=0-len;
	recv_n(sockfd,buf,len);
	int fd;
	fd=open(buf,O_CREAT|O_RDWR,0666);
	while(1){
		recv_n(sockfd,(char*)&len,sizeof(int));
		len=0-len;
		if(len==0) break;
		bzero(buf,sizeof(buf));
		recv_n(sockfd,buf,len);
		write(fd,buf,len);
	}
	printf("success recv!\n");
	close(fd);
}
