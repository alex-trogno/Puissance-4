#pragma once

#include "Basic.h"

typedef struct Player
{
    int           id;      /* 1 ou 2          */
    enum CellType type;    /* PLAYER1/PLAYER2 */
    char          name[32];
    int           score;
} Player;

Player* CreatePlayer(int id, const char* name);
void    DestroyPlayer(Player* player);
