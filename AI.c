#include "AI.h"

/*function tabulates scores from current board,
 * defend, and underAttack functions and returns 
 * to main
 **/
int final_board_score(GAME *AIgame, color AI)
{

	unsigned int x, y, a;
	int score;
	score =0;

	score = readCurrentBoard(AIgame, AI);

	score += defendPieceValue(AIgame, AI);
	
	score -= pieceUnderAttackValue(AIgame, AI);


	/*checkPawnPosition*/

	if(AI == white)
	{
		for(x = 0; x<8; x++) /*pawn at the 2nd and 7th row*/
		{

			a = 6;
			if((AIgame->Board[x][a])&& (AIgame->Board[x][a]->value == white) && (AIgame->Board[x][a]->type == pawn))
			{
				if(!LookForCheck(AIgame->Board, AIgame->Board[x][a], x, a, AIgame -> List))
				{
					score += 100;
				}
			}
			a = 1;
			if((AIgame->Board[x][a])&& (AIgame->Board[x][a]->value == black) && (AIgame->Board[x][a]->type == pawn))
			{
				if(!LookForCheck(AIgame->Board, AIgame->Board[x][a], x, a, AIgame -> List))
				{
					score -= 100;
				}
			}

			
		}

		for(y = 0; y<8;y++) /*pawn at the 1st and 7th column*/
		{
			a = 0;
			if((AIgame->Board[a][y]) &&(AIgame->Board[a][y]->value == white) && (AIgame->Board[a][y]->type == pawn))
				{
					score -=15;		
				}
			a = 7;
			if((AIgame->Board[a][y])&&(AIgame->Board[a][y]->value == white) && (AIgame->Board[a][y]->type == pawn))
				{
					score -=15;
				}
			a = 0;
			if((AIgame->Board[a][y]) &&(AIgame->Board[a][y]->value == black) && (AIgame->Board[a][y]->type == pawn))
				{
					score +=15;		
				}
			a = 7;
			if((AIgame->Board[a][y])&&(AIgame->Board[a][y]->value == black) && (AIgame->Board[a][y]->type == pawn))
				{
					score +=15;
				}			
		}	
		/*Check for castling*/
		/*if(AIgame ->Board [3][0] && AIgame ->Board[3][0] -> value == white && AIgame ->Board[3][0] ->type == king)
		{
			if(!(AIgame ->Board[0][0] ->type == rook)) 
	 		{
				score -= 10;
			}
			if(!(AIgame ->Board[7][0] ->type == rook))
			{
				score -=10;
			}
		}
		if(AIgame ->Board [4][7] && AIgame ->Board[4][7] -> value == black && AIgame ->Board[4][7] ->type == king)
		{
			if(!(AIgame ->Board[0][7] ->type == rook))
			{
				score +=10;
			} 
			if(!(AIgame ->Board[7][7] ->type == rook))
			{
				score += 10;
			}
		}
	*/
	}

	if(AI == black)
	{
		for(x = 0; x<8; x++) /*pawn at the 2nd and 7th row*/
		{
			a = 6;
			if((AIgame->Board[x][a])&& (AIgame->Board[x][a]->value == white) && (AIgame->Board[x][a]->type == pawn))
			{
				if(!LookForCheck(AIgame->Board, AIgame->Board[x][a], x, a, AIgame -> List))
				{
					score -= 100;
				}
			}
			a = 1;
			if((AIgame->Board[x][a])&& (AIgame->Board[x][a]->value == black) && (AIgame->Board[x][a]->type == pawn))
			{
				if(!LookForCheck(AIgame->Board, AIgame->Board[x][a], x, a, AIgame -> List))
				{
					score += 100;
				}
			}

			
		}

		for(y = 0; y<8;y++) /*pawn at the 1st and last column*/
		{
			a = 0;
			if((AIgame->Board[a][y]) &&(AIgame->Board[a][y]->value == white) && (AIgame->Board[a][y]->type == pawn))
				{
					score +=15;		
				}
			a = 7;
			if((AIgame->Board[a][y])&&(AIgame->Board[a][y]->value == white) && (AIgame->Board[a][y]->type == pawn))
				{
					score +=15;
				}
			a = 0;
			if((AIgame->Board[a][y]) &&(AIgame->Board[a][y]->value == black) && (AIgame->Board[a][y]->type == pawn))
				{
					score -=15;		
				}
			a = 7;
			if((AIgame->Board[a][y])&&(AIgame->Board[a][y]->value == black) && (AIgame->Board[a][y]->type == pawn))
				{
					score -=15;
				}

			
		}

		/*Check for Castling*/
		/*if(AIgame ->Board [3][0] && AIgame ->Board[3][0] -> value == white && AIgame ->Board[3][0] ->type == king)
		{
			if(!(AIgame ->Board[0][0] ->type == rook))  
			{
				score += 10;
			}
			if(!(AIgame ->Board[7][0] ->type == rook))
			{
				score += 10;
			}
		}
		if(AIgame ->Board [4][7] && AIgame ->Board[4][7] -> value == black && AIgame ->Board[4][7] ->type == king)
		{
			if(!(AIgame ->Board[0][7] ->type == rook)) 
			{
				score -= 10;
			}
			if(!(AIgame ->Board[7][7] ->type == rook))
			{
				score -=10;
			}
		}*/
		
	}

	return score;
}

