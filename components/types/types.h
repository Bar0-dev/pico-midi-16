#ifndef TYPES_H
#define TYPES_H

#define NUM_OF_BTNS 16
#define NUM_OF_CC 8
#define NUM_OF_AUX_BTNS 3

#define MIDI_MODE 0
#define EDIT_MODE 1
#define DRUM_MACHINE_MODE 2

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

typedef struct ccread
{
    uint8_t id;
    uint8_t value;
} ccread_t;

typedef struct ccStack
{
    ccread_t changed[NUM_OF_CC];
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
    btnRead_t note_to_edit;
    uint8_t notes[NUM_OF_BTNS];
    uint8_t ccs[NUM_OF_CC];
} state_t;

#endif