#include "lcd.h"

static void fill_msg(char msg_str[]){
    uint8_t size = 0;
    uint8_t to_fill;
    while (msg_str[size] != '\0')
    {
        size++;
    }
    to_fill = MSG_MAXSIZE - size;
    while (to_fill > 0)
    {
        msg_str[size+to_fill-1] = ' ';
        to_fill--;
    }
    msg_str[MSG_MAXSIZE-1]='\0';
}

void send_to_lcd(char msg_str[]){
    fill_msg(msg_str);
    if(i2c_get_write_available(i2c_default)){
        i2c_write_blocking(i2c_default, LCD_CONTROLLER_ADDR, msg_str, MSG_MAXSIZE, true);
    }
}

void lcd_init(){
    i2c_init(i2c_default, I2C_BAUDRATE);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_I2C_SDA_PIN);
    gpio_pull_up(PICO_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_I2C_SDA_PIN, PICO_I2C_SCL_PIN, GPIO_FUNC_I2C));
}