#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include <sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include <unistd.h>
#include<pthread.h>
#define N 1
int s;
void *accept_th(char **argv) {
  int ss=socket(PF_INET,SOCK_STREAM,0);
  if(ss==-1){
    perror("socket error");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[1]));
  addr.sin_addr.s_addr=INADDR_ANY; //accept all IP address
  bind(ss,(struct sockaddr *)&addr,sizeof(addr));

  listen(ss,10);

  struct sockaddr_in client_addr;
  socklen_t len=sizeof(struct sockaddr_in);
  s=accept(ss,(struct sockaddr *)&client_addr,&len);
}

void *read_th() {
  short data[N];
  int n;

  while(1){

    n=read(0,data,2);  //from console

    if(n==0) break;


    n=send(s,data,2,0);  //send

    if(n==-1){

      perror("send error");
      exit(1);
    }

  }
  close(s);
}

int main(int argc,char **argv){

  pthread_t accept_thread, read_thread;
  pthread_create(&accept_thread, NULL, accept_th,  &argv);
  pthread_create(&read_thread, NULL, read_th);

  pthread_join(accept_thread, NULL);
  pthread_join(read_thread, NULL);



  return 0;
}
