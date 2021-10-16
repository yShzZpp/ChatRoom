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
#define MAXLINE 1024
#define PORT 5555
#define MAXCLIENT_NUM 1  

void *clientIsFull(void *fd)
{
	int *clientfd=(int *)fd;
	printf("client%d is closing",*clientfd);
	send(*clientfd,FULLCLIENT,strlen(FULLCLIENT),0)	;
	close(*clientfd);
	return NULL;
}

Map map[MAXCLIENT_NUM];

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
	int clientNum=0; 
	  
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
					if(clientNum<MAXCLIENT_NUM){
						event.events=EPOLLIN|EPOLLET;
						event.data.fd=clientfd;
						if(epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&event)==-1){
							perror("new client epoll add error");
							exit(-1);
						}
						clientNum++;
						printf("%d accept client %02d :%s %d\n",clientfd,clientNum,inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));
					}
					
					/** 超过可连接数量	 */
					else if(clientNum ==MAXCLIENT_NUM){
						pthread_t pid;
						pthread_create(&pid,NULL,clientIsFull,(void*)&clientfd);
						/** pthread_detach(pid); */
					}
				}/** 可读事件 */
				else {  
					int fd=events[i].data.fd;
					ret=recv(fd,buff,MAXLINE,0);

					/** 读取错误 */
					if(ret==-1){
						perror("recv error");
						exit(-1);
					/** 断开连接 */

					}else if(ret==0){
						printf("%d is disconnect\n",fd);
						epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
						clientNum--;
						/** 查找与删除断开的用户 */
						continue;
					}

					printf("server recv:%s\n",buff);
					bzero(buff,MAXLINE); 
				}

			/** 可写事件 */
			}else if(events[i].events==EPOLLOUT){
				
			}

		}
	}

}


















