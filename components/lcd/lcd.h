#ifndef LCD_H
#define LCD_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "../types/types.h"

#define PICO_I2C_SDA_PIN 20
#define PICO_I2C_SCL_PIN 21
#define I2C_BAUDRATE 100000
#define LCD_CONTROLLER_ADDR 8

void lcd_init();
void lcd_send_raw(char msg_str[]);
void lcd_update_notes(bool pressed[], bool pressed_old);

#endif