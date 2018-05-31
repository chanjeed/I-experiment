#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<math.h>

int main(int argc,char ** argv){
  int A,f,n,F;
  int i;
  A=atoi(argv[1]);
  f=atoi(argv[2]);
  n=atoi(argv[3]);
  F=44100;
  short data[1];
  //printf("hoge %d %d %d\n",A,f,n);
  for(i=0;i<n;i++){
    data[0]=A*sin(2*M_PI*f*(i*1.0/F));
    //printf("%c |\n",data[i] );
    write(1,data,2);
  }
  return 0;
}
