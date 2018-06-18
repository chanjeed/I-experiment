#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<math.h>

float note(float base,int a){
  return base*pow(2,(float)a/(float)12);
}

int main(int argc,char ** argv){
  int A,n,a;
  int i,j;
  float f=130.81,F=44100;
  A=atoi(argv[1]);
  n=atoi(argv[2]);

  short data[1];

  for(i=1;i<=n;i++){
    if(i!=1){
      if(i%7==0 ||i%7==3 ) a=1;
      else a=2;
      f=note(f,a);
    }
    for(j=0;j<13230;j++){
      data[0]=A*sin(2*M_PI*f*(j*1.0/F));
      write(1,data,2);
    }
  }
  return 0;
}
