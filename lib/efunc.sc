//
//	Symbolic-C extended function definition
//
SYMLIST	append(x,y)
SYMLIST	x,y;
{	SYMLIST	a = NIL;
	count(x);
	count(y);

	if (x == NIL) {
		collect(x);
		return valueOf(y);
	}
	setq(a,cons(car(x),append(cdr(x),y)));
	collect(x);
	collect(y);
	return valueOf(a);
}
