all: Chess

CC	= gcc
CFLAGS	= -ansi -Wall -c
LFLAGS	= -Wall
AR	= ar rc
RAN	= ranlib



Piece.o: Piece.c Piece.h Structures.h
	$(CC) $(CFLAGS) Piece.c -o Piece.o

Game.o: Game.c Game.h Structures.h
	$(CC) $(CFLAGS) Game.c -o Game.o

MoveList.o: MoveList.c MoveList.h Structures.h
	$(CC) $(CFLAGS) MoveList.c -o MoveList.o

PossibleMoves.o: PossibleMoves.c PossibleMoves.h Structures.h
	$(CC) $(CFLAGS) PossibleMoves.c -o PossibleMoves.o

AI.o: AI.c AI.h Structures.h Piece.h MoveList.h PossibleMoves.h
	$(CC) $(CFLAGS) AI.c -o AI.o

Chess.o: Chess.c
	$(CC) $(CFLAGS) Chess.c -o Chess.o

Chess: Piece.o Game.o MoveList.o PossibleMoves.o Chess.o AI.o
	$(CC) $(LFLAGS) Piece.o Game.o MoveList.o PossibleMoves.o Chess.o AI.o -o Chess

clean:
	rm -f Chess *.o
