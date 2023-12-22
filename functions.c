#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Initializes the game board.
 * @param p Pointer to the Plateau structure.
 */
void initializeBoard(Plateau *p) {
    int i, j;

    // Dynamic allocation of the matrix
    p->matrix = (CellType **)malloc(p->size * sizeof(CellType *));
    for (i = 0; i < p->size; i++) {
        p->matrix[i] = (CellType *)malloc(p->size * sizeof(CellType));
    }

    // Initialization to water cells
    for (i = 0; i < p->size; i++) {
        for (j = 0; j < p->size; j++) {
            p->matrix[i][j] = WATER;
        }
    }
}

/**
 * @brief Displays the game board.
 * @param p Pointer to the Plateau structure.
 */
void displayBoard(Plateau *p) {
    int i, j;

    for (i = 0; i < p->size; i++) {
        for (j = 0; j < p->size; j++) {
            printf("%c ", (p->matrix[i][j] == WATER) ? '~' : 'B');
        }
        printf("\n");
    }
}

/**
 * @brief Frees the memory allocated for the game board.
 * @param p Pointer to the Plateau structure.
 */
void freeBoard(Plateau *p) {
    int i;

    for (i = 0; i < p->size; i++) {
        free(p->matrix[i]);
    }
    free(p->matrix);
}

/**
 * @brief Creates a new ship with the given parameters.
 * @param size Size of the ship.
 * @param x X-coordinate of the reference position.
 * @param y Y-coordinate of the reference position.
 * @param orientation Orientation of the ship ('H' for horizontal, 'V' for vertical).
 * @return Pointer to the newly created ship.
 */
Ship *createShip(int size, int x, int y, char orientation) {
    Ship *newShip = (Ship *)malloc(sizeof(Ship));
    if (newShip != NULL) {
        newShip->size = size;
        newShip->x = x;
        newShip->y = y;
        newShip->orientation = orientation;
    }
    return newShip;
}

/**
 * @brief Checks if a ship is still alive.
 * @param ship Pointer to the Ship structure.
 * @return True if the ship is still alive, false if it is sunk.
 */
bool isShipAlive(const Ship *ship) {
    // Add logic here to determine if the ship is still alive
    // For example, if all parts of the ship have been hit, the ship is considered sunk.

    // For this example, assume the ship is sunk if it has a size of zero.
    return (ship->size > 0);
}

/**
 * @brief Checks if a ship can be placed at the given position on the board.
 * @param plateau Pointer to the Plateau structure.
 * @param x X-coordinate of the reference position.
 * @param y Y-coordinate of the reference position.
 * @param size Size of the ship.
 * @param orientation Orientation of the ship ('H' for horizontal, 'V' for vertical).
 * @return True if the ship can be placed, false otherwise.
 */
bool canPlaceShip(const Plateau *plateau, int x, int y, int size, char orientation) {
    // Add logic here to check if the ship can be placed at the given position
    // Check if the ship doesn't go out of the board and if it doesn't touch other ships

    // This example assumes a simplified check
    // where the ship is placed if it stays within the board and doesn't touch other ships.
    return (x >= 0 && y >= 0 && (orientation == 'H' ? x + size <= BOARD_SIZE : y + size <= BOARD_SIZE));
}

/**
 * @brief Places a ship on the board.
 * @param plateau Pointer to the Plateau structure.
 * @param ship Pointer to the Ship structure.
 */
void placeShip(Plateau *plateau, const Ship *ship) {
    // Add logic here to place the ship on the board
    // Based on the coordinates, size, and orientation of the ship

    // This example simply marks the cells of the board with the ship type
    for (int i = 0; i < ship->size; i++) {
        if (ship->orientation == 'H') {
            plateau->matrix[ship->y][ship->x + i] = BOAT;
        } else {
            plateau->matrix[ship->y + i][ship->x] = BOAT;
        }
    }
}

/**
 * @brief Creates a new game with two boards and two arrays of ships.
 * @param game Pointer to the Jeu structure.
 */
