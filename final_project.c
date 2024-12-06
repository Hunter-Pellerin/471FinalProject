#include "buttons.h"
#include "debounce.h"
#include "display.h"
#include "pouring.h"
#include "gpio.h"
#include <stdio.h>

#define DOWN_BUTTON 0
#define UP_BUTTON 1
#define POUR_BUTTON 2

int main()
{
    open_gpio();
    init_buttons();
    int disp_fd = init_display();
    gpio_init();

    while (1)
    {
        handle_button(DOWN_BUTTON);
        handle_button(UP_BUTTON);
        handle_button(POUR_BUTTON);

        pour(pouring);

        if (pouring)
        {
            display_pouring(disp_fd);
        }
        else
        {
            display_oz(disp_fd, ounces_to_pour);
        }
    }
}
