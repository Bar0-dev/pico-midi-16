#ifndef LCD_OS_H
#define LCD_OS_H

#include <stdint.h>
#include "../types/types.h"
#include "lcd.h"

void lcd_os_show_key_edit();
void lcd_os_show_cc_edit();
void lcd_os_show_midi_mode();
void lcd_os_show_drum_mode();
void lcd_os_show_key_setting(uint8_t id, uint8_t note_val);
void lcd_os_show_cc_setting(uint8_t id, uint8_t cc_id);
void lcd_os_show_note(uint8_t note_val);
void lcd_os_show_cc(uint8_t id, uint8_t cc_id, uint8_t cc_val);
void lcd_os_show_octave(uint8_t note_val);
    
#endif