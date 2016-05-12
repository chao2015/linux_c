#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

int ticket=10;
sem_t lock;

void* salewindows1(void *args){
	while(1){
		sem_wait(&lock);
		if(ticket>0){
			sleep(1);
			printf("windows1 start sale ticket! the ticket is:%d\n",ticket);
			ticket--;
			sleep(2);
			printf("sale ticket finish! the last ticket is %d\n",ticket);
		}else{
			sem_post(&lock);
			pthread_exit(NULL);
		}
		sem_post(&lock);
		sleep(1);
	}
}

void* salewindows2(void *args){
	while(1){
		sem_wait(&lock);
		if(ticket>0){
			sleep(1);
			printf("windows2 start sale ticket! the ticket is:%d\n",ticket);
			ticket--;
			sleep(2);
			printf("sale ticket finish! the last ticket is %d\n",ticket);
		}else{
			sem_post(&lock);
			pthread_exit(NULL);
		}
		sem_post(&lock);
		sleep(1);
	}
}

int main(){
	pthread_t pthid1,pthid2;
	sem_init(&lock,0,1);
	pthread_create(&pthid1,NULL,salewindows1,NULL);
	pthread_create(&pthid2,NULL,salewindows2,NULL);
	pthread_join(pthid1,NULL);
	pthread_join(pthid2,NULL);
	sem_destroy(&lock);

	return 0;
}
