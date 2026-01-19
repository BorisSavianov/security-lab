# Learn x86-64 assembly

## regs_demo.c
### Disassembly
```bash
pwndbg> disass main
Dump of assembler code for function main:
   0x0000555555555139 <+0>:     push   rbp
   0x000055555555513a <+1>:     mov    rbp,rsp
   0x000055555555513d <+4>:     sub    rsp,0x10
   0x0000555555555141 <+8>:     mov    DWORD PTR [rbp-0xc],0x2
   0x0000555555555148 <+15>:    mov    DWORD PTR [rbp-0x8],0x3
   0x000055555555514f <+22>:    mov    edx,DWORD PTR [rbp-0xc]
   0x0000555555555152 <+25>:    mov    eax,DWORD PTR [rbp-0x8]
   0x0000555555555155 <+28>:    add    eax,edx
   0x0000555555555157 <+30>:    mov    DWORD PTR [rbp-0x4],eax
   0x000055555555515a <+33>:    mov    eax,DWORD PTR [rbp-0x4]
   0x000055555555515d <+36>:    lea    rdx,[rip+0xea0]        # 0x555555556004
   0x0000555555555164 <+43>:    mov    esi,eax
   0x0000555555555166 <+45>:    mov    rdi,rdx
   0x0000555555555169 <+48>:    mov    eax,0x0
   0x000055555555516e <+53>:    call   0x555555555030 <printf@plt>
   0x0000555555555173 <+58>:    mov    eax,0x0
   0x0000555555555178 <+63>:    leave
   0x0000555555555179 <+64>:    ret
End of assembler dump.
```

### run the program
```bash
pwndbg> run
Starting program: /home/boris/security-lab/week4/day1/regs_demo 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib/libthread_db.so.1".

Breakpoint 1, main () at regs_demo.c:5
5         int c = a + b;
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]─────────────────────────────────────────────────
 RAX  0x7ffff7e0fe28 (environ) —▸ 0x7fffffffe1e8 —▸ 0x7fffffffe5df ◂— 'SHELL=/usr/bin/bash'
 RBX  0
 RCX  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550e0 (__do_global_dtors_aux) ◂— endbr64
 RDX  0x7fffffffe1e8 —▸ 0x7fffffffe5df ◂— 'SHELL=/usr/bin/bash'
 RDI  1
 RSI  0x7fffffffe1d8 —▸ 0x7fffffffe5b1 ◂— '/home/boris/security-lab/week4/day1/regs_demo'
 R8   0x7ffff7e08680 (__exit_funcs) —▸ 0x7ffff7e09fe0 (initial) ◂— 0
 R9   0x7ffff7e09fe0 (initial) ◂— 0
 R10  0x7fffffffddf0 ◂— 0
 R11  0x203
 R12  0x7fffffffe1d8 —▸ 0x7fffffffe5b1 ◂— '/home/boris/security-lab/week4/day1/regs_demo'
 R13  1
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe2f0 —▸ 0x555555554000 ◂— 0x10102464c457f
 R15  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550e0 (__do_global_dtors_aux) ◂— endbr64
 RBP  0x7fffffffe0b0 —▸ 0x7fffffffe150 —▸ 0x7fffffffe1b0 ◂— 0
 RSP  0x7fffffffe0a0 ◂— 0x200000000
 RIP  0x55555555514f (main+22) ◂— mov edx, dword ptr [rbp - 0xc]
─────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────
 ► 0x55555555514f <main+22>    mov    edx, dword ptr [rbp - 0xc]     EDX, [{a}] => 2
   0x555555555152 <main+25>    mov    eax, dword ptr [rbp - 8]       EAX, [{b}] => 3
   0x555555555155 <main+28>    add    eax, edx                       EAX => 5 (3 + 2)
   0x555555555157 <main+30>    mov    dword ptr [rbp - 4], eax       [{c}] <= 5
b+ 0x55555555515a <main+33>    mov    eax, dword ptr [rbp - 4]       EAX, [{c}] => 5
   0x55555555515d <main+36>    lea    rdx, [rip + 0xea0]             RDX => 0x555555556004 ◂— 0xa64253d63 /* 'c=%d\n' */
   0x555555555164 <main+43>    mov    esi, eax                       ESI => 5
   0x555555555166 <main+45>    mov    rdi, rdx                       RDI => 0x555555556004 ◂— 0xa64253d63 /* 'c=%d\n' */
   0x555555555169 <main+48>    mov    eax, 0                         EAX => 0
   0x55555555516e <main+53>    call   printf@plt                  <printf@plt>
 
   0x555555555173 <main+58>    mov    eax, 0       EAX => 0
───────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────
In file: /home/boris/security-lab/week4/day1/regs_demo.c:5
   1 #include <stdio.h>
   2 
   3 int main(void) {
   4   int a = 2, b = 3;
 ► 5   int c = a + b;
   6   printf("c=%d\n", c);
   7   return 0;
   8 }
───────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe0a0 ◂— 0x200000000
01:0008│-008 0x7fffffffe0a8 {b} ◂— 0x7fff00000003
02:0010│ rbp 0x7fffffffe0b0 —▸ 0x7fffffffe150 —▸ 0x7fffffffe1b0 ◂— 0
03:0018│+008 0x7fffffffe0b8 —▸ 0x7ffff7c27635 (__libc_start_call_main+117) ◂— mov edi, eax
04:0020│+010 0x7fffffffe0c0 —▸ 0x7ffff7fc2000 ◂— 0x3010102464c457f
05:0028│+018 0x7fffffffe0c8 —▸ 0x7fffffffe1d8 —▸ 0x7fffffffe5b1 ◂— '/home/boris/security-lab/week4/day1/regs_demo'
06:0030│+020 0x7fffffffe0d0 ◂— 0x1ffffe110
07:0038│+028 0x7fffffffe0d8 —▸ 0x555555555139 (main) ◂— push rbp
─────────────────────────────────────────────────────────────────────[ BACKTRACE ]─────────────────────────────────────────────────────────────────────
 ► 0   0x55555555514f main+22
   1   0x7ffff7c27635 __libc_start_call_main+117
   2   0x7ffff7c276e9 __libc_start_main+137
   3   0x555555555065 _start+37
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
pwndbg> x $rbp - 4
0x7fffffffe0ac: 0x00007fff
```
We see that the value of c is stored in the stack at the address $rbp - 4
For now we can see that it has some random value (not set by our program logic)
We continue the execution until the breakpoint is hit again

