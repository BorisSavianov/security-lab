# week03_overview.md

## Goal
Combined evidence demonstrating pointer, heap, linked-list, and stack labs from week03. Includes one scripted gdb session and essential outputs.

## Tools
gcc, gdb, pwndbg

## Commands (full gdb script)
```gdb
# gdb_session_week03.gdb — scripted session for week03
set pagination off
set logging file gdb_session_week03.txt
set logging on

# ========== pointers_demo ==========
file ../day1/pointers_demo
# breakpoint at main, inspect arr/p
b main
run
# inspect locals & array
info locals
print p
ptype arr
x/3d arr
# modify nothing here, just evidence
continue
# ========== dyn_array ==========
file ../day2/dyn_array
b main
run
# inspect heap pointer a before realloc
next
next
print a
x/3d a
vmmap
# breakpoint before realloc (line number may vary) — use source-level breakpoint if available
# assume realloc at line 20; adjust if different
b 20
continue
# print addresses before and after
print a
# step over realloc
next
print a
x/5d a
continue
# ========== linked_list (UAF demo) ==========
file ../day3/linked_list
b main
run
# stop at free(head)
b linked_list.c:54
continue
# verify head before free
print head
print *head
x/2dw head
# execute free(head)
b linked_list.c:55
continue
# use-after-free read
print head
print *head
x/2gx head
# execute free(head)
b linked_list.c:56
continue
# use-after-free read
print head
print *head
x/2gx head
# allocator metadata visible
vmmap
# force reuse of freed chunk
set $p = (node *)malloc(sizeof(node))
call (void*) memset($p, 0x41, sizeof(node))
print $p
# confirm same address reused
print head
x/16bx head
# valid pointer for contrast (not UAF)
print dangling
print *dangling
# ========== stack_demo ==========
file ../day4/stack_mini
b f
run
# disassemble function
disassemble f
step
# inspect stack frame
info frame
info registers
print/x $rbp
x/20xg $rbp
p &buf
x/4dw &buf
# modify local then check
set var buf[0] = 42
print buf[0]
continue

set logging off
quit

```

## Key outputs 
### pointer_demo: addresses, types and values
```gdb
arr = {0, 0, -134340976}
p = 0x0
arr2 = 0x0
$1 = (int *) 0x0
type = int [3]
0x7fffffffe080:	0	0	-134340976
```

### dyn_array: a before/after realloc and vmmap
```gdb
$2 = (int *) 0x405310
0x405310:	0	0	0
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
             Start                End Perm     Size  Offset File (set vmmap-prefer-relpaths on)
          0x400000           0x401000 r--p     1000       0 ../day2/dyn_array
          0x401000           0x402000 r-xp     1000    1000 ../day2/dyn_array
          0x402000           0x403000 r--p     1000    2000 ../day2/dyn_array
          0x403000           0x404000 r--p     1000    2000 ../day2/dyn_array
          0x404000           0x405000 rw-p     1000    3000 ../day2/dyn_array
          0x405000           0x426000 rw-p    21000       0 [heap]
    0x7ffff7c00000     0x7ffff7c24000 r--p    24000       0 /usr/lib/libc.so.6
    0x7ffff7c24000     0x7ffff7d95000 r-xp   171000   24000 /usr/lib/libc.so.6
    0x7ffff7d95000     0x7ffff7e04000 r--p    6f000  195000 /usr/lib/libc.so.6
    0x7ffff7e04000     0x7ffff7e08000 r--p     4000  203000 /usr/lib/libc.so.6
    0x7ffff7e08000     0x7ffff7e0a000 rw-p     2000  207000 /usr/lib/libc.so.6
    0x7ffff7e0a000     0x7ffff7e12000 rw-p     8000       0 [anon_7ffff7e0a]
    0x7ffff7f8d000     0x7ffff7f92000 rw-p     5000       0 [anon_7ffff7f8d]
    0x7ffff7fba000     0x7ffff7fbe000 r--p     4000       0 [vvar]
    0x7ffff7fbe000     0x7ffff7fc0000 r--p     2000       0 [vvar_vclock]
    0x7ffff7fc0000     0x7ffff7fc2000 r-xp     2000       0 [vdso]
    0x7ffff7fc2000     0x7ffff7fc3000 r--p     1000       0 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7fc3000     0x7ffff7fed000 r-xp    2a000    1000 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7fed000     0x7ffff7ffb000 r--p     e000   2b000 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7ffb000     0x7ffff7ffd000 r--p     2000   39000 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7ffd000     0x7ffff7ffe000 rw-p     1000   3b000 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7ffe000     0x7ffff7fff000 rw-p     1000       0 [anon_7ffff7ffe]
    0x7ffffffde000     0x7ffffffff000 rw-p    21000       0 [stack]
0xffffffffff600000 0xffffffffff601000 --xp     1000       0 [vsyscall]
Breakpoint 3 at 0x40121a: file dyn_array.c, line 20.

Breakpoint 3, main () at dyn_array.c:20
20	in dyn_array.c
$3 = (int *) 0x405310
21	in dyn_array.c
$4 = (int *) 0x405310
0x405310:	0	10	20	0
0x405320:	0
```

