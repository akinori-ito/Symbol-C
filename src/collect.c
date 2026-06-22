#include	"symcc.h"

extern SYMLIST	_Strage,_EosPtr;
extern int	_StrgSize;

collect(cell)
union cell	cell;
{	unsigned char	c;
	if (cell.ptr != NIL && !atom(cell)) {
		c = cell.ptr->count;
		if (c != FIXED_CELL && c != GC_CELL && c!= INIT_CELL) {
			if (c <= 1) {
				cell.ptr->count = GC_CELL;
				collect(cell.ptr->next);
				if (!atom(cell.ptr->head))
					collect(cell.ptr->head.ptr);
				cell.ptr->head.s = UNDEF;
				_EosPtr->next = cell.ptr;
				cell.ptr->next = NIL;
				_EosPtr = cell.ptr;
			}
			else {
				c--;
				cell.ptr->count = c;
			}
		}
	}
}

fix(cell)
SYMLIST	cell;
{	SYMLIST	l;

	for (l = cell; l != NIL; l = cdr(l)) {
		l->count = FIXED_CELL;
		if (!atom(l->head))
			fix(l->head.ptr);
	}
}
