#include <stdio.h>

int main(void) {
  int arr[4];
  int *p = arr;
  p = &arr[2];
  printf("%p\n", (void *)p);
  return 0;
}
