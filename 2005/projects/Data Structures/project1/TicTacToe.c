/*
 * This is the main code for the TicTacToe game that links everything together
 * A user is asked if he/she wishes to play a game
 * then we ask if they want to go first
 * a game of tic-tac-toe then follows
 * computer moves are computed in the file ComputerMoves.c
 * written by Abrie Greeff
*/
#include <stdlib.h>
#include <stdio.h>
#include "ComputerMoves.h"

infoPtr createInitialGameState()
/* The first procedure to call, this initialises the structure with all the
 * information for the playing area and the game
 * Pre-condition: None
 * Post-condition: all the game info has been setup
*/
{
	int i,j = 0;
	//create a pointer for the structure
	infoPtr gameInfo = malloc(sizeof(struct info));

	//make the play area empty
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			gameInfo->playArea[i][j] = EMPTY;
	//set the score to 0-0
	gameInfo->score[0] = 0;
	gameInfo->score[1] = 0;

	//it hasn't been decided who's playing first, this also tells us we're not playing a game
	gameInfo->turn = EMPTY;
	//return the pointer to the structure
	return gameInfo;
}

void resetPlayArea(infoPtr gameInfo)
/* This resets the gameInfo structure to the initial game state
 * Pre-condition: None
 * Post-condition: all the game info has been reset
*/
{
	int i,j = 0;

	//make the play area empty
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			gameInfo->playArea[i][j] = EMPTY;
}

void displayGameArea(infoPtr gameInfo)
/* This procedure draws the playing area on the screen
 * Pre-condition: gameInfo exists
 * Post-condition: the playing area has been drawn on the screen
*/
{
	//make sure we're drawing at the top of the screen
	system("clear");
	//does the game info structure exist?
	if (gameInfo != NULL)
	{
		printf("\n");
		int i,j;

		//this loops through the rows of the playing area
		for (i = 0; i < 3; i++)
		{
			//this loops through the columns of the playing area
			for (j = 0; j < 3; j++)
			{
				//check what is the state of the square currently being referenced
				switch(gameInfo->playArea[i][j])
				{
					//does the user own it, if he does draw a X
					case USER:
						printf("X");
						break;
					//does the computer own it, if he does draw a O
					case COMPUTER:
						printf("O");
						break;
					//does no one own it, then leave it empty
					case EMPTY:
						printf(" ");
						break;
				}
				//this draws a gridline between col. 1 and 2, and between col.2 and 3
				if (j < 2)
				{
					printf("|");
				}
			}
			//this draws a gridline between row. 1 and 2, and between row.2 and 3
			if (i < 2)
			{
				printf("\n-----\n");
			}else
			{
				printf("\n");
			}
		}
	}
}

void startAGame(infoPtr gameInfo)
/* This procedure finds out if the user wishes to play a game
 * It also asks if he/she wishes to play first
 * Pre-condition: gameInfo exists
 * Post-condition: we are playing a game and user/computer starts
 		   or we are not playing a game
*/
{
	//does gameInfo exist
	if (gameInfo == NULL)
	{
		//if it doesn't then create it
		gameInfo = createInitialGameState();
	}

	//ask if we want to start a new game?
	printf("\nDo you want to play a game? (y/n):");

	//get the input
	char input;
	input =  getchar();

	if (input == 10)
	{
		//newline received after enter was hit
		input =  getchar();
	}

	//the user didn't press y or n
	while ((input != 121) & (input != 110))
	{
		printf("\nPlease enter y for yes or n for no\n");
		//ask if we want to start a new game?
		printf("Do you want to play a game? (y/n):");

		//get the input
		input =  getchar();

		if (input == 10)
		{
			//newline received after enter was hit
			input =  getchar();
		}
	}

	//the user wants to play a game
	if (input == 121)
	{
		//ask if the user wants to play first
		printf("Do you want to play first? (y/n):");

		//get the answer
		input =  getchar();

		if (input == 10)
		{
			//newline received after enter was hit
			input =  getchar();
		}

		//the user didn't press y or n
		while ((input != 121) & (input != 110))
		{
			printf("\nPlease enter y for yes or n for no\n");
			//ask if we want to start a new game?
			printf("Do you want to play first? (y/n):");

			//get the input
			input =  getchar();

			if (input == 10)
			{
				//newline received after enter was hit
				input =  getchar();
			}
		}

		//who's going first?
		if (input == 121)
		{
			//user goes first
			gameInfo->turn = USER;
			//put the empty playing area on the screen
			displayGameArea(gameInfo);
		}else
		{
			//computer goes first
			printf("\nPlacing first move for computer");
			gameInfo->playArea[1][1] = COMPUTER;
			gameInfo->turn = USER;
			displayGameArea(gameInfo);
		}
	}else
	{
		//the user doesn't want to play
		gameInfo->turn = EMPTY;
	}
}

