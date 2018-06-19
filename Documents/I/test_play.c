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
void die(char* str){
  perror(str);
  exit(1);
}
int main(int argc,char **argv){
  FILE *pipe,*pipe_play;
  //popen
  char rec_command[]="rec -t raw -b 16 -c 1 -e s -r 44100 -";
  if (( pipe = popen(rec_command, "r") )== NULL) die("popen");
  char play_command[]="play -t raw -b 16 -c 1 -e s -r 44100 -";
  if (( pipe_play = popen(play_command, "w") )== NULL) die("popen_play");


  //close(ss);

  unsigned char temp[2];
  int n;
  FILE *doremi=fopen("doremi.raw","r");
  while(1){
    fread( temp,1, 2, doremi ); //from console
    fwrite(temp,1,2,pipe_play);
  }
  pclose(pipe);
  return 0;
}
