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
#include "../inc/my_tcp.h"

#define PORT 5555
#define IP "120.77.40.75"

int main(void)
{
	char *buff=malloc(MAXLINE);
	bzero(buff,MAXLINE);
	memset(buff,80,20);
	printf("%s\n",buff);
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
	send(clientfd, buff, MAXLINE, 0);

	free(buff);
	close(clientfd);
	return 0;
}
