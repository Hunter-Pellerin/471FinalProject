#include <inttypes.h>

static uint8_t ounces_to_pour = 0;

static uint8_t pour_button = 0;

void init_buttons();
void read_button(int idx);