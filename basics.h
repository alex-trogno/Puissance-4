#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics/Types.h>

#define WINDOW_TITLE "LiteBot"
#define WINDOW_WIDTH 800    // CELL_SIZE * GRID_COLS
#define WINDOW_HEIGHT 800   // CELL_SIZE * GRID_ROWS

#define GRID_ROWS 20
#define GRID_COLS 20
#define CELL_SIZE 40
#define GRAPHIC_OFFSET 20

static sfFont* GAME_FONT;

/// <summary>
/// Use this enum to define some exit codes for the program (just for debug purpose)
/// </summary>
enum ExitCode {
    FAILURE = -1,
    SUCCESS = 1,
    NULL_TEXT = 60,
    NULL_WINDOW = 61,
    NULL_SPRITE = 62,
    NULL_TEXTURE = 63,
    NULL_FONT = 64,
};

/// <summary>
/// Use this enum to define the different scenes of the game
/// </summary>
enum Scene {
    MAP_SELECTION,
    GAME,
};

/// <summary>
/// Use this enum to define the different types of cells in the grid
/// </summary>
enum CellType {
    EMPTY,
    P1,
    P2,

};