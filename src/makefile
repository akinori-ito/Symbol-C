SOURCES=symcc.c hash.c trans.c list.c bfunc.c collect.c symrun.c
OBJECTS=symcc.o hash.o trans.o list.o bfunc.o collect.o symrun.o
CFLAGS=-g -Wall -DSYSV
symcc2: $(OBJECTS)
	cc $(CFLAGS) $(OBJECTS) -o symcc2
$(OBJECTS): symcc.h
lint:
	lint $(SOURCES)
clean:
	rm $(OBJECTS)

