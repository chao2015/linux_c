#include "factory.h"

void factory_init(pfactory pf,pfunc child_handle,int num,int cap){
	pf->pth=(pthread_t*)malloc(num*sizeof(pthread_t));
	pf->func=child_handle;
	pf->pnum=num;
	que_init(&pf->q);
	pf->capability=cap;
	strcpy(pf->cwdbuf,getcwd(NULL,0));
	pthread_cond_init(&pf->cond,NULL);
	pthread_mutex_init(&pf->mutex,NULL);
	pf->flag=0;
}

void factory_start(pfactory pf){
	int i,ret;
	for(i=0;i<pf->pnum;++i){
		ret=pthread_create((pf->pth)+i,NULL,pf->func,(void*)pf);
		if(ret!=0){
			perror("pthread_create");
			exit(-1);
		}
	}
	pf->flag=1;
}


int assign(pfactory pf){
	int ret;
	pnode pn;
	pthread_mutex_lock(&pf->mutex);
	while(!q_get(&pf->q,&pn)){
		pthread_cond_wait(&pf->cond,&pf->mutex);
	}
	ret=pn->fd;
	free(pn);
	pthread_mutex_unlock(&pf->mutex);
	return ret;
}

void visit(pfactory pf,pnode pn){
	int size;
	pthread_mutex_lock(&pf->mutex);
	size=q_getlen(&pf->q);
	if(size==pf->capability){
	}else{
		q_add(&pf->q,pn);
	}
	pthread_mutex_unlock(&pf->mutex);
	pthread_cond_signal(&pf->cond);
}