```bash
pwndbg> continue
Continuing.

Breakpoint 2, main () at regs_demo.c:6
6         printf("c=%d\n", c);
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]─────────────────────────────────────────────────
*RAX  5
 RBX  0
 RCX  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550e0 (__do_global_dtors_aux) ◂— endbr64
*RDX  2
 RDI  1
 RSI  0x7fffffffe1d8 —▸ 0x7fffffffe5b1 ◂— '/home/boris/security-lab/week4/day1/regs_demo'
 R8   0x7ffff7e08680 (__exit_funcs) —▸ 0x7ffff7e09fe0 (initial) ◂— 0
 R9   0x7ffff7e09fe0 (initial) ◂— 0
 R10  0x7fffffffddf0 ◂— 0
 R11  0x203
 R12  0x7fffffffe1d8 —▸ 0x7fffffffe5b1 ◂— '/home/boris/security-lab/week4/day1/regs_demo'
 R13  1
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe2f0 —▸ 0x555555554000 ◂— 0x10102464c457f
 R15  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550e0 (__do_global_dtors_aux) ◂— endbr64
 RBP  0x7fffffffe0b0 —▸ 0x7fffffffe150 —▸ 0x7fffffffe1b0 ◂— 0
 RSP  0x7fffffffe0a0 ◂— 0x200000000
*RIP  0x55555555515a (main+33) ◂— mov eax, dword ptr [rbp - 4]
─────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────
b+ 0x55555555514f <main+22>    mov    edx, dword ptr [rbp - 0xc]     EDX, [{a}] => 2
   0x555555555152 <main+25>    mov    eax, dword ptr [rbp - 8]       EAX, [{b}] => 3
   0x555555555155 <main+28>    add    eax, edx                       EAX => 5 (3 + 2)
   0x555555555157 <main+30>    mov    dword ptr [rbp - 4], eax       [{c}] <= 5
 ► 0x55555555515a <main+33>    mov    eax, dword ptr [rbp - 4]       EAX, [{c}] => 5
   0x55555555515d <main+36>    lea    rdx, [rip + 0xea0]             RDX => 0x555555556004 ◂— 0xa64253d63 /* 'c=%d\n' */
   0x555555555164 <main+43>    mov    esi, eax                       ESI => 5
   0x555555555166 <main+45>    mov    rdi, rdx                       RDI => 0x555555556004 ◂— 0xa64253d63 /* 'c=%d\n' */
   0x555555555169 <main+48>    mov    eax, 0                         EAX => 0
   0x55555555516e <main+53>    call   printf@plt                  <printf@plt>
 
   0x555555555173 <main+58>    mov    eax, 0       EAX => 0
───────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────
In file: /home/boris/security-lab/week4/day1/regs_demo.c:6
   1 #include <stdio.h>
   2 
   3 int main(void) {
   4   int a = 2, b = 3;
   5   int c = a + b;
 ► 6   printf("c=%d\n", c);
   7   return 0;
   8 }
───────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe0a0 ◂— 0x200000000
01:0008│-008 0x7fffffffe0a8 {b} ◂— 0x500000003
02:0010│ rbp 0x7fffffffe0b0 —▸ 0x7fffffffe150 —▸ 0x7fffffffe1b0 ◂— 0
03:0018│+008 0x7fffffffe0b8 —▸ 0x7ffff7c27635 (__libc_start_call_main+117) ◂— mov edi, eax
04:0020│+010 0x7fffffffe0c0 —▸ 0x7ffff7fc2000 ◂— 0x3010102464c457f
05:0028│+018 0x7fffffffe0c8 —▸ 0x7fffffffe1d8 —▸ 0x7fffffffe5b1 ◂— '/home/boris/security-lab/week4/day1/regs_demo'
06:0030│+020 0x7fffffffe0d0 ◂— 0x1ffffe110
07:0038│+028 0x7fffffffe0d8 —▸ 0x555555555139 (main) ◂— push rbp
─────────────────────────────────────────────────────────────────────[ BACKTRACE ]─────────────────────────────────────────────────────────────────────
 ► 0   0x55555555515a main+33
   1   0x7ffff7c27635 __libc_start_call_main+117
   2   0x7ffff7c276e9 __libc_start_main+137
   3   0x555555555065 _start+37
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
pwndbg> x $rbp - 4
0x7fffffffe0ac: 0x00000005
pwndbg> 
```
This time we see that the value of c after the execution of the add instruction is stored in the stack at the address $rbp - 4

