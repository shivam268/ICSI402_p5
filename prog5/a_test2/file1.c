#include  <stdio.h>

void  do_print(void);

/* A simple C program with two functions. */
int main(void) {
  int j;
  while (scanf("%d", &j) == 1) {
    printf("j = %d\n", j); fflush(stdout);
  }
  do_print();
  return 0;
}

void  do_print(void) {
  printf("Hello World!\n");
  fflush(stdout);
}
