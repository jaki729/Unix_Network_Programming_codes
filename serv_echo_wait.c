#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<errno.h>
#include <signal.h>
#include <sys/wait.h>

void str_echo(int connfd);
void sig_chld(int signo);

int main(int argc, char **argv)
{
int listenfd,connfd,len;
pid_t pid;
struct sockaddr_in servaddr,clientaddr;
len=sizeof(struct sockaddr_in);
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(0);
	bind(listenfd,(struct sockaddr *)&servaddr, sizeof(servaddr));
	getsockname(listenfd, (struct sockaddr *)&servaddr, &len);
	printf("After bind ephemeral port=%d\n",(int)ntohs(servaddr.sin_port));
	listen(listenfd,1);
signal(SIGCHLD,sig_chld);
for(;;){
	connfd=accept(listenfd, (struct sockaddr *)&clientaddr,&len);
	pid=fork();
	if(pid==0)
		{
		printf("Connection from ::%s: port %d:: child pid %ld \n", inet_ntoa(clientaddr.sin_addr),(int)ntohs(clientaddr.sin_port),(long)getpid());
		close(listenfd);
		str_echo(connfd);
		exit(0);
		}
	close(connfd);
	}//end of for
}//end of main

void str_echo(int connfd)
{
int n;
char buff[1024];
again:
while( (n=read(connfd,buff,1024))>0)
   { write(connfd, buff,n);
     write(1, buff,n); // buff[n+1]='\0'; printf("%s",buff);
   
   }
    
   if(n<0 && errno==EINTR)
                    goto again;
  else if(n<0)
                perror("error:");

}



void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,NULL, WNOHANG))>0)
		//pid=wait(NULL);
		printf("Child %d terminated \n", pid);
	return;
}



