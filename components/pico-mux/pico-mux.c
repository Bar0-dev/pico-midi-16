#include "pico-mux.h"

static uint8_t select_pins[SELECT_PINS];
static uint8_t cc_channels[NUM_OF_CC];
static uint8_t sig_pin;

static void mux_select_channel(uint8_t channel){
    for(int i=0; i<SELECT_PINS; i++){
        gpio_put(select_pins[i], (bool)(channel & (1<<i)));
    }
}

void cc_read(CCStack_t *cc_stack, uint8_t current[], uint8_t old[]){
    for(int i=0, j=0; i<NUM_OF_CC; i++){
        mux_select_channel(cc_channels[i]);
        current[i] = adc_read()>>5;
        if(i<8 && abs(current[i] - old[i])>POT_THRESHOLD){
            cc_stack->ids[j]=i;
            j++;
            cc_stack->length = j;
        }
        old[i] = current[i];
    }
}

void aux_btns_read(uint8_t btn_channels[], uint16_t state[]){
    uint16_t btn_read;
    for(int i=0; i<NUM_OF_AUX_BTNS; i++){
        mux_select_channel(btn_channels[i]);
        btn_read = adc_read();
        state[i] = btn_read;
        // if(btn_read>BTN_THRESHOLD){
        //     state[i] = true;
        // } else {
        //     state[i] = false;
        // }
    }
}

void mux_init(uint8_t select[], uint8_t sig, uint8_t channels[], bool out, bool analog){
    memcpy(select_pins, select, sizeof(uint8_t)*SELECT_PINS);
    memcpy(cc_channels, channels, sizeof(uint8_t)*NUM_OF_AUX_BTNS);
    sig_pin = sig;
    for(int i=0; i<SELECT_PINS; i++){
        gpio_init(select_pins[i]);
        gpio_set_dir(select_pins[i], out);
    }
    if(analog){
        adc_init();
        adc_gpio_init(sig_pin);
        adc_select_input(ADC_CHANNEL); //
    } else {
        gpio_init(sig_pin);
        gpio_set_dir(sig_pin, out);
    }
};