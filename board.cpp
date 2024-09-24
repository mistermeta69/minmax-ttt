#include "board.h"

// Part of ttt example code
// Rev2 	221228	T. Barake  5x5
// Rev1		211121	T. Barake
// Rev0		950227	T. Barake
//
// Defines the board object - pass this class object to the minmax algorithm
// Can be asked to make moves
// can return game status and
// can display itself
// Change this class to change the game being evaluated by the minmax algorithm

// initialize the board
ttt::ttt()
{
	for (int i = 0; i < RSIZE; i++)
		for (int j = 0; j < RSIZE; j++)
			board[i][j] = 0;
		
	depth = 0;
	maxmoves = MAXMOVE;
} // constructor

// Input:
// 	move is a number 0 to MAXMOVE-1
// returns bool TRUE for legal
// 		!TRUE otherwise

bool ttt::legal(int move)
{
	if (move >= 0 && move < MAXMOVE)
		if (board[move/RSIZE][move%RSIZE] == 0)
			return true;
	return !true;
}

// display the board in ASCII to stdout
void ttt::display(bool empty)
{
	printf("\n");
	for (int i = 0; i < RSIZE; i++) 
	{
		for (int j = 0; j < RSIZE; j++)
		{
			switch (board[i][j])
			{
			case X:
				printf(" X");
				break;
			case O:
				printf(" O");
				break;
			default:
				if (empty)
					printf("\x1b[90m%2d\x1b[0m",i * RSIZE + j);
				else
					printf("  ");
				break;
			}
		if (j < RSIZE-1) 
			printf(" |");
		}
		printf("\n");
		if (i < RSIZE - 1) {
			for (int k = 0; k < RSIZE - 1; k++)
				printf("---|");
			printf("---\n");
		}
	}
	printf("\n\n");
}

// play a move 0 to MAXMOVES
// returns 0 if legal, non-zero if not
// Inputs:
//	move is board position 0-8
// 	type is X or O

bool ttt::play(int move, int type)
{
	int mi = move / RSIZE;
	int mj = move % RSIZE;
	if (!legal(move) || (type != O && type != X))
		return !true; // illegal
	board[mi][mj] = type;
	depth++;
	return true;
}

// for a  more complex game, say chess, you would return a score to be used to decide whether to
// follow that branch or to try elsewhere...
// scoring a board is hard - see the literature
// for tic tac toe it is easy, we only return -1, 0, 1
// checks if a row of O's or X's has been played
// returns 0 if no one won, 1 if X's won, -1 if O's won
int ttt::win()
{
	int sum;

	for (int i = 0; i < RSIZE; i++) // horizontals
	{
		int sum = 0;
		for (int j = 0; j < RSIZE; j++)
		{
			if (board[i][j] == 0) break;
			sum += board[i][j];
		}
		if (sum == RSIZE * O)
			return O;
		else if (sum == RSIZE * X)
			return X;
	}

	for (int i = 0; i < RSIZE; i++) // verticals
	{
		int sum = 0;
		for (int j = 0; j < RSIZE; j++)
		{
			if (board[j][i] == 0) break;
			sum += board[j][i];
		}
		if (sum == RSIZE * X)
			return X;
		else if (sum == RSIZE * O)
			return O;
	}

	// diagonals 
	sum = 0;
	for (int i = 0; i < RSIZE; i++) {
		{
			if (board[i][i] == 0) break;
			sum += board[i][i];
		}
		if (sum == RSIZE * O)
			return O;
		else if (sum == RSIZE * X)
			return X;
	}
	sum = 0;
	for (int i = 0; i < RSIZE; i++) {
		{
			if (board[i][RSIZE - i - 1] == 0) break;
			sum += board[i][RSIZE-i-1];
		}
		if (sum == RSIZE * O)
			return O;
		else if (sum == RSIZE * X)
			return X;
	}
	return 0; // nada - no win or lose, but we could return a score between -1 and 1 if to improve play

}
