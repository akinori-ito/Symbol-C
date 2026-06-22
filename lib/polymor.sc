//
//	Symbolic-C polymorphism support package
//

#include	<stdio.h>

extern char	leftParenChar,rightParenChar;

SYMLIST readCell(stream)
FILE	*stream;
{	char	c;
	SYMLIST	sc = NIL;

	while ((c = fgetc(stream)) == ' ' || c == '\t' || c == '\n');
	ungetc(c,stream);
	if (c == leftParenChar) {
		setq(sc,readList(stream));
	}
	else {
		setq(sc,readSym(stream));
	}
	return valueOf(sc);
}

printCell(c,stream)
CELL	c;
FILE	*stream;
{
	if (atom(c))
		printSym(c.s,stream);
	else {
		count(c.ptr);
		printList(c.ptr,stream);
		collect(c.ptr);
	}
}

printCelln(c,stream)
CELL	c;
FILE	*stream;
{
	printCell(c,stream);
	putchar('\n');
}
