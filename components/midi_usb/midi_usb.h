#ifndef MIDI_USB_H
#define MIDI_USB_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "bsp/board.h"
#include "tusb.h"

#define NUM_OF_BTNS 16

void midi_init(void);
uint32_t led_interval(void);
void midi_send(uint16_t btns_pressed, uint16_t *btns_pressed_old);

#endif