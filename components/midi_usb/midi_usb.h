#ifndef MIDI_USB_H
#define MIDI_USB_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "bsp/board.h"
#include "tusb.h"
#include "../types/types.h"

void midi_init(void);
uint32_t led_interval(void);
void midi_start(uint8_t btn);
void midi_stop(uint8_t btn);


#endif