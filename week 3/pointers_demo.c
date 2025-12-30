#include <stdio.h>
#include <stdlib.h>

void foo(int a[10]) { printf("inside foo sizeof(a)=%zu\n", sizeof(a)); }

void make_arr(int **out, int n) { *out = malloc(n * sizeof(int)); }

int main(void) {
  int arr[3] = {10, 20, 30};
  int *p = arr;
  printf("arr=%p, &arr[0]=%p\n", (void *)arr, (void *)&arr[0]);
  printf("p[1]=%d\n", p[1]);
  printf("*(p+2)=%d\n", *(p + 2));

  printf("%ld\n", sizeof(arr));
  printf("%ld\n", sizeof((void *)p));

  printf("%p\n", (void *)(arr + 1));
  printf("%p\n", (void *)((char *)arr + 1));

  foo(arr);

  int *arr2;
  make_arr(&arr2, 10);
  printf("arr2=%p\n", (void *)arr2);
  free(arr2);
  return 0;
}
