#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <unistd.h>


int main(int argc,char ** argv){
  short data[1];
  int count=0,n;
  int fd = open(argv[1],O_RDONLY,0644);
  if(fd==-1){
    perror("open");
    exit(1);
  }
  while(1){
    n=read(fd,data,2);
    if(n==-1){
      perror("read");
      exit(1);
    }
    if(n==0) break;
    printf("%d %d\n",count,data[0]);
    count++;
  }
  close(fd);
  return 0;
}
