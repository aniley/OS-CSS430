/*
 * Name: Aniley Sabi
 * Project: OS Soduku Validator Using Multi-threading
 * Description: Using Pthreads check if a given 9x9 soduku's row, column and sub grids are valid.
 * 				This program creates 11 pthreads and the first two threads are for the rows and columns of
 * 				the soduku. And the rest 9 threads are to check the validity of the 9 sub grids of the soduku
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# define NUM_OF_THREADS 11 // Number of threads

/*================================================================================
 * @indexHolder: global variable that is to hold the index of the threads
 *               The reason I started from 1, is that row and column threads take the
 *               index flag zeroth and first index @Valid array respectively. The rest
 *               is for the subgrids flags
 *
 *====================================================================================
 */
int indexHolder = 1;

// valid[0] : this is index for row thread
// valid[1] : this is index for column thread check
// valid[2-10]: these indices are for the 9 subgrids section

int valid[NUM_OF_THREADS] = {0}; // An array to hold the validity flag either 1 or 0, initialized to 0


 //struct: the struct has two data fields: row and column

typedef struct{
	int row;
	int column;
} parameters;

//int grid[9][9]  this data is shared by the thread(s)
// a given 9x9 soduku sections
const int grid[9][9] = {

		{6, 2, 4, 5, 3, 9, 1, 8, 7},

		{5, 1, 9, 7, 2, 8, 6, 3, 4},

		{8, 3, 7, 6, 1, 4, 2, 9, 5},

		{1, 4, 3, 8, 6, 5, 7, 2, 9},

		{9, 5, 8, 2, 4, 7, 3, 6, 1},

		{7, 6, 2, 3, 9, 1, 4, 5, 8},

		{3, 7, 1, 9, 5, 6, 8, 4, 2},

		{4, 9, 6, 1, 8, 2, 5, 7, 3},

		{2, 8, 5, 4, 7, 3, 9, 1, 6}
};
/*==================================================================
 *runnerRow: void pointer method, accepts a pointer @param and checks
 *           that each of the rows of the soduku is valid or no.
 *           if the a row is valid it updates the @valid array flag with 1,
 *           if not the thread would exit.
 *
 * ==================================================================
 */