### linked_list: head, *head, UAF demonstration (malloc reuse)
```gdb
Breakpoint 6, main () at linked_list.c:55
55	  free(head);                  // free first node
$7 = (node *) 0x405390
$8 = {
  v = 99,
  next = 0x405370
}
0x405390:	0x0000000000000063	0x0000000000405370
Breakpoint 7 at 0x4012ff: file linked_list.c, line 56.

Breakpoint 7, main () at linked_list.c:56
56	  head = NULL;
$9 = (node *) 0x405390
$10 = {
  v = 1029,
  next = 0x5d53d7a373603aa7
}
0x405390:	0x0000000000000405	0x5d53d7a373603aa7
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
             Start                End Perm     Size  Offset File (set vmmap-prefer-relpaths on)
          0x400000           0x401000 r--p     1000       0 ../day3/linked_list
          0x401000           0x402000 r-xp     1000    1000 ../day3/linked_list
          0x402000           0x403000 r--p     1000    2000 ../day3/linked_list
          0x403000           0x404000 r--p     1000    2000 ../day3/linked_list
          0x404000           0x405000 rw-p     1000    3000 ../day3/linked_list
          0x405000           0x426000 rw-p    21000       0 [heap]
    0x7ffff7c00000     0x7ffff7c24000 r--p    24000       0 /usr/lib/libc.so.6
    0x7ffff7c24000     0x7ffff7d95000 r-xp   171000   24000 /usr/lib/libc.so.6
    0x7ffff7d95000     0x7ffff7e04000 r--p    6f000  195000 /usr/lib/libc.so.6
    0x7ffff7e04000     0x7ffff7e08000 r--p     4000  203000 /usr/lib/libc.so.6
    0x7ffff7e08000     0x7ffff7e0a000 rw-p     2000  207000 /usr/lib/libc.so.6
    0x7ffff7e0a000     0x7ffff7e12000 rw-p     8000       0 [anon_7ffff7e0a]
    0x7ffff7f8d000     0x7ffff7f92000 rw-p     5000       0 [anon_7ffff7f8d]
    0x7ffff7fba000     0x7ffff7fbe000 r--p     4000       0 [vvar]
    0x7ffff7fbe000     0x7ffff7fc0000 r--p     2000       0 [vvar_vclock]
    0x7ffff7fc0000     0x7ffff7fc2000 r-xp     2000       0 [vdso]
    0x7ffff7fc2000     0x7ffff7fc3000 r--p     1000       0 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7fc3000     0x7ffff7fed000 r-xp    2a000    1000 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7fed000     0x7ffff7ffb000 r--p     e000   2b000 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7ffb000     0x7ffff7ffd000 r--p     2000   39000 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7ffd000     0x7ffff7ffe000 rw-p     1000   3b000 /usr/lib/ld-linux-x86-64.so.2
    0x7ffff7ffe000     0x7ffff7fff000 rw-p     1000       0 [anon_7ffff7ffe]
    0x7ffffffde000     0x7ffffffff000 rw-p    21000       0 [stack]
0xffffffffff600000 0xffffffffff601000 --xp     1000       0 [vsyscall]
$11 = (void *) 0x405390
$12 = (node *) 0x405390
$13 = (node *) 0x405390
0x405390:	0x41	0x41	0x41	0x41	0x41	0x41	0x41	0x41
0x405398:	0x41	0x41	0x41	0x41	0x41	0x41	0x41	0x41
$14 = (node *) 0x405370
$15 = {
  v = 3,
  next = 0x405350
}
```

