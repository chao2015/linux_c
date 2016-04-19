#ifndef __QUE_H__
#define __QUE_H__
#include "head.h"

typedef struct nod{
	int fd;
	struct nod* next;
}node,*pnode;

typedef struct{
	pnode head,tail;
	int size;
}que,*pque;


void que_init(pque);
#endif
