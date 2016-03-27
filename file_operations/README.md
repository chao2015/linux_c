#Linux文件操作

###1.文件的创建，打开与关闭
>函数原型
```c
#include <stdio.h> 
FILE *fopen(const char *path,const char *mode);  //文件名  模式
int fclose(FILE *stream);
```

###2.读写文件

#####2.1数据块读写
```c
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream);
```

#####2.2格式化读写
```c
#include <stdio.h>
int printf(const char *format, ...);      //相当于fprintf(stdout,format,…);
int scanf(const char *format, …);

int fprintf(FILE *stream, const char *format, ...); 
int fscanf(FILE *stream, const char *format, …);

int sprintf(char *str, const char *format, ...);  
int sscanf(char *str, const char *format, …); 
```

fprintf将格式化后的字符串写入到文件流stream中
sprintf将格式化后的字符串写入到字符串str中  

#####2.3单个字符读写
```c
#include <stdio.h>
int fgetc(FILE *stream);
int fputc(int c, FILE *stream);

int getc(FILE *stream);               //等同于 fgetc(FILE* stream)
int putc(int c, FILE *stream);        //等同于 fputc(int c, FILE* stream)

int getchar(void);                    //等同于 fgetc(stdin);
int putchar(int c);                   //等同于 fputc(int c, stdout)    
```
getchar和putchar从标准输入输出流中读写数据，其他函数从文件流stream中读写数据。

#####2.4字符串读写
```c
char *fgets(char *s, int size, FILE *stream);
int fputs(const char *s, FILE *stream);

char *gets(char *s);                       //等同于 fgets(const char *s, int size, stdin);
int puts(const char *s);                   //等同于 fputs(const char *s,stdout);
```
fgets读取一行字符时，保留行尾的换行符。  
fputs不会在行尾自动添加换行符。  
puts会在标准输出流中自动添加一换行符。  

#####2.5文件定位
```c
#include <stdio.h>
int feof(FILE * stream);      //通常的用法为while(!feof(fp))
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);  
void rewind(FILE *stream);    //等同于fseek(fp, 0, SEEK_SET);
```
feof判断是否到达文件末尾的下一个（注意到达文件末尾之后还会做一次）
fseek设置当前读写点到偏移whence 长度为offset处。
>whence可选项：  
SEEK_SET     (0：文件开头)  
SEEK_CUR    (1：文件当前位置 )  
SEEK_END    (2：文件末尾)  

ftell获取文件流当前的读写位置  
rewind将文件当前读写点移动到文件头

###3.目录操作

#####3.1改变目录或文件的访问权限
```c
#include <sys/stat.h>
int chmod(const char* path, mode_t mode);      //mode形如：0777
```

#####3.2获取、改变当前目录
```c
#include <unistd.h>   //头文件
char *getcwd(char *buf, size_t size);   //相当于pwd命令
int chdir(const char *path);                //相当于cd命令
```
常用的形式：getcwd(NULL, 0) ,自动malloc()配置内存。
   
#####3.3创建和删除目录
```c
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int mkdir(const char *pathname, mode_t mode);   //mode是目录权限
int rmdir(const char *pathname);      
```

#####3.4获取目录信息
```c
#include <sys/types.h>
#include <dirent.h>
DIR *opendir(const char *name);     //打开一个目录
struct dirent *readdir(DIR *dir);   //读取目录的一项信息，并返回该项信息的结构体指针
int closedir(DIR *dir);         

struct dirent
{
    ino_t  d_ino;             /* inode number（此目录进入点的inode） */
    off_t  d_off;             /* offset to the next dirent（目录开头到进入点的位移 */
    unsigned short d_reclen;  /* length of this record（目录名的长度） */
    unsigned char d_type;     /* type of file（所指的文件类型） */
    char   d_name[256];       /* filename（文件名） */
};

void rewinddir(DIR *dir);                   //重新定位到目录文件的头部
void seekdir(DIR *dir,off_t offset);        //用来设置目录流目前的读取位置
off_t telldir(DIR *dir);                    //返回目录流当前的读取位置
```

