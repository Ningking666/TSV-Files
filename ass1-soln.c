/* Program to generate hierarchical reports from TSV structured data

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2024, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2024

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: [Enter your full name and student number here before submission]
   Dated:     [Enter the date that you "signed" the declaration]

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>

#define CHAR_CR '\r'         /* CR character, in PC-format text files */
#define CHAR_NL '\n'         /* newline character, ends each input line */
#define CHAR_TB '\t'         /* tab character, ends each TSV field */

#define STATUS_EOL 1         /* return value for end of line field */
#define STATUS_EOF 2         /* return value for end of file field */
#define STATUS_NML 3         /* return value if neither of previous two */

#define MAXFIELDLEN 50       /* maximum field length */

/* one tsv field, stored within a fixed-length character string */
typedef char field_t[MAXFIELDLEN+1];

/* SOLUTION-START */
#include <unistd.h>

#define DEBUG 0              /* set to 1 if debugging output wanted */

#define MAXROWS 1000         /* maximum number of TSV rows */
#define MAXCOLS 30           /* maximum number of TSV columns */

#define HEADINGS 0           /* to access the heading row labels */

/* the whole tsv structure, stored as a 2d array of tsv entries */
typedef field_t tsvmat_t[MAXROWS][MAXCOLS];
/* SOLUTION-END */

/**************************************************************/

int  mygetchar(void);
int  getfield(field_t dest);

/* SOLUTION-START */
void read_whole_tsv(tsvmat_t T, int *nrows, int *ncols);
void get_col_nums(char *argv[], int argc, int C[], int *ncs);
void check_col_nums(int C[], int ncs, int ncols);
void stage_one(tsvmat_t T, int nrows, int ncols);
void print_stage(int stage);
void print_row(tsvmat_t T, int ncols, int row);
void print_tsv(tsvmat_t T, int nrows, int ncols);
void stage_two(tsvmat_t T, int nrows, int ncols, int C[], int ncs);
int  compare_rows(tsvmat_t T, int ncols, int rowi, int rowj, 
		int C[], int ncs);
void swap_rows(tsvmat_t T, int ncols, int rowi, int rowj);
void stage_three(tsvmat_t T, int nrows, int ncols, int C[], int ncs);
void print_heading(tsvmat_t T, int row, int C[],
		int col1, int col2, int lastcolwid);
int  longest_in_col(tsvmat_t T, int nrows, int col);
void print_divider(int ncs, int lastcolwid);
/* SOLUTION-END */

/**************************************************************/

/* main program provides traffic control
*/
int
main(int argc, char *argv[]) {

/* SOLUTION-START */

	/* the main data */
	tsvmat_t T;
	int nrows=0, ncols=0;
	/* auxiliary data, the column list */
	int C[MAXCOLS];
	int ncs;

	/* read the whole of the input stream in to T */
	read_whole_tsv(T, &nrows, &ncols);
	// print_tsv(T, nrows, ncols);
	get_col_nums(argv, argc, C, &ncs);
	check_col_nums(C, ncs, ncols);

	/* debugging output if needed */
	if (DEBUG) {
		print_tsv(T, nrows, ncols);
	}

	stage_one(T, nrows, ncols);
	if (ncs>0) {
		stage_two(T, nrows, ncols, C, ncs);
		if (DEBUG) {
			print_tsv(T, nrows, ncols);
		}
		stage_three(T, nrows, ncols, C, ncs);
	}
/* SOLUTION-END */

	/* all done, time to go home */
	printf("ta daa!\n");
	return 0;
}

/**************************************************************/

/* read characters and build a string, stopping when a tab or newline
   as encountered, with the return value indicating what that
   terminating character was
*/
int
getfield(field_t dest) {

	int ch, nchs=0;
	int status=STATUS_NML;

	dest[0] = '\0';
	while ((ch=mygetchar())
		&& (ch != CHAR_TB) && (ch != CHAR_NL) && (ch != EOF)) {

		if (nchs < MAXFIELDLEN) {
			/* ok to save this character */
			dest[nchs++] = ch;
			dest[nchs] = '\0';
		} else {
			/* silently discard extra characters if present */
		}
	}

	/* return status is defined by what character stopped the loop */
	if (ch == EOF) {
		status = STATUS_EOF;
	} else if (ch == CHAR_NL) {
		status = STATUS_EOL;
	} else if (ch == CHAR_TB) {
		status = STATUS_NML;
	}
	return status;
}

