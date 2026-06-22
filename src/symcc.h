#include	<stdio.h>

#define	WORDLEN	32
#define	FNLEN	80
#define	SYMNUM	2048
#define	HSHSIZ	512
#define	BUFLEN	512

extern
struct htable {
	int		num;
	char		*word;
	struct htable	*next;
} *Htable,*Hindex[HSHSIZ],**phash;

#define	alpha(c)	(('A'<=c&&c<='Z')||('a'<=c&&c<='z')||c=='_')
#define	alphanum(c)	(('0'<=c&&c<='9')||alpha(c))

extern
char	word[BUFLEN],
	buf[BUFLEN],buf2[BUFLEN],pwd[FNLEN],
	outfile[FNLEN],newarg[15][FNLEN],
	symfile[FNLEN],
	*c_strage,*st_btm,*st_limit;

extern
int		tnum;

extern
unsigned	st_size, sym_num;

#define	SYM_TAG		0xffff
#define	FIXED_CELL	0xffff
#define	GC_CELL		0xfffe
#define	INIT_CELL	0xfffd

#define	NIL		0
#define	UNDEF		(unsigned)0xffffffff

union cell {
	unsigned	s;
	struct sym_list	*ptr;
};

struct sym_list {
	unsigned short	count;
	union cell	head;
	struct sym_list	*next;
};

typedef struct sym_list	*SYMLIST;
typedef struct sym_list	STRAGE;
typedef unsigned	SYMBOL;

extern SYMBOL	symbol();
extern char	*string();

extern SYMLIST	getfree(),readList(),_Strage,cons(),valueOf(),lcons();

extern SYMLIST	car();
extern SYMLIST	cdr();

#define	setq(x,l)	setSL(&(x),l)
