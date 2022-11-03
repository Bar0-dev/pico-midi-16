#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define PICO_I2C_SDA_PIN 20
#define PICO_I2C_SCL_PIN 21
#define I2C_BAUDRATE 100000
#define LCD_CONTROLLER_ADDR 8
#define MSG_MAXSIZE 32
#define BUTTONS_NUM 16

void fill_msg(char msg_str[]){
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
}

void send_to_lcd(char msg_str[]){
    // fill_msg(msg_str);
    if(i2c_get_write_available(i2c_default)){
        i2c_write_blocking(i2c_default, LCD_CONTROLLER_ADDR, msg_str, MSG_MAXSIZE, true);
    }
}

uint16_t update_lcd(uint16_t pressed, uint16_t pressed_old){
    char msg[MSG_MAXSIZE];
    // if(pressed != pressed_old){
        for(int i=0; i<BUTTONS_NUM+1; i++){
            if(i == 8){
                msg[8]='\n';
                continue;
            }
            if(pressed & (1<<i)){
                msg[i]='*';
            } else {
                msg[i]='O';
            }
        }
        msg[BUTTONS_NUM+1] = '\0';
        printf("\n\n %s \n\n", msg);
        send_to_lcd(msg);
        return pressed;
    // }
    // return pressed_old;
}

uint16_t check_buttons_state(uint8_t button_pins[]){
    uint16_t pressed;
    for(int i=0; i<BUTTONS_NUM; i++){
        //all pullups, check for low state
        if(!gpio_get(button_pins[i])){
            pressed |= 1 << i;
        } else {
            pressed &= ~(1<<i);
        }
    }
    return pressed;
}

void buttons_init(uint8_t button_pins[], uint8_t buttons_num){
    for(int i=0; i<buttons_num; i++){
        gpio_init(button_pins[i]);
        gpio_set_dir(button_pins[i], false);
        gpio_pull_up(button_pins[i]);
    }
}

int main() {
    //variables
    uint8_t btn_pins[BUTTONS_NUM];
    //button_pins = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10. 11. 12, 13, 14, 15];
    //for buttons connected dicetly to th pico GPIO in order from 0 -> 16
    for(int i=0; i<BUTTONS_NUM; i++){
        btn_pins[i] = i;
    }
    //16 bit variable reperesents 16 buttons inputs state, one bit for each
    //if more buttons is used this needs to be expanded to 32 bit variable or even larger
    uint16_t btn_pressed = 0;
    uint16_t btn_pressed_old = -1;

    //setup
    stdio_init_all();
    i2c_init(i2c_default, I2C_BAUDRATE);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_I2C_SDA_PIN);
    gpio_pull_up(PICO_I2C_SCL_PIN);
    buttons_init(btn_pins, BUTTONS_NUM);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_I2C_SDA_PIN, PICO_I2C_SCL_PIN, GPIO_FUNC_I2C));
    sleep_ms(500);
    while(1){
        btn_pressed = check_buttons_state(btn_pins);
        btn_pressed_old = update_lcd(btn_pressed, btn_pressed_old);
    } 
    return 0;
}