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
void dg_cli(FILE *fp,int sockfd,const struct sockaddr *pservaddr,socklen_t servlen)
{
int n;
char sendline[MAXLINE],recvline[MAXLINE+1];
while(fgets(sendline,MAXLINE,fp)!=NULL)
{
sendto(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
n=recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
recvline[n]=0;
/*null terminator*/
fputs(recvline,stdout);
}
}
int main(int argc,char **argv)
{
int sockfd;
struct sockaddr_in servaddr;
if(argc !=2)
{
fprintf(stderr,"usages udpcli <IP ADDRESS>");
return 1;
}
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
servaddr.sin_port=htons(SERVERPORT);
sockfd=socket(AF_INET,SOCK_DGRAM,0);
dg_cli(stdin,sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
exit(0);
}

