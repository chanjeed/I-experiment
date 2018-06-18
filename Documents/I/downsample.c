#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int main(int argc,char ** argv){
  int n,N;
  int i,count=0;
  N=atoi(argv[1]);
  short data[1];
  while(1){
    n=read(0,data,2);
    if(n==-1){
      perror("read");
      exit(1);
    }
    if(n==0) break;
    if(count%N==0) write(1,data,2);
    count++;
  }
  return 0;
}
