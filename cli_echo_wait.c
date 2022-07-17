/*Socket : Echo Multiple client */
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<arpa/inet.h>
void str_cli(int sockfd);
int main(int argc, char *argv[])
{
int sockfd[10],n,conn,len;
int i;
char recvline[1024];

struct sockaddr_in servaddr;
len=sizeof(struct sockaddr_in);


servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr(argv[1]);//Server IP
servaddr.sin_port=htons(atoi(argv[2])); // Server Port

for(i=0;i<10;i++){ // Create and connect 10 sockets to same server
sockfd[i]=socket(AF_INET,SOCK_STREAM,0);
connect(sockfd[i],(struct sockaddr *)&servaddr,sizeof(servaddr));
}

str_cli(sockfd[0]);
exit(0);
}

void str_cli(int sockfd){

char sendline[1024], recvline[1024];
while(fgets(sendline,1024,stdin)!=NULL)
{
write(sockfd, sendline,strlen(sendline));
if(read(sockfd,recvline,1024)==0)
    perror("error");
    
    fputs(recvline, stdout);
    }
    
    }



