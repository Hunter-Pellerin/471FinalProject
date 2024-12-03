#include "buttons.h"
#include "debounce.h"

#define DOWN_BUTTON 0
#define UP_BUTTON 1
#define POUR_BUTTON 2

int main()
{
    init_buttons();

    while (1)
    {
        button_state_t down_button = get_button(DOWN_BUTTON);
        button_state_t up_button = get_button(UP_BUTTON);
        button_state_t pour_button = get_button(POUR_BUTTON);
    }
}