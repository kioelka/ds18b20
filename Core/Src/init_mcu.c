#include "init_mcu.h"

void SystemClock_Config(void)
    {
        /*
        //HSE
        RCC->CR |= RCC_CR_HSEON;//HSE turn on
        unsigned counter = 0;
        while (!RCC->CR & RCC_CR_HSERDY)
        {
        if (counter++ >= 1600000)
        Error_Handler("Failed to Start HSE");
    }
        
        //PLL
        RCC->CFGR = RCC_CFGR_PLLMULL2 | RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE;// HSE / 2 -> PLL * 2
        
        
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        while (!FLASH->ACR & FLASH_ACR_PRFTBS)
        {
        if (counter++ >= 1600000)
        Error_Handler("Failed to setup Flash");
    }
        
        RCC->CFGR |= RCC_CFGR_HPRE_DIV4;// sysclk / 4 = 2MHz
        RCC->CFGR &= (~RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk);//prescalers APB1 APB2 off
        
        RCC->CR |= RCC_CR_PLLON;//turn on PLL
        counter = 0;
        while (!RCC->CR & RCC_CR_PLLRDY)
        {
        if (counter++ >= 1600000)
        Error_Handler("Failed to Start PLL");
    }
        
        //Set Clock from PLL
        RCC->CFGR |= RCC_CFGR_SW_PLL;
        counter = 0;
        while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (RCC_CFGR_SWS_PLL))
        {
        if (counter++ >= 1600000)
        Error_Handler("Failed to  Switch to PLL");
    }
        //RCC->CR &= ~RCC_CR_HSION;//HSI Off
        SystemCoreClock = 2000000;
        printf("Clock = %d",SystemCoreClock);
        
        // doesnt start w\o debug
        */
        
        
        
        RCC_OscInitTypeDef RCC_OscInitStruct = {0};
        RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
        
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
        RCC_OscInitStruct.HSIState = RCC_HSI_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
            {
                Error_Handler("");
            }
        
        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
        
        if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
            {
                Error_Handler("");
            }
        
    }

void TIM_Init()
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM3EN; //TIM4 clock TIM3 clock
        
        NVIC_SetPriority(TIM4_IRQn,0);
        NVIC_EnableIRQ(TIM4_IRQn);
        TIM4->DIER |= TIM_DIER_UIE;//interrupt update enable
        TIM4->PSC = 2000 - 1;
        TIM4->ARR = 1; // 1000Hz
        TIM4->CR1 |= TIM_CR1_CEN;
        
        NVIC_SetPriority(TIM3_IRQn,1);
        NVIC_EnableIRQ(TIM3_IRQn);
        TIM3->DIER |= TIM_DIER_UIE;
        TIM3->PSC = 2000 - 1;
        TIM3->ARR = 1000 - 1; // 1Hz
        TIM3->CR1 |= TIM_CR1_CEN;
        
    }

void Init_MCU(void)
    {
        RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
        AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
        NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        HAL_InitTick(TICK_INT_PRIORITY);//Init Systick
    }

void TIM3_IRQHandler()
    {
        
        ds_IRQHandler();
        TIM3->SR = 0;
    }

void TIM4_IRQHandler()
    {
        
        Indicator_IRQHandler();
        TIM4->SR = 0;
    }


void Error_Handler(const char *msg)
    {
        printf(msg);
        __disable_irq();
        while (1)
            {
            }
    }

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
    {
        
    }
#endif