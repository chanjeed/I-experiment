#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#define N 1
int main(int argc,char **argv){
  int s=socket(AF_INET,SOCK_DGRAM,0);
  if(s==-1){
    perror("socket error");
    exit(1);
  }
  struct sockaddr_in addr;

  struct sockaddr_in from_addr;

  addr.sin_family = AF_INET;
  //inet_aton(argv[1],&addr.sin_addr);
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));

  sendto(s, "Hi", 2, 0,(struct sockaddr *)&addr,sizeof(addr));

  char *myadd="192.168.100.50";
  addr.sin_addr.s_addr = inet_addr(myadd);
  // バインドする
  bind(s, (struct sockaddr *)&addr, sizeof(addr));

  int n;
  //send dummy 1 byte data
  unsigned int sin_size=sizeof(addr);
  char buf[1000];
//  memset(buf, 0, sizeof(buf));

  char allone[1000];
  memset(allone, '1', sizeof(allone));

  //recieve 1000 byte
  int buflen;
  while(1)
  {
    memset(buf, 0, sizeof(buf));
    buflen=recvfrom(s, buf, sizeof(buf), 0,(struct sockaddr *)&addr, &sin_size);

    if(buflen < 0) {
      perror("recvfrom");
      return -1;
    }
    if(strcmp(buf,allone)==0)break;

    printf("%s",buf);
  }
  close(s);
  return 0;
}
