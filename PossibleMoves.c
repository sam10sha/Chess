#include "PossibleMoves.h"

PMOVE *CreatePossibleMove(unsigned int i, unsigned int j) {
	PMOVE *pmove;
	pmove = malloc(sizeof(PMOVE));
	pmove -> List = NULL;
	pmove -> Next = NULL;
	pmove -> Prev = NULL;
	pmove -> x = i;
	pmove -> y = j;
	return pmove;
}

LPMOVES *CreateListOfPossibleMoves() {
	LPMOVES *list;
	list = malloc(sizeof(LPMOVES));
	list -> Length = 0;
	list -> First = NULL;
	list -> Last = NULL;
	return list;
}

void DeletePossibleMove(LPMOVES *list) {
	PMOVE *pmove;
	assert(list);
	if(list->Length){
		pmove = list -> Last;
		if(list -> Length == 1){
			list -> First = NULL;
			list -> Last = NULL;
		}
		else{
			list -> Last = pmove -> Prev;
			list -> Last -> Next = NULL;
		}
		free(pmove);
		list->Length--;
	}
}

void DeleteListOfPossibleMoves(LPMOVES *list) {
	assert(list);
	while(list->Length) {
		DeletePossibleMove(list);
	}
	free(list);
}

void AppendPossibleMove(LPMOVES *list, PMOVE *pmove) {
	assert(list);
	assert(pmove);
	if(!list->Length) {
		list -> First = pmove;
		pmove -> Prev = NULL;
	}
	else {
		list -> Last -> Next = pmove;
		pmove -> Prev = list -> Last;
	}
	list -> Last = pmove;
	pmove -> Next = NULL;
	list->Length++;
}

LPMOVES *FindPossibleMoves(GAME *game, PIECE *piece) {
	LPMOVES *newList;
	PIECE *king;
	assert(game -> PossibleMoves);
	DeleteListOfPossibleMoves(game -> PossibleMoves);
	newList = CreateListOfPossibleMoves();

	switch(piece -> type) {
		case pawn:	MovesForPawn(game -> Board, piece, newList, game -> List);
				break;
		case rook:	MovesForRook(game -> Board, piece, newList);
				break;
		case knight:	MovesForKnight(game -> Board, piece, newList);
				break;
		case bishop:	MovesForBishop(game -> Board, piece, newList);
				break;
		case queen:	MovesForQueen(game -> Board, piece, newList);
				break;
		default:	MovesForKing(game -> Board, piece, newList);
	}
	switch(piece -> value) {
		case white:	king = game -> WhiteKing;
				break;
		default:	king = game -> BlackKing;
	}

	/* return newList; */

	game -> PossibleMoves = RefineForCheck(game, king, piece, newList);

	return game -> PossibleMoves;
}

void MovesForKing(PIECE *board[8][8], PIECE *piece, LPMOVES *list) {
	PMOVE *move;
	int i, j, x, y;

	for(i = -1; i < 2; i++)
		for(j = -1; j < 2; j++)
			if(i == 0 && j == 0)
				continue;
			else {
				x = piece -> x_coord + i;
				y = piece -> y_coord + j;
				if(x >= 0 && x < 8 && y >= 0 && y < 8) {
					if((!board[x][y] || (board[x][y] -> value != piece -> value))/* && !LookForCheck(board, piece, x, y)*/) {
						move = CreatePossibleMove(x, y);
						AppendPossibleMove(list, move);
					}
				}
			}
	CheckCastle(board, piece, list);
}

void CheckCastle(PIECE *board[8][8], PIECE *piece, LPMOVES *list){
	PMOVE *move;
	int i, x, y;
	int left_cass = 1; 
	int right_cass = 1;
	
	x = piece->x_coord;
	y = piece->y_coord;
	for (i=x+1; i<7; i++){
		if (board[i][y])
			right_cass = 0;
	}
	for (i=1; i<x-1; i++){
		if (board[i][y])
			left_cass = 0;
	}
	if (!piece->hasMoved && board[0][y] && !board[0][y]->hasMoved && left_cass){
		move = CreatePossibleMove(2, y);
		AppendPossibleMove(list, move);
	}
	if (!piece->hasMoved && board[7][y] && !board[7][y]->hasMoved && right_cass){
		move = CreatePossibleMove(6, y);
		AppendPossibleMove(list, move);
	}
}

void MovesForQueen(PIECE *board[8][8], PIECE *piece, LPMOVES *list) {
	MovesForRook(board, piece, list);
	MovesForBishop(board, piece, list);
}