int AI_Main(GAME *AIgame, GAME *game, color AI)
{
	int score, maxScore, minScore, iLose, iFine, scoreInCase, startScore;
	iLose = 0;
	iFine = 0;

	unsigned int  x, y, x_Decision_initial, y_Decision_initial, x_Decision_move, y_Decision_move,x_Original, y_Original, x_next, y_next;
	unsigned int  x_initial_incase, y_initial_incase, x_move_incase, y_move_incase;
	LPMOVES *listMove;
	PMOVE *possibleMove;	
	PIECE *piece;	
	minScore = INT_MAX;
	startScore = final_board_score(AIgame,AI);
	maxScore = INT_MIN;
	
	for(y=0;y<8;y++)
		for(x = 0;x<8;x++)
		{
			piece = AIgame->Board[x][y];
			if(AI == white)
			{
				if(piece && piece ->value == white)
				{
					AIgame -> PossibleMoves = FindPossibleMoves(AIgame, piece);
					if(AIgame -> PossibleMoves ->Length == 0){		
						continue;
					}
					listMove = CreateListOfPossibleMoves();
					CopyPossibleMoveList(AIgame->PossibleMoves, listMove);
					possibleMove = listMove->First;
					while(possibleMove != NULL)
					{
						iLose = 0;
						move(AIgame, x, y, possibleMove->x, possibleMove->y);
						score = final_board_score(AIgame, AI);
						scoreInCase = score - minScore;
						if(Checkmate(AIgame, black) ==0) /*If CheckMate, use it*/
						{
							Undo(AIgame -> Board, AIgame -> List);
							x_Decision_initial = x;
							y_Decision_initial = y;
							x_Decision_move = possibleMove->x;
							y_Decision_move =possibleMove->y;


							DeleteListOfPossibleMoves(listMove);
							return 0;
						}
						if(maxScore < score)
						{
						
						x_Original = x;
						y_Original = y;
						x_next = possibleMove->x;
						y_next = possibleMove->y;
						x_Decision_initial = x_Original;
						y_Decision_initial = y_Original;
						x_Decision_move = x_next;
						y_Decision_move =y_next;
						maxScore = score;

						}
						if(maxScore == score)
						{
							srand(time(NULL));
							int r = rand()%2;
							if(!r)
							{
								x_Original = x;
								y_Original = y;
								x_next = possibleMove->x;
								y_next = possibleMove->y;
								x_Decision_initial = x_Original;
								y_Decision_initial = y_Original;
								x_Decision_move = x_next;
								y_Decision_move =y_next;
								maxScore = score;
							}
						}
						if(minScore <= score && score< maxScore)
						{
							minScore = score;
							x_initial_incase = x;
							y_initial_incase =y;
							x_move_incase = possibleMove->x;
							y_move_incase = possibleMove ->y;
							
						}
						

				/* PASTE HERE */


						
						Undo(AIgame ->Board, AIgame -> List);						
						possibleMove = possibleMove->Next;
					}
				}
				
			}
			if(AI == black)
			{
				if(piece && piece ->value == black)
				{
					AIgame -> PossibleMoves = FindPossibleMoves(AIgame, piece);
					if(AIgame -> PossibleMoves ->Length == 0){		
						continue;
					}
					listMove = CreateListOfPossibleMoves();
					CopyPossibleMoveList(AIgame->PossibleMoves, listMove);
					possibleMove = listMove->First;
					while(possibleMove != NULL)
					{
						iLose = 0;
						move(AIgame, x, y, possibleMove->x, possibleMove->y);
						score = final_board_score(AIgame, AI);
						scoreInCase = score - minScore;
						if(Checkmate(AIgame, black) ==0) /*If CheckMate, use it*/
						{
							Undo(AIgame -> Board, AIgame -> List);
							x_Decision_initial = x;
							y_Decision_initial = y;
							x_Decision_move = possibleMove->x;
							y_Decision_move =possibleMove->y;


							DeleteListOfPossibleMoves(listMove);
							return 0;
						}
						if(maxScore < score)
						{
						
						x_Original = x;
						y_Original = y;
						x_next = possibleMove->x;
						y_next = possibleMove->y;
						x_Decision_initial = x_Original;
						y_Decision_initial = y_Original;
						x_Decision_move = x_next;
						y_Decision_move =y_next;
						maxScore = score;

						}
						if(maxScore == score)
						{
							srand(time(NULL));
							int r = rand()%2;
							if(!r)
							{
								x_Original = x;
								y_Original = y;
								x_next = possibleMove->x;
								y_next = possibleMove->y;
								x_Decision_initial = x_Original;
								y_Decision_initial = y_Original;
								x_Decision_move = x_next;
								y_Decision_move =y_next;
								maxScore = score;
							}
						}
						if(minScore <= score && score< maxScore)
						{
							minScore = score;
							x_initial_incase = x;
							y_initial_incase =y;
							x_move_incase = possibleMove->x;
							y_move_incase = possibleMove ->y;
							
						}
						

				/* PASTE HERE */


						
						Undo(AIgame ->Board, AIgame -> List);						
						possibleMove = possibleMove->Next;
					}
				}

			}


		}
		
		printf("It is max. Moving from (%d, %d) to (%d, %d)\n", x_Decision_initial, y_Decision_initial, x_Decision_move, y_Decision_move);

		DeleteListOfPossibleMoves(listMove);
		move(game, x_Decision_initial, y_Decision_initial, x_Decision_move, y_Decision_move);
		move(AIgame, x_Decision_initial, y_Decision_initial, x_Decision_move, y_Decision_move);
		return 1;
}