void getUserMove(infoPtr gameInfo)
/* This procedure get's the user's next move and applies it
 * Pre-condition: gameInfo exists
 * Post-condition: the user's move has been added to game info
*/
{
	if (gameInfo != NULL)
	{
		char input;
		int row,col;

		//ask in which row does the user want to play
		printf("\nMake your move!\nWhat row? (1-3):");

		//get the input
		input = getchar();

		if (input == 10)
		{
			//newline received after enter was hit
			input =  getchar();
		}

		//was the row a valid value (1-3)
		while ((input < 49) || (input > 51))
		{
			//it was not, tell the user
			printf("You must enter a value between 1 and 3\nWhat row? (1-3):");
			//get the input
			input = getchar();

			if (input == 10)
			{
				//newline received after enter was hit
				input =  getchar();
			}
		}
		//turn the ascii value into an int value
		row = input - 48;

		//ask which column the user want to play
		printf("What Column? (1-3):");

		//get the input
		input = getchar();

		if (input == 10)
		{
			//newline received after enter was hit
			input =  getchar();
		}

		//was the column a valid value (1-3)
		while ((input < 49) || (input > 51))
		{
			//it was not, tell the user
			printf("You must enter a value between 1 and 3\nWhat column? (1-3):");
			//get the input
			input = getchar();

			if (input == 10)
			{
				//newline received after enter was hit
				input =  getchar();
			}
		}
		//turn the ascii value into an int value
		col = input - 48;

		//now we check if the square the user wants to play on is available
		if (gameInfo->playArea[row-1][col-1] == EMPTY)
		{
			//it is available
			gameInfo->playArea[row-1][col-1] = USER;
		}else
		{
			printf("\nSorry this square has already been taken!\nTry again");
			//get the move again
			getUserMove(gameInfo);
		}
	}
}

