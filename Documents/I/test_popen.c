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
    printf("%s",temp);
  }
  pclose(pipe);
  return 0;
}
