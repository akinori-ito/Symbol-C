.SUFFIXES:.sc .c .s .o
OBJECTS=symrun.o list.o polymor.o collect.o bfunc.o efunc.o
CFLAGS=-g
.sc.o:
	../src/symcc2 -c $(CFLAGS) $*.sc
	rm $*.c
.c.o:
	cc -c $(CFLAGS) $*.c
.s.o:
	as -o $*.o $*.s

../symrun.a: $(OBJECTS)
	ar rvus ../symrun.a $(OBJECTS);\
#	ranlib ../symrun.a
list.o polymor.o collect.o bfunc.o efunc.o: symrun.h
clean:
	rm $(OBJECTS)
