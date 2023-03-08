#ifndef MUX_H
#define MUX_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "../types/types.h"
#include "../lcd/lcd.h"

#define ADC_CHANNEL 1
#define NUM_OF_SEL_PINS 4

void mux_init(uint8_t select_pins[], uint8_t signal_pin, bool direction_is_out, bool read_is_analog);
void mux_read(uint16_t redings[], uint8_t length, uint8_t channels[]);

#endif