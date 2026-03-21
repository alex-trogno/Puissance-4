#include "Game.h"
#include "Renderer.h"
#include <math.h>

static int  CheckWin(Game* game, int row, int col);
static int  CheckDraw(Game* game);
static void SetStatusText(Game* game, const char* msg);

static void SetStatusText(Game* game, const char* msg)
{
    if (!game->statusText || !game->font) return;
    sfText_setString(game->statusText, msg);
    sfFloatRect tb = sfText_getLocalBounds(game->statusText);
    sfText_setOrigin(game->statusText, (sfVector2f) { tb.width / 2.f, tb.height / 2.f });
    sfText_setPosition(game->statusText, (sfVector2f) {
        WINDOW_WIDTH / 2.f, (float)(OFFSET_TOP / 3)
    });
}

static void SetOverlayText(Game* game, const char* msg)
{
    if (!game->overlayText) return;
    sfText_setString(game->overlayText, msg);
    sfFloatRect tb = sfText_getLocalBounds(game->overlayText);
    sfText_setOrigin(game->overlayText, (sfVector2f) { tb.width / 2.f, tb.height / 2.f });
    sfText_setPosition(game->overlayText, (sfVector2f) {
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 4.f + 80
    });
}

static int CheckWin(Game* game, int row, int col)
{
    enum CellType t = game->grid->cell[row][col]->type;
    if (t == EMPTY) return 0;

    int dirs[4][2] = { {0,1}, {1,0}, {1,1}, {1,-1} };
    for (int d = 0; d < 4; d++)
    {
        int count = 1;
        for (int s = -1; s <= 1; s += 2)
        {
            int r = row, c = col;
            while (1)
            {
                r += dirs[d][0] * s;
                c += dirs[d][1] * s;
                if (r < 0 || r >= GRID_ROWS || c < 0 || c >= GRID_COLS) break;
                if (game->grid->cell[r][c]->type != t) break;
                count++;
            }
        }
        if (count >= 4) return 1;
    }
    return 0;
}

static int CheckDraw(Game* game)
{
    for (int col = 0; col < GRID_COLS; col++)
        if (game->grid->cell[0][col]->type == EMPTY) return 0;
    return 1;
}

static void LandFallingToken(Game* game)
{
    FallingToken* ft = &game->fallingToken;
    if (!ft->pendingCell) { ft->active = 0; return; }

    Cell* cell = (Cell*)ft->pendingCell;
    cell->type = (enum CellType)ft->pendingType;
    ft->pendingCell = NULL;

    int row = ft->pendingRow;
    int col = ft->pendingCol;

    char msg[64];

    if (CheckWin(game, row, col))
    {
        game->winner = game->players[game->currentPlayer]->id;
        game->gameOver = 1;
        snprintf(msg, sizeof(msg), "%s gagne !", game->players[game->currentPlayer]->name);
        SetStatusText(game, msg);
        SetOverlayText(game, msg);

        if (game->winnerSprite) { sfSprite_destroy(game->winnerSprite); game->winnerSprite = NULL; }
        sfTexture* wtex = Renderer_GetCharTexture((game->currentPlayer == 0) ? PLAYER1 : PLAYER2);
        if (wtex)
        {
            game->winnerSprite = sfSprite_create();
            if (game->winnerSprite)
            {
                sfSprite_setTexture(game->winnerSprite, wtex, sfTrue);
                sfFloatRect b = sfSprite_getLocalBounds(game->winnerSprite);
                float h = 120.f;
                float scale = (b.height > 0.f) ? h / b.height : 1.f;
                sfSprite_setScale(game->winnerSprite, (sfVector2f) { scale, scale });
                sfFloatRect gb = sfSprite_getGlobalBounds(game->winnerSprite);
                sfSprite_setPosition(game->winnerSprite, (sfVector2f) {
                    WINDOW_WIDTH / 2.f - gb.width / 2.f,
                        WINDOW_HEIGHT / 4.f - 60.f
                });
            }
        }

        ft->active = 0;
        return;
    }
    if (CheckDraw(game))
    {
        game->winner = 0;
        game->gameOver = 1;
        SetStatusText(game, "Egalite !");
        SetOverlayText(game, "Egalite !");
        ft->active = 0;
        return;
    }

    game->currentPlayer = (game->currentPlayer == 0) ? 1 : 0;
    snprintf(msg, sizeof(msg), "Tour de %s", game->players[game->currentPlayer]->name);
    SetStatusText(game, msg);
    ft->active = 0;
}

