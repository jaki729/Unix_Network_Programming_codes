#include<stdio.h>
#include<netinet/in.h>
#include <sys/socket.h>
#include<stdlib.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#include<sys/types.h>
#include<sys/un.h>
#include<errno.h>
#include <signal.h>
#define MAXLINE 1024
# define UNIXSTR_PATH  "/tmp/unix.str"
#define LISTENQ 10
static void sig_chld(int signo)
{
	return;
}
void  str_echo(int sockfd)
 {

ssize_t n;
char buf[MAXLINE];
 again:
while ( (n = read(sockfd, buf, MAXLINE)) > 0)
    write(sockfd, buf, n);
if (n < 0 && errno == EINTR)
    goto again;
    else if (n < 0)
perror("str_echo: read error");
}

int main(int argc, char *argv[])
{
	int	listenfd, connfd;
	pid_t	childpid;
	socklen_t clilen;
	struct	sockaddr_un cliaddr, servaddr;

	if ((listenfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0)
		perror("socket error");

	unlink(UNIXSTR_PATH);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		perror("bind error");
	if (listen(listenfd, LISTENQ) < 0)
		perror("listen error");
	if (signal(SIGCHLD, sig_chld) == SIG_ERR)
		perror("signal error");

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
			if (errno = EINTR) /* interrupted by SIGCHLD */
				continue;
			else 
				perror("accept error");
		}

		if ((childpid = fork()) < 0) {
			perror("fork error");
		} else if (childpid == 0) {
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	}
}


