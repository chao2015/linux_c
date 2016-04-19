#include "factory.h"

void* child_handle(void* p){
	pfactory pf=(pfactory)p;
	int newfd,ret;
	char comd[128];	//存储命令
	char argc[128];	//存储命令中的参数
	//初始化sendfile的绝对路径（信息传递的中间文件）
	char sendfile[64]={0};
	memcpy(sendfile,pf->cwdbuf,sizeof(pf->cwdbuf));
	sprintf(sendfile,"%s/%s",pf->cwdbuf,"sendfile");
	//循环等待客户端visit进来
	while(1){
		newfd=assign(pf);	//获取当前连接客户端线程的fd
		printf("newfd=%d\n",newfd);
		uid_t uid;
		//循环等待客户端发送来的指令
		while(1){
			bzero(comd,sizeof(comd));
			ret=recv(newfd,comd,sizeof(comd),0);
			if(ret>0){
				bzero(argc,sizeof(argc));
				if(strncmp(comd,"cd",2)==0){
					strncpy(argc,comd+3,strlen(comd)-2);
					cd_func(argc,sendfile);	//-->func.c
					send_msg(newfd,sendfile);	//-->send_msg.c
					//注意：线程结束时要维护当前工作目录,写在quit函数里
				}
				else if(strncmp(comd,"ls",2)==0){
					strncpy(argc,comd+3,strlen(comd)-2);
					ls_func(argc,sendfile);	//-->func.c
					send_msg(newfd,sendfile);
				}
				else if(strncmp(comd,"puts",4)==0){
					recv_file(newfd);	//-->recv_file.c
				}
				else if(strncmp(comd,"gets",4)==0){
					strncpy(argc,comd+5,strlen(comd)-4);
					send_file(newfd,argc);	//-->send_file.c
				}
				else if(strncmp(comd,"remove",6)==0){
					strncpy(argc,comd+7,strlen(comd)-6);
					remove_func(argc);	//-->func.c
				}
				else if(strncmp(comd,"pwd",3)==0){
					pwd_func(sendfile);	//-->func.c
					send_msg(newfd,sendfile);
				}
				else if(strncmp(comd,"quit",4)==0){
					quit_func(pf->cwdbuf);	//-->func.c
				}
			}
		}
	}
}

int main(int argc,char* argv[]){
	/*参数：ip port num capability*/
	//启动方法:./server ../conf/server.conf
	//读取配置文件的参数信息
	char confbuf[128]={0};
	int i;
	FILE* fin;
	char arg[4][32];
	fin=fopen(argv[1],"r");
	if(!fin){
		perror("open server.conf");
		return -1;
	}
	i=0;
	while(bzero(confbuf,sizeof(confbuf)),fgets(confbuf,sizeof(confbuf),fin)!=NULL){
		strcpy(arg[i],confbuf);
		switch(i){
		case 0:printf("IP:%s",arg[i]);break;
		case 1:printf("Port:%s",arg[i]);break;
		case 2:printf("threads num:%s",arg[i]);break;
		case 3:printf("capability:%s",arg[i]);break;
		default:break;
		}
		i++;
	}
	fclose(fin);
	printf("Server Start!\n--------------------------\n");
	//初始化工厂结构体，并启动线程
	int num=atoi(arg[2]);
	int cap=atoi(arg[3]);
	factory f;
	factory_init(&f,child_handle,num,cap);	//-->factory.c
	factory_start(&f);	//-->factory.c
	//SOCKET
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sockfd){
		perror("socket");
		return -1;
	}
	struct sockaddr_in server, client;
	int socklen=sizeof(struct sockaddr);
	bzero(&server,sizeof(struct sockaddr));
	bzero(&client,sizeof(struct sockaddr));
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(arg[1]));
	server.sin_addr.s_addr=inet_addr(arg[0]);
	//验证传参是否正确:
	//printf("%s,%d\n",inet_ntoa(server.sin_addr),ntohs(server.sin_port));
	int ret;
	ret=bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr));
	if(-1==ret){
		perror("bind");
		return -1;
	}
	ret=listen(sockfd,num);
	if(-1==ret){
		perror("listen");
		return -1;
	}
	//EPOLL
	struct epoll_event ev;
	int epollfd,epollret;
	epollfd=epoll_create(1);
	if(-1==epollfd){
		perror("epoll_create");
		return -1;
	}
	ev.events=EPOLLIN;
	ev.data.fd=sockfd;
	ret=epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);
	if(-1==ret){
		perror("epoll_ctl_add_1");
		return -1;
	}
	char buf[1024];
	pnode pn;
	while(1){
		epollret=epoll_wait(epollfd,&ev,1,-1);
		if(epollret>0){
			if(ev.events==EPOLLIN && ev.data.fd==sockfd){
				pn=(pnode)malloc(sizeof(node));
				pn->fd=accept(sockfd,(struct sockaddr*)&client,&socklen);
				if(-1==pn->fd){
					perror("accept");
					return -1;
				}
				printf("Client-(IP %s)-(Port %d):Connected success!\n",
					inet_ntoa(client.sin_addr),ntohs(client.sin_port));
				pn->next=NULL;
				visit(&f,pn);//
			}
		}
	}
	return 0;
}
