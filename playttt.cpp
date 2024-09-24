// playttt.cpp
// plays the tic tac toe game
// Rev0 	950227		creation		T. Barake
// Rev1		950304		removed pruning 	T. Barake
//						to simplify
// Rev2     230107      cleanup, generalization in progress.
// todo: translate to go
// using min max tree search - can be generalized to any logical game
// by changing the class ttt object

#include <ctype.h>
#include <windows.h>


#include "board.h" // tic tac toe board object

#define LOSE -1 // scores given, must be:  LOSE < TIE < WIN

#define WIN 1
#define TIE 0

#define COMPUTER	X
#define HUMAN		O

int respond(ttt b); // function to calculate a move
char getch() { return getchar(); }
void clrscr() {}

int getMove(ttt board)
{
	int r, move;
	do
	{
		printf("move:");
		r = scanf_s("%d", &move);
		fseek(stdin, 0, SEEK_END);
		if (move >= MAXMOVE || r == EOF || (r == 0 && move < 0))
			return -1;
	} while (r <= 0 || !board.legal(move)); // reject bad moves

	printf("%d\n", move);
	return move;
}


int main()
{
	do
	{
		clrscr();
		printf(" Tic Tac Toe: \n");
		printf(" You are %c, you play first\n", HUMAN == O ? 'O' : 'X');
		printf(" Legal moves are 0 to %d\n\n", MAXMOVE - 1);
		{
			ttt board;
			board.display(true);

			printf("\n");
		}

		int win = 0;
		{
			ttt board; // init and create the board
			do {
				int move = getMove(board); // human player's move
				if (move == -1)
					return 0;

				board.play(move, HUMAN);
				board.display(true);

				if (!(win = board.win())) {
					board.play(respond(board), COMPUTER);	// respond here
					board.display(true);
					win = board.win();
				}

			} while (!win && board.depth < board.maxmoves);
		}
		if (win) {
			printf("\n******* %c Wins!\nPress \'q\' to exit. *******\n\n\n\n", win == O ? 'O' : 'X');
			Sleep(2000);
		}
		else {
			printf("Tie game.\n\n");
			Sleep(2000);
		}
	} while (toupper(getch()) != 'Q');
	return 0;
}

// minimum and maximum functions
// inline int min(int a, int b) { return (a < b ? a:b); }
// inline int max(int a, int b) { return ( a > b ? a: b);}

// Calculate the score for a board position
// INPUTS:
// 	b: 		the board configuration and depth
// 	move:	proposed legal move 0 to MAXMOVE
// 	player: 	O or X
//
// OUTPUT:
//	a score for the proposed move: -1, 0 ,+1
//
// ALGORITHM:
// 	this is a depth first min-max tree search.
//	the moves are generated, until b.depth==b.maxmoves or a TIE or WIN occurs
//	we go back up the tree, selecting the max for COMPUTER, min for opponent
// 	the search stops if further recursion cannot improve the result
//	note: since the weights are binary, the fastest win is not guaranteed.
//		to modify would require wins at shallow depths to be weighed more
//		and the pruning values to be passed as parameters - (see wikipedia for minmax tree pruning algorithms)
//
int value(ttt b, int move, int player, int alpha, int beta)
{
    if (!b.legal(move))
        return TIE;

    b.play(move, player);

    if (b.win())
        return (player == COMPUTER) ? WIN : LOSE;

    if (b.depth == MAXDEPTH - 1)
        return TIE;

    player = -player;

    if (player == COMPUTER) // maximize score
    {
        int score = LOSE * 2;
        for (move = 0; move < b.maxmoves; move++)
        {
            if (b.legal(move))
            {
                score = max(score, value(b, move, player, alpha, beta));
                alpha = max(alpha, score);
                if (beta <= alpha)
                    break; // Beta cutoff
            }
        }
        return score;
    }
    else // minimize score
    {
        int score = WIN * 2;
        for (move = 0; move < b.maxmoves; move++)
        {
            if (b.legal(move))
            {
                score = min(score, value(b, move, player, alpha, beta));
                beta = min(beta, score);
                if (beta <= alpha)
                    break; // Alpha cutoff
            }
        }
        return score;
    }
}
//
// get the best move for the computer give board configuration b
// Input: the object b: is the board
// returns the position of the best move for that board
//
int respond(ttt b)
{
    int move, best_move = -1, score, best_score = LOSE * 2;
    int alpha = LOSE * 2;
    int beta = WIN * 2;

    for (move = 0; move < b.maxmoves; move++)
    {
        if (b.legal(move))
        {
            score = value(b, move, COMPUTER, alpha, beta);
            if (best_score < score)
            {
                best_move = move;
                best_score = score;
            }
            alpha = max(alpha, best_score);
        }
    }

    return best_move;
}