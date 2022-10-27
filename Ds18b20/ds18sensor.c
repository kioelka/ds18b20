#include "ds18sensor.h"

#define SKIP_ROM_ADR 0xCC // пропуск адресации
#define CONVERT_TEMP 0x44 // запускает преобразование температуры
#define READ_DATA_COMAND 0xBE //команда чтения содержимого памяти

extern indicatorTypedef indicator;

static void delay_us(unsigned T)
    {
	asm(
            "cmp r0, #0\n"
		"beq end\n"	
                    
                    "loop:\n"
                        "subs r0, r0, #1\n"
                            "bne loop\n"
                                
                                "end:\n"
                                    );
        
    }


uint8_t ds_reset_pulse()
{
   short result;   
 
   if((SENSOR_GPIO->IDR & SENSOR_PIN) == 0)  return 2;         //проверить линию на отсутствие замыкания
   SENSOR_GPIO->ODR &= ~SENSOR_PIN;                          //потянуть шину к земле
   delay_us(240);                                     //ждать 480 микросекунд
   SENSOR_GPIO->ODR |=  SENSOR_PIN;                          //отпустить шину
   delay_us(35);                                   //ждать 70 микросекунд
   result =  SENSOR_GPIO->IDR & SENSOR_PIN;                 //прочитать шину 
   delay_us(205);                                   //дождаться окончания инициализации
   if(result) return 1;                            //датчик не обнаружен
   return 0;                                       //датчик обнаружен      
}

void ds_write_bit(uint8_t bit)
{
   SENSOR_GPIO->ODR &= ~SENSOR_PIN;                          //потянуть шину к земле
   delay_us(1);                                            //ждать 2 микросекунду
   if(bit) 
       SENSOR_GPIO->ODR |=  SENSOR_PIN;                  //если передаем 1, то отпускаем шину
   delay_us(30);                         //задержка 60 микросекунд 
   SENSOR_GPIO->ODR |=  SENSOR_PIN;                          //отпускаем шину 
}

uint8_t ds_read_bit()
{
   uint16_t result;
   SENSOR_GPIO->ODR &= ~SENSOR_PIN;                          //потянуть шину к земле
   delay_us(1);
   SENSOR_GPIO->ODR |=  SENSOR_PIN;                          //отпускаем шину  
   delay_us(7);                                 //задержка 15 микросекунд
   result =  SENSOR_GPIO->IDR & SENSOR_PIN;              //прочитать шину
   delay_us(30);                                   //оставшееся время 
   return result;                                  //возвратить результат
}

void ds_write_byte(uint8_t byte)
{
   for(uint8_t i=0;i<8;i++)
       ds_write_bit(byte & (1 << i) );
}

uint8_t ds_read_byte()
{
   uint8_t i,result = 0;
   for(i=0;i<8;i++) 
   if(ds_read_bit()) result |= 1<<i; 
   return result;
}


void ds_IRQHandler(void)
    {
        
        DS_ON_PWR;//Включить питание
        
        ds_reset_pulse();          //послать импульс сброса                                       
        ds_write_byte(SKIP_ROM_ADR);//разрешить доступ к датчику не используя адрес
        ds_write_byte(CONVERT_TEMP);      //запустить преобразование
        
        delay_us(375);               //дать время для измерения
        
        ds_reset_pulse();          //послать импульс сброса              
        ds_write_byte(SKIP_ROM_ADR);//разрешить доступ к датчику не используя адрес
        ds_write_byte(READ_DATA_COMAND);      //команда, заставляющая датчик выдать 9 байт своей памяти
        
        uint8_t ds_buff[9];        
        for(int i=0; i<9; i++ )           //прочитать 9 байт в массив
            ds_buff[i] = ds_read_byte();
        
        
        short *dt = (short*)ds_buff;
        short temp = 0;
        
        temp += (( *dt & 0x07FF ) >> 4) * 10;
            
            //Прибавим дробную часть

        temp += ( *dt & 0x000F ) / 1.6f;
        
        sprintf(indicator.value,"%3d",temp);//передать в индикатор
        
        
        DS_OFF_PWR;//выключить питание
    }

void Init_DS18b20()
    {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//GPIOA clock
        SENSOR_GPIO->CRL |= GPIO_CRL_MODE0_Msk | GPIO_CRL_CNF0_0 | //GPIO PA0 50 Mhz OD
            GPIO_CRL_MODE1_Msk; // GPIO PA1 50MHz PP 
        SENSOR_GPIO->BSRR = SENSOR_PIN | (SENSOR_PWR_PIN << 16);
    }
