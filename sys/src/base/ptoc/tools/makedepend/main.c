/* @(#)tools/makedepend/main.c
 */
#ifdef hpux
#define sigvec sigvector
#endif /* hpux */
#include	<sys/signal.h>

#ifdef SYSV
#include <unistd.h>
#endif


#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


#include "def.h"	/* 1992-Jul-13 J.Bruehe  take system includes first */


void redirect(char *linep, char *makefile);


#if u3b || M32 || u3b15 || u3b5 || u3b2 || pyr || sun || ultrix
#define sigvec sigvector
#endif /* several */

#if __hp9000s700 || __ia64
#define sigvec sigvectorit
#endif /* hpux */

#ifdef DEBUG
int	debug;
#endif

char	*directives[] = {
	"if",
	"ifdef",
	"ifndef",
	"else",
	"endif",
	"define",
	"undef",
	"include",
	"line",
	NULL
};

struct symtab	predefs[] = {
#ifdef apollo
	{"apollo", NULL},
#endif
#ifdef ibm032
	{"ibm032", NULL},
#endif
#ifdef sun
	{"sun", NULL},
#endif
#ifdef hpux
	{"hpux", NULL},
#endif
#ifdef vax
	{"vax", NULL},
#endif
#ifdef VMS
	{"VMS", NULL},
#endif
	{NULL, NULL}
};

struct symtab	deflist[ MAXDEFINES ];
struct	inclist inclist[ MAXFILES ],
		*inclistp = inclist;

char	*filelist[ MAXFILES ];
char	*includedirs[ MAXDIRS ];
char	*notdotdot[ MAXDIRS ];
char	*objfile = ".o";
char	*startat = "# DO NOT DELETE THIS LINE -- make depend depends on it.";
int	width = 78;
boolean	printed = FALSE;
boolean	verbose = FALSE;
boolean	show_where_not = FALSE;

#if _IBMR2 || OSF1 || LINUX	/* 1992-Jul-13 J.Bruehe  type is machine specific */
void	catch();
#else
int	catch();
#endif

#if !( _IBMR2 || OSF1 || LINUX || HP9 || FREEBSD )	/* 1992-Jul-13 J.Bruehe  here, from <sys/signal.h> */
struct  sigvec {
        int     (*sv_handler)();        /* signal handler */
        int     sv_mask;                /* signal mask to apply */
        int     sv_flags;               /* see signal options below */
};
#endif	/* _IBMR2 || OSF1 */

struct sigvec sig_vec = {
	catch,
	 (1<<(SIGINT -1))
	|(1<<(SIGQUIT-1))
	|(1<<(SIGBUS-1))
	|(1<<(SIGILL-1))
	|(1<<(SIGSEGV-1))
	|(1<<(SIGHUP-1))
	|(1<<(SIGPIPE-1))
#ifdef LINUX
	,
#else
	|(1<<(SIGSYS-1)),
#endif
	0
};

