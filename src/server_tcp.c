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
#include "../inc/cjson.h"

#define MAXLINE 1024
#define PORT 5555

int main(void)
{
	int ret;
	char *buff=malloc(MAXLINE);
	bzero(buff,MAXLINE);
	socklen_t clientSize;
	int listenfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serverAddr,clientAddr;
	/** 设置客户端与服务端重复使用IP */
	int opt=1;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(PORT);

	ret=bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret!=0)
		perror("bind error");
	ret=listen(listenfd,10);
	if(ret!=0)
		perror("listen error");
	/** epoll设置 */
	int epfd=epoll_create(1);
	struct epoll_event event,events[10];
	bzero(&event,sizeof(event));
	bzero(events,sizeof(events));

	event.events=EPOLLIN;
	event.data.fd=listenfd;
	if (epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&event)==-1)	
		perror("epoll ctl error");

	while(1){
		int nEvents=epoll_wait(epfd,events,10,-1);
		if(nEvents==-1)perror("epoll wait error");
		for(int i=0;i<nEvents;i++){
			if(events[i].events==EPOLLIN){
				if(events[i].data.fd==listenfd){
					int clientfd=accept(listenfd,(struct sockaddr*)&clientAddr,&clientSize);
					if(clientfd==-1)perror("accept error");
					printf("accept %s %d\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));
					event.events=EPOLLIN|EPOLLET;
					event.data.fd=clientfd;
					if(epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&event)==-1)perror("new client epoll add error");
				}else {  
					int fd=events[i].data.fd;
					recv(fd,buff,MAXLINE,0);
					printf("server recv:%s\n",buff);
					bzero(buff,MAXLINE); 
				}
			}
		}
	}

}


















