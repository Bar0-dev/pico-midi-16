#include "lcd.h"

static void lcd_send_raw(char msg_str[]){
    if(i2c_get_write_available(i2c_default)){
        i2c_write_blocking(i2c_default, LCD_CONTROLLER_ADDR, msg_str, CMD_MAX_LENGTH, true);
    }
}

void lcd_print(char msg[]){
    char command[CMD_MAX_LENGTH];
    sprintf(command, "print(%s)", msg);
    lcd_send_raw(command);
}

void lcd_print_int(uint32_t num){
    char command[CMD_MAX_LENGTH];
    sprintf(command, "print(%u)", num);
    lcd_send_raw(command);
}


void lcd_clear(){
    char command[] = "clear()";
    lcd_send_raw(command);
}

void lcd_home(){
    char command[] = "home()";
    lcd_send_raw(command);
}

void lcd_debug_ccstack(ccStack_t *ccstack){
    lcd_home();
    lcd_print_int(ccstack->changed[0].value);
}

// void lcd_debug_btn_stack(BtnStack_t *btn_stack){
//     uint8_t num = btn_stack->stack[0];
//     char msg[32];
//     lcd_clear();
//     sprintf(msg, "%d", num);
//     lcd_print(msg);
// }

void lcd_init(){
    i2c_init(i2c_default, I2C_BAUDRATE);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_I2C_SDA_PIN);
    gpio_pull_up(PICO_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_I2C_SDA_PIN, PICO_I2C_SCL_PIN, GPIO_FUNC_I2C));
    lcd_clear();
}