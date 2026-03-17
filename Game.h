#pragma once

#include "Basic.h"

typedef struct Game
{
    sfText* currentMapText;
    sfSprite* leftArrowSprite;
    sfSprite* rightArrowSprite;
    sfSprite* modeIconSprite;
    sfText* modeText;
    sfSprite* TextureIconSprite;
    sfText* TextureSelect;
} Game;

Game* CreateGame();