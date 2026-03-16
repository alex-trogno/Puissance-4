#pragma once

#include <SFML/System.h>

#include "Basic.h"

typedef struct Grid
{
    Cell* cell[GRID_ROWS][GRID_COLS];
} Grid;

Grid* CreateGrid(int gridData[GRID_ROWS][GRID_COLS]);

void DestroyGrid(Grid* grid);

void DrawGrid(sfRenderWindow* window, Grid* grid);