## lea_demo.c
### Disassembly
```bash
pwndbg> disass main
Dump of assembler code for function main:
   0x0000000000001149 <+0>:     push   rbp
   0x000000000000114a <+1>:     mov    rbp,rsp
   0x000000000000114d <+4>:     sub    rsp,0x30
   0x0000000000001151 <+8>:     mov    rax,QWORD PTR fs:0x28
   0x000000000000115a <+17>:    mov    QWORD PTR [rbp-0x8],rax
   0x000000000000115e <+21>:    xor    eax,eax
   0x0000000000001160 <+23>:    lea    rax,[rbp-0x20]
   0x0000000000001164 <+27>:    mov    QWORD PTR [rbp-0x28],rax
   0x0000000000001168 <+31>:    lea    rax,[rbp-0x20]
   0x000000000000116c <+35>:    add    rax,0x8
   0x0000000000001170 <+39>:    mov    QWORD PTR [rbp-0x28],rax
   0x0000000000001174 <+43>:    mov    rax,QWORD PTR [rbp-0x28]
   0x0000000000001178 <+47>:    lea    rdx,[rip+0xe85]        # 0x2004
   0x000000000000117f <+54>:    mov    rsi,rax
   0x0000000000001182 <+57>:    mov    rdi,rdx
   0x0000000000001185 <+60>:    mov    eax,0x0
   0x000000000000118a <+65>:    call   0x1040 <printf@plt>
   0x000000000000118f <+70>:    mov    eax,0x0
   0x0000000000001194 <+75>:    mov    rdx,QWORD PTR [rbp-0x8]
   0x0000000000001198 <+79>:    sub    rdx,QWORD PTR fs:0x28
   0x00000000000011a1 <+88>:    je     0x11a8 <main+95>
   0x00000000000011a3 <+90>:    call   0x1030 <__stack_chk_fail@plt>
   0x00000000000011a8 <+95>:    leave
   0x00000000000011a9 <+96>:    ret
End of assembler dump.
```
### after running the program
```bash
pwndbg> stepi
5         int *p = arr;
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]─────────────────────────────────────────────────
*RAX  0
 RBX  0
 RCX  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550f0 (__do_global_dtors_aux) ◂— endbr64
 RDX  0x7fffffffe1e8 —▸ 0x7fffffffe5e0 ◂— 'SHELL=/usr/bin/bash'
 RDI  1
 RSI  0x7fffffffe1d8 —▸ 0x7fffffffe5b3 ◂— '/home/boris/security-lab/week4/day1/lea_demo'
 R8   0x7ffff7e08680 (__exit_funcs) —▸ 0x7ffff7e09fe0 (initial) ◂— 0
 R9   0x7ffff7e09fe0 (initial) ◂— 0
 R10  0x7fffffffddf0 ◂— 0
 R11  0x203
 R12  0x7fffffffe1d8 —▸ 0x7fffffffe5b3 ◂— '/home/boris/security-lab/week4/day1/lea_demo'
 R13  1
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe2f0 —▸ 0x555555554000 ◂— 0x10102464c457f
 R15  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550f0 (__do_global_dtors_aux) ◂— endbr64
 RBP  0x7fffffffe0b0 —▸ 0x7fffffffe150 —▸ 0x7fffffffe1b0 ◂— 0
 RSP  0x7fffffffe080 ◂— 0
*RIP  0x555555555160 (main+23) ◂— lea rax, [rbp - 0x20]
─────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────
b+ 0x555555555151 <main+8>     mov    rax, qword ptr fs:[0x28]        RAX, [0x7ffff7f8b768] => 0x9e32f5d12f6c0f00
   0x55555555515a <main+17>    mov    qword ptr [rbp - 8], rax        [0x7fffffffe0a8] <= 0x9e32f5d12f6c0f00
   0x55555555515e <main+21>    xor    eax, eax                        EAX => 0
 ► 0x555555555160 <main+23>    lea    rax, [rbp - 0x20]               RAX => 0x7fffffffe090 ◂— 0
   0x555555555164 <main+27>    mov    qword ptr [rbp - 0x28], rax     [{p}] <= 0x7fffffffe090 ◂— 0
   0x555555555168 <main+31>    lea    rax, [rbp - 0x20]               RAX => 0x7fffffffe090 ◂— 0
   0x55555555516c <main+35>    add    rax, 8                          RAX => 0x7fffffffe098 {arr+0x8} (0x7fffffffe090 + 0x8)
   0x555555555170 <main+39>    mov    qword ptr [rbp - 0x28], rax     [{p}] <= 0x7fffffffe098 ◂— 0
   0x555555555174 <main+43>    mov    rax, qword ptr [rbp - 0x28]     RAX, [{p}] => 0x7fffffffe098 ◂— 0
   0x555555555178 <main+47>    lea    rdx, [rip + 0xe85]              RDX => 0x555555556004 ◂— 0x3b031b01000a7025 /* '%p\n' */
   0x55555555517f <main+54>    mov    rsi, rax                        RSI => 0x7fffffffe098 ◂— 0
───────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────
In file: /home/boris/security-lab/week4/day1/lea_demo.c:5
   1 #include <stdio.h>
   2 
   3 int main(void) {
   4   int arr[4];
 ► 5   int *p = arr;
   6   p = &arr[2];
   7   printf("%p\n", (void *)p);
   8   return 0;
   9 }
───────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe080 ◂— 0
... ↓        4 skipped
05:0028│-008 0x7fffffffe0a8 ◂— 0x9e32f5d12f6c0f00
06:0030│ rbp 0x7fffffffe0b0 —▸ 0x7fffffffe150 —▸ 0x7fffffffe1b0 ◂— 0
07:0038│+008 0x7fffffffe0b8 —▸ 0x7ffff7c27635 (__libc_start_call_main+117) ◂— mov edi, eax
─────────────────────────────────────────────────────────────────────[ BACKTRACE ]─────────────────────────────────────────────────────────────────────
 ► 0   0x555555555160 main+23
   1   0x7ffff7c27635 __libc_start_call_main+117
   2   0x7ffff7c276e9 __libc_start_main+137
   3   0x555555555075 _start+37
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
```

