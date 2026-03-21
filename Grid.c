#include "Grid.h"
#include "Renderer.h"

Cell* CreateCell(sfVector2i cellCoord, float size, enum CellType type, int grid[GRID_ROWS][GRID_COLS])
{
    (void)grid;
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    if (!cell) return NULL;

    cell->coord = cellCoord;
    cell->type = type;

    cell->sprite = sfSprite_create();
    if (!cell->sprite) { free(cell); return NULL; }

    sfTexture* norm = Renderer_GetNormalTexture();
    sfSprite_setTexture(cell->sprite, norm, sfTrue);
    sfFloatRect b = sfSprite_getLocalBounds(cell->sprite);
    float sx = (b.width > 0.f) ? (float)CELL_SIZE / b.width : 1.f;
    float sy = (b.height > 0.f) ? (float)CELL_SIZE / b.height : 1.f;
    sfSprite_setScale(cell->sprite, (sfVector2f) { sx, sy });

    sfSprite_setPosition(cell->sprite, (sfVector2f) {
        OFFSET_SIDE + (float)cellCoord.x * size,
            OFFSET_TOP + (float)cellCoord.y * size
    });

    return cell;
}

void DestroyCell(Cell* cell)
{
    if (!cell) return;
    if (cell->sprite) sfSprite_destroy(cell->sprite);
    free(cell);
}

void DrawCellToken(sfRenderWindow* window, Cell* cell)
{
    if (!window || !cell || !cell->sprite || cell->type == EMPTY) return;
    sfRenderWindow_drawSprite(window, cell->sprite, NULL);
}

void DrawCellBoard(sfRenderWindow* window, Cell* cell)
{
    if (!window || !cell || !cell->sprite) return;
    sfRenderWindow_drawSprite(window, cell->sprite, NULL);
}

Grid* CreateGrid(int gridData[GRID_ROWS][GRID_COLS])
{
    Grid* grid = (Grid*)malloc(sizeof(Grid));
    if (!grid) { printf("ERROR: Failed to allocate Grid!\n"); return NULL; }

    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            sfVector2i coord = { col, row };
            grid->cell[row][col] = CreateCell(coord, (float)CELL_SIZE,
                (enum CellType)gridData[row][col], gridData);

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

void DrawGridTokens(sfRenderWindow* window, Grid* grid)
{
    if (!window || !grid) return;
    for (int row = 0; row < GRID_ROWS; row++)
        for (int col = 0; col < GRID_COLS; col++)
        {
            Cell* cell = grid->cell[row][col];
            if (!cell || cell->type == EMPTY) continue;
            sfTexture* tok = Renderer_GetTexture(cell->type);
            if (tok) sfSprite_setTexture(cell->sprite, tok, sfTrue);
            sfRenderWindow_drawSprite(window, cell->sprite, NULL);
            sfTexture* norm = Renderer_GetNormalTexture();
            if (norm) sfSprite_setTexture(cell->sprite, norm, sfTrue);
        }
}

void DrawGridBoard(sfRenderWindow* window, Grid* grid)
{
    if (!window || !grid) return;
    for (int row = 0; row < GRID_ROWS; row++)
        for (int col = 0; col < GRID_COLS; col++)
            DrawCellBoard(window, grid->cell[row][col]);
}