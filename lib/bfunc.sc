#include	<stdio.h>
#include	<stdlib.h>

extern SYMLIST	_Strage,_EosPtr;
extern int	_StrgSize;

#define	LPAREN_CHAR	'['
#define	RPAREN_CHAR	']'

#define ATOMCELL(x)	(((unsigned)((x).ptr) & 0xffff0000) == 0xffff0000)

char	leftParenChar = LPAREN_CHAR,
	rightParenChar = RPAREN_CHAR;

char	NilIsCadble = 1,CoreDumpOnError = 0;

#ifdef EOF
#undef EOF
#define EOF 255
#endif

eq(x,y)
register unsigned	x,y;
{
	return (x == y);
}

atom(x)
register unsigned	x;
{
	if ((x & 0xffff0000) == 0xffff0000)
		return 1;
	if (x == NIL)
		return 1;
	return 0;
}

SYMLIST	 car(x)
register union cell	x;
{
	if (x.ptr == NIL) {
		if (NilIsCadble)
			return NIL;
		fprintf(stderr,"Cannot get CAR of NIL\n");
		if (CoreDumpOnError)
			abort();
		exit(1);
	}
	if (ATOMCELL(x)) {
		fprintf(stderr,"Cannot get CAR of symbol ");
		printSym(x,stderr);
		putchar('\n');
		if (CoreDumpOnError)
			abort();
		exit(1);
	}
	return x.ptr->head.ptr;
}

SYMLIST cdr(x)
register union cell	x;
{
	if (x.ptr == NIL) {
		if (NilIsCadble)
			return NIL;
		fprintf(stderr,"Cannot get CDR of NIL\n");
		if (CoreDumpOnError)
			abort();
		exit(1);
	}
	if (ATOMCELL(x)) {
		fprintf(stderr,"Cannot get CDR of symbol ");
		printSym(x,stderr);
		putchar('\n');
		if (CoreDumpOnError)
			abort();
		exit(1);
	}
	return x.ptr->next;
}
	
SYMLIST readList(stream)
FILE	*stream;
{	short	i;
	SYMLIST	s,e = NIL,l;
	char	c,str[32];

	c = fgetc(stream);
	while (c != EOF) {
		if (c != ' ' && c != '\t' && c != '\n') {
			if (c == leftParenChar) {
				if (e != NIL) {
					ungetc(c,stream);
					if (!atom(e->head)
						 || e->head.s != UNDEF) {
						l = getfree(_Strage);
						e->next = l;
						e = l;
						e->count = 1;
					}
					e->head.ptr = NIL;
					setq(e->head.ptr,readList(stream));
					c = fgetc(stream);
				}
				else {
					s = getfree(_Strage);
					e = s;
					s->head.s = UNDEF;
					s->count = 1;
					c = fgetc(stream);
				}
			}
			else if (c ==  rightParenChar) {
				if (e == NIL) {
					fprintf(stderr,"List read error: %c appeared before %c\n",rightParenChar,leftParenChar);
					if (CoreDumpOnError)
						abort();
					exit(1);
				}
				if (e == s && e->head.s == UNDEF) {
					return NIL;
				}
				e->next = NIL;
				return valueOf(s);
			}
			else {
				str[0] = c;
				i = 1;
				while ((c = fgetc(stream)) != ' ' &&
				       c != '\t' &&
				       c != '\n' &&
				       c != rightParenChar &&
				       c != leftParenChar ) {
					if (c == EOF) {
						c = rightParenChar;
						break;
					}
					else
						str[i++] = c;
				}
				str[i] = '\0';
				if (e == NIL) {
					fprintf(stderr,"List read error: symbol %s appeared\n",str);
					if (CoreDumpOnError)
						abort();
					exit(1);
				}
				if (e->head.s != UNDEF) {
					l = getfree(_Strage);
					e->next = l;
					e = l;
					e->count = 1;
				}
				e->head.s = symbol(str);
			}
		}
		else
			c = fgetc(stream);
	}
	return cons(UNDEF,NIL);
}

count(Cell)
register union cell	Cell;
{
	register int c;
	if (Cell.ptr != NIL && !ATOMCELL(Cell)) {
		if ((c = Cell.ptr->count) < INIT_CELL) {
			Cell.ptr->count = ++c;
			if (c == INIT_CELL)
				Cell.ptr->count = FIXED_CELL;
		}
	}
}
		
SYMLIST cons(s,l)
union cell	s,l;
{	register SYMLIST	a;

	a = getfree(_Strage);
	a->head = s;
	a->next = l.ptr;
	a->count = 0;
	count(l);
	count(s);
	return a;
}


_pr_elem(s,stream)
SYMLIST	s;
FILE	*stream;
{
	if (atom(s->head))
		printSym(s->head.s,stream);
	else
		printList(s->head.ptr,stream);
}

printList(l,stream)
SYMLIST	l;
FILE	*stream;
{	SYMLIST	s;

	count(l);
	fputc(leftParenChar,stream);
	if (l == NIL) {
		fputc(rightParenChar,stream);
		return;
	}
	if (atom(l)) {
		fprintf(stderr,"printList: argument is an atom:%s\n",string(l));
		return;
	}
	for (s = l; s->next != NIL; s = s->next) {
		_pr_elem(s,stream);
		fputc(' ',stream);
	}
	_pr_elem(s,stream);
	fputc(rightParenChar,stream);
	collect(l);
}

SYMLIST setSL(pointer,list)
SYMLIST	*pointer;
SYMLIST	list;
{
	count(list);
	collect(*pointer);
	*pointer = list;
	return list;
}
