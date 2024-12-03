#include "linux/gpio.h"
#include <stdint.h>

typedef enum
{
	BUTTON_RELEASED,
	BUTTON_PRESSED
} button_state_t;

void handleError(void);
int8_t read_button(int fd);