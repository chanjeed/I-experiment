//popen

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
  printf("before listen\n" );
  listen(ss,10);
  printf("after listen\n" );
  struct sockaddr_in client_addr;
  socklen_t len=sizeof(struct sockaddr_in);
  int s=accept(ss,(struct sockaddr *)&client_addr,&len);
  if (s < 0)
  {
      perror("accept failed");
      exit(1);
  }
  printf("accept finish\n" );
  FILE *pipe;
  //popen
  char rec_command[]="rec -t raw -b 16 -c 1 -e s -r 44100 -";
  if (( pipe = popen(rec_command, "r") )== NULL)
  {
    perror("popen");
    exit(1);
  }

  //close(ss);

  unsigned char temp[2];
  int n;

  while(1){
    fgets( temp, 2, pipe ); //from console
    n=send(s,temp,2,0);  //send
    if(n==-1){
      perror("send error");
      exit(1);
    }
    printf("%s\n",temp );
  }
  pclose(pipe);
  close(s);
  close(ss);
  return 0;
}
