#ifndef __MY_TCP_H
#define __MY_TCP_H
#define MAXLINE 1024

#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>


/*******************结构体***************/
/*
struct sockaddr {
				sa_family_t sa_family;
				char	sa_data[14];
		};
	@struct sockaddr_in {
		short int sin_family;					//Address family 
		unsigned short int sin_port;		//Port number 
		struct in_addr sin_addr;				//Internet address 
		unsigned char sin_zero[8];			//Same size as struct sockaddr 
	};
	$window:<winsock.h>定义:{
		@struct in_addr {  
	       union {    
	                 struct {      u_char s_b1,s_b2,s_b3,s_b4;    } S_un_b;    
	                 struct {      u_short s_w1,s_w2;    } S_un_w;    
	                 u_long S_addr;  
	                } S_un;
		};
		#define s_addr  S_un.S_addr			//can be used for most tcp & ip code 
		#define s_host  S_un.S_un_b.s_b2	//host on imp 
		#define s_net   S_un.S_un_b.s_b1	//network 
		#define s_imp   S_un.S_un_w.s_w2	//imp 
		#define s_impno S_un.S_un_b.s_b4	//imp # 
		#define s_lh    S_un.S_un_b.s_b3		//logical host 
	}
	$linux：{
		struct in_addr {
			in_addr_t s_addr; //32位
		};
	}
*/
/******************server******************/
/*
	@af:
		Address Family ip地址类型：
			AF_INET(IPv4) 
			AF_INET6(IPv6) 
		127.0.0.1 代表这本机localhost
	@type:
		连接类型：
			SOCK_STREAM（流格式套接字/面向连接的套接字）TCP
			SOCK_DGRAM（数据报套接字/无连接套接字）UDP
			SOCK_RAW 原始套接字允许对底层协议如IP、ICMP直接访问，主要用于新网络协议测试
			SOCK_SEQPACKET 长度固定、有序、可靠的面向连接报文传递
	@protocol
		传输协议：
			 IPPROTO_TCP
			 IPPTOTO_UDP
	return:文件描述符
		error：INVALID_SOCKET=-1
		success：>0 
		一般使用listenfd
*/
 // int socket(int af,int type,int protocol);

/*
	@sockfd:
		创建的套接字的文件描述符 一般为监听fd
	@addr:
		本地协议地址的结构体 sockaddr 的指针
		一般使用sockaddr_in强制转化为sockaddr结构体指针
	@addrlen:
		sockaddr结构体长度

	@struct sockaddr 可指向sockaddr_in且一个字符串拆成多个成员，更 用以使用
	return：
		0：success
		-1：error 并将错误写入errno
*/
// int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

/*
	@fd:
		sockfd
	@backlog:
		是在TCP连接中已经申请连接正在握手的套接字（处在未完成连接队列）的数量和已经完成握手处在连接状态的套接字（处在已完成连接队列）的数量之和的最大值。
	return:
		0：success
		-1：error 并将错误写入errno
*/
// int listen(int fd, int backlog);


/***
 	@sockfd:
 		监听socket描述符
 	@addr：
 		sockaddr 结构体指针，用于返回客户端的协议地址
 	@addrlen：
 		socklen_t类型的指针
 	return ：
 		返回一个描述字表示tcp连接，-1错误
 * ***/
// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/**
 * 将十进制网络IP转化为点分制IP
 * #include <arpa/inet.h>
 * 
 * @in:
 * 	in_addr结构体in
 * return ：
 * 	返回点分制IP的字符串首地址
 * 
 * 
 * 
 * **/
// char*inet_ntoa(struct in_addr in);
/******************client*****************/

/***
 * 用于填充sockaddr_in 内部的sin成员变量
	name:convert IPv4 and IPv6 addresses from text to binary form
	@family:
		Address Family ip地址类型：
			AF_INET(IPv4) 
			AF_INET6(IPv6) 
	@*strptr:
		字符串形式的IP地址
	@addrptr：
		void类型指针，用于的到存放大端十进制数据ip的地址
		一般为&(struct sockaddr_in).sin_addr或&(struct sockaddr_in).sin_addr.s_addr,因为其地址相同
	@return:
		成功返回1，输入错误为0,其他错误返回-1
 ***/