We break just before the lea instruction.
We see that rax is still 0.
Let's see what happens after the lea instruction.

```bash
pwndbg> stepi
0x0000555555555164      5         int *p = arr;
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]─────────────────────────────────────────────────
*RAX  0x7fffffffe090 {arr} ◂— 0
 RBX  0
 RCX  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550f0 (__do_global_dtors_aux) ◂— endbr64
 RDX  0x7fffffffe1e8 —▸ 0x7fffffffe5e0 ◂— 'SHELL=/usr/bin/bash'
 RDI  1
 RSI  0x7fffffffe1d8 —▸ 0x7fffffffe5b3 ◂— '/home/boris/security-lab/week4/day1/lea_demo'
 R8   0x7ffff7e08680 (__exit_funcs) —▸ 0x7ffff7e09fe0 (initial) ◂— 0
 R9   0x7ffff7e09fe0 (initial) ◂— 0
 R10  0x7fffffffddf0 ◂— 0
 R11  0x203
 R12  0x7fffffffe1d8 —▸ 0x7fffffffe5b3 ◂— '/home/boris/security-lab/week4/day1/lea_demo'
 R13  1
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe2f0 —▸ 0x555555554000 ◂— 0x10102464c457f
 R15  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550f0 (__do_global_dtors_aux) ◂— endbr64
 RBP  0x7fffffffe0b0 —▸ 0x7fffffffe150 —▸ 0x7fffffffe1b0 ◂— 0
 RSP  0x7fffffffe080 ◂— 0
*RIP  0x555555555164 (main+27) ◂— mov qword ptr [rbp - 0x28], rax
─────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────
b+ 0x555555555151 <main+8>     mov    rax, qword ptr fs:[0x28]        RAX, [0x7ffff7f8b768] => 0x9e32f5d12f6c0f00
   0x55555555515a <main+17>    mov    qword ptr [rbp - 8], rax        [0x7fffffffe0a8] <= 0x9e32f5d12f6c0f00
   0x55555555515e <main+21>    xor    eax, eax                        EAX => 0
   0x555555555160 <main+23>    lea    rax, [rbp - 0x20]               RAX => 0x7fffffffe090 ◂— 0
 ► 0x555555555164 <main+27>    mov    qword ptr [rbp - 0x28], rax     [{p}] <= 0x7fffffffe090 ◂— 0
   0x555555555168 <main+31>    lea    rax, [rbp - 0x20]               RAX => 0x7fffffffe090 ◂— 0
   0x55555555516c <main+35>    add    rax, 8                          RAX => 0x7fffffffe098 {arr+0x8} (0x7fffffffe090 + 0x8)
   0x555555555170 <main+39>    mov    qword ptr [rbp - 0x28], rax     [{p}] <= 0x7fffffffe098 ◂— 0
   0x555555555174 <main+43>    mov    rax, qword ptr [rbp - 0x28]     RAX, [{p}] => 0x7fffffffe098 ◂— 0
   0x555555555178 <main+47>    lea    rdx, [rip + 0xe85]              RDX => 0x555555556004 ◂— 0x3b031b01000a7025 /* '%p\n' */
   0x55555555517f <main+54>    mov    rsi, rax                        RSI => 0x7fffffffe098 ◂— 0
───────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────
In file: /home/boris/security-lab/week4/day1/lea_demo.c:5
   1 #include <stdio.h>
   2 
   3 int main(void) {
   4   int arr[4];
 ► 5   int *p = arr;
   6   p = &arr[2];
   7   printf("%p\n", (void *)p);
   8   return 0;
   9 }
───────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe080 ◂— 0
01:0008│-028 0x7fffffffe088 {p} ◂— 0
02:0010│ rax 0x7fffffffe090 {arr} ◂— 0
... ↓        2 skipped
05:0028│-008 0x7fffffffe0a8 ◂— 0x9e32f5d12f6c0f00
06:0030│ rbp 0x7fffffffe0b0 —▸ 0x7fffffffe150 —▸ 0x7fffffffe1b0 ◂— 0
07:0038│+008 0x7fffffffe0b8 —▸ 0x7ffff7c27635 (__libc_start_call_main+117) ◂— mov edi, eax
─────────────────────────────────────────────────────────────────────[ BACKTRACE ]─────────────────────────────────────────────────────────────────────
 ► 0   0x555555555164 main+27
   1   0x7ffff7c27635 __libc_start_call_main+117
   2   0x7ffff7c276e9 __libc_start_main+137
   3   0x555555555075 _start+37
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
pwndbg> 
```
We now observe that rax is pointing to the address of arr.
Another interesting thing we notice:
`mov rax, QWORD PTR fs:0x28` sets/reads the stack canary; the later `__stack_chk_fail` call is the stack-protector check.

