#ifndef CONTROLS_H
#define CONTROLS_H

#define NUM_OF_BTNS 16
#include "pico/stdlib.h"


uint16_t check_buttons_state(uint8_t button_pins[]);
void buttons_init(uint8_t button_pins[]);

#endif