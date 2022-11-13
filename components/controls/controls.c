#include "controls.h"

static uint8_t pins[NUM_OF_BTNS];

void buttons_pooling(BtnStack_t *btn_stack){
    for(int i=0, j=0; i<NUM_OF_BTNS; i++){
        if(!gpio_get(pins[i])){
            btn_stack->stack[j] = i+1;
            btn_stack->lenght = j+1;
            j++;
        }
    }
}

void buttons_init(uint8_t button_pins[]){
    memcpy(pins, button_pins, NUM_OF_BTNS);
    for(int i=0; i<NUM_OF_BTNS; i++){
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], false);
        gpio_pull_up(pins[i]);
    }
}