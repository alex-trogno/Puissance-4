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
};

enum CellType {
    EMPTY,
    PLAYER1,
    PLAYER2,
};

enum Direction
{
    NONE = 0,
    EAST = 1,
    WEST = 2,
    EAST_WEST = 3,
    NORTH = 10,
    NORTH_EAST = 11,
    NORTH_WEST = 12,
    NORTH_EAST_WEST = 13,
    SOUTH = 20,
    SOUTH_EAST = 21,
    SOUTH_WEST = 22,
    SOUTH_EAST_WEST = 23,
    NORTH_SOUTH = 30,
    NORTH_SOUTH_EAST = 31,
    NORTH_SOUTH_WEST = 32,
    ALL = 33,
};


typedef struct Cell
{
    sfVector2i coord;      // Coordonnée dans la grille (col, row)
    enum CellType type;
    sfSprite* sprite;

    // === AJOUT POUR ANIMATION ===
    bool isFalling;         // Si le pion est en train de tomber
    float fallY;            // Position Y actuelle (pixels)
    float targetY;          // Position Y d'arrivée
    float fallSpeed;        // Vitesse de chute
} Cell;