void getMatchState(infoPtr gameInfo)
/* This procedure checks if we have a winner, or a tie
 * or just need to change turns
 * Pre-condition: gameInfo exists
 * Post-condition: we have a winner, a tie, or a change of turn
*/
{
	if (gameInfo != NULL)
	{
		int i;
		//check if there is a winner
		//one of the rows?
		for (i = 0; i < 3; i++)
		{
			if ((gameInfo->playArea[i][0] == gameInfo->playArea[i][1]) &
				(gameInfo->playArea[i][1] == gameInfo->playArea[i][2]))
			{
				//who has this row?
				switch (gameInfo->playArea[i][1])
				{
					case USER:
						//the user has won
						printf("\nCongratulations!!! You have won");
						//increment the score for the user
						++(gameInfo->score[0]);
						//display the score
						printf("\nThe score is %d - %d", gameInfo->score[0],gameInfo->score[1]);
						//end this game
						gameInfo->turn = EMPTY;
						break;
					case COMPUTER:
						//the computer has won
						printf("\nI'm sorry but you have lost");
						//increment the score for the computer
						++(gameInfo->score[1]);
						//display the score
						printf("\nThe score is %d - %d", gameInfo->score[0],gameInfo->score[1]);
						//end this game
						gameInfo->turn = EMPTY;
						break;
					case EMPTY:
						//just an empty row, no winner
						break;
				}
			}
		}



		//has there already been a winning row?
		if (gameInfo->turn != EMPTY)
		{
			//one of the columns?
			for (i = 0; i < 3; i++)
			{
				if ((gameInfo->playArea[0][i] == gameInfo->playArea[1][i]) &
					(gameInfo->playArea[1][i] == gameInfo->playArea[2][i]))
				{
					//who has this column?
					switch (gameInfo->playArea[1][i])
					{
						case USER:
							//the user has won
							printf("\nCongratulations!!! You have won");
							//increment the score for the user
							++(gameInfo->score[0]);
							//display the score
							printf("\nThe score is %d - %d",
								gameInfo->score[0],gameInfo->score[1]);
							//end this game
							gameInfo->turn = EMPTY;
							break;
						case COMPUTER:
							//the computer has won
							printf("\nI'm sorry but you have lost");
							//increment the score for the computer
							++(gameInfo->score[1]);
							//display the score
							printf("\nThe score is %d - %d",
								gameInfo->score[0],gameInfo->score[1]);
							//end this game
							gameInfo->turn = EMPTY;
							break;
						case EMPTY:
							//just an empty column, no winner
							break;
					}
				}
			}
		}



		//has there been a winning row or column?
		if (gameInfo->turn != EMPTY)
		{
			//one of the diagonals?
			for (i = 0; i < 2; i++)
			{
				if ((gameInfo->playArea[0][0+(2*i)] == gameInfo->playArea[1][1]) &
					(gameInfo->playArea[1][1] == gameInfo->playArea[2][2-(2*i)]))
				{
					//who has this diagonal?
					switch (gameInfo->playArea[1][1])
					{
						case USER:
							//the user has won
							printf("\nCongratulations!!! You have won");
							//increment the score for the user
							++(gameInfo->score[0]);
							//display the score
							printf("\nThe score is %d - %d",
								gameInfo->score[0],gameInfo->score[1]);
							//end this game
							gameInfo->turn = EMPTY;
							break;
						case COMPUTER:
							//the computer has won
							printf("\nI'm sorry but you have lost");
							//increment the score for the computer
							++(gameInfo->score[1]);
							//display the score
							printf("\nThe score is %d - %d",
								gameInfo->score[0],gameInfo->score[1]);
							//end this game
							gameInfo->turn = EMPTY;
							break;
						case EMPTY:
							//just an empty diagonal, no winner
							break;
					}
				}
			}
		}

		//is there a tie?

		//this just means there is no empty squares and we don't have a winner
		if (gameInfo->turn != EMPTY)
		{
			if (squaresAvailable(gameInfo) == 0)
			{
				printf("\nThis game is tie. Nice try");
				printf("\nScore remains %d - %d", gameInfo->score[0], gameInfo->score[1]);
				gameInfo->turn = EMPTY;
			}
		}
		//change turn
		//there wasn't a tie or a winner
		if (gameInfo->turn != EMPTY)
		{
			// did the user play last?
			if (gameInfo->turn == USER)
			{
				//it's the computer's turn next
				gameInfo->turn = COMPUTER;
			}else
			{
				//it's the user's turn next
				gameInfo->turn = USER;
			}
		}
	}
}


void shutDown(infoPtr gameInfo)
/* The last procedure that's called, which frees all the memory the program used
 * Pre-condition: gameInfo exists
 * Post-condition: all pointers have been freed
*/
{
	//does the pointer exist?
	if (gameInfo != NULL)
	{
		//it does, then free it
		free(gameInfo);
	}
}

int main()
/* This is the main procedure of the tictactoe program
 * everything happens here.
 * Pre-condition: none
 * Post-condition: there was no problems
*/
{
	//program starts

	//create the intial game state structure
	infoPtr gameInfo = createInitialGameState();

	//find out if the user wishes to play and who must go first
	startAGame(gameInfo);

	//this is the main loop of the program
	//gameInfo->turn can only be EMPTY when the user doesn't want to play anymore
	while (gameInfo->turn != EMPTY)
	{
		//it's the user's turn to make a move
		if (gameInfo->turn == USER)
		{
			getUserMove(gameInfo);
		}else
		//it's the computer's turn to make a move
		{
			printf("Computing computer move...\n");
			getComputerMove(gameInfo);
		}

		//show how the game area now looks
		displayGameArea(gameInfo);

		//do we have a winner?, do we have a tie?, change turn?
		getMatchState(gameInfo);

		//if there was a tie or a win we need to ask if the user want's to play again
		if (gameInfo->turn == EMPTY)
		{
			//reset the play area
			resetPlayArea(gameInfo);
			//ask if we're playing
			startAGame(gameInfo);
		}
	}
	//shutdown everything
	shutDown(gameInfo);

	//program ends
}
