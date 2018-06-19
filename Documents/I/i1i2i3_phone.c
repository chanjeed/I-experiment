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
  if(argc==2){
  //server

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
    if (s < 0) die("accept failed");
      printf("accept finish\n" );
      FILE *pipe;
      FILE *pipe_play;
      //popen
      char rec_command[]="rec -t raw -b 16 -c 1 -e s -r 44100 -";
      if (( pipe = popen(rec_command, "r") )== NULL) die("popen");
      char play_command[]="play -t raw -b 16 -c 1 -e s -r 44100 -";
      if (( pipe_play = popen(play_command, "w") )== NULL) die("popen_play");

    unsigned char temp[2];
    int n;

    while(1){
      //recieve
      n=recv(s,temp,2,0);
      if(n==-1) die("recv error");
      if(n==0) break;
      fwrite(temp,1,2,pipe_play);

        //send
      fread( temp, 1,2, pipe ); // from rec
      n=send(s,temp,2,0);  //send
      if(n==-1) die("send error");
    }
    pclose(pipe);
    pclose(pipe_play);
    close(s);
    close(ss);
  }
  else if(argc==3){
    int s=socket(PF_INET,SOCK_STREAM,0);
    if(s==-1) die("socket error");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_aton(argv[1],&addr.sin_addr);
    //addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    int ret=connect(s,(struct sockaddr *)&addr,sizeof(addr));
    if(ret==-1) die("connect error");

    unsigned char temp[2];
    int n;
    FILE *pipe;
    FILE *pipe_play;
    //popen
    char rec_command[]="rec -t raw -b 16 -c 1 -e s -r 44100 -";
    if (( pipe = popen(rec_command, "r") )== NULL) die("popen");
    char play_command[]="play -t raw -b 16 -c 1 -e s -r 44100 -";
    if (( pipe_play = popen(play_command, "w") )== NULL) die("popen_play");
    while(1){
      //recieve
      n=recv(s,temp,2,0);
      if(n==-1) die("recv error");

      if(n==0) break;
      fwrite(temp,1,2,pipe_play);

        //send
      fread( temp, 1,2, pipe ); // from rec
      n=send(s,temp,2,0);  //send
      if(n==-1) die("send error");
    }
    pclose(pipe);
    pclose(pipe_play);
    close(s);
  }
  else{
    printf("command error\n");
  }
  return 0;
}
