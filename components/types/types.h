#ifndef TYPES_H
#define TYPES_H

#define NUM_OF_BTNS 16
#define MSG_MAXSIZE 32
#define NUM_OF_CC 8
#define NUM_OF_AUX_BTNS 3

typedef struct BtnStack
{
    uint8_t stack[NUM_OF_BTNS];
    uint8_t lenght;
} BtnStack_t;

typedef struct CCStack
{
    uint8_t ids[NUM_OF_CC];
    uint8_t length;
} CCStack_t;

#endif