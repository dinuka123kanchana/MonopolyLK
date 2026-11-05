#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"

void initialize_game(Game *game);

void display_game_status(Game *game);

void display_board(Game *game);

void play_game(Game *game);

void display_winner(Game *game);

int main(void)
{
    Game game;

    srand((unsigned int)time(NULL));

    printf("\n");
    printf("==============================================\n");
    printf("              MONOPOLY-LK\n");
    printf("       SCS 1301 - Data Structures\n");
    printf("==============================================\n");

    initialize_game(&game);

    printf("\n");
    printf("Game initialized successfully.\n");

    display_game_status(&game);

    printf("\n");
    printf("Initial Board:\n");

    display_board(&game);

    printf("\n");
    printf("Starting game...\n");

    play_game(&game);

    display_winner(&game);

    printf("\n");
    printf("Thank you for playing MONOPOLY-LK!\n");

    return 0;
}