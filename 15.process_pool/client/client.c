#include "pool.h"

int main(int argc,char* argv[]){
	if(argc!=3){
		printf("error args\n");
		return -1;
	}
	int sockfd;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in server;
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	server.sin_addr.s_addr=inet_addr(argv[1]);
	
	connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr));

	char buf[1024]={0};
	int len;
	recv_n(sockfd,(char*)&len,sizeof(int));

	recv(sockfd,buf,len,0);

	int fd;
	fd=open(buf,O_CREAT|O_RDWR,0666);

	while(1){
		recv_n(sockfd,(char*)&len,sizeof(int));
		if(len==0)
			break;

		bzero(buf,sizeof(buf));
		recv_n(sockfd,buf,len);

		write(fd,buf,len);
	}
	close(fd);
	close(sockfd);
}
