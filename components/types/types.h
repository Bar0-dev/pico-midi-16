#ifndef TYPES_H
#define TYPES_H

#define NUM_OF_BTNS 16
#define NUM_OF_CC 8
#define NUM_OF_AUX_BTNS 3

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
    uint8_t count;
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

#endif