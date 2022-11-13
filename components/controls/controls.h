#ifndef CONTROLS_H
#define CONTROLS_H

#include "pico/stdlib.h"
#include "../types/types.h"
#include <string.h>

void buttons_pooling(BtnStack_t btn_stack[]);
void buttons_init(uint8_t button_pins[]);

#endif