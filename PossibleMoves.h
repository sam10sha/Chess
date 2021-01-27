#ifndef POSSIBLEMOVES_H
#define POSSIBLEMOVES_H

#include "Game.h"
#include "Structures.h"

PMOVE *CreatePossibleMove(unsigned int i, unsigned int j);

LPMOVES *CreateListOfPossibleMoves();

void DeletePossibleMove(LPMOVES *list);

void DeleteListOfPossibleMoves(LPMOVES *list);

void AppendPossibleMove(LPMOVES *list, PMOVE *pmove);

LPMOVES *FindPossibleMoves(GAME *game, PIECE *piece);

void MovesForKing(PIECE *board[8][8], PIECE *piece, LPMOVES *list);

void CheckCastle(PIECE *board[8][8], PIECE *piece, LPMOVES *list);

void MovesForQueen(PIECE *board[8][8], PIECE *piece, LPMOVES *list);

void MovesForBishop(PIECE *board[8][8], PIECE *piece, LPMOVES *list);

void MovesForKnight(PIECE *board[8][8], PIECE *piece, LPMOVES *list);

void MovesForRook(PIECE *board[8][8], PIECE *piece, LPMOVES *list);

void MovesForPawn(PIECE *board[8][8], PIECE *piece, LPMOVES *list, LMOVES *history);

void en_passant(PIECE *board[][8], PIECE *piece, LMOVES *moveList, LPMOVES *list);

/* Returns 1 if the position is safe, 0 otherwise */
int LookForCheck(PIECE *board[8][8], PIECE *piece, unsigned int x, unsigned int y, LMOVES *history);

LPMOVES *RefineForCheck(GAME *game, PIECE *King, PIECE *piece, LPMOVES *list);

void CopyPossibleMoveList(LPMOVES *Original, LPMOVES *Copy);

#endif
