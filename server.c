#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

pthread_mutex_t recvMutex;
struct sockaddr_in c_addr;
int threadnum;
int threadid;
long int  size;
fpos_t *position;
int i=0;
char *fname;
void* SendFileToClient(int *arg)
{

  pthread_mutex_lock(&recvMutex);

      struct stat fileStat;
      int connfd=(int)*arg;
      write(connfd, fname,256);
      if(stat(fname,&fileStat) < 0)
        return 1;

        size = fileStat.st_size;
        FILE *fp = fopen(fname,"rb");
        if(fp==NULL)
        {
            printf("Error file opening...!!");
            return 1;
        }
        long int total_bytes_read=0;
        int p = 0;

        fsetpos(fp,&position);

        if(i==threadnum-1){
          while(!feof(fp)){

            unsigned char buff[1024]={0};
            int nread = fread(buff,1,1024,fp);
            total_bytes_read += nread;
            write(connfd, buff, nread);
          }
          for (p = 0; p <= 100; p++){
            printf("\rIn progress  percentage of %d for thread:  %d", p,threadid);
            fflush(stdout);
          }
        }
        else if(i<threadnum-1){

        while(total_bytes_read < (size/threadnum))
        {
            unsigned char buff[1024]={0};
            int nread = fread(buff,1,1024,fp);
            total_bytes_read += nread;
            write(connfd, buff, nread);
            if(nread <=0)
            break;
            }

      for (p = 0; p <= 100; p++){
          printf("\rIn progress  percentage of %d for thread:  %d", p,threadid);
          fflush(stdout);
      }

      printf("\n");
    }

      fgetpos(fp,&position);
      i++;
      printf("\n");
fclose(fp);
close(connfd);
shutdown(connfd,SHUT_WR);
pthread_mutex_unlock(&recvMutex);
pthread_exit(NULL);

}

int main(int argc, char *argv[])
{

    pthread_mutex_init(&recvMutex,NULL);

    int connfd = 0,err;
    pthread_t tid;
    struct sockaddr_in serv_addr;
    int listenfd = 0,ret;
    char sendBuff[1025];
    int numrv;
    size_t clen=0;
    char client_message[2000];
    char *read_int;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0)
	{
	  printf("Error in socket creation\n");
	  exit(2);
	}


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8000);

    ret=bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(ret<0)
    {
      printf("Error in bind\n");
      exit(2);
    }

    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }

    while((  connfd = accept(listenfd, (struct sockaddr*)&c_addr,&clen)))
    {
        clen=sizeof(c_addr);
        if(connfd<0)
        {
	  printf("Error in accept\n");
	  continue;
	}
      recv(connfd , client_message , 2000 , 0);
      read_int = strtok(client_message, ","); // Splits spaces between words in str
      threadid =  atoi(read_int);
      read_int = strtok(NULL, ",");
      threadnum = atoi(read_int);
      fname = (char*)malloc(1000);
      fname=  strtok(NULL,",");
     err = pthread_create(&tid, NULL, &SendFileToClient, &connfd);
      if (err != 0)
          printf("\ncan't create thread :[%s]", strerror(err));
 }

  close(connfd);
  return 0;
}
