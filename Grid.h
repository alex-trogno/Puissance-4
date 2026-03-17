#pragma once

#include <SFML/System.h>

#include "Basic.h"

typedef struct Grid
{
    Cell* cell[GRID_ROWS][GRID_COLS];
} Grid;

Cell* CreateCell(sfVector2i cellCoord, float size, enum CellType type, int grid[GRID_ROWS][GRID_COLS]);
void  DestroyCell(Cell* cell);
void  DrawCell(sfRenderWindow* window, Cell* cell);

Grid* CreateGrid(int gridData[GRID_ROWS][GRID_COLS]);
void  DestroyGrid(Grid* grid);
void  DrawGrid(sfRenderWindow* window, Grid* grid);
