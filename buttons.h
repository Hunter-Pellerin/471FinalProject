#include <inttypes.h>

static uint8_t ounces_to_pour = 8;

static uint8_t pouring = 0;

void init_buttons();
void read_button(int idx);