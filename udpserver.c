#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<signal.h>
#include<sys/wait.h>
#define SERVERPORT 33456
#define MAXLINE 1024
void dg_echo(int listenfd,struct sockaddr *pcliaddr,socklen_t clilen)
{
int n;
socklen_t len;
char buffer[MAXLINE];
memset(buffer,'\0',sizeof(buffer));
for(; ;){
  len=clilen;
  n=recvfrom(listenfd,buffer,MAXLINE,0,pcliaddr,&len);
   sendto(listenfd,buffer,n,0,pcliaddr,len);
  }
}
int main(int argc, char *argv)
{
int sockfd;
struct sockaddr_in servaddr,cliaddr;
sockfd=socket(AF_INET,SOCK_DGRAM,0);
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
servaddr.sin_port=htons(SERVERPORT);
bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
dg_echo(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
}
