#System V 信号量

>函数原型

```c
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
int semget(key_t key, int nsems, int flag);
int semctl(int semid, int semnum, int cmd, …);
int semop(int semid, struct sembuf *sops, size_t num_sops);
```

>信号量使用的常规步骤

```c
int semid=semget((key_t)1234, 1, IPC_CREAT|0600);    //1、创建信号量集合
int ret=semctl(semid ,0, SETVAL, 1);    			 //2、初始化
struct sembuf p,v;    								 //3、定义结构体并初始化(略)
semop(semid, &p, 1);    							 //4、p+1，v-1操作信号量
semop(semid, &v, 1);
```

>更改信号量属性的常规操作

```c
struct semid_ds ds;    				//1、定义结构体
ds.sem_perm.mode=0666;    			//2、重新赋值
da.sem_nsems=6;    					//注：信号量个数无法更改，但是不会返回失败值0
semctl(semid, 0, IPC_SET, &ds);     //3、设置新值
```

>设置信号量值的常规操作

```c
unsigned short array[5]={1,2,3,4,5};
semctl(semid, 0, SETALL, array);    //初始化设置信号量的值
semctl(semid, 2, GETVAL);    		//获取指定信号量(2)的值，无初始化时值为0
unsigned short array1[5]={0};
semctl(semid, 0, GETALL, array1);    //获取全部信号量的值
```


###1.函数semget：创建一个信号量集或访问一个已存在的信号量集。  

>参数nsems：指定需要使用的信号量数目——  
创建新集合必须制定nsems。引用现存的集合，nsems指定为0。  
参数flag：例如0600|IPC_CREAT|IPC_EXCL


###2.函数semctl：用来直接控制信号量信息。 
 
> 参数semnum为集合中信号量的编号，从0开始。一般取值为0，表示第一个也是唯一的一个信号量。  
参数cmd为执行的操作。通常为：   
> * IPC_RMID（立即删除信号集，唤醒所有被阻塞的进程）   
> * GETVAL（根据semun返回信号的值，从0开始，第一个信号量编号为0）    
> * SETVAL（根据semun设定信号的值，从0开始，第一个信号量编号为0）    
> * GETALL（获取所有信号量的值，第二个参数为0，将所有信号的值存入semun.array中）    
> * SETALL（将所有semun.array的值设定到信号集中，第二个参数为0）。    
>       参数…是一个union semun（需要由程序员自己定义），它至少包含以下几个成员：

```c
union semun{
    int val;                         /* Value for SETVAL */
    struct semid_ds *buf;     		 /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array;    		 /* Array for GETALL, SETALL */
};    //通常情况仅使用val，给val赋值为1
         
    struct semid_ds {
               struct ipc_perm sem_perm;  /* Ownership and permissions */
               time_t          sem_otime; /* Last semop time */
               time_t          sem_ctime; /* Last change time */
               unsigned long   sem_nsems; /* No. of semaphores in set */
           };
    struct ipc_perm {
               key_t          __key; /* Key supplied to semget(2) */
               uid_t          uid;	 /* Effective UID of owner */
               gid_t          gid;	 /* Effective GID of owner */
               uid_tcuid;  			 /* Effective UID of creator */
               gid_t          cgid;	 /* Effective GID of creator */
               unsigned short mode;  /* Permissions */
               unsigned short __seq; /* Sequence number */
           };
```


###3.函数semop用于改变信号量对象中各个信号量的状态。
  
>参数semid：由semget返回的信号量标识符。   
参数sops：指向一个结构体数组的指针sembuf。  
参数nops：信号操作结构的数量，恒大于或等于1。

```c
struct sembuf {
    unsigned short sem_num; /* semaphore number：0 */
    short sem_op; 			/* semaphore operation：P V*/
    short sem_flg; 			/* operation flags： *SEM_UNDO/
};
```



