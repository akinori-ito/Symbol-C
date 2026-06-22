#include	"symcc.h"
		
char *string(i)
short	i;
{
	if (i >= tnum)
		return NULL;
	return Htable[i & 0xffff].word;
}

unsigned readSym(stream)
FILE	*stream;
{	char	str[WORDLEN];

	if (fscanf(stream,"%s",str) == EOF)
		return UNDEF;
	return symbol(str);
}

printSym(sym,stream)
unsigned	sym;
FILE	*stream;
{
	if (sym == UNDEF)
		fputs("*UNDEF*",stream);
	else
		fputs(string(sym),stream);
}
