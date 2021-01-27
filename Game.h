#ifndef GAME_H
#define GAME_H

#include "MoveList.h"
#include "Piece.h"
#include "Structures.h"
#include "PossibleMoves.h"

GAME *CreateGame();
void DeleteGame(GAME *game);
void SetBoard(GAME *game, PIECE *Board[8][8]);
int Checkmate(GAME *game, color Color);

#endif
