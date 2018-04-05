a.out: termjump.c
	gcc -std=c99 -Wall -Og -g3 -Fdwarf64 -Wextra -pedantic -I${HOME}/.local/include termjump.c -o a.out -ltermbox -L${HOME}/.local/lib

run: a.out
	env LD_LIBRARY_PATH=${HOME}/.local/lib ./$<

.PHONY: run