## regs_demo_modified.c
### Disassembly
```bash
pwndbg> disass main
Dump of assembler code for function main:
   0x000000000000114d <+0>:     push   rbp
   0x000000000000114e <+1>:     mov    rbp,rsp
   0x0000000000001151 <+4>:     sub    rsp,0x10
   0x0000000000001155 <+8>:     mov    DWORD PTR [rbp-0xc],0xa
   0x000000000000115c <+15>:    mov    DWORD PTR [rbp-0x8],0x7
   0x0000000000001163 <+22>:    mov    edx,DWORD PTR [rbp-0x8]
   0x0000000000001166 <+25>:    mov    eax,DWORD PTR [rbp-0xc]
   0x0000000000001169 <+28>:    mov    esi,edx
   0x000000000000116b <+30>:    mov    edi,eax
   0x000000000000116d <+32>:    call   0x1139 <add>
   0x0000000000001172 <+37>:    mov    DWORD PTR [rbp-0x4],eax
   0x0000000000001175 <+40>:    mov    eax,DWORD PTR [rbp-0x4]
   0x0000000000001178 <+43>:    lea    rdx,[rip+0xe85]        # 0x2004
   0x000000000000117f <+50>:    mov    esi,eax
   0x0000000000001181 <+52>:    mov    rdi,rdx
   0x0000000000001184 <+55>:    mov    eax,0x0
   0x0000000000001189 <+60>:    call   0x1030 <printf@plt>
   0x000000000000118e <+65>:    mov    eax,0x0
   0x0000000000001193 <+70>:    leave
   0x0000000000001194 <+71>:    ret
End of assembler dump.
```

