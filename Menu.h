#pragma once

#include "Basic.h"

typedef struct Menu
{
    sfSprite* backgroundSprite;
    sfTexture* backgroundTexture;
    sfSprite* playButtonSprite;
    sfTexture* playButtonTexture;
    sfText* titleText;
    sfFont* font;

    sfSprite* p1Sprite;
    sfTexture* p1Texture;
    sfSprite* p2Sprite;
    sfTexture* p2Texture;
    sfText* label1;
    sfText* label2;
    sfText* input1Text;
    sfText* input2Text;
    sfRectangleShape* box1;
    sfRectangleShape* box2;

    char         name1[32];
    char         name2[32];
    int          activeField;   /* 0 = aucun, 1 = champ1, 2 = champ2 */
    int          readyToPlay;
    int          readyToQuit;
    sfSprite* quitButtonSprite;
    sfTexture* quitButtonTexture;
} Menu;

Menu* CreateMenu();
void  DestroyMenu(Menu* menu);
void  UpdateMenu(Menu* menu);
void  DrawMenu(sfRenderWindow* window, Menu* menu);
void  HandleTextMenu(Menu* menu, sfUint32 unicode);
void  HandleClickMenu(Menu* menu, int mouseX, int mouseY);
int   MenuReadyToPlay(Menu* menu);
int   MenuReadyToQuit(Menu* menu);
void  MenuGetNames(Menu* menu, char* out1, char* out2);