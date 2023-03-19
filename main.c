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
#include "components/mux/mux.h"

//defines
#define CC_SIG_PIN 27

//Global variables
//For buttons connected dicetly to th pico GPIO in order from 0 -> 16
uint8_t btn_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
/*16 bit variable reperesents 16 buttons inputs state, one bit for each
if more buttons is used this needs to be expanded to 32 bit variable or higher*/
uint8_t cc_select_pins[] = {16, 17, 18, 19};
uint8_t cc_mux_channels[] = {0, 1, 2, 3, 4, 5, 6, 11};
//on mux channels for aux buttons
uint8_t aux_btns_mux_channels[] = {8, 9, 10};

void handle_buttons(void *arg){
    btnStack_t btns;
    btns.length=0;

    while(1){
        update_buttons(&btns);
        for(int i=0; i<btns.length; i++){
            midi_send_note(btns.stack[i].id, btns.stack[i].key_down);
        }
        btns.length = 0;
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

void handle_cc(void *arg){
    ccStack_t cc_stack;

    while (1){
        cc_update(&cc_stack);
        if(cc_stack.count){
            for(int i=0; i<cc_stack.count; i++){
                midi_send_cc(cc_stack.changed[i].id, cc_stack.changed[i].value);
            }
        }
        cc_stack.count = 0;
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}

void handle_aux_buttons(void *arg){
    auxBtnStack_t aux_btns;
    aux_btns.length=0;
    while(1){
        aux_btns_update(&aux_btns);
        for(int i=0; i<aux_btns.length; i++){
            if(aux_btns.stack[i].key_down){
                switch (aux_btns.stack[i].id)
                {
                case 0:
                    lcd_clear();
                    lcd_print("test1");
                    break;

                case 1:
                    lcd_clear();
                    lcd_print("test2");
                    break;

                case 2:
                    lcd_clear();
                    lcd_print("test3");
                    break;
                
                default:
                    break;
                }
            }
        }
        aux_btns.length = 0;
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void usb_task(void *arg){
    while(1){
        tud_task();
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}


// void blink(void *arg){
//     uint32_t blink_interval;
//     while(1){
//         blink_interval = led_interval();
//         gpio_put(PICO_DEFAULT_LED_PIN, 1);
//         vTaskDelay(blink_interval/portTICK_PERIOD_MS);
//         gpio_put(PICO_DEFAULT_LED_PIN, 0);
//         vTaskDelay(blink_interval/portTICK_PERIOD_MS);
//     }
// }

int main() {
    //setup
    stdio_init_all();
    buttons_init(btn_pins);
    cc_init(cc_mux_channels);
    mux_init(cc_select_pins, CC_SIG_PIN, true, true);
    aux_btns_init(aux_btns_mux_channels);
    lcd_init();
    midi_init();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    //main tasks
    xTaskCreate(handle_buttons, "buttons-pooling-task", 1024, NULL, 10, NULL);
    xTaskCreate(handle_cc, "cc-polling-task", 1024, NULL, 9, NULL);
    xTaskCreate(handle_aux_buttons, "aux-buttons-pooling-task", 1024, NULL, 8, NULL);
    xTaskCreate(usb_task, "usb-task", 256, NULL, 15, NULL);

    vTaskStartScheduler();
    while(1){
    }
    return 0;
}