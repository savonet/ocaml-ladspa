SOURCES=amp.ml
RESULT=amp
INCDIRS=../../src
LIBS=bigarray ladspa
OCAMLMAKEFILE=../OCamlMakefile

test: all
	./amp

valgrind: all
	valgrind --leak-check=full --show-reachable=yes ./amp

all: nc

include $(OCAMLMAKEFILE)
