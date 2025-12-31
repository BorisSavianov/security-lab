// c_refresh/dyn_array.c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int n = 3;
  int *a = malloc(n * sizeof(int));
  if (!a)
    return 1;

  for (int i = 0; i < n; i++)
    a[i] = i * 10;

  printf("Before realloc: a=%p\n", (void *)a);
  for (int i = 0; i < n; i++)
    printf("a[%d]=%d\n", i, a[i]);

  n = 5;
  int *old = a;
  a = realloc(a, n * sizeof(int));
  if (!a)
    return 1;

  for (int i = 3; i < n; i++)
    a[i] = i * 10;

  printf("After realloc: a=%p (old=%p)\n", (void *)a, (void *)old);
  for (int i = 0; i < n; i++)
    printf("a[%d]=%d\n", i, a[i]);

  free(a);
  return 0;
}
