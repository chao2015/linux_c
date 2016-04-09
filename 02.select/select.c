#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

int main(){
	char buf[128];
	fd_set inputs,testfds;
	struct timeval timeout;
	FD_ZERO(&inputs);
	FD_SET(0,&inputs);
	//timeout.tv_sec=2;//写在外面，第二次循环时timeout=0
	//timeout.tv_usec=500000;
	int ret,nread;
	while(1){
		timeout.tv_sec=2;
		timeout.tv_usec=500000;//0.5s
		testfds=inputs;
		ret=select(FD_SETSIZE,&testfds,NULL,NULL,&timeout);//FD_SETSIZE为可监控最大描述符1024
		switch(ret){
		case 0:printf("time out\n");break;
		case -1:perror("select");exit(1);
		default:
			if(FD_ISSET(0,&testfds)){
				/*通过由ioctl的第三个参数指向的整数返回当前接收缓冲区中的字
				节数，可用于文件，管道，套接口和终端。回车键也是一个字符*/
				ioctl(0,FIONREAD,&nread);
				if(nread==0){//ctrl+D 是EOF的意思，使nread=0
					printf("keyboard done\n");
					exit(0);
				}
				bzero(buf,sizeof(buf));
				nread=read(0,buf,nread);
				buf[nread]=0;
				printf("read %d from keyboard:%s",nread,buf);
			}
			break;
		}
	}
	return 0;
}
