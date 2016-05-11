#include <stdio.h>
#include <pthread.h>

int ticket=20;
pthread_mutex_t lock;

void* salewinds1(void* args){
	while(1){
		pthread_mutex_lock(&lock);//lock make winds can't sale negative ticket
		if(ticket>0){
			sleep(1);
			printf("windows1 start sale ticket!the ticket is:%d\n",ticket);
			sleep(2);
			ticket--;
			printf("w1 sale ticket finish!the last ticket is:%d\n",ticket);
		}else{
			pthread_mutex_unlock(&lock);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&lock);
		sleep(1);//sleep make salewinds2 can run 
	}
}

void* salewinds2(void* args){
	while(1){
		pthread_mutex_lock(&lock);
		if(ticket>0){
			sleep(1);
			printf("windows2 start sale ticket!the ticket is:%d\n",ticket);
			sleep(3);
			ticket--;
			printf("w2 sale ticket finish!the last ticket is:%d\n",ticket);
		}else{
			pthread_mutex_unlock(&lock);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}	
}

int main(){
	pthread_t pthid1=0;
	pthread_t pthid2=0;
	
	pthread_mutex_init(&lock,NULL);
	
	pthread_create(&pthid1,NULL,salewinds1,NULL);
	pthread_create(&pthid2,NULL,salewinds2,NULL);
	
	pthread_join(pthid1,NULL);
	pthread_join(pthid2,NULL);

	pthread_mutex_destroy(&lock);

	return 0;
}
