#Linux 共享内存

>函数原型

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
key_t ftok(const char *pathname, int proj_id);
int shmget(key_t key, int size, int shmflg);
void *shmat(int shmid, const void *shmaddr, int shmflg);
int shmdt(const void *shmaddr);
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

>使用的常规步骤

```c
int shmid=shmget((key_t)1234,1<<10,IPC_CREAT|0600);    //1、创建共享内存(失败-1)
char* p=(char*)shmat(shmid,NULL,0);    //2、连接共享内存(失败-1)
```

>其他操作

```c
int ret=shmdt(p);    //断开连接(失败!=0)
struct shmid_ds ds;
int ret=shmctl(shmid,IPC_STAT,&ds);    //获取结构体信息(失败!=0)
shmctl(shmid, IPC_RMID, NULL);    //删除共享内存(失败-1)
```

###1.函数ftok：用于创建一个关键字，可以用该关键字关联一个共享内存段。

> * 如果调用成功，返回一关键字，否则返回-1。  

> * 参数pathname：为一个全路径文件名，并且该文件必须可访问。
> * 参数proj_id：通常传入一非0字符。

###2.函数shmget：用于创建或打开一共享内存段。  

> * 函数成功，则返回一个唯一的共享内存标识号（相当于进程号），失败返回-1。  

> * 参数key：是一个与共享内存段相关联关键字。  
key的值既可以用ftok函数产生，也可以是IPC_PRIVATE（用于创建一个只属于创建进程的共享内存，主要用于父子通信）,表示总是创建新的共享内存段；
> * 参数size：指定共享内存段的大小，以字节为单位；
> * 参数shmflg：是一掩码合成值，可以是访问权限值与(IPC_CREAT或IPC_EXCL)的合成。

###3.函数shmat：将共享内存段映射到进程空间的某一地址。  

> * 如果调用成功，返回映射后的进程空间的首地址，否则返回(char *)-1。  

> * 参数shmid：是共享内存段的标识  通常应该是shmget的成功返回值。
> * 参数shmaddr：是共享内存连接到当前进程中的地址位置。通常是NULL，表示让系统来选择共享内存出现的地址。
> * 参数shmflg：是一组位标识，通常为0即可。

###4.函数shmdt：用于将共享内存段与进程空间分离(断开连接)。

> * 函数成功返回0，失败时返回-1。  

> * 参数shmaddr：通常为shmat的成功返回值。

###5.函数shmctl：共享内存的控制函数，可以用来删除共享内存段。

> * 参数shmid：是共享内存段标识 通常应该是shmget的成功返回值。
> * 参数cmd：是对内存段的操作方式，可选IPC_STAT, IPC_SET, IPC_RMID。通常为IPC_RMID:删除共享内存段。
> * 参数buf：是表示共享内存段的shmid_ds信息结构体数据，通常为NULL。

> shmid_ds结构体

```c
struct shmid_ds {
    struct ipc_perm shm_perm;    /* Ownership and permissions */
    size_t          shm_segsz;   /* Size of segment (bytes) */
    time_t          shm_atime;   /* Last attach time */
    time_t          shm_dtime;   /* Last detach time */
    time_t          shm_ctime;   /* Last change time */
    pid_t           shm_cpid;    /* PID of creator */
    pid_t           shm_lpid;    /* PID of last shmat(2)/shmdt(2) */
    shmatt_t        shm_nattch;  /* No. of current attaches */
};
struct ipc_perm {
    key_t          __key;    /* Key supplied to shmget(2) */
    uid_t          uid;      /* Effective UID of owner */
    gid_t          gid;      /* Effective GID of owner */
    uid_t          cuid;     /* Effective UID of creator */
    gid_t          cgid;     /* Effective GID of creator */
    unsigned short mode;     /* Permissions + SHM_DEST and SHM_LOCKED flags */
    unsigned short __seq;    /* Sequence number */
};
```