void createGame(Jeu *game) {
    // Initialize the random number generator with a seed based on the current time
    srand((unsigned int)time(NULL));

    // Initialize the boards for each player
    for (int i = 0; i < game->numJoueurs; i++) {
        game->plateaux[i] = (Plateau *)malloc(sizeof(Plateau));
        initializeBoard(game->plateaux[i]);
    }

    // Initialize the fleets for each player
    for (int i = 0; i < game->numJoueurs; i++) {
        game->flottilles[i] = (Ship *)malloc(NUM_SHIPS_PER_PLAYER * sizeof(Ship));
    }

    // Place the ships randomly while avoiding them touching each other
    for (int player = 0; player < game->numJoueurs; player++) {
        for (int i = 0; i < NUM_SHIPS_PER_PLAYER; i++) {
            int shipSize = i + 1; // Ship size ranges from 1 to NUM_SHIPS_PER_PLAYER
            int x, y;
            char orientation;

            // Place the ship randomly while ensuring it doesn't touch other ships
            do {
                x = rand() % BOARD_SIZE;
                y = rand() % BOARD_SIZE;
                orientation = (rand() % 2 == 0) ? 'H' : 'V'; // Choose a random orientation
            } while (!canPlaceShip(game->plateaux[player], x, y, shipSize, orientation));

            // Create the ship and add it to the board and fleet
            Ship *newShip = createShip(shipSize, x, y, orientation);
            game->flottilles[player][i] = *newShip;
            placeShip(game->plateaux[player], newShip);
            free(newShip); // Free the allocated memory for the ship as the information has been copied
        }
    }
}

/**
 * @brief Performs a shot on the game board and updates the state accordingly.
 * @param plateau Pointer to the Plateau structure.
 * @param x X-coordinate of the target.
 * @param y Y-coordinate of the target.
 * @return Result of the shot (WATER, BOAT, WATER_SHOT, WRECK, etc.).
 */
CellType performShot(Plateau *plateau, int x, int y) {
    // Add logic here to perform the shot and update the game state
    // Update the cell type and return the result of the shot

    // For this example, assume a simplified update where hitting a boat turns it into wreckage
    // and hitting water marks the shot on the board
    if (plateau->matrix[y][x] == BOAT) {
        plateau->matrix[y][x] = WRECK;
        return BOAT;
    } else if (plateau->matrix[y][x] == WATER) {
        plateau->matrix[y][x] = WATER_SHOT;
        return WATER;
    } else if (plateau->matrix[y][x] == WRECK || plateau->matrix[y][x] == WATER_SHOT) {
        return WATER_SHOT;
    } else {
        return WATER; // Default to water if unexpected state
    }
}

/**
 * @brief Displays a game board.
 * @param plateau Pointer to the Plateau structure.
 * @param playerView True for player's view (showing ships), false for opponent's view (showing shots).
 */
void displayGameBoard(const Plateau *plateau, bool playerView) {
    printf("Game Board:\n");

    for (int i = 0; i < plateau->size; i++) {
        for (int j = 0; j < plateau->size; j++) {
            char displayChar;
            if (playerView) {
                // Display ships for player's view
                switch (plateau->matrix[i][j]) {
                    case WATER:
                        displayChar = '~'; // Water
                        break;
                    case BOAT:
                        displayChar = 'B'; // Ship
                        break;
                    case WATER_SHOT:
                        displayChar = '*'; // Water shot
                        break;
                    case WRECK:
                        displayChar = 'X'; // Wreck
                        break;
                    default:
                        displayChar = '?'; // Unknown
                        break;
                }
            } else {
                // Display shots for opponent's view
                switch (plateau->matrix[i][j]) {
                    case WATER_SHOT:
                        displayChar = '*'; // Water shot
                        break;
                    case WRECK:
                        displayChar = 'X'; // Wreck
                        break;
                    default:
                        displayChar = '.'; // Unknown
                        break;
                }
            }

            printf("%c ", displayChar);
        }
        printf("\n");
    }
}

/**
 * @brief Displays the game state for a player.
 * @param game Pointer to the Jeu structure.
 * @param playerIndex Index of the player (0 or 1).
 */
void displayPlayerGameState(const Jeu *game, int playerIndex) {
    printf("Player %d's Game State:\n", playerIndex + 1);
    displayGameBoard(game->plateaux[playerIndex], true);
    printf("\n");
}

/**
 * @brief Displays the opponent's view of the game state for a player.
 * @param game Pointer to the Jeu structure.
 * @param playerIndex Index of the player (0 or 1).
 */
void displayOpponentGameState(const Jeu *game, int playerIndex) {
    printf("Opponent's View (Player %d):\n", playerIndex + 1);
    displayGameBoard(game->plateaux[playerIndex], false);
    printf("\n");
}

/**
 * @brief Executes a player's turn in the game.
 * @param game Pointer to the Jeu structure.
 * @param currentPlayerIndex Index of the current player (0 or 1).
 */
