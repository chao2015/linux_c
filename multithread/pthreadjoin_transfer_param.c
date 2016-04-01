#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void* pfunc(void* pt){
	printf("thread create success!\n");
	char *p=NULL;//不用全局变量时
	p=(char*)malloc(10);
	strcpy(p,"hello");
	return (void*)p;
}

int main(){
	pthread_t pthid;
	int ret;
	ret=pthread_create(&pthid,NULL,pfunc,NULL);
	if(ret!=0){
		printf("pthread_create failed,ret=%d\n",ret); 
		return -1;
	}
	printf("I am main thread\n");
	char* p;
	ret=pthread_join(pthid,(void**)&p);//注意非全局变量时的传参
	if(ret!=0){
		printf("pthread_join failed,ret=%d\n",ret);
		return -1;
	}
	printf("p is %s\n",p);
	return 0;
}