static void LaunchFallingToken(Game* game, int col, int targetRow, Cell* targetCell, int tokenType)
{
    FallingToken* ft = &game->fallingToken;

    if (ft->sprite) { sfSprite_destroy(ft->sprite); ft->sprite = NULL; }

    sfTexture* tex = Renderer_GetTexture((enum CellType)tokenType);
    if (!tex) return;

    ft->sprite = sfSprite_create();
    if (!ft->sprite) return;

    sfSprite_setTexture(ft->sprite, tex, sfTrue);
    sfFloatRect b = sfSprite_getLocalBounds(ft->sprite);
    float sx = (b.width > 0.f) ? (float)CELL_SIZE / b.width : 1.f;
    float sy = (b.height > 0.f) ? (float)CELL_SIZE / b.height : 1.f;
    sfSprite_setScale(ft->sprite, (sfVector2f) { sx, sy });

    ft->currentY = (float)OFFSET_TOP - CELL_SIZE;
    ft->targetY = OFFSET_TOP + (float)targetRow * CELL_SIZE;
    ft->active = 1;
    ft->pendingCell = targetCell;
    ft->pendingType = tokenType;
    ft->pendingRow = targetRow;
    ft->pendingCol = col;

    sfSprite_setPosition(ft->sprite, (sfVector2f) {
        OFFSET_SIDE + (float)col * CELL_SIZE, ft->currentY
    });
}

static int DropToken(Game* game, int col)
{
    for (int row = GRID_ROWS - 1; row >= 0; row--)
    {
        Cell* cell = game->grid->cell[row][col];
        if (cell->type == EMPTY)
        {
            int tokenType = (game->currentPlayer == 0) ? PLAYER1 : PLAYER2;
            cell->type = (enum CellType)tokenType;
            LaunchFallingToken(game, col, row, cell, tokenType);
            cell->type = EMPTY;
            return row;
        }
    }
    return -1;
}

static void ResetFallingToken(Game* game)
{
    FallingToken* ft = &game->fallingToken;
    if (ft->sprite) { sfSprite_destroy(ft->sprite); ft->sprite = NULL; }
    ft->active = 0;
    ft->pendingCell = NULL;
    ft->pendingType = 0;
    ft->pendingRow = 0;
    ft->pendingCol = 0;
}