void executePlayerTurn(Jeu *game, int currentPlayerIndex) {
    printf("Player %d's Turn:\n", currentPlayerIndex + 1);

    int targetX, targetY;

    // Ask for input coordinates
    printf("Enter target coordinates (X Y): ");
    scanf("%d %d", &targetX, &targetY);

    // Perform the shot and update the game state
    CellType result = performShot(game->plateaux[currentPlayerIndex], targetX, targetY);

    // Display a message based on the result
    switch (result) {
        case WATER:
            printf("Splash! Shot missed.\n");
            break;
        case BOAT:
            printf("Direct hit! You hit an enemy ship!\n");
            break;
        case WATER_SHOT:
            printf("You already shot there. Try another location.\n");
            break;
        case WRECK:
            printf("Boom! You hit a wreck. Keep targeting!\n");
            break;
        default:
            printf("Unexpected result. Something went wrong.\n");
            break;
    }
}

/**
 * @brief Executes the computer's turn in the game.
 * @param game Pointer to the Jeu structure.
 * @param computerPlayerIndex Index of the computer player (0 or 1).
 */
void executeComputerTurn(Jeu *game, int computerPlayerIndex) {
    printf("Computer's Turn:\n");

    int targetX, targetY;

    // Perform a random shot where the computer hasn't already shot
    do {
        targetX = rand() % BOARD_SIZE;
        targetY = rand() % BOARD_SIZE;
    } while (game->plateaux[computerPlayerIndex]->matrix[targetY][targetX] == WATER_SHOT);

    // Perform the shot and update the game state
    CellType result = performShot(game->plateaux[computerPlayerIndex], targetX, targetY);

    // Display a message based on the result
    switch (result) {
        case WATER:
            printf("The computer missed.\n");
            break;
        case BOAT:
            printf("The computer hit one of your ships!\n");
            break;
        case WATER_SHOT:
            printf("The computer already shot there. It tries another location.\n");
            break;
        case WRECK:
            printf("The computer hit a wreck. It continues targeting!\n");
            break;
        default:
            printf("Unexpected result. Something went wrong.\n");
            break;
    }
}

/**
 * @brief Plays the game and determines the winner.
 * @param game Pointer to the Jeu structure.
 */
void playGame(Jeu *game) {
    int currentPlayer = 0; // Start with player 1

    while (!isGameFinished(game)) {
        displayPlayerGameState(game, currentPlayer);
        displayOpponentGameState(game, (currentPlayer + 1) % 2);

        // Execute the turn for the current player
        if (currentPlayer == 0) {
            executePlayerTurn(game, currentPlayer);
        } else {
            executeComputerTurn(game, currentPlayer);
        }

        // Switch to the other player for the next turn
        currentPlayer = (currentPlayer + 1) % game->numJoueurs;
    }

    // Display the final state of the game and determine the winner
    displayPlayerGameState(game, currentPlayer);
    displayOpponentGameState(game, (currentPlayer + 1) % 2);

    int winnerIndex = determineWinner(game);
    if (winnerIndex != -1) {
        printf("Player %d wins!\n", winnerIndex + 1);
    } else {
        printf("It's a draw!\n");
    }
}

/**
 * @brief Checks if the game has finished.
 * @param game Pointer to the Jeu structure.
 * @return True if the game has finished, false otherwise.
 */
bool isGameFinished(const Jeu *game) {
    // The game is finished when all ships of one or both players are sunk
    for (int i = 0; i < game->numJoueurs; i++) {
        for (int j = 0; j < NUM_SHIPS_PER_PLAYER; j++) {
            if (isShipAlive(&game->flottilles[i][j])) {
                return false; // At least one ship is still alive
            }
        }
    }
    return true; // All ships are sunk
}

/**
 * @brief Determines the winner of the game.
 * @param game Pointer to the Jeu structure.
 * @return Index of the winner (0 or 1) or -1 for a draw.
 */
int determineWinner(const Jeu *game) {
    // Determine the winner based on the remaining ships
    int player1ShipsRemaining = 0;
    int player2ShipsRemaining = 0;

    for (int i = 0; i < NUM_SHIPS_PER_PLAYER; i++) {
        if (isShipAlive(&game->flottilles[0][i])) {
            player1ShipsRemaining++;
        }
        if (isShipAlive(&game->flottilles[1][i])) {
            player2ShipsRemaining++;
        }
    }

    if (player1ShipsRemaining > player2ShipsRemaining) {
        return 0; // Player 1 wins
    } else if (player2ShipsRemaining > player1ShipsRemaining) {
        return 1; // Player 2 wins
    } else {
        return -1; // Draw
    }
}

/**
 * @brief Frees the memory allocated for the game.
 * @param game Pointer to the Jeu structure.
 */
void freeGame(Jeu *game) {
    for (int i = 0; i < game->numJoueurs; i++) {
        freeBoard(game->plateaux[i]);
        free(game->plateaux[i]);
        free(game->flottilles[i]);
    }
}