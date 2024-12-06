#include <inttypes.h>

extern uint8_t ounces_to_pour;

extern uint8_t pouring;

void init_buttons();
int8_t read_button(int idx);
void handle_button(int idx);
