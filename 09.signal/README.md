#1.信号概念

+ 由进程的某个操作产生的信号称为**同步信号(synchronous signals)**,例如除0；
+ 由像用户击键这样的进程外部事件产生的信号叫做**异步信号(asynchronous signals)**。

+ 每个信号用一个整型常量宏表示，以**SIG**开头，比如SIGCHLD、SIGINT等。  
+ 在系统头文件**< signal.h >**中定义,  
+ 可以通过在shell下键入**kill –l**查看信号列表，  
+ 键入**man 7 signal**查看更详细的说明。 

>进程接收到信号以后，可以有如下3种选择进行处理：

+ **接收默认处理**：接收默认处理的进程通常会导致进程本身消亡。例如连接到终端的进程，用户按下CTRL+c，将导致内核向进程发送一个SIGINT的信号，进程如果不对该信号做特殊的处理，系统将采用默认的方式处理该信号，即终止进程的执行； signal(SIGINT,SIG_DFL)；
+ **忽略信号**：进程可以通过代码，显示地忽略某个信号的处理，例如：signal(SIGINT,SIG_IGN);但是某些信号是不能被忽略的,例如9号信号；
+ **捕捉信号并处理**：进程可以事先注册信号处理函数，当接收到信号时，由信号处理函数自动捕捉并且处理信号。

#2.signal信号处理机制

函数原型为：

```c
#include <signal.h>
typedef void (*sighandler_t)(int);  //函数指针
sighandler_t  signal(int signum, sighandler_t handler);
```

+ 第1个参数signum：表示要捕捉的信号；
+ 第2个参数：是个函数指针，表示要对该信号进行捕捉的函数；
+ signal如果调用成功，返回以前该信号的处理函数的地址，否则返回SIG_ERR。
+ sighandler_t是信号捕捉函数，由signal函数注册，注册以后，在整个进程运行过程中均有效，并且对不同的信号可以注册同一个信号捕捉函数。该函数只有一个整型参数，表示信号值。

EXAMPLE 1:    signal.c

```c
#include <signal.h>
#include <stdlib.h>

int main(){
    if(signal(SIGINT,SIG_IGN)==SIG_ERR){
        perror("signal");
        return -1; 
    }   
    while(1);
}        
```

EXAMPLE 2:    signal_set.c

```c
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void handle(int sig){
    printf("the sig is %d\n",sig);
    exit(0);
}

int main(){
    if(signal(SIGINT,handle)==SIG_ERR){
        perror("signal");
        return -1; 
    }   
    while(1);
    return 0;
}
```

EXAMPLE 3:   signal_sleep.c 

```c
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    if(signal(SIGINT,SIG_IGN)==SIG_ERR){
        perror("signal");
        return -1; 
    }   
    sleep(10);
    signal(SIGINT,SIG_DFL);
    while(1);
    return 0;
}
```

EXAMPLE 4:    signal_again.c

```c
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void handle(int sig){
    printf("before sleep,the sig is %d\n",sig);
    sleep(5);
    printf("after sleep,the sig is %d\n",sig);
}

int main(){
    if(signal(SIGINT,handle)==SIG_ERR){
        perror("signal");
        return -1; 
    }   
    while(1);
    return 0;
}     
```

EXAMPLE 5:    signal_two.c

```c
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void handle(int sig){
    printf("before sleep,the sig is %d\n",sig);
    sleep(5);
    printf("after sleep,the sig is %d\n",sig);
}

int main(){
    if(signal(SIGINT,handle)==SIG_ERR){
        perror("signal");
        return -1; 
    }   
    signal(SIGQUIT,handle);
    while(1);
    return 0;
}
```

EXAMPLE 6:    signal_read.c

```c
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void handle(int sig){
    printf("before sleep,the sig is %d\n",sig);
    sleep(5);
    printf("after sleep,the sig is %d\n",sig);
}

int main(){
    if(signal(SIGINT,handle)==SIG_ERR){
        perror("signal");
        return -1; 
    }   
    char buf[218] = {0};
    read(0,buf,sizeof(buf));
    return 0;
}
```

