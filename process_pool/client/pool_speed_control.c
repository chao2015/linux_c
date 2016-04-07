#include "pool.h"

int send_n(int sockfd,char* p,int len){
	int ret;
	int total=0;
	while(total<len){
		ret=send(sockfd,p+total,len-total,0);
		total=total+ret;
	}
	return 0;
}

int recv_n(int sfd,char* p,int len){
	int ret;
	int total=0;
	while(total<len){
		ret=recv(sfd,p+total,len-total,0);
		total=total+ret;
	}
	return 0;
}