### stack_demo: disassemble sub, prologue lines, x/20xg $rbp
```gdb
Breakpoint 1, main () at stack.c:13
13	  int r = f(5, 7);
Dump of assembler code for function f:
   0x0000000000401130 <+0>:	push   rbp
   0x0000000000401131 <+1>:	mov    rbp,rsp
   0x0000000000401134 <+4>:	mov    DWORD PTR [rbp-0x4],edi
   0x0000000000401137 <+7>:	mov    DWORD PTR [rbp-0x8],esi
   0x000000000040113a <+10>:	mov    eax,DWORD PTR [rbp-0x4]
   0x000000000040113d <+13>:	mov    DWORD PTR [rbp-0x20],eax
   0x0000000000401140 <+16>:	mov    eax,DWORD PTR [rbp-0x8]
   0x0000000000401143 <+19>:	mov    DWORD PTR [rbp-0x1c],eax
   0x0000000000401146 <+22>:	mov    eax,DWORD PTR [rbp-0x4]
   0x0000000000401149 <+25>:	add    eax,DWORD PTR [rbp-0x8]
   0x000000000040114c <+28>:	mov    DWORD PTR [rbp-0x18],eax
   0x000000000040114f <+31>:	mov    eax,DWORD PTR [rbp-0x4]
   0x0000000000401152 <+34>:	sub    eax,DWORD PTR [rbp-0x8]
   0x0000000000401155 <+37>:	mov    DWORD PTR [rbp-0x14],eax
   0x0000000000401158 <+40>:	mov    eax,DWORD PTR [rbp-0x20]
   0x000000000040115b <+43>:	add    eax,DWORD PTR [rbp-0x1c]
   0x000000000040115e <+46>:	pop    rbp
   0x000000000040115f <+47>:	ret
End of assembler dump.
Breakpoint 8, f (a=5, b=7) at stack.c:5
5	  buf[0] = a;
Stack level 0, frame at 0x7fffffffe080:
 rip = 0x40113a in f (stack.c:5); saved rip = 0x40117e
 called by frame at 0x7fffffffe0a0
 source language c.
 Arglist at 0x7fffffffe070, args: a=5, b=7
 Locals at 0x7fffffffe070, Previous frame's sp is 0x7fffffffe080
 Saved registers:
 rbp at 0x7fffffffe070, rip at 0x7fffffffe078
```

## Variable modification proof
```gdb
$17 = (int (*)[4]) 0x7fffffffe050
0x7fffffffe050:	0	0	0	0
$18 = 42
```

## Self assessment
- Pointers: 8/10
- Heap: 9/10
- Stack: 9/10

## Lessons learned
- Verified how pointers, arrays, and types map to concrete memory addresses and how to inspect them reliably in gdb.
- Observed heap behavior across `malloc`/`realloc`/`free`, including chunk reuse and how use-after-free manifests in allocator metadata and reused memory.
- Understood stack frame layout at the assembly level, including function prologue/epilogue, local variable offsets from `rbp`, and direct stack modification via gdb.
