#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "AI.h"

/* CONSTANTS */
#define CHAR_SIZE 10;
char ConvertInt(int x);
void PrintBoard(GAME *game);
void PrintPossibleMoves(LPMOVES *list);
int CheckPossibleMoves(LPMOVES *list, int x, int y);
int Checkmate(GAME *game, color Color);
int CheckInput(char str[]);
unit_type Promotion();

int main() {
	GAME *game, *AIgame; /* Make real game & AI game */
	PIECE *piece, *AIpiece;
	char response[10];
	int coordinates[2];
	int i, position, bw, choice;
	int quit, question, turn, randForAI_Main;
	color current, colorAI;
	unit_type selection;

	quit = 0;


	game = CreateGame();
	AIgame = CreateGame();


	printf("Would you like an PvP(0) or vs AI(1) game?\n");
	scanf("%d", &choice);

	if(choice) {
		AIgame = CreateGame();

		printf("Which color would you like? Black(0) or White(1)?\n");
		scanf("%d", &bw);

		colorAI = bw ? black : white;
	}


	current = white;

	/* printf("Initial Board\n"); */

	PrintBoard(game);

	while(!quit && Checkmate(game, current)) {
		printf("The current color is ");		
		switch(current) {
			case white:	printf("white.\n");
					break;
			default:	printf("black.\n");
		}

		turn = 0;
		question = 0;

		if(choice && colorAI == current) {
			randForAI_Main = AI_Main(AIgame, game, colorAI);
			turn = 1;
		}


		while(!turn) {

			if(!question) {
				printf("Please type the coordinates for your unit.\n");
				scanf("%s", response);
			}

			else if(question == 1) {
				printf("Please select where you would like to move the unit: \n");
				scanf("%s", response);
			}

	
			if(!CheckInput(response)) {
				printf("Invalid selection. Please try again\n");
				question = 0;
			}

			else if(CheckInput(response) == 1) {
				Undo(game -> Board, game -> List);
				if(choice) {
					Undo(game -> Board, game -> List);
					Undo(AIgame -> Board, AIgame -> List);
					Undo(AIgame -> Board, AIgame -> List);
					PrintBoard(game);
				}
				else
					turn = 1;
			}

			else if(CheckInput(response) == 2) {
				quit = 1;
				break;
			}

			else {
				if(!question) {
					position = 0;
					for(i = 0; i < 10; i++) {
						if(position == 2)
							break;
						else if(response[i] >= '0' && response[i] < '8') {
							coordinates[position] = response[i];
							position++;
						}
					}

					if(!game -> Board[(int)coordinates[0]-48][(int)coordinates[1]-48] || (game -> Board[(int)coordinates[0]-48][(int)coordinates[1]-48] -> value != current)) {
						printf("Invalid selection. Please try again\n");
						question = 0;
					}

					else {
						piece = game -> Board[(int)coordinates[0]-48][(int)coordinates[1]-48];
						if(choice)
							AIpiece = AIgame -> Board[(int)coordinates[0]-48][(int)coordinates[1]-48];
								
						printf("Your available positions for this unit are:\n");
						game -> PossibleMoves = FindPossibleMoves(game, piece);
						PrintPossibleMoves(game -> PossibleMoves);
						question++;
					}
				}

				else if(question == 1) {
					position = 0;
					for(i = 0; i < 10; i++) {
						if(position == 2)
							break;
						else if(response[i] >= '0' && response[i] < '8') {
							coordinates[position] = response[i];
							position++;
						}
					}
					if(!CheckPossibleMoves(game -> PossibleMoves, (int)coordinates[0]-48, (int)coordinates[1]-48)) {
						printf("Invalid selection. Please try again\n");
						question = 0;
					}
					else {
						move(game, piece -> x_coord, piece -> y_coord, (int)coordinates[0]-48, (int)coordinates[1]-48);
						if(choice)
							move(AIgame, AIpiece -> x_coord, AIpiece -> y_coord, (int)coordinates[0]-48, (int)coordinates[1]-48);
						if(PromptPromotion(piece)) {
							selection = Promotion();
							piece -> type = selection;
							if(choice)
								AIpiece -> type = selection;
						}

						turn = 1;
					}
				}
			}

		}



		/* printf("Game Board\n"); */

		PrintBoard(game);

		/* printf("AI Board\n");

		PrintBoard(AIgame); */

		current = (current == white) ? black : white;

	}


	/* int test;
	MOVE *test1;
	printf("Undoing. Acknowledge: ");
	scanf("%d", &test);
	Undo(game -> Board, game -> List);
	PrintBoard(game); */

	if(quit)
		printf("Forfeit\n");
	else
		printf("Checkmate\n");



	DeleteGame(game);
	DeleteGame(AIgame);

	return 0;
}

void PrintBoard(GAME *game)
{
	PIECE *piece;
	int i, j;
	char c, t;
	printf(" +--+--+--+--+--+--+--+--+\n");
	for (j = 7; j>=0;j--)
	{
		printf("%d|",j);
		for(i = 0; i < 8;i++)
		{
			piece = game->Board[i][j];
			if(piece == NULL)
			{
				printf("  |");
			}
			else
			{
				if(piece ->value == black)
				{
					c = 'B';
				}
				if(piece->value == white){
					c = 'W';
				}
				if(piece -> type == king)
				{t = 'k';}
				if(piece ->type == queen)
				{t = 'q';}
				if(piece ->type == bishop)
				{t = 'b';}
				if(piece ->type==knight)
				{t= 'n';}
				if(piece->type==rook)
				{t = 'r';}
				if(piece->type ==pawn)
				{t = 'p';}
				printf("%c%c|", c,t);
			}
		}
	printf("\n +--+--+--+--+--+--+--+--+\n");
	}
	printf("  0  1  2  3  4  5  6  7  \n");
}

void PrintPossibleMoves(LPMOVES *list) {
	PMOVE *move;
	int i;

	assert(list);

	move = list -> First;
	for(i = 0; i < list -> Length; i++) {
		printf("(%d, %d) ", move -> x, move -> y);
		move = move -> Next;
	}
	printf("\n");
}

int CheckPossibleMoves(LPMOVES *list, int x, int y) {
	PMOVE *move;
	int i;
	move = list -> First;

	for(i = 0; i < list -> Length; i++) {
		if(x == move -> x && y == move -> y)
			return 1;
		move = move -> Next;
	}
	return 0;
}

int CheckInput(char str[]) {
	int i, position;
	int coordinates[2];;

	position = 0;

	if(str[0] == 'z' || str[0] == 'Z')
		return 1;

	if(str[0] == 'q' || str[0] == 'Q')
		return 2;

	else
		for(i = 0; i < 10; i++) {
			if(position == 2)
				break;
			else if(str[i] >= '0' && str[i] < '8') {
				coordinates[position] = str[i];
				position++;
			}
		}
	if(position == 2)
		return 3;

	return 0;

}

unit_type Promotion() {
	int choice;
	unit_type selection;
	printf("What would you like to promote your pawn to?\n1 for queen\n2 for bishop\n3 for knight\n4 for rook\n");
	do{
		scanf("%d", &choice);
	} while(choice < 1 || choice > 4);
	switch(choice) {
		case 1:	selection = queen;
			break;
		case 2: selection = bishop;
			break;
		case 3: selection = knight;
			break;
		default: selection = rook;
	}

	return selection;
}
