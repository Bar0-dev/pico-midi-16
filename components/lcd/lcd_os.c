#include "lcd_os.h"

lookupNote_t lookup[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

void lcd_os_show_key_edit(){
    lcd_clear();
    lcd_print("Press a key to");
    lcd_new_line();
    lcd_print("edit a note");
}

void lcd_os_show_cc_edit(){
    lcd_clear();
    lcd_print("Rotate a cc");
    lcd_new_line();
    lcd_print("to edit");
}

void lcd_os_show_midi_mode(){
    lcd_clear();
    lcd_print("Midi mode");
}

void lcd_os_show_drum_mode(){
    lcd_clear();
    lcd_print("Drum mode");
}

void lcd_os_show_key_setting(uint8_t id, uint8_t note_val){
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

void lcd_os_show_cc_setting(uint8_t id, uint8_t cc_id){
    lcd_clear();
    lcd_print("knob id:");
    lcd_print_int(id);
    lcd_new_line();
    lcd_print("cc:");
    lcd_print_int(cc_id);
}

void lcd_os_show_note(uint8_t note_val){
    uint8_t octave = (uint8_t)((note_val-12)/12);
    lcd_clear();
    lcd_print(lookup[note_val%12].str);
    lcd_print_int(octave);
}

void lcd_os_show_cc(uint8_t id, uint8_t cc_id, uint8_t cc_val){
    lcd_clear();
    lcd_print("id:");
    lcd_print_int(id);
    lcd_print(" cc:");
    lcd_print_int(cc_id);
    lcd_new_line();
    lcd_print("value:");
    lcd_print_int(cc_val);
}

void lcd_os_show_octave(uint8_t note_val){
    uint8_t octave = (uint8_t)((note_val-12)/12);
    lcd_clear();
    lcd_print("octave:");
    lcd_print_int(octave);
}