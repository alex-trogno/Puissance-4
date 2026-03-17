#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <time.h>

#include "Basic.h"
#include "Game.h"

int main(void)
{
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
    sfRenderWindow* window;
    sfEvent event;
    sfClock* clock;

    window = sfRenderWindow_create(mode, "Puissance 4", sfClose, NULL);
    if (!window)
        return FAILURE;

    srand((unsigned int)time(NULL));

    Game* game = CreateGame();
    clock = sfClock_create();

    while (sfRenderWindow_isOpen(window))
    {
        float deltaTime = sfTime_asSeconds(sfClock_restart(clock));

        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        UpdateGame(game, deltaTime);

        sfRenderWindow_clear(window, sfColor_fromRGB(0, 174, 192));
        DrawGame(window, game);
        sfRenderWindow_display(window);
    }

    DestroyGame(game);
    sfClock_destroy(clock);
    sfRenderWindow_destroy(window);

    return SUCCESS;
}