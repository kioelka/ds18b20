#ifndef INDICATOR_H
#define INDICATOR_H

/**
Indicator pinned P0-P7 segments and P8-P10 digits
*/

#include "config.h"

typedef enum
    {
        DIGIT1 = 1,
        DIGIT2 = 2,
        DIGIT3 = 4
    }
digitState;

typedef struct
    {
        char value[3];
        volatile short* gpio_addr;
        digitState display_state : 3;
        
    }
indicatorTypedef;


void Init_Indicator(void);
void Draw_Digit(void);
void Indicator_IRQHandler(void);

#endif