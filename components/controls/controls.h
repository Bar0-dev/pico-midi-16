#ifndef CONTROLS_H
#define CONTROLS_H

#include "pico/stdlib.h"

#define NUM_OF_BTNS 16


uint16_t buttons_pooling(uint8_t button_pins[]);
void buttons_init(uint8_t button_pins[]);

#endif