总结：
+ 注册一个信号处理函数，并且处理完毕一个信号之后，不需要重新注册，就能够捕捉下一个信号。
+ 如果信号处理函数正在处理信号，并且还没有处理完毕时，又发生了一个同类型的信号，这时挨着执行，后续相同信号忽略,会多执行一次。
+ 如果信号处理函数正在处理信号，并且还没有处理完毕时，又发生了一个不同类型的信号，这时跳转去执行另一个信号，之后再执行剩下的没有处理完的信号。
+ 如果程序阻塞在一个系统调用(如read(...))时，发生了一个信号，这时先跳转到信号处理函数，等信号处理完毕后，系统调用再返回。

#3.sigaction信号处理机制

函数原型:

```c
#include <signal.h>
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```

+ 参数signum：为需要捕捉的信号；
+ 参数act：是一个结构体，里面包含信号处理函数地址、处理方式等信息；
+ 参数oldact：是一个传出参数，sigaction函数调用成功后，oldact里面包含以前对signum的处理方式的信息，通常为NULL；
+ 如果函数调用成功，将返回0，否则返回-1。

结构体的原型：

```c
struct sigaction {
    void (*sa_handler)(int);//老类型的信号处理函数指针
    void (*sa_sigaction)(int, siginfo_t *, void *);//新类型的信号处理函数指针
    sigset_t sa_mask;//将要被阻塞的信号集合
    int sa_flags;//信号处理方式掩码 ( SA_SIGINFO )
    void (*sa_restorer)(void); //保留，不要使用
};
```

EXAMPLE 1:    sigaction.c

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle(int sig,siginfo_t* p,void* p1){
    printf("the sig is %d\n",sig);
}

int main(){
    int ret;
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_sigaction = handle;  
    act.sa_flags = SA_SIGINFO;
    ret = sigaction(SIGINT,&act,NULL);
    if(ret != 0){ 
        perror("sigaction");
        return -1; 
    }   
    while(1);
    return 0;
}
```

EXAMPLE 2:    sigaction_reset.c

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle(int sig,siginfo_t* p,void* p1){
    printf("the sig is %d\n",sig);
}

int main(){
    int ret;
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_sigaction = handle;  
    act.sa_flags = SA_SIGINFO|SA_RESETHAND;
    ret = sigaction(SIGINT,&act,NULL);
    if(ret != 0){ 
        perror("sigaction");
        return -1; 
    }   
    while(1);
    return 0;
}
```

EXAMPLE 3:    sigaction_restart.c

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void handle(int sig,siginfo_t* p,void* p1){
    printf("the sig is %d\n",sig);
}

int main(){
    int ret;
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_sigaction = handle;  
    act.sa_flags = SA_SIGINFO|SA_RESTART;
    ret = sigaction(SIGINT,&act,NULL);
    if(ret != 0){ 
        perror("sigaction");
        return -1; 
    }   
    char buf[128] = {0};
    ret = read(0,buf,sizeof(buf));
    printf("ret =  %d\n",ret);
    return 0;
}
```

EXAMPLE 4:    sigaction_nodefer.c

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void handle(int sig,siginfo_t* p,void* p1){
    printf("before sleep,the sig is %d\n",sig);
    sleep(5);
    printf("before sleep,the sig is %d\n",sig);
}

int main(){
    int ret;
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_sigaction = handle;  
    act.sa_flags = SA_SIGINFO|SA_NODEFER;
    ret = sigaction(SIGINT,&act,NULL);
    if(ret != 0){ 
        perror("sigaction");
        return -1; 
    }   
    while(1);
    return 0;
}
```

EXAMPLE 5:    sigaction_mask.c

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void handle(int sig,siginfo_t* p,void* p1){
    printf("before sleep,the sig is %d\n",sig);
    sleep(5);
    sigset_t s1; 
    sigpending(&s1);
    if(sigismember(&s1,SIGQUIT)){
        printf("the sigquit is here\n");
    }   
    printf("before sleep,the sig is %d\n",sig);
}