void *runnerRow(void *param)
{

	parameters* params = (parameters*)param;
	int row = params->row;
	int col = params->column;

	if (col != 0 || row > 8)
	{
		fprintf(stderr, "Invalid row or column for row subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}
	// Check if numbers 1-9 only appear once in the row
	int i;
		for (i = row; i < 9; i++)
		{
	// @validityCheck: local  array variable with size of 9, to hold
	// validity flag of a given row. it gets updated with 1 if a number is not seen
	// else that number is seen and the thread would exit.

			int validityCheck[9] = { 0 };
			for (int j = col; j < 9; j++)
			{
				int num = grid[i][j];
				// check if @num is valid, and not duplicated in a row
				if (num < 1 || num > 9 || validityCheck[num - 1] == 1)
				{
					//@num is either duplicated or not in between 1- 9
					pthread_exit(NULL);
				}
				else
				{
				// number not seen and validityCheck flag updtated to 1
					validityCheck[num - 1] = 1;
				}
			}
}

// If reached this point, row subsection is valid.
// valid[0]: update the the Zeroth index of Valid array flag to 1
	valid[0] = 1;

	pthread_exit(NULL);

}
/*==================================================================
 *runnerColumn: void pointer method, accepts a pointer @param and checks
 *           that each of the columns of the soduku is valid or no.
 *           if the a column is valid it updates the @valid array flag with 1,
 *           if not the thread would exit.
 *
 * ==================================================================
 */
void *runnerColumn(void *param){


	parameters *params = (parameters*) param;
	int row = params->row;
	int col = params->column;
	if (row != 0 || col > 8) {
		fprintf(stderr, "Invalid row or column for row subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}
	// Check if numbers 1-9 only appear once in the column

	int i;
	for (i = col; i < 9; i++) {
        // @validityCheck: local  array variable with size of 9, to hold
		// validity flag of a given column. it gets updated with 1 if a number is not seen
		// else that number is seen and the thread would exit.
		int validityCheck[9] = {0};
		for(int j = row; j < 9; j++)
		{
			int num = grid[j][i];
			// check if @num is valid, and not duplicated in a column
			if (num < 1 || num > 9 || validityCheck[num - 1] == 1)
			{
				//@num is either duplicated or not in between 1- 9
				pthread_exit(NULL);
			}
			else
			{
				// @num is not seen and validityCheck flag updtated to 1
				validityCheck[num - 1] = 1;

			}
		}
	}

// If reached this point, column subsection is valid.
// Valid[1]: update the the first index of Valid array flag to 1
	valid[1] = 1;

	pthread_exit(NULL);

}

/*=========================================================================
 **runnerBlocks: void pointer method, accepts a pointer @param and checks
 *            each of the  3x3 sub grids the soduku is valid or not.
 *           if the a 3x3 sub grid is valid it updates the @valid array flag with 1,
 *           if not the thread would exit.
 *
 * ==========================================================================
 */

void *runnerBlocks(void *param){

// Confirm that parameters indicate a valid 3x3 subsection
	parameters *params = (parameters*) param;

	int row = params->row;
	int col = params->column;
	if (row > 6 || row % 3 != 0 || col > 6 || col % 3 != 0) {

		fprintf(stderr, "Invalid row or column for subsection! row=%d, col=%d\n", row, col);

		pthread_exit(NULL);

	}
	// @validityCheck: local array variable  initialized to 0 with size of 9, to hold
	// validity flag of a given 3x3 subgrid. it gets updated with 1 if the subgrid is valid
	// else it is not valid and  the thread would exit.
	int validityCheck[9] = {0};

	int i, j;
   // Loops through each sub grids of the soduku,
   // the loop, i and j incremented by 3, to accommodate 3x3 rule
	for (i = row; i < row + 3; i++) {

		for (j = col; j < col + 3; j++) {

			int num = grid[i][j];
			// check if @num is valid, and not duplicated in the sub grid
			if (num < 1 || num > 9 || validityCheck[num - 1] == 1)
			{
				//@num is either duplicated or not in between 1- 9
				pthread_exit(NULL);

			}
			else
				{
				// @num is not seen or valid, then  validityCheck flag updated to 1
				validityCheck[num - 1] = 1;
			}
		}
	}

  // If reached this point, 3x3 subsection is valid.
	//valid[2-10] = 1; // updates the subsection to an indices from valid[2] to valid[10]
	//@indexHolder gets pre incremented, because indexHolder started with 1
	// indexHolder 0 is for row, and indexHolder 1 is for column validity flags
	// to be able to update valid[2-10] indices, @indexHolder gets pre incremented by 1
	valid[++indexHolder] = 1;

	pthread_exit(NULL);

}


int main(int argc, char *argv[])
{

	pthread_t threads[NUM_OF_THREADS];

	int threadIndex = 0; // local variable associated with number of threads created
						// it gets incremented each time a thread is created

	// this for loop creates 2 threads that checks the validity of row and column in the soduku
	// by calling runnerRow and runnerColumn pointer method
	for(int a = 0; a < 2 ; a++){

		if(a == 0)
		{
			parameters* rowData = (parameters*)malloc(sizeof(parameters));
			rowData->row = a;
			rowData->column = a;
			pthread_create(&threads[threadIndex++], NULL, runnerRow, rowData);
		}
		else
		{
			parameters* columnData = (parameters*)malloc(sizeof(parameters));
			columnData->row = a - 1;
			columnData->column = a - 1;
			pthread_create(&threads[threadIndex++], NULL, runnerColumn, columnData);
		}
	}

	//this loop create the rest 9 threads that is to check the validity of the sub grids

	for(int i = 0; i < 9; i = i+3){
		for(int j = 0; j < 9; j = j+3){
			parameters *data = (parameters *) malloc(sizeof(parameters));
			data->row = i;
			data->column = j;

			pthread_create(&threads[threadIndex++], NULL, runnerBlocks, data);

		}
	}

	// this loop joins the 11 threads created
	for(int c = 0; c < NUM_OF_THREADS; c++)
	{
		pthread_join(threads[c], NULL);
	}

	// this loop through the @valid[] array and checks is the flags in the valid array is 0
	// then, the soduku is not valid and the program exit
	for (int i = 0; i < NUM_OF_THREADS; i++) {

		if (valid[i] == 0) {

			printf("Sudoku solution is invalid!\n");

			return EXIT_SUCCESS;

		}

	}
	// if reaches here, the soduku's row, column, and sub grids are valid. Prints valid
	printf("Sudoku solution is valid!\n");

	return 0;
}








