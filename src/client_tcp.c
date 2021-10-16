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
#include <time.h>
#include "../inc/cjson.h"
#include "../inc/my_tcp.h"

#define PORT 5555
#define MAXLINE 1024
/** #define IP "120.77.40.75" */
#define IP "172.28.255.15"

int main(void)
{
	char *buff=malloc(MAXLINE);
	User user;
	bzero(buff,MAXLINE);
	int clientfd=socket(AF_INET,SOCK_STREAM , IPPROTO_TCP); 
	struct sockaddr_in clientAddr;
	clientAddr.sin_port=htons(PORT);
	clientAddr.sin_family=AF_INET;
	inet_pton(AF_INET, IP, &clientAddr.sin_addr.s_addr);
	int ret=connect(clientfd,(struct sockaddr*) &clientAddr,sizeof(clientAddr));
	if(ret!=0){
		perror("connect error");
		exit(-1);
	}
	printf("who are you?\n");
	scanf("%s",user.userName);
	printf("who do you want to chat to?\n");
	scanf("%s",user.charWithWho);
	jsonFirstConnect_P(&user,buff);
	printf("%d\n",jsonProtocol(buff));
	if( send(clientfd, buff, MAXLINE, 0)== -1)perror("send error");

	while(1 ){
		printf("--------------------------------\n");
		scanf("%s",buff);
		jsonChat_P(&user,buff,buff);
		if( send(clientfd, buff, MAXLINE, 0)== -1)break;
		bzero(buff,strlen(buff));
		printf("--------------------------------\n");
		if(recv(clientfd,buff,MAXLINE,0)==-1)break;
		printf("server : %s\n",buff);
		bzero(buff,strlen(buff));
	}	
	free(buff);
	close(clientfd);
	return 0;
}
