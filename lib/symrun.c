/*
	symbolic-C run-time routine package version 2.0

			by A.ITO
*/

#include	<stdio.h>
#include	<stdlib.h>

#define	WORDLEN		64
#define	STRAGE_SIZE	64
#define	FNLEN		64
#define	SYMNUM		2048
#define	HSHSIZ		512
#define	BUFLEN		512
#define	NIL		0
#define	UNDEF		-1

struct htable {
	int		num;
	char		*word;
	struct htable	*next;
	unsigned	value;			/* pointer to some value. */
	unsigned	func;
} *Htable,*Hindex[HSHSIZ],**phash;

char		*c_strage,*st_btm,*st_limit;
char		alloced = 0;
extern char	*symfile,
		leftParenChar,rightParenChar;

int		tnum = 0;
unsigned	st_size = SYMNUM*WORDLEN,
		sym_num = SYMNUM;

int hashnum(s)
char	*s;
{	unsigned int	n = 0;
	char		*c = s;
	for (; *c != '\0'; c++) {
		n *= n;
		n ^= *c;
	}
	return n%HSHSIZ;
}

short symnum(s)
char	*s;
{	short		n,i,symput();
	struct htable	*h;

	n = hashnum(s);
	h = Hindex[n];
	phash = &Hindex[n];
	if (h == NIL)
		return -1;
	else {
		for (; h != NIL; h = h->next) {
			if (strcmp(h->word,s) == 0)
				return h->num;
			phash = &(h->next);
		}
		return -1;
	}
}

unsigned symbol(s)
char	*s;
{	short		n,i,symput();
	struct htable	*h;
	unsigned	a;
	n = hashnum(s);
	h = Hindex[n];
	phash = &Hindex[n];
	if (h == NIL)
		a = ((unsigned)symput(s) | 0xffff0000);
	else {
		for (; h != NIL; h = h->next) {
			if (strcmp(h->word,s) == 0) {
				a = ((unsigned)(h->num) | 0xffff0000);
				goto ret;
			}
			phash = &(h->next);
		}
		a = ((unsigned)symput(s) | 0xffff0000);
	}
ret:
	return a;
}

short symput(s)
char	*s;
{	struct htable	*h = &Htable[tnum];
	short	i = tnum;
	char	*c;

	if (tnum == sym_num) {
		fprintf(stderr,"Symbol table overflow\n");
		exit(1);
	}
	h->num = tnum++;
	h->next = NIL;
	h->word = st_btm;
	for (c = s; *c != '\0'; *(st_btm++) = *(c++)) {
		if (st_btm >= st_limit) {
			fprintf(stderr,"Out of string space\n");
			exit(1);
		}
	}
	*(st_btm++) = '\0';

	*phash = h;
	return i;
}
	
clr_table()
{	short	i;

	for (i = 0; i < HSHSIZ; i++)
		Hindex[i] = NIL;
	tnum = 0;
	st_btm = c_strage;
}
		
char *string(i)
short	i;
{
	if (i >= tnum)
		return NULL;
	else if (i == UNDEF)
		return " *UNDEF*";
	return Htable[i & 0xffff].word;
}

str_init()
{	/* char	*malloc(),*calloc(); */
	if (alloced) {
		free(c_strage);
		free((char*)Htable);
	}
	if ((c_strage = malloc(st_size)) == NULL) {
		fprintf(stderr,"Too large string area\n");
		exit(1);
	}
	if ((Htable = (struct htable*)calloc(sym_num,sizeof(struct htable)))
	    == NULL) {
		fprintf(stderr,"Too large table area\n");
		exit(1);
	}
	st_btm = c_strage;
	st_limit = c_strage+st_size;
	alloced = 1;
}

main(argc,argv)
int	argc;
char	*argv[];
{	char	*paren,*getenv();
	in_table();
	defstrage(STRAGE_SIZE);
	if ((paren = getenv("PARENTHESIS")) != NULL) {
		leftParenChar = paren[0];
		rightParenChar = paren[1];
	}
	MAIN(argc,argv);
}

out_table()
{	FILE		*f;
	int		i,nxtnum;
	struct htable	*h;

	f = fopen(symfile,"w");
	fprintf(f,"%d %d\n",sym_num,st_size);
	fprintf(f,"%d\n",tnum);
	for (i = 0; i < tnum; i++) {
		h = &Htable[i];
		if (h->next == NIL)
			nxtnum = -1;
		else
			nxtnum = h->next->num;
		fprintf(f,"%d %s %d\n",h->num,h->word,nxtnum);
	}
	for (i = 0; i < HSHSIZ; i++) {
		h = Hindex[i];
		if (h == NIL)
			nxtnum = -1;
		else
			nxtnum = h->num;
		fprintf(f,"%d\n",nxtnum);
	}
	fclose(f);
}

in_table()
{	FILE		*f;
	int		i,nxtnum;
	struct htable	*h;
	char		*c,word[WORDLEN*2];

	f = fopen(symfile,"r");
	if (f == NULL) {
		str_init();
		clr_table();
		return;
	}
	fscanf(f,"%d %d",&sym_num,&st_size);
	str_init();
	fscanf(f,"%d",&tnum);
	for (i = 0; i < tnum; i++) {
		h = &Htable[i];
		fscanf(f,"%d %s %d",&(h->num),word,&nxtnum);
		h->word = st_btm;

		for (c = word; *c != '\0'; *(st_btm++) = *(c++)) {
			if (st_btm >= st_limit) {
				fprintf(stderr,"Out of string space\n");
				exit(1);
			}
		}
		*(st_btm++) = '\0';
		if (nxtnum == -1)
			h->next = NIL;
		else
			h->next = &Htable[nxtnum];
		h->value = NIL;
	}
	for (i = 0; i < HSHSIZ; i++) {
		fscanf(f,"%d",&nxtnum);
		if (nxtnum == -1)
			Hindex[i] = NIL;
		else
			Hindex[i] = &Htable[nxtnum];
	}
	fclose(f);
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
	else if (sym == NIL)
		fprintf(stream,"%c%c",leftParenChar,rightParenChar);
	else
		fputs(string(sym),stream);
}
