#include "lcd_os.h"

void lcd_os_show_edit(){
    lcd_clear();
    lcd_print("Press a key to");
    lcd_new_line();
    lcd_print("edit a note/cc.");
}

void lcd_os_show_home(){
    lcd_clear();
    lcd_print("PicoMidi");
}

void lcd_os_show_note(uint8_t id, uint8_t note_val){
    lcd_clear();
    lcd_print("key: ");
    lcd_print_int(id);
    lcd_new_line();
    lcd_print("note: ");
    lcd_print_int(note_val);
}