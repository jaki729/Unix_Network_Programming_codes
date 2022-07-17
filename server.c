/*Socket : Day Time Server*/
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
int main(int argc, char **argv)
{
int listenfd,connfd,len;

struct sockaddr_in servaddr,clientaddr;
char buff[1024];
time_t ticks;

len=sizeof(struct sockaddr_in);
listenfd=socket(AF_INET,SOCK_STREAM,0);   // /*---- Create the socket. The three arguments are: ----*/
/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */

 /*---- Configure settings of the server address struct ----*/
 /* Address family = Internet */
servaddr.sin_family=AF_INET;
/* Set port number, using htons function to use proper byte order */
servaddr.sin_addr.s_addr=htonl(INADDR_ANY); /* Set IP address to localhost or your any dynamic IP */
servaddr.sin_port=htons(0); /* Set port number, using htons function to use proper byte order */
bind(listenfd,(struct sockaddr *)&servaddr, sizeof(servaddr));  /*---- Bind the address struct to the socket ----*/
getsockname(listenfd, (struct sockaddr *)&servaddr, &len);
printf("After bind ephemeral port=%d\n",(int)ntohs(servaddr.sin_port));
listen(listenfd,5);  /*---- Listen on the socket, with 5 max connection requests queued ----*/
connfd=accept(listenfd, (struct sockaddr *)&clientaddr,&len); /*---- Accept call creates a new socket for the incoming connection ----*/


ticks=time(NULL); // please check time.h header file
snprintf(buff,sizeof(buff),"%s\r\n",ctime(&ticks));// please check time.h header file

/*---- Send message to the socket of the incoming connection ----*/
write(connfd,buff,strlen(buff));
write(connfd,"ITER",4);
close(connfd);
}
