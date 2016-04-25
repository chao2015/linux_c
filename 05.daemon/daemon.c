#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void Daemon(){
	const int MAXFD=64;
	int i=0;
	if(fork()!=0)
		exit(0);
	setsid();
	chdir("/");
	umask(0);
	for(;i<MAXFD;++i)
		close(i);
}

int main(){
	Daemon();
	while(1){
		sleep(1);
	}
	return 0;
}
