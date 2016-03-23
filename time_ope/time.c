#include <stdio.h>
#include <time.h>

int main(){
	long t;
	time(&t);
	printf("t is %ld\n",t);

	struct tm *p;
	p=gmtime(&t);
	printf("%d-%d-%d %d:%d:%d\n",p->tm_year,p->tm_mon,
		p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	return 0;
}
