#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.h>

#define WINDOW_TITLE "Puissance 4"
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

#define GRID_ROWS 6
#define GRID_COLS 7
#define CELL_SIZE 50
#define GRAPHIC_OFFSET 20

enum ExitCode {
    FAILURE = -1,
    SUCCESS = 0,
    NULL_WINDOW = 61,
    NULL_SPRITE = 62,
    NULL_TEXTURE = 63,
    NULL_FONT = 64,
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