/**************************************************************/

/* read a single character, bypassing any CR characters encountered,
   so as to work correctly with either PC-type or Unix-type input
 */
int
mygetchar(void) {
	int c;
	while ((c=getchar()) == CHAR_CR) {
		/* empty loop body */
	}
	return c;
}

/**************************************************************/

/* SOLUTION-START */

/* read the whole of the input data into the tsvmat_t structure,
   and sets nrows and ncols to reflect. Note that it is assumed
   that MAXROWS and MAXCOLS and MAXFIELDLEN won't be violated, and
   that the input tsv will be well-formed, with same number of
   data items in every row, and a single newline at end of every
   line
*/
void
read_whole_tsv(tsvmat_t T, int *nrows, int *ncols ) {
	int status;
	int nr=0, nc=0;
	while (1) {
		/* get the next entry */
		status = getfield(T[nr][nc]);
		/* and see where it fits */
		if (status==STATUS_NML) {
			/* all ok, advance to next entry in same row */
			nc++;
		} else if (status==STATUS_EOL) {
			/* times for a new row to start */
			nr++;
			*ncols = nc+1;
			nc = 0;
		} else if (status==STATUS_EOF) {
			/* that's it, no more coming */
			*nrows = nr;
			return;
		}
	}
	return;
}


/**************************************************************/

/* extract the command-line column subset into array C
   and buddy variable ncs */
void
get_col_nums(char* argv[], int argc, int C[], int *ncs) {
	int i;
	for (i=1, *ncs=0; i<argc; i++) {
		C[*ncs] = atoi(argv[i])-1;
		/* the minus one is to set origin of counting to be zero */
		(*ncs)++;
	}
	return;
}

/**************************************************************/

/* and now check that all the given column numbers are within 
   the correct range, just to be careful
*/
void
check_col_nums(int C[], int ncs, int ncols) {
	int c;
	int have_a_problem = 0;
	for (c=0; c < ncs; c++) {
		if (C[c] >= ncols) {
			printf("column number %d is not defined\n",
				C[c]+1);
			have_a_problem = 1;
		}
	}
	if (have_a_problem) {
		printf("execution ended\n");
		exit(EXIT_FAILURE);
	}
}

/**************************************************************/

/* carry out the required stage one action:
   just print the last input line in the required format
*/
void
stage_one(tsvmat_t T, int nrows, int ncols) {
	print_stage(1);
	printf("input tsv data has %d rows and %d columns\n",
			nrows-1, ncols);
	print_row(T, ncols, nrows-1);
	printf("\n");
	return;
}

/**************************************************************/

/* print the stage number in the required format
*/
void
print_stage(int stage) {
	printf("Stage %d\n", stage);
	return;
}

/**************************************************************/

/* print out the fields from a specified row of the tsv matrix
   using the required format
*/
void
print_row(tsvmat_t T, int ncols, int row) {
	int c;
	printf("row %d is:\n", row);
	for (c=0; c<ncols; c++) {
		printf("  %2d: %-10s %s\n",
			c+1, T[HEADINGS][c], T[row][c]);
	}
}

/**************************************************************/

/* carry out the required stage two action:
    -- sort the rows of the tsv matrix according to the list of
       column numbers specified by vector C;
    -- if same across the set of field numbers, original ordering
       is to be preserved
   then print three particular rows using the required output format
*/
void
stage_two(tsvmat_t T, int nrows, int ncols, int C[], int ncs) {
	int c, result;
	int rowi, rowj;

	print_stage(2);
	for (c=0; c < ncs; c++) {
		if (c==0) {
			printf("sorting by ");
		} else {
			printf("\n   then by ");
		}
		printf("\"%s\"", T[HEADINGS][C[c]]);
		if (c < ncs-1) {
			printf(",");
		}
	}
	printf("\n");

	/* insertionsort over the rows of T, using the permuted columns
	   as the sort key via function compare_rows(). Note that the
	   first row of T is **not** part of the sort and does not get
	   moved */
	for (rowi=2; rowi<nrows; rowi++) {
		for (rowj=rowi-1; rowj>=1; rowj--) {
			result = compare_rows(T, ncols, rowj, rowj+1, C, ncs);
			if (result<=0) {
				/* element has reached right place */
				break;
			}
			/* otherwise, swap and continue */
			swap_rows(T, ncols, rowj, rowj+1);
		}
	}
	/* and now the "proof of correctness" output that is required
	   by the project specification */
	print_row(T, ncols, 1);
	print_row(T, ncols, nrows/2);
	print_row(T, ncols, nrows-1);
	printf("\n");
	return;
}

