// Matthew Somers - Assignment 10 - CS49c
// Sudoku Game

#include <stdio.h>
#include <string.h>

int solver(int *board[36][36], int solutions);
int checkinvalid(int *copy[36][36], int row, int col);
void printboard(int *board[36][36]);

int *data[36][36];
int *temp2[36][36];

int boardrow = 9, boardcol = 9, nonetsize = 3; // defining default print size
int row, col; // for counting loops

int main() 
{
	char command[80];
	int i, commaposition1, commaposition2, invalidnum;
	int number1, number2, number3, setrow, setcol, temp;
	
	//fill board
	for (row = 0; row < 36; row++)
		for (col = 0; col < 36; col++)
			data[row][col] = 0;

	printf("\n\nWELCOME TO a Assignment 10: The Sudoku Solver.\n");
	printboard(data);

	while(1) 
	{
		printf("\nEnter your command: ");
		fgets(command, sizeof(command), stdin);
		command[strlen(command)-1] = '\0';
		invalidnum = 1;
		number1 = 0;
		number2 = 0;
		number3 = 0;
		setrow = 0;
		setcol = 0;


		// check if 2 commas
		for (commaposition1 = 0; commaposition1 < strlen(command); commaposition1++) 
		{
			if ( command[commaposition1] == ',' ) 
			{
				for (commaposition2 = commaposition1+1; commaposition2 < strlen(command); commaposition2++) 
				{
					if ( command[commaposition2] == ',' ) 
					{
						invalidnum = 0;
						break;
					}
				}
				break;
			}
		}
		
		//convert to lower
		for (i = 0; i < strlen(command); i++)
			command[i] = tolower( command[i] );
		
		//check if row or column modifier		
		if (command[0] == 'c' && command[1] == 'o' && command[2] == 'l' && command[3] == 'u' && command[4] == 'm' && command[5] == 'n' && command[6] == 's' )
			setcol = 1;
			
		if (command[0] == 'r'  && command[1] == 'o' && command[2] == 'w' && command[3] == 's' )
				setrow = 1;

		
		//------------------- IF BLOCKS
		
		if (strcmp("quit", command) == 0) 
		{
			printf("Thank you for using this silly program.\n\n");
			break;
		}


		else if (strcmp("help", command) == 0) 
		{
			printf("\nThis is the help menu. Available commands:\n\n");
			printf("Entering a number, then a comma, then another number, then another comma, then \n\ta final number - marks that square with the final number.\n");
			printf("reset - resets the board\n");
			printf("rows then a space, then a number - sets the number of rows to that.\n");
			printf("columns then a space, then a number - sets the number of columns to that.\n");
			printf("solve - solves the entered sudoku problem.\n");
			printf("unsolve - unsolves the recently solved sudoku problem.\n");
			printf("quit - exits the program.\n");
		}
		
		else if (strcmp("reset", command) == 0) 
		{
			printf("Board has been reset\n");
			boardrow = 9;
			boardcol = 9;
			nonetsize = 3;
			
			for (row = 0; row < 36; row++)
				for (col = 0; col < 36; col++)
					data[row][col] = 0;
					
			printboard(data);
		}
		
		else if (strcmp("solve", command) == 0) 
		{
			int numberofsolutions = solver(data, 0);

			switch(numberofsolutions) 
			{
				case -1:
					printf("\nBroken board.\n");
					break;
				case 0:
					printf("\nNo solution.\n");
					break;
				case 1:
					printf("\n%d solution.\n", numberofsolutions);
					break;
				default:
					printf("\n%d solutions.\n", numberofsolutions);
				
			}
		}
		
		else if (strcmp("unsolve", command) == 0) 
		{
			for (row = 0; row < 36; row++)
				for (col = 0; col < 36; col++)
					data[row][col] = temp2[row][col];
			
			printboard(data);
		}
		
		else if (setrow == 1)
		{
			temp = boardrow;
			boardrow = 0;
			// start at two after the end of "rows"
			for (i = 5; i < strlen(command); i++)
				boardrow = 10*boardrow + (command[i] - '0');
			
			switch(boardrow)
			{
				case 4: nonetsize = 2; break;
				case 9: nonetsize = 3; break;
				case 16: nonetsize = 4; break;
				case 25: nonetsize = 5; break;
				case 36: nonetsize = 6; break;
				default:
					printf("Rows/columns number must be the square of an integer.\n");
					boardrow = temp;
			}
			
			boardcol = boardrow;
			printboard(data);
		}
		
		else if (setcol == 1)
		{
			temp = boardcol;
			boardcol = 0;
			// start at two after the end of "columns"
			for (i = 8; i < strlen(command); i++)
				boardcol = 10*boardcol + (command[i] - '0');

			switch(boardcol)
			{
				case 4: nonetsize = 2; break;
				case 9: nonetsize = 3; break;
				case 16: nonetsize = 4; break;
				case 25: nonetsize = 5; break;
				case 36: nonetsize = 6; break;
				default:
					printf("Rows/columns number must be the square of an integer.\n");
					boardcol = temp;
			}
			
			boardrow = boardcol;
			printboard(data);
		}
		
		// if it has 2 commas, assume valid
		else if (invalidnum == 0) 
		{
			// cut apart comma separation
			for ( i = 0; i < commaposition1; i++)
				number1 = 10*number1 + (command[i] - '0');

			for ( i = commaposition1+1; i < commaposition2; i++)
				number2 = 10*number2 + (command[i] - '0');
			
			for ( i = commaposition2+1; i < strlen(command); i++) 
				number3 = 10*number3 + (command[i] - '0');
			
			
			if ( number1 <= 0 || number2 <= 0 || number3 < 0 || number1 > 36 || number2 > 36 )
				printf("ERROR, the board is only a max of 36x36.\n");
			
			else
			{
				//mark it (or remove mark if 0)
				if (number1 > boardrow || number2 > boardcol || number3 > boardcol)
					printf("\nOff the board. Input not accepted.\n");
				else
					(data[number1-1][number2-1]) = number3; //user not counting from 0
			}
			
			printboard(data);
		}

		else
		{
			printf("Invalid input.\n");
			printboard(data);
		}
	}

	return 0;

}