int readCurrentBoard(GAME *game, color player)
{
	int pawnValue[8][8] = {{0,50,10,5,0,5,5,0}, {0,50,10,5,0,-5,10,0}, 
			   {0,50,20,10,0,-10,10,0}, {0,50,30,27,25,0,-25,0}, 
			   {0,50,30,27,25,0,-25,0}, {0,50,20,10,0,-10,10,0},
			   {0,50,10,5,0,-5,10,0}, {0,50,10,5,0,5,5,0}};

	int knightValue[8][8] = {{-50,-40,-30,-30,-30,-30,-40,-50}, {-40,-20,0,5,0,5,-20,-40},
			{-30,0,10,15,15,10,0,-20}, {-30,0,15,20,20,15,5,-30},
			{-30,0,15,20,20,15,5,-30}, {-30,0,10,15,15,10,0,-20},
			{-40,-20,0,5,0,5,-20,-40}, {-50,-40,-30,-30,-30,-30,-40,-50}};

	int bishopValue[8][8] = {{-20,-10,-10,-10,-10,-10,-10,-20}, {-10,0,0,5,0,10,5,-10},
			{-10,0,5,5,10,10,0,-40}, {-10,0,10,10,10,10,0,-10},
			{-10,0,10,10,10,10,0,-10}, {-10,0,5,5,10,10,0,-40},
			{-10,0,0,5,0,10,5,-10}, {-20,-10,-10,-10,-10,-10,-10,-20}};

	int kingValue[8][8] = {{-30,-30,-30,-30,-20,-10,20,20}, {-40,-40,-40,-40,-30,-20,20,30},
			{-40,-40,-40,-40,-30,-20,0,10}, {-50,-50,-50,-50,-40,-20,0,0},
			{-50,-50,-50,-50,-40,-20,0,0}, {-40,-40,-40,-40,-30,-20,0,10},
			{-40,-40,-40,-40,-30,-20,20,30}, {-30,-30,-30,-30,-20,-10,20,20}};
	
	int kingEndValue[8][8] ={{-50,-30,-30,-30,-30,-30,-30,-50},{-40,-20,-10,-10,-10,-10,-30,-30},
			{-30,-10,20,30,30,20,0,-30},{-20,0,30,40,40,30,0,-30},
				{-20,0,30,40,40,30,0,-30},{-30,-10,20,30,30,20,0,-30},
				{-40,-20,-10,-10,-10,-10,-30,-30},{-50,-30,-30,-30,-30,-30,-30,-50}};

        int x, y, a, b, endGameCheck, numpiece;
        int blackScore = 0;
        int whiteScore = 0;
        PIECE *piece;
		endGameCheck = 0;
		numpiece =0;
		for(y = 0; y < 8; y++)
                for(x = 0; x < 8; x++)
				{
					if(game->Board[x][y])
					{
						numpiece +=1;
					}
				}
		if(numpiece <=10)
		{
			endGameCheck = 1;
		}

        for(y = 0; y < 8; y++)
                for(x = 0; x < 8; x++)
                {
                        piece = game->Board[x][y];

                        if(piece == NULL)
                                continue;

                        if(piece->value == black)
                        {
                                a = 7 - x;
                                b = y;
                                if(piece->type == pawn)
                                        blackScore += pawnValue[a][b] + 100;
                                if(piece->type == rook)
                                        blackScore += 525;
                                if(piece->type == knight)
                                        blackScore += knightValue[a][b] + 350;
                                if(piece->type == queen)
                                        blackScore += 1000;
                                if(piece->type == king && !endGameCheck)
                                        blackScore += kingValue[a][b] + 32767;
								if(piece ->type == king && endGameCheck)
										blackScore +=kingEndValue[a][b] + 32767; 
                                if(piece->type == bishop)
                                        blackScore += bishopValue[a][b] + 350;
                        }
                        if(piece ->value ==white)
                        {
                                a = x;
                                b = 7 - y;
                                if(piece->type == pawn)
                                        whiteScore += pawnValue[a][b] + 100;
                                if(piece->type == rook)
                                        whiteScore += 525;
                                if(piece->type == knight)
                                        whiteScore += knightValue[a][b] + 350;
                                if(piece->type == queen)
                                        whiteScore += 1000;
                                if(piece->type == king && !endGameCheck)
                                        whiteScore += kingValue[a][b] + 32767;
								if(piece ->type == king && endGameCheck)
										whiteScore += kingEndValue[a][b]+32767;
                                if(piece->type == bishop)
                                        whiteScore += bishopValue[a][b] + 350;
                        }
                }

     
        if(player == white) {
                return (whiteScore - blackScore);
	}
        else
		return (blackScore - whiteScore);
	
}

