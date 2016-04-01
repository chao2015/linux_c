#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void* pfunc(void* p){
	printf("thread create success!\n");

	//let the child thread change transfer parameters
	char *s=(char*)malloc(10);
	memcpy(p,&s,sizeof(s));
	strcpy(s,"hello");
	
	sleep(1);
	pthread_exit(NULL);;
}

int main(){
	pthread_t pthid;
	int ret;
	char *p;
	
	//1.create func transfer
	ret=pthread_create(&pthid,NULL,pfunc,(void*)&p);
	if(ret!=0){
		printf("pthread_create failed,ret=%d\n",ret); 
		return -1;
	}
	printf("I am main thread\n");

	sleep(3);
	//show the result of transfered parameter
	printf("p is %s\n",p);
	return 0;
}
