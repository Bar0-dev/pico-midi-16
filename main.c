#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <stdlib.h>
#include "components/lcd/lcd.h"
#include "components/controls/controls.h"
#include "components/midi_usb/midi_usb.h"


int main() {
    //variables
    //For buttons connected dicetly to th pico GPIO in order from 0 -> 16
    uint8_t btn_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    /*16 bit variable reperesents 16 buttons inputs state, one bit for each
    if more buttons is used this needs to be expanded to 32 bit variable or higher*/
    uint16_t btn_pressed = 0;
    uint16_t btn_pressed_old = -1;
    
    //setup
    stdio_init_all();
    buttons_init(btn_pins);
    lcd_init();
    midi_init();

    //main loop
    while(1){
        btn_pressed = check_buttons_state(btn_pins);
        btn_pressed_old = update_lcd(btn_pressed, btn_pressed_old);
        tud_task(); // tinyusb device task
        led_blinking_task();
        midi_task();
    } 
    return 0;
}