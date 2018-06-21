//popen
//human voice 85-255 Hz
#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#define N 100
typedef char sample_t;

void die(char* str){
  perror(str);
  exit(1);
}

/* fd から 必ず n バイト読み, bufへ書く.
   n バイト未満でEOFに達したら, 残りは0で埋める.
   fd から読み出されたバイト数を返す */
ssize_t read_n(int fd, ssize_t n, void * buf) {
  ssize_t re = 0;
  while (re < n) {
    ssize_t r = read(fd, buf + re, n - re);
    if (r == -1) die("read");
    if (r == 0) break;
    re += r;
  }
  memset(buf + re, 0, n - re);
  return re;
}

/* fdへ, bufからnバイト書く */
ssize_t write_n(int fd, ssize_t n, void * buf) {
  ssize_t wr = 0;
  while (wr < n) {
    ssize_t w = write(fd, buf + wr, n - wr);
    if (w == -1) die("write");
    wr += w;
  }
  return wr;
}

/* 標本(整数)を複素数へ変換 */
void sample_to_complex(sample_t * s,
		       complex double * X,
		       long n) {
  long i;
  for (i = 0; i < n; i++) X[i] = s[i];
}

/* 複素数を標本(整数)へ変換. 虚数部分は無視 */
void complex_to_sample(complex double * X,
		       sample_t * s,
		       long n) {
  long i;
  for (i = 0; i < n; i++) {
    s[i] = creal(X[i]);
  }
}

/* 高速(逆)フーリエ変換;
   w は1のn乗根.
   フーリエ変換の場合   偏角 -2 pi / n
   逆フーリエ変換の場合 偏角  2 pi / n
   xが入力でyが出力.
   xも破壊される
 */
void fft_r(complex double * x,
	   complex double * y,
	   long n,
	   complex double w) {
  if (n == 1) { y[0] = x[0]; }
  else {
    complex double W = 1.0;
    long i;
    for (i = 0; i < n/2; i++) {
      y[i]     =     (x[i] + x[i+n/2]); /* 偶数行 */
      y[i+n/2] = W * (x[i] - x[i+n/2]); /* 奇数行 */
      W *= w;
    }
    fft_r(y,     x,     n/2, w * w);
    fft_r(y+n/2, x+n/2, n/2, w * w);
    for (i = 0; i < n/2; i++) {
      y[2*i]   = x[i];
      y[2*i+1] = x[i+n/2];
    }
  }
}

void fft(complex double * x,
	 complex double * y,
	 long n) {
  long i;
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) - 1.0j * sin(arg);
  fft_r(x, y, n, w);
  for (i = 0; i < n; i++) y[i] /= n;
}

void ifft(complex double * y,
	  complex double * x,
	  long n) {
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) + 1.0j * sin(arg);
  fft_r(y, x, n, w);
}

int pow2check(long N0) {
  long n = N0;
  while (n > 1) {
    if (n % 2) return 0;
    n = n / 2;
  }
  return 1;
}

void print_complex(FILE * wp,
		   complex double * Y, long n) {
  long i;
  for (i = 0; i < n; i++) {
    fprintf(wp, "%ld %f %f %f %f\n",
	    i,
	    creal(Y[i]), cimag(Y[i]),
	    cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
  }
}

void bandpass(char buf[]){

  complex double * X = calloc(sizeof(complex double), N);
  complex double * Y = calloc(sizeof(complex double), N);
    /* 複素数の配列に変換 */
    sample_to_complex(buf, X, N);
    /* FFT -> Y */
    fft(X, Y, N);


    //TO DO
    int fs=44100;
    int i1,i2;
    i1=80*N/fs;   // range 85-255
    i2=300*N/fs;

    int i;
    for (i = 0; i < N/2; i++) {
      if(i>i2 || i<i1){
        Y[i]=0+0*_Complex_I;
        Y[N-i]=0+0*_Complex_I;
      }
    //  else printf("%f%+fi\n", crealf(Y[i]), cimagf(Y[i]));
    }

    //END TO DO

    /* IFFT -> Z */
    ifft(Y, X, N);
    /* 標本の配列に変換 */
    complex_to_sample(X, buf, N);

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
      //printf("before loop\n");
    unsigned char temp[2];
    int n;

    while(1){
      //recieve
      //printf("loop1\n");
      n=read(s,temp,N);

      if(n==-1) die("recv error");
      //if(n==0) break;
      fwrite(temp,1,N,pipe_play);
      //printf("loop2\n");
      //send
      fread( temp, 1,N, pipe ); // from rec
      bandpass(temp);
      n=send(s,temp,N,0);  //send
      if(n==-1) die("send error");

    }
    printf("EEENNNDDD\n");
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

    unsigned char temp[N];
    int n;
    FILE *pipe;
    FILE *pipe_play;
    //popen
    char rec_command[]="rec -t raw -b 16 -c 1 -e s -r 44100 -";
    if (( pipe = popen(rec_command, "r") )== NULL) die("popen");
    char play_command[]="play -t raw -b 16 -c 1 -e s -r 44100 -";
    if (( pipe_play = popen(play_command, "w") )== NULL) die("popen_play");
    while(1){
      //send
      fread( temp, 1,N, pipe ); // from rec
      bandpass(temp);
      n=send(s,temp,N,0);  //send
      if(n==-1) die("send error");
      //recieve
      n=recv(s,temp,N,0);
      if(n==-1) die("recv error");

      //if(n==0) break;
      fwrite(temp,1,N,pipe_play);


    }
    printf("EEENNNDDD\n");
    pclose(pipe);
    pclose(pipe_play);
    close(s);
  }
  else{
    printf("command error\n");
  }
  return 0;
}
