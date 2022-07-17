/*Socket : Day Time Server*/
#include<stdio.h>
#include <unistd.h> 
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
	int pid; // 
	time_t ticks;
	len=sizeof(struct sockaddr_in);
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(0);
	bind(listenfd,(struct sockaddr *)&servaddr, sizeof(servaddr));
	getsockname(listenfd, (struct sockaddr *)&servaddr, &len);
	printf("After bind ephemeral port=%d\n",(int)ntohs(servaddr.sin_port));
	listen(listenfd,5);
	for(;;){ //WE RUN SERVER ALWAYS
	connfd=accept(listenfd, (struct sockaddr *)&clientaddr,&len);
	printf("Connection from ::%s: port %d\n", inet_ntoa(clientaddr.sin_addr),(int)ntohs(clientaddr.sin_port));
	pid=fork(); // NOW YOU HAVE A COPY OF SERVER
		if(pid==0){  /* CHILD COPY shall HANDLE DATA TRANSFER*/
		close(listenfd); // THE CHILD DOES NOT HANDLE NEW CONNECTION, so close it
		snprintf(buff,sizeof(buff),"YOUR REQUEST PROCESS BY SERVER COPY PID= %ld \r\n",(long)getpid());
		/*PREPARE A MESSAGE (buff) CONTAINING CHILD SERVER PID, THAT WILL BE SENT TO CHILD*/
		write(connfd,buff,strlen(buff));
		write(connfd,"BYE BYE",7); // JUST A MESSAGE
		close(connfd); // Done with Client
		exit(0); // Child process exits. 
		}
	}
	close(connfd); // PARENT SERVER CLOSES CONNECTION
}
