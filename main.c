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

void controlls_polling(void *arg){
    uint16_t btns_pressed = 0;
    uint16_t btns_pressed_old = 0xFFFFFFFF;
    while(1){
        btns_pressed = buttons_pooling(arg);
        if(btns_pressed != btns_pressed_old){
            xQueueSend(logic_queue, (void *) &btns_pressed, (TickType_t) 0);
            btns_pressed_old = btns_pressed;
        }
    }
}

void logic_controller(void *arg){
    uint16_t pressed_buff;
    while(1){
        if(xQueueReceive(logic_queue, (void *) &pressed_buff, (TickType_t) 0) == pdTRUE){
            update_lcd(pressed_buff);
        }
    }
    
}



int main() {
    //variables
    //For buttons connected dicetly to th pico GPIO in order from 0 -> 16
    uint8_t btn_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    /*16 bit variable reperesents 16 buttons inputs state, one bit for each
    if more buttons is used this needs to be expanded to 32 bit variable or higher*/
    
    //setup
    stdio_init_all();
    buttons_init(btn_pins);
    lcd_init();
    midi_init();

    //queue definitions
    logic_queue = xQueueCreate(10, sizeof(uint16_t));

    //main tasks
    xTaskCreate(controlls_polling, "Controls-pooling-task", 256, (void *)btn_pins, 10, NULL);
    xTaskCreate(logic_controller, "Logic-controller-task", 1024, NULL, 11, NULL);
    while(1){
        // btn_pressed_old = update_lcd(btn_pressed, btn_pressed_old);
        tud_task(); // tinyusb device task
        led_blinking_task();
        midi_task();
    } 
    return 0;
}