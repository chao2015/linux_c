#4.socket编程

###a.TCP协议的socket编程

+ 头文件

```c
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
```

+ socket函数:生成一个套接口描述符

```c
int socket(int domain,int type,int protocol);
/*
domain{ AF_INET：Ipv4网络协议AF_INET6：IPv6网络协议}
type｛tcp：SOCK_STREAM   udp：SOCK_DGRAM}
protocol指定socket所使用的传输协议编号。通常为0.
返回值：成功则返回套接口描述符，失败返回-1。
*/
```

+ bind函数:绑定端口号和IP地址，使套接口与指定的端口号和IP地址关联

```c
int bind(int sockfd,struct sockaddr * my_addr,int addrlen);
/*
sockfd为前面socket的返回值。
my_addrà为结构体指针变量
struct sockaddr_in  //常用的结构体{
    unsigned short int sin_family;  //即为sa_family èAF_INET
    uint16_t sin_port;  //为使用的port编号
    struct in_addr sin_addr;  //为IP 地址
    unsigned char sin_zero[8];  //未使用
};

addrlenàsockaddr的结构体长度。通常是计算sizeof(struct sockaddr);
返回值：成功则返回0，失败返回-1
*/
```

+ listen函数:使服务器的这个端口和IP处于监听状态

```c
int listen(int sockfd,int backlog);
/*
sockfd为前面socket的返回值.即sfd
backlog指定同时能处理的最大连接要求，通常为10或者5。最大值可设至128
返回值：成功则返回0，失败返回-1
*/

```

+ accept函数：接受远程计算机的连接请求

```c
int accept(int s,struct sockaddr * addr,int * addrlen);
/*
s为前面socket的返回值.即sfd
addr为结构体指针变量，和bind的结构体是同种类型的，系统会把远程主机的信息（远程主机的地址和端口号信息）保存到这个指针所指的结构体中。
addrlen表示结构体的长度，为整型指针
返回值：成功则返回新的socket处理代码new_fd，失败返回-1
*/
```

+ recv函数：用新的套接字来接收传来的数据

```c
int recv(int sockfd,void *buf,int len,unsigned int flags);
/*
sockfd为前面accept的返回值.即new_fd，也就是新的套接字。
buf表示缓冲区
len表示缓冲区的长度
flags通常为0 
返回值：成功则返回实际接收到的字符数，可能会少于你所指定的接收长度。失败返回-1
*/
```

+ send函数：用新的套接字发送数据

```c
int send(int s,const void * msg,int len,unsigned int flags);
/*
s为前面accept的返回值.即new_fd
msg一般为常量字符串
len表示长度
flags通常为0
返回值：成功则返回实际传送出去的字符数，可能会少于你所指定的发送长度。失败返回-1
*/
```

+ connect函数：用来请求连接远程服务器

```c
int connect (int sockfd,struct sockaddr * serv_addr,int addrlen);
/*
sockfd为前面socket的返回值，即sfd
serv_addr为结构体指针变量，存储着远程服务器的IP与端口号信息。
addrlen表示结构体变量的长度
返回值：成功则返回0，失败返回-1
*/
```

+ close函数：关闭文件

```c
int close(int fd);
/*
fd为前面的sfd,new_fd
返回值：若文件顺利关闭则返回0，发生错误时返回-1
*/

```

###UDP协议编程

+ sendto函数&recvfrom函数

```c
int sendto(int sockfd, const void *msg,int len,unsigned int flags,const struct sockaddr *to, int tolen);

int recvfrom(int sockfd,void *buf,int len,unsigned int flags,struct sockaddr *from,int *fromlen);
```
