#ifndef __MENU_H
#define __MENU_H

#include "stdint.h"



static uint8_t Temp=0;
extern uint8_t current_menu;
extern uint8_t column;
extern uint8_t FLAG;
// 菜单系统函数声明
void Menu_Init(void);
void Main_Menu(void);
void Menu_LEDControl(void);
void Menu_PID(void);void PID_Key_Handler(void);
void Menu_Image(void);
void Menu_Angle(void);
void key_up_down(void);
uint8_t Menu_Select(uint8_t);
uint8_t Menu_GetCurrentIndex(void);
uint8_t key_sure(void);
void Encoder_AdjustPID(void);
uint8_t bian(uint8_t Temp);

#endif
