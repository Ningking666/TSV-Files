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

   Signed by: XIAONINGTIAN 1441462
   Dated:     2024.9.8
   algorithms are fun!
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>

#define CHAR_CR '\r'	     /* CR character, in PC-format text files */
#define CHAR_NL '\n'         /* newline character, ends each input line */
#define CHAR_TB '\t'         /* tab character, ends each TSV field */

#define STATUS_EOL 1         /* return value for end of line field */
#define STATUS_EOF 2         /* return value for end of file field */
#define STATUS_NML 3         /* return value if neither of previous two */

#define MAXFIELDLEN 50
#define MAXCOL 30
#define MAXROW 1000
/* maximum field length */

/* one tsv field, stored within a fixed-length character string */
typedef char field_t[MAXFIELDLEN+1];


/**************************************************************/

int  mygetchar(void);
int  getfield(field_t dest);
int  read_data(field_t header[]);
void insertion_sort(field_t total[][MAXCOL],int num_row,int sorting_byindex[],int num_col, int argc);
void row_swap(field_t row1[],field_t row2[],int num_col);
void print_row(int row_num, field_t row[], field_t header[],int num_col);
int longest_len(field_t total[][MAXCOL], int j, int num_row);
void column_counting(int sorting_byindex[], int index_col, int num_index, field_t total[][MAXCOL], int start, int end, int num_row);
void freq_count(field_t total[][MAXCOL], int start, int end, int j, int index_col, int num_row);
void print_stage3row(field_t total[][MAXCOL],int num_row,field_t header[],int argc, char *argv[]);
void print_separator(int length);
void printing_s(field_t total[][MAXCOL], int argc, char * argv[], int num_row);
void process_stage1(field_t total[][MAXCOL], field_t header[], int num_row, int num_col);
void process_stage2(field_t total[][MAXCOL], field_t header[], int argc, char *argv[], int sorting_byindex[], int num_row, int num_col);
void process_stage3(field_t total[][MAXCOL], int num_row, field_t header[], int argc, char *argv[], int sorting_byindex[]);
/**************************************************************/

/* main program provides traffic control
*/
int
main(int argc, char *argv[]) {

    /*Stage1*/
    int num_col, num_row=0;
    field_t header[MAXCOL];
    field_t total[MAXROW][MAXCOL];
    int sorting_byindex[MAXCOL]; 
    num_col= read_data(header);
    while (read_data(total[num_row]) != 0){
      num_row++;
   }
   
    process_stage1(total,header,num_row, num_col);
	
    /*stage1 finished*/
    
    
    process_stage2(total,header,argc, argv, sorting_byindex, num_row, num_col);

    /*Stage2 finished*/

        
    
    process_stage3(total,num_row, header, argc, argv, sorting_byindex);

    /*Stage3 finished*/
    printing_s(total,argc,argv,num_row);

    printf("ta daa!\n");
   
    return 0;
}

/* the required stage 1 output
    num of row and col
    and the content of last row 

*/

 void 
 process_stage1(field_t total[][MAXCOL], field_t header[], int num_row, int num_col){
    
    
    
    printf("Stage 1\n");
    printf("input tsv data has %d rows and %d columns\n", num_row, num_col);
    printf("row %d is:\n", num_row);

    for (int i=0;i<num_col;i++){
        printf("%4d: %-11s%s\n", i+1,header[i],total[num_row -1][i]);
   }
  
    printf("\n");
	

 }
 /* the required stage 2 output
    sorting by several columns
    print the content of first, middle and last row 
 
 */

 void 
 process_stage2(field_t total[][MAXCOL],field_t header[], int argc, char *argv[], int sorting_byindex[], int num_row, int num_col){
    
    printf("Stage 2\n");

    if(argc == 1){
        return;
    }
   

    for (int i=1; i< argc; i++){
        int n= atoi(argv[i]);
        if (i==1){
            printf("sorting by \"%s\",\n", header[n-1]);

        }else if(i != argc-1){
            printf("   then by \"%s\",\n", header[n-1]);


        }else{
            printf("   then by \"%s\"\n", header[n-1]);
        }
        sorting_byindex[i-1]=n-1;

    }
      insertion_sort(total,num_row,sorting_byindex,num_col,argc-1);
      print_row(1, total[0],header, num_col);
      int a= (num_row+1)/2;
      print_row(a,total[a-1],header,num_col);
      print_row(num_row, total[num_row-1],header, num_col);

      printf("\n");
 }


