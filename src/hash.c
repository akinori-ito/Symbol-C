#include	<stdlib.h>
#include	"symcc.h"

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

unsigned symsrch(s)
char	*s;
{	int		n;
	struct htable	*h;

	n = hashnum(s);
	h = Hindex[n];
	phash = &Hindex[n];
	if (h == NIL)
		return UNDEF;
	else {
		for (; h != NIL; h = h->next) {
			if (strcmp(h->word,s) == 0)
				return h->num | 0xffff0000;
			phash = &(h->next);
		}
		return UNDEF;
	}
}

unsigned symput(s)
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
	return i | 0xffff0000;
}

unsigned symbol(s)
char	*s;
{	unsigned	i;
	if ((i = symsrch(s)) == UNDEF)
		i = symput(s);
	return i;
}
	
str_init()
{	/* char	*malloc(),*calloc();*/

	if ((c_strage = malloc(st_size)) == NULL) {
		fprintf(stderr,"Too large strage area\n");
		exit(1);
	}
	if ((Htable = (struct htable*)calloc(sym_num,sizeof(struct htable)))
	    == NULL) {
		fprintf(stderr,"Too large table area\n");
		exit(1);
	}
	st_btm = c_strage;
	st_limit = c_strage+st_size;
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

in_table(rst,rsm)
short	rst,rsm;
{	FILE		*f;
	int		i,nxtnum,st,sm;
	struct htable	*h;
	char		*c;

	f = fopen(symfile,"r");
	if (f == NULL) {
		str_init();
		return;
	}
	fscanf(f,"%d %d",&sm,&st);
	if (!rst)
		st_size = st;
	if (!rsm)
		sym_num = sm;
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

