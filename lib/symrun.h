#ifndef SYMCC
/*
	Symbolic-C run-time definition
*/

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
	unsigned	value;
	unsigned	func;
} *Htable,*Hindex[HSHSIZ],**phash;

#define	alpha(c)	(('A'<=c&&c<='Z')||('a'<=c&&c<='z')||c=='_')
#define	alphanum(c)	(('0'<=c&&c<='9')||alpha(c))

extern
char	*symfile,
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

struct init_list {
	unsigned short	count;
	unsigned	head;
	struct init_list	*next;
};

typedef struct sym_list	*SYMLIST;
typedef struct sym_list	STRAGE;
/*typedef unsigned	SYMBOL;*/
typedef long		SYMBOL;
typedef	union cell	CELL;

extern SYMBOL	symbol(),readSym();
extern char	*string();
extern SYMLIST	getfree(),readList(),_Strage,cons(),valueOf(),lcons();
extern SYMLIST		car(),cdr(),setSL();
extern int		atom(),eq();

extern char	leftParenChar,rightParenChar,NilIsCadble,CoreDumpOnError;

#define	setq(x,l)	setSL(&(x),l)
#define	main		MAIN
#define	printL(x)	printList(x,stdout)
#define	printS(x)	printSym(x,stdout)
#define	printC(x)	printCell(x,stdout)
#define	printCn(x)	printCelln(x,stdout)
#define	readL()		readList(stdin)
#define	readS()		readSym(stdin)
#define	readC()		readCell(stdin)

/*
#define	val(x)	Htable[(unsigned)(x)&0xffff].value
#define	fun(x)	Htable[(unsigned)(x)&0xffff].func
*/
#define	SymValue(x)	Htable[(unsigned)(x)&0xffff].value
#define	SymFunc(x)	Htable[(unsigned)(x)&0xffff].func

#define SYMCC
#endif
