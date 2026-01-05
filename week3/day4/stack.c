#include <stdio.h>

int f(int a, int b) {
  int buf[4];
  buf[0] = a;
  buf[1] = b;
  buf[2] = a + b;
  buf[3] = a - b;
  return buf[0] + buf[1];
}

int main(void) {
  int r = f(5, 7);
  printf("r=%d\n", r);
  return 0;
}
