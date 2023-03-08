#include "mux.h"

static uint8_t s_pins[NUM_OF_SEL_PINS];
static uint8_t sig_pin;

static void mux_select_channel(uint8_t channel){
    for(int i=0; i<NUM_OF_SEL_PINS; i++){
        gpio_put(s_pins[i], (bool)(channel & (1<<i)));
    }
}

void mux_read(uint16_t readings[], uint8_t length, uint8_t channels[]){
    for(int i=0; i<length; i++){
        mux_select_channel(channels[i]);
        readings[i] = adc_read();
    }
}

void mux_init(uint8_t select_pins[], uint8_t signal_pin, bool direction_is_out, bool read_is_analog){
    //Copy settings to local statics
    memcpy(s_pins, select_pins, sizeof(uint8_t)*NUM_OF_SEL_PINS);
    sig_pin = signal_pin;
    
    for(int i=0; i<NUM_OF_SEL_PINS; i++){
        gpio_init(select_pins[i]);
        gpio_set_dir(select_pins[i], direction_is_out);
    }
    if(read_is_analog){
        adc_init();
        adc_gpio_init(sig_pin);
        adc_select_input(ADC_CHANNEL);
    } else {
        gpio_init(sig_pin);
        gpio_set_dir(sig_pin, direction_is_out);
    }
}