#include "controls.h"

static uint8_t btn_pins[NUM_OF_BTNS];
static uint16_t btn_mask_old;

static uint8_t cc_pins[NUM_OF_CC];
static uint8_t old_cc[NUM_OF_CC];
static uint8_t current_cc[NUM_OF_CC];

static uint8_t aux_btn_pins[NUM_OF_AUX_BTNS];
static uint8_t aux_btn_mask_old;
static uint8_t menu_pin;

static uint16_t buttons_get_mask(){
    uint16_t mask;
    for(int i=0; i<NUM_OF_BTNS; i++){
        if(!gpio_get(btn_pins[i])){
            mask |= 1 << i;
        } else {
            mask &= ~(1 << i);
        }
    }
    return mask;
}

void update_buttons(btnStack_t *btns){
    uint16_t current_mask = buttons_get_mask();
    for(int i=0, j=0; i<NUM_OF_BTNS; i++){
        if((current_mask ^ btn_mask_old) & (1 << i)){
            btns->stack[j].id = i;
            btns->stack[j].key_down = (bool)(current_mask & (1 << i));
            j++;
            btns->length = j;
        }
    }
    btn_mask_old = current_mask;
}

void button_press_action(btnStack_t *btns, state_t *state_now){
    for(int i=0; i<btns->length; i++){
            if(state_now->mode == MIDI_MODE){
                midi_send_note(btns->stack[i].key_down, state_now->notes[btns->stack[i].id]);
                lcd_os_show_note(state_now->notes[btns->stack[i].id]);
            }
            if(state_now->mode == EDIT_MODE) {
                if(btns->stack[i].key_down){
                    lcd_os_show_setting(btns->stack[i].id, state_now->notes[btns->stack[i].id]);
                    state_now->note_to_edit = btns->stack[i];
                }
            }
        }
        btns->length = 0;
}

void buttons_init(uint8_t button_pins[]){
    memcpy(btn_pins, button_pins, sizeof(uint8_t)*NUM_OF_BTNS);
    btn_mask_old = buttons_get_mask();
    for(int i=0; i<NUM_OF_BTNS; i++){
        gpio_init(btn_pins[i]);
        gpio_set_dir(btn_pins[i], false);
        gpio_pull_up(btn_pins[i]);
    }
}

static void cc_read(uint8_t values[]){
    uint16_t readings[NUM_OF_CC];
    mux_read(readings, NUM_OF_CC, cc_pins);
    for(int i=0; i<NUM_OF_CC; i++){
        values[i] = readings[i]>>5; //convert uint16_t into int8_t
    }
}

void cc_update(ccStack_t *cc_stack){
    cc_read(current_cc);
    for(int i=0, j=0; i<NUM_OF_CC; i++){
        if(abs(current_cc[i]-old_cc[i])>CC_POT_THRESHOLD){
            cc_stack->changed[j].id = i;
            cc_stack->changed[j].value = current_cc[i];
            j++;
            cc_stack->length = j;
        }
    }
    memcpy(old_cc, current_cc, sizeof(uint8_t)*NUM_OF_CC);
}

void cc_change_action(ccStack_t *cc_stack, state_t *state_now){
    if(cc_stack->length){
            for(int i=0; i<cc_stack->length; i++){
                midi_send_cc(cc_stack->changed[i].value, state_now->ccs[cc_stack->changed[i].id]);
            }
        }
        cc_stack->length = 0;
}

// void aux_btns_read(uint8_t btn_channels[], uint16_t state[]){
//     uint16_t btn_read;
//     for(int i=0; i<NUM_OF_AUX_BTNS; i++){
//         mux_select_channel(btn_channels[i]);
//         btn_read = adc_read();
//         state[i] = btn_read;
//     }
// }

void cc_init(uint8_t mux_channels[]){
    memcpy(cc_pins, mux_channels, sizeof(uint8_t)*NUM_OF_CC);
}

static uint8_t aux_btns_get_mask(){
    uint8_t btns_mask=0;
    uint16_t readings[NUM_OF_AUX_BTNS];
    mux_read(readings, NUM_OF_AUX_BTNS, aux_btn_pins);
    for(int i=0; i<NUM_OF_AUX_BTNS; i++){
        if(abs(readings[i])>AUX_BTN_THRESHOLD){
            btns_mask |= 1<<i;
        }
    }
    btns_mask |= !gpio_get(menu_pin)<<(NUM_OF_AUX_BTNS);
    return btns_mask;
}

void aux_btns_update(auxBtnStack_t *btns){
    uint8_t current_mask = aux_btns_get_mask();
    for(int i=0, j=0; i<(NUM_OF_AUX_BTNS+1); i++){
        if((current_mask ^ aux_btn_mask_old) & (1 << i)){
            btns->stack[j].id = i;
            btns->stack[j].key_down = (bool)(current_mask & (1 << i));
            j++;
            btns->length = j;
        }
    }
    aux_btn_mask_old = current_mask;
}

static void toggle_mode(state_t *now){
    switch (now->mode)
    {
    case MIDI_MODE:
        now->mode = EDIT_MODE;
        lcd_os_show_edit();
        break;
    
    case EDIT_MODE:
        now->mode = MIDI_MODE;
        lcd_os_show_home();
        break;
    
    default:
        break;
    }
}

void aux_buttons_press_action(auxBtnStack_t *btns, state_t *state_now){
    for(int i=0; i<btns->length; i++){
        if(btns->stack[i].key_down){
            switch (btns->stack[i].id){
            case MODE_BTN:
                toggle_mode(state_now);
                break;

            case NOTE_UP_BTN:
                if(state_now->mode == EDIT_MODE){
                    state_now->notes[state_now->note_to_edit.id] += 1;
                    lcd_os_show_setting(state_now->note_to_edit.id, state_now->notes[state_now->note_to_edit.id]);
                }
                break;

            case NOTE_DOWN_BTN:
                if(state_now->mode == EDIT_MODE){
                    state_now->notes[state_now->note_to_edit.id] -= 1;
                    lcd_os_show_setting(state_now->note_to_edit.id, state_now->notes[state_now->note_to_edit.id]);
                }
                break;

            case CHANGE_OCTAVE_BTN:
                if(state_now->notes[NUM_OF_BTNS-1]+12<=127){
                    for(int i=0; i<NUM_OF_BTNS; i++){
                        state_now->notes[i] += 12;
                    }
                } else {
                    for(int i=0; i<NUM_OF_BTNS; i++){
                        state_now->notes[i] = 24+i;
                    }
                }
                lcd_os_show_octave(state_now->notes[0]);
                break;

            default:
                break;
            }
        }
    }
    btns->length = 0;
}

void aux_btns_init(uint8_t mux_channels[], uint8_t menu_btn_pin){
    memcpy(aux_btn_pins, mux_channels, sizeof(uint8_t)*NUM_OF_AUX_BTNS);
    aux_btn_mask_old = aux_btns_get_mask();
    menu_pin = menu_btn_pin;
    gpio_init(menu_pin);
    gpio_set_dir(menu_pin, false);
    gpio_pull_up(menu_pin);
}