/*==================================================
 * Name: Aniley Sabi
 * Project: CSS 430 OS Memory Management
 * Description: Contigious Memory Allocations
 * Date: May 5, 2021
 * ==================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MEMSIZE 80
#define SIZE 100
char memory[MEMSIZE];

/*******************************************************************************
 * The Execution flow: In the main method, there's a variable called should_run
 * 						which tells the program whether or not to keep executing.
 * 						should_run become 0 and the while loop exit if the allocated
 * 						memory is greater than of the MEMSIZE,
 * Output:  		   Anytime after reading and executing from file, the program should exit
 *
 *
 * ******************************************************************************
 */

/*================================================================================
 *void method: @firstFit accepts two arguments @name and @size and search for the
 * 				first empty slots occurrence memory block and assign the process
 * 				in that block of memory by asserting if the empty slots is greater
 * 				than or equal to the size the to be assigned process.
 *
 * =================================================================================
 */
void firstFit(char name, int size)
{
	int position = 0, last_position = 0;
	int index = 0;

	for (int i = 0; i < MEMSIZE; i++)
	{
		int count = 0; // to keep track how many empty slots are there
		int allocated_size = size; // this captures the allocated size of memory block

		if (memory[i] == '.')
		{
			position = i; // position set be the  i, where the first '.', i.e empty slot is found
			index = i;   // index set to be i, where the first '.', i.e empty slots is found
			while (memory[index] == '.' && allocated_size > 0)
			{
				count++;
				index++;
				allocated_size--;
			}
			last_position = index; // last_position set be the index where the last '.', i.e empty slot is found
			// check if the empty slots are greater than or equal to the allocation size
			if (count >= size)
			{

				for (; position < last_position; position++)
				{
					memory[position] = name;
				}
				break;
			}
			else
			{
				i += count;  // update i, to start where it left before plus the empty slots
			}
		}
		// if the empty slots are greater than the Max size of array, break
		if (count > MEMSIZE)
		{
			break;
		}
	}
}
/*===================================================================================
 *void method: @bestFit accepts two arguments @name i.e name of to be assigned process
 * 				and @size i.e block of memory size, and search the entire list to find
 * 				the best fit and allocate the smallest hole that is big enough
 *
 * ===================================================================================
 */
void bestFit(char name, int size)
{
	int min = MEMSIZE; // set min to be the MEMSIZE
	int index = 0;
	int i = 0, j = 0;
	for (; i < MEMSIZE; i++)
	{
		int count = 0; // this is hold how many holes are there
		if (memory[i] == '.')
		{
			j = i;
			while (memory[j] == '.' && j < MEMSIZE)
			{
				count++;
				j++;
			}
		}
		// check and swap the min value
		if (count < min && count >= size)
		{
			min = count;
			index = i;
		}
		i += count; // update i to the next free index
	}
	// assign the process where the best fit is
	for (int k = index; k < index + size; k++)
	{
		memory[k] = name;
	}
}

/*================================================================================
 *void method: @worstFit accepts two arguments @name and @size, Allocate the largest
 * 				hole by search the entire list.
 *
 * =================================================================================
 */
void worstFit(char name, int size)
{

	int max = size;  // max set to be the size, i.e the smallest value
	int index = 0;
	int i = 0, j = 0;  // this are to help track the position
	for (; i < MEMSIZE; i++)
	{
		int count = 0; // to keep track how many holes are there
		if (memory[i] == '.')
		{
			j = i;  // j set to be i, because in meantime j is used to check if the empty holes is less than MEMSIZE
			while (memory[j] == '.' && j < MEMSIZE)
			{
				count++;
				j++;
			}
		}
		//check if the empty holes are > max and less than of the MEMSIZE
		if (count > max && count < MEMSIZE)
		{
			max = count;
			index = i;
		}
		i += count;  // update i to the next empty holes
	}
	//finally allocate the block memory in the largest holes
	for (int k = index; k < index + size; k++)
	{
		memory[k] = name;
	}
}

/*=================================================================================
 *void method: @show accepts no argument and print the current allocated memory
 *
 * =================================================================================
 */
void show()
{

	for (int i = 0; i < MEMSIZE; i++)
	{
		printf("%c", memory[i]);
	}
	printf("\n");
}
/*===================================================================================
 *int method: @freeMemory accepts one argument @name i.e the process to be freed
 * 				it goes through the entire list and free the block, then returns the
 * 				freed size od the block of memory
 *
 * ==================================================================================
 */
int freeMemory(char name)
{
    int count = 0;
	for (int i = 0; i < MEMSIZE; i++)
	{
		if (memory[i] == name)
		{   count++;
			memory[i] = '.';
		}
	}
	return count;
}

/*================================================================================
 *void method: @compact, with no argument Compact the memory pool, sliding all
 * 				allocations to lower addresses so they become one contiguous block,
 * 				and so that all the free space lies to the right as one contiguous block
 *
 * =================================================================================
 */
