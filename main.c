#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stdio.h>
#include <stdlib.h>
#include "components/lcd/lcd.h"
#include "components/controls/controls.h"
#include "components/midi_usb/midi_usb.h"

//Global variables
//queque handles
QueueHandle_t logic_queue;
//For buttons connected dicetly to th pico GPIO in order from 0 -> 16
uint8_t btn_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
/*16 bit variable reperesents 16 buttons inputs state, one bit for each
if more buttons is used this needs to be expanded to 32 bit variable or higher*/

void controlls_polling(void *arg){
    uint16_t btns_pressed = 0;
    uint16_t btns_pressed_old = 0xFF;
    while(1){
        btns_pressed = buttons_pooling(btn_pins);
        if(btns_pressed != btns_pressed_old){
            xQueueSend(logic_queue, (void *)&btns_pressed, (TickType_t) 0);
            btns_pressed_old=btns_pressed;
        } else {
            vTaskDelay(portTICK_PERIOD_MS);
        }
    }
}

void logic_controller(void *arg){
    uint16_t pressed_buff;
    while(1){
        if(xQueueReceive(logic_queue, &(pressed_buff), (TickType_t) 10) == pdTRUE){
            midi_send(pressed_buff);
        } else {
            vTaskDelay(portTICK_PERIOD_MS);
        }
    }
    
}

void usb_task(void *arg){
    while(1){
        tud_task();
        vTaskDelay(portTICK_PERIOD_MS);
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
    lcd_init();
    midi_init();

    //TESTING!!!
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    //TESTING!!!

    //queue definitions
    logic_queue = xQueueCreate(2, sizeof(uint16_t));

    //main tasks
    xTaskCreate(controlls_polling, "Controls-pooling-task", 1024, NULL, 10, NULL);
    xTaskCreate(logic_controller, "Logic-controller-task", 1024, NULL, 11, NULL);
    xTaskCreate(usb_task, "usb-task", 256, NULL,15,NULL);

    //TESTING
    xTaskCreate(blink, "blink", 256, NULL, 12, NULL);
    //TESTING

    vTaskStartScheduler();
    while(1){

    }
    return 0;
}