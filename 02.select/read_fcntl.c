#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <strings.h>

void fcntl_ch(int fd){
	int status;
	status=fcntl(fd,F_GETFL);
	status=status|O_NONBLOCK;
	fcntl(fd,F_SETFL,status);
}

int main(){
	char buf[128]={0};
	int ret,nread;
	fcntl_ch(0);	//stdin
	while(1){
		sleep(2);
		usleep(500000);	//0.5s
		bzero(buf,sizeof(buf));
		ret=read(0,buf,sizeof(buf));
		switch(ret){
		case 0:printf("keyboard done\n");exit(0);	//ctrl+D
		case -1:printf("time out\n");break;	//nonblock:read return -1
		default:printf("read %d from keyboard:%s",ret,buf);break;
		}
	}
	return 0;
}
