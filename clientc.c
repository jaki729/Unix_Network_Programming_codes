/*Socket : Day Time Client*/
#include<stdio.h>
#include <unistd.h> 
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<arpa/inet.h>
	int main(int argc, char *argv[])
	{
		int sockfd,n,conn,len;
		//int len;
		char recvline[1024];
struct sockaddr_in servaddr;
len=sizeof(struct sockaddr_in);
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);//get ip from server
	servaddr.sin_port=htons(atoi(argv[2])); // Get the port from the server
	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	n=read(sockfd,recvline,1024);
	printf("%d\n",n);
	recvline[n]=0;
printf("%s",recvline);
close(sockfd);
}
