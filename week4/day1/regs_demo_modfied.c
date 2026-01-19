#include <stdio.h>

int add(int x, int y) { return x + y; }

int main(void) {
  int a = 10, b = 7;
  int c = add(a, b);
  printf("c=%d\n", c);
  return 0;
}