int solver(int *board[36][36], int solutions)
{
	int *copy[36][36];
	int invalidflag, space, status;
	
	//fill a copy board
	for (row = 0; row < 36; row++)
		for (col = 0; col < 36; col++)
			copy[row][col] = board[row][col];

			
	//fill in a new place
	for (row = 0; row < boardrow; row++)
	{
		for (col = 0; col < boardcol; col++)
		{
			//check if default board is broken
			if (copy[row][col] != 0)
			{
				if (checkinvalid(copy, row, col) == 1)
					return -1;
			}
				
			//found EMPTY space
			else
			{
				//fill in the empty space
				for (space = 1; space <= boardrow; space++)
				{
					//put in new value
					copy[row][col] = space;
					
					//check if duplicates
					invalidflag = checkinvalid(copy, row, col);


					//if it has passed, go to next level
					if (invalidflag == 0)
					{
						status = solver(copy, 0); //RECURSION
						
						if ( status > 0) 
						{
							return status; //exit strategy
						}
							
						else if ( status == -1 )
							return -1; //already-broken matrix, EJECT!
					}
				}
				
				//didn't pass
				return 0; //broken matrix, WE HAVE TO GO BACK
			}
		}
	}
	
	//no more empty spaces, only a complete board gets here
	solutions++;
	
	if (solutions == 1)
	{
		//save old board:
		for (row = 0; row < 36; row++)
		{
			for (col = 0; col < 36; col++)
			{
				temp2[row][col] = data[row][col];
				data[row][col] = copy[row][col];
			}
		}
		
		//print first solution
		printboard(data);
	}
	
	return solutions;
}

int checkinvalid(int *copy[36][36], int row, int col)
{
	//function for determining whether a value should go into a position
	
	int i, j, k;
	int nonetrow, nonetcol;
	
	for (i = 0; i < boardrow; i++)
	{
		//check row for invalid
		if ((i == row)); //do nothing
		else if (copy[row][col] == copy[i][col])
			return 1;
		
		//check columns for invalid
		if ((i == col)); //do nothing
		else if (copy[row][col] == copy[row][i])
			return 1;
	}

	//check nonets for invalid
	nonetrow = row / nonetsize;
	nonetcol = col / nonetsize;

	for(j = nonetrow*nonetsize; j < (nonetrow+1)*nonetsize; j++)
	{
		for(k = nonetcol*nonetsize; k < (nonetcol+1)*nonetsize; k++)
		{
			if (row == j && col == k); //do nothing

			else if (copy[row][col] == copy[j][k])
				return 1;
		}
	}
	
	//passed through the gauntlet and lived, it is a valid mark
	return 0;
}

void printboard(int *board[36][36])
{
	//loops in this method are all 1 ahead for usability's sake
	//this is corrected when accessing array data
	
	for (row = 0; row <= boardrow; row++)
	{
		//print numbers on top
		if (row == 0)
		{
			printf("\n");
			
			for (col = 1; col <= boardcol; col++)
			{
				if (col == 1)
					printf("    %d", col);
				else if (col > 9)
					printf("  %d", col);
				else
					printf("   %d", col);
			}
		}

		else
		{
			printf("\n  ");
			
			for (col = 1; col <= boardcol; col++)
			{
				if ( (((row-1) % (nonetsize)) == 0) && ((row-1) != 0))
					printf("+===");
				else
					printf("+---");
			}
			
			printf("+");
			
			if (row > 9)
				printf("\n%d", row);
			else
				printf("\n%d ", row);
			
			for (col = 1; col <= boardcol; col++)
			{
				if ( (((col-1) % (nonetsize)) == 0) && ((col-1) != 0))
					printf("#");
				else
					printf("|");
				//actually print data into square
				if (board[row-1][col-1] != 0)
				{
					if ((board[row-1][col-1]) > 9)
						printf(" %d", board[row-1][col-1]);
					else
						printf(" %d ", board[row-1][col-1]);
				}
				else
					printf("   ");
			}
			
			printf("|");
			
			if (row == boardrow)
			{
				printf("\n  ");
				
				for (col = 1; col <= boardcol; col++)
				{
					printf("+---");
				}
				
				printf("+\n");
			}

		}
		
	}
}