int main(argc, argv)
	int	argc;
	char	**argv;
{
	register struct symtab	*symp = deflist;
	register char	**fp = filelist;
	register char	**incp = includedirs;
	register char	*p;
	register struct inclist	*ip;
	char	*makefile = NULL;
	struct filepointer	*filecontent;
	struct symtab *psymp = predefs;
	char *endmarker = NULL;

	while (psymp->s_name)
	    *symp++ = *psymp++;
	for(argc--, argv++; argc; argc--, argv++) {
	    	/* if looking for endmarker then check before parsing */
		if (endmarker && strcmp (endmarker, *argv) == 0) {
		    endmarker = NULL;
		    continue;
		}
		if (**argv != '-') {
			*fp++ = argv[0];
			continue;
		}
		switch(argv[0][1]) {
		case '-':
			endmarker = &argv[0][2];
			if (endmarker[0] == '\0') endmarker = "--";
			break;
		case 'D':
			symp->s_name = argv[0]+2;
			if (*symp->s_name == '\0') {
				symp->s_name = *(++argv);
				argc--;
			}
			for (p=symp->s_name; *p ; p++)
				if (*p == '=') {
					*p++ = '\0';
					break;
				}
			symp->s_value = p;
			symp++;
			break;
		case 'I':
			*incp++ = argv[0]+2;
			if (**(incp-1) == '\0') {
				*(incp-1) = *(++argv);
				argc--;
			}
			break;
		/* do not use if endmarker processing */
		case 'w':
			if (endmarker) break;
			if (argv[0][2] == '\0') {
				argv++;
				argc--;
				width = atoi(argv[0]);
			} else
				width = atoi(argv[0]+2);
			break;
		case 'o':
			if (endmarker) break;
			if (argv[0][2] == '\0') {
				argv++;
				argc--;
				objfile = argv[0];
			} else
				objfile = argv[0]+2;
			break;
		case 'v':
			if (endmarker) break;
			verbose = TRUE;
#ifdef DEBUG
			if (argv[0][2])
				debug = atoi(argv[0]+2);
#endif
			break;
		case 's':
			if (endmarker) break;
			startat = argv[0]+2;
			if (*startat == '\0') {
				startat = *(++argv);
				argc--;
			}
			if (*startat != '#')
				log_fatal("-s flag's value should start %s\n",
					"with '#'.");
			break;
		case 'f':
			if (endmarker) break;
			makefile = argv[0]+2;
			if (*makefile == '\0') {
				makefile = *(++argv);
				argc--;
			}
			break;
		
		/* Ignore -O, -g so we can just pass ${CFLAGS} to
		   makedepend
		 */
		case 'O':
		case 'g':
			break;
		default:
			if (endmarker) break;
	/*		log_fatal("unknown opt = %s\n", argv[0]); */
			log_warn("ignoring option %s\n", argv[0]);
		}
	}
	*incp++ = INCLUDEDIR;

	redirect(startat, makefile);

	/*
	 * catch signals.
	 */
/*	sigvec(SIGHUP, &sig_vec, (struct sigvec *)0);
	sigvec(SIGINT, &sig_vec, (struct sigvec *)0);
	sigvec(SIGQUIT, &sig_vec, (struct sigvec *)0);
	sigvec(SIGILL, &sig_vec, (struct sigvec *)0);
	sigvec(SIGBUS, &sig_vec, (struct sigvec *)0);
	sigvec(SIGSEGV, &sig_vec, (struct sigvec *)0);
	sigvec(SIGSYS, &sig_vec, (struct sigvec *)0);
*/
/*
signal(SIGHUP, catch());
signal(SIGINT, catch());
signal(SIGQUIT, catch());
signal(SIGILL, catch());
signal(SIGBUS, catch());
signal(SIGSEGV, catch());
signal(SIGSYS, catch());
*/
	/*
	 * now peruse through the list of files.
	 */
	for(fp=filelist; *fp; fp++) {
		filecontent = getfile(*fp);
		ip = newinclude(*fp, (char *)NULL);

		find_includes(filecontent, ip, ip, 0);
		freefile(filecontent);
		recursive_pr_include(ip, ip->i_file, basename(*fp));
		inc_clean();
	}
	if (printed)
		printf("\n");
	exit(0);
}

struct filepointer *getfile(file)
	char	*file;
{
	register int	fd;
	struct filepointer	*content;
	struct stat	st;

	content = (struct filepointer *)malloc(sizeof(struct filepointer));
	if ((fd = open(file, O_RDONLY)) < 0) {
		log_warn("cannot open \"%s\"\n", file);
		content->f_p = content->f_base = content->f_end = malloc(1);
		*content->f_p = '\0';
		return(content);
	}
	fstat(fd, &st);
	content->f_len = st.st_size+1;
	content->f_base = malloc(content->f_len);
	if (content->f_base == NULL)
		log_fatal("cannot allocate mem\n");
	if (read(fd, content->f_base, st.st_size) != st.st_size)
		log_fatal("cannot read all of %s\n", file);
	close(fd);
	content->f_p = content->f_base;
	content->f_end = content->f_base + st.st_size;
	*content->f_end = '\0';
	content->f_line = 0;
	return(content);
}

void freefile(struct filepointer *fp)
{
	free(fp->f_base);
	free(fp);
}

/*VARARGS*/
void _old_log_fatal(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9)
char *x0 ;
void *x1, *x2, *x3, *x4, *x5, *x6, *x7, *x8, *x9 ;
{
	log_warn(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9);
	exit (1);
}

/*VARARGS0*/
void _old_log_warn(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9)
char * x0 ;	/* J.Bruehe 91-Aug-01  ANSI-C has prototype for "fprintf" */
void *x1, *x2, *x3, *x4, *x5, *x6, *x7, *x8, *x9 ;
{
	fprintf(stderr, x0,x1,x2,x3,x4,x5,x6,x7,x8,x9);
}

