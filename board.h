// board.hpp
// Tic-Tac-Toe class object declaration representing a board	
// Rev1 	221228		T. Barake
// Rev0 	950227		T. Barake

//#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define X -1
#define O  1

#define RSIZE		5
#define MAXMOVE		(RSIZE*RSIZE)
#define MAXDEPTH	9
inline int min(int a,int b) {return ((a) < (b)?(a):(b));}
inline int max(int a,int b) {return ((a) > (b)?(a):(b));}
class ttt
{
public:
	ttt(void);
	void display(bool dummy=false);	// display board message
	bool legal(int move);	// is move legal ?
	bool play(int move, int type);	// play a move
	int win();              		// has someone won?
	int depth;						// how many moves have been played?
	int maxmoves;					// maximum moves to search

private:
	short int board[RSIZE][RSIZE];				// array representing board
};
