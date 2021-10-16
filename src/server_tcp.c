#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include <signal.h>
#include <sys/epoll.h>
#include <assert.h>
#include <pthread.h>
#include "../inc/cjson.h"
#include "../inc/my_tcp.h"

#define FULLCLIENT "sorry client if full,please wait"
#define PORT 5555
#define MAXCLIENT_NUM 10  

void showMap();

void *clientIsFull(void *fd)
{
	int *clientfd=(int *)fd;
	printf("client%d is closing\n",*clientfd);
	send(*clientfd,FULLCLIENT,strlen(FULLCLIENT),0)	;
	close(*clientfd);
	return NULL;
}

Map map[MAXCLIENT_NUM];
	
/** 客户端信息录入 */
void clientMsgWrite(int clientNum,char *buff)
{
	jsonFirstConnect_A(buff,&map[clientNum]);
	showMap();
}

/** 在用户表中找到空的表位置 */
int findEmptyMap()
{
	for(int i=0;i<MAXCLIENT_NUM;i++){
		if(map[i].fd==0)return i;
	}
	return -1;
}

/** 通过fd查询用户表的位置 */
int findClinetNum(int fd)
{
	for(int i=0;i<MAXCLIENT_NUM;i++){
		if(map[i].fd==fd){
			return i;
		}
	}
	printf("the fd %d is no finded\n",fd);
	return -1;
}

/** 通过用户的num删除该用户 */
void deleteClient(int clientNum)
{
	bzero(&map[clientNum],sizeof(Map));

}

/** 展示用户表 */
void showMap()
{ 
	printf("----------------------------------------------------------\n");
	printf("    fd       name     chatwho              ip          port\n");
	for(int i=0;i<MAXCLIENT_NUM;i++){
		if(map[i].fd==0)continue;
		printf("%02d  %02d %10s  %10s %15s %13d\n",i,map[i].fd,map[i].userName,map[i].charWithWho,map[i].ip,map[i].port);
	}
	printf("----------------------------------------------------------\n");
}

int main(void)
{
	int ret;
	char *buff=malloc(MAXLINE);
	bzero(buff,MAXLINE);
	socklen_t clientSize=0;
	int listenfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serverAddr,clientAddr;
	/** 设置客户端与服务端重复使用IP */
	int opt=1;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(PORT);

	if(bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))!=0){ 
		perror("bind error");
		exit(-1);
	}
	  

	if(listen(listenfd,MAXCLIENT_NUM)!=0){ 
		perror("listen error");
		exit(-1);
	}

	/** epoll设置 */
	  
	int epfd=epoll_create(1);
	struct epoll_event event,events[MAXCLIENT_NUM];
	bzero(&event,sizeof(event));
	bzero(events,sizeof(events));

	event.events=EPOLLIN;
	event.data.fd=listenfd;
	if (epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&event)==-1){ 	
		perror("epoll ctl error");
		exit(-1);
	}

	bzero(map,sizeof(map));
	printf("map size %ld\n",sizeof(map));
	showMap();
	int clientSum=0; 
	  
	while(1){
		int nEvents=epoll_wait(epfd,events,MAXCLIENT_NUM,-1);
		if(nEvents==-1){
			perror("epoll wait error");
			exit(-1);
		}	
		for(int i=0;i<nEvents;i++){
			/** 输入事件 */
			if(events[i].events==EPOLLIN){

				/** 连接事件 */
				if(events[i].data.fd==listenfd){
					int clientfd=accept(listenfd,(struct sockaddr*)&clientAddr,&clientSize);
					if(clientfd==-1){
						perror("accept error");
						continue;
					}
					/** 未超出可连接数量 */
					if(clientSum<MAXCLIENT_NUM){
						int emptyNum=findEmptyMap();
						map[emptyNum].fd=clientfd;
						/** strcpy(map[emptyNum].userName,"test1"); */
						/** strcpy(map[emptyNum].charWithWho,"test2"); */
						strcpy(map[emptyNum].ip,inet_ntoa(clientAddr.sin_addr));
						map[emptyNum].port=ntohs(clientAddr.sin_port);
						clientSum++;
						showMap();

						event.events=EPOLLIN|EPOLLET;
						event.data.ptr=(void*)&map[emptyNum];
						if(epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&event)==-1){
							perror("new client epoll add error");
							exit(-1);
						}
					}
					
					/** 超过可连接数量	 */
					else if(clientSum ==MAXCLIENT_NUM){
						pthread_t pid;
						pthread_create(&pid,NULL,clientIsFull,(void*)&clientfd);
						/** pthread_detach(pid); */
					}
				}/** 可读事件 */
				else {  
					Map *readMap=(Map*)events[i].data.ptr;
					int fd=readMap->fd;
					int clientNum=findClinetNum(fd);
					ret=recv(fd,buff,MAXLINE,0);

					/** 读取错误 */
					if(ret==-1){
						perror("recv error");
						exit(-1);
					/** 断开连接 */

					}else if(ret==0){
						printf("%d is disconnect\n",fd);
						epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
						clientSum--;
						/** 查找与删除断开的用户 */
						deleteClient(clientNum);
						showMap();
						continue;
					}
					/** 分析数据 */
					printf("server recv:%s\n",buff);
					int protocol=jsonProtocol(buff);
					switch(protocol){
						case PROTO_FIRST:clientMsgWrite(clientNum,buff);break;
					}
					bzero(buff,MAXLINE); 
				}

			/** 可写事件 */
			}else if(events[i].events==EPOLLOUT){
				
			}

		}
	}

}


















