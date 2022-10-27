#ifndef INIT_H
#define INIT_H

#include "indicator.h"
#include "ds18sensor.h"

void SystemClock_Config(void);
void Error_Handler(const char*);
void TIM_Init(void);
void Init_MCU(void);

#endif 