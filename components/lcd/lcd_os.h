#ifndef LCD_OS_H
#define LCD_OS_H

#include <stdint.h>
#include "../types/types.h"
#include "lcd.h"

void lcd_os_show_edit();
void lcd_os_show_home();
void lcd_os_show_setting(uint8_t id, uint8_t note_val, uint8_t octave);
void lcd_os_show_note(uint8_t note_val, uint8_t octave);
    
#endif