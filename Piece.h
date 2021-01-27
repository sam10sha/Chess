#ifndef PIECE_H
#define PIECE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Structures.h"
#include "MoveList.h"

PIECE *CreatePiece(color shade, unit_type unit, unsigned int x, unsigned int y);

void DeletePiece(PIECE *piece);

void move(GAME *game, unsigned int x_current, unsigned int y_current, unsigned int x_next, unsigned int y_next);

int Try(GAME *game, PIECE *King, unsigned int x_current, unsigned int y_current, unsigned int x_next, unsigned int y_next);

void Castle(GAME *game, unsigned int x_current, unsigned int y_current, unsigned int x_next, unsigned int y_next);

void en_Passant_Maneuver(GAME *game, unsigned int x_current, unsigned int y_current, unsigned int x_next, unsigned int y_next);

int CastleSelected(PIECE *piece, unsigned int x_coord, unsigned int y_coord);

int  en_passantSelected(PIECE *board[8][8], PIECE *piece, unsigned int x, unsigned int y);

int PromptPromotion(PIECE *piece);

#endif
