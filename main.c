#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <time.h>

#include "Basic.h"
#include "Game.h"
#include "Menu.h"

int main(void)
{
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
    sfRenderWindow* window;
    sfEvent event;
    sfClock* clock;

    window = sfRenderWindow_create(mode, "Puissance 4", sfClose, NULL);
    if (!window) return FAILURE;

    srand((unsigned int)time(NULL));
    clock = sfClock_create();

    enum Scene currentScene = SCENE_MENU;
    Menu* menu = CreateMenu();
    Game* game = NULL;

    while (sfRenderWindow_isOpen(window))
    {
        float deltaTime = sfTime_asSeconds(sfClock_restart(clock));

        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);

            if (currentScene == SCENE_MENU)
            {
                if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
                    HandleClickMenu(menu, event.mouseButton.x, event.mouseButton.y);
                if (event.type == sfEvtTextEntered)
                    HandleTextMenu(menu, event.text.unicode);
            }
            else if (currentScene == SCENE_GAME)
            {
                if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
                {
                    int result = HandleClickGame(game, event.mouseButton.x, event.mouseButton.y);
                    if (result == SCENE_MENU)
                    {
                        DestroyGame(game);
                        game = NULL;
                        menu = CreateMenu();
                        currentScene = SCENE_MENU;
                    }
                }
            }
        }

        if (currentScene == SCENE_MENU)
        {
            if (MenuReadyToQuit(menu))
                sfRenderWindow_close(window);

            if (MenuReadyToPlay(menu))
            {
                char n1[32], n2[32];
                MenuGetNames(menu, n1, n2);
                DestroyMenu(menu);
                menu = NULL;
                game = CreateGame(n1, n2);
                currentScene = SCENE_GAME;
            }
        }

        if (currentScene == SCENE_GAME)
            UpdateGame(game, deltaTime);

        sfRenderWindow_clear(window, sfColor_fromRGB(0, 174, 192));

        if (currentScene == SCENE_MENU)
            DrawMenu(window, menu);
        else if (currentScene == SCENE_GAME)
            DrawGame(window, game);

        sfRenderWindow_display(window);
    }

    if (menu) DestroyMenu(menu);
    if (game) DestroyGame(game);
    sfClock_destroy(clock);
    sfRenderWindow_destroy(window);

    return SUCCESS;
}