void compact()
{
	int i = 0;
	while (i < MEMSIZE)
	{
		while (memory[i] != '.' && memory[i - 1] == '.')
		{
			char temp = memory[i];
			memory[i] = memory[i - 1];
			memory[i - 1] = temp;
			i--;
		}
		i++;
	}
}

/*================================================================================
 *void method: @allocateMemory accepts three argument @name, @size, and @algorithm
 * 				Allocate <@size> bytes for process <@name> using algorithm <@algorithm>.
 * 				<@algorithm> can be any of F for First-Fit, B for Best-Fit or W for Worst-Fit.
 *
 * =================================================================================
 */
void allocateMemory(char name, int size, char algorithm)
{
	switch (algorithm)
	{
	case 'F':

		firstFit(name, size);

		break;
	case 'B':
		bestFit(name, size);
		break;
	case 'W':
		worstFit(name, size);
		break;
	default:
		printf("Error! Invalid Algorithm \n");
	}
}
/*=================================================================================
 *void method: @readScript accepts one argument @name i.e file name. Using the given
 * 				file name open file, read and parse the command and execute them
 *
 * =================================================================================
 */
void readScript(char *name)
{
	FILE *input_file;   // file pointer
	char command[SIZE];  //holds the list of commands, from file
	char *argument[SIZE]; //pointer to hold the token
	input_file = fopen(name, "r"); // file opening

	if (input_file == NULL)
	{
		printf("Error: couldn't open  file");
		return;
	}
	while (fgets(command, SIZE, input_file) != NULL)
	{
		command[strlen(command) - 2] = '\0'; // resetting the last index - 2 to NULL, so when parsing it don't contain
		// the new line and space

		int index = 0;
		argument[index] = strtok(command, " ");  //tokenizing by " " delimiter
		while (argument[index] != NULL)
		{
			index++;
			argument[index] = strtok(NULL, " ");
		}

		// if the index is 4, i.e the we have A B 5 F form, we have four entries
		if (index == 4)
		{

			if (strcmp((argument[0]), "A") == 0)
			{

				allocateMemory(*argument[1], atoi(argument[2]), *argument[3]);
			}

		}
			// if the index is 2, i.e the we have F X form, we have two entries
		else if(index == 2)
		{
			if (strcmp((argument[0]), "F") == 0)
			{

				freeMemory(*argument[1]);
			}
		}
			// if the index is 1, i.e the we have S | C form, we have one entry
		else if (index == 1)
		{
			if (strcmp((argument[0]), "C") == 0)
			{

				compact();
			}

			if (strcmp((argument[0]), "S") == 0)
			{

				show();
			}

		}
		else{
			printf("Error, Reading Files\n");
			exit(0);
		}


	}
	fclose(input_file); // file close
}

int main()
{
	char input_command[SIZE];  // holds input from command line
	char *token_command[SIZE]; // holds the inputs after tokenized
	int should_run = 1; // should_run, until the memory is not full keep running
	int isFull = 0; // keep track of the allocated memory
	//initialized the memory to '.', i.e empty or holes
	for (int i = 0; i < MEMSIZE; i++)
	{
		memory[i] = '.';
	}
	while (should_run)
	{

		fgets(input_command, SIZE, stdin); // read from command line
		input_command[strlen(input_command) - 1] = '\0'; // reset the last index to NULL

		int position = 0;
		token_command[position] = strtok(input_command, " "); // split the inputs by delimiter, " "
		while (token_command[position] != NULL)
		{
			position++;
			token_command[position] = strtok(NULL, " ");
		}
		// if the first input is A, which is allocate memory , calls allocate
		if (strcmp((token_command[0]), "A") == 0)
		{
			allocateMemory(*token_command[1], atoi(token_command[2]), *token_command[3]);
			isFull = isFull + atoi(token_command[2]);
		}
			// if the first is F, i.e free memory, calls the freeMemory method with argument
		else if (strcmp((token_command[0]), "F") == 0)
		{

			int free = freeMemory(*token_command[1]);
			isFull = isFull - free; // deduct the freed space from the allocated space
		}
			// if the first input is S, i.e show, calls show() method
		else if (strcmp((token_command[0]), "S") == 0)
		{

			show();
		}
			//if the first inputs is R, i.e read from file and execute, calls readscript method with file name
		else if (strcmp((token_command[0]), "R") == 0)
		{

			readScript(token_command[1]);
			exit(0);
		}
			//if the first input is C, which is Compact, calls the compact method
		else if (strcmp((token_command[0]), "C") == 0)
		{

			compact();
		}
		else
		{
			printf("Error! Reading from Command line \n");
			exit(0);
		}
		// this check if the memory if full or not
		if(isFull > MEMSIZE){
			printf("MEMORY IS FULL\n");
			should_run = 0;
		}


	}
	return 0;
}