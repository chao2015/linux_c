#Linux IPC 之管道

###1、标准流管道

>函数原形

```c
#include <stdio.h>
FILE* popen(const char* command, const char* open_mode);
int pclose(FILE* fp);
```

###2、无名管道(PIPE)

>函数原形

```c
#include <unistd.h>
int pipe(int fds[2]);    
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
