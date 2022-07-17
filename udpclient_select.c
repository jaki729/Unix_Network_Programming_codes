/*udP client echo */
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#define PORT 33456
#define MAXLINE 200
void handle_fun(int signo)
{
	printf("Signal Raised=%d\n",signo);
	printf("Alarm raised up: Recieved failed\n");
}
void dg_cli(FILE *fp, int sockfd,const struct sockaddr *pservaddr, socklen_t servlen)
{
        int n,maxfd,ns;
        fd_set rset;
        FD_ZERO(&rset);
        signal(SIGALRM,handle_fun);
        char sendline[MAXLINE],recvline[MAXLINE+1];
// Clean buffers:
    memset(sendline, '\0', sizeof(sendline));
    memset(recvline, '\0', sizeof(recvline));
        while(1)
        {        
        		FD_SET(fileno(fp),&rset);
        		FD_SET(sockfd,&rset);
        		maxfd=sockfd>fileno(fp)?sockfd:fileno(fp);
        		ns=select(maxfd+1,&rset,NULL,NULL,NULL);
                if(FD_ISSET(fileno(fp),&rset))
                {
                if(fgets(sendline,MAXLINE,fp)!=NULL)
                {
                sendto(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
                }
                }
                alarm(1);
                {
                //sleep(12)
                if(FD_ISSET(fileno(fp),&rset))
                {
                n=recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
                recvline[n]=0;
                printf("data recieved from server:%s\n",recvline);
                }
           
        }
        }
        }   
        

int main(int argc, char *argv[]){
   int sockfd;
   struct sockaddr_in servaddr;
   socklen_t len;
   len=sizeof(struct sockaddr_in);
   if(argc!=3){
       fprintf(stderr,"Usage %s  <IP>  <PORT>\n",argv[0]);
       return 1;
   }
   servaddr.sin_family=AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(atoi(argv[2]));
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
   if(sockfd>0){
         fprintf(stderr,"Socket created success\n");
   }
   else{
      fprintf(stderr,"socket creat error\n");
      return 1;
    }
     dg_cli(stdin,sockfd,(struct sockaddr*) &servaddr,sizeof(servaddr));
         
  return 0;
  }
