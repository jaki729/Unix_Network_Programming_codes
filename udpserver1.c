/* TCP echo server */
// add headers
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#define PORT 33456
#define MAXLINE 200
void dg_echo(int listenfd,struct sockaddr *pcliaddr,socklen_t clilen)
{
    int n;
    socklen_t len;
    char buffer[MAXLINE];
    memset(buffer, '\0', sizeof(buffer));
    for(; ;){
       len=clilen;
       n=recvfrom(listenfd,buffer,MAXLINE,0,pcliaddr,&len);
     
       printf("Msg from client: %s\n", buffer);
     // Respond to client:
       sendto(listenfd,buffer,n,0,pcliaddr,len);
}
}
int main(int argc, char *argv[]){
   int listenfd,br;
   socklen_t clilen,len;
   struct sockaddr_in servaddr,cliaddr;
   len=sizeof(servaddr);
    if(argc!=2){
       fprintf(stderr,"Usage %s   <PORT>\n",argv[0]);
       return 1;
   }
   servaddr.sin_family=AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(atoi(argv[1]));
   listenfd=socket(AF_INET,SOCK_DGRAM,0);
   if(listenfd>0){
         fprintf(stderr,"Socket created success\n");
   }
   else{
      fprintf(stderr,"socket creat error\n");
      return 1;
  }
  br=bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  if(br==0){
       printf("bind success: with return value=%d\n",br);
   }
   else{
    printf("bind unsuccess: with return value=%d\n",br);
    printf("Retry different port\n");
    exit(2);
   }
        dg_echo(listenfd,(struct sockaddr*) &cliaddr,sizeof(cliaddr));
       return 0;
    }
        
