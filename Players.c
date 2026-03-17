#define _CRT_SECURE_NO_WARNINGS

#include "Players.h"

Player* CreatePlayer(int id, const char* name)
{
    Player* player = (Player*)malloc(sizeof(Player));
    if (!player)
    {
        printf("ERROR: Failed to allocate Player!\n");
        return NULL;
    }

    player->id    = id;
    player->type  = (id == 1) ? PLAYER1 : PLAYER2;
    player->score = 0;
    strncpy(player->name, name ? name : "Joueur", sizeof(player->name) - 1);
    player->name[sizeof(player->name) - 1] = '\0';

    return player;
}

void DestroyPlayer(Player* player)
{
    free(player);
}
