SOURCES=sc4.ml
RESULT=sc4
INCDIRS=../../src
LIBS=bigarray ladspa
OCAMLMAKEFILE=../OCamlMakefile

test: all
	./sc4

gdb: all
	gdb --batch -ex run -ex where ./sc4

valgrind: all
	valgrind --leak-check=full --show-reachable=yes ./sc4

all: nc

include $(OCAMLMAKEFILE)
