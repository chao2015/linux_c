#1.互斥：互斥锁

```c
#include <pthread.h>
//静态初始化
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//动态初始化
int pthread_mutex_init(pthread_mutex_t *mutex,  const pthread_mutexattr_t *mutexattr)
//注销
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

>锁操作

```c
int pthread_mutex_lock(pthread_mutex_t *mutex)    //加锁
int pthread_mutex_unlock(pthread_mutex_t *mutex)    //解锁
int pthread_mutex_trylock(pthread_mutex_t *mutex)    //测试加锁
```

>常规使用方式

```c
pthread_mutex_t mutex;
int ret=pthread_mutex_init(&mutex,NULL);    //1、创建快速锁
pthread_mutex_lock(&mutex)    //2、加锁
pthread_mutex_unlock(&mutex)    //3、解锁
```

#2.同步：条件变量

```c 
//静态初始化
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
//动态初始化
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
//注销
int pthread_cond_destroy(pthread_cond_t *cond);
```

+ 尽管POSIX标准中为条件变量定义了属性，但在Linux Threads中没有实现，因此cond_attr值通常为NULL，且被忽略。
+ 只有在没有线程在该条件变量上等待的时候能注销这个条件变量，否则返回EBUSY。因为Linux实现的条件变量没有分配什么资源，所以注销动作只包括检查是否有等待线程。

>等待和激发

```c
//无条件等待
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
//计时等待
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
//激活一个等待该条件的线程，存在多个等待线程时按入队顺序激活其中一个
pthread_cond_signal();	
//激活所有等待线程
pthread_cond_broadcast();	
```

>常规使用方式

```c
pthread_cond_t cond;
int ret=pthread_cond_init(&cond,NULL);    //1、创建条件变量
pthread_mutex_wait(&cond)    //2、等待
pthread_mutex_signal(&cond)    //3、激发1
pthread_cond_broadcast(&cond)	//4、激发2：广播
```

+ 超时是指:当前时间 + 多长时间超时。
+ 线程解开mutex指向的锁并被条件变量cond阻塞。其中计时等待方式表示经历abstime段时间后，
即使条件变量不满足，阻塞也被解除。无论哪种等待方式，都必须和一个互斥锁配合，
以防止多个线程同时请求pthread_cond_wait()（或pthread_cond_timedwait()，下同）的竞争条件（Race Condition）。
+ mutex互斥锁必须是普通锁（PTHREAD_MUTEX_TIMED_NP），且在调用pthread_cond_wait()前必须由本线程加锁（pthread_mutex_lock()），
而在更新条件等待队列以前，mutex保持锁定状态，并在线程挂起进入等待前解锁。
在条件满足从而离开pthread_cond_wait()之前，mutex将被重新加锁，
以与进入pthread_cond_wait()前的加锁动作对应。（也就是说在做pthread_cond_wait之前，
往往要用pthread_mutex_lock进行加锁，而调用pthread_cond_wait函数会将锁解开，
然后将线程挂起阻塞。直到条件被pthread_cond_signal激发，再将锁状态恢复为锁定状态，
最后再用pthread_mutex_unlock进行解锁）。

