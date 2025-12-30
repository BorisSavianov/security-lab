Day 1 Lab — Pointer & Array Exploration
Goal

Reinforce pointer knowledge, pointer-to-pointer allocation, arrays vs pointers, and common undefined behaviors.

Practice gdb/pwndbg memory inspection.

Tools used

Compiler: clang -g -O0 -fno-omit-frame-pointer -fno-stack-protector -no-pie pointer_lab.c

Debugger: gdb ./pointer_lab + pwndbg commands: x/6gx, telescope, info locals, ptype, watch, set var.

Lab program overview

Heap allocation: make_array uses pointer-to-pointer to allocate 5 ints on the heap.

Pointer/array behavior: prints addresses and sizeof results.

Buggy function: off-by-one stack write (buf[2] = 0x41414141) demonstrates stack memory corruption.

String literal modification: tries to write to s[0] (undefined behavior, will crash if uncommented in real literals).

Interactive sentinel: pause to experiment with watchpoints and modifying variables in gdb.

Program output (example)
Heap array @ 0x55b2f54092a0: 0 10 20 30 40 
sizeof(local_arr)=16 sizeof(&local_arr)=8 sizeof((int*)local_arr)=8
Inside buggy_function: small=0xdeadbeef, buf[0..2]=1 2 1094795585
Setting sentinel variable and entering pause. addr of arr=0x55b2f54092a0
String literal address: 0x55b2f5409010 contents: HELLO_LITERAL
After modification attempt: hELLO_LITERAL




Key findings

Heap memory allocated correctly via pointer-to-pointer.

Array decays to pointer in most expressions; sizeof distinguishes actual array vs pointer.

Off-by-one bug overwrites stack memory — easily visible in gdb/pwndbg.

String literals reside in modifiable arrays if declared as char s[]; directly modifying literals is UB.

Watchpoints and telescope are useful to observe memory changes interactively.

Lessons learned

Pointers and arrays behave subtly differently — types and sizes matter.

Heap vs stack memory: allocations are separate and observable in gdb.

Off-by-one errors can corrupt memory and are common sources of vulnerabilities.

pwndbg commands simplify inspection of stack, heap, and variables.

Interactive debugging builds intuition about program memory layout.