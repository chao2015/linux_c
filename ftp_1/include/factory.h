#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "que.h"

typedef void* (*pfunc)(void*);
typedef struct{
	pthread_t *pth;
	pfunc func;
	int pnum;	//pthread number
	que q;	//struct que
	int capability;	//waiting max
	char cwdbuf[100];
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	int flag;
	char pwd[1024];
}factory,*pfactory;

int assign(pfactory);
void visit(pfactory,pnode);

void factory_init(pfactory,pfunc,int,int);
void factory_start(pfactory);
int recv_n(int,char*,int);
void send_file(int,char*);
void recv_file(int);
void send_msg(int,char*);

void cd_func(char*,char*);
void ls_func(char*,char*);
void remove_func(char*);
void pwd_func(char*);
void quit_func(char*);
#endif
