#include "factory.h"

void que_init(pque pq){
	pq->head=NULL;
	pq->tail=NULL;
	pq->size=0;
}

void q_add(pque q,pnode pn){
	if(0==q->size){
		q->head=pn;
		q->tail=pn;
	}else{
		q->tail->next=pn;
		q->tail=pn;
	}
	q->size++;
}

int q_get(pque q,pnode *pn){
	if(0==q->size){
		return 0;
	}else if(1==q->size){
		*pn=q->head;
		q->head=NULL;
		q->tail=NULL;
	}else{
		*pn=q->head;
		q->head=q->head->next;
	}
	q->size--;
	return 1;
}

int q_getlen(pque q){
	return q->size;
}
