#include "pool.h"

void send_fd(int sendfd,int fd){
	struct msghdr msg;
	bzero(&msg,sizeof(msg));
	
	struct iovec iov;
	iov.iov_base="1";
	iov.iov_len=1;		
	msg.msg_iov=&iov;
	msg.msg_iovlen=1;
	
	struct cmsghdr* cmsg;
	int len=CMSG_LEN(sizeof(int));//sendfd
	cmsg=(struct cmsghdr*)malloc(len);
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	cmsg->cmsg_len=len;

	*(int*)CMSG_DATA(cmsg)=fd;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret;
	sendmsg(sendfd,&msg,0);
	if(-1==ret){
		perror("sendmsg");
		return;
	}
}
