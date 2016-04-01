#include  <stdio.h>

void  do_print(void);

void  more_print(void);

/* A simple C program with three functions. */

int main(void) {
  int j;
  while (scanf("%d", &j) == 1) {
    printf("j = %d\n", j); fflush(stdout);
  }
  do_print();
  more_print();
  return 0;
}

void  do_print(void) {
  printf("Hello World!\n");
  fflush(stdout);
}

void  more_print(void) {
  printf("Hello World!\n");
  printf("Hello World!\n");
  fflush(stdout);
}
