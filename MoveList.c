#include "MoveList.h"

MOVE *CreateMove(PIECE *item, unsigned int captured, unsigned int hasMoved, unsigned int special, unsigned int x_coord, unsigned int y_coord) 
{
	MOVE *move;
	move = malloc(sizeof(MOVE));
	move -> List = NULL;
	move -> Next = NULL;
	move -> Prev = NULL;
	move -> piece = item;
	move -> captured = captured;
	move -> hasMoved = hasMoved;
	move -> special = special;
	move -> x = x_coord;
	move -> y = y_coord;
	return move;
}

LMOVES *CreateList() {
	LMOVES *moveList;
	moveList = malloc(sizeof(LMOVES));
	moveList -> numMoves = 0;
	moveList -> First = NULL;
	moveList -> Last = NULL;

	return moveList;
}

void DeleteMove(LMOVES *list) {
	MOVE *move;
	assert(list);
	if(list->numMoves){
		move = list -> Last;
		if(list -> numMoves == 1){
			list -> First = NULL;
			list -> Last = NULL;
		}
		else{
			list -> Last = move -> Prev;
			list -> Last -> Next = NULL;
		}
		free(move);
		list->numMoves--;
	}
}

/* void DeleteMove(MOVE *move) {
	LMOVES *list;
	assert(move);
	list = move -> List;
	if(list -> numMoves == 1) {
		list -> First = NULL;
		list -> Last = NULL;
	}
	else {
		list -> Last = move -> Prev;
		list -> Last -> Next = NULL;
	}
	free(move);
	list -> numMoves--;
}

void DeleteList(LMOVES *moveList) {
	MOVE *e, *n;

	assert(moveList);
	e = moveList -> Last;
	while(moveList -> numMoves)
	{
		n = e -> Prev;
		DeleteMove(e);
		e = n;
		moveList -> numMoves--;
	}
	free(moveList);
} */

void DeleteList(LMOVES *list) {
	assert(list);
	while(list->numMoves) {
		DeleteMove(list);
	}
	free(list);
}

void AppendMove(LMOVES *list, MOVE *move) {
	assert(move);
	assert(list);
	move -> List = list;
	if(!list -> numMoves){
		list -> First = move;
		move -> Prev = NULL;
	}
	else {
		move -> Prev = list -> Last;
		list -> Last -> Next = move;
	}
	list -> Last = move;
	move -> Next = NULL;

	list -> numMoves++;
}

void LogMove(LMOVES *list, PIECE *piece, unsigned int captured, unsigned int special, unsigned int x, unsigned int y){
	MOVE *move;
	move = CreateMove(piece, captured, piece -> hasMoved, special, x, y);
	AppendMove(list, move);
}

void Undo(PIECE *board[8][8], LMOVES *list) {
	MOVE *lastMove, *prevMove;
	unsigned int x, y;

	if(list -> numMoves) {
		lastMove = list -> Last;
		prevMove = lastMove -> Prev;

		x = lastMove -> piece -> x_coord;
		y = lastMove -> piece -> y_coord;

		board[lastMove -> x][lastMove -> y] = lastMove -> piece;
		lastMove -> piece -> x_coord = lastMove -> x;
		lastMove -> piece -> y_coord = lastMove -> y;
		lastMove -> piece -> hasMoved = lastMove -> hasMoved;
		DeleteMove(list);

		if(prevMove && prevMove -> captured) {
			board[prevMove -> x][prevMove -> y] = prevMove -> piece;
			prevMove -> captured = 0;
			DeleteMove(list);
		}

		else if(prevMove && prevMove -> special == 1) {
			board[x][y] = NULL;
			x = prevMove -> piece -> x_coord;
			y = prevMove -> piece -> y_coord;
			board[prevMove -> x][prevMove -> y] = prevMove -> piece;
			prevMove -> piece -> x_coord = prevMove -> x;
			prevMove -> piece -> y_coord = prevMove -> y;
			prevMove -> piece -> hasMoved = prevMove -> hasMoved;
			board[x][y] = NULL;
			DeleteMove(list);
		}

		else if(prevMove && prevMove -> special == 2) {
			printf("%d, %d\n", x, y);
			board[x][y] = NULL;
			board[prevMove -> x][prevMove -> y] = prevMove -> piece;
			DeleteMove(list);
		}

		else {
			board[x][y] = NULL;
		}
	}
}
