Today we are looking at dynamic memory allocation.

[boris@t480s day2]$ ./dyn_array 
Before realloc: a=0x7a16310
a[0]=0
a[1]=10
a[2]=20
After realloc: a=0x7a16310 (old=0x7a16310)
a[0]=0
a[1]=10
a[2]=20
a[3]=30
a[4]=40
[boris@t480s day2]$ gdb ./dyn_array 

We can see that the array is allocated on the heap.
We assume that because the address of the array is the same before and after the reallocation.

Now we will prove it with pwndbg.
Starting with setting a breakpoint at the main function.
pwndbg> break main

pwndbg> run
Breakpoint 1, main () at dyn_array.c:6
6         int n = 3;
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
─────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]──────────────────────────────────────────────────
 RAX  0x7ffff7e0fe28 (environ) —▸ 0x7fffffffe1c8 —▸ 0x7fffffffe5c5 ◂— 'SHELL=/usr/bin/bash'
 RBX  0
 RCX  0x403df0 —▸ 0x401120 ◂— endbr64
 RDX  0x7fffffffe1c8 —▸ 0x7fffffffe5c5 ◂— 'SHELL=/usr/bin/bash'
 RDI  1
 RSI  0x7fffffffe1b8 —▸ 0x7fffffffe596 ◂— '/home/boris/security-lab/week 3/day2/dyn_array'
 R8   0x7ffff7e08680 (__exit_funcs) —▸ 0x7ffff7e09fe0 (initial) ◂— 0
 R9   0x7ffff7e09fe0 (initial) ◂— 0
 R10  0x7fffffffddd0 ◂— 0
 R11  0x203
 R12  0x7fffffffe1b8 —▸ 0x7fffffffe596 ◂— '/home/boris/security-lab/week 3/day2/dyn_array'
 R13  1
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe2f0 ◂— 0
 R15  0x403df0 —▸ 0x401120 ◂— endbr64
 RBP  0x7fffffffe090 —▸ 0x7fffffffe130 —▸ 0x7fffffffe190 ◂— 0
 RSP  0x7fffffffe060 ◂— 0
 RIP  0x40116f (main+15) ◂— mov dword ptr [rbp - 8], 3
──────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]───────────────────────────────────────────────────────────
 ► 0x40116f <main+15>     mov    dword ptr [rbp - 8], 3       [{n}] <= 3
   0x401176 <main+22>     movsxd rdi, dword ptr [rbp - 8]     RDI, [{n}] => 3
   0x40117a <main+26>     shl    rdi, 2
   0x40117e <main+30>     call   malloc@plt                  <malloc@plt>
 
   0x401183 <main+35>     mov    qword ptr [rbp - 0x10], rax
   0x401187 <main+39>     cmp    qword ptr [rbp - 0x10], 0
   0x40118c <main+44>     jne    main+58                     <main+58>
 
   0x40118e <main+46>     mov    dword ptr [rbp - 4], 1
   0x401195 <main+53>     jmp    main+359                    <main+359>
    ↓
   0x4012c7 <main+359>    mov    eax, dword ptr [rbp - 4]
   0x4012ca <main+362>    add    rsp, 0x30
────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]────────────────────────────────────────────────────────────────────
In file: /home/boris/security-lab/week 3/day2/dyn_array.c:6
    1 // c_refresh/dyn_array.c
    2 #include <stdio.h>
    3 #include <stdlib.h>
    4 
    5 int main(void) {
 ►  6   int n = 3;
    7   int *a = malloc(n * sizeof(int));
    8   if (!a)
    9     return 1;
   10 
   11   for (int i = 0; i < n; i++)
   12     a[i] = i * 10;
   13 
   14   printf("Before realloc: a=%p\n", (void *)a);
────────────────────────────────────────────────────────────────────────[ STACK ]────────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe060 ◂— 0
... ↓        4 skipped
05:0028│-008 0x7fffffffe088 {n} ◂— 0xf7fe1e90
06:0030│ rbp 0x7fffffffe090 —▸ 0x7fffffffe130 —▸ 0x7fffffffe190 ◂— 0
07:0038│+008 0x7fffffffe098 —▸ 0x7ffff7c27635 (__libc_start_call_main+117) ◂— mov edi, eax
──────────────────────────────────────────────────────────────────────[ BACKTRACE ]──────────────────────────────────────────────────────────────────────
 ► 0         0x40116f main+15
   1   0x7ffff7c27635 __libc_start_call_main+117
   2   0x7ffff7c276e9 __libc_start_main+137
   3         0x401095 _start+37
─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────

We skip execution unit the pointer "a" is allocated on the heap.
pwndbg> next
...
pwndbg> next
...

We can see that the pointer "a" is now allocated on the heap.
pwndbg> print n
$2 = 3

We see that that a has the address 0x405310.
pwndbg> print a
$3 = (int *) 0x405310
pwndbg> x/3d a
0x405310:       0       0       0

Looking at the memory map we can see that the heap starts at 0x405000.
That confirms that the pointer "a" is allocated on the heap.
pwndbg> vmmap
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
             Start                End Perm     Size  Offset File (set vmmap-prefer-relpaths on)
          0x400000           0x401000 r--p     1000       0 dyn_array
          0x401000           0x402000 r-xp     1000    1000 dyn_array
          0x402000           0x403000 r--p     1000    2000 dyn_array
          0x403000           0x404000 r--p     1000    2000 dyn_array
          0x404000           0x405000 rw-p     1000    3000 dyn_array
          0x405000           0x426000 rw-p    21000       0 [heap]
    0x7ffff7c00000     0x7ffff7c24000 r--p    24000       0 /usr/lib/libc.so.6
    0x7ffff7c24000     0x7ffff7d95000 r-xp   171000   24000 /usr/lib/libc.so.6
    0x7ffff7d95000     0x7ffff7e04000 r--p    6f000  195000 /usr/lib/libc.so.6
    0x7ffff7e04000     0x7ffff7e08000 r--p     4000  203000 /usr/lib/libc.so.6
    0x7ffff7e08000     0x7ffff7e0a000 rw-p     2000  207000 /usr/lib/libc.so.6
    0x7ffff7e0a000     0x7ffff7e12000 rw-p     8000       0 [anon_7ffff7e0a]
    0x7ffff7f94000     0x7ffff7f99000 rw-p     5000       0 [anon_7ffff7f94]
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
pwndbg> 

We break at line 20 so that we can see the value of "a" before and after the realloc.
pwndbg> break 20
Breakpoint 2 at 0x40121a: file dyn_array.c, line 20.
pwndbg> continue
Continuing.
Before realloc: a=0x405310
a[0]=0
a[1]=10
a[2]=20

Breakpoint 2, main () at dyn_array.c:20
20        a = realloc(a, n * sizeof(int));
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
─────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]──────────────────────────────────────────────────
*RAX  0x405310 ◂— 0xa00000000
 RBX  0
 RCX  0
 RDX  0
