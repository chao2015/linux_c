#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

//open(), read()

int main(int argc, char* argv[]){
	if(argc!=2){
		printf("error args\n");
		return -1;
	}
	
	int fd;
	fd=open(argv[1],O_RDONLY|O_CREAT,0666);
	if(-1==fd){
		perror("open");
		return -1;
	}
	printf("fd is %d\n",fd);
	
	char buf[128]={0};
	int ret;
	while(bzero(buf,sizeof(buf)),ret=read(fd,buf,sizeof(buf)-1)>0){
		printf("%s",buf);
	}
	printf("\n");
	
	close(fd);
	return 0;
}
