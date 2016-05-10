#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
struct node{
	int number;
	struct node *next;
}*head=NULL;

void cleanup(void *arg){
	printf("Cleanup handler of consumer thread\n");
	free(arg);
	pthread_mutex_unlock(&mutex);
}

void *func(void *arg){	//consumer
	struct node *p=NULL;
	pthread_cleanup_push(cleanup,p);
	pthread_mutex_lock(&mutex);
	while(1){
		while(head==NULL){
			printf("-->wait-unlock to product\n"); //-->son thread
			pthread_cond_wait(&cond,&mutex);	//
			printf("-->wait-lock to consume\n");
		}
		sleep(2);
		p=head;
		head=head->next;
		printf("-->Got %d from front of queue\n",p->number);
		free(p);
	}
	pthread_mutex_unlock(&mutex);
	pthread_cleanup_pop(0);
}

int main(){
	pthread_t pthid;
	int i;
	struct node *p;
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	pthread_create(&pthid,NULL,func,NULL);
	sleep(1);
	for(i=1;i<10;++i){	//producer
		p=(struct node*)malloc(sizeof(struct node));
		p->number=i;	
		printf("product:%d\n",p->number);
		
		pthread_mutex_lock(&mutex);
		p->next=head;
		head=p;
		pthread_cond_signal(&cond);	//
		sleep(2);
		printf("unlock before signal to wait-lock\n");
		sleep(2);
		pthread_mutex_unlock(&mutex);
		
		//pthread_cond_signal(&cond);  //normal run
		sleep(1);
	}
	while(head!=NULL);
	sleep(2);
	printf("Oh,NO!\nproducer(main thread) wanna kill the consumer.\n");
	pthread_cancel(pthid);
	pthread_join(pthid,NULL);
	printf("All done.\n");
	return 0;
}