#include "pool.h"

void recv_fd(int recvfd,int* fd){
	struct msghdr msg;
	bzero(&msg,sizeof(msg));

	struct iovec iov;
	char buf[10]={0};
	iov.iov_base=buf;
	iov.iov_len=1;		
	msg.msg_iov=&iov;
	msg.msg_iovlen=1;
	
	struct cmsghdr* cmsg;
	int len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr*)malloc(len);
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	cmsg->cmsg_len=len;

	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	
	int ret;
	recvmsg(recvfd,&msg,0);
	if(-1==ret){
		perror("recvmsg");
		return;
	}
	*fd=*(int*)CMSG_DATA(cmsg);
}