### after running the program and breaking at the add function
We observe that rdi and rsi are pointing to the arguments of the function (10 and 7)
We also see that eax already has 7 moved into it and edx has 10 moved into it.

```bash
pwndbg> stepi
0x0000555555555149      3       int add(int x, int y) { return x + y; }
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]─────────────────────────────────────────────────
*RAX  7
 RBX  0
 RCX  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550e0 (__do_global_dtors_aux) ◂— endbr64
 RDX  0xa
 RDI  0xa
 RSI  7
 R8   0x7ffff7e08680 (__exit_funcs) —▸ 0x7ffff7e09fe0 (initial) ◂— 0
 R9   0x7ffff7e09fe0 (initial) ◂— 0
 R10  0x7fffffffdde0 ◂— 0
 R11  0x203
 R12  0x7fffffffe1c8 —▸ 0x7fffffffe5a1 ◂— '/home/boris/security-lab/week4/day1/regs_demo_modfied'
 R13  1
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe2f0 —▸ 0x555555554000 ◂— 0x10102464c457f
 R15  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550e0 (__do_global_dtors_aux) ◂— endbr64
 RBP  0x7fffffffe080 —▸ 0x7fffffffe0a0 —▸ 0x7fffffffe140 —▸ 0x7fffffffe1a0 ◂— 0
 RSP  0x7fffffffe080 —▸ 0x7fffffffe0a0 —▸ 0x7fffffffe140 —▸ 0x7fffffffe1a0 ◂— 0
*RIP  0x555555555149 (add+16) ◂— add eax, edx
─────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────
   0x55555555513a <add+1>      mov    rbp, rsp                     RBP => 0x7fffffffe080 —▸ 0x7fffffffe0a0 —▸ 0x7fffffffe140 —▸ 0x7fffffffe1a0 ◂— ...
   0x55555555513d <add+4>      mov    dword ptr [rbp - 4], edi     [{x}] <= 0xa
   0x555555555140 <add+7>      mov    dword ptr [rbp - 8], esi     [{y}] <= 7
   0x555555555143 <add+10>     mov    edx, dword ptr [rbp - 4]     EDX, [{x}] => 0xa
   0x555555555146 <add+13>     mov    eax, dword ptr [rbp - 8]     EAX, [{y}] => 7
 ► 0x555555555149 <add+16>     add    eax, edx                     EAX => 0x11 (0x7 + 0xa)
   0x55555555514b <add+18>     pop    rbp                          RBP => 0x7fffffffe0a0
   0x55555555514c <add+19>     ret                                <main+37>
    ↓
   0x555555555172 <main+37>    mov    dword ptr [rbp - 4], eax     [0x7fffffffe09c] <= 0x11
   0x555555555175 <main+40>    mov    eax, dword ptr [rbp - 4]     EAX, [0x7fffffffe09c] => 0x11
   0x555555555178 <main+43>    lea    rdx, [rip + 0xe85]           RDX => 0x555555556004 ◂— 0xa64253d63 /* 'c=%d\n' */
───────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────
In file: /home/boris/security-lab/week4/day1/regs_demo_modfied.c:3
    1 #include <stdio.h>
    2 
 ►  3 int add(int x, int y) { return x + y; }
    4 
    5 int main(void) {
    6   int a = 10, b = 7;
    7   int c = add(a, b);
    8   printf("c=%d\n", c);
    9   return 0;
   10 }
───────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────
00:0000│ rbp rsp 0x7fffffffe080 —▸ 0x7fffffffe0a0 —▸ 0x7fffffffe140 —▸ 0x7fffffffe1a0 ◂— 0
01:0008│+008     0x7fffffffe088 —▸ 0x555555555172 (main+37) ◂— mov dword ptr [rbp - 4], eax
02:0010│+010     0x7fffffffe090 ◂— 0xa00000000
03:0018│+018     0x7fffffffe098 ◂— 0x7fff00000007
04:0020│+020     0x7fffffffe0a0 —▸ 0x7fffffffe140 —▸ 0x7fffffffe1a0 ◂— 0
05:0028│+028     0x7fffffffe0a8 —▸ 0x7ffff7c27635 (__libc_start_call_main+117) ◂— mov edi, eax
06:0030│+030     0x7fffffffe0b0 —▸ 0x7ffff7fc2000 ◂— 0x3010102464c457f
07:0038│+038     0x7fffffffe0b8 —▸ 0x7fffffffe1c8 —▸ 0x7fffffffe5a1 ◂— '/home/boris/security-lab/week4/day1/regs_demo_modfied'
─────────────────────────────────────────────────────────────────────[ BACKTRACE ]─────────────────────────────────────────────────────────────────────
 ► 0   0x555555555149 add+16
   1   0x555555555172 main+37
   2   0x7ffff7c27635 __libc_start_call_main+117
   3   0x7ffff7c276e9 __libc_start_main+137
   4   0x555555555065 _start+37
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
```

