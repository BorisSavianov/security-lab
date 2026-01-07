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