int main(){
    int ret;
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_sigaction = handle;  
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGQUIT);
    ret = sigaction(SIGINT,&act,NULL);
    if(ret != 0){ 
        perror("sigaction");
        return -1; 
    }   
    ret = sigaction(SIGQUIT,&act,NULL);
    if(ret != 0){ 
        perror("sigaction");
        return -1; 
    }   
    while(1);
    return 0;
}
```

EXAMPLE 6:    sigaction_siginfo.c

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle(int sig,siginfo_t* p,void* p1){
    printf("the sig is %d\n",sig);
    printf("the sending pid is %d\n",p->si_pid);
    printf("the sending uid is %d\n",p->si_uid);
}

int main(){
    int ret;
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_sigaction = handle;  
    act.sa_flags = SA_SIGINFO;
    ret = sigaction(SIGINT,&act,NULL);
    if(ret != 0){ 
        perror("sigaction");
        return -1; 
    }   
    while(1);
    return 0;
}
```

#4.sigprocmask信号阻塞

函数原型：

```c
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```

+ 参数how的值为如下3者之一：
	a：SIG_BLOCK ,将参数2的信号集合添加到进程原有的阻塞信号集合中
	b：SIG_UNBLOCK ,从进程原有的阻塞信号集合移除参数2中包含的信号
	c：SIG_SETMASK，重新设置进程的阻塞信号集为参数2的信号集
+ 参数set为阻塞信号集
+ 参数oldset是传出参数，存放进程原有的信号集，通常为NULL

EXAMPLE 1:sigprocmask.c

```c
#include <signal.h>
#include <stdio.h>

int main(){
    sigset_t s1; 
    sigemptyset(&s1);
    sigaddset(&s1,SIGINT);
    int ret;
    ret = sigprocmask(SIG_BLOCK,&s1,NULL);
    if(ret != 0){ 
        perror("sigprocmask");
        return 1;
    }   
    sleep(5);
    ret = sigprocmask(SIG_UNBLOCK,&s1,NULL);
    if(ret != 0){ 
        perror("sigprocmask");
        return 1;
    }   
    return 0;
}
```

EXAMPLE 2:sigprocmask_setmask.c

```c
#include <signal.h>
#include <stdio.h>

void handle(int sig){
    printf("sig is %d\n",sig);
}

int main(){
    signal(SIGINT,handle);
    sigset_t s1; 
    sigemptyset(&s1);
    sigaddset(&s1,SIGINT);
    int ret;
    ret = sigprocmask(SIG_BLOCK,&s1,NULL);
    if(ret != 0){ 
        perror("sigprocmask");
        return -1; 
    }   
    sleep(5);
    sigemptyset(&s1);
    sigaddset(&s1,SIGQUIT);
    ret = sigprocmask(SIG_SETMASK,&s1,NULL);
    if(ret != 0){ 
        perror("sigprocmask");
        return -1; 
    }   
    while(1);
    return 0;
}
```

#5.kill信号发送函数

函数原型：

```c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
```

+ 参数pid为将要接受信号的进程的pid，可以通过getpid()函数获得来给自身发送信号，还可以发送信号给指定的进程，此时pid有如下描述：
		pid > 0	将信号发给ID为pid的进程
		pid == 0	将信号发送给与发送进程属于同一个进程组的所有进程
		pid < 0	将信号发送给进程组ID等于pid绝对值的所有进程
		pid == -1	将信号发送给该进程有权限发送的系统里的所有进程
+ 参数sig为要发送的信号
+ 如果成功，返回0，否则为-1

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if(argc!=3){
        perror("error args\n");
        return -1; 
    }   
    int ret;
    ret = kill(atoi(argv[1]),atoi(argv[2]));
    if(ret != 0){ 
        perror("kill");
        return -1; 
    }   
    return 0;
}
```

#6.计时器

Linux为每个进程维护3个计时器，分别是真实计时器、虚拟计时器和实用计时器

+ 真实计时器计算的是程序运行的实际时间；——直接 
+ 虚拟计时器计算的是程序运行在用户态时所消耗的时间（可认为是实际时间减掉(系统调用和程序睡眠所消耗)的时间）---需要了解内核
+ 实用计时器计算的是程序处于用户态和处于内核态所消耗的时间之和。——常用


