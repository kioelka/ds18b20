#include "init_mcu.h"
#include "indicator.h"
#include "ds18sensor.h"


int main(void)
    {
        Init_MCU();
        SystemClock_Config();
        Init_Indicator();
        Init_DS18b20();
        TIM_Init();

            while (1)
                {
                    
                }
    }