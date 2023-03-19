#include "lcd.h"

static bool edit_mode = false;
cursor_t cursor_pos;

static void lcd_send_raw(char msg_str[]){
    if(i2c_get_write_available(i2c_default)){
        i2c_write_blocking(i2c_default, LCD_CONTROLLER_ADDR, msg_str, CMD_MAX_LENGTH, true);
        i2c_write_blocking(i2c_default, LCD_CONTROLLER_ADDR, "", CMD_MAX_LENGTH, true);
    }
}

void lcd_set_cursor(cursor_t *pos){
    char command[] = "setCursor(00,0)";
    sprintf(command, "setCursor(%u,%u)", pos->col, pos->row);
    lcd_send_raw(command);
}

static void cursor_reset(cursor_t *pos){
    pos->row = 0;
    pos->col = 0;
}

static void cursor_right(cursor_t *pos){
    if(pos->col < LCD_COLS-1){
        pos->col += 1;
    }else if(pos->col >= LCD_COLS-1){
        pos->col = 0;
    }
    lcd_set_cursor(pos);
}

static void cursor_down(cursor_t *pos){
    if(pos->row < LCD_ROWS-1){
        pos->row += 1;
    }else if(pos->col >= LCD_ROWS-1){
        pos->row = 0;
    }
    lcd_set_cursor(pos);
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
    cursor_reset(&cursor_pos);
}

void lcd_home(){
    char command[] = "home()";
    lcd_send_raw(command);
    cursor_reset(&cursor_pos);
}

void lcd_print_notes(uint8_t notes[]){
    char command[] = "print(000 )";
    cursor_t new_line;
    new_line.col=0;
    new_line.row=1;
    for(int i = 0; i<NUM_OF_BTNS; i++){
        sprintf(command, "print(%u )", notes[i]);
        if(i == (uint8_t)(NUM_OF_BTNS/2)){
            lcd_set_cursor(&new_line);
        }
        lcd_send_raw(command);
    }
}

void lcd_cursor(){
    char command[] = "cursor()";
    lcd_send_raw(command);
}

void lcd_no_cursor(){
    char command[] = "noCursor()";
    lcd_send_raw(command);
}

void lcd_blink(){
    char command[] = "blink()";
    lcd_send_raw(command);
}

void lcd_no_blink(){
    char command[] = "noBlink()";
    lcd_send_raw(command);
}

void lcd_update(uint8_t button_id){
    switch (button_id)
    {
    case 0:
        lcd_print("hello");
        break;
    
    case 1:
        if(edit_mode) cursor_down(&cursor_pos);
        break;
    case 2:
        if(edit_mode) cursor_right(&cursor_pos);
        break;
    case 3:
        edit_mode = !edit_mode;
        if(edit_mode){
            lcd_cursor();
        }else{
            lcd_no_cursor();
        }
        break;
    
    default:
        break;
    }
}

void lcd_init(){
    i2c_init(i2c_default, I2C_BAUDRATE);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_I2C_SDA_PIN);
    gpio_pull_up(PICO_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_I2C_SDA_PIN, PICO_I2C_SCL_PIN, GPIO_FUNC_I2C));
    cursor_reset(&cursor_pos);
}