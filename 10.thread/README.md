#Linux 多线程

线程是进程的基本调度单元，每个进程至少都有一个main线程。它与同进程中的其他线程共享进程空间｛堆 代码 数据 文件描述符 信号等｝，只拥有自己的栈空间，大大减少了上下文切换的开销。  
第3方posix标准的pthread实现线程的访问和控制。编译的时候要在后面加上 –lpthread。   
增加man信息 ，apt-get install manpages-posix-dev。

| ---|创建|退出|等待|
|---------|:----:|:----:|:----:|
|多进程|fork()|exit()|wait()|
|多线程|pthread_create()|pthread_exit()|pthread_join()|

>函数原型

```c
#include <pthread.h>
int pthread_create(pthread_t* thread,  pthread_attr_t * attr,
                   void *(*start_routine)(void *),  void * arg);
void pthread_exit(void *retval);
int pthread_join(pthread_t th,  void **thread_return);
int pthread_cancel(pthread_t thread);    
```

>使用的常规步骤

```c
void* pthfunc()
pthread_t pthid;
pthread_create(&pthid,NULL,pthfunc,NULL);    //1、创建
pthread_create(&pthid,NULL,pthfunc,(void*)3);
pthread_exit(NULL);    //2、退出
pthread_exit((void*)3); 
int i; pthread_join(pthid,(void**)&i);    //3、wait
char* p; pthread_join(pthid,(void**)&p);
pthread_cancel(pthid);
```

其他操作1：线程终止清理函数（退出回调函数）

```c
void pthread_cleanup_push(void (*routine) (void *), void *arg) 
void pthread_cleanup_pop(int execute) 
```

>常规使用方式

```c
void cleanfunc()
pthread_cleanup_push(cleanfunc, (void*)1);
pthread_cleanup_pop(1);
```

其他操作2：互斥锁

```c
#include <pthread.h>
int pthread_mutex_init(pthread_mutex_t *mutex,  const pthread_mutexattr_t *mutexattr)
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

>常规使用方式

```c
pthread_mutex_t mutex;
int ret=pthread_mutex_init(&mutex,NULL);    //1、创建快速锁
pthread_mutex_lock(&mutex)    //2、加锁
pthread_mutex_unlock(&mutex)    //3、解锁
```

>锁操作

```c
int pthread_mutex_lock(pthread_mutex_t *mutex)    //加锁
int pthread_mutex_unlock(pthread_mutex_t *mutex)    //解锁
int pthread_mutex_trylock(pthread_mutex_t *mutex)    //测试加锁
```

###1.函数pthread_create：用来创建线程。

> * 返回值：成功返回0；失败返回-1。
> * 参数thread是传出参数，保存新线程的标识；
> * 参数attr是一个结构体指针，结构中的元素分别指定新线程的运行属性,attr可以用pthread_attr_init等函数设置各成员的值，但通常传入为NULL 即可；
> * 参数start_routine是一个函数指针，指向新线程的入口点函数，线程入口点函数带有一个void *的参数由pthread_create的第4个参数传入；
> * 参数arg用于传递给第3个参数指向的入口点函数的参数，可以为NULL，表示不传递。

###2.函数pthread_exit：表示线程的退出。

> * 其参数可以被其它线程用pthread_join函数捕获。

###3.函数pthread_join：是一个阻塞函数，一直等到参数th指定的线程返回。 

如果线程类型并不是自动清理资源类型的，则线程退出后，线程本身的资源必须通过其它线程调用pthread_join来清除,这相当于多进程程序中的waitpid。
> * 返回值：成功返回0；失败返回错误码。
> * 参数thread_return：是一个传出参数，接收线程函数的返回值。  
如果线程通过调用pthread_exit()终止，则pthread_exit()中的参数相当于自然返回值，可以被其它线程用pthread_join获取到。
> * 线程id传递0值时，join返回ESRCH错误。    

###4.函数pthread_cancel：

> * 返回值：成功返回0；失败返回错误码。
> * 子线程中有while时，无法cancel。

###5.线程终止清理函数pthread_cleanup_push()/pthread_cleanup_pop()

###6.互斥（互斥锁）与同步
