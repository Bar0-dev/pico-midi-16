#ifndef TYPES_H
#define TYPES_H

#define NUM_OF_BTNS 16
#define NUM_OF_CC 8
#define NUM_OF_AUX_BTNS 3

#define MIDI_MODE 0
#define EDIT_KEY_MODE 1
#define EDIT_CC_MODE 2
#define DRUM_MACHINE_MODE 3

typedef struct btnRead
{
    uint8_t id;
    uint8_t key_down;
} btnRead_t;

typedef struct btnStack
{
    btnRead_t stack[NUM_OF_BTNS];
    uint8_t length;
} btnStack_t;

typedef struct ccRead
{
    uint8_t id;
    uint8_t value;
} ccRead_t;

typedef struct ccStack
{
    ccRead_t changed[NUM_OF_CC];
    uint8_t length;
} ccStack_t;

typedef struct auxBtnStack
{
    btnRead_t stack[NUM_OF_AUX_BTNS];
    uint8_t length;
} auxBtnStack_t;

typedef struct cursor
{
    uint8_t row;
    uint8_t col;
} cursor_t;

typedef struct lookup_note
{
    char str[4];
} lookupNote_t;

typedef struct state
{
    uint8_t mode;
    uint8_t notes[NUM_OF_BTNS];
    uint8_t ccs[NUM_OF_CC];
    btnRead_t note_to_edit;
    ccRead_t cc_to_edit;
} state_t;

#endif