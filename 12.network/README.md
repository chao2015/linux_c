#1.TCP/IP协议概述

###a.TCP/IP四层模型:

+ 应用层{http超文本传输协议 ftp文件传输协议 telnet远程登录ssh安全外壳协议stmp简单邮件发送 pop3收邮件}

+ 传输层{tcp传输控制协议,udp用户数据包协议}

+ 网络层{ip网际互联协议 icmp网络控制消息协议 igmp网络组管理协议}

+ 网络接口层{arp地址转换协议,rarp反向地址转换协议，mpls多协议标签交换}

###b.TCP/UDP协议：            

+ TCP协议：传输控制协议 面向连接的协议 能保证传输安全可靠 速度慢（有3次握手）

+ UDP协议：用户数据包协议 非面向连接  速度快 不可靠

通常是ip地址后面跟上端口号：ip用来定位主机 port区别应用（进程）

http的端口号80 ssh-->22 telnet-->23 ftp-->21  用户自己定义的通常要大于1024

###c.TCP的三次握手机制

+ 源主机A的TCP向主机B发送连接请求报文段，其首部中的SYN（同步）标志位应置为1，表示想跟目标主机B建立连接，进行通信，并发送一个同步序列号X（例：SEQ=100）进行同步，表明在后面传送数据时的第一个数据字节的序号为X+1（即101）。

+ 目标主机B的TCP收到连接请求报文段后，如同意，则发回确认。再确认报中应将ACK位和SYN位置为1.确认号为X+1，同时也为自己选择一个序号Y。

+ 源主机A的TCP收到目标主机B的确认后要想目标主机B给出确认。其ACK置为1，确认号为Y+1，而自己的序号为X+1。TCP的标准规定，SYN置1的报文段要消耗掉一个序号。

#2.网络相关概念

###a.套接口

+ 套接口，也叫“套接字”。是操作系统内核中的一个数据结构，它是网络中的节点进行相互通信的门户。它是网络进程的ID。 使用端口号和网络地址的组合可以唯一的确定整个网络中的一个网络进程。

+ 例如，如网络中某一台计算机的IP为10.92.20.160，操作系统分配给计算机中某一应用程序进程的端口号为1500，则此时 10.92.20.160  1500就构成了一个套接口。 

###b.端口号

+ 一是物理意义上的端口，如集线器、交换机、路由器等用于连接其他网络设备的接口。

+ 二是指TCP/IP协议中的端口，端口号的范围从0~65535，一类是由互联网指派名字和号码公司ICANN负责分配给一些常用的应用程序固定使用的“周知的端口”，其值一般为0~1023.

+ 例如http的端口号是80，ftp为21，ssh为22，telnet为23等。还有一类是用户自己定义的，通常是大于1024的整型值。 

###c.ip地址的表示

+ 通常用户在表达IP地址时采用的是点分十进制表示的数值（或者是为冒号分开的十进制Ipv6地址），而在通常使用的socket编程中使用的则是二进制值，这就需要将这两个数值进行转换。

+ ipv4地址：32bit, 4字节，通常采用点分十进制记法。

+ 例如对于：10000000 00001011 00000011 00011111

点分十进制表示为：128.11.3.31

#3.socket接口

###a.概念：

+ Linux中的网络编程是通过socket接口来进行的。socket是一种特殊的I/O接口，它也是一种文件描述符。它是一种常用的进程之间通信机制，通过它不仅能实现本地机器上的进程之间的通信，而且通过网络能够在不同机器上的进程之间进行通信。

+ 每一个socket都用一个半相关描述{协议、本地地址、本地端口}来表示；一个完整的套接字则用一个相关描述{协议、本地地址、本地端口、远程地址、远程端口}来表示。socket也有一个类似于打开文件的函数调用，该函数返回一个整型的socket描述符，随后的连接建立、数据传输等操作都是通过socket来实现的； 

###b.类型：

+ 流式socket（SOCK_STREAM）-> 用于TCP通信

+ 数据报socket（SOCK_DGRAM） ->用于UDP通信

+ 原始socket（SOCK_RAW） ->用于新的网络协议实现的测试等

###c.socket信息数据结构

```c

<netinet/in.h>//头文件

struct sockaddr{
   unsigned short sa_family;       /*地址族*/
   char sa_data[14];                   /*14字节的协议地址，包含该socket的IP地址和端口号。*/
};

struct sockaddr_in{
   short int sa_family;               /*地址族*/
   unsigned short int sin_port;    /*端口号*/
   struct in_addr sin_addr;          /*IP地址*/
   unsigned char sin_zero[8];      /*填充0 以保持与struct sockaddr同样大小*/
};

struct in_addr{
          unsigned long int  s_addr;     /* 32位IPv4地址，网络字节序 */
};

sa_family:AF_INET  //Pv4协议   
          AF_INET6  //IPv6协议
```

###d.数据存储优先顺序

对于内存中存放的数0x12345678来说

+ 如果是采用大端模式存放的，则其真实的数是：0x12345678

+ 如果是采用小端模式存放的，则其真实的数是：0x78563412

```c

unit16_t htons(unit16_t host16bit)//主机字节序转网络字节序

unit32_t htonl(unit32_t host32bit)//主机字节序转网络字节序

unit16_t ntohs(unit16_t host16bit)//网络字节序转主机字节序

unit32_t ntohs(unit32_t host32bit)//网络字节序转主机字节序

```

EXAMPLE 1:    htons.c

```c

#include <netinet/in.h>
                                                                                    
int main(){
    unsigned short i = 0x1234;
    unsigned short j;
    j = htons(i);
    return 0;
}
```

###e.名字地址转换

函数原型：

```c
#include <netdb.h>

#define  h_addr  h_addr_list[0]   /*保存的是ip地址*/

struct hostent* gethostbyname(const char* hostname);

struct hostent* gethostbyaddr(const char* addr, size_t len, int family);

struct hostent{
       char *h_name;        /*正式主机名*/
       char **h_aliases;    /*主机别名*/
       int h_addrtype;      /*主机IP地址类型 IPv4为AF_INET*/
       int h_length;        /*主机IP地址字节长度，对于IPv4是4字节，即32位*/
       char **h_addr_list;  /*主机的IP地址列表*/
}
```

EXAMPLE 1:    init_aton.c

```c
#include <sys/socket.h>                                                             
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){
    if(argc!=2){
        printf("error args\n");
        return -1; 
    }   
    struct in_addr addr;
    int ret;
    ret = inet_aton(argv[1],&addr);
    if(0 == ret){
        perror("inet_aton");
        return -1; 
    }   
    printf("addr = %x\n",addr.s_addr);
    
}
```

EXAMPLE 2:    gethostbyname.c

```c
#include <netdb.h>                                                                  
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char* argv[]){
    char *ptr,**pptr;
    struct hostent *ht;
    if(argc!=2){
        printf("error args\n");
        return -1; 
    }   
    if((ht =gethostbyname(argv[1]))==NULL){
        perror("gethostbyname");
        return -1; 
    }   
    printf("hname = %s\n",ht->h_name);
    for(pptr = ht->h_aliases;*pptr!=NULL;pptr++){
        printf("%s\n",*pptr);
    }   
    char buf[32] = {0};
    for(pptr = ht->h_addr_list;*pptr!=NULL;pptr++){
        printf("%s\n",inet_ntop(ht->h_addrtype,*pptr,buf,sizeof(buf)));
    }   
    return 0;
}
```