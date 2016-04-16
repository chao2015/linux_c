#1、定义数据结构：

+ 描述子进程的数据结构

```c
typedef struct{
    pid_t pid;    //子进程pid
    int fd;    //子进程fd
    int busy;    //判断子进程是否空闲可用的标志
}child,*pchild

pchild p=(pchild)malloc(num*sizeof(child));    //开辟num数量的子进程空间
```
+ 传输过程的数据结构

```c
typedef struct{
    int len;    //代表后面buf的实际发送长度，主要用于控制结束
    char buf[1000];    //控制每次发送长度
}
```

#2、创建子进程

+ 创建子进程，主进程与每一个子进程都有一条socketpair的管道，初始化所有子进程状态为非忙碌。

```c
void make_child(pchild p,int n){
	int fds[2];
	pid_t pid;
	int i;
	for(i=0;i<n;++i){
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);    //（1）socketpair
		if(!(pid=fork())){                    //（2）fork
			close(fds[1]);
			child_handle(fds[0]);        //（3）child_handle
		}else{
			close(fds[0]);
			p[i].pid=pid;            //（4）initialize：struct child
			p[i].fd=fds[1];
			p[i].busy=0;
		}
	}
}
```

+ 多个子进程来完成发送3动作：

```c
void child_handle(int fdr){
	int new_fd;
	int flag=1;
	while(1){
		recv_fd(fdr,&new_fd);    //(1)recv_fd：接收new_fd,可以与客户端进行通信
		send_file(new_fd);    //(2)send_file：发送文件
		write(fdr,&flag,sizeof(int));   //(3)write：通知主进程已经完成文件发送
	}
}
```

#3、服务器tcp-socket流程（+epoll）

+ epoll监控客户端请求，当接收到请求以后，将new_fd发送给非忙碌的子进程（epoll监控了和子进程写端(fds[0])对应的fds[1]）。

+ 子进程发送了数据给主进程，主进程读取，并标示子进程为非忙碌。

```c
/*args:1.ip;2.port;3.num:child numbers*/
sockfd=socket(AF_INET,SOCK_STREAM,0);    //(1)socket
        //(2)Initialize the struct sockaddr_in server;
bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr));    //(3)bind
listen(sockfd,num);                                        //(4)listen
epollfd=epoll_create(1);                    //(5)epoll_create
        //(6)Initialize the struct epoll_event ev;
epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);    //(7)epoll_ctl:循环注册
while(1){
//returns the number of file descriptors ready for the requested I/O
retep=epoll_wait(epollfd,EPOLLIN,1,-1);    //(8)epoll_wait
if(retep>0)
/*循环判断wait的描述符
  如果sockfd可读：*/
    new_fd=accept(sockfd,NULL,NULL);        //(9)accept:return a new_fd
    /*选用busy==0的进程;
      j=num时所有进程都忙碌.*/
    send_fd(p[j].fd ,new_fd)        //(10)send_fd发送给对应进程，并标志busy==1.
/*如果子进程描述符可读*/
    read(p[j].fd, &flag, sizeof(int));    /*(11)接受子进程write来的结束发送的通知，结束时设置该进程busy==0.*/
}
wait(NULL);
```

#4、主进程发送fd给子进程

```c
 /*args:1:子进程socketpair构造管道的另一端；2:int fd:new_fd=accept().*/
void send_fd(int sendfd,int fd)   
struct msghdr msg;        //(1)
bzero;
//initialize struct msg:
struct iovec iov;        //(2)or an array
//initialize struct iovec:
iov.iov_base=" ";    //starting address
iov,iov_len= ;    //number of bytes to transfer
msg.msg_iov=&iov;    //gather array
msg.msg_iovlen=1;    //elements in msg_iov
struct cmsghdr cmsg;        //(3)
//initialize struct cmsghdr
int len=CMSG_LEN(sizeof(int));
cmsg->cmsg_len=len;
cmsg->cmsg_level=SOL_SOCKET;    //originating protocol    
cmsg->cmsg_type=SCM_RIGHTS;    //protocol-specific type
*(int*)CMSG_DATA(cmsg)=fd;        //(4)
msg.msg_control=cmsg;
msg.msg_controllen=len;
/*sendmsg(3):
1:int sockfd;
2:struct msghdr* msg;
3:int flags.*/
sendmsg(sendfd,&msg,0);        //(5)sendmsg
```

#5、子进程接收new_fd,可以与客户端进行通信

```c
void recv_fd(int recvfd, int *fd)
struct msghdr msg;        //(1)
bzero;
//initialize struct msg:

/*struct iovec iov;    //or an array
char buf[10]={0};
//initialize struct iovec:
iov.iov_base=buf;    //starting address
iov,iov_len= ;    //number of bytes to transfer
msg.msg_iov=&iov;    //gather array
msg.msg_iovlen=1;    //elements in msg_iov
This is unnecessary different from sendmsg.*/

struct cmsghdr cmsg;        //(2)
//initialize struct cmsghdr
int len=CMSG_LEN(sizeof(int));
cmsg->cmsg_len=len;
cmsg->cmsg_level=SOL_SOCKET;    //originating protocol    
cmsg->cmsg_type=SCM_RIGHTS;    //protocol-specific type
msg.msg_control=cmsg;
msg.msg_controllen=len;
/*recvmsg(3):
1:int sockfd;
2:struct msghdr* msg;
3:int flags.*/
recvmsg(recvfd,&msg,0);        //(3)
*fd=*(int*)CMSG_DATA(cmsg);    //(4)
```

#6、发送文件
+ send_n：（1）发送filename给客户端；（2）发送file content； （3）发送结束标志。

```c
void send_file(int sockfd){
    data d;
    bzero;
    d.len=strlen(" ");
    strcpy(d.buf," ");
    send_n(sockfd,(char*)&d, 4+d.len);        //(1)

    int fd=open(" ", O_RDONLY);
    while(bzero(&d, sizeof(d)),(d.len=read(fd, d.buf, sizeof(d.buf)))>0){
        send_n(sockfd, &d, 4+d.len);        //(2)
    }

    int flag=0;
    send_n(sockfd, &flag, sizeof(int));    //(3)将flag传给子进程，子进程再write给主进程
    close(socket);
}
```

#7、控制发送与接收同步

```c
//server:
void send_n(int sockfd, char* p, int len)
int ret, total=0;
while(total<len){
    ret=send(sockfd, p+total, len-total, 0);    //(1)send
    total=total+ret;
}
//client:
void recv_n(int sockfd, char* p, int len)
int ret, total=0;
while(total<len){
    ret=recv(sockfd, p+total, len-total, 0);    //(2)recv
    total=total+ret;
}
```

#8、客户端（+之前的recv_n）

+ recv_n：（1）文件名长度；（2）文件名内容；（3）循环接收文件内容的长度和内容。

```c
sockfd=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in server;
bzero;
//and initialize
connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr));    //(1)connect
char buf[1024]={0};
recv_n(sockfd,(char*)&len,sizeof(int));    //(2)receive filiename.length
/*recv(4):
4:int flags:default:0.*/
recv_n(sockfd,buf,len);        //(3)receive filename
int fd=open(buf,O_CREAT|O_RDWR.0666);    //(4)open:use the filename create a file
while(1){
    recv_n(sockfd,(char*)&len,sizeof(int));    //(5)loop receive content.length
    if(len==0) break;    //control to end 
    bzero;
    recv_n(sockfd,buf,len);    //(6)loop receive content
    write(fd,buf,len);        //(7)write:写入文件
}
close;
```