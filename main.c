#include "functions.h"

int main() {
    Jeu game;
    game.numJoueurs = 2;

    // Create the game with two players
    createGame(&game);

    // Play the game
    playGame(&game);

    // Free the allocated memory
    freeGame(&game);

    return 0;
}