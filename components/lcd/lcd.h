#ifndef LCD_H
#define LCD_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "../types/types.h"
#include <stdio.h>

#define PICO_I2C_SDA_PIN 20
#define PICO_I2C_SCL_PIN 21
#define I2C_BAUDRATE 100000
#define LCD_CONTROLLER_ADDR 80
#define CMD_MAX_LENGTH 50

void lcd_init();
void lcd_send_raw(char msg_str[]);
void lcd_update_notes(bool pressed[], bool pressed_old);
void lcd_print(char msg[]);
void lcd_print_int(uint32_t num);
void lcd_clear();
void lcd_home();
// void lcd_debug_ccstack(CCStack_t *ccstack);
// void lcd_debug_btn_stack(BtnStack_t *btn_stack);

#endif