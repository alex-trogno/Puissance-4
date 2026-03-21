#pragma once

#include "Basic.h"
#include "Grid.h"
#include "Players.h"

typedef struct Game
{
    Grid* grid;
    Player* players[2];
    int     currentPlayer;
    int     winner;
    int     gameOver;

    sfText* statusText;
    sfFont* font;

    sfSprite* overlaySprite;
    sfTexture* overlayTexture;
    sfText* overlayText;
    sfSprite* restartButtonSprite;
    sfTexture* restartButtonTexture;
    sfSprite* menuButtonSprite;
    sfTexture* menuButtonTexture;

    FallingToken fallingToken;
    sfSprite* winnerSprite;
} Game;

Game* CreateGame(const char* name1, const char* name2);
void  DestroyGame(Game* game);
void  UpdateGame(Game* game, float deltaTime);
void  DrawGame(sfRenderWindow* window, Game* game);
int   HandleClickGame(Game* game, int mouseX, int mouseY);