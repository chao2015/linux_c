#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

//mmap() replace read() and write() to operate big files.
//eg. convert the case.

void mmap_holy(char* dir){
	int fp;
	char* p;
	fp=open(dir,O_RDWR);
	if(-1==fp){
		perror("open failed!");
		return;
	}
	p=(char*)mmap(0,4351658,PROT_READ|PROT_WRITE,MAP_SHARED,fp,0);
	if(p==(char*)-1){
		perror("mmap failed!");
		return;
	}
	
	int i=0;
	while(p[i]!='\0'){
		if(islower(p[i]))   
		//if(isupper(p[i]))
			p[i]^=32;
		++i;
	}

	munmap(p,4351658);
}

int main(int argc, char* argv[]){
	if(argc!=2){
		printf("no file open!");
		return -1;
	}
	mmap_holy(argv[1]);
	return 0;
}
