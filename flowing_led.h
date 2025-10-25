#ifndef __FLOWING_LED_H
#define __FLOWING_LED_H

#include "stm32f10x.h"

// 函数声明
void FlowLED_Init(void);
void FlowLED_GPIO_Init(void);
void FlowLED_TIM_Init(void);
void LED_Tick(void);

void FlowLED_SetSpeed(uint16_t new_speed);
void FlowLED_SetDirection(uint8_t new_dir);
uint16_t FlowLED_GetSpeed(void);
uint8_t FlowLED_GetDirection(void);




#endif 
