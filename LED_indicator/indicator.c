#include "indicator.h"

indicatorTypedef indicator;

const char buf[] =
    {
        0x04,
        0x5E,
        0x09,
        0x0A,
        0x52,
        0x22,
        0x20,
        0x4E,
        0x00,
        0x02,
        0xFF
    };

void Init_Indicator(void)
    {
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;//GPIOB clock
        
        INDICATOR_GPIO->CRL = GPIO_CRL_MODE0_Msk | GPIO_CRL_MODE1_Msk | GPIO_CRL_MODE2_Msk |
            GPIO_CRL_MODE3_Msk | GPIO_CRL_MODE4_Msk | GPIO_CRL_MODE5_Msk |
                GPIO_CRL_MODE6_Msk | GPIO_CRL_MODE7_Msk; //PB0-PB7 Output mode 50 MHZ  
        
        INDICATOR_GPIO->CRH |= GPIO_CRH_MODE8_Msk | GPIO_CRH_MODE9_Msk | GPIO_CRH_MODE10_Msk;//PB8-PB10 Output Mode 50 MHz
        
        INDICATOR_GPIO->BSRR = (0x3FF<<16); //P0-P10 low
                        
        indicator.value[0] = 1; indicator.value[1] = 2; indicator.value[2] = 3;
        indicator.gpio_addr = (volatile short*)&(INDICATOR_GPIO->ODR);
        indicator.display_state = DIGIT1;
    }

inline void Shift_Digit(void)
    {
        indicator.display_state <<= 1;
        if (indicator.display_state == 0)
            indicator.display_state = DIGIT1;
        *(indicator.gpio_addr) = (*(indicator.gpio_addr) & ( 0<<8 | 0<<9 | 0<<10 | 0xFF)) | ((~indicator.display_state & 7) << 8); //need to save high pins states before change
    }

void Draw_Digit(void)
    {
        char val = indicator.value[indicator.display_state >> 1];
        if (val >= '0' && val <= '9')
            val -= '0';
        else if (val > 10)
            val = 10;
        *(indicator.gpio_addr) = (*(indicator.gpio_addr) & ( 0<<1 | 0<<2 | 0<<3 | 0<<4 | 0<<5 | 0<<6 | 0<<7 | 0xFF00)) | (buf[val]); //need to save low pins states before change
    }

void Indicator_IRQHandler(void)
    {
        Shift_Digit();
        Draw_Digit();
    }


