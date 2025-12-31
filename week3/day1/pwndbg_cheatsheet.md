break <func>

info locals - shows local variables and their contents.

print p - shows pointer value and type.

print &arr - shows pointer-to-array type (the address but annotated as int (*)[3]).

x/3d arr - dumps 3 integers starting at arr.

x/3x arr - dumps the 3 integers in hex.

ptype arr - prints the static type of arr as known in the debug info.

finish - runs to function end and returns to caller.

set var <name> = <value> — change variable

finish — run to function return
