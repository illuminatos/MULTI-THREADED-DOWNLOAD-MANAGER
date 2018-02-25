#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
pthread_mutex_t recvMutex;
char fname[2000];

void *connection_handler(void *socket_desc);

 char ip[50];

int thread_num;
 int main(int argc, char *argv[]){
    int i;
    system("clear");
    pthread_mutex_init(&recvMutex,NULL);

    strcpy(ip,argv[1]);
    thread_num = strtol(argv[2], NULL, 0);
    pthread_t thread[thread_num];
    strcpy(fname,argv[3]);
    for (i=0; i<thread_num; i++) {
        if( pthread_create( &thread[i] , NULL ,  connection_handler , (void*)(intptr_t) i) < 0)
        {
            perror("could not create thread");
            return 1;
        }
    }

    for (i=0; i<thread_num; i++)
    pthread_join(&thread[i], NULL);

    pthread_exit(NULL);

    return 0;
}

void *connection_handler(void *threadid)
{  pthread_mutex_lock(&recvMutex);

    int threadnum = (intptr_t)threadid;
    char message[2000];
    char message2[2000];
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[1024];
    memset(recvBuff, '0', sizeof(recvBuff));

    struct sockaddr_in serv_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8000); // port


    serv_addr.sin_addr.s_addr = inet_addr(ip);


    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {

        printf("\n Error : Connect Failed \n");
        return 1;
    }
    sprintf(message, "%d", threadnum);
    sprintf(message + strlen(message),",");
    sprintf(message + strlen(message),"%d",thread_num);
    sprintf(message + strlen(message),",");
    sprintf(message + strlen(message),"%s",fname);
    if(send(sockfd , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }




  FILE *fp;
  char fname[100];
  read(sockfd, fname, 1024);

     fp = fopen(fname, "ab");
      if(NULL == fp)
      {
         printf("Error opening file");
         return 1;
      }
    long double sz=1;
    int total_bytes_read=0;
    int p=0;



    while((bytesReceived = read(sockfd, recvBuff, 1024)) > 0)
    {

        total_bytes_read +=bytesReceived;
        fwrite(recvBuff, 1,bytesReceived,fp);

}

  for (p = 0; p <= 100; p++){
  printf("\rIn progress  percentage of %d for thread:  %d", p,threadnum);
  fflush(stdout);
  }
  printf("\n");

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }


    pthread_mutex_unlock(&recvMutex);

    pthread_exit(NULL);

    return 0;

}