void MovesForBishop(PIECE *board[8][8], PIECE *piece, LPMOVES *list) {
	PMOVE *move;
	int mult_x, mult_y;
	int x, y, i;
	mult_x = 1;

	for(i = 0; i < 4; i++) {
		int blocked = 0;
		mult_y = (i / 2 % 2) ? -1 : 1;
		x = piece -> x_coord + mult_x;
		y = piece -> y_coord + mult_y;
		while(x >= 0 && x < 8 && y >= 0 && y < 8 && !blocked) {
			if(!board[x][y]) {
				move = CreatePossibleMove(x, y);
				AppendPossibleMove(list, move);
			}
			else if(board[x][y] -> value != piece -> value) {
				blocked = 1;
				move = CreatePossibleMove(x, y);
				AppendPossibleMove(list, move);
			}
			else
				blocked = 1;
			x += mult_x;
			y += mult_y;
		}
		mult_x = (mult_x + 1) ? -1 : 1;
	}
}

void MovesForKnight(PIECE *board[8][8], PIECE *piece, LPMOVES *list) {
	PMOVE *move;
	int x, y, i, j;
	int incr_x, incr_y, mult_x, mult_y;
	incr_x = 2;
	incr_y = 1;
	mult_x = 1;

	for(i = 0; i < 2; i++) {
		for(j = 0; j < 4; j++) {
			mult_y = (j / 2 % 2) ? -1 : 1;
			x = piece -> x_coord + incr_x * mult_x;
			y = piece -> y_coord + incr_y * mult_y;
			if(x >= 0 && x < 8 && y >= 0 && y < 8)
				if(!board[x][y] || (board[x][y] -> value != piece -> value)) {
					move = CreatePossibleMove(x, y);
					AppendPossibleMove(list, move);
				}
			mult_x = (mult_x + 1) ? -1 : 1;
		}
		incr_x = (incr_x - 1) ? 1 : 2;
		incr_y = (incr_y - 1) ? 1 : 2;
	}
}

void MovesForRook(PIECE *board[8][8], PIECE *piece, LPMOVES *list) {
	PMOVE *move;
	int mult_x, mult_y;
	int x, y, i;
	mult_x = 1;
	mult_y = 0;

	for(i = 0; i < 4; i++) {
		int blocked = 0;
		mult_x *= (i / 2 % 2) ? -1 : 1;
		mult_y *= (i / 2 % 2) ? -1 : 1;
		x = piece -> x_coord + mult_x;
		y = piece -> y_coord + mult_y;
		while(x >= 0 && x < 8 && y >= 0 && y < 8 && !blocked) {
			if(!board[x][y]) {
				move = CreatePossibleMove(x, y);
				AppendPossibleMove(list, move);
			}
			else if(board[x][y] -> value != piece -> value) {
				blocked = 1;
				move = CreatePossibleMove(x, y);
				AppendPossibleMove(list, move);
			}
			else
				blocked = 1;
			x += mult_x;
			y += mult_y;
		}
		mult_x = mult_x ? 0 : 1;
		mult_y = mult_y ? 0 : 1;
	}
}

void MovesForPawn(PIECE *board[8][8], PIECE *piece, LPMOVES *list, LMOVES *history) {
	PMOVE *move;
	int direction;
	int x, y;
	direction = (piece -> value == white) ? 1 : -1;
	x = piece -> x_coord;
	y = piece -> y_coord;

/* addition of the option to moving two steps for the first time*/
/* ------ beginning ------*/
	if (!(piece->hasMoved) && !board[x][y+direction] && !board[x][y+(2*direction)]){    /*if piece has not moved yet and nothing is blocking */
		move = CreatePossibleMove(x, y+(direction*2));
		AppendPossibleMove(list,move);
	}
/* ------- end --------- */

	if(!board[x][y + direction]) {
		move = CreatePossibleMove(x, y + direction);
		AppendPossibleMove(list, move);
		}
	if(x > 0 && y + direction >= 0 && y + direction < 8 && board[x - 1][y + direction] && board[x - 1][y + direction] -> value != piece -> value) {
		move = CreatePossibleMove(x - 1, y + direction);
		AppendPossibleMove(list, move);
	}

	if(x < 7 && y + direction >= 0 && y + direction < 8 && board[x + 1][y + direction] && board[x + 1][y + direction] -> value != piece -> value) {
		move = CreatePossibleMove(x + 1, y + direction);
		AppendPossibleMove(list, move);
	}

	en_passant(board, piece, history, list);
}

void en_passant(PIECE *board[][8], PIECE *piece, LMOVES *moveList, LPMOVES *list){
	PMOVE *move;
	int x, y, i;
	unsigned int has_moved;
	
	x = piece->x_coord;
	y = piece->y_coord;
	/* get the piece */
	/* check whether there is another piece right next to it */
	/* the piece to its very right or left must be of the different color */
	/* two minus the position of the next door piece must be either 1 or 6 */
	for (i=0; i<2; i++){
		if(x-1+2*i >= 0 && x-1+2*i < 8 && board[x-1+2*i][y]) {
			has_moved = board[x-1+2*i][y]->hasMoved;
			if (board[x-1+2*i][y]->type == pawn && board[x-1+2*i][y]->value != piece->value && moveList->Last->y - y == 2 && has_moved == 1 && moveList->Last->piece->type == pawn){
				move = CreatePossibleMove(x-1+2*i, 5);
				AppendPossibleMove(list, move);
			}
			if (board[x-1+2*i][y]->type == pawn && board[x-1+2*i][y]->value != piece->value && y - moveList->Last->y == 2 && has_moved == 1 && moveList->Last->piece->type == pawn){
				move = CreatePossibleMove(x-1+2*i, 1);
				AppendPossibleMove(list, move);
			}
		}
	}
}

