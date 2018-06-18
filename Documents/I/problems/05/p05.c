/* 
 * p05.c
 */

/* 
 * usage:
 *   ./a.out
 * 
 * Intended behavior
 *   It reads the contents of this file and print it.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
  FILE *fp = fopen("p05.c", "rb");
  if(fp == NULL){
  perror("Could not open input file");
  return -1;
  }
  char buf[100];
  while (1) {
    int n = fread(buf, sizeof(char), 100, fp);
    if (n == 0) break;
    fwrite(buf, sizeof(char), n, stdout);
  }
  fclose(fp);
  return 0;
}

