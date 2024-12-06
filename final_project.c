#include "buttons.h"
#include "debounce.h"
#include "display.h"
#include "pouring.h"
#include "gpio.h"
#include <stdio.h>

int main()
{
    // Setup the gpios for buttons, display, and relay output
    open_gpio();
    init_buttons();
    int disp_fd = init_display();
    gpio_init();

    // Event loop
    while (1)
    {
        // Check for each button press
        // state variable pouring and quantity ounces_to_pour are updated
        handle_button(DOWN_BUTTON);
        handle_button(UP_BUTTON);
        handle_button(POUR_BUTTON);

        // Pours if pouring == POURING
        pour(pouring);

        // If pouring, display "Pour"
        if (pouring == POURING)
        {
            display_pouring(disp_fd);
        }
        // If not pouring, display the ounce selection
        else
        {
            display_oz(disp_fd, ounces_to_pour);
        }
    }
}
