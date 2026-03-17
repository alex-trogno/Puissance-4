#pragma once

#include "Basic.h"

typedef struct Player
{
    int           id;
    enum CellType type;
    char          name[32];
    int           score;
} Player;

Player* CreatePlayer(int id, const char* name);
void    DestroyPlayer(Player* player);
