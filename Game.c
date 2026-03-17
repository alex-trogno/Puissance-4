#include "Game.h"
#include <math.h>

static sfTexture* NORMAL_TEXTURE;
static sfTexture* RED_TEXTURE;
static sfTexture* YELLOW_TEXTURE;
static sfTexture* BACKGROUND_TEXTURE;
static sfSprite* BACKGROUND_SPRITE;
static sfTexture* ARROW_TEXTURE;
static sfSprite* ARROW_SPRITES[GRID_COLS];
static float      ARROW_ANIM_TIME = 0.f;
static float      ARROW_BASE_Y;

static void LoadAllTextures()
{
    NORMAL_TEXTURE = sfTexture_createFromFile("./Assets/normal.png", NULL);
    if (!NORMAL_TEXTURE)
        printf("ERREUR: impossible de charger normal.png\n");

    RED_TEXTURE = sfTexture_createFromFile("./Assets/red.png", NULL);
    if (!RED_TEXTURE)
        printf("ERREUR: impossible de charger red.png\n");

    YELLOW_TEXTURE = sfTexture_createFromFile("./Assets/yellow.png", NULL);
    if (!YELLOW_TEXTURE)
        printf("ERREUR: impossible de charger yellow.png\n");

    BACKGROUND_TEXTURE = sfTexture_createFromFile("./Assets/background.png", NULL);
    if (!BACKGROUND_TEXTURE)
        printf("ERREUR: impossible de charger background.png\n");

    BACKGROUND_SPRITE = sfSprite_create();
    if (BACKGROUND_SPRITE && BACKGROUND_TEXTURE)
    {
        sfSprite_setTexture(BACKGROUND_SPRITE, BACKGROUND_TEXTURE, sfTrue);
        sfFloatRect bounds = sfSprite_getLocalBounds(BACKGROUND_SPRITE);
        float scaleX = (bounds.width > 0.f) ? (float)WINDOW_WIDTH / bounds.width : 1.f;
        float scaleY = (bounds.height > 0.f) ? (float)WINDOW_HEIGHT / bounds.height : 1.f;
        sfSprite_setScale(BACKGROUND_SPRITE, (sfVector2f) { scaleX, scaleY });
    }

    ARROW_TEXTURE = sfTexture_createFromFile("./Assets/arrow.png", NULL);
    if (!ARROW_TEXTURE)
        printf("ERREUR: impossible de charger arrow.png\n");

    ARROW_BASE_Y = (float)(OFFSET_TOP - CELL_SIZE * 1.5f);

    for (int col = 0; col < GRID_COLS; col++)
    {
        ARROW_SPRITES[col] = sfSprite_create();
        if (ARROW_SPRITES[col] && ARROW_TEXTURE)
        {
            sfSprite_setTexture(ARROW_SPRITES[col], ARROW_TEXTURE, sfTrue);

            sfFloatRect bounds = sfSprite_getLocalBounds(ARROW_SPRITES[col]);
            float arrowSize = CELL_SIZE * 0.8f;
            float scaleX = (bounds.width > 0.f) ? arrowSize / bounds.width : 1.f;
            float scaleY = (bounds.height > 0.f) ? arrowSize / bounds.height : 1.f;
            sfSprite_setScale(ARROW_SPRITES[col], (sfVector2f) { scaleX, scaleY });

            sfSprite_setPosition(ARROW_SPRITES[col], (sfVector2f) {
                (float)(OFFSET_SIDE + col * CELL_SIZE),
                    ARROW_BASE_Y
            });
        }
    }
}

static void UnloadAllTextures()
{
    if (NORMAL_TEXTURE) { sfTexture_destroy(NORMAL_TEXTURE);     NORMAL_TEXTURE = NULL; }
    if (RED_TEXTURE) { sfTexture_destroy(RED_TEXTURE);        RED_TEXTURE = NULL; }
    if (YELLOW_TEXTURE) { sfTexture_destroy(YELLOW_TEXTURE);     YELLOW_TEXTURE = NULL; }
    if (BACKGROUND_SPRITE) { sfSprite_destroy(BACKGROUND_SPRITE);   BACKGROUND_SPRITE = NULL; }
    if (BACKGROUND_TEXTURE) { sfTexture_destroy(BACKGROUND_TEXTURE); BACKGROUND_TEXTURE = NULL; }
    for (int col = 0; col < GRID_COLS; col++)
        if (ARROW_SPRITES[col]) { sfSprite_destroy(ARROW_SPRITES[col]); ARROW_SPRITES[col] = NULL; }
    if (ARROW_TEXTURE) { sfTexture_destroy(ARROW_TEXTURE);      ARROW_TEXTURE = NULL; }
}

