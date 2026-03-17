#include "Grid.h"

Grid* CreateGrid(int gridData[GRID_ROWS][GRID_COLS])
{
    Grid* grid = (Grid*)malloc(sizeof(Grid));
    if (!grid)
    {
        printf("ERROR: Failed to allocate Grid!\n");
        return NULL;
    }

    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            sfVector2i coord = { col, row };
            grid->cell[row][col] = CreateCell(
                coord,
                (float)CELL_SIZE,
                (enum CellType)gridData[row][col],
                gridData
            );

            if (!grid->cell[row][col])
            {
                printf("ERROR: Failed to create Cell[%d][%d]!\n", row, col);
                for (int r = 0; r < row; r++)
                    for (int c = 0; c < GRID_COLS; c++)
                        DestroyCell(grid->cell[r][c]);
                for (int c = 0; c < col; c++)
                    DestroyCell(grid->cell[row][c]);
                free(grid);
                return NULL;
            }
        }
    }

    return grid;
}

void DestroyGrid(Grid* grid)
{
    if (!grid) return;

    for (int row = 0; row < GRID_ROWS; row++)
        for (int col = 0; col < GRID_COLS; col++)
            DestroyCell(grid->cell[row][col]);

    free(grid);
}

void DrawGrid(sfRenderWindow* window, Grid* grid)
{
    if (!window || !grid) return;

    for (int row = 0; row < GRID_ROWS; row++)
        for (int col = 0; col < GRID_COLS; col++)
            DrawCell(window, grid->cell[row][col]);
}
