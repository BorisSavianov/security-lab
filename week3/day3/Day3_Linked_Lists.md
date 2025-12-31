# week03_linkedlist.md

## Goal
Implement a singly linked list, traverse it, modify nodes at runtime, and inspect heap nodes with gdb/pwndbg.

## Tools
clang, gdb, pwndbg

## Steps (commands used)
- Compile:
  clang -g -O0 -fno-omit-frame-pointer -fno-stack-protector -no-pie -o linked_list linked_list.c
- Run:
  ./linked_list | tee linked_list_stdout.txt
- Debug session:
  gdb ./linked_list
  break main
  run
  break print_list
  continue
  print head
  print *head
  x/2dw 0xADDR      # raw memory dump (replace 0xADDR with node address)
  set ((node*)head)->v = 777
  watch *(int*)head
  continue
  vmmap

## Findings 
- Program stdout:
Initial list: 4 -> 3 -> 2 -> 1 -> 0 -> NULL
After changing head->v = 99: 99 -> 3 -> 2 -> 1 -> 0 -> NULL
Freed head (first node); 'dangling' still points to second node: dangling->v=3

- gdb `print head`:
$1 = (node *) 0x405390

`- gdb `ptype head`:
type = struct node {
    int v;
    struct node *next;
} *

- gdb `print *head`:
$2 = {
  v = 4,
  next = 0x405370
}

- gdb `print head->v`:
$3 = 4

- gdb `print head->next`:
$4 = (struct node *) 0x405370

- gdb `print head->next->v`:
$5 = 3

- gdb `print head->next->next`:
$6 = (struct node *) 0x405350

- Raw memory (`x/2dw 0x405390`):
  0x405390: 0x04 0x00 

- gdb 'set (z(node*)head)->v = 777':
$7 = 777

- gdb 'continue'
Continuing.
Initial list: 77 -> 3 -> 2 -> 1 -> 0 -> NULL

- gdb 'continue'
Continuing.
After changing head->v = 99: 99 -> 3 -> 2 -> 1 -> 0 -> NULL

- gdb 'break 50'

- gdb 'print dangling'
$14 = (node *) 0x405370

- gdb 'print *dangling'
$15 = {
  v = 3,
  next = 0x405350
}

- Watchpoint trigger:        
Hardware watchpoint 4: *(int*)head
Old value = 99
New value = 1029

- vmmap snippet:
  0x405000           0x426000 rw-p    21000       0 [heap]



## Demonstration: Use-After-Free (UAF)

### Step 1: Stop just before `free(head)`

```
(gdb) print head
$1 = (node *) 0x405390

(gdb) print *head
$2 = { v = 4, next = 0x405370 }

(gdb) x/2dw 0x405390
0x405390: 99 0
```

At this point, `head` points to a valid heap node.

---

### Step 2: Step over `free(head)`

```
(gdb) step   # free(head) executed

(gdb) print *head
$3 = { v = 1029, next = 0x70edbe4230a5dfd6 }
```

After `free(head)`, the pointer `head` still holds the same address, but the memory is no longer owned by the program. The contents were overwritten by allocator metadata. Dereferencing `head` at this point is a **use-after-free read**.

---

### Step 3: Allocate a new chunk and overwrite it

```
(gdb) set $pnew = (node *) malloc(sizeof(struct node))
(gdb) call (void*) memset($pnew, 0x41, sizeof(struct node))
(gdb) print $pnew
$4 = (node *) 0x405390
```

The allocator reused the freed chunk at the same address (`0x405390`). Filling it with `0x41` overwrote the memory previously referenced by `head`.

```
(gdb) x/16bx 0x405390
0x405390: 0x41 0x41 0x41 ...
```

---

### Step 4: Compare with a valid pointer

```
(gdb) print dangling
$5 = (node *) 0x405370
```

`dangling` still points to a valid, allocated node and is **not** part of the UAF. The use-after-free occurs only through the freed pointer `head`.

---

### Conclusion

This demonstrates a classic **use-after-free** scenario: after `free(head)`, the pointer `head` still existed and was dereferenced, first showing allocator metadata and later showing data from a new allocation that reused the same memory. Accessing freed memory through such pointers is undefined behavior and can lead to memory corruption or exploitation.

## Lessons Learned
- Nodes are separate heap chunks; follow `next` pointers to traverse.
- Modifying node values at runtime is straightforward (`set ((node*)head)->v = X`).
- Freeing nodes creates dangling pointers; do not dereference freed pointers in real code.

- Always nullify pointers after free (e.g., free(head); head = NULL;) and avoid keeping multiple live pointers to the same heap object unless ownership is clear.


Use tools: AddressSanitizer (ASan), Valgrind (memcheck), and systematic code review to detect UAFs.

Compile-time (ASan): clang -fsanitize=address -g linked_list.c -o linked_list_asan

Hardened allocators and safe patterns (smart pointers in higher-level languages) prevent many errors.