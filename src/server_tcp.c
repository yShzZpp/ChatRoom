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
#include "../inc/my_tcp.h"
#define PORT 5555

int main(void)
{
	int ret;
	char *buff=malloc(MAXLINE);
	bzero(buff,MAXLINE);
	socklen_t clientSize;
	int listenfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serverAddr,clientAddr;
	int opt=1;
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(PORT);

	ret=bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret!=0)
		perror("bind error");
	ret=listen(listenfd,10);
	if(ret!=0)
		perror("listen error");

	while(1){
		int clientfd=accept(listenfd,(struct sockaddr*)&clientAddr,&clientSize);
		recv(clientfd,buff,MAXLINE,0);
		printf("%s %d\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));
		printf("server recv:%s\n",buff);
		bzero(buff,MAXLINE);
	}

}


















