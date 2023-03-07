#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "components/lcd/lcd.h"
#include "components/controls/controls.h"
#include "components/midi_usb/midi_usb.h"
#include "components/types/types.h"
#include "components/pico-mux/pico-mux.h"

//defines
#define CC_SIG_PIN 27

//Global variables
//queque handles
QueueHandle_t logic_queue;
//For buttons connected dicetly to th pico GPIO in order from 0 -> 16
uint8_t btn_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
/*16 bit variable reperesents 16 buttons inputs state, one bit for each
if more buttons is used this needs to be expanded to 32 bit variable or higher*/
uint8_t cc_pins[] = {16, 17, 18, 19};
uint8_t cc_channels[] = {0, 1, 2, 3, 4, 5, 6, 11};
//on mux channels for aux buttons
uint8_t aux_btns_pins[] = {8, 9, 10};

//helpers
//check if given position is in stack
bool in_stack(uint8_t btn_num, BtnStack_t *trgt){
    if(!trgt->lenght) return false;
    for(int i=0; i<trgt->lenght; i++){
        if(btn_num == trgt->stack[i]){
            return true;
            }
        return false;
    }
}

void cpy_btn_stack(BtnStack_t *src, BtnStack_t *trgt){
    memcpy(trgt->stack, src->stack, NUM_OF_BTNS);
    trgt->lenght = src->lenght;
}

void reset_btn_stack(BtnStack_t *btn_stack){
    memset(btn_stack->stack, 0, NUM_OF_BTNS);
    btn_stack->lenght = 0;
}

void check_to_stop(BtnStack_t *current, BtnStack_t *pressed){
    for(int i=0; i<pressed->lenght; i++){
        if(!in_stack(pressed->stack[i], current)){
            midi_stop(pressed->stack[i]-1);
        }
    }
}

void check_to_start(BtnStack_t *current, BtnStack_t *pressed){
    for(int i=0; i<current->lenght; i++){
        if(!in_stack(current->stack[i], pressed)){
            midi_start(current->stack[i]-1);
        }
    }
}

void stop_all(){
    for(int i=0; i<NUM_OF_BTNS; i++){
        midi_stop(btn_pins[i]);
    }
}

void buttons_polling(void *arg){
    BtnStack_t current;
    BtnStack_t pressed;
    reset_btn_stack(&current);
    reset_btn_stack(&pressed);
    
    while(1){
        buttons_pooling(&current);
        if(!current.lenght){
            stop_all(&pressed);
        }
        check_to_stop(&current, &pressed);
        check_to_start(&current, &pressed);
        cpy_btn_stack(&current, &pressed);
        reset_btn_stack(&current);
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

void cc_polling(void *arg){
    CCStack_t cc_stack;
    uint8_t current[NUM_OF_CC];
    uint8_t old[NUM_OF_CC];
    uint8_t val;

    while (1){
        cc_read(&cc_stack, current, old);
        if(cc_stack.length>0){
            for(int i=0; i<cc_stack.length; i++){
                val = current[cc_stack.ids[i]];
                midi_send_cc(cc_stack.ids[i], val);
            }
            cc_stack.length = 0;
        }
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}

void usb_task(void *arg){
    while(1){
        tud_task();
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}

void blink(void *arg){
    uint32_t blink_interval;
    while(1){
        blink_interval = led_interval();
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        vTaskDelay(blink_interval/portTICK_PERIOD_MS);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        vTaskDelay(blink_interval/portTICK_PERIOD_MS);
    }
}

int main() {
    //setup
    stdio_init_all();
    buttons_init(btn_pins);
    mux_init(cc_pins, CC_SIG_PIN, cc_channels, true, true);
    lcd_init();
    midi_init();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    //queue definitions
    logic_queue = xQueueCreate(2, sizeof(BtnStack_t));

    //main tasks
    xTaskCreate(buttons_polling, "Buttons-pooling-task", 1024, NULL, 10, NULL);
    xTaskCreate(cc_polling, "CC-polling-task", 1024, NULL, 9, NULL);
    xTaskCreate(usb_task, "usb-task", 256, NULL, 15, NULL);

    xTaskCreate(blink, "blink", 256, NULL, 5, NULL);

    vTaskStartScheduler();
    while(1){

    }
    return 0;
}