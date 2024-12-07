#ifndef BUTTONS_H
#define BUTTONS_H

#include <inttypes.h>
#include "pouring.h"

// Button IDs
#define DOWN_BUTTON 0
#define UP_BUTTON 1
#define POUR_BUTTON 2

// Variables used in buttons.c and final_project.c for subsystem communication
extern uint8_t ounces_to_pour;
extern pour_state_t pouring;

void init_buttons();
int8_t read_button(int idx);
void handle_button(int idx);

#endif