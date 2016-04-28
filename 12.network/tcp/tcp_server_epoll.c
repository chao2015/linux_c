#include "tcp.h"

/*EPOLLLT:水平触发:
LT是默认模式，只要这个fd还有数据可读，每次epoll_wait都会返回它的事件，提醒
用户程序去操作。即数据流入的字节数大于接收的buf大小，会循环读取直到结束。*/

int main(int argc,char* argv[]){
	if(argc>3){
		printf("error:more args.\n");
		return -1;
	}
	//else if(argc<2){
	//	printf("need a server ip.\n");
	//}
	unsigned short port;
	if(argc==3){
		port=(unsigned short)atoi(argv[2]);
	}else{
		port=DEFAULT_SVR_PORT;
	}

	int sockfd;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sockfd){
		perror("socket");
		return -1;
	}

	struct sockaddr_in server;
	bzero(&server,ADDRLEN);
	server.sin_family=AF_INET;
	server.sin_port=htons(port);
	//server.sin_addr.s_addr=inet_addr(argv[1]);
	server.sin_addr.s_addr=INADDR_ANY;

	int ret;
	ret=bind(sockfd,(struct sockaddr*)&server,ADDRLEN);
	if(-1==ret){
		perror("socket");
		return -1;
	}

	ret=listen(sockfd,10);
	if(-1==ret){
		perror("listen");
		return -1;
	}
	
	char buf[10];
	struct sockaddr_in client;
	bzero(&client,ADDRLEN);
	
	int epollfd;
	/*epoll(1):
	1:int size.*/
	epollfd=epoll_create(128);
	if(-1==epollfd){
		perror("epoll_create");
		return -1;
	}

	int len=ADDRLEN;
	int newfd=-1;
	
	struct epoll_event event,evs[3];
	/*Initialize the struct epoll_event:(1)events;(2)epoll_data_t data;
	typedef union epoll_data{...int fd...}epoll_data_t.*/
	event.events=EPOLLIN;
	event.data.fd=0;	
	
	/*epoll_ctl(4):
	1:epoll_create()returned value;
	2:EPOLL_CTL_ADD|MOD|DEL;
	3:int fd;
	4:struct epoll_event*.*/
	ret=epoll_ctl(epollfd,EPOLL_CTL_ADD,0,&event);
	if(-1==ret){
		perror("epoll_ctl_ADD");
		return -1;
	}
	
	event.events=EPOLLIN;
	event.data.fd=sockfd;
	ret=epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&event);
	if(-1==ret){
		perror("epoll_ctl_ADD");
		return -1;
	}
	int i,ret2;
	while(1){
		bzero(evs,sizeof(evs));
		
		/*epoll_wait(4):return the number of events to deal with;
		2:struct epoll_event*;
		3:int maxevents;
		4:int timeout(milliseconds):
		(1)return immediately:0;
		(2)permanent block:-1.*/
		ret2=epoll_wait(epollfd,evs,3,-1);
		
		for(i=0;i<ret2;++i){
			if(evs[i].events==EPOLLIN&&evs[i].data.fd==sockfd){				
				newfd=accept(sockfd,(struct sockaddr*)&client,&len);
				if(-1==newfd){
					perror("accept");
					return -1;
				}
				printf("%s %d success connect\n",inet_ntoa(client.sin_addr),
					ntohs(client.sin_port));	
				event.events=EPOLLIN;
				event.data.fd=newfd;
				epoll_ctl(epollfd,EPOLL_CTL_ADD,newfd,&event);	
			}
			if(evs[i].events==EPOLLIN&&evs[i].data.fd==newfd){				
				bzero(buf,sizeof(buf));
				ret=recv(newfd,buf,sizeof(buf),0);
				if(ret<0){
					perror("recv");
					return -1;
				}else if(ret==0){
					close(newfd);
					event.events=EPOLLIN;
					event.data.fd=newfd;
					epoll_ctl(epollfd,EPOLL_CTL_DEL,newfd,&event);	
				}else{
					printf("%s\n",buf);
				}
			}
			if(evs[i].events==EPOLLIN&&evs[i].data.fd==0){
				bzero(buf,sizeof(buf));
				read(0,buf,sizeof(buf));
				ret=send(newfd,buf,strlen(buf)-1,0);
				if(ret<0){
					perror("send");
					return -1;
				}
			}
		}
	}
	close(sockfd);
	return 0;
}
