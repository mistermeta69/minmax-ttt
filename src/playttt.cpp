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

int getMove(ttt b)
{
	int r, move;
	do
	{
		printf("move:");
		r = scanf_s("%d", &move);
		fseek(stdin, 0, SEEK_END);
		if (move >= MAXMOVE || r == EOF || (r == 0 && move < 0))
			return -1;
	} while (r <= 0 || !b.legal(move)); // reject bad moves

	printf("%d\n", move);
	return move;
}


int main()
{
	do
	{
		ttt b;
		clrscr();
		printf(" Tic Tac Toe: \n");
		printf(" You are %c, you play first\n", HUMAN == O ? 'O' : 'X');
		printf(" Legal moves are 0 to %d\n\n", MAXMOVE - 1);
		b.display(true);
		printf("\n");

		int win = 0;
		do {
				int move = getMove(b); // human player's move
				if (move == -1)
					return 0;

				b.play(move, HUMAN);
				b.display(true);

				if (!(win = b.win())) {
					b.play(respond(b), COMPUTER);	// respond here
					b.display(true);
					win = b.win();
				}

			} while (!win && b.depth < b.maxmoves);

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
/*
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

*/
int value(ttt b, int move, int player, int alpha, int beta);
int evaluateLine(const ttt& b, int startRow, int startCol, int dRow, int dCol, int player);
int evaluateBoard(const ttt& b, int player);
int positionalBonus(const ttt& b, int player);
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

// ... (previous includes and definitions)

#define BOARD_SIZE RSIZE // Adjust this for larger boards
#define WIN_LENGTH BOARD_SIZE // Adjust this for the number of pieces needed to win

// New evaluation function
int evaluateBoard( ttt& b, int player)
{
    int score = 0;
    
    // Check rows, columns, and diagonals
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        score += evaluateLine(b, i, 0, 0, 1, player); // Row
        score += evaluateLine(b, 0, i, 1, 0, player); // Column
    }
    score += evaluateLine(b, 0, 0, 1, 1, player); // Main diagonal
    score += evaluateLine(b, 0, BOARD_SIZE - 1, 1, -1, player); // Anti-diagonal

    // Add positional bonuses
    score += positionalBonus(b, player);

    return score;
}

// Evaluate a single line (row, column, or diagonal)
int evaluateLine( ttt& b, int startRow, int startCol, int dRow, int dCol, int player)
{
    int score = 0;
    int playerCount = 0;
    int opponentCount = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int row = startRow + i * dRow;
        int col = startCol + i * dCol;
        int cell = b.board[row][col];

        if (cell == player)
            playerCount++;
        else if (cell == -player)
            opponentCount++;
    }

    if (opponentCount == 0)
        score += pow(10, playerCount);
    else if (playerCount == 0)
        score -= pow(10, opponentCount);

    return score;
}

// Add positional bonuses
int positionalBonus( ttt& b, int player)
{
    int score = 0;
    
    // Bonus for center positions
    int centerStart = BOARD_SIZE / 2 - 1;
    for (int i = centerStart; i <= centerStart + 1; i++)
    {
        for (int j = centerStart; j <= centerStart + 1; j++)
        {
            if (b.board[BOARD_SIZE][j] == player)
                score += 3;
        }
    }

    // Bonus for corners
    int corners[4][2] = {{0, 0}, {0, BOARD_SIZE-1}, {BOARD_SIZE-1, 0}, {BOARD_SIZE-1, BOARD_SIZE-1}};
    for (auto& corner : corners)
    {
        if (b.board[corner[0]][corner[1]] == player)
            score += 2;
    }

    return score;
}

// Modified value function
int value(ttt b, int move, int player, int alpha, int beta)
{
    if (!b.legal(move))
        return TIE;

    b.play(move, player);

    if (b.win())
        return (player == COMPUTER) ? WIN * 1000 : LOSE * 1000;

    if (b.depth == MAXDEPTH - 1)
        return evaluateBoard(b, COMPUTER) - evaluateBoard(b, HUMAN);

    player = -player;

    if (player == COMPUTER) // maximize score
    {
        int score = LOSE * 2000;
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
        int score = WIN * 2000;
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

