#include "Piece.h"

PIECE *CreatePiece(color shade, unit_type unit, unsigned int x, unsigned int y)
{
	PIECE *piece;
	piece = malloc(sizeof(PIECE));
	piece->x_coord = x;
	piece->y_coord = y;
	piece->hasMoved = 0;
	piece->value = shade;
	piece->type = unit;
	return piece;
}

void DeletePiece(PIECE *piece)
{
	assert(piece);
	free(piece);
}

void move(GAME *game, unsigned int x_current, unsigned int y_current, unsigned int x_next, unsigned int y_next)
{
	PIECE *piece = game -> Board[x_current][y_current];

	if(CastleSelected(piece, x_next, y_next))
		Castle(game, x_current, y_current, x_next, y_next);

	else if(en_passantSelected(game -> Board, game -> Board[x_current][y_current], x_next, y_next))
		en_Passant_Maneuver(game, x_current, y_current, x_next, y_next);

	else {
		if (game->Board[x_next][y_next])  /* eating*/
		{
			LogMove(game -> List, game -> Board[x_next][y_next], 1, 0, x_next, y_next); /* records the eating */
			/* DeletePiece(game -> Board[x_next][y_next]); */
		}
		/* printf("Test"); */
		LogMove(game -> List, piece, 0, 0, x_current, y_current);
		game -> Board[x_current][y_current] = NULL;
		game -> Board[x_next][y_next] = piece;
		piece -> x_coord = x_next;
		piece -> y_coord = y_next;
		piece -> hasMoved++;
	}
}

int Try(GAME *game, PIECE *King, unsigned int x_current, unsigned int y_current, unsigned int x_next, unsigned int y_next) {
	PIECE *piece = game -> Board[x_current][y_current];
	PIECE *temp = NULL;
	int output = 0;

	if(CastleSelected(game -> Board[x_current][y_current], x_next, y_next)) {
		Castle(game, x_current, y_current, x_next, y_next);
		output = !LookForCheck(game -> Board, King, King -> x_coord, King -> y_coord, game -> List);
		Undo(game -> Board, game -> List);
	}

	else if(en_passantSelected(game -> Board, game -> Board[x_current][y_current], x_next, y_next)) {
		en_Passant_Maneuver(game, x_current, y_current, x_next, y_next);
		output = !LookForCheck(game -> Board, King, King -> x_coord, King -> y_coord, game -> List);
		Undo(game -> Board, game -> List);
	}

	else {
		if (game->Board[x_next][y_next])  /* eating*/
			temp = game -> Board[x_next][y_next];

		game -> Board[x_current][y_current] = NULL;
		game -> Board[x_next][y_next] = piece;
		piece -> x_coord = x_next;
		piece -> y_coord = y_next;

		output = !LookForCheck(game -> Board, King, King -> x_coord, King -> y_coord, game -> List);

		game -> Board[x_current][y_current] = game -> Board[x_next][y_next];
		piece -> x_coord = x_current;
		piece -> y_coord = y_current;


		game -> Board[x_next][y_next] = temp;
	}

	return output;
}

void Castle(GAME *game, unsigned int x_current, unsigned int y_current, unsigned int x_next, unsigned int y_next) {
	unsigned int i, x_rook, y_rook;
	int change;

	change = (x_next > x_current) ? 1 : -1;
	y_rook = y_current;

	for(i = x_current + change; i >= 0 && i < 8; i += change) {
		if(game -> Board[i][y_rook] && game -> Board[i][y_rook] -> type == rook) {
			x_rook = i;
			break;
		}
	}

	LogMove(game -> List, game -> Board[x_rook][y_rook], 0, 1, x_rook, y_rook);
	game -> Board[x_rook][y_rook] -> hasMoved++;
	game -> Board[x_rook][y_rook] -> x_coord = x_next - change;
	game -> Board[x_rook][y_rook] -> y_coord = y_rook;
	game -> Board[x_next - change][y_rook] = game -> Board[x_rook][y_rook];
	game -> Board[x_rook][y_rook] = NULL;

	LogMove(game -> List, game -> Board[x_current][y_current], 0, 0, x_current, y_current);
	game -> Board[x_current][y_current] -> hasMoved++;
	game -> Board[x_current][y_current] -> x_coord = x_next;
	game -> Board[x_current][y_current] -> y_coord = y_next;
	game -> Board[x_next][y_next] = game -> Board[x_current][y_current];
	game -> Board[x_current][y_current] = NULL;
}

void en_Passant_Maneuver(GAME *game, unsigned int x_current, unsigned int y_current, unsigned int x_next, unsigned int y_next) {
	PIECE *piece_1, *piece_2;

	piece_1 = game -> Board[x_current][y_current];
	piece_2 = game -> Board[x_next][y_current];

	LogMove(game -> List, piece_2, 0, 2, x_next, y_current);
	game -> Board[x_next][y_current] = NULL;

	LogMove(game -> List, piece_1, 0, 0, x_current, y_current);
	game -> Board[x_next][y_next] = piece_1;
	piece_1 -> x_coord = x_next;
	piece_1 -> y_coord = y_next;
	game -> Board[x_current][y_current] = NULL;
}

int CastleSelected(PIECE *piece, unsigned int x_coord, unsigned int y_coord) {
	if(piece -> type == king)
		if(y_coord == piece -> y_coord)
			if((x_coord == piece -> x_coord + 2) || (x_coord == piece -> x_coord - 2))
				return 1;
	return 0;
}

int  en_passantSelected(PIECE *board[8][8], PIECE *piece, unsigned int x, unsigned int y) {
	if(piece -> type == pawn)
		if(x != piece -> x_coord)
			if(!board[x][y])
				return 1;
	return 0;
}

int PromptPromotion(PIECE *piece) {
	if(piece -> type == pawn)
		if(piece -> y_coord == 0 || piece -> y_coord == 7)
			return 1;

	return 0;
}
