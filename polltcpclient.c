/*TCP client echo */
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<poll.h>
#include<sys/stropts.h>
#define INFTIM -1
#define MAXLINE 200
void str_cli(FILE *fp, int fd)
{
        struct pollfd r[2];
        r[0].fd = fileno(fp);
        r[0].events=POLLRDNORM;
        r[1].fd = fd;
        r[1].events = POLLRDNORM;
        int n;
        char buffer[MAXLINE];
        for(;;)
        {
                n = poll(r, 2, INFTIM);
                printf("Poll returned %d\n",n);
                if(r[0].revents & POLLRDNORM)
                {
                        if(fgets(buffer, MAXLINE, fp) !=NULL)
                        {
                                write(fd, buffer, strlen(buffer));
                        }
                }
                if(r[1].revents & (POLLRDNORM | POLLERR))
                {
                        printf("2ndPoll returned %d\n",n);
                        n = read(fd,  buffer, MAXLINE);
                        if(n ==0)
                        {
                            printf("Connection ended ...\n");
                            break;
                        }
                        if(n<0)
                        {
                            printf("Read filed .... \n");
                        }
                        buffer[n]='\0';
                        printf("data recieved from server : %s\n", buffer);
                 }
        }
        
        /*while(fgets(buffer, MAXLINE, fp) !=NULL)
        {
                write(fd, buffer, strlen(buffer));
                n = read(fd,  buffer, MAXLINE);
                if(n ==0)
                {
                    printf("Connection ended ...\n");
                    break;
                }
                if(n<0)
                {
                    printf("Read filed .... \n");
                }
                buffer[n]='\0';
                printf("data recieved from server : %s\n", buffer);
        }*/
        
}

int main(int argc, char *argv[]){
    int sockfd,cr;
    struct sockaddr_in ca,sa;
    socklen_t len;
    len=sizeof(struct sockaddr_in);
    if(argc!=3){
        fprintf(stderr,"Usage %s  <IP>  <PORT>\n",argv[0]);
        return 1;
    }
    ca.sin_family=AF_INET;
    ca.sin_addr.s_addr=inet_addr(argv[1]);
    ca.sin_port=htons(atoi(argv[2]));
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd>0){
        fprintf(stderr,"Socket created success\n");
    }
    else{
        fprintf(stderr,"socket creat error\n");
    return 1;
    }
    cr=connect(sockfd,(struct sockaddr *)&ca,sizeof(ca));
    if(cr==0){
        fprintf(stderr,"Connect success return=%d\n",cr);
        printf("Connected server details\n");
        getpeername(sockfd,(struct sockaddr *)&sa,&len);
        printf("Sver port=%d\n",ntohs(sa.sin_port)); 
        printf("Server IP=%s\n",inet_ntoa(sa.sin_addr));

    }
    else{
    fprintf(stderr,"Connect Error=%d\n",cr);
    exit(1);
    }
    str_cli(stdin, sockfd);
    return 0;
  }
