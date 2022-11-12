#ifndef LCD_H
#define LCD_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#define PICO_I2C_SDA_PIN 20
#define PICO_I2C_SCL_PIN 21
#define I2C_BAUDRATE 100000
#define LCD_CONTROLLER_ADDR 8
#define MSG_MAXSIZE 32
#define NUM_OF_BTNS 16

void lcd_init();
void send_to_lcd(char msg_str[]);

#endif