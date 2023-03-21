#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "components/controls/controls.h"
#include "components/types/types.h"
#include "components/mux/mux.h"

//defines
#define CC_SIG_PIN 27
#define MENU_BTN_PIN 22

//Global variables
//For buttons connected dicetly to th pico GPIO in order from 0 -> 16
uint8_t btn_pins[] = {12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3};
/*16 bit variable reperesents 16 buttons inputs state, one bit for each
if more buttons is used this needs to be expanded to 32 bit variable or higher*/
uint8_t cc_select_pins[] = {16, 17, 18, 19};
uint8_t cc_mux_channels[] = {0, 1, 2, 3, 4, 5, 6, 11};
//on mux channels for aux buttons
uint8_t aux_btns_mux_channels[] = {8, 9, 10};

//machine state initialization
struct state cur_state = {
    .mode = MIDI_MODE,
    .note_to_edit = {.id = 0, .key_down = 0},
    .notes = {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 74, 75, 76},
    .ccs = {7, 1, 10, 2, 3, 64, 65, 71},
};

void handle_buttons(void *arg){
    btnStack_t btns;
    btns.length=0;

    while(1){
        update_buttons(&btns);
        button_press_action(&btns, &cur_state);
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}

void handle_cc(void *arg){
    ccStack_t cc_stack;
    cc_stack.length = 0;

    while (1){
        cc_update(&cc_stack);
        cc_change_action(&cc_stack, &cur_state);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void handle_aux_buttons(void *arg){
    auxBtnStack_t aux_btns;
    aux_btns.length=0;
    static int i=0;
    while(1){
        aux_btns_update(&aux_btns);
        aux_buttons_press_action(&aux_btns, &cur_state);
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
    aux_btns_init(aux_btns_mux_channels, MENU_BTN_PIN);
    lcd_init();
    midi_init();


    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);


    lcd_os_show_home();

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