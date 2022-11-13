#include "controls.h"

void buttons_pooling(uint8_t button_pins[], bool pressed[], bool *changed){
    uint8_t checksum_cur = 0;
    uint8_t checksum_old = 0;
    for(int i=0; i<NUM_OF_BTNS; i++){
        bool state = !gpio_get(button_pins[i]);
        checksum_cur += (uint8_t)state;
        checksum_old += (uint8_t)pressed[i];
        pressed[i] = state;
    }
    if(checksum_cur != checksum_old) *changed = true;
}

void buttons_init(uint8_t button_pins[]){
    for(int i=0; i<NUM_OF_BTNS; i++){
        gpio_init(button_pins[i]);
        gpio_set_dir(button_pins[i], false);
        gpio_pull_up(button_pins[i]);
    }
}
