#include "controls.h"

static uint8_t btn_pins[NUM_OF_BTNS];
static uint16_t btn_mask_old;

static uint8_t cc_pins[NUM_OF_CC];
static uint8_t old_cc[NUM_OF_CC];
static uint8_t current_cc[NUM_OF_CC];

static uint16_t buttons_get_mask(){
    uint16_t mask;
    for(int i=0; i<NUM_OF_BTNS; i++){
        if(gpio_get(btn_pins[i])){
            mask |= 1 << i;
        } else {
            mask &= ~(1 << i);
        }
    }
    return mask;
}

void update_buttons(btnStack_t *btns){
    uint16_t current_mask = buttons_get_mask();
    for(int i=0, j=0; i<NUM_OF_BTNS; i++){
        if((current_mask ^ btn_mask_old) & (1 << i)){
            btns->stack[j].id = i;
            btns->stack[j].key_down = (current_mask & (1 << i));
            j++;
            btns->lenght = j;
        }
    }
    btn_mask_old = current_mask;
}

void buttons_init(uint8_t button_pins[]){
    memcpy(btn_pins, button_pins, sizeof(uint8_t)*NUM_OF_BTNS);
    btn_mask_old = buttons_get_mask();
    for(int i=0; i<NUM_OF_BTNS; i++){
        gpio_init(btn_pins[i]);
        gpio_set_dir(btn_pins[i], false);
        gpio_pull_up(btn_pins[i]);
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
    for(int i=0, j=0; i<NUM_OF_CC; i++){
        if(abs(current_cc[i]-old_cc[i])>CC_POT_THRESHOLD){
            cc_stack->changed[j].id = i;
            cc_stack->changed[j].value = current_cc[i];
            j++;
            cc_stack->count = j;
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

void cc_init(uint8_t mux_channels[]){
    memcpy(cc_pins, mux_channels, sizeof(uint8_t)*NUM_OF_CC);
}