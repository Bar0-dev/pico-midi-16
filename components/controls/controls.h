#ifndef CONTROLS_H
#define CONTROLS_H

#include <string.h>
#include "pico/stdlib.h"
#include "../mux/mux.h"
#include "../types/types.h"
#include "../midi_usb/midi_usb.h"
#include "../lcd/lcd_os.h"

#define CC_POT_THRESHOLD 3
#define AUX_BTN_THRESHOLD 2000
#define MODE_BTN 3
#define UP_BTN 2
#define DOWN_BTN 1
#define CHANGE_OCTAVE_BTN 0

void buttons_init(uint8_t button_pins[]);
void update_buttons(btnStack_t *btns);
void button_press_action(btnStack_t *btns, state_t *state_now);
void cc_init(uint8_t mux_channels[]);
void cc_update(ccStack_t *cc_stack);
void cc_change_action(ccStack_t *cc_stack, state_t *state_now);
void aux_btns_init(uint8_t mux_channels[], uint8_t menu_btn_pin);
void aux_btns_update(auxBtnStack_t *btns);
void aux_buttons_press_action(auxBtnStack_t *btns, state_t *state_now);

#endif