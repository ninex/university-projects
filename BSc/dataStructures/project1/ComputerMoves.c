/* this is the code where the computer's next move is computed
 * a tree is generated with all the moves possible for a maximum of 3 turns
 * the best move is then selected from this 3 levels and played
 * written by Abrie Greeff
*/

#include <stdlib.h>
#include <stdio.h>
#include "BinaryTree.h"
#include "ComputerMoves.h"

void copyGameArea(infoPtr source, infoPtr dest)
/* This procedure copies the game area from source to dest
 * Pre-condition: none
 * Post-condition: source's game area has been copied to dest's game area
*/
{
	int i,j;
	//the rows
	for (i = 0; i < 3; i++)
			//the columns
			for (j = 0; j < 3; j++)
				//copy square
				dest->playArea[i][j] = source->playArea[i][j];
}

int squaresAvailable(infoPtr gameInfo)
/* This function checks if there's squares available in the gameArea of gameInfo
 * Pre-condition: gameInfo exists
 * Post-condition: number of squares available is returned
*/
{
	int emptySquares = 0;
	//does gameInfo exist?
	if (gameInfo != NULL)
	{
		int i,j;
		//the rows
		for (i = 0; i < 3; i++)
			//the columns
			for (j = 0; j < 3; j++)
			{
				//is this square empty
				if (gameInfo->playArea[i][j] == EMPTY)
				{
					//it is, so increment the number available squares
					++emptySquares;
				}
			}
		//return the number of available squares
		return emptySquares;
	}else
	{
		//it doesn't exist
		return -1;
	}
}

void deleteElement(infoPtr gameInfo)
/* This procedure removes the element from memory
 * Pre-condition: info exists
 * Post-condition: info has been freed
*/
{
	//does gameInfo exist?
	if (gameInfo != NULL)
	{
		//it does, so free it from memory
		free(gameInfo);
	}
}

