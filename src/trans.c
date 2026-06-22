#include	"symcc.h"

char	eof = 0;
int	ListNum = 0;
extern 
char	leftParenChar,rightParenChar;

symtran(f,ff)
FILE	*f,*ff;
{	char	c;
	FILE	*fo,*file_open();
	static char	tmpfn[FNLEN];

	sprintf(tmpfn,"/tmp/SCtemp1_%d",getpid());
	fo = file_open(tmpfn,"w");
	while (!eof) {
		switch (c = fgetc(f)) {
		case 0x27:
			fputc(c,fo);
			while((c = fgetc(f)) != 0x27 && !feof(f))
				fputc(c,fo);
			if (feof(f))
				eof = 1;
			else
				fputc(c,fo);
			break;
		case '/':
			comment(f,fo,c);
			break;
		case '"':
			fputc(c,fo);
			do {
				c = fgetc(f);
				fputc(c,fo);
			} while (c != '"' && !feof(f));
			if (feof(f))
				eof = 1;
			break;
		case '@':
			trSymbol(f,fo,ff);
			break;
		default:
			if (feof(f))
				eof = 1;
			else
				fputc(c,fo);
		}
	}
	fclose(fo);
	fo = file_open(tmpfn,"r");
	while ((c = fgetc(fo)) ,!feof(fo))
		fputc(c,ff);
	fclose(fo);
	unlink(tmpfn);
}

comment(f,fo,c0)
FILE	*f,*fo;
char	c0;
{	char	c;

	fputc(c0,fo);
	switch (c = fgetc(f)) {
	case '*':
		fputc(c,fo);

		do {
			while((c = fgetc(f)) != '*' && !feof(f))
				fputc(c,fo);
			if (feof(f))
				eof = 1;
			else
				fputc(c,fo);
		} while (c = fgetc(f),
			   c == '/'?
				(fputc(c,fo),0):
				(ungetc(c,f),1));

		break;
	case EOF:
		eof = 1;
		break;
	case '/':
		fputc('*',fo);
		while ((c = fgetc(f)) != '\n' && !feof(f))
			fputc(c,fo);
		if (feof(f))
			eof = 1;
		fputs("*/\n",fo);
		break;
	default:
		ungetc(c,f);
	}
}

trSymbol(f,fo,ff)
FILE	*f,*fo,*ff;
{	short		i;
	unsigned	j;
	char		c,q;
	SYMLIST		l = NIL;

	i = 0;
	c = fgetc(f);
	if (c == leftParenChar) {
		ungetc(c,f);
		setq(l,readList(f));
		if (l == NIL) {
			fprintf(fo,"NIL");
		}
		else {
			fprintf(fo,"(SYMLIST)&_Lstcn%d[%d]",ListNum,
				putLStructure(ff,l,ListNum));
			ListNum++;
		}
		goto end;
	}
	switch (c) {
	case '"':
	case '|':
		q = c;
		while (c = fgetc(f),c != q && !feof(f)) {
			word[i++] = c;
			if (i == 255) {
				while(c = fgetc(f),alphanum(c));
				break;
			}
		}
		word[i] = '\0';
		if (feof(f))	eof = 1;
		break;
	default:
		ungetc(c,f);
		while (c = fgetc(f),alphanum(c)) {
			word[i++] = c;
			if (i == 255) {
				while(c = fgetc(f),alphanum(c));
				break;
			}
		}
		word[i] = '\0';
		ungetc(c,f);
	}
	if ((j = symsrch(word)) == UNDEF) {
		j = symput(word);
	}
	fprintf(fo," (SYMBOL)%u ",j);
end:
	collect(l);
}

static int	StNumber;

putLStructure(f,l,n)
FILE	*f;
SYMLIST	l;
int	n;
{	int	ln;
	SYMLIST	p = NIL;
	
	count(l);
	StNumber = 0;
	fprintf(f,"static struct init_list _Lstcn%d[] = \n{",n);
	ln = putLS1(f,l,n);
	fprintf(f,"};\n");
	collect(l);
	return ln;
}

#define	S_LIST	0
#define	S_ATOM	1
#define	S_NIL	2

putLS1(f,l,n)
FILE	*f;
SYMLIST	l;
int	n;
{	short	carn,carspec,cdrn,cdrspec;

	if (cdr(l) == NIL)
		cdrspec = S_NIL;
	else {
		cdrn = putLS1(f,cdr(l),n);
		cdrspec = S_LIST;
	}
	if (car(l) == NIL)
		carspec = NIL;
	else if (atom(car(l)))
		carspec = S_ATOM;
	else {
		carn = putLS1(f,car(l),n);
		carspec = S_LIST;
	}
	fprintf(f,"{0xffff,");
	if (carspec == S_ATOM)
		fprintf(f,"0x%x,",car(l));
	else if (carspec == S_LIST)
		fprintf(f,"(unsigned)&_Lstcn%d[%d],",n,carn);
	else
		fprintf(f,"0,");
	if (cdrspec == S_LIST)
		fprintf(f,"&_Lstcn%d[%d]",n,cdrn);
	else
		fprintf(f,"(struct init_list*)0");
	fprintf(f,"},\n");
	return StNumber++;
}
