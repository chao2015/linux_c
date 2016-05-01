#include "pool.h"

int main(int argc,char* argv[]){
	/*args:1.ip;2.port;3.child numbers*/
	if(argc!=4){
		printf("error args\n");
		return -1;
	}
	
	int num=atoi(argv[3]);
	pchild p=(pchild)malloc(num*sizeof(child));
	
	make_child(p,num);//
	
	int sockfd;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sockfd){
		perror("sockfd");
		return -1;
	}

	struct sockaddr_in server;
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	server.sin_addr.s_addr=inet_addr(argv[1]);
	
	int ret;
	bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr));
	if(-1==ret){
		perror("bind");
		return -1;
	}
	listen(sockfd,num);
	if(-1==ret){
		perror("listen");
		return -1;
	}
	int epollfd=epoll_create(1);
	struct epoll_event event,*evs;
	evs=(struct epoll_event*)malloc((num+1)*sizeof(struct epoll_event));;
	event.events=EPOLLIN;
	event.data.fd=sockfd;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&event);

	int i;
	for(i=0;i<num;++i){
		event.events=EPOLLIN;
		event.data.fd=p[i].fd;
		ret=epoll_ctl(epollfd,EPOLL_CTL_ADD,p[i].fd,&event);
		if(-1==ret){
			perror("epoll_ctl");
			return -1;
		}
	}

	int new_fd;
	int j,flag,retep;
	while(1){
		retep=epoll_wait(epollfd,evs,num+1,-1);//num+1:child num and sockfd
		if(retep>0){
			for(i=0;i<retep;++i){
				if(evs[i].events==EPOLLIN && evs[i].data.fd==sockfd){
					new_fd=accept(sockfd,NULL,NULL);
					printf("new_fd=%d\n",new_fd);
					for(j=0;j<num;++j){
						if(p[j].busy==0){
							break;
						}
					}
					if(j==num)
						continue;
					send_fd(p[j].fd,new_fd);//
					p[j].busy=1;
				}
				for(j=0;j<num;++j){
					if(evs[i].events==EPOLLIN && evs[i].data.fd==p[j].fd){
						ret=read(p[j].fd,&flag,sizeof(int));
						if(ret<0){
							perror("read");
							return -1;
						}
						p[j].busy=0;
					}
				}
			}
		}
	}
	wait(NULL);
	return 0;
}
