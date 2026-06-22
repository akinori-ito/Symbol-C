# Symbol-C
[Historical] A C-preprocessor to enable Lisp-like description

This program was written in 1988 to realize a Lisp-like description in K&R C program. Here is an example:
```c
#include	<stdio.h>
SYMLIST	aho = @[a b c d];

main()
{
	printCn(aho);
}
```
The type `SYMLIST` can be used to hold a Lisp-style linked list. Lisp-style functions are available such as `car`,`cdr`,`cons`,`setq`,`append`.

When we execute a code
```c
SYMLIST x;
setq(x,@[aaa bbb ccc]);
```
the identifiers `aaa`,`bbb`, and `ccc` are symbols. These symbols are internally treated as ID numbers, and the hash table `symtable` is created in the same directory as other programs.

Here is another example:
```c
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

```
The function `multilist` is a program to repeat the list `x` `n` times. Since C has no constructor or descructor, we need to do the object creation and collection manually; `count` is a function to increase the reference count of the object denoted by the argument variable, and `collect` counts down the reference count and free the memory if needed. The function `valueOf` is a function just count down the reference count but do not collect the object (because the reference count will be increased when the returned value is assigned to another variable).
