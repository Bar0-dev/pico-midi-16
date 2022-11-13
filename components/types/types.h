#ifndef TYPES_H
#define TYPES_H

#define NUM_OF_BTNS 16
#define MSG_MAXSIZE 32

typedef struct BtnStack
{
    uint8_t stack[NUM_OF_BTNS];
    uint8_t lenght;
} BtnStack_t;

#endif