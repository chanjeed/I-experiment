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
  char rec_command[]="play -t raw -b 16 -c 1 -e s -r 44100 -";
  if (( pipe = popen(rec_command, "w") )== NULL)
  {
    perror("popen");
    exit(1);
  }

  //close(ss);

  unsigned char temp[2];
  int n;
  FILE *doremi=fopen("doremi.raw","r");


  while(1){
    fread( temp,1, 2, doremi ); //from console
    fwrite(temp,1,2,pipe);
  }
  pclose(pipe);
  return 0;
}
