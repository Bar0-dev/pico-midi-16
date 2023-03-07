#include "controls.h"

static uint8_t btn_pins[NUM_OF_BTNS];
static uint8_t cc_pins[NUM_OF_CC];
static uint8_t old_cc[NUM_OF_CC];
static uint8_t current_cc[NUM_OF_CC];

void buttons_init(uint8_t button_pins[]){
    memcpy(btn_pins, button_pins, sizeof(uint8_t)*NUM_OF_BTNS);
    for(int i=0; i<NUM_OF_BTNS; i++){
        gpio_init(btn_pins[i]);
        gpio_set_dir(btn_pins[i], false);
        gpio_pull_up(btn_pins[i]);
    }
}

void cc_init(uint8_t mux_channels[]){
    memcpy(cc_pins, mux_channels, sizeof(uint8_t)*NUM_OF_CC);
}

void buttons_pooling(BtnStack_t *btn_stack){
    for(int i=0, j=0; i<NUM_OF_BTNS; i++){
        if(!gpio_get(btn_pins[i])){
            btn_stack->stack[j] = btn_pins[i]+1;
            btn_stack->lenght = j+1;
            j++;
        }
    }
}

static void cc_read(uint8_t values[]){
    uint16_t readings[NUM_OF_CC];
    mux_read(readings, NUM_OF_CC, cc_pins);
    for(int i=0; i<NUM_OF_CC; i++){
        values[i] = readings[i]>>5; //convert uint16_t into int8_t
    }
}

void cc_update(ccStack_t *cc_stack){
    cc_read(current_cc);
    for(int i=0; i<NUM_OF_CC; i++){
        if(abs(current_cc[i]-old_cc[i])>CC_POT_THRESHOLD){
            cc_stack->changed[i].id = i;
            cc_stack->changed[i].value = current_cc[i];
            cc_stack->count++;
        }
    }
    memcpy(old_cc, current_cc, sizeof(uint8_t)*NUM_OF_CC);
}

// void aux_btns_read(uint8_t btn_channels[], uint16_t state[]){
//     uint16_t btn_read;
//     for(int i=0; i<NUM_OF_AUX_BTNS; i++){
//         mux_select_channel(btn_channels[i]);
//         btn_read = adc_read();
//         state[i] = btn_read;
//     }
// }