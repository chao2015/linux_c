#include "udp.h"

int main(int argc,char* argv[]){
	if(argc!=3){
		printf("error args\n");
		return -1;
	}

	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(-1==sockfd){
		perror("socket");
		return -1;
	}
	
	struct sockaddr_in addr;
	bzero(&addr,ADDRLEN);
	addr.sin_family=AF_INET;
	addr.sin_port=htons(atoi(argv[2]));
	addr.sin_addr.s_addr=inet_addr(argv[1]);
		
	int ret;
	char buf[128]={0};
	int len=ADDRLEN;
	
	strcpy(buf,"I am client");
	ret=sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&addr,ADDRLEN);
	if(-1==ret){
		perror("sendto");
		return -1;
	}
	
	bzero(&buf,sizeof(buf));
	ret=recvfrom(sockfd,buf,sizeof(buf),0,NULL,NULL);
	if(-1==ret){
		perror("recvfrom");
		return -1;
	}
	
	printf("receive from %s %d,the message is:%s\n",
		inet_ntoa(addr.sin_addr),ntohs(addr.sin_port),buf);

	close(sockfd);
	return 0;
}
