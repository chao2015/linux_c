#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	int semid;
	semid=semget((key_t)1234,5,IPC_CREAT|0600);
	if(semid<0){
		perror("semget");
		return -1;
	}
	
	unsigned short array[5]={1,2,3,4,5};
	int ret;
	ret=semctl(semid,0,SETALL,array); //初始化设置信号量的值
	if(ret!=0){
		perror("semctl1");
		return -1;
	}

	int value;
	value=semctl(semid,2,GETVAL);	//获取指定信号量(2)的值，无初始化默认为0
	if(-1==value){
		perror("semctl2");
		return -1;
	}
	printf("value=%d\n",value);

	unsigned short array1[5]={0};
	ret=semctl(semid,0,GETALL,array1);	//获取全部信号量的值	
	if(ret!=0){
		perror("semctl3");
		return -1;
	}
	printf("array1[4]=%d\n",array1[4]);

	return 0;
}