static void GetRequiredSpriteForCell(Cell* cell, int grid[GRID_ROWS][GRID_COLS])
{
    if (!cell || !cell->sprite) return;

    switch (cell->type)
    {
    case PLAYER1: sfSprite_setTexture(cell->sprite, RED_TEXTURE, sfTrue); break;
    case PLAYER2: sfSprite_setTexture(cell->sprite, YELLOW_TEXTURE, sfTrue); break;
    default:      sfSprite_setTexture(cell->sprite, NORMAL_TEXTURE, sfTrue); break;
    }
    (void)grid;
}

Cell* CreateCell(sfVector2i cellCoord, float size, enum CellType type, int grid[GRID_ROWS][GRID_COLS])
{
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    if (!cell) return NULL;

    cell->coord = cellCoord;
    cell->type = type;

    cell->sprite = sfSprite_create();
    if (!cell->sprite)
    {
        free(cell);
        return NULL;
    }

    GetRequiredSpriteForCell(cell, grid);

    sfSprite_setPosition(cell->sprite, (sfVector2f) {
        OFFSET_SIDE + (float)cellCoord.x * size,
            OFFSET_TOP + (float)cellCoord.y * size
    });

    sfFloatRect bounds = sfSprite_getLocalBounds(cell->sprite);
    float scaleX = (bounds.width > 0.f) ? (float)CELL_SIZE / bounds.width : 1.f;
    float scaleY = (bounds.height > 0.f) ? (float)CELL_SIZE / bounds.height : 1.f;
    sfSprite_setScale(cell->sprite, (sfVector2f) { scaleX, scaleY });

    return cell;
}

void DestroyCell(Cell* cell)
{
    if (!cell) return;
    if (cell->sprite) sfSprite_destroy(cell->sprite);
    free(cell);
}

void DrawCell(sfRenderWindow* window, Cell* cell)
{
    if (window && cell && cell->sprite)
        sfRenderWindow_drawSprite(window, cell->sprite, NULL);
}

Game* CreateGame()
{
    LoadAllTextures();

    Game* game = (Game*)malloc(sizeof(Game));
    if (!game)
    {
        printf("ERROR: Failed to allocate Game!\n");
        return NULL;
    }

    int emptyGrid[GRID_ROWS][GRID_COLS] = { 0 };
    game->grid = CreateGrid(emptyGrid);

    return game;
}

void DestroyGame(Game* game)
{
    if (!game) return;
    DestroyGrid(game->grid);
    free(game);
    UnloadAllTextures();
}

void UpdateGame(Game* game, float deltaTime)
{
    (void)game;
    ARROW_ANIM_TIME += deltaTime;
    float bounce = sinf(ARROW_ANIM_TIME * 3.f) * 8.f;
    for (int col = 0; col < GRID_COLS; col++)
        if (ARROW_SPRITES[col])
            sfSprite_setPosition(ARROW_SPRITES[col], (sfVector2f) {
            (float)(OFFSET_SIDE + col * CELL_SIZE),
                ARROW_BASE_Y + bounce
        });
}

int HandleClickGame(Game* game, int mouseX, int mouseY)
{
    (void)game;
    for (int col = 0; col < GRID_COLS; col++)
    {
        if (!ARROW_SPRITES[col]) continue;
        sfFloatRect bounds = sfSprite_getGlobalBounds(ARROW_SPRITES[col]);
        if (sfFloatRect_contains(&bounds, (float)mouseX, (float)mouseY))
        {
            printf("Colonne %d cliquee\n", col);
            return col;
        }
    }
    return -1;
}

void DrawGame(sfRenderWindow* window, Game* game) {
    if (!window || !game) return;
    if (BACKGROUND_SPRITE)
        sfRenderWindow_drawSprite(window, BACKGROUND_SPRITE, NULL);
    for (int col = 0; col < GRID_COLS; col++)
        if (ARROW_SPRITES[col])
            sfRenderWindow_drawSprite(window, ARROW_SPRITES[col], NULL);
    DrawGrid(window, game->grid);
}