void log_warn(const char *format,...)
{
	va_list args;
	va_start (args, format);
	vfprintf (stderr,format, args);
	va_end (args);
}

void log_fatal(const char *format,...)
{
	va_list args;
	va_start (args, format);
	vfprintf (stderr,format, args);
	va_end (args);
	exit(1);
}






char *copy(str)
	register char	*str;
{
	register char	*p = malloc(strlen(str) + 1);

	strcpy(p, str);
	return(p);
}

int match(char * str, char ** list)
	/*register char	*str, **list;*/
{
	register int	i;

	for (i=0; *list; i++, list++)
		if (strcmp(str, *list) == 0)
			return(i);
	return(-1);
}

/*
 * Get the next line.  We only return lines beginning with '#' since that
 * is all this program is ever interested in.
 */
char *getline(filep)
	register struct filepointer	*filep;
{
	register char	*p,	/* walking pointer */
			*eof,	/* end of file pointer */
			*bol;	/* beginning of line pointer */
	register int	lineno;	/* line number */

	p = filep->f_p;
	eof = filep->f_end;
	if (p >= eof)
		return((char *)NULL);
	lineno = filep->f_line;

	for(bol = p--; ++p < eof; ) {
		if (*p == '/' && *(p+1) == '*') { /* consume comments */
			*p++ = ' ', *p++ = ' ';
			while (*p) {
				if (*p == '*' && *(p+1) == '/') {
					*p++ = ' ', *p = ' ';
					break;
				}
				else if (*p == '\n')
					lineno++;
				*p++ = ' ';
			}
			continue;
		}
		else if (*p == '\n') {
			lineno++;
			if (*bol == '#') {
				*p++ = '\0';
				goto done;
			}
			bol = p+1;
		}
	}
	if (*bol != '#')
		bol = NULL;
done:
	filep->f_p = p;
	filep->f_line = lineno;
	return(bol);
}

char *basename(file)
	register char	*file;
{
	register char	*p;

	for (p=file+strlen(file); p>file && *p != '/'; p--) ;

	if (*p == '/')
		p++;

	file = copy(p);
	for(p=file+strlen(file); p>file && *p != '.'; p--) ;

	if (*p == '.')
		*p = '\0';
	return(file);
}

void redirect(char *linep, char *makefile)
{
	struct stat	st;
	FILE	*fdin, *fdout;
	char	backup[ BUFSIZ ],
		buf[ BUFSIZ ];
	boolean	found = FALSE;
	int	len;

	/*
	 * if makefile is "-" then let it pour onto stdout.
	 */
	if (makefile && *makefile == '-' && *(makefile+1) == '\0')
		return;

	/*
	 * use a default makefile is not specified.
	 */
	if (!makefile) {
		if (stat("makefile", &st) == 0)
			makefile = "makefile";
		else if (stat("Makefile", &st) == 0)
			makefile = "Makefile";
		else
			log_fatal("[mM]akefile is not present\n");
	}
	else
	    stat(makefile, &st);
#ifdef SYSV
		if (access(makefile, R_OK) < 0)
			log_fatal("cannot access \"%s\"\n", makefile);
#else			
		if ((fdin = fopen(makefile, "r")) == NULL)
			log_fatal("cannot open \"%s\"\n", makefile);
#endif			
	sprintf(backup, "%s.bak", makefile);
	unlink(backup);
#ifdef SYSV
	if (link(makefile, backup) < 0)
		log_fatal("cannot link %s to %s\n", makefile, backup);

	unlink(makefile);
	if ((fdin = fopen(backup, "r")) == NULL)
		log_fatal("cannot open \"%s\"\n", backup);
#else
	if(rename(makefile, backup) < 0)
                log_fatal("cannot rename %s to %sn", makefile, backup);
#endif				
	if ((fdout = freopen(makefile, "w", stdout)) == NULL)
		log_fatal("cannot reopen \"%s\"\n", makefile);

	len = strlen(linep);
	while (fgets(buf, BUFSIZ, fdin) && !found) {
		if (*buf == '#' && strncmp(linep, buf, len) == 0)
			found = TRUE;
		fputs(buf, fdout);
	}
	if (!found) {
		log_warn("delimiting line \"%s\" not found...\nAppending...\n",
			linep);
		puts(linep); /* same as fputs(fdout); but with newline */
	}
	fflush(fdout);
#ifdef SYSV
	chmod(makefile,st.st_mode);
#else	
	fchmod(fileno(fdout), st.st_mode);
#endif /* SYSV	*/


}
