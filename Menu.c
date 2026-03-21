#define _CRT_SECURE_NO_WARNINGS
#include "Menu.h"

static sfTexture* LoadTex(const char* path)
{
    sfTexture* t = sfTexture_createFromFile(path, NULL);
    if (!t) printf("ERREUR: impossible de charger %s\n", path);
    return t;
}

static void RefreshInputText(Menu* menu, int field)
{
    sfText* txt = (field == 1) ? menu->input1Text : menu->input2Text;
    char* name = (field == 1) ? menu->name1 : menu->name2;
    sfRectangleShape* box = (field == 1) ? menu->box1 : menu->box2;

    if (!txt) return;

    char display[64];
    if (name[0] == '\0')
    {
        snprintf(display, sizeof(display), "Joueur %d", field);
        sfText_setFillColor(txt, sfColor_fromRGBA(200, 200, 200, 180));
    }
    else
    {
        strncpy(display, name, sizeof(display) - 1);
        display[sizeof(display) - 1] = '\0';
        sfText_setFillColor(txt, sfWhite);
    }
    sfText_setString(txt, display);

    if (box)
    {
        sfColor active = sfColor_fromRGB(100, 180, 255);
        sfColor inactive = sfColor_fromRGBA(255, 255, 255, 80);
        sfRectangleShape_setOutlineColor(box,
            (menu->activeField == field) ? active : inactive);
    }
}