/**************************************************************/

/* comparison function to compare two rows of the tsv in the order of
   the field numbers (columns) supplied in vector C
*/
int
compare_rows(tsvmat_t T, int ncols, int rowi, int rowj, 
		int C[], int ncs) {
	int c, res, col;
	for (c=0; c<ncs; c++) {
		col = C[c];
		res = strcmp(T[rowi][col], T[rowj][col]);
		if (res) {
			/* we have an outcome */
			return res;
		}
	}
	/* got all way through without an outcome being being
	   identified using the selected columns, so are regarded as
	   being equal */
	return 0;
}

/**************************************************************/

/* swap over two complete rows of the tsv matrix, field-by-field
*/
void
swap_rows(tsvmat_t T, int ncols, int rowi, int rowj) {
	int c;
	char tmp[MAXFIELDLEN+1];
	for (c=0; c<ncols; c++) {
		strcpy(tmp,        T[rowi][c]);
		strcpy(T[rowi][c], T[rowj][c]);
		strcpy(T[rowj][c], tmp);
	}
	return;
}

/**************************************************************/

/* carry out the required stage three action:
   process through the rows printing a summary tabulated according
   to the fields supplied in vector C
*/
void
stage_three(tsvmat_t T, int nrows, int ncols, int C[], int ncs) {
	int r, c, i;
	int count=1;
	int lastcolwid;
	print_stage(3);
	lastcolwid = longest_in_col(T, nrows, C[ncs-1]);
	print_divider(ncs, lastcolwid);
	print_heading(T, 0, C, 0, ncs-1, lastcolwid);
	printf(" Count\n");
	print_divider(ncs, lastcolwid);
	print_heading(T, 1, C, 0, ncs-1, lastcolwid);
	for (r=2; r<nrows; r++) {
		for (i=0; i<ncs; i++) {
			c = C[i];
			if (strcmp(T[r-1][c], T[r][c])) {
				/* difference found */
				printf(" %5d\n", count);
				print_heading(T, r, C, i, ncs-1, lastcolwid);
				count = 0;
				break;
			}
		}
		count++;
	}
	/* and the last count still has to be output, need to be sure
	   that the format descriptor here matches the one a few lines 
	   back, should really make it a function */
	printf(" %5d\n", count);
	print_divider(ncs, lastcolwid);
	return;
}

/**************************************************************/

/* returns the length of the longest string in the col'th of the 
   tsv matrix's columns, including the header row
*/
int
longest_in_col(tsvmat_t T, int nrows, int col) {
	int r;
	int l, maxl=0;
	for (r=0; r<nrows; r++) {
		l = strlen(T[r][col]);
		if (l>maxl) {
			maxl = l;
		}
	}
	return maxl;
}

/**************************************************************/

/* print the divider row for the stage 3 table. Note the magic numbers
   4 and 2 arise as part of the layout requirements in the program
   specification
*/
void
print_divider(int ncs, int lastcolwid) {
	int i;
	for (i=0; i<4*ncs+lastcolwid+2; i++) {
		printf("-");
	}
	printf("\n");
	return;
}

/**************************************************************/

/* print a multi-line progress heading that covers col1 through
   to col2, then leaves the output "hanging" while the number of
   matching rows is counted
*/
void
print_heading(tsvmat_t T, int row, int C[],
		int col1, int col2, int lastcolwid) {
	int i, c, t;
	for (i=col1; i<=col2; i++) {
		c = C[i];
		for (t=0; t<i; t++) {
			printf("    ");
		}
		if (i<col2) {
			printf("%s\n", T[row][c]);
		} else {
			printf("%-*s", lastcolwid, T[row][c]);
		}
	}
	return;
}

/**************************************************************/

/* function to help with debugging, just prints out the whole
   of the tsv structure
*/
void
print_tsv(tsvmat_t T, int nrows, int ncols) {
	int r, c;
	for (r=0; r<nrows; r++) {
		printf("  %2d: ", r);
		for (c=0; c<ncols; c++) {
			printf("%-15s", T[r][c]);
		}
		printf("\n");
	}
	return;
}
/* SOLUTION-END */
