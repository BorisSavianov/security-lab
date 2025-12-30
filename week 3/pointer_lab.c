// pointer_lab.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Lab program overview:
 * - Demonstrates array vs pointer behavior
 * - Uses pointer-to-pointer allocator
 * - Contains an intentional off-by-one bug (write past a small buffer)
 * - Demonstrates a string-literal modification attempt (UB)
 */

void make_array(int **out, size_t n) {
  *out = malloc(n * sizeof(int));
  if (!*out) {
    perror("malloc");
    exit(1);
  }
  for (size_t i = 0; i < n; ++i)
    (*out)[i] = (int)i * 10;
}

void buggy_function(void) {
  int small = 0xdeadbeef;
  int buf[3] = {1, 2, 3}; // allocated on stack
  // Intentional off-by-one: writes buf[3] which is past end (UB)
  buf[3] = 0x41414141;
  printf("Inside buggy_function: small=0x%x, buf[0..2]=%d %d %d\n", small,
         buf[0], buf[1], buf[2]);
}

void try_modify_literal(void) {
  char *s = "HELLO_LITERAL";
  printf("String literal address: %p contents: %s\n", (void *)s, s);
  // Intentional UB: try to modify a string literal
  s[0] = 'h';
  printf("After modification attempt: %s\n", s);
}

int main(void) {
  int *arr = NULL;
  make_array(&arr, 5);
  printf("Heap array @ %p: ", (void *)arr);
  for (int i = 0; i < 5; ++i)
    printf("%d ", arr[i]);
  printf("\n");

  // Show pointer & sizeof behaviors
  int local_arr[4];
  printf("sizeof(local_arr)=%zu sizeof(&local_arr)=%zu "
         "sizeof((int*)local_arr)=%zu\n",
         sizeof(local_arr), sizeof(&local_arr), sizeof((int *)local_arr));

  buggy_function();

  // Keep program alive for interactive debugging / watchpoints
  printf("Setting sentinel variable and entering pause. addr of arr=%p\n",
         (void *)arr);
  volatile int sentinel = 0;
  while (sentinel == 0) { /* spin until you change sentinel in gdb */
  }

  // After sentinel modified to nonzero, try to modify literal
  try_modify_literal();

  free(arr);
  return 0;
}
