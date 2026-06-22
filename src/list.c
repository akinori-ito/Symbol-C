/*
	symbolic-C list manupilation extension - basic package ver. 2.0

	                Feb. 18, 1988 by A.ITO
*/

#include	"symcc.h"
#include	<stdlib.h>

#define		ALLOCSIZ	64

SYMLIST	_Strage = NULL,_EosPtr;
int	_StrgSize;

defstrage(n)
unsigned	n;
{	/*char		*malloc(),*realloc();*/
	unsigned	size = n*sizeof(STRAGE);

	if (_Strage == NULL) 
		_Strage = (SYMLIST)malloc(size);
	else 
		_Strage = (SYMLIST)realloc(_Strage,size);
	if (_Strage == NULL) {
		fprintf(stderr,"Too large strage area\n");
		exit(1);
	}
	initList(_Strage,n);
	_StrgSize = n;
}

SYMLIST	getfree(strage)
SYMLIST	strage;
{	SYMLIST	f, f1;
	f = strage[0].next;
	if (f == NIL) {
		short	i;
		
		f = (SYMLIST)calloc(ALLOCSIZ, sizeof(struct sym_list));
		strage[0].next = f;
		for(f1 = f,i = 1; i < ALLOCSIZ-1; i++) {
			f1->next = f1+1;
			f1 = f1->next;
		}
		f = f1;
	}
	else
		strage[0].next = f->next;

	f->count = 0;
	f->next = NIL;
	f->head.s = UNDEF;
	return f;
}

initList(strage,n)
SYMLIST	strage;
int	n;
{	int	i;

	strage[0].head.s = UNDEF;
	for (i = 0; i < n-1; i++) {
		strage[i].count = INIT_CELL;
		strage[i].head.s = UNDEF;
		strage[i].next = &strage[i+1];
	}
	strage[n-1].next = NIL;
	_EosPtr = &strage[n-1];
}