// int inet_pton(int family, const char *strptr, void *addrptr);

/***
	@s:
		socket
	@name:
		struct sockaddr_in 
	@namelen:
		结构体长度

***/
// int connect(SOCKET s, const struct sockaddr * name, int namelen);


/**********************both****************/

/***
 	@s:
 		待接收数据的套接字
 	@buf:
 		为存放数据的缓冲区地址
 	@len:
 		为缓冲区长度
	@flags:
		为特殊操作的标志位，一般写0即可
		
	
	MSG_DONTWAIT		仅本操作非阻塞 
	MSG_OOB　　　　	发送或接收带外数据
	MSG_PEEK　　		窥看外来消息		 
	MSG_WAITALL　　等待所有数据 

	@return:
		数据字节数
 ***/
// int recv( SOCKET s, char *buf, int len, int flags);

 /***
 	@s:
 		待接收数据的套接字
 	@buf:
 		为需要发送的数据的地址
 	@len:
 		为发送数据的长度
	@flags:
		为特殊操作的标志位，一般写0即可
		MSG_DONTROUTE	绕过路由表查找 
		MSG_DONTWAIT		仅本操作非阻塞 
		MSG_OOB　　　　	发送或接收带外数据
	@return:
		数据字节数
 ***/
// int send( SOCKET s,const char* buf,int len,int flags);


/****************************字节序序函数******************************/
//网络字节序是大端字节序(低地址存放更高位的字节)所以，对于字节序为小端的机器需要收发网络数据的场景，要对这些数据进行字节序转换。
 //字节序转换函数，常用的有四个：

//将32位整数由主机字节序转为网络字节序
// uint32_t htonl(uint32_t hostlong);

// 将16位整数由主机字节序转为网络字节序
// uint16_t htons(uint16_t hostshort);
	
// 将32位整数由网络字节序转为主机字节序
// uint32_t ntohl(uint32_t netlong);
	
// 将16位整数由网络字节序转为主机字节序
// uint16_t htons(uint16_t hostshort);


// 其他点分制与十进制转化：
// 将点分制转为大端十进制写入到addrptr中，实际输入为sockaddr_in.sin_addr的地址
// int inet_pton(int family, const char *strptr, void *addrptr);

// 将大端十进制转为小端点分制字符串
// char *inet_ntoa(struct in_addr in);
	

/*


@sockfd:
	套接字描述符
@level：
	级别
		1)SOL_SOCKET:通用套接字选项.
		2)IPPROTO_IP:IP选项.
		3)IPPROTO_TCP:TCP选项.
@optname:
	控制选项名
		选项名称　　　　　　　　说明　　　　　　　　　　　　数据类型
	========================================================
	　　　　　　　　　　　　SOL_SOCKET
	------------------------------------------------------------------------
	SO_BROADCAST					允许发送广播数据						int
	SO_DEBUG							允许调试									int
	SO_DONTROUTE					不查找路由								int
	SO_ERROR							获得套接字错误							int
	SO_KEEPALIVE					保持连接									int
	SO_LINGER						延迟关闭连接								struct linger
	SO_OOBINLINE					带外数据放入正常数据流			int
	SO_RCVBUF						接收缓冲区大小							int
	SO_SNDBUF						发送缓冲区大小							int
	SO_RCVLOWAT					接收缓冲区下限							int
	SO_SNDLOWAT					发送缓冲区下限							int
	SO_RCVTIMEO					接收超时									struct timeval
	SO_SNDTIMEO					发送超时									struct timeval
	SO_REUSERADDR				允许重用本地地址和端口			int
	SO_TYPE							获得套接字类型							int
	SO_BSDCOMPAT					与BSD系统兼容							int
	=================================================
	　　　　　　　　　　　　IPPROTO_IP
	------------------------------------------------------------------------
	IP_HDRINCL						在数据包中包含IP首部				int
	IP_OPTINOS						IP首部选项								int
	IP_TOS								服务类型									int
	IP_TTL								生存时间									int
	===============================================
	　　　　　　　　　　　　IPPRO_TCP
	------------------------------------------------------------------------
	TCP_MAXSEG						TCP最大数据段的大小				int
	TCP_NODELAY					不使用Nagle算法						int
	================================================
@optval:
	设置值的地址
@optlen:
	设置值的大小

return:
	0:成功
	-1:错误，并把错误写入errno
设置SO_RCVBUF 就是设定窗口大小，小于1152设定无效，
不设置默认为28960

*/
// int getsockopt(int sockfd, int level, int optname,void *optval, socklen_t *optlen);

