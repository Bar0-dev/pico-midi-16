#ifndef PICOMUX_H
#define PICOMUX_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "../types/types.h"

#include "../lcd/lcd.h"

#define CHANNELS 16
#define ADC_CHANNEL 1
#define SELECT_PINS 4
#define MAX_VOUT 3.0f
#define MIN_VOUT 0.2f
#define POT_THRESHOLD 2
#define BTN_THRESHOLD 2000

void mux_init(uint8_t select[], uint8_t sig, uint8_t channels[], bool out, bool analog);
void cc_read(CCStack_t *cc_stack, uint8_t current[], uint8_t old[]);
void aux_btns_read(uint8_t btn_channels[], uint16_t state[]);

#endif