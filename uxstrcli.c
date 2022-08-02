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
#define max(a,b) (((a)>(b))?(a):(b))

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1, stdineof = 0;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];



	FD_ZERO(&rset);
	for ( ; ; ) {
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
			if (errno == EINTR)
				continue;
			else
				perror("select error");
		}

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (read(sockfd, recvline, MAXLINE) == 0) {
				if (stdineof == 1)
					return;		/* normal termination */
				else
					perror("str_cli: server terminated prematurely");
			}

			write(STDOUT_FILENO, recvline, strlen(recvline));
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (fgets(sendline, MAXLINE, fp) == NULL) {
				stdineof = 1;
				//alarm(0);			/* turn of heartbeat */
				shutdown(sockfd, SHUT_WR);	/* send FIN */
				FD_CLR(fileno(fp), &rset);
				continue;
			}

			write(sockfd, sendline, strlen(sendline));
		}
	}
}


int main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_un	servaddr;

	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);

	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