Menu* CreateMenu()
{
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    if (!menu) return NULL;

    menu->name1[0] = '\0';
    menu->name2[0] = '\0';
    menu->activeField = 0;
    menu->readyToPlay = 0;
    menu->readyToQuit = 0;
    menu->quitButtonSprite = NULL;
    menu->quitButtonTexture = NULL;

    menu->backgroundTexture = LoadTex("./Assets/background.png");
    menu->backgroundSprite = sfSprite_create();
    if (menu->backgroundSprite && menu->backgroundTexture)
    {
        sfSprite_setTexture(menu->backgroundSprite, menu->backgroundTexture, sfTrue);
        sfFloatRect b = sfSprite_getLocalBounds(menu->backgroundSprite);
        float sx = (b.width > 0.f) ? (float)WINDOW_WIDTH / b.width : 1.f;
        float sy = (b.height > 0.f) ? (float)WINDOW_HEIGHT / b.height : 1.f;
        sfSprite_setScale(menu->backgroundSprite, (sfVector2f) { sx, sy });
    }

    menu->playButtonTexture = LoadTex("./Assets/play.png");
    menu->playButtonSprite = sfSprite_create();
    if (menu->playButtonSprite && menu->playButtonTexture)
    {
        sfSprite_setTexture(menu->playButtonSprite, menu->playButtonTexture, sfTrue);
        sfFloatRect b = sfSprite_getLocalBounds(menu->playButtonSprite);
        float bw = 220.f, bh = 65.f;
        float sx = (b.width > 0.f) ? bw / b.width : 1.f;
        float sy = (b.height > 0.f) ? bh / b.height : 1.f;
        sfSprite_setScale(menu->playButtonSprite, (sfVector2f) { sx, sy });
        sfSprite_setPosition(menu->playButtonSprite, (sfVector2f) {
            WINDOW_WIDTH / 2.f - bw / 2.f,
                WINDOW_HEIGHT * 0.78f
        });
    }

    float charH = 120.f;

    menu->p1Texture = LoadTex("./Assets/p1_m.png");
    menu->p1Sprite = sfSprite_create();
    if (menu->p1Sprite && menu->p1Texture)
    {
        sfSprite_setTexture(menu->p1Sprite, menu->p1Texture, sfTrue);
        sfFloatRect b = sfSprite_getLocalBounds(menu->p1Sprite);
        float scale = (b.height > 0.f) ? charH / b.height : 1.f;
        sfSprite_setScale(menu->p1Sprite, (sfVector2f) { scale, scale });
    }

    menu->p2Texture = LoadTex("./Assets/p2_m.png");
    menu->p2Sprite = sfSprite_create();
    if (menu->p2Sprite && menu->p2Texture)
    {
        sfSprite_setTexture(menu->p2Sprite, menu->p2Texture, sfTrue);
        sfFloatRect b = sfSprite_getLocalBounds(menu->p2Sprite);
        float scale = (b.height > 0.f) ? charH / b.height : 1.f;
        sfSprite_setScale(menu->p2Sprite, (sfVector2f) { scale, scale });
    }

    menu->quitButtonTexture = LoadTex("./Assets/stop.png");
    menu->quitButtonSprite = sfSprite_create();
    if (menu->quitButtonSprite && menu->quitButtonTexture)
    {
        sfSprite_setTexture(menu->quitButtonSprite, menu->quitButtonTexture, sfTrue);
        sfFloatRect b = sfSprite_getLocalBounds(menu->quitButtonSprite);
        float bw = 220.f, bh = 65.f;
        float sx = (b.width > 0.f) ? bw / b.width : 1.f;
        float sy = (b.height > 0.f) ? bh / b.height : 1.f;
        sfSprite_setScale(menu->quitButtonSprite, (sfVector2f) { sx, sy });
        sfSprite_setPosition(menu->quitButtonSprite, (sfVector2f) {
            WINDOW_WIDTH / 2.f - bw / 2.f,
                WINDOW_HEIGHT * 0.88f
        });
    }

    menu->font = sfFont_createFromFile("./Assets/Geo-Regular.ttf");
    if (!menu->font) printf("ERREUR: impossible de charger Geo-Regular.ttf\n");

    /* Titre */
    menu->titleText = sfText_create();
    if (menu->titleText && menu->font)
    {
        sfText_setString(menu->titleText, "Puissance 4");
        sfText_setFont(menu->titleText, menu->font);
        sfText_setCharacterSize(menu->titleText, 64);
        sfText_setFillColor(menu->titleText, sfWhite);
        sfText_setOutlineColor(menu->titleText, sfBlack);
        sfText_setOutlineThickness(menu->titleText, 4.f);
        sfFloatRect tb = sfText_getLocalBounds(menu->titleText);
        sfText_setOrigin(menu->titleText, (sfVector2f) { tb.width / 2.f, tb.height / 2.f });
        sfText_setPosition(menu->titleText, (sfVector2f) {
            WINDOW_WIDTH / 2.f, WINDOW_HEIGHT * 0.15f
        });
    }

    float boxW = 260.f, boxH = 48.f;
    float col1X = WINDOW_WIDTH / 2.f - boxW - 40.f;
    float col2X = WINDOW_WIDTH / 2.f + 40.f;
    float rowY = WINDOW_HEIGHT * 0.5f;

    float btnY = WINDOW_HEIGHT * 0.78f;

    if (menu->p1Sprite)
    {
        sfFloatRect b = sfSprite_getGlobalBounds(menu->p1Sprite);
        sfSprite_setPosition(menu->p1Sprite, (sfVector2f) {
            col1X - b.width + 120.f,
                btnY
        });
    }
    if (menu->p2Sprite)
    {
        sfFloatRect b = sfSprite_getGlobalBounds(menu->p2Sprite);
        sfSprite_setPosition(menu->p2Sprite, (sfVector2f) {
            col2X + boxW - 120.f,
                btnY
        });
    }

    /* Labels */
    menu->label1 = sfText_create();
    menu->label2 = sfText_create();
    if (menu->font)
    {
        for (int i = 0; i < 2; i++)
        {
            sfText* lbl = (i == 0) ? menu->label1 : menu->label2;
            float x = (i == 0) ? col1X : col2X;
            if (!lbl) continue;
            sfText_setFont(lbl, menu->font);
            sfText_setCharacterSize(lbl, 26);
            sfText_setFillColor(lbl, sfWhite);
            sfText_setOutlineColor(lbl, sfBlack);
            sfText_setOutlineThickness(lbl, 2.f);
            sfText_setString(lbl, (i == 0) ? "Joueur 1" : "Joueur 2");
            sfText_setPosition(lbl, (sfVector2f) { x, rowY - 44.f });
        }
    }

    /* Boites de saisie */
    menu->box1 = sfRectangleShape_create();
    menu->box2 = sfRectangleShape_create();
    for (int i = 0; i < 2; i++)
    {
        sfRectangleShape* box = (i == 0) ? menu->box1 : menu->box2;
        float x = (i == 0) ? col1X : col2X;
        if (!box) continue;
        sfRectangleShape_setSize(box, (sfVector2f) { boxW, boxH });
        sfRectangleShape_setFillColor(box, sfColor_fromRGBA(0, 0, 0, 120));
        sfRectangleShape_setOutlineThickness(box, 2.f);
        sfRectangleShape_setOutlineColor(box, sfColor_fromRGBA(255, 255, 255, 80));
        sfRectangleShape_setPosition(box, (sfVector2f) { x, rowY });
    }

    /* Textes de saisie */
    menu->input1Text = sfText_create();
    menu->input2Text = sfText_create();
    for (int i = 0; i < 2; i++)
    {
        sfText* txt = (i == 0) ? menu->input1Text : menu->input2Text;
        float x = (i == 0) ? col1X : col2X;
        if (!txt || !menu->font) continue;
        sfText_setFont(txt, menu->font);
        sfText_setCharacterSize(txt, 24);
        sfText_setPosition(txt, (sfVector2f) { x + 10.f, rowY + 8.f });
        RefreshInputText(menu, i + 1);
    }

    return menu;
}