int LookForCheck(PIECE *board[8][8], PIECE *piece, unsigned int x, unsigned int y, LMOVES *history) {
	PIECE *temp;
	LPMOVES *list;
	PMOVE *move;
	int i, score;
	score = 0;
	temp = CreatePiece(piece -> value, piece -> type, x, y);

	/* Checks for enemy pawn */
	list = CreateListOfPossibleMoves();
	MovesForPawn(board, temp, list, history);
	move = list -> First;
	for(i = 0; i < list -> Length; i++) {
		if(board[move -> x][move -> y] && (board[move -> x][move -> y] -> type == pawn)) {
			score += 6;

			/* DeletePiece(temp);
			DeleteListOfPossibleMoves(list);
			return 0; */
		}
		move = move -> Next;
	}
	DeleteListOfPossibleMoves(list);

	/* Checks for enemy bishop or queen */
	list = CreateListOfPossibleMoves();
	MovesForBishop(board, temp, list);
	move = list -> First;
	for(i = 0; i < list -> Length; i++) {
		if(board[move -> x][move -> y] && (board[move -> x][move -> y] -> type == bishop)) {
			score += 3;

			/* DeletePiece(temp);
			DeleteListOfPossibleMoves(list);
			return 0; */
		}

		else if(board[move -> x][move -> y] && (board[move -> x][move -> y] -> type == queen))
			score += 1;


		move = move -> Next;
	}
	DeleteListOfPossibleMoves(list);

	/* Checks for enemy rook or queen */
	list = CreateListOfPossibleMoves();
	MovesForRook(board, temp, list);
	move = list -> First;
	for(i = 0; i < list -> Length; i++) {
		if(board[move -> x][move -> y] && (board[move -> x][move -> y] -> type == rook)) {
			score += 2;

			/* DeletePiece(temp);
			DeleteListOfPossibleMoves(list);
			return 0; */
		}

		else if(board[move -> x][move -> y] && (board[move -> x][move -> y] -> type == queen))
			score += 1;

		move = move -> Next;
	}
	DeleteListOfPossibleMoves(list);

	/* Checks for enemy knight */
	list = CreateListOfPossibleMoves();
	MovesForKnight(board, temp, list);
	move = list -> First;
	for(i = 0; i < list -> Length; i++) {
		if(board[move -> x][move -> y] && (board[move -> x][move -> y] -> type == knight)) {
			score += 3;

			/* DeletePiece(temp);
			DeleteListOfPossibleMoves(list);
			return 0; */
		}
		move = move -> Next;
	}
	DeleteListOfPossibleMoves(list);

	DeletePiece(temp);
	return score;
}

LPMOVES *RefineForCheck(GAME *game, PIECE *King, PIECE *piece, LPMOVES *list) {
	LPMOVES *newList;
	PMOVE *pmove, *moveToAdd;
	int i;

	assert(list);

	if(!list -> Length)
		return list;
	else {
		newList = CreateListOfPossibleMoves();
		pmove = list -> First;
		for(i = 0; i < list -> Length; i++) {
			if(Try(game, King, piece -> x_coord, piece -> y_coord, pmove -> x, pmove -> y)) {
				moveToAdd = CreatePossibleMove(pmove -> x, pmove -> y);
				AppendPossibleMove(newList, moveToAdd);
			}
			pmove = pmove -> Next;


			/* if(pmove) {
				move(game, piece -> x_coord, piece -> y_coord, pmove -> x, pmove -> y);
				if(!LookForCheck(game -> Board, King, King -> x_coord, King -> y_coord)) {
					moveToAdd = CreatePossibleMove(pmove -> x, pmove -> y);
					AppendPossibleMove(newList, moveToAdd);
				}
				Undo(game -> Board, game -> List);

			} */

		}
		DeleteListOfPossibleMoves(list);
		return newList;
	}
}

void CopyPossibleMoveList(LPMOVES *Original, LPMOVES *Copy) {
	int i;
	PMOVE *current, *pmove;
	DeleteListOfPossibleMoves(Copy);
	Copy = CreateListOfPossibleMoves();
	current = Original -> First;
	for(i = 0; i < Original -> Length; i++) {
		pmove = CreatePossibleMove(current -> x, current -> y);
		AppendPossibleMove(Copy, pmove);
		current = current -> Next;
	}
}
