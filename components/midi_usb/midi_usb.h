#ifndef MIDI_USB_H
#define MIDI_USB_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "bsp/board.h"
#include "tusb.h"

void midi_init(void);
void led_blinking_task(void);
void midi_task(void);

#endif