Game* CreateGame(const char* name1, const char* name2)
{
    Game* game = (Game*)malloc(sizeof(Game));
    if (!game) { printf("ERROR: Failed to allocate Game!\n"); return NULL; }

    Renderer_Load();

    int emptyGrid[GRID_ROWS][GRID_COLS] = { 0 };
    game->grid = CreateGrid(emptyGrid);
    game->players[0] = CreatePlayer(1, (name1 && name1[0]) ? name1 : "Joueur 1");
    game->players[1] = CreatePlayer(2, (name2 && name2[0]) ? name2 : "Joueur 2");
    game->currentPlayer = 0;
    game->winner = -1;
    game->gameOver = 0;

    game->winnerSprite = NULL;
    game->fallingToken.sprite = NULL;
    game->fallingToken.currentY = 0.f;
    game->fallingToken.targetY = 0.f;
    game->fallingToken.active = 0;
    game->fallingToken.pendingCell = NULL;
    game->fallingToken.pendingType = 0;
    game->fallingToken.pendingRow = 0;
    game->fallingToken.pendingCol = 0;

    game->font = sfFont_createFromFile("./Assets/Geo-Regular.ttf");
    if (!game->font) printf("ERREUR: impossible de charger Geo-Regular.ttf\n");

    game->statusText = sfText_create();
    if (game->statusText && game->font)
    {
        sfText_setFont(game->statusText, game->font);
        sfText_setCharacterSize(game->statusText, 36);
        sfText_setFillColor(game->statusText, sfWhite);
        sfText_setOutlineColor(game->statusText, sfBlack);
        sfText_setOutlineThickness(game->statusText, 3.f);
    }

    float btnW = 200.f, btnH = 60.f;
    float btnX = WINDOW_WIDTH / 2.f - btnW / 2.f;

    game->overlayTexture = Renderer_GetOverlayTexture();
    game->overlaySprite = Renderer_CreateScaledSprite(game->overlayTexture,
        (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
    if (game->overlaySprite)
        sfSprite_setPosition(game->overlaySprite, (sfVector2f) { 0.f, 0.f });

    game->restartButtonTexture = Renderer_GetRestartTexture();
    game->restartButtonSprite = Renderer_CreateScaledSprite(game->restartButtonTexture, btnW, btnH);
    if (game->restartButtonSprite)
        sfSprite_setPosition(game->restartButtonSprite, (sfVector2f) { btnX, WINDOW_HEIGHT * 0.52f });

    game->menuButtonTexture = Renderer_GetMenuTexture();
    game->menuButtonSprite = Renderer_CreateScaledSprite(game->menuButtonTexture, btnW, btnH);
    if (game->menuButtonSprite)
        sfSprite_setPosition(game->menuButtonSprite, (sfVector2f) { btnX, WINDOW_HEIGHT * 0.52f + btnH + 20.f });

    game->overlayText = sfText_create();
    if (game->overlayText && game->font)
    {
        sfText_setFont(game->overlayText, game->font);
        sfText_setCharacterSize(game->overlayText, 48);
        sfText_setFillColor(game->overlayText, sfWhite);
        sfText_setOutlineColor(game->overlayText, sfBlack);
        sfText_setOutlineThickness(game->overlayText, 3.f);
    }

    char initMsg[64];
    snprintf(initMsg, sizeof(initMsg), "Tour de %s", game->players[0]->name);
    SetStatusText(game, initMsg);

    return game;
}

void DestroyGame(Game* game)
{
    if (!game) return;
    DestroyGrid(game->grid);
    DestroyPlayer(game->players[0]);
    DestroyPlayer(game->players[1]);
    if (game->winnerSprite) { sfSprite_destroy(game->winnerSprite); game->winnerSprite = NULL; }
    ResetFallingToken(game);
    if (game->statusText)          sfText_destroy(game->statusText);
    if (game->overlayText)         sfText_destroy(game->overlayText);
    if (game->overlaySprite)       sfSprite_destroy(game->overlaySprite);
    if (game->winnerSprite)        sfSprite_destroy(game->winnerSprite);
    if (game->restartButtonSprite) sfSprite_destroy(game->restartButtonSprite);
    if (game->menuButtonSprite)    sfSprite_destroy(game->menuButtonSprite);
    if (game->font)                sfFont_destroy(game->font);
    free(game);
    Renderer_Unload();
}

void UpdateGame(Game* game, float deltaTime)
{
    Renderer_UpdateArrows(deltaTime);

    FallingToken* ft = &game->fallingToken;
    if (ft->active && ft->sprite)
    {
        ft->currentY += TOKEN_FALL_SPEED * deltaTime;
        if (ft->currentY >= ft->targetY)
            ft->currentY = ft->targetY;
        sfVector2f pos = sfSprite_getPosition(ft->sprite);
        sfSprite_setPosition(ft->sprite, (sfVector2f) { pos.x, ft->currentY });
        if (ft->currentY >= ft->targetY)
            LandFallingToken(game);
    }
}

int HandleClickGame(Game* game, int mouseX, int mouseY)
{
    if (!game) return;

    if (game->gameOver)
    {
        if (game->restartButtonSprite)
        {
            sfFloatRect b = sfSprite_getGlobalBounds(game->restartButtonSprite);
            if (sfFloatRect_contains(&b, (float)mouseX, (float)mouseY))
            {
                DestroyGrid(game->grid);
                int emptyGrid[GRID_ROWS][GRID_COLS] = { 0 };
                game->grid = CreateGrid(emptyGrid);
                game->currentPlayer = 0;
                game->winner = -1;
                game->gameOver = 0;
                if (game->winnerSprite) { sfSprite_destroy(game->winnerSprite); game->winnerSprite = NULL; }
                ResetFallingToken(game);
                char initMsg[64];
                snprintf(initMsg, sizeof(initMsg), "Tour de %s", game->players[0]->name);
                SetStatusText(game, initMsg);
                return SCENE_GAME;
            }
        }
        if (game->menuButtonSprite)
        {
            sfFloatRect b = sfSprite_getGlobalBounds(game->menuButtonSprite);
            if (sfFloatRect_contains(&b, (float)mouseX, (float)mouseY))
                return SCENE_MENU;
        }
        return SCENE_GAME;
    }

    if (game->fallingToken.active)
    {
        game->fallingToken.currentY = game->fallingToken.targetY;
        LandFallingToken(game);
    }

    for (int col = 0; col < GRID_COLS; col++)
    {
        sfSprite* arrow = Renderer_GetArrowSprite(col);
        if (!arrow) continue;
        sfFloatRect bounds = sfSprite_getGlobalBounds(arrow);
        if (!sfFloatRect_contains(&bounds, (float)mouseX, (float)mouseY)) continue;
        DropToken(game, col);
        return SCENE_GAME;
    }
    return SCENE_GAME;
}
void DrawGame(sfRenderWindow* window, Game* game)
{
    if (!window || !game) return;

    Renderer_DrawBackground(window);
    DrawGridTokens(window, game->grid);
    Renderer_DrawFallingToken(window, &game->fallingToken);
    DrawGridBoard(window, game->grid);

    if (!game->gameOver)
        Renderer_DrawArrows(window);

    if (game->statusText)
        sfRenderWindow_drawText(window, game->statusText, NULL);

    if (game->gameOver)
    {
        Renderer_DrawOverlay(window,
            game->overlaySprite, game->overlayText,
            game->restartButtonSprite, game->menuButtonSprite);
        if (game->winnerSprite)
            sfRenderWindow_drawSprite(window, game->winnerSprite, NULL);
    }
}