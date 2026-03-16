#pragma once

#include "Basic.h"

typedef struct Affichage
{
    sfText* currentMapText;
    sfSprite* leftArrowSprite;
    sfSprite* rightArrowSprite;
    sfSprite* modeIconSprite;
    sfText* modeText;
    sfSprite* TextureIconSprite;
    sfText* TextureSelect;
} Affichage;

Affichage* CreateAffichage();