*RDI  0x7fffffffde80 —▸ 0x7fffffffdeb0 ◂— 0xa30323d5d325b61 ('a[2]=20\n')
*RSI  0x400
*R8   0
*R9   0
*R10  0
*R11  0x202
 R12  0x7fffffffe1b8 —▸ 0x7fffffffe596 ◂— '/home/boris/security-lab/week 3/day2/dyn_array'
 R13  1
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe2f0 ◂— 0
 R15  0x403df0 —▸ 0x401120 ◂— endbr64
 RBP  0x7fffffffe090 —▸ 0x7fffffffe130 —▸ 0x7fffffffe190 ◂— 0
 RSP  0x7fffffffe060 ◂— 0
*RIP  0x40121a (main+186) ◂— mov rdi, qword ptr [rbp - 0x10]
──────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]───────────────────────────────────────────────────────────
 ► 0x40121a <main+186>    mov    rdi, qword ptr [rbp - 0x10]     RDI, [{a}] => 0x405310 ◂— 0xa00000000
   0x40121e <main+190>    movsxd rsi, dword ptr [rbp - 8]        RSI, [{n}] => 5
   0x401222 <main+194>    shl    rsi, 2
   0x401226 <main+198>    call   realloc@plt                 <realloc@plt>
 
   0x40122b <main+203>    mov    qword ptr [rbp - 0x10], rax
   0x40122f <main+207>    cmp    qword ptr [rbp - 0x10], 0
   0x401234 <main+212>    jne    main+226                    <main+226>
 
   0x401236 <main+214>    mov    dword ptr [rbp - 4], 1
   0x40123d <main+221>    jmp    main+359                    <main+359>
    ↓
   0x4012c7 <main+359>    mov    eax, dword ptr [rbp - 4]
   0x4012ca <main+362>    add    rsp, 0x30
────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]────────────────────────────────────────────────────────────────────
In file: /home/boris/security-lab/week 3/day2/dyn_array.c:20
   15   for (int i = 0; i < n; i++)
   16     printf("a[%d]=%d\n", i, a[i]);
   17 
   18   n = 5;
   19   int *old = a;
 ► 20   a = realloc(a, n * sizeof(int));
   21   if (!a)
   22     return 1;
   23 
   24   for (int i = 3; i < n; i++)
   25     a[i] = i * 10;
   26 
   27   printf("After realloc: a=%p (old=%p)\n", (void *)a, (void *)old);
   28   for (int i = 0; i < n; i++)
────────────────────────────────────────────────────────────────────────[ STACK ]────────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe060 ◂— 0
01:0008│-028 0x7fffffffe068 ◂— 0
02:0010│-020 0x7fffffffe070 {old} —▸ 0x405310 ◂— 0xa00000000
03:0018│-018 0x7fffffffe078 ◂— 0x300000003
04:0020│-010 0x7fffffffe080 {a} —▸ 0x405310 ◂— 0xa00000000
05:0028│-008 0x7fffffffe088 {n} ◂— 5
06:0030│ rbp 0x7fffffffe090 —▸ 0x7fffffffe130 —▸ 0x7fffffffe190 ◂— 0
07:0038│+008 0x7fffffffe098 —▸ 0x7ffff7c27635 (__libc_start_call_main+117) ◂— mov edi, eax
──────────────────────────────────────────────────────────────────────[ BACKTRACE ]──────────────────────────────────────────────────────────────────────
 ► 0         0x40121a main+186
   1   0x7ffff7c27635 __libc_start_call_main+117
   2   0x7ffff7c276e9 __libc_start_main+137
   3         0x401095 _start+37
─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────

We see that the address of "a" has not changed after the realloc.
This is because the realloc function was able to resize the memory block in place.
pwndbg> print a
$4 = (int *) 0x405310
pwndbg> print old
$5 = (int *) 0x405310

Seeing the same address is not always guaranteed; 
Allocator may move the chunk in other runs or different heap conditions. 
Always treat the returned pointer as the canonical one and never use the saved old after a (potential) successful realloc unless you checked it.

Example of correct realloc usage:
int *tmp = realloc(a, n * sizeof *a);
if (!tmp) {
    // handle error; a still valid
} else {
    a = tmp;
}

Lessons learned:

- Always check allocation return values.

- Use safe realloc pattern.

- Freed memory is invalid; free(NULL) is safe.