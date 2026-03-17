#include "Game.h"

static sfTexture* NORMAL_TEXTURE;
static sfTexture* RED_TEXTURE;
static sfTexture* YELLOW_TEXTURE;

static void LoadAllTextures()
{
    NORMAL_TEXTURE = sfTexture_createFromFile("./Assets/normal.png", NULL);
    RED_TEXTURE = sfTexture_createFromFile("./Assets/red.png", NULL);
    YELLOW_TEXTURE = sfTexture_createFromFile("./Assets/yellow.png", NULL);
}

Cell* CreateCell(sfVector2i cellCoord, float size, enum CellType type, int grid[20][20]) {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    if (!cell) {
        return NULL;
    }

    cell->coord = cellCoord;

    cell->type = type;

    cell->sprite = sfSprite_create();
    if (!cell->sprite) {
        free(cell);
        return NULL;
    }
    GetRequiredSpriteForCell(cell, grid);
    sfSprite_setPosition(cell->sprite, (sfVector2f) { (float)cellCoord.x* size, (float)cellCoord.y* size });
    float scale = (float)CELL_SIZE / 8.f;
    sfSprite_setScale(cell->sprite, (sfVector2f) { scale, scale });

    return cell;
}

void DestroyCell(Cell* cell) {
    if (cell) {
        if (cell->sprite) {
            sfSprite_destroy(cell->sprite);
        }
        free(cell);
    }
}

void DrawCell(sfRenderWindow* window, Cell* cell) {
    if (window && cell && cell->sprite) {
        sfRenderWindow_drawSprite(window, cell->sprite, NULL);
    }
}