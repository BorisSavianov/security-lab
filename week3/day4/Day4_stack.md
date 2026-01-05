# week03_stack.md

## Goal
Understand function prologue/epilogue and stack frame layout for x86_64 SysV; map C source to assembly and inspect locals, saved RBP, and return address using gdb/pwndbg.

## Tools
gcc, gdb, pwndbg

## Source file
`c_refresh/stack_mini.c`

## Compile command
gcc -g -O0 -fno-omit-frame-pointer -fno-stack-protector -no-pie -o stack_mini stack_mini.c

## Commands used
gdb ./stack_mini
break f
run
disassemble f
info registers
info frame
x/6xg $rbp
p a
p b
p &buf
x/4dw &buf
set var buf[0] = 42
p buf[0]
continue
quit

## Findings

### disassemble f output:

```assembly
Dump of assembler code for function f:
   0x0000000000401130 <+0>:     push   rbp // Save old frame pointer
   0x0000000000401131 <+1>:     mov    rbp,rsp // Set new frame pointer
   0x0000000000401134 <+4>:     mov    DWORD PTR [rbp-0x4],edi // Save argument a
   0x0000000000401137 <+7>:     mov    DWORD PTR [rbp-0x8],esi // Save argument b
=> 0x000000000040113a <+10>:    mov    eax,DWORD PTR [rbp-0x4] // Load argument a into eax
   0x000000000040113d <+13>:    mov    DWORD PTR [rbp-0x20],eax // Save argument a into local variable buf[0]
   0x0000000000401140 <+16>:    mov    eax,DWORD PTR [rbp-0x8] // Load argument b into eax
   0x0000000000401143 <+19>:    mov    DWORD PTR [rbp-0x1c],eax // Save argument b into local variable buf[1]
   0x0000000000401146 <+22>:    mov    eax,DWORD PTR [rbp-0x4] // Load argument a into eax
   0x0000000000401149 <+25>:    add    eax,DWORD PTR [rbp-0x8] // Add argument a and argument b
   0x000000000040114c <+28>:    mov    DWORD PTR [rbp-0x18],eax // Save argument a + argument b into local variable buf[2]
   0x000000000040114f <+31>:    mov    eax,DWORD PTR [rbp-0x4] // Load argument a into eax
   0x0000000000401152 <+34>:    sub    eax,DWORD PTR [rbp-0x8] // Subtract argument b from argument a
   0x0000000000401155 <+37>:    mov    DWORD PTR [rbp-0x14],eax // Save argument a - argument b into local variable buf[3]
   0x0000000000401158 <+40>:    mov    eax,DWORD PTR [rbp-0x20] // Load local variable buf[0] into eax
   0x000000000040115b <+43>:    add    eax,DWORD PTR [rbp-0x1c] // Add local variable buf[1] and local variable buf[2]
   0x000000000040115e <+46>:    pop    rbp // Restore old frame pointer
   0x000000000040115f <+47>:    ret // Return
End of assembler dump.
```

### info frame output:

```assembly
Stack level 0, frame at 0x7fffffffe080:
 rip = 0x40113a in f (stack.c:5); saved rip = 0x40117e
 called by frame at 0x7fffffffe0a0
 source language c.
 Arglist at 0x7fffffffe070, args: a=5, b=7
 Locals at 0x7fffffffe070, Previous frame's sp is 0x7fffffffe080
 Saved registers:
  rbp at 0x7fffffffe070, rip at 0x7fffffffe078
```

### x/6xg $rbp output (showing saved RBP and return address):

```assembly
0x7fffffffe070: 0x00007fffffffe090      0x000000000040117e
0x7fffffffe080: 0x0000000000000000      0x00000000f7fe1e90
0x7fffffffe090: 0x00007fffffffe130      0x00007ffff7c27635
```

### p &buf:

```assembly
$1 = (int (*)[4]) 0x7fffffffe050
```

### x/4dw &buf (contents of buffer):

```assembly
0x7fffffffe050: 0       0       0       0
```

### After set var buf[0] = 42:

```assembly
0x7fffffffe050: 42      0       0       0
```

### Stack layout:

```
Higher addresses
────────────────────────
rbp + 0x08 : return address
rbp + 0x00 : saved caller rbp
────────────────────────
rbp - 0x04 : int a   (spilled from edi)
rbp - 0x08 : int b   (spilled from esi)
rbp - 0x14 : buf[3]
rbp - 0x18 : buf[2]
rbp - 0x1c : buf[1]
rbp - 0x20 : buf[0]
────────────────────────
Lower addresses
```

### Stack layout after set var buf[0] = 42:

```
Higher addresses
────────────────────────
rbp + 0x08 : return address
rbp + 0x00 : saved caller rbp
────────────────────────
rbp - 0x14 : buf[3]
rbp - 0x18 : buf[2]
rbp - 0x1c : buf[1]
rbp - 0x20 : buf[0] = 42
────────────────────────
Lower addresses
```

---
