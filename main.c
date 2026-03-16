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

    /* Create the main window */
    window = sfRenderWindow_create(mode, "Puissance 4", sfClose, NULL);
    if (!window)
    {
        return FAILURE;
    }

    srand((unsigned int)time(NULL));

    Affichage* menu = CreateAffichage();

    /* Start the game loop */
    while (sfRenderWindow_isOpen(window))
    {
        /* Process events */
        while (sfRenderWindow_pollEvent(window, &event))
        {
            /* Close window : exit */
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
            }
        }

        /* Clear the screen */
        sfRenderWindow_clear(window, sfColor_fromRGB(0, 174, 192));

        /* Draw menu */
        if (menu)
        {
            sfRenderWindow_drawSprite(window, menu->leftArrowSprite, NULL);
            sfRenderWindow_drawSprite(window, menu->rightArrowSprite, NULL);
        }

        /* Update the window */
        sfRenderWindow_display(window);
    }

    /* Cleanup resources */
    sfRenderWindow_destroy(window);

    return SUCCESS;
}