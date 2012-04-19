//Header file for ComputerMoves.c

//all states that a square can take
//current player uses USER and COMPUTER
enum playerState{
  USER,
  COMPUTER,
  EMPTY
};

//the structure that contains everything about the game itself
typedef struct info
{
	enum playerState playArea[3][3];
	int score[2], eval;
	enum playerState turn;
}*infoPtr;

int squaresAvailable(infoPtr gameInfo);
/* This function checks if there's squares available in the gameArea of gameInfo
 * Pre-condition: gameInfo exists
 * Post-condition: number of squares available is returned
*/

void getComputerMove(infoPtr gameInfo);
/* This procedure creates a move for the player depending on the current game state
 * as given by gameInfo
 * Pre-condition: gameInfo exists
 * Post-Condition: a move has been made, and the game state updated
*/

