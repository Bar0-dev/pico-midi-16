#ifndef PICOMUX_H
#define PICOMUX_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define CHANNELS 16
#define ADC_CHANNEL 2
#define SELECT_PINS 4
#define MAX_VOUT 3.0f

void mux_init(uint8_t select[], uint8_t sig, bool out, bool analog);
void mux_read(uint16_t reads[], uint8_t length);

#endif