#include  <stdio.h>
/* A simple C program. */
int main(void) {
  int j;
  while (scanf("%d", &j) == 1) {
    printf("j = %d\n", j); fflush(stdout);
  }
  return 0;
}
