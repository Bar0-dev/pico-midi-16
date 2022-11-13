#ifndef CONTROLS_H
#define CONTROLS_H

#include "pico/stdlib.h"

#define NUM_OF_BTNS 16

void buttons_pooling(uint8_t button_pins[], bool pressed[], bool *changed);
void buttons_init(uint8_t button_pins[]);

#endif