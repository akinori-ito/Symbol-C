//
//	symbolic-C list manupilation extension - basic package ver. 2.0
//
//	                Feb. 18, 1988 by A.ITO
//
//#define GETDEBUG

#include	<stdio.h>
#include	<stdlib.h>

#define		ALLOCSIZ	4096	/* was 64 */

int AllocatedCell = 0;
SYMLIST	_Strage = NULL,_EosPtr;
int	_StrgSize;
static
void	_CannotAlloc();
static
void (*AllocError)() = _CannotAlloc;

#ifdef GETDEBUG
int	_GetCount = 0;
#endif

setAllocError(f)
void (*f)();
{
	AllocError = f;
}

defstrage(n)
unsigned	n;
{	char		/* *malloc(),*realloc(),*/ *s;
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

static
void _CannotAlloc()
{
	fprintf(stderr,"Too large strage area\n");
	exit(1);
}

SYMLIST	getfree(strage)
SYMLIST	strage;
{	register SYMLIST	f,g;
	f = strage[0].next;
	if (f == _EosPtr) {
		short	i;
		g = (SYMLIST)malloc(ALLOCSIZ*sizeof(struct sym_list));
#ifdef GETDEBUG
		_GetCount++;
		fprintf(stderr,"Getting new area %d\r",_GetCount);
		fflush(stderr);
		if (g == NULL) {
			fprintf(stderr,"Failed to get new cell area %d!\n",
				_GetCount);
			fprintf(stderr,"Block size=%d (%d bytes)\n",
				ALLOCSIZ,ALLOCSIZ*sizeof(struct sym_list));
			fprintf(stderr,"I'll try again...");
			g = (SYMLIST)malloc(ALLOCSIZ*sizeof(struct sym_list));
			if (g == NULL) {
				fprintf(stderr,"Too large strage area\n");
				exit(1);
			}
			else
				fprintf(stderr,"OK\n");
		}
#else
		if (g == NULL) {
			(*AllocError)();
		}
#endif /* GETDEBUG */
		
		for(i = 0; i < ALLOCSIZ-1; i++) {
			g[i].count = INIT_CELL;
			g[i].head.s = UNDEF;
			g[i].next = &g[i+1];
		}
		g[ALLOCSIZ-1].count = INIT_CELL;
		g[ALLOCSIZ-1].head.s = UNDEF;
		g[ALLOCSIZ-1].next = NIL;
		_EosPtr = &g[ALLOCSIZ-1];
		f->next = g;
	}
	strage[0].next = f->next;

	f->count = 0;
	f->next = NIL;
	f->head.s = UNDEF;
	AllocatedCell++;
	return f;
}

initList(strage,n)
SYMLIST	strage;
int	n;
{	int	i;

	for (i = 0; i < n-1; i++) {
		strage[i].count = INIT_CELL;
		strage[i].head.s = UNDEF;
		strage[i].next = &strage[i+1];
	}
	strage[n-1].next = NIL;
	_EosPtr = &strage[n-1];
}
