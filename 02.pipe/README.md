#Linux IPC 之管道

###1、标准流管道

>函数原形

```c
#include <stdio.h>
FILE* popen(const char* command, const char* open_mode);
int pclose(FILE* fp);
```

> Example1：从标准管道流中读取，打印/etc/profile的内容

```c
#include <stdio.h>
int main()
{
FILE* fp = popen("cat /etc/profile", "r");
    char buf[512] = {0};
    while(fgets(buf, sizeof(buf), fp))
    {
    puts(buf);
    }
    pclose(fp);
    return 0;
}
```

> Example2：写到标准管道流，统计buf单词数(被调用程序必须阻塞等待标准输入)
 
```c
#include<stdio.h>
int main()
{
char buf[]={"aaa  bbb  ccc  ddd  eee  fff  ggg  hhh"};
FILE *fp = popen("wc -w", "w");
fwrite(buf, sizeof(buf), 1, fp);
pclose(fp);
return 0;
}
```

###2、无名管道(PIPE)

>无名管道的特点：
 
+ 1.只能在亲缘关系进程间通信（父子或兄弟）
+ 2.半双工（固定的读端和固定的写端）
+ 3.他是特殊的文件，可以用read、write等，只能在内存中

>函数原形

```c
#include <unistd.h>
int pipe(int fds[2]);    
```

>Example1: 创建父子进程，创建无名管道，子写父读

```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    int fds[2];
    pipe(fds);//打开管道
    if(!fork())//子进程
    {
        close(fds[0]);//先关闭读端
        write(fds[1], "hello", 5);
        close(fds[1]);//写完之后关闭写端
        exit(0);
    }else{//父进程
        close(fds[1]);//先关闭写端
        char buf[10] = {0};
        read(fds[0], buf, sizeof(buf));
        printf("i am a parent. the buf is %s\n", buf);
        close(fds[0]);//读完之后关闭读端
        return 0;
    }
}
```

###3、命名管道(FIFO)

>创建FIFO文件的函数原型

```c
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode); 
```

>删除FIFO文件的函数原型

```c
#include <unistd.h>
int unlink(constchar *pathname);
```

>打开、关闭FIFO文件

使用open和close函数。

>读写FIFO

使用read和write函数。