###4.基于文件描述符的操作

#####4.1打开、创建和关闭文件
```c
int open(const char *pathname, int flags);             //文件名  打开方式
int open(const char *pathname, int flags, mode_t mode);//文件名  打开方式  权限
int creat();                                   //不常用，等同于open(pathname,O_CREAT|O_TRUNC|O_WRONLY,mode)
int close(int fd);                             //fd表示文件描述词,是先前由open或creat创建文件时的返回值
```
>flags的可选项：

|掩码|含义|
|----------|-----------------|
|O_RDONLY|只读|
|O_WRONLY|只写|
|O_RDWR|读写|
|O_CREAT|如果不存在，则创建|
|O_EXCL|与O_CREAT连用，如果文件已存在，则open失败|
|O_TRUNC|如果文件存在，将长度截为0|
|O_APPEND|追加方式，每次调用write时，文件指针自动先移到文件尾，用于多进程写同一个文件的情况。|

#####4.2读写文件
>函数原型
```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);    //文件描述词  缓冲区  长度
ssize_t write(int fd, const void *buf, size_t count);
```

出错返回-1，读取完了之后，返回0， 其他情况返回读写的个数。    

#####4.3改变文件大小
>函数原型
```c
#include <unistd.h>
int ftruncate(int fd, off_t length);
```

执行成功则返回0，失败返回-1。 

#####4.4文件定位
>函数原型
```c
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);    
```

>whence的可选项  
SEEK_SET 从文件头开始计算  
SEEK_CUR 从当前指针开始计算  
SEEK_END 从文件尾开始计算  

利用该函数可以实现文件空洞，通常用于多进程间通信的时候的共享内存。


#####4.5获取文件信息
>函数原型
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int stat(const char *file_name, struct stat *buf);      //文件名  stat结构体指针
int fstat(int fd, struct stat *buf); 

    struct stat {
           dev_t         st_dev;      /*如果是设备，返回设备表述符，否则为0*/
           ino_t         st_ino;      /* i节点号 */
           mode_t        st_mode;     /* 文件类型 */  无符号短整型
           nlink_t       st_nlink;    /* 链接数 */
           uid_t         st_uid;      /* 属主ID */
           gid_t         st_gid;      /* 组ID */
           dev_t         st_rdev;     /* 设备类型*/
           off_t         st_size;     /* 文件大小，字节表示 */
           blksize_t     st_blksize;  /* 块大小*/
           blkcnt_t      st_blocks;   /* 块数 */
           time_t        st_atime;    /* 最后访问时间*/
           time_t        st_mtime;    /* 最后修改时间*/
           time_t        st_ctime;    /* 最后权限修改时间 */
};
```

#####4.6文件描述符的复制

>函数原型

```c
#include <unistd.h>  
int dup(int oldfd);    
int dup2(int oldfd, int newfd);
```
dup返回新的文件描述符（没有使用的文件描述符的最小编号），内核的文件打开引用计数+1。

#####4.7标准文件描述符
>对应的宏

标准输入文件描述符         0：STDIN_FILENO   
标准输出文件描述符         1：STDOUT_FILENO    
标准错误输出文件描述符  2：STDERR_FILENO   

>对应的流

0：stdin  
1：stdout  
2：stderr  
stdin具有可读属性，缺省情况下是指从键盘的读取输入；  
stdout和stderr具有可写属性，缺省情况下是指向屏幕输出数据。  

###5.I/O多路转接模型
>select()函数
```c
#include <sys/select.h>
#include <sys/time.h>
int select(int maxfd, fd_set *readset,fd_set *writeset, fd_set *exceptionset, 
	const struct timeval * timeout);
```

###6.mmap文件映射
>函数原型
```c
<sys/mman.h>
void* mmap(void* start,size_t length,int prot,int flags,int fd,off_t offset);
int munmap(void* start,size_t length);
```