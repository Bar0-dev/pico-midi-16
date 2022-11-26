#include "pico-mux.h"

static uint8_t select_pins[CHANNELS];
static uint8_t sig_pin;

static void select_channel(uint8_t channel){
    for(int i=0; i<SELECT_PINS; i++){
        gpio_put(select_pins[i], (1<<i) & channel);
    }
}

void mux_read(uint16_t reads[], uint8_t length){
    for(int i=0; i<length; i++){
        select_channel(i);
        reads[i] = adc_read();
    }
}

void mux_init(uint8_t select[], uint8_t sig, bool out, bool analog){
    memcpy(select_pins, select, CHANNELS);
    sig_pin = sig;
    for(int i=0; i<CHANNELS; i++){
        gpio_init(select_pins[i]);
        gpio_set_dir(select_pins[i], true);
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