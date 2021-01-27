#ifndef AI_H
#define AI_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "Game.h"

int final_board_score(GAME *AIgame, color AI);
int readCurrentBoard(GAME *game, color player);
int pieceUnderAttackValue(GAME *game, color player);
int defendPieceValue(GAME *game, color player);
int AI_Main(GAME *AIgame, GAME *game, color AI);

#endif
