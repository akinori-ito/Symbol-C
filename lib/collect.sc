#include	<stdio.h>

extern SYMLIST	_Strage,_EosPtr;
extern int	_StrgSize,AllocatedCell;

#define ATOMCELL(x)	(((unsigned)((x).ptr) & 0xffff0000) == 0xffff0000)

collect(cell)
register union cell	cell;
{	register unsigned short	c;
	if (cell.ptr != NIL && !ATOMCELL(cell)) {
		c = cell.ptr->count;
		if (c != FIXED_CELL && c != GC_CELL && c!= INIT_CELL) {
			if (c <= 1) {
				cell.ptr->count = GC_CELL;
				collect(cell.ptr->next);
				collect(cell.ptr->head.ptr);
				cell.ptr->head.s = UNDEF;
				_EosPtr->next = cell.ptr;
				cell.ptr->next = NIL;
				_EosPtr = cell.ptr;
				AllocatedCell--;
			}
			else {
				c--;
				cell.ptr->count = c;
			}
		}
	}
}

fix(cell)
CELL	cell;
{
	_mark_all(cell,FIXED_CELL);
}

debugList(cell)
CELL	cell;
{
	_mark_all(cell,INIT_CELL);
}

_mark_all(cell,mark)
CELL	cell;
int	mark;
{	SYMLIST	l;

	if (atom(cell))	return;
	for (l = cell.ptr; l != NIL; l = cdr(l)) {
		l->count = mark;
		if (!atom(l->head))
			_mark_all(l->head.ptr,mark);
	}
}

GCAll()
{	SYMLIST	l = &_Strage[0];
	long	i;
	long	fixed = 0, clct = 0, nclct = 0, unused = 0;
	
	for (i = 1; i < _StrgSize; i++) {
		switch (_Strage[i].count) {
		case FIXED_CELL:
			fixed++;
			break;
		case GC_CELL:
			clct++;
			unused--;
		case INIT_CELL:
			unused++;
			nclct--;
		default:
			l->next = &_Strage[i];
			l = l->next;
			l->count = INIT_CELL;
			nclct++;
		}
	}
	l->next = NIL;
	_EosPtr = l;
	fprintf(stderr,
		"%d fixed, %d already collected, %d newly collected, %d unused\n",
		fixed,clct,nclct,unused);
}

SYMLIST valueOf(x)
CELL	x;
{
	if (x.ptr != NIL && !atom(x))
		if (x.ptr->count != FIXED_CELL)
			x.ptr->count--;
	return x.ptr;
}

GCreport(stream)
FILE	*stream;
{	long	i;
	long	fixed = 0, clct = 0, nclct = 0, unused = 0;
	
	for (i = 1; i < _StrgSize; i++) {
		switch (_Strage[i].count) {
		case FIXED_CELL:
			fixed++;
			break;
		case GC_CELL:
			clct++;
			unused--;
		case INIT_CELL:
			unused++;
			nclct--;
		default:
			nclct++;
		}
	}
	fprintf(stream,
		"%d fixed, %d already collected, %d in use, %d unused\n",
		fixed,clct,nclct,unused);
}
