#include "pool.h"

void make_child(pchild p,int n){
	int fds[2];
	pid_t pid;
	int i;
	for(i=0;i<n;++i){
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		if(!(pid=fork())){
			close(fds[1]);
			child_handle(fds[0]);
		}else{
			close(fds[0]);
			p[i].pid=pid;
			p[i].fd=fds[1];
			p[i].busy=0;
		}
	}
}

void child_handle(int fdr){
	int new_fd;
	int flag=1;
	while(1){
		recv_fd(fdr,&new_fd);//接收new_fd,可以与客户端进行通信
		send_file(new_fd);//发送文件
		write(fdr,&flag,sizeof(int));//通知主进程已经完成文件发送
	}
}
