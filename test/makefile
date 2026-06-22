.SUFFIXES:.sc .c .s .o
CFLAGS=-g
.sc.o:
	../src/symcc2 -c $(CFLAGS) $*.sc
	rm $*.c
.c.o:
	cc -c $(CFLAGS) $*.c

all: aho test2

aho: aho.o
	../src/symcc2 -o aho aho.o

test2: test2.o
	../src/symcc2 -o test2 test2.o
