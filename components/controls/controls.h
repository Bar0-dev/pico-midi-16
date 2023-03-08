#ifndef CONTROLS_H
#define CONTROLS_H

#include "pico/stdlib.h"
#include "../mux/mux.h"
#include "../types/types.h"
#include <string.h>

#define CC_POT_THRESHOLD 5
#define CC_BTN_THRESHOLD 2000
#define NUM_OF_AUX_BTNS 3

void buttons_pooling(BtnStack_t btn_stack[]);
void buttons_init(uint8_t button_pins[]);
void cc_init(uint8_t mux_channels[]);
void cc_update(ccStack_t *cc_stack);

#endif