/* the required output of stage3
    the seperating line 
    and the header of sorting_col
    count of sorting_row 
*/
 void 
 process_stage3(field_t total[][MAXCOL], int num_row, field_t header[], int argc, char *argv[], int sorting_byindex[]) {
    printf("Stage 3\n");
    printing_s(total,argc,argv,num_row);

    for (int i = 1; i < argc; i++) {
        int n = atoi(argv[i]);
        int max_len = longest_len(total, n, num_row);

        if (i == 1) {
            printf("%s\n", header[n-1]);
        } else {
            printf("    %-*s   Count\n", max_len, header[n-1]);
            printing_s(total,argc,argv,num_row);
        }
    }

    column_counting(sorting_byindex, 0, argc-1, total, 0, num_row, num_row);
 }

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

			dest[nchs++] = ch;
			dest[nchs] = '\0';
		} else {

		}
	}


	if (ch == EOF) {
		status = STATUS_EOF;
	} else if (ch == CHAR_NL) {
		status = STATUS_EOL;
	} else if (ch == CHAR_TB) {
		status = STATUS_NML;
	}
	return status;
}



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


/*read the header data and judge the status to let the process stop*/

int 
read_data(field_t header[]){
   int count=0,s;
   while(1){
      s=getfield(header[count]);
      if (strlen(header[count])==0){
         return count;
      }
      count +=1;
      if ((s == STATUS_EOF) || (s ==  STATUS_EOL) ){
         break;
      }
   }
    return count;
}

/*using a insertion_sort and read the column we need to sort 
 comparing the row by using strcmp 
 if they are different. just row_swap
*/

void 
insertion_sort(field_t total[][MAXCOL],int num_row,int sorting_byindex[],int num_col, int num_index){
    for (int i = 1; i < num_row; i++){
        for (int j = i;j > 0; j--){
            for(int index = 0;index < num_index; index++){
                int sort_col=sorting_byindex[index];
                if (strcmp(total[j-1][sort_col],total[j][sort_col])>0){
                    row_swap(total[j-1],total[j],num_col);
                    break;
                }else if(strcmp(total[j-1][sort_col],total[j][sort_col])==0){
                    continue;

                }else{
                    break;
                }
            }

        }

    }
}

/* a function that make row swap easily
   using strcpy to copy the original str and replace with next row
*/
void
row_swap(field_t row1[],field_t row2[],int num_col){
    for(int i = 0; i < num_col; i++){
        field_t temp;
        strcpy(temp, row1[i]);
        strcpy(row1[i], row2[i]);
        strcpy(row2[i], temp);

    }
}

/* to print each row in stage2 */
void
print_row(int row_num, field_t row[], field_t header[],int num_col){
    printf("row %d is:\n", row_num);
    for (int i = 0; i < num_col; i++){
        printf("%4d: %-11s%s\n", i+1, header[i], row[i]);
    }
}

/* If index_col equals num_index - 1 indicates that the innermost column has been reached, 
   and freq_count is called to count and print the frequency of each value.

    A for loop is used to iterate through all rows in the current segment.
    For each row, strcmp is used to check if adjacent rows have the same value
     and the occurrences of the current value are counted.


*/
void
column_counting(int sorting_byindex[], int index_col, int num_index, field_t total[][MAXCOL], int start, int end, int num_row) {
    int j = sorting_byindex[index_col];

    if (index_col == num_index - 1) {
        freq_count(total, start, end, j, index_col, num_row);
        return;
    }

    for (int i = start; i < end;) {
        int count = 1;
        while (i + 1 < end && strcmp(total[i][j], total[i + 1][j]) == 0) {
            count++;
            i++;

        }
        
        printf("%s\n", total[i][j]);
        column_counting(sorting_byindex, index_col + 1, num_index, total, i - count + 1, i + 1, num_row);
        i++;
    }
}

/* calculating freq
    using strcmp to compare if equal, freq++ and check the next row
    the max_len should be used in formatting
    print 4* index_col is to check the space we need
*/
void
freq_count(field_t total[][MAXCOL], int start, int end, int j, int index_col, int num_row) {
    int max_len = longest_len(total, j, num_row);
    for (int i = start; i < end;) {
        int freq = 1;
        while (i + 1 < end && strcmp(total[i][j], total[i + 1][j]) == 0) {
            freq++;
            i++;
      
        }
        printf("    %-*s %d\n",max_len+4,total[i][j],freq);
      i++;
    
    
    }
}
/* calculating the longestlen of str 
    and all str should be suitable for it
*/

int
longest_len(field_t total[][MAXCOL], int j, int num_row) {
    int max_len = 0;
    for (int i = 0; i < num_row; i++) {
        int len = strlen(total[i][j]);
        if (len > max_len) {
            max_len = len;
        }
    }
    return max_len;
}

// using for loop to print separator
void 
print_separator(int length) {
    for (int i = 0; i < length; i++) {
      printf("-");
    
    }
}

//separator line should always be formatting by longest_word

 void
      printing_s(field_t total[][MAXCOL], int argc, char * argv[], int num_row){
      for (int i=1; i< argc; i++){
            int n= atoi(argv[i]); 
            int max_len=longest_len(total, n, num_row); 
            print_separator(max_len-1);
         }
      printf("\n");
      }



