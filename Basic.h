#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.h>

#define WINDOW_TITLE "Puissance 4"
#define GRID_ROWS 6
#define GRID_COLS 7
#define CELL_SIZE 60

#define OFFSET_SIDE   150
#define OFFSET_TOP    250
#define OFFSET_BOTTOM 80

#define WINDOW_WIDTH  (GRID_COLS * CELL_SIZE + OFFSET_SIDE * 2)
#define WINDOW_HEIGHT (GRID_ROWS * CELL_SIZE + OFFSET_TOP + OFFSET_BOTTOM)
#define GRAPHIC_OFFSET 20

#define TOKEN_FALL_SPEED 800.f

enum ExitCode {
    FAILURE = -1,
    SUCCESS = 0,
    NULL_WINDOW = 61,
    NULL_SPRITE = 62,
    NULL_TEXTURE = 63,
    NULL_FONT = 64,
};

enum Scene {
    SCENE_MENU,
    SCENE_GAME,
    SCENE_END,
};

enum CellType {
    EMPTY,
    PLAYER1,
    PLAYER2,
};

typedef struct Cell
{
    sfVector2i    coord;
    enum CellType type;
    sfSprite* sprite;
} Cell;

typedef struct FallingToken
{
    sfSprite* sprite;
    float         currentY;
    float         targetY;
    int           active;
    void* pendingCell;
    int           pendingType;
    int           pendingRow;
    int           pendingCol;
} FallingToken;