// int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);





/*************************select**********************/
 #include <sys/select.h>
/*

@nfds：
	指定待测试的描述子个数
@readfds,writefds,exceptfds：
	指定了我们让内核测试读、写和异常条件的描述字的结构体指针

fd_set结构体大小为128byte

@timeout：内核等待指定的描述字中就绪的时间长度
return：失败-1 超时0 成功返回可执行数据数量
*/
// int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);


/*
@set：
	集合
作用：情况监视的集合
*/
// void FD_ZERO(fd_set *set);



/*
@fd：
	需要监视的文件描述符
@set：
	监视集合
*/
// void FD_SET(int fd, fd_set *set);

/*
@fd：
	需要判断的文件描述符
@set：
	监视集合
*/
// int  FD_ISSET(int fd, fd_set *set);

/*
@fd：
	需要清除的文件描述符
@set：
	监视集合
*/
// void FD_CLR(int fd, fd_set *set);

/*******************epoll************************/
#include <sys/epoll.h>
/*
typedef union epoll_data {
	void *ptr;
	int fd;
	uint32_t u32;
	uint64_t u64;
} epoll_data_t;

struct epoll_event {
	uint32_t     events;	// Epoll events 
	epoll_data_t data;	// User data variable 
};
*/


/*
@size：
	不起作用了，大于0就行
@flags:
	0:与create相同
	不为0的话只支持EPOLL_CLOEXEC：
		用于设置该描述符的close-on-exec(FD_CLOEXEC)标志。执行时关闭
return：
	0：成功
	-1：错误，并写入errno
*/
//int epoll_create(int size);
//int epoll_create1(int flags);

/*
@epfd:
	epoll专用描述符，由epoll_create创建
@op:
	EPOLL_CTL_ADD:在epfd中添加fd
	EPOLL_CTL_MOD:更改目标相关联的事件
	EPOLL_CTL_DEL:在epfd中删除fd，此时事件可为空
@fd:
	需要检测的文件描述符
@event:
	epool_event 结构体指针
	结构体中的evnet成员变量：
	EPOLLIN：可写
	EPOLLOUT：可读
	EPOLLRDHUP：套接字关闭或关闭一半的连接
	EPOLLPRI：可读紧急数据
	EPOLLERR：发生在相关联文件之间的错误，非必要不使用
	EPOLLHUP：文件被挂断时。非必要不使用
	EPOLLET：边沿触发
	EPOLLONESHOT：fd一次性使用，想要重新使用则使用EPOLL_CTL_MOD

return:
	0：成功
	-1：错误 并写入errno
*/
// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
//问题：为什么结构体中有了fd还需要传入fd

/*
@epfd:
	epoll专用描述符，由epoll_create创建
@events：
	epoll_event 结构体执政
@maxevents：
	最大事件总数
@timeout：
	超时时间：-1 一直阻塞;0：立即返回
return：
	大于0：可使用的文件数量
	等于0：超时
	-1：错误，并写入errno
*/
//int epoll_wait  (int epfd, struct epoll_event *events,int maxevents, int timeout);
//int epoll_pwait(int epfd, struct epoll_event *events,int maxevents, int timeout,const sigset_t *sigmask);

/***************************总结*************************/
/*
	边沿触发非阻塞：在高并发情况下，会丢失连接
	边沿触发阻塞：如果读取缓冲区过小且没有循环读取，例如发送10个字节，但缓冲区只有5,那么读取了5个字节后就不再读了，
		直到下次出现边沿触发，再把上次没读的信息读取。
		如果有循环读取的话，则会全部读取
	水平触发非阻塞：在fd可读可写时，会一直触发，直到将数据全部读取完
	水平触发阻塞：在fd可读可写时，会一直触发，直到将数据全部读取完
*/














