/*Socket : Day Time Client*/
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<arpa/inet.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

void str_cli(int sockfd);
int main(int argc, char *argv[])
{
int sockfd,n,conn,len;
char recvline[1024];
struct sockaddr_in servaddr;
len=sizeof(struct sockaddr_in);
sockfd=socket(AF_INET,SOCK_STREAM,0);
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr(argv[1]);//get ip from server
servaddr.sin_port=htons(atoi(argv[2])); // Get the port from the server
connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

str_cli(sockfd);
exit(0);
}

void str_cli(int sockfd){
int maxfdp1; // store maximum no of file descriptors
fd_set rset; // to hold file descriptors to monitor
char sendline[1024], recvline[1024];
FD_ZERO(&rset);
for ( ; ; ) {
FD_SET(STDIN_FILENO, &rset); // add stdin file descriptor to rset
FD_SET(sockfd, &rset);// add socketfd file descriptor to rset
maxfdp1 = MAX(STDIN_FILENO, sockfd) + 1; // the highest actine filedescriptor+1
select(maxfdp1, &rset, NULL, NULL, NULL);

if (FD_ISSET(sockfd, &rset)) { /* socket is readable */
 if (read(sockfd, recvline, 1024) == 0)
 	{perror("str_cli: server terminated prematurely");
 	exit(0);
 	}
 fputs(recvline, stdout);
 }
 if (FD_ISSET(STDIN_FILENO, &rset)) { /* input is readable */
 	if (fgets(sendline, 1024, stdin) == NULL)
 		return; /* all done */
 write(sockfd, sendline, strlen(sendline));
 		}
 	}
 }


