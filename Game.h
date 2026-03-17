#pragma once

#include "Basic.h"
#include "Grid.h"
#include "Players.h"

typedef struct Game
{
    Grid* grid;
} Game;

Game* CreateGame();
void  DestroyGame(Game* game);
void  UpdateGame(Game* game, float deltaTime);
void  DrawGame(sfRenderWindow* window, Game* game);