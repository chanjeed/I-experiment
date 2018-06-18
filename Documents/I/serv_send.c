#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#define N 1
int main(int argc,char **argv){
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
  int s=accept(ss,(struct sockaddr *)&client_addr,&len);
  //close(ss);
//After this same as client_send_recv.c (only send part)
  unsigned char data[N];
  int n;

  while(1){
    n=read(0,data,N);  //from console
    if(n==0) break;

    n=send(s,data,N,0);  //send
    if(n==-1){

      perror("send error");
      exit(1);
    }

  }
  close(s);
  return 0;
}
