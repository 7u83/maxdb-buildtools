/* @(#)pgram/yycosts.c

    ========== licence begin LGPL
    Copyright (C) 2002 SAP AG

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    ========== licence end

 */

#include "0.h"
#include "yy.h"

/*
 * Symbol costs for Pascal.
 *
 * Cost strategy of August 14, 1977.
 *
 * The costs determined by the routines in this file are used by
 * the recovery in choosing appropriate corrections.
 * The cost vectors and the error productions in the grammar
 * work together to define the corrective capacity of the grammar.
 *
 * The costs here largely derive from those given in Steve Rhode's
 * thesis for the Pascal-I error correcting parser which he implemented.
 * Some minor changes have been made to adjust for the fact that
 * the current error recovery is not as "smart", both because of the
 * limited forward move and because of the lack of any type information
 * about identifiers.
 *
 * These adjustments largely take the form of increased costs for certain
 * tokens, noticeably keywords which are major brackets such as "begin"
 * "label", "procedure", etc.
 *
 * The overall weighting strategy is still similar to Rhodes' strategy.
 * The costs can be considered:
 *
 *	LOW	<= 3
 *	MEDIUM	4 or 5
 *	HIGH	>= 6
 */
 
/*
 * Insertion costs
 *
 * In addition to the normal symbol insertion costs,
 * there are zero cost insertions here.
 * The current error recovery system treats symbols
 * which have zero insertion cost in a special way,
 * inserting them but suppressing diagnostics.
 * This allows the system to hold of on bracketing
 * error diagnostics about missing end's until the
 * reduction occurs which knows the line number of the
 * corresponding "begin", "repeat", etc.
 * A more intelligent and useful diagnostic can then
 * be printed.
 *
 * Although this routine never allows the insertion
 * of the keyword begin, it can be inserted after a
 * procedure or function body starts, if it was omitted
 * by a special case in the panic routine, which notices
 * the keywords in the statement body of the procedure
 * and inserts the begin to recover.
 *
 * Similarly, we do not insert end-of-file, but
 * the fact that end-of-file is the unique input
 * is noticed by the recovery routines as a special
 * case and handled there.
 */
inscost(sy, before)
	register int sy, before;
{

	switch (before) {
		case YEND:
			if (sy == YEND)
				break;
		case YPROCEDURE:
		case YFUNCTION:
			if (sy == YUNTIL || sy == YEND)
				return (0);
	}
	switch (sy) {
		case ';':
			return (1);
		case ',':
		case ':':
		case YOF:
		case YDO:
			return (2);
		case YARRAY:
		case '+':
		case '*':
			return (3);
		default:
			return (4);
		case '^':
		case YNOT:
		case YLABEL:
		case YCONST:
		case YTYPE:
		case YVAR:
		case YUNTIL:
		case '(':
		case '[':
		case YWHILE:
		case YWITH:
			return (5);
		case YPROCEDURE:
		case YFUNCTION:
		case YCASE:
			return (6);
		case YEND:
			return (8);
		case YBEGIN:
		case YEOF:
		case YREPEAT:
		case YRECORD:
			return (INFINITY);
	}
}

/*
 * Replacement costs
 *
 * Most replacement costs are the same as an insertion
 * plus a deletion cost.  One special case is the replacement
 * of a large number of keywords by an identifier.
 * These are given lower costs, especially the keyword "to".
 */
repcost(what, with)
	register int what, with;
{
	register int c;

	if (with == what)
		return (INFINITY);
	if (with == id && what > ERROR)
		switch (what) {
			case id:		/* EF4: 11-08-86 WB ... */
			case Ydotdot:
/*
 *			case YINT:
 *			case YBINT:
 */
			case string:
			case number:
				break;
			case YTO:
				return (3);
			default:
				return (5);
			case YRECORD:
			case YTHEN:
				return (6);
			case YBEGIN:
				break;
		}
	if (what == ';' && (with == ',' || with == '.'))
		return (CLIMIT - 1);
	c = delcost(what) + inscost(with, what);
	/*
	 * It costs extra to replace something which has
	 * semantics by something which doesn't.
	 */
	if (nullsem(what) == NIL && nullsem(with) != NIL)
		c += 4;
	return (c);
}

/*
 * Deletion costs
 */
delcost(what)
	int what;
{

	switch (what) {
		case '.':
		case ':':
		case ',':
		case '=':
		case '(':
			return (3);
		case YELSE:
		case YTHEN:
			return (4);
		default:
			return (5);
		case YLABEL:
		case YCONST:
		case YTYPE:
		case YVAR:
			return (10);
		case YPROCEDURE:
		case YFUNCTION:
		case YBEGIN:
		case YEND:
			return ((CLIMIT * 3) / 4);
		case ';':
		case YEOF:
			return (INFINITY);
	}
}
#ifdef DEBUG

/*
 * Routine to print out costs with "-K" option.
 */
char	yysyms[] = ";,:=*+/-|&()[]<>~^";


yycosts()
{
	register int c;
	register char *cp;

	printf("Insert\tDelete\tRep(ID)\tSymbol\n");
	for (cp = yysyms; *cp; cp++)
		yydocost(*cp);
	for (c = ERROR + 1; c < LASTKEYWORD; c++)
		yydocost(c);
}

yydocost(c)
	int c;
{

	printf("%4d\t", inscost(c, -1));
	printf("%4d\t", delcost(c));
	if (repcost(c, id) != inscost(id, c) + delcost(c))
		printf("%4d", repcost(c, id));
	printf("\t%s%s\n", charname(c,1));
}
#endif