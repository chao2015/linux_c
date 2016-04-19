#include "client.h"

void command_handle(int sockfd,char* buf){
	char argc[100]={0};
	sleep(1);
	if(strncmp(buf,"puts",4)==0){
		strncpy(argc,buf+5,strlen(buf)-6);//减去换行符,否则argc传参错误
		printf("%s\n",argc);
		send_file(sockfd,argc);
	}
}

int recv_msg(int sockfd){
	int len;
	char buf[1024]={0};
	while(1){
		recv_n(sockfd,(char*)&len,sizeof(int));
		if(len<0)
			return len;
		if(len==0){
			break;
		}
		bzero(buf,sizeof(buf));
		recv_n(sockfd,buf,len);
		printf("%s",buf);
	}
	return 0;
}
	
int main(int argc,char* argv[]){
	//启动方法：./client "ip" "port"
	if(argc != 3){
		printf("args error\n");
		return -1;
	}
	int ret;
	int sockfd;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		perror("socket");
		return -1;
	}
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(atoi(argv[2]));
	ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(ret == -1){
		perror("connect");
		return -1;
	}
	char buf[128] = {0};
	int epfd = epoll_create(1);
	if(epfd == -1){
		perror("epoll_create");
		return -1;
	}
	struct epoll_event event,evs[2];
	event.events = EPOLLIN;
	event.data.fd = 0;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
	if(ret == -1){
		perror("epoll_ctl");
		return -1;
	}
	event.events = EPOLLIN;
	event.data.fd = sockfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
	int i;
	int ret1;
	int len;
	while(1){
		bzero(evs,sizeof(evs));
		ret1 = epoll_wait(epfd,evs,2,-1);
		for(i = 0;i < ret1;i++){
			if(evs[i].events == EPOLLIN && evs[i].data.fd == 0){
				bzero(buf,sizeof(buf));
				ret = read(0,buf,sizeof(buf));
				if(ret == -1){
					perror("read");
					return -1;
				}
				ret = send(sockfd,buf,strlen(buf) - 1,0);
				if(ret == -1){
					perror("send");
					return -1;
				}
				command_handle(sockfd,buf);
			}
			if(evs[i].events == EPOLLIN && evs[i].data.fd == sockfd){
				bzero(buf,sizeof(buf));
					len=recv_msg(sockfd);
					if(len)
						recv_file(sockfd,len);		
			}
		}
	}
	close(sockfd);
	return 0;
}