void dspElement(infoPtr gameInfo)
/* this procedure displays the element
 * Pre-condition: info exists
 * Post-condition: the element has been displayed on the screen
*/
{
	if (gameInfo != NULL)
	{
		//make a new line
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

int cmpElement(infoPtr info1, infoPtr info2)
/* This procedure compares 2 elements, because they can't be bigger
 * or greater than each other. They can only be equal
 * Pre-condition: info1 and info2 exists
 * Post-condition: 0 is returned if they are equal, else 1 is returned
*/
{
	//do they exist?
	if ((info1 != NULL) && (info2 != NULL))
	{
		int i,j,equal;
		//initially they are equal
		equal = 0;

		//rows
		for (i = 0; i < 3; i++)
			//columns
			for (j = 0; j < 3; j++)
				//are they equal?
				if (info1->playArea[i][j] != info2->playArea[i][j])
				{
					//not equal
					equal = 1;
				}
		//result?
		if (equal == 0)
		{
			//they were equal
			return 0;
		}else
		{
			//they were inequal
			return 1;
		}

	}
}

int evaluateSquares(infoPtr gameInfo)
/* This procedure takes the gameArea of gameInfo and evaluates the gameArea through
 * the formula of: possible squares for a win for computer - possible squares for a win for the user
 * if the user has a winning move -9*winning moves is returned
 * if the computer has a winning move return 9*winning moves
 * Pre-condition: none
 * Post-condition: a value has been returned
*/
{
	int count1 = 0;
	int count2 = 0;
	int winCount1 = 0;
	int winCount2 = 0;
	int i;

	//check rows
	for (i = 0; i < 3; i++)
	{
		//XX' '
		if ( (gameInfo->playArea[i][0] == gameInfo->playArea[i][1]) &&
			(gameInfo->playArea[i][2] == EMPTY) )
		{
			if (gameInfo->playArea[i][0] == USER)
				count1++;
			if (gameInfo->playArea[i][0] == COMPUTER)
				count2++;
		}else
		{
			//' 'XX
			if ( (gameInfo->playArea[i][1] == gameInfo->playArea[i][2]) &&
				(gameInfo->playArea[i][0] == EMPTY) )
			{
				if (gameInfo->playArea[i][1] == USER)
					count1++;
				if (gameInfo->playArea[i][1] == COMPUTER)
					count2++;
			}else
			{
				//X' 'X
				if ( (gameInfo->playArea[i][0] == gameInfo->playArea[i][2]) &&
					(gameInfo->playArea[i][1] == EMPTY) )
				{
					if (gameInfo->playArea[i][0] == USER)
						count1++;
					if (gameInfo->playArea[i][0] == COMPUTER)
						count2++;
				}
			}
		}
		//XXX wining row
		if ( (gameInfo->playArea[i][0] == gameInfo->playArea[i][1]) &&
			(gameInfo->playArea[i][2] == gameInfo->playArea[i][1]) )
		{
			if (gameInfo->playArea[i][0] == COMPUTER)
				winCount2++;
			if (gameInfo->playArea[i][0] == USER)
				winCount1++;
		}
	}

	//check columns
	for (i = 0; i < 3; i++)
	{
		//X
		//X
		//
		if ( (gameInfo->playArea[0][i] == gameInfo->playArea[1][i]) &&
		     (gameInfo->playArea[2][i] == EMPTY) )
		{
			if (gameInfo->playArea[0][i] == USER)
				count1++;
			if (gameInfo->playArea[0][i] == COMPUTER)
				count2++;
		}else
		{
			//
			//X
			//X
			if ( (gameInfo->playArea[1][i] == gameInfo->playArea[2][i]) &&
			     (gameInfo->playArea[0][i] == EMPTY) )
			{
				if (gameInfo->playArea[1][i] == USER)
					count1++;
				if (gameInfo->playArea[1][i] == COMPUTER)
					count2++;
			}else
			{
				//X
				//
				//X
				if ( (gameInfo->playArea[0][i] == gameInfo->playArea[2][i]) &&
				     (gameInfo->playArea[1][i] == EMPTY) )
				{
					if (gameInfo->playArea[0][i] == USER)
						count1++;
					if (gameInfo->playArea[0][i] == COMPUTER)
						count2++;
				}
			}
		}
		//X
		//X
		//X winning column
		if ( (gameInfo->playArea[0][i] == gameInfo->playArea[1][i]) &&
		     (gameInfo->playArea[2][i] == gameInfo->playArea[1][i]) )
		{
			if (gameInfo->playArea[0][i] == COMPUTER)
				winCount2++;
			if (gameInfo->playArea[0][i] == USER)
				winCount1++;
		}
	}
	//check diagonals
	for (i = 0; i < 2; i++)
	{
		//X		X
		// X	       X
		//  ' '	     ' '
		if ( (gameInfo->playArea[0][(2*i)] == gameInfo->playArea[1][1]) &&
		     (gameInfo->playArea[2][2-(2*i)] == EMPTY) )
		{
			if (gameInfo->playArea[1][1] == USER)
				count1++;
			if (gameInfo->playArea[1][1] == COMPUTER)
				count2++;
		}else
		{
			//' '    ' '
			//  X   X
			//   X X
			if ( (gameInfo->playArea[1][1] == gameInfo->playArea[2][2-(2*i)]) &&
			     (gameInfo->playArea[0][(2*i)] == EMPTY) )
			{
				if (gameInfo->playArea[1][1] == USER)
					count1++;
				if (gameInfo->playArea[1][1] == COMPUTER)
					count2++;
			}else
			{
				//X          X
				// ' '    ' '
				//    X  X
				if ( (gameInfo->playArea[0][(2*i)] == gameInfo->playArea[2][2-(2*i)]) &&
				     (gameInfo->playArea[1][1] == EMPTY) )
				{
					if (gameInfo->playArea[0][(2*i)] == USER)
						count1++;
					if (gameInfo->playArea[0][(2*i)] == COMPUTER)
						count2++;
				}
			}
		}

		//a winning diagonal
		if ( (gameInfo->playArea[0][(2*i)] == gameInfo->playArea[1][1]) &&
		     (gameInfo->playArea[2][2-(2*i)] == gameInfo->playArea[1][1]) )
		{
			if (gameInfo->playArea[1][1] == COMPUTER)
				winCount2++;
			if (gameInfo->playArea[1][1] == USER)
				winCount1++;
		}
	}

	//did the computer have more winning moves than the user
	if (winCount2 > winCount1)
	{
		return (9*winCount2);
	}
	//did the user have more winning moves than the computer
	if ( (winCount2 <= winCount1) && (winCount1 != 0) )
	{
		return ((-9)*winCount1);
	}
	//return eval
	return (count1 - count2);
}

void generateSquares(binaryTreePtr tree)
/* this procedure inserts children nodes into the tree for the current node in the tree
 * the number of nodes inserted will be equal to the number of empty squares that the current node has
 * each node will represent one empty square
 * Pre-condition: none
 * Post-condition: all the children has been created
*/
{
	int i,j,first;
	first = 0;
	infoPtr temp;

	//get the current node in the tree's gameinfo
	infoPtr gameInfo = retrieveNodeElm(tree);


	//goes through rows
	for (i = 0; i < 3; i++)
		//goes through columns
		for (j = 0; j < 3; j++)
		{
			//is this a empty square?
			if (gameInfo->playArea[i][j] == EMPTY)
			{
				//create memory for a new element
				temp = malloc(sizeof(struct info));
				//copy gameInfo's playArea
				copyGameArea(gameInfo, temp);
				//make sure it doesn't have a value
				temp->eval = 0;
				//who was playing for temp's parent
				if (gameInfo->turn == USER)
				{
					//the computer will play this round
					temp->playArea[i][j] = COMPUTER;
					temp->turn = COMPUTER;
				}else
				{
					//the user will play this round
					temp->playArea[i][j] = USER;
					temp->turn = USER;
				}

				//is this the first child?
				if (first == 0)
				{
					//insert the node to the left
					first = 1;
					insertNode(tree, temp, TOLEFT);
				}else
				{
					//insert the node to the right
					insertNode(tree, temp, TORIGHT);
				}
			}
		}
}

void expandTree(binaryTreePtr tree)
/* This procedure expands a unique tree for the current game state
 * Pre-condition: the tree has been allocated in memory
 * Post-condition: a tree has been created for the given game state
*/
{
	if (tree != NULL)
	{
		//get the current gamestate for the level in tree we are on
		infoPtr gameInfo = retrieveNodeElm(tree);

		//store the adress of the current node in the tree
		treeNodePtr current;
		current = getCurrentNode(tree);

		//get the root level's game state
		nextNode(tree,TOROOT);
		infoPtr rootInfo = retrieveNodeElm(tree);

		//go back to current level
		setCurrentNode(tree,current);
		//we only extend 2 levels, this counts how many we have done
		int cmpSquares = squaresAvailable(rootInfo) - squaresAvailable(gameInfo);

		//is there any sqaures available?
		if ( (cmpSquares  < 3) && (squaresAvailable(gameInfo) != 0) )
		{
			//generate the next level for the tree
			generateSquares(tree);
			//go back to the node we started with
			setCurrentNode(tree, current);
			//go to the first node on the next level
			nextNode(tree, TOLEFT);
			//set this as the current node
			current = getCurrentNode(tree);
			//call this procedure again, to expand the next level
			expandTree(tree);
			//go back to first node of level we are working with
			setCurrentNode(tree, current);

			//does it have a next node on this level?
			while (existNode(tree, TORIGHT) == 1)
			{
				//then go to the next node
				nextNode(tree, TORIGHT);
				//get node on this level we are working with
				current = getCurrentNode(tree);
				//generate a next level for this node
				expandTree(tree);
				//back to working level
				setCurrentNode(tree, current);
			}
		}
	}
}
int canWeWinNow(infoPtr gameInfo)
/* Is it possible to make a winning move right now?
 * this procedure is always called on the second level of the gamestate
 * because it checks if the next move is possibly a winning move
 * Pre-condition: none
 * Post-condition: 1 is returned if there's a winning move now, 0 otherwise
*/
{
	int i,j;
	//go through rows and columns
	for (i = 0; i < 3; i++)
	{
		//is there a winning move on a row
		if ( (gameInfo->playArea[i][0] == gameInfo->playArea[i][1]) &&
			(gameInfo->playArea[i][2] == gameInfo->playArea[i][1]) )
		{
			if (gameInfo->playArea[i][0] == COMPUTER)
				return 1;

		}
		//is there a winning move on a column
		if ( (gameInfo->playArea[0][i] == gameInfo->playArea[1][i]) &&
		     (gameInfo->playArea[2][i] == gameInfo->playArea[1][i]) )
		{
			if (gameInfo->playArea[0][i] == COMPUTER)
				return 1;
		}
		//there's only 2 diagonals
		if (i < 2)
		{
			//is there a winning move on a diagonal
			if ( (gameInfo->playArea[0][(2*i)] == gameInfo->playArea[1][1]) &&
				(gameInfo->playArea[2][2-(2*i)] == gameInfo->playArea[1][1]) )
			{
				if (gameInfo->playArea[1][1] == COMPUTER)
					return 1;
			}
		}

	}
	//no winning moves
	return 0;

}

int canWeLoseNow(infoPtr gameInfo)
/* Is it possible to have a losing move right now?
 * this procedure is always called on the second level of the gamestate
 * because it checks if the next move is possibly a losing move
 * Pre-condition: none
 * Post-condition: 1 is returned if there's a losing move now, 0 otherwise
*/
{
	int i,j;
	//go through rows and columns
	for (i = 0; i < 3; i++)
	{
		//is there a losing move on a row
		if ( (gameInfo->playArea[i][0] == gameInfo->playArea[i][1]) &&
			(gameInfo->playArea[i][2] == gameInfo->playArea[i][1]) )
		{
			if (gameInfo->playArea[i][0] == USER)
				return 1;

		}
		//is there a losing move on a column
		if ( (gameInfo->playArea[0][i] == gameInfo->playArea[1][i]) &&
		     (gameInfo->playArea[2][i] == gameInfo->playArea[1][i]) )
		{
			if (gameInfo->playArea[0][i] == USER)
				return 1;
		}
		//there's only 2 diagonals
		if (i < 2)
		{
			//is there a losing move on a diagonal
			if ( (gameInfo->playArea[0][(2*i)] == gameInfo->playArea[1][1]) &&
				(gameInfo->playArea[2][2-(2*i)] == gameInfo->playArea[1][1]) )
			{
				if (gameInfo->playArea[1][1] == USER)
					return 1;
			}
		}

	}
	//no losing moves
	return 0;
}


void lookForWinningMove(binaryTreePtr tree)
/* here we check if there's any winning moves for the user right now
 * this is because the computer would rather block a move, than go for the win
 * when there's a move eval is set to 1000 so that we know it will be the biggest value on minmax
 * the same happens on a losing move with a value off -1000
 * Pre-condition: none
 * Post-condition: we have checked for a winning move
*/
{
	//go to the root level
	nextNode(tree,TOROOT);
	//get the game area
	infoPtr root = retrieveNodeElm(tree);
	//goto the second level
	nextNode(tree,TOLEFT);
	//get the game area for this option
	infoPtr temp = retrieveNodeElm(tree);
	//can we win now?
	if (canWeWinNow(temp) == 1)
	{
		//make this option a possible winning move
		temp->eval = 1000;
		root->eval = 1000;
	}else
	{
		//can we lose now? given that there's not a winning move somewhere
		if ( (canWeLoseNow(temp) == 1) && (root->eval != 1000) )
		{
			//make this a possible losing move
			temp->eval = -1000;
			root->eval = -1000;
		}
	}
	//travese through the rest of the options on this level
	while (existNode(tree,TORIGHT) == 1)
	{
		//go to next option
		nextNode(tree,TORIGHT);
		//get the game area for this option
		temp = retrieveNodeElm(tree);
		//can we win now?
		if (canWeWinNow(temp) == 1)
		{
			//make this option a possible winning move
			temp->eval = 1000;
			root->eval = 1000;
		}else
		{
			//can we lose now? given that there's not a winning move somewhere
			if ( (canWeLoseNow(temp) == 1) && (root->eval != 1000) )
			{
				//make this a possible losing move
				temp->eval = -1000;
				root->eval = -1000;
			}
		}
	}
}

void bestMove(binaryTreePtr tree)
/* This procedure will determine the best move available
 * by using a minmax method to either compute the value (on a leaf)
 * or sending it one level up where will be decided if the minimum or the maximum value will be used
 * Pre-condition:none
 * Post-condition: the root of the tree now has the minmax value needed
*/
{
	infoPtr gameInfo,temp;
	treeNodePtr current;
	//retrieve the current node's info
	gameInfo = retrieveNodeElm(tree);

	//is this a leaf?
	if ((existNode(tree,TOLEFT)== -1) )
	{
		//then compute the value of this square
		gameInfo->eval = evaluateSquares(gameInfo);
	}else
	{
		//go down a level, to first node
		nextNode(tree, TOLEFT);
		//get the current node in tree
		current = getCurrentNode(tree);

		//get the info for this node
		temp = retrieveNodeElm(tree);

		//call this procedure again, this will send up a value
		bestMove(tree);

		//go back to first node on the level
		setCurrentNode(tree, current);
		//set the parent level's value to this node's
		gameInfo->eval = temp->eval;

		//is there a next node on this level?
		while (existNode(tree,TORIGHT) == 1)
		{
			//then go to the next node
			nextNode(tree, TORIGHT);

			//set this as the current node on this level
			current = getCurrentNode(tree);

			//get the info for this node
			temp = retrieveNodeElm(tree);

			//call this procedure again, this will send up a value
			bestMove(tree);

			//go back to node on this level we're working on
			setCurrentNode(tree, current);
			//who's turn was it last
			if (gameInfo->turn == COMPUTER)
			{
				//if temp->eval is smaller
				if (gameInfo->eval > temp->eval)
				{
					//this is the new minimum value
					gameInfo->eval = temp->eval;
				}
			}else
			{
				//if temp->eval is bigger
				if (gameInfo->eval < temp->eval)
				{
					//this is the new maximum value
					gameInfo->eval = temp->eval;
				}
			}
		}
	}
}

void makeMove(binaryTreePtr tree, infoPtr gameInfo)
/* This procedure will make the best move available
 * Pre-condition: none
 * Post-condition: the best move has been made
*/
{
	//go to the root
	nextNode(tree, TOROOT);
	//get the info for root/the top level
	infoPtr root = retrieveNodeElm(tree);
	//goto first node of next level
	nextNode(tree, TOLEFT);
	//get the info for this node
	infoPtr temp = retrieveNodeElm(tree);

	//is this the value on this level that gave root it's value?
	if (root->eval == temp->eval)
	{
		//it is! Then copy the game area to root
		copyGameArea(temp, root);
	}else
	{
		//is there a next node on this level and is the current node's value not
		//equal to the root's?
		while ((existNode(tree,TORIGHT) == 1) && (root->eval != temp->eval))
		{
			//goto next node on level
			nextNode(tree, TORIGHT);
			//get the info for this node
			temp = retrieveNodeElm(tree);
			//is this the value on this level that gave root it's value?
			if (root->eval == temp->eval)
			{
				//it is! Then copy the game area to root
				copyGameArea(temp, root);
			}
		}
	}

	//copy the playArea from the root to the game's playArea
	copyGameArea(root,gameInfo);
}

void getComputerMove(infoPtr gameInfo)
/* This procedure creates a move for the player depending on the current game state
 * as given by gameInfo
 * Pre-condition: gameInfo exists
 * Post-Condition: a move has been made, and the game state updated
*/
{
	//does gameinfo exist?
	if (gameInfo == NULL)
	{
		//don't do anything
		return;
	}

	//create a pointer for tree
	binaryTreePtr tree;

	//empty pointers that will reference the unique functions for this tree
	void (*delElem)();
	void (*dspElem)();
	int (*cmpElem)();

	//reference the unique functions
	delElem = deleteElement;
	dspElem = dspElement;
	cmpElem = cmpElement;

	//create a pointer to a new empty tree
	//with the unique functions passed as parameters
	tree = createTree( delElem, dspElem, cmpElem);
	if (tree == NULL)
	{
		//quit procedure there was a fault
		return;
	}

	//make a new node that will serve as the root in our tree
	infoPtr root = NULL;
	root = malloc(sizeof(struct info));
	root->turn = USER;
	root->eval = 0;
	//copy the current game's playing area to root
	copyGameArea(gameInfo,root);


	//insert the new root into the tree
	if (insertNode(tree, root, TOROOT) != 1)
	{
		//it didn't work. don't go further
		return;
	}

	//go to the root level
	nextNode(tree,TOROOT);
	//create the unique tree for the current game state
	expandTree(tree);

	//look if there's a winning move now
 	lookForWinningMove(tree);

	//go to the root level and get the gamestate
	nextNode(tree, TOROOT);
	root = retrieveNodeElm(tree);

	//pick the best move for the current game state from the tree
	//if there hasn't been a winning move already
	if ( (root-> eval != 1000) && (root->eval != -1000) )
	{
		//get the best move
		bestMove(tree);
	}
	//make the best move possible
	makeMove(tree, gameInfo);
	//remove the tree from memory
	deleteTree(tree);
}

