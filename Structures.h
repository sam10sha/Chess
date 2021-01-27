#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Game GAME;
typedef struct Piece PIECE;
typedef struct ListOfMoves LMOVES;
typedef struct Move MOVE;
typedef struct PossibleMove PMOVE;
typedef struct ListOfPossibleMoves LPMOVES;


typedef enum
{
	black, white
}color;
       
typedef enum
{
	king, queen, bishop, knight, rook, pawn
}unit_type;

struct Piece
{ 
	color value;
	unit_type type;
	unsigned int hasMoved;
	unsigned int x_coord;
	unsigned int y_coord;
};

struct Game
{
	LMOVES *List;
	LPMOVES *PossibleMoves;
	PIECE *ListOfPieces[32];
	PIECE *Board[8][8];
	PIECE *BlackKing;
	PIECE *WhiteKing;
};

struct Move
{
	LMOVES *List;
	MOVE *Next;
	MOVE *Prev;
	PIECE *piece;
	unsigned int x;
	unsigned int y;
	unsigned int captured;
	unsigned int hasMoved;
	unsigned int special;
};

struct ListOfMoves
{
	unsigned int numMoves;
	MOVE *First;
	MOVE *Last;
};

struct PossibleMove {
	LPMOVES *List;
	PMOVE *Next;
	PMOVE *Prev;
	unsigned int x;
	unsigned int y;
};

struct ListOfPossibleMoves {
	unsigned int Length;
	PMOVE *First;
	PMOVE *Last;
};

#endif
