/*Socket : Day Time Client*/
#include<stdio.h>
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
char recvline[1024];
struct sockaddr_in servaddr;
len=sizeof(struct sockaddr_in);
/*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
sockfd=socket(AF_INET,SOCK_STREAM,0);
/*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr(argv[1]);//get ip from server
servaddr.sin_port=htons(atoi(argv[2])); // Get the port from the server
connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)); /*---- Connect the socket to the server using the address struct ----*/
n=read(sockfd,recvline,1024); /*---- Read the message from the server into the buffer ----*/
printf("%d\n",n);
recvline[n]=0;
printf("%s",recvline);
close(sockfd);
}

https://www.educative.io/edpresso/how-to-implement-tcp-sockets-in-c
