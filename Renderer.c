#include "Renderer.h"
#include <math.h>

static sfTexture* NORMAL_TEXTURE;
static sfTexture* P1_TEXTURE;
static sfTexture* P2_TEXTURE;
static sfTexture* BACKGROUND_TEXTURE;
static sfSprite* BACKGROUND_SPRITE;
static sfTexture* ARROW_TEXTURE;
static sfSprite* ARROW_SPRITES[GRID_COLS];
static sfTexture* OVERLAY_TEXTURE;
static sfTexture* RESTART_TEXTURE;
static sfTexture* QUIT_TEXTURE;
static sfTexture* MENU_TEXTURE;
static sfTexture* P1_M_TEXTURE;
static sfTexture* P2_M_TEXTURE;
static float      ARROW_ANIM_TIME = 0.f;
static float      ARROW_BASE_Y;

static sfTexture* LoadTex(const char* path)
{
    sfTexture* t = sfTexture_createFromFile(path, NULL);
    if (!t) printf("ERREUR: impossible de charger %s\n", path);
    return t;
}

void Renderer_Load()
{
    NORMAL_TEXTURE = LoadTex("./Assets/normal.png");
    P1_TEXTURE = LoadTex("./Assets/p1.png");
    P2_TEXTURE = LoadTex("./Assets/p2.png");
    BACKGROUND_TEXTURE = LoadTex("./Assets/background.png");
    OVERLAY_TEXTURE = LoadTex("./Assets/ending.png");
    RESTART_TEXTURE = LoadTex("./Assets/restart.png");
    QUIT_TEXTURE = LoadTex("./Assets/stop.png");
    MENU_TEXTURE = LoadTex("./Assets/menu.png");
    P1_M_TEXTURE = LoadTex("./Assets/p1_m.png");
    P2_M_TEXTURE = LoadTex("./Assets/p2_m.png");
    ARROW_TEXTURE = LoadTex("./Assets/arrow.png");

    BACKGROUND_SPRITE = sfSprite_create();
    if (BACKGROUND_SPRITE && BACKGROUND_TEXTURE)
    {
        sfSprite_setTexture(BACKGROUND_SPRITE, BACKGROUND_TEXTURE, sfTrue);
        sfFloatRect b = sfSprite_getLocalBounds(BACKGROUND_SPRITE);
        float sx = (b.width > 0.f) ? (float)WINDOW_WIDTH / b.width : 1.f;
        float sy = (b.height > 0.f) ? (float)WINDOW_HEIGHT / b.height : 1.f;
        sfSprite_setScale(BACKGROUND_SPRITE, (sfVector2f) { sx, sy });
    }

    ARROW_BASE_Y = OFFSET_TOP - CELL_SIZE * 1.5f;
    float arrowSize = CELL_SIZE * 0.8f;

    for (int col = 0; col < GRID_COLS; col++)
    {
        ARROW_SPRITES[col] = sfSprite_create();
        if (ARROW_SPRITES[col] && ARROW_TEXTURE)
        {
            sfSprite_setTexture(ARROW_SPRITES[col], ARROW_TEXTURE, sfTrue);
            sfFloatRect b = sfSprite_getLocalBounds(ARROW_SPRITES[col]);
            float sx = (b.width > 0.f) ? arrowSize / b.width : 1.f;
            float sy = (b.height > 0.f) ? arrowSize / b.height : 1.f;
            sfSprite_setScale(ARROW_SPRITES[col], (sfVector2f) { sx, sy });
            float offsetX = (CELL_SIZE - arrowSize) / 2.f;
            sfSprite_setPosition(ARROW_SPRITES[col], (sfVector2f) {
                (float)(OFFSET_SIDE + col * CELL_SIZE) + offsetX, ARROW_BASE_Y
            });
        }
    }
}

