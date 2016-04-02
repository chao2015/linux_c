#include <pthread.h>
#include <stdio.h>

int ticket=10;
pthread_mutex_t lock;
pthread_cond_t cond;

void* salewinds1(void* args){
	while(1){
		pthread_mutex_lock(&lock);
		if(ticket>0){
			sleep(1);
			printf("win1 start sale ticket! the ticket is:%d\n",ticket);
			ticket--;
			sleep(2);
			printf("sale tikcet finish! ticket has:%d\n",ticket);
		}else{
			pthread_mutex_unlock(&lock);
			pthread_exit(NULL);
		}
		if(ticket==0)
			pthread_cond_signal(&cond);//break block
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
}

void* salewinds2(void* args){
	while(1){
		pthread_mutex_lock(&lock);
		if(ticket>0){
			sleep(1);
			printf("win2 start sale ticket! the ticket is:%d\n",ticket);
			ticket--;
			sleep(2);
			printf("sale tikcet finish! ticket has:%d\n",ticket);
		}else{
			pthread_mutex_unlock(&lock);
			pthread_exit(NULL);
		}
		if(ticket==0)
			pthread_cond_signal(&cond);//break block
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
}

void* setticket(void* args){
	pthread_mutex_lock(&lock);
	if(ticket>0)
		pthread_cond_wait(&cond,&lock);//block
	ticket=10;
	pthread_mutex_unlock(&lock);
	sleep(1);
	pthread_exit(NULL);
}

int main(){
	pthread_t pthid1,pthid2,pthid3;
	
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&cond,NULL);
	
	pthread_create(&pthid1,NULL,salewinds1,NULL);
	pthread_create(&pthid2,NULL,salewinds2,NULL);
	pthread_create(&pthid3,NULL,setticket,NULL);

	pthread_join(pthid1,NULL);
	pthread_join(pthid2,NULL);
	pthread_join(pthid3,NULL);
	
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	
	return 0;
}
