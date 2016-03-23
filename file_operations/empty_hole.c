#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

//create a empty-hole-file in two ways
//lseek()+write() and ftruncate()

void lseek_EH(char *dir){
	char buf[128]="h";
	int lseek_ret, write_ret;
	int fd;
	
	fd=open(dir,O_RDWR|O_CREAT,0666);
	if(-1==fd){
	    perror("open");
		return;
	}
	printf("fd is %d\n",fd);
	
	lseek_ret=lseek(fd,1024,SEEK_CUR);
	printf("lseek_ret is %d\n",lseek_ret);
	
	write_ret=write(fd,buf,strlen(buf));
	printf("write_ret is %d\n",write_ret);
	
	close(fd);
}

void ftruncate_EH(char* dir){
	int fd, ret;
	fd=open(dir,O_WRONLY,0666);
	if(-1==fd){
		perror("open");
		return;
	}
	printf("fd is %d\n",fd);
	
	ret=ftruncate(fd,10);
	if(-1==ret){
		perror("ftruncate");
		return;
	}

	close(fd);
}

int main(int argc, char* argv[]){
	if(argc!=2){
		printf("error args\n");
		return -1;
	}

	//lseek_EH(argv[1]);
	ftruncate_EH(argv[1]);

	return 0;
}
