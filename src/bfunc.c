#include	<stdlib.h>
#include	"symcc.h"

extern SYMLIST	_Strage,_EosPtr;
extern int	_StrgSize;

#define	LPAREN_CHAR	'['
#define	RPAREN_CHAR	']'

char	leftParenChar = LPAREN_CHAR,
	rightParenChar = RPAREN_CHAR;

atom(x)
unsigned	x;
{
	if ((x & 0xffff0000) == 0xffff0000)
		return 1;
	return 0;
}

SYMLIST car(x)
union cell	x;
{
	if (atom(x)) {	
		fprintf(stderr,"CAR of symbol ");
		printSym(x,stderr);
		exit(1);
	}
	return x.ptr->head.ptr;
}

SYMLIST cdr(x)
union cell	x;
{
	if (atom(x)) {	
		fprintf(stderr,"CAR of symbol ");
		printSym(x,stderr);
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
	while (!feof(stream)) {
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
					s->count = 0;
					c = fgetc(stream);
				}
			}
			else if (c ==  rightParenChar) {
				if (e == NIL) {
					fprintf(stderr,"List read error: ] appeared before [\n");
					exit(1);
				}
				if (e == s && e->head.s == UNDEF) {
					collect(e);
					return NIL;
				}
				e->next = NIL;
				return s;
			}
			else {
				str[0] = c;
				i = 1;
				while ((c = fgetc(stream)) != ' ' &&
				       c != '\t' &&
				       c != '\n' &&
				       c != rightParenChar) {
					if (feof(stream)) {
						c = rightParenChar;
						break;
					}
					else
						str[i++] = c;
				}
				str[i] = '\0';
				if (e == NIL) {
					fprintf(stderr,"List read error: symbol %s appeared\n",str);
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
union cell	Cell;
{
	if (Cell.ptr != NIL && !atom(Cell)) {
		if (Cell.ptr->count < INIT_CELL) {
			(Cell.ptr->count)++;
			if (Cell.ptr->count == INIT_CELL)
				Cell.ptr->count = FIXED_CELL;
		}
	}
}
		
SYMLIST cons(s,l)
union cell	s,l;
{	SYMLIST	a;

	a = getfree(_Strage);
	a->head = s;
	a->next = l.ptr;
	a->count = 0;
	count(l);
	count(s);
	return a;
}

setSL(pointer,list)
SYMLIST	*pointer;
SYMLIST	list;
{
	count(list);
	collect(*pointer);
	*pointer = list;
}
