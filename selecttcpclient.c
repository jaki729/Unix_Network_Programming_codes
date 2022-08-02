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
#define MAXLINE 200

void print_fdset(fd_set set,int n){
    for(int i  = 0;i< n;i++)
        printf("%d",FD_ISSET(i,&set));
    printf("\n"); 
}

void str_cli(FILE *fp, int fd)
{
        int n, ns, maxfd;
        char buffer[MAXLINE];
        fd_set rset;
        printf("At init ");
        print_fdset(rset,10);
        FD_ZERO( &rset);
        printf("At Zero ");
        print_fdset(rset,10);

        for(;;){
         FD_SET(fileno(fp),&rset);
          printf("At Set 1 ");
        print_fdset(rset,10);
        FD_SET(fd, &rset);
         printf("At Set sockfd ");
        print_fdset(rset,10);
        maxfd=fd>fileno(fp)?fd:fileno(fp);
        ns=select(maxfd+1,&rset, NULL,NULL,NULL);
         printf("At select ");
        print_fdset(rset,10);
         
         if(FD_ISSET(fileno(fp),&rset))
         { if(fgets(buffer, MAXLINE, fp) !=NULL)
                {write(fd, buffer, strlen(buffer));
                write(1,"sent to server",14);}
                   else
                return;
         }
            if(FD_ISSET(fd,&rset)){
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
