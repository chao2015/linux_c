#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>//malloc

char* p=NULL;//定义全局变量

void* pfunc(void* pt){
	printf("thread create success!\n");
	p=(char*)malloc(10);
	strcpy(p,"hello");//线程结束后，看线程中malloc的空间还在否
	return (void*)5;
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
	int i;
	ret=pthread_join(pthid,(void**)&i);
	if(ret!=0){
		printf("pthread_join failed,ret=%d\n",ret);
		return -1;
	}
	printf("i=%d\n",i);
	printf("p is %s\n",p);//主线程可以看到结束后的子线程的malloc，与多进程不同
	return 0;
}
