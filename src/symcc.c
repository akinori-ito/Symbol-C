#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

#define	WORDLEN	32
#define	FNLEN	80
#define	SYMNUM	2048
#define	HSHSIZ	512
#define	BUFLEN	512
#define	ARGNUM	50
#define	NIL	0

#define	HEADER	"/home/usrs/aito/develop/symcc2/symrun.h"
#define LIBRARY	"/home/usrs/aito/develop/symcc2/symrun.a "

struct htable {
	int		num;
	char		*word;
	struct htable	*next;
} *Htable,*Hindex[HSHSIZ],**phash;

#define	alphanum(c)	(('A'<=c&&c<='Z')||('a'<=c&&c<='z')||('0'<=c&&c<='9')||c=='_')

char	*version = "(C) Copyright 1986 by A.ITO  version 1.0",
	*origin = "R.C.A.I.S., Tohoku University";

char	word[BUFLEN],
	buf[BUFLEN],buf2[BUFLEN],pwd[FNLEN],
	outfile[FNLEN],newarg[ARGNUM][FNLEN],
	symfile[FNLEN] = "",
	*c_strage,*st_btm,*st_limit;

int		tnum = 0;
unsigned	st_size = SYMNUM*WORDLEN,
		sym_num = SYMNUM;
extern char	leftParenChar,rightParenChar;

FILE *file_open(fn,m)
char	*fn,*m;
{	FILE	*f;
	f = fopen(fn,m);
	if (f == NULL) {
		fprintf(stderr,"Cannot open %s\n",fn);
		exit(1);
	}
	return f;
}

int chname(s,o)
char	*s,*o;
{
	short	i;

	i = strlen(s);
	if (strcmp(&s[i-3],".sc") == 0) {
		strcpy(o,s);
		o[i-2] = 'c';
		o[i-1] = '\0';
		return 0;
	}
	return -1;
}

s_sympath(tmpf)
char	*tmpf;
{	char	tmp_c[FNLEN];
	FILE	*tf;

	sprintf(tmpf,"%s/SCtemp%d",pwd,getpid());
	strcpy(tmp_c,tmpf);
	strcat(tmp_c,".c");
	tf = file_open(tmp_c,"w");
	fprintf(tf,"char *symfile=%c%s%c;\n",'"',symfile,'"');
	fclose(tf);
	sprintf(buf2,"cc %s -c",tmp_c);
	system(buf2);
	strcat(tmpf,".o");
	unlink(tmp_c);
}

main(argc,argv)
int	argc;
char	*argv[];
{	FILE	*f,*fo;
	short	i,j = 0,rd_st = 0, rd_sm = 0;
	char	t_only = 0,c_only = 0,show = 0,
		tmpf[FNLEN];

	defstrage(256);
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i],"-s") == 0) {
				strcpy(symfile,argv[i+1]);
				i++;
			}
			else if (strcmp(argv[i],"-r") == 0) 
				t_only = 1;
			else if (argv[i][1] == 'm') {
				st_size = atoi(&argv[i][2]);
				rd_st = 1;
			}
			else if (argv[i][1] == 'z') {
				sym_num = atoi(&argv[i][2]);
				rd_sm = 1;
			}
			else if (argv[i][1] == 'w')
				show = 1;
			else if (strcmp(argv[i],"-c") == 0) {
				strcpy(newarg[j++],argv[i]);
				c_only = 1;
			}
			else if (strcmp(argv[i],"-P") == 0) {
				i++;
				leftParenChar = argv[i][0];
				rightParenChar = argv[i][1];
			}
			else 
				strcpy(newarg[j++],argv[i]);
			continue;
		}

		strcpy(newarg[j++],argv[i]);
	}
	if (!rd_st)
		st_size = sym_num*WORDLEN;
#ifdef SYSV
	getcwd(pwd,FNLEN);
#else
	getwd(pwd);
#endif
	if (symfile[0] == '\0') {
		strcpy(symfile,pwd);
		strcat(symfile,"/symtable");
	}

	in_table(rd_st,rd_sm);

	for (i = 0; i < j; i++) {
		if (chname(newarg[i],outfile) == -1) {
			continue;
		}
		f = file_open(newarg[i],"r");
		fo = fopen(outfile,"w");
		if (fo == NULL) {
			fprintf(stderr,"sc: Cancot write to %s\n",outfile);
			exit(1);
		}
		fprintf(fo,"#include	<%s>\n",HEADER);
		symtran(f,fo);
		fclose(f);
		fclose(fo);
		strcpy(newarg[i],outfile);
	}
	out_table();
	if (t_only == 0) {
		strcpy(buf,"cc ");
		for (i = 0; i < j; i++) {
			strcat(buf,newarg[i]);
			strcat(buf," ");
		}
		if (c_only == 0) {
			s_sympath(tmpf);
			strcat(buf,LIBRARY);
			strcat(buf,tmpf);
		}
		if (show)
			puts(buf);
		system(buf);
		if (c_only == 0)
			unlink(tmpf);
	}
}