int defendPieceValue(GAME *AIgame, color player)
{
        unsigned int x, y, x_assume, y_assume;
	int whiteScore, blackScore;
	PIECE *piece;
	LPMOVES *listMove;
	PMOVE *possibleMove;
        for(y = 0; y < 8; y++)
                for(x = 0; x < 8; x++)
                {

/*

			if(AIgame ->Board[x][y] && AIgame ->Board[x][y] ->type == pawn)
			{
				piece = AIgame ->Board[x][y];


				if(piece -> value == white)
				{
					if( ((x+1) <=7) && ((y+1) <=7) && AIgame ->Board[x+1][y+1] && AIgame ->Board[x+1][y+1] ->value == white && AIgame ->Board[x+1][y+1]) 
						{
							whiteScore +=6;
						} 
					if(((x-1) >=0) && ((y+1) <=7) && AIgame ->Board[x-1][y+1] && AIgame ->Board[x-1][y+1] ->value == white && AIgame ->Board[x-1][y+1])
						{
							whiteScore +=6;
						}
										
				}
				if(piece ->value == black)
				{	
					if(((x+1) <=7) && ((y-1) >=0) && AIgame ->Board[x+1][y-1] ->value == black && AIgame ->Board[x+1][y-1]) 
						{
							blackScore +=6;
						} 
					if(((x-1) >=7) && ((y-1) >=0) && AIgame ->Board[x-1][y-1] ->value == black && AIgame ->Board[x-1][y-1])
						{
							blackScore +=6;
						}
				}
				
			}
*/
                    	if((AIgame ->Board[x][y]) && (AIgame ->Board[x][y]-> type != pawn) && (AIgame ->Board[x][y] ->value == white))
			{

				AIgame ->Board[x][y] ->value = black;
				AIgame -> PossibleMoves = FindPossibleMoves(AIgame, AIgame ->Board[x][y]);
				listMove = CreateListOfPossibleMoves();
				CopyPossibleMoveList(AIgame ->PossibleMoves, listMove);
				AIgame ->Board[x][y]->value = white;


				if(listMove->Length == 0)
				{	
					DeleteListOfPossibleMoves(listMove);
					continue;
				}
				possibleMove = listMove -> First;
				while(possibleMove!=NULL)
				{	
					x_assume =possibleMove ->x;
					y_assume=possibleMove->y;
					piece = AIgame ->Board[x][y];
					if(AIgame->Board[x_assume][y_assume] != NULL)
					{
					
						if((piece->type == knight) || (piece->type == bishop))
						{
							whiteScore += 3; 
						}
						if(piece->type == rook)
						{
							whiteScore += 2; 
						}
						if((piece->type == queen) || (piece->type == king))
						{
							whiteScore += 1; 
						}

			
					}
					possibleMove = possibleMove->Next;
				}
			
				DeleteListOfPossibleMoves(listMove);	
			}

		
			if((AIgame ->Board[x][y]) && (AIgame ->Board[x][y] ->value == black) && (AIgame ->Board[x][y] ->type != pawn))
			{
				AIgame ->Board[x][y] ->value = white;
				AIgame -> PossibleMoves = FindPossibleMoves(AIgame, AIgame ->Board[x][y]);
				listMove = CreateListOfPossibleMoves();
				CopyPossibleMoveList(AIgame ->PossibleMoves, listMove);
				AIgame->Board[x][y] ->value = black;
				if(listMove->Length == 0)
				{	
					DeleteListOfPossibleMoves(listMove);
					continue;
				}
				possibleMove = listMove -> First;
				while(possibleMove!=NULL)
				{	
					x_assume =possibleMove ->x;
					y_assume=possibleMove->y;
					piece = AIgame ->Board[x][y];
					if(AIgame->Board[x_assume][y_assume] != NULL)
					{
					
						if((piece->type == knight) || (piece->type == bishop))
						{
							blackScore += 3; 
						}
						if(piece->type == rook)
						{
							blackScore += 2; 
						}
						if((piece->type == queen) || (piece->type == king))
						{
							blackScore += 1; 
						}

					}
					possibleMove = possibleMove->Next;
				}
			
				piece->value = black;
				DeleteListOfPossibleMoves(listMove);	
			}
		
                }
	if(player == white)
		{
        		return (whiteScore - blackScore);
		}
	return (blackScore - whiteScore);

}

int pieceUnderAttackValue(GAME *AIgame, color player)
{
	unsigned int x, y;
	int whiteScore, blackScore;
	for(y = 0; y<8; y++)
		for(x = 0; x<8;x++)
		{
			
				if((AIgame ->Board[x][y]) && (AIgame -> Board[x][y] -> value == white))
				{
					whiteScore += LookForCheck(AIgame->Board, AIgame->Board[x][y], x, y, AIgame -> List);
				}
			
			
				if((AIgame ->Board[x][y]) && (AIgame -> Board[x][y] -> value == black))
				{
					blackScore+= LookForCheck(AIgame->Board, AIgame->Board[x][y], x, y, AIgame -> List);
				}
			

			
		}		

	if(player == white)
		{
        		return (whiteScore - blackScore);
		}
	return (blackScore - whiteScore);

}
