#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
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
	int serverfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr serverAddr,clientAddr;
	
}

