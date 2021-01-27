#include "Game.h"

GAME *CreateGame() {
	GAME *game;
	game = malloc(sizeof(GAME));
	game -> List = CreateList();
	game -> PossibleMoves = CreateListOfPossibleMoves();
	SetBoard(game, game -> Board);

	game -> WhiteKing = game -> Board[4][0];
	game -> BlackKing = game -> Board[4][7];

	return game;
}

void DeleteGame(GAME *game) {
	assert(game);
	int x;
	DeleteList(game -> List);

	for(x = 0; x < 32; x++)
		DeletePiece(game -> ListOfPieces[x]);

	/* for(j = 0; j < 8; j++) {
		for(i = 0; i < 8; i++) {
			if(game -> Board[i][j]) {
				DeletePiece(game -> Board[i][j]);
			}
		}
	} */
	DeleteListOfPossibleMoves(game -> PossibleMoves);

	free(game);
}

int Checkmate(GAME *game, color Color){
	int x, y;

	for (x=0; x<8; x++)
		for (y=0; y<8; y++){
			if (game -> Board[x][y] && game->Board[x][y]->value == Color){ /* whether color of that piece matches*/
				game -> PossibleMoves = FindPossibleMoves(game, game->Board[x][y]);
				if (game -> PossibleMoves -> Length){
					return 1;
				}
			}
		}
	return 0;
}

void SetBoard(GAME *game, PIECE *Board[8][8])
{
	PIECE *piece;
	int i, j;
	int x;
	x=0;
	for(j=2; j<6; j++)
	{
		for(i=0; i<8; i++)
		{
			Board[i][j] = NULL;
		}
	}
	piece = CreatePiece(white, rook, 0, 0);
	Board[0][0] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(white, knight, 1, 0);
	Board[1][0] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(white, bishop, 2, 0);
	Board[2][0] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(white, queen, 3, 0);
	Board[3][0] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(white, king, 4, 0);
	Board[4][0] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(white, bishop, 5, 0);
	Board[5][0] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(white, knight, 6, 0);
	Board[6][0] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(white, rook, 7, 0);
	Board[7][0] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	for( i=0; i<8; i++)
	{
		j = 1;
		piece = CreatePiece(white, pawn, i, j);
		Board[i][j] = piece;
		game -> ListOfPieces[x] = piece;
		x++;
	}
	piece = CreatePiece(black, rook, 0, 7);
	Board[0][7] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(black, knight, 1, 7);
	Board[1][7] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(black, bishop, 2, 7);
	Board[2][7] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(black, queen, 3, 7);
	Board[3][7] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(black, king, 4, 7);
	Board[4][7] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(black, bishop, 5, 7);
	Board[5][7] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(black, knight, 6, 7);
	Board[6][7] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	piece = CreatePiece(black, rook, 7, 7);
	Board[7][7] = piece;
	game -> ListOfPieces[x] = piece;
	x++;
	for( i=0; i<8; i++)
	{
		j = 6;
		piece = CreatePiece(black, pawn, i, j);
		Board[i][j] = piece;
		game -> ListOfPieces[x] = piece;
	x++;
	}
}
