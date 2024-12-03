#include <inttypes.h>

static uint8_t ounces_to_pour = 8;

static uint8_t pouring = 0;

void init_buttons();
int8_t read_button(int idx);
void handle_button(int idx);
