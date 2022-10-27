#include "config.h"
#include "indicator.h"

#define DS_OFF_PWR      SENSOR_PWR_GPIO->BSRR = SENSOR_PWR_PIN << 16
#define DS_ON_PWR       SENSOR_PWR_GPIO->BSRR = SENSOR_PWR_PIN

uint8_t ds_read_byte(void);
void ds_write_byte(uint8_t);
uint8_t ds_read_bit(void);
void ds_write_bit(uint8_t);
uint8_t ds_reset_pulse(void);
void Init_DS18b20(void);
void ds_IRQHandler(void);