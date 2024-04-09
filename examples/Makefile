ML = $(wildcard *.ml)
PROG = $(ML:.ml=)

all: test

build:
	@dune build

test: build
	@dune exec ./amp.exe
	@dune exec ./inspect.exe
