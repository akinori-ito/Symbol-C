#include <stdio.h>

SYMLIST multilist(SYMLIST x, int n)
{
	SYMLIST y = NIL;
	int i;
	count(x);
	if (n <= 0)
		return NIL;
	if (n == 1)
		return valueOf(x);
	return valueOf(append(x,multilist(x,n-1)));	
}


main()
{
	SYMLIST x = NIL,y = NIL;
	int n;
	printf("input number: ");
	fflush(stdout);
	scanf("%d",&n);
	printf("input list (enclose the list with [ ]): ");
	fflush(stdout);
	x = readL();
	setq(y,multilist(x,n));
	printCn(y);
}
