#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>

#define MATRIX_SIZE 10
#define NUM_SHIPS_PER_PLAYER 5
#define BOARD_SIZE 10

typedef enum {
    WATER,
    WATER_SHOT,
    BOAT,
    WRECK
} CellType;

typedef struct {
    int size;
    int x, y;
    char orientation;
} Ship;

typedef struct {
    CellType **matrix;
    int size;
    Ship *ships;
    int numShips;
} Plateau;

typedef struct {
    Plateau *plateaux[2];
    Ship *flottilles[2];
    int numJoueurs;
} Jeu;

void initializeBoard(Plateau *p);
void displayBoard(Plateau *p);
void freeBoard(Plateau *p);
Ship *createShip(int size, int x, int y, char orientation);
bool isShipAlive(const Ship *ship);
bool canPlaceShip(const Plateau *plateau, int x, int y, int size, char orientation);
void placeShip(Plateau *plateau, const Ship *ship);
void createGame(Jeu *game);
CellType performShot(Plateau *plateau, int x, int y);
void displayGameBoard(const Plateau *plateau, bool playerView);
void displayPlayerGameState(const Jeu *game, int playerIndex);
void displayOpponentGameState(const Jeu *game, int playerIndex);
void executePlayerTurn(Jeu *game, int currentPlayerIndex);
void executeComputerTurn(Jeu *game, int computerPlayerIndex);
void playGame(Jeu *game);
bool isGameFinished(const Jeu *game);
int determineWinner(const Jeu *game);
void freeGame(Jeu *game);

#endif