void Renderer_Unload()
{
    if (NORMAL_TEXTURE) { sfTexture_destroy(NORMAL_TEXTURE);     NORMAL_TEXTURE = NULL; }
    if (P1_TEXTURE) { sfTexture_destroy(P1_TEXTURE);         P1_TEXTURE = NULL; }
    if (P2_TEXTURE) { sfTexture_destroy(P2_TEXTURE);         P2_TEXTURE = NULL; }
    if (BACKGROUND_SPRITE) { sfSprite_destroy(BACKGROUND_SPRITE);   BACKGROUND_SPRITE = NULL; }
    if (BACKGROUND_TEXTURE) { sfTexture_destroy(BACKGROUND_TEXTURE); BACKGROUND_TEXTURE = NULL; }
    if (OVERLAY_TEXTURE) { sfTexture_destroy(OVERLAY_TEXTURE);    OVERLAY_TEXTURE = NULL; }
    if (RESTART_TEXTURE) { sfTexture_destroy(RESTART_TEXTURE);    RESTART_TEXTURE = NULL; }
    if (QUIT_TEXTURE) { sfTexture_destroy(QUIT_TEXTURE);       QUIT_TEXTURE = NULL; }
    if (MENU_TEXTURE) { sfTexture_destroy(MENU_TEXTURE);       MENU_TEXTURE = NULL; }
    if (P1_M_TEXTURE) { sfTexture_destroy(P1_M_TEXTURE);       P1_M_TEXTURE = NULL; }
    if (P2_M_TEXTURE) { sfTexture_destroy(P2_M_TEXTURE);       P2_M_TEXTURE = NULL; }
    for (int col = 0; col < GRID_COLS; col++)
        if (ARROW_SPRITES[col]) { sfSprite_destroy(ARROW_SPRITES[col]); ARROW_SPRITES[col] = NULL; }
    if (ARROW_TEXTURE) { sfTexture_destroy(ARROW_TEXTURE);      ARROW_TEXTURE = NULL; }
}

void Renderer_UpdateArrows(float deltaTime)
{
    ARROW_ANIM_TIME = fmodf(ARROW_ANIM_TIME + deltaTime, 6.2832f);
    float bounce = sinf(ARROW_ANIM_TIME * 3.f) * 8.f;
    for (int col = 0; col < GRID_COLS; col++)
        if (ARROW_SPRITES[col])
        {
            float arrowSize2 = CELL_SIZE * 0.8f;
            float offsetX = (CELL_SIZE - arrowSize2) / 2.f;
            sfSprite_setPosition(ARROW_SPRITES[col], (sfVector2f) {
                (float)(OFFSET_SIDE + col * CELL_SIZE) + offsetX,
                    ARROW_BASE_Y + bounce
            });
        }
}

void Renderer_DrawBackground(sfRenderWindow* window)
{
    if (BACKGROUND_SPRITE)
        sfRenderWindow_drawSprite(window, BACKGROUND_SPRITE, NULL);
}

void Renderer_DrawArrows(sfRenderWindow* window)
{
    for (int col = 0; col < GRID_COLS; col++)
        if (ARROW_SPRITES[col])
            sfRenderWindow_drawSprite(window, ARROW_SPRITES[col], NULL);
}

void Renderer_DrawFallingToken(sfRenderWindow* window, FallingToken* ft)
{
    if (ft && ft->active && ft->sprite)
        sfRenderWindow_drawSprite(window, ft->sprite, NULL);
}

void Renderer_DrawOverlay(sfRenderWindow* window,
    sfSprite* overlaySprite, sfText* overlayText,
    sfSprite* restartSprite, sfSprite* quitSprite)
{
    if (overlaySprite) sfRenderWindow_drawSprite(window, overlaySprite, NULL);
    if (overlayText)   sfRenderWindow_drawText(window, overlayText, NULL);
    if (restartSprite) sfRenderWindow_drawSprite(window, restartSprite, NULL);
    if (quitSprite)    sfRenderWindow_drawSprite(window, quitSprite, NULL);
}

sfSprite* Renderer_CreateScaledSprite(sfTexture* tex, float w, float h)
{
    if (!tex) return NULL;
    sfSprite* s = sfSprite_create();
    if (!s) return NULL;
    sfSprite_setTexture(s, tex, sfTrue);
    sfFloatRect b = sfSprite_getLocalBounds(s);
    float sx = (b.width > 0.f) ? w / b.width : 1.f;
    float sy = (b.height > 0.f) ? h / b.height : 1.f;
    sfSprite_setScale(s, (sfVector2f) { sx, sy });
    return s;
}

sfSprite* Renderer_GetArrowSprite(int col) { return (col >= 0 && col < GRID_COLS) ? ARROW_SPRITES[col] : NULL; }
sfTexture* Renderer_GetTexture(enum CellType t) { return (t == PLAYER1) ? P1_TEXTURE : (t == PLAYER2) ? P2_TEXTURE : NULL; }
sfTexture* Renderer_GetNormalTexture() { return NORMAL_TEXTURE; }
sfTexture* Renderer_GetOverlayTexture() { return OVERLAY_TEXTURE; }
sfTexture* Renderer_GetRestartTexture() { return RESTART_TEXTURE; }
sfTexture* Renderer_GetQuitTexture() { return QUIT_TEXTURE; }
sfTexture* Renderer_GetMenuTexture() { return MENU_TEXTURE; }
sfTexture* Renderer_GetCharTexture(enum CellType t) { return (t == PLAYER1) ? P1_M_TEXTURE : P2_M_TEXTURE; }