#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(){
	char buf[128];
	
	int fifo=open("1.pip",O_RDONLY);
	if(-1==fifo){
		perror("fifo open");
		return -1;
	}
	
	if(read(fifo,buf,sizeof(buf))>0)
		puts(buf);
	
	close(fifo);
	unlink("1.pip");
	
	return 0;
}