void DestroyMenu(Menu* menu)
{
    if (!menu) return;
    if (menu->quitButtonSprite)  sfSprite_destroy(menu->quitButtonSprite);
    if (menu->quitButtonTexture) sfTexture_destroy(menu->quitButtonTexture);
    if (menu->p1Sprite)          sfSprite_destroy(menu->p1Sprite);
    if (menu->p1Texture)         sfTexture_destroy(menu->p1Texture);
    if (menu->p2Sprite)          sfSprite_destroy(menu->p2Sprite);
    if (menu->p2Texture)         sfTexture_destroy(menu->p2Texture);
    if (menu->backgroundSprite)  sfSprite_destroy(menu->backgroundSprite);
    if (menu->backgroundTexture) sfTexture_destroy(menu->backgroundTexture);
    if (menu->playButtonSprite)  sfSprite_destroy(menu->playButtonSprite);
    if (menu->playButtonTexture) sfTexture_destroy(menu->playButtonTexture);
    if (menu->titleText)         sfText_destroy(menu->titleText);
    if (menu->label1)            sfText_destroy(menu->label1);
    if (menu->label2)            sfText_destroy(menu->label2);
    if (menu->input1Text)        sfText_destroy(menu->input1Text);
    if (menu->input2Text)        sfText_destroy(menu->input2Text);
    if (menu->box1)              sfRectangleShape_destroy(menu->box1);
    if (menu->box2)              sfRectangleShape_destroy(menu->box2);
    if (menu->font)              sfFont_destroy(menu->font);
    free(menu);
}

void UpdateMenu(Menu* menu)
{
    (void)menu;
}

void DrawMenu(sfRenderWindow* window, Menu* menu)
{
    if (!window || !menu) return;
    if (menu->backgroundSprite) sfRenderWindow_drawSprite(window, menu->backgroundSprite, NULL);
    if (menu->titleText)        sfRenderWindow_drawText(window, menu->titleText, NULL);
    if (menu->p1Sprite)         sfRenderWindow_drawSprite(window, menu->p1Sprite, NULL);
    if (menu->p2Sprite)         sfRenderWindow_drawSprite(window, menu->p2Sprite, NULL);
    if (menu->label1)           sfRenderWindow_drawText(window, menu->label1, NULL);
    if (menu->label2)           sfRenderWindow_drawText(window, menu->label2, NULL);
    if (menu->box1)             sfRenderWindow_drawRectangleShape(window, menu->box1, NULL);
    if (menu->box2)             sfRenderWindow_drawRectangleShape(window, menu->box2, NULL);
    if (menu->input1Text)       sfRenderWindow_drawText(window, menu->input1Text, NULL);
    if (menu->input2Text)       sfRenderWindow_drawText(window, menu->input2Text, NULL);
    if (menu->playButtonSprite)  sfRenderWindow_drawSprite(window, menu->playButtonSprite, NULL);
    if (menu->quitButtonSprite)  sfRenderWindow_drawSprite(window, menu->quitButtonSprite, NULL);
}

void HandleTextMenu(Menu* menu, sfUint32 unicode)
{
    if (!menu || menu->activeField == 0) return;

    char* name = (menu->activeField == 1) ? menu->name1 : menu->name2;
    int   len = (int)strlen(name);

    if (unicode == 8) /* Backspace */
    {
        if (len > 0) name[len - 1] = '\0';
    }
    else if (unicode >= 32 && unicode < 127 && len < 31)
    {
        name[len] = (char)unicode;
        name[len + 1] = '\0';
    }

    RefreshInputText(menu, menu->activeField);
}

void HandleClickMenu(Menu* menu, int mouseX, int mouseY)
{
    if (!menu) return;

    /* Clic bouton jouer */
    if (menu->playButtonSprite)
    {
        sfFloatRect b = sfSprite_getGlobalBounds(menu->playButtonSprite);
        if (sfFloatRect_contains(&b, (float)mouseX, (float)mouseY))
        {
            menu->readyToPlay = 1;
            return;
        }
    }

    /* Clic bouton quitter */
    if (menu->quitButtonSprite)
    {
        sfFloatRect b = sfSprite_getGlobalBounds(menu->quitButtonSprite);
        if (sfFloatRect_contains(&b, (float)mouseX, (float)mouseY))
        {
            menu->readyToQuit = 1;
            return;
        }
    }

    /* Clic boite 1 */
    if (menu->box1)
    {
        sfFloatRect b = sfRectangleShape_getGlobalBounds(menu->box1);
        if (sfFloatRect_contains(&b, (float)mouseX, (float)mouseY))
        {
            menu->activeField = 1;
            RefreshInputText(menu, 1);
            RefreshInputText(menu, 2);
            return;
        }
    }

    /* Clic boite 2 */
    if (menu->box2)
    {
        sfFloatRect b = sfRectangleShape_getGlobalBounds(menu->box2);
        if (sfFloatRect_contains(&b, (float)mouseX, (float)mouseY))
        {
            menu->activeField = 2;
            RefreshInputText(menu, 1);
            RefreshInputText(menu, 2);
            return;
        }
    }

    menu->activeField = 0;
    RefreshInputText(menu, 1);
    RefreshInputText(menu, 2);
}

int MenuReadyToPlay(Menu* menu)
{
    return menu ? menu->readyToPlay : 0;
}

int MenuReadyToQuit(Menu* menu)
{
    return menu ? menu->readyToQuit : 0;
}

void MenuGetNames(Menu* menu, char* out1, char* out2)
{
    if (!menu) return;
    strncpy(out1, menu->name1[0] != '\0' ? menu->name1 : "Joueur 1", 31);
    strncpy(out2, menu->name2[0] != '\0' ? menu->name2 : "Joueur 2", 31);
    out1[31] = '\0';
    out2[31] = '\0';
}