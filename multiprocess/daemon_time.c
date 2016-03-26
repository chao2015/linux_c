#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <time.h>

void Daemon(){
	const int MAXFD=64;
	int i=0;
	if(fork()!=0)
		exit(0);
	setsid();
	chdir("/");
	umask(0);
	for(;i<MAXFD;++i){
		close(i);
	}
}

int main(){
	int i=0;
	Daemon();
	while(i<10){
		printf("%d\n",i);
		time_t ttime;
		time(&ttime);
		struct tm *pTm=gmtime(&ttime);
		syslog(LOG_INFO,"%d %04d-%02d-%02d %02d:%02d:%02d",i
			,(1900+pTm->tm_year),(1+pTm->tm_mon),(pTm->tm_mday)
			,(pTm->tm_hour),(pTm->tm_min),(pTm->tm_sec));
			i++;
			sleep(2);
	}
}

//: vim /var/log/syslog
