#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
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
#define IP "120.77.40.75"
/** #define IP "172.28.255.15" */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *getServerMsg(void* arg)
{

	char *buff=malloc(MAXLINE);
	int fd=*(int *)arg,ret=0;
	fd_set readSet ;
	FD_ZERO(&readSet);
	FD_SET(fd,&readSet);

	while(1){
		ret=select(fd+1,&readSet,NULL,NULL,NULL);
		if(ret<0){ 
			perror("select error"); 
			pthread_cancel(pthread_self());
			printf("\n");
		}
		if(FD_ISSET(fd,&readSet)){
			pthread_mutex_lock(&mutex);
			bzero(buff,strlen(buff));
			if(recv(fd,buff,MAXLINE,0)==-1){
				perror("recv error"); 
				pthread_cancel(pthread_self());
				printf("\n");
			}
			/** jsonFromWho_A(buff); */
			pthread_mutex_unlock(&mutex);
		}
	}
	return NULL;
}

int main(void)
{
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
	char buff[1024];

	recv(clientfd,buff,MAXLINE,0);
	printf("recv :%s\n",buff);
	
}

int main2(void)
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
	/** 用户名 */
	printf("who are you?\n");
	scanf("%s",user.userName);
	printf("who do you want to chat with?\n");
	scanf("%s",user.charWithWho);

	/** jsonFirstConnect_P(&user,buff); */
	if( send(clientfd, buff, MAXLINE, 0)== -1)perror("send error");

	pthread_t pid;
	pthread_create(&pid, NULL, getServerMsg, (void*)&clientfd);

	while(1 ){
		printf("--------------------------------\n");
		scanf("%s",buff);
		/** jsonChat_P(&user,buff,buff); */
		if( send(clientfd, buff, MAXLINE, 0)== -1)break;
		bzero(buff,strlen(buff));
	}	
	free(buff);
	close(clientfd);
	return 0;
}
