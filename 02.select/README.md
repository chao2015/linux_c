#I/O多路转接模型
 
> I/O多路转接模型：在这种模型下，如果请求的I/O操作阻塞，且它不是真正阻塞I/O，而是让其中的一个函数等待，在这期间，I/O还能进行其他操作。select()函数，就是属于这种模型。
 
> select函数原型与说明
 
```c
#include <sys/select.h>
#include <sys/time.h>
int select(int maxfd, fd_set *readset,fd_set *writeset, fd_set *exceptionset, const struct timeval * timeout);
```

+ 返回:就绪描述字的正数目，0——超时，-1——出错
 
+ 参数解释
 
|参数|说明|
|------|-----------------|
|maxfd：| 最大的文件描述符（其值应该为最大的文件描述符字 + 1）|
|readset： |内核读操作的描述符字集合|
|writeset：|内核写操作的描述符字集合|
|exceptionset：|内核异常操作的描述符字集合|
|timeout：|等待描述符就绪需要多少时间。NULL代表永远等下去，一个固定值代表等待固定时间，0代表根本不等待，检查描述字之后立即返回|
 
 
+ 其中readset、writeset、exceptionset都是fd_set集合。该集合的相关操作如下
 
```c
void FD_ZERO(fd_set *fdset);  /* 将所有fd清零 */
void FD_SET(int fd, fd_set *fdset);  /* 增加一个fd */
void FD_CLR(int fd, fd_set *fdset);  /* 删除一个fd */
int FD_ISSET(int fd, fd_set *fdset);  /* 判断一个fd是否有设置 */
```
 
> 一般来说，在使用select函数之前，首先要使用FD_ZERO和FD_SET来初始化文件描述符集，在使用select函数时，可循环使用FD_ISSET测试描述符集，在执行完对相关文件描述符之后，使用FD_CLR来清除描述符集。
 
+ 另外，select函数中的timeout是一个struct timeval类型的指针，该结构体如下
 
```c
struct timeval
{
long tv_sec; /* second */   //秒
long tv_usec;  /* microsecond */ //微秒
};
```
 