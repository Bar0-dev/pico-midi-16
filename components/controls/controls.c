#include "controls.h"

uint16_t check_buttons_state(uint8_t button_pins[]){
    uint16_t pressed;
    for(int i=0; i<NUM_OF_BTNS; i++){
        //all pullups, check for low state
        if(!gpio_get(button_pins[i])){
            pressed |= 1 << i;
        } else {
            pressed &= ~(1<<i);
        }
    }
    return pressed;
}

void buttons_init(uint8_t button_pins[]){
    for(int i=0; i<NUM_OF_BTNS; i++){
        gpio_init(button_pins[i]);
        gpio_set_dir(button_pins[i], false);
        gpio_pull_up(button_pins[i]);
    }
}
