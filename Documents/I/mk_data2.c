#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


int main(int argc,char ** argv){
  int fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fd==-1){
    perror("open");
    exit(1);
  }
  int N=256;
  unsigned char data[N];
  int i;
  for(i=0;i<N;i++){
    data[i]=i;
  }
  check=write(fd,data,N);
  if(check!=N){
    perror("cannot write file");
    exit(1);
  }
  close(fd);
  return 0;
}
