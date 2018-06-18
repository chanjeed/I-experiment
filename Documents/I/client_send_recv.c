#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include <sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include <unistd.h>
#define N 1
int main(int argc,char **argv){
  int s=socket(PF_INET,SOCK_STREAM,0);
  if(s==-1){
    perror("socket error");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  inet_aton(argv[1],&addr.sin_addr);
  //addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));
  int ret=connect(s,(struct sockaddr *)&addr,sizeof(addr));
  if(ret==-1){
    perror("connect error");
    exit(1);
  }

  unsigned char data[N];
  int n;

  while(1){

    n=read(0,data,N);

    if(n==0) break;


    n=send(s,data,N,0);
  
    if(n==-1){

      perror("send error");
      exit(1);
    }

  }
  shutdown(s,SHUT_WR);

  while(1){

    n=recv(s,data,N,0);
    
    if(n==-1){

      perror("recv error");
      exit(1);
    }
    if(n==0) break;

    write(1,data,N);
  
  }
  //close(s);
  return 0; 
}
