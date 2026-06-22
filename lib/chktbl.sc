#define	WORDLEN	32
#define	FNLEN	64
#define	SYMNUM	512
#define	HSHSIZ	512
#define	BUFLEN	512
#define	NIL	0

extern struct htable	*Htable,*Hindex[HSHSIZ],**phash;

#define	alphanum(c)	(('A'<=c&&c<='Z')||('a'<=c&&c<='z')||('0'<=c&&c<='9')||c=='_')

extern char	word[],
	buf[BUFLEN],
	outfile[FNLEN],newarg[15][FNLEN],
	symfile[FNLEN];

extern int	tnum;

main()
{	short	i;
	struct htable	*h;

	for (i = 0; i < HSHSIZ; i++) {
		for (h = Hindex[i]; h != NIL; h = h->next) {
			printf("%s ",h->word);
		}
		putchar('\n');
	}
}
