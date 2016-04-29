#include "udp.h"

int main(int argc,char* argv[]){
	if(argc<2){
		printf("need an ip address\n");
		return -1;
	}
	int port;
	port=DEFAULT_SVR_PORT;
	if(argc==3){
		port=(unsigned short)atoi(argv[2]);
	}
	if(argc>3){
		printf("error:more args\n");
		return -1;
	}

	/*socket(3 params):generate a socket descriptor.
	1:AF_INET/AF_INET6; 
	2:SOCK_DGRAM/SOCK_STREAM; 
	3:transfer protocol num:default:0.*/
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(-1==sockfd){
		perror("sockfd");
		return -1;
	}

	/*htons(1):host to network short(16 ip port).
	inet_addr(1):change ip address string into network binary digits.
	sin_port=0:automatically choose a free port for you.
	sin_addr.s_addr=INADDR_ANY:automatically fill in the machine ip.*/
	struct sockaddr_in addr;
	bzero(&addr,ADDRLEN);
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr=inet_addr(argv[1]);
	
	int ret;
	/*bind(3):*/
	ret=bind(sockfd,(struct sockaddr*)&addr,ADDRLEN);
	if(-1==ret){
		perror("bind");
		return -1;
	}
	
	/*recvfrom(6):
	4:flags default:0.
	5:Save the connection machine IP address and port:
	(1)server:create a new structure to store information;
	(2)client:NULL;
	6:&len(int len=sizeof(addr)),client:NULL.*/
	char buf[128]={0};
	while(1){
		int len=ADDRLEN;
		struct sockaddr_in client;
		bzero(&client,ADDRLEN);
		ret=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&client,&len);
		if(-1==ret){
			perror("recvfrom");
			return -1;
		}
		printf("receive from %s %d,the message is:%s\n",
			inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
	
		/*sendto(6):
		2:buf;
		3:strlen(buf);when use read():strlen(buf)-1;
		4:flags default:0;
		6:int sizeof(addr).*/
		ret=sendto(sockfd,"Hello",5,0,(struct sockaddr*)&client,ADDRLEN);
		if(-1==ret){
			perror("sendto");
			return -1;
		}
	}
	close(sockfd);
	return 0;
}