We continue to see the addition.
And after the addition, we see that the second argument (edx) is added with eax and the result is stored in rax (17).
```bash
*RAX  0x11
 RBX  0
 RCX  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550e0 (__do_global_dtors_aux) ◂— endbr64
 RDX  0xa
 RDI  0xa
 RSI  7
 R8   0x7ffff7e08680 (__exit_funcs) —▸ 0x7ffff7e09fe0 (initial) ◂— 0
 R9   0x7ffff7e09fe0 (initial) ◂— 0
 R10  0x7fffffffdde0 ◂— 0
 R11  0x203
 R12  0x7fffffffe1c8 —▸ 0x7fffffffe5a1 ◂— '/home/boris/security-lab/week4/day1/regs_demo_modfied'
 R13  1
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe2f0 —▸ 0x555555554000 ◂— 0x10102464c457f
 R15  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x5555555550e0 (__do_global_dtors_aux) ◂— endbr64
 RBP  0x7fffffffe080 —▸ 0x7fffffffe0a0 —▸ 0x7fffffffe140 —▸ 0x7fffffffe1a0 ◂— 0
 RSP  0x7fffffffe080 —▸ 0x7fffffffe0a0 —▸ 0x7fffffffe140 —▸ 0x7fffffffe1a0 ◂— 0
*RIP  0x55555555514b (add+18) ◂— pop rbp
─────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────
   0x55555555513d <add+4>      mov    dword ptr [rbp - 4], edi     [{x}] <= 0xa
   0x555555555140 <add+7>      mov    dword ptr [rbp - 8], esi     [{y}] <= 7
   0x555555555143 <add+10>     mov    edx, dword ptr [rbp - 4]     EDX, [{x}] => 0xa
   0x555555555146 <add+13>     mov    eax, dword ptr [rbp - 8]     EAX, [{y}] => 7
   0x555555555149 <add+16>     add    eax, edx                     EAX => 0x11 (0x7 + 0xa)
 ► 0x55555555514b <add+18>     pop    rbp                          RBP => 0x7fffffffe0a0
   0x55555555514c <add+19>     ret                                <main+37>
    ↓
   0x555555555172 <main+37>    mov    dword ptr [rbp - 4], eax     [0x7fffffffe09c] <= 0x11
   0x555555555175 <main+40>    mov    eax, dword ptr [rbp - 4]     EAX, [0x7fffffffe09c] => 0x11
   0x555555555178 <main+43>    lea    rdx, [rip + 0xe85]           RDX => 0x555555556004 ◂— 0xa64253d63 /* 'c=%d\n' */
   0x55555555517f <main+50>    mov    esi, eax                     ESI => 0x11
───────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────
In file: /home/boris/security-lab/week4/day1/regs_demo_modfied.c:3
    1 #include <stdio.h>
    2 
 ►  3 int add(int x, int y) { return x + y; }
    4 
    5 int main(void) {
    6   int a = 10, b = 7;
    7   int c = add(a, b);
    8   printf("c=%d\n", c);
    9   return 0;
   10 }
───────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────
00:0000│ rbp rsp 0x7fffffffe080 —▸ 0x7fffffffe0a0 —▸ 0x7fffffffe140 —▸ 0x7fffffffe1a0 ◂— 0
01:0008│+008     0x7fffffffe088 —▸ 0x555555555172 (main+37) ◂— mov dword ptr [rbp - 4], eax
02:0010│+010     0x7fffffffe090 ◂— 0xa00000000
03:0018│+018     0x7fffffffe098 ◂— 0x7fff00000007
04:0020│+020     0x7fffffffe0a0 —▸ 0x7fffffffe140 —▸ 0x7fffffffe1a0 ◂— 0
05:0028│+028     0x7fffffffe0a8 —▸ 0x7ffff7c27635 (__libc_start_call_main+117) ◂— mov edi, eax
06:0030│+030     0x7fffffffe0b0 —▸ 0x7ffff7fc2000 ◂— 0x3010102464c457f
07:0038│+038     0x7fffffffe0b8 —▸ 0x7fffffffe1c8 —▸ 0x7fffffffe5a1 ◂— '/home/boris/security-lab/week4/day1/regs_demo_modfied'
─────────────────────────────────────────────────────────────────────[ BACKTRACE ]─────────────────────────────────────────────────────────────────────
 ► 0   0x55555555514b add+18
   1   0x555555555172 main+37
   2   0x7ffff7c27635 __libc_start_call_main+117
   3   0x7ffff7c276e9 __libc_start_main+137
   4   0x555555555065 _start+37
───────────
```


# Important for all files
`lea rdx, [rip + 0xea0]` loads the address of the format string (RIP-relative data access)