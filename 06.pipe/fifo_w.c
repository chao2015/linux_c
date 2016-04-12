#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	char buf[128]="Hi, this is a fifo.";
	
	if(mkfifo("1.pip",0666)!=0){
		perror("mkfifo failed");
		return -1;
	}
	
	int fifo=open("1.pip",O_WRONLY);
	if(-1==fifo){
		perror("open fifo");
		return -1;
	}
	
	write(fifo,buf,sizeof(buf));
	
	close(fifo);
	
	return 0;
}
