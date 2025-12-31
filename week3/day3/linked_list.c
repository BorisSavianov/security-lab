// linked_list.c
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int v;
  struct node *next;
} node;

node *push(node *head, int val) {
  node *n = malloc(sizeof(node));
  if (!n) {
    perror("malloc");
    exit(1);
  }
  n->v = val;
  n->next = head;
  return n;
}

void print_list(node *head) {
  node *cur = head;
  while (cur) {
    printf("%d -> ", cur->v);
    cur = cur->next;
  }
  printf("NULL\n");
}

void free_list(node *head) {
  while (head) {
    node *tmp = head->next;
    free(head);
    head = tmp;
  }
}

int main(void) {
  node *head = NULL;

  for (int i = 0; i < 5; ++i)
    head = push(head, i);

  printf("Initial list: ");
  print_list(head);

  // Modify a node in place: change head->v
  if (head)
    head->v = 99;
  printf("After changing head->v = 99: ");
  print_list(head);

  // Demonstrate freeing and leaving a dangling pointer for inspection:
  node *dangling = head->next; // second node
  free(head);                  // free first node
  head = NULL;

  printf("Freed head (first node); 'dangling' still points to second node: ");
  if (dangling)
    printf("dangling->v=%d\n", dangling->v);
  else
    printf("dangling is NULL\n");

  // free remaining nodes properly
  free_list(dangling);

  return 0;
}
