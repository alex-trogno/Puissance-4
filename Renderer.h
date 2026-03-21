#pragma once

#include "Basic.h"

void       Renderer_Load();
void       Renderer_Unload();

void       Renderer_UpdateArrows(float deltaTime);
void       Renderer_DrawBackground(sfRenderWindow* window);
void       Renderer_DrawArrows(sfRenderWindow* window);
void       Renderer_DrawFallingToken(sfRenderWindow* window, FallingToken* ft);
void       Renderer_DrawOverlay(sfRenderWindow* window,
    sfSprite* overlaySprite, sfText* overlayText,
    sfSprite* restartSprite, sfSprite* quitSprite);

sfSprite* Renderer_CreateScaledSprite(sfTexture* tex, float w, float h);
sfSprite* Renderer_GetArrowSprite(int col);
sfTexture* Renderer_GetTexture(enum CellType type);
sfTexture* Renderer_GetNormalTexture();
sfTexture* Renderer_GetOverlayTexture();
sfTexture* Renderer_GetRestartTexture();
sfTexture* Renderer_GetQuitTexture();
sfTexture* Renderer_GetMenuTexture();
sfTexture* Renderer_GetCharTexture(enum CellType t);