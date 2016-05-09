#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define BUF_SIZE 16  //the largest of continuous production is 16
#define OVER (-1)

struct prodcons{
	int buf[BUF_SIZE];	//data
	pthread_mutex_t lock;	//mutex
	int readpos,writepos;	//read and write position
	pthread_cond_t notempty;	//not empty can read
	pthread_cond_t notfull;	//not full can write
}prodcons;

void init(struct prodcons *b){
	pthread_mutex_init(&b->lock,NULL);
	pthread_cond_init(&b->notempty,NULL);
	pthread_cond_init(&b->notfull,NULL);
	b->readpos=0;
	b->writepos=0;
}

void destroy(struct prodcons *b){
	pthread_mutex_destroy(&b->lock);
	pthread_cond_destroy(&b->notempty);
	pthread_cond_destroy(&b->notfull);
}

void put(struct prodcons *b,int data){
	pthread_mutex_lock(&b->lock);
	while((b->writepos+1)%BUF_SIZE==b->readpos){
		printf("wait for not full\n");	//4
		pthread_cond_wait(&b->notfull,&b->lock);	//2
	}
	b->buf[b->writepos]=data;
	b->writepos++;
	if(b->writepos >= BUF_SIZE)
		b->writepos=0;
	pthread_cond_signal(&b->notempty);	//1
	pthread_mutex_unlock(&b->lock);
}

void* producer(void* data){
	int n;
	for(n=0;n<=50;++n){
		sleep(1);
		printf("put-->%d\n",n);
		put(&prodcons,n);
	}
}

int get(struct prodcons *b){
	pthread_mutex_lock(&b->lock);
	while(b->writepos==b->readpos){
		printf("wait for not empty\n");	//3
		pthread_cond_wait(&b->notempty,&b->lock);	//1
	}
	int data=b->buf[b->readpos];
	b->readpos++;
	if(b->readpos>=BUF_SIZE)
		b->readpos=0;
	pthread_cond_signal(&b->notfull);	//2
	pthread_mutex_unlock(&b->lock);
	return data;
}

void* consumer(void* data){
	while(1){
		//sleep(1);	//3
		sleep(2);	//4
		int d=get(&prodcons);
		if(d==OVER)
			break;
		printf("%d-->get\n",d);
	}
	printf("consumer stopped!\n");
	pthread_exit(NULL);
}

int main(){
	pthread_t pthid1,pthid2;
	init(&prodcons);
	pthread_create(&pthid1,NULL,producer,NULL);
	pthread_create(&pthid2,NULL,consumer,NULL);
	pthread_join(pthid1,NULL);
	pthread_join(pthid2,NULL);
	destroy(&prodcons);
	return 0;
}
