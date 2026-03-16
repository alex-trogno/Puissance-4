#include "Game.h"

sfFont* GAME_FONT;
static sfTexture* ARROW_TEXTURE;

static void LoadAllTextures()
{
    ARROW_TEXTURE = sfTexture_createFromFile("./Assets/Arrow_R.png", NULL);
    if (!ARROW_TEXTURE)
        printf("ERREUR: impossible de charger Arrow_R.png\n");

    GAME_FONT = sfFont_createFromFile("./Assets/Geo-Regular.ttf");
    if (!GAME_FONT)
        printf("ERREUR: impossible de charger Geo-Regular.ttf\n");
}

Affichage* CreateAffichage()
{
    LoadAllTextures();

    Affichage* menu = (Affichage*)malloc(sizeof(Affichage));
    if (!menu) {
        printf("ERROR: Failed to allocate Affichage!\n");
        return NULL;
    }

    menu->leftArrowSprite = sfSprite_create();
    menu->rightArrowSprite = sfSprite_create();

    sfSprite_setTexture(menu->leftArrowSprite, ARROW_TEXTURE, sfTrue);
    sfSprite_setRotation(menu->leftArrowSprite, 180);
    sfSprite_setTexture(menu->rightArrowSprite, ARROW_TEXTURE, sfTrue);

    sfSprite_setOrigin(menu->leftArrowSprite, (sfVector2f) { 32.f, 32.f });
    sfSprite_setOrigin(menu->rightArrowSprite, (sfVector2f) { 32.f, 32.f });

    sfSprite_setPosition(menu->leftArrowSprite, (sfVector2f) { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 });
    sfSprite_setPosition(menu->rightArrowSprite, (sfVector2f) { WINDOW_WIDTH - (WINDOW_WIDTH / 4), WINDOW_HEIGHT / 2 });

    menu->currentMapText = NULL;
    menu->modeIconSprite = NULL;
    menu->modeText = NULL;
    menu->TextureIconSprite = NULL;
    menu->TextureSelect = NULL;

    return menu;
}