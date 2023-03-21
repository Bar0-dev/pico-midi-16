#include "lcd_os.h"

lookupNote_t lookup[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

void lcd_os_show_edit(){
    lcd_clear();
    lcd_print("Press a key to");
    lcd_new_line();
    lcd_print("edit a note");
}

void lcd_os_show_home(){
    lcd_clear();
    lcd_print("PicoMidi");
}

void lcd_os_show_setting(uint8_t id, uint8_t note_val){
    uint8_t octave = (uint8_t)((note_val-12)/12);
    lcd_clear();
    lcd_print("key:");
    lcd_print_int(id);
    lcd_print(" midi:");
    lcd_print_int(note_val);
    lcd_new_line();
    lcd_print("note:");
    lcd_print(lookup[note_val%12].str);
    lcd_print_int(octave);
}

void lcd_os_show_note(uint8_t note_val){
    uint8_t octave = (uint8_t)((note_val-12)/12);
    lcd_clear();
    lcd_print(lookup[note_val%12].str);
    lcd_print_int(octave);
}

void lcd_os_show_octave(uint8_t note_val){
    uint8_t octave = (uint8_t)((note_val-12)/12);
    lcd_clear();
    lcd_print("octave:");
    lcd_print_int(octave);
}