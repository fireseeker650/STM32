#include "menu.h"
#include "KEY.h"
#include "OLED.h"
#include "flowing_led.h"
#include "stm32f10x.h"
#include "stdint.h"
#include "Encode.h"

uint8_t current_menu=0;
uint8_t column=1;
uint8_t FLAG=0;

extern uint8_t Key_Flag[KEY_COUNT];
static uint8_t sp=0;
static uint8_t di=0;
static float kp=0.8;
static float ki=0.1;
static float kd=0.5;
static uint8_t menu_idex[5]={0,1,2,3,4};

static uint8_t column_max=4;
static uint8_t column_min=1;

static uint8_t temp=1;

static const float ENCODER_STEP = 0.1;

void Key_Init();

void key_up_down(void)
{
	if(FLAG==0)
	{
		if( current_menu == 0)
		{
			if(Key_Check(KEY_1, KEY_DOWN))
			{
				uint8_t old_column = column;
				column = column - 1;
				if(column < column_min)
				{
					column = column_max;
				}
				OLED_ShowChar(old_column, 1, ' ');
				OLED_ShowChar(column, 1, '>');
				Key_Flag[KEY_1] = 0;  // 立即清除
			}
			if(Key_Check(KEY_2, KEY_DOWN))
			{
				uint8_t old_column = column;
				column = column + 1;
				if(column > column_max)
				{
					column = column_min;
				}
				OLED_ShowChar(old_column, 1, ' ');
				OLED_ShowChar(column, 1, '>');
				Key_Flag[KEY_2] = 0;
			}
		}
	}

}
uint8_t key_sure(void)
{
	if(KEY_3,KEY_DOWN)
	{
		FLAG=1;
	}
	if(KEY_4,KEY_DOWN)
	{
		FLAG=0;
	}
	return FLAG;
}

void Menu_Init(void)
{
	OLED_Clear();
}

uint8_t bian(uint8_t Temp)
{
	if(Temp!=current_menu)
	{
		Menu_Init();
		Temp=current_menu;
	}
}
//void Encoder_AdjustPID(void)
//{
//    int16_t encoder_change = Encoder_Get();
//    
//    if(encoder_change != 0)
//    {
//        if(encoder_change > 5) encoder_change = 5;
//        if(encoder_change < -5) encoder_change = -5;
//		
//		float adjustment = encoder_change * ENCODER_STEP/2;
//        float old_kp = kp, old_ki = ki, old_kd = kd;  // 保存旧值

//		if(column==2)
//		{
//			kp += adjustment;
//			if(kp > 9.9) kp = 9.9;
//			if((old_kp < 0 && kp >= 0) || (old_kp >= 0 && kp < 0)) 
//			{
//                    OLED_ShowString(2, 5, "    ");  // 清除旧显示
//			}	
//		}
//		if(column==3)
//		{
//			ki += adjustment;
//			if(ki > 9.9) ki = 9.9;
//			if((old_ki < 0 && ki >= 0) || (old_ki >= 0 && ki < 0)) 
//			{
//				OLED_ShowString(3, 5, "    ");
//			}
//		}
//		if(column==4)
//		{
//			kd += adjustment;
//			if(kd > 9.9) kd = 9.9;
//			if((old_kd < 0 && ki >= 0) || (old_kd >= 0 && ki < 0)) 
//			{
//                    OLED_ShowString(3, 5, "    ");
//			}
//		}
//		OLED_ShowFloat(2, 5, kp, 1, 1);
//        OLED_ShowFloat(3, 5, ki, 1, 1);
//        OLED_ShowFloat(4, 5, kd, 1, 1);
//    }
//}
void Encoder_AdjustPID(void)
{
    int16_t encoder_change = Encoder_Get();
    
    if(encoder_change != 0)
    {
        if(encoder_change > 5) encoder_change = 5;
        if(encoder_change < -5) encoder_change = -5;
        
        float adjustment = encoder_change * ENCODER_STEP / 2;
        
        if(column==2)
        {
            kp += adjustment;
            
            if(kp > 9.9f) kp = 9.9f;
            // 清除显示区域（包括可能的负号）
            OLED_ShowString(2, 4, "     ");  // 清除从第4列开始的5个字符
        }
        if(column==3)
        {
            ki += adjustment;
            
            if(ki > 9.9f) ki = 9.9f;
            OLED_ShowString(3, 4, "     ");
        }
        if(column==4)
        {
            kd += adjustment;
            
            if(kd > 9.9f) kd = 9.9f;
            OLED_ShowString(4, 4, "     ");
        }
        
        // 重新显示所有PID参数
        OLED_ShowFloat(2, 5, kp, 1, 1);
        OLED_ShowFloat(3, 5, ki, 1, 1);
        OLED_ShowFloat(4, 5, kd, 1, 1);
    }
}
		

void Main_Menu(void)
{
	
	void key_up_down();
	
	OLED_ShowChar(column,1,'>');
	OLED_ShowString(1,2,"LED Control");
	OLED_ShowString(2,2,"PID");
	OLED_ShowString(3,2,"Image");
	OLED_ShowString(4,2,"Angle");
	while(temp!=column)
	{
		OLED_ShowChar(temp,1,' ');
		OLED_ShowChar(column,1,'>');
		temp=column;
	}
	if(Key_Check(KEY_3,KEY_DOWN))
	{
		current_menu=column;
//		Menu_Select(current_menu);
		FLAG=0;
	}
	
	
	
	
}
void Menu_LEDControl(void)
{
	
    if (Key_Check(KEY_3, KEY_DOWN))
    {
        FLAG = !FLAG;  
    }
    
    OLED_ShowChar(column, 1, '>');
    static uint8_t first_enter = 1;
    if(first_enter) 
	{
        OLED_ShowString(1, 1, "LED Control");
        OLED_ShowString(2, 2, "LED_speed");
        OLED_ShowString(3, 2, "LED_dir");
        first_enter = 0;
    }
    OLED_ShowNum(2, 14, sp, 1);
    OLED_ShowNum(3, 14, di, 1);
    
    
    if (FLAG == 1)
    {
        OLED_ShowChar(1, 15, 'E');
        
        
         if (Key_Check(KEY_1, KEY_DOWN))
        {
            if (column == 2)  
            {
                sp = (sp + 1) % 3;  
                
             
                if(sp == 0)
                {
                    FlowLED_SetSpeed(500);
                }
                else if(sp == 1)
                {
                    FlowLED_SetSpeed(1000);
                }
                else if(sp == 2)
                {
                    FlowLED_SetSpeed(200);
                }
            }
            else if (column == 3)  
            {
                di = !di;  
                FlowLED_SetDirection(di);  
            }
        }
        else if (Key_Check(KEY_2, KEY_DOWN))
        {
            if (column == 2)  
            {
                sp = (sp + 2) % 3; 
                
               
                if(sp == 0)
                {
                    FlowLED_SetSpeed(500);
                }
                else if(sp == 1)
                {
                    FlowLED_SetSpeed(1000);
                }
                else if(sp == 2)
                {
                    FlowLED_SetSpeed(200);
                }
            }
            else if (column == 3)  
            {
                di = !di;  
                FlowLED_SetDirection(di); 
            }
        }
    }
    else
    {

        OLED_ShowChar(1, 15, ' ');
		if (Key_Check(KEY_1, KEY_DOWN))
        {
            uint8_t old_column = column;
            column = column - 1;
            if (column < column_min) column = column_max;
            OLED_ShowChar(old_column, 1, ' ');
            OLED_ShowChar(column, 1, '>');
        }
        if (Key_Check(KEY_2, KEY_DOWN))
        {
            uint8_t old_column = column;
            column = column + 1;
            if (column > column_max) column = column_min;
            OLED_ShowChar(old_column, 1, ' ');
            OLED_ShowChar(column, 1, '>');
        }
        
        Key_ClearRepeatFlags();

	}
        
    
    
 
    while(temp != column)
    {
        OLED_ShowChar(temp, 1, ' ');
        OLED_ShowChar(column, 1, '>');
        temp = column;
    }
    
    OLED_ShowNum(2, 14, sp, 1);
    OLED_ShowNum(3, 14, di, 1);
}
// 专门的PID菜单按键处理函数
void PID_Key_Handler(void)
{
    static uint8_t last_FLAG = 0xFF;

    
    // FLAG状态变化时清除所有按键标志
    if(last_FLAG != FLAG)
    {
        for(uint8_t i = 0; i < KEY_COUNT; i++)
        {
            Key_Flag[i] = 0;
        }

        last_FLAG = FLAG;
    }
    
    if(FLAG == 1)  // 编辑模式
    {
        // KEY_1: 增加参数值 - 使用KEY_DOWN而不是KEY_SINGLE/REPEAT
        if(Key_Check(KEY_1, KEY_DOWN)|| Key_Check(KEY_1, KEY_REPEAT))
        {
            switch(column)
            {
                case 2: 
                    kp += 0.1f;
                    if(kp > 9.9f) kp = 9.9f;
                    break;
                case 3: 
                    ki += 0.1f;
                    if(ki > 9.9f) ki = 9.9f;
                    break;
                case 4: 
                    kd += 0.1f;
                    if(kd > 9.9f) kd = 9.9f;
                    break;
            }
			OLED_ShowString(column+1, 4, "     ");
            // 更新显示
            OLED_ShowFloat(2, 5, kp, 1, 1);
            OLED_ShowFloat(3, 5, ki, 1, 1);
            OLED_ShowFloat(4, 5, kd, 1, 1);
            // 立即清除按键标志，避免重复触发
            if(Key_Check(KEY_1, KEY_REPEAT))
            {
                Key_Flag[KEY_1] &= ~KEY_REPEAT;
            }
        }
        
        // KEY_2: 减少参数值 - 使用KEY_DOWN而不是KEY_SINGLE/REPEAT
        if(Key_Check(KEY_2, KEY_DOWN)||Key_Check(KEY_2, KEY_REPEAT))
        {
            switch(column)
            {
                case 2: 
                    kp -= 0.1f;
                    
                    break;
                case 3: 
                    ki -= 0.1f;
                    
                    break;
                case 4: 
                    kd -= 0.1f;
                    
                    break;
            }
            OLED_ShowString(column+1, 4, "     ");
            // 更新显示
            OLED_ShowFloat(2, 5, kp, 1, 1);
            OLED_ShowFloat(3, 5, ki, 1, 1);
            OLED_ShowFloat(4, 5, kd, 1, 1);
            
            // 清除重复标志，避免过快
            if(Key_Check(KEY_2, KEY_REPEAT))
            {
                Key_Flag[KEY_2] &= ~KEY_REPEAT;
            }
        }
    }
    else  // 非编辑模式
    {
        // KEY_1: 上移光标
        if(Key_Check(KEY_1, KEY_DOWN))
        {
            uint8_t old_column = column;
            column = column - 1;
            if(column < column_min) column = column_max;
            
            OLED_ShowChar(old_column, 1, ' ');
            OLED_ShowChar(column, 1, '>');
            
            Key_Flag[KEY_1] = 0;  // 立即清除
        }
        
        // KEY_2: 下移光标
        if(Key_Check(KEY_2, KEY_DOWN))
        {
            uint8_t old_column = column;
            column = column + 1;
            if(column > column_max) column = column_min;
            
            OLED_ShowChar(old_column, 1, ' ');
            OLED_ShowChar(column, 1, '>');
            
            Key_Flag[KEY_2] = 0;  // 立即清除
        }
    }
    
    // KEY_3: 切换编辑模式
    if(Key_Check(KEY_3, KEY_DOWN))
    {
        FLAG = !FLAG;
        Key_Flag[KEY_3] = 0;  // 立即清除
    }
}
//void Menu_PID(void)
//{
//	
//	if (Key_Check(KEY_3, KEY_DOWN))
//    {
//        FLAG = !FLAG;  // 切换编辑模式
//		Key_ClearRepeatFlags();
//    }
//    
//	
//	
//	OLED_ShowChar(column,1,'>');
//	
//    OLED_ShowString(1, 1, "PID");
//    OLED_ShowString(2, 2, "kp");
//    OLED_ShowString(3, 2, "ki");
//    OLED_ShowString(4, 2, "kd");
//	
//        
//        
//    
//	
//	if(FLAG==1)
//	{
//		OLED_ShowChar(1, 15, 'E');
//		Encoder_AdjustPID();
//		if(Key_Check(KEY_1,KEY_SINGLE)||Key_Check(KEY_1,KEY_REPEAT))
//		{
//			if(column==2)
//			{
//				kp+=0.1;
//			}
//			if(column==3)
//			{
//				ki+=0.1;
//			}
//			if(column==4)
//			{
//				kd+=0.1;
//			}
//		}
//		if(Key_Check(KEY_2,KEY_SINGLE)||Key_Check(KEY_2,KEY_REPEAT))
//		{
//			if(column==2)
//			{
//				kp-=0.1;
//			}
//			if(column==3)
//			{
//				ki-=0.1;
//			}
//			if(column==4)
//			{
//				kd-=0.1;
//			}
//		}
//		if(Key_Check(KEY_1, KEY_REPEAT) || Key_Check(KEY_2, KEY_REPEAT))
//        {
//            Key_ClearRepeatFlags();
//        }
//	}
//	else
//	{
//		OLED_ShowChar(1, 15, ' ');
//		if (Key_Check(KEY_1, KEY_DOWN))
//        {
//            uint8_t old_column = column;
//            column = column - 1;
//            if (column < column_min)
//            {
//                column = column_max;
//            }
//    
//            OLED_ShowChar(old_column, 1, ' ');
//            OLED_ShowChar(column, 1, '>');
//        }
//        if (Key_Check(KEY_2, KEY_DOWN))
//        {
//            uint8_t old_column = column;
//            column = column + 1;
//            if (column > column_max)
//            {
//                column = column_min;
//            }
//            OLED_ShowChar(old_column, 1, ' ');
//            OLED_ShowChar(column, 1, '>');
//        }
//        Key_ClearRepeatFlags();
//        
//	}
//	OLED_ShowFloat(2,5,kp,1,1);
//	OLED_ShowFloat(3,5,ki,1,1);
//	OLED_ShowFloat(4,5,kd,1,1);
//	Key_ClearRepeatFlags();
//}
void Menu_PID(void)
{
    static uint8_t initialized = 0;
    
    // 初始化显示（只在第一次进入时）
    if(!initialized)
    {
        OLED_ShowString(1, 1, "PID        ");
        OLED_ShowString(2, 2, "kp");
        OLED_ShowString(3, 2, "ki");
        OLED_ShowString(4, 2, "kd");
        OLED_ShowChar(column, 1, '>');
        initialized = 1;
    }
    
    // 处理按键
    PID_Key_Handler();
    
    // 编码器调整（只在编辑模式下）
    if(FLAG == 1)
    {
        Encoder_AdjustPID();
    }
    
    // 更新显示状态
    if(FLAG == 1)
    {
        OLED_ShowChar(1, 15, 'E');
    }
    else
    {
        OLED_ShowChar(1, 15, ' ');
    }
    
    // 确保光标显示正确
    OLED_ShowChar(column, 1, '>');
    
    // 实时更新PID参数显示
    OLED_ShowFloat(2, 5, kp, 1, 1);
    OLED_ShowFloat(3, 5, ki, 1, 1);
    OLED_ShowFloat(4, 5, kd, 1, 1);
    
    // 微延迟，避免处理过快
    for(volatile int i = 0; i < 1000; i++);
}
void Menu_Image(void)
{
	
	void key_up_down();
	Menu_Init();
	column=2;
	OLED_ShowChar(column,1,'>');
	OLED_ShowString(1,1,"Image");
	OLED_ShowString(2,2,"Image");
	while(temp!=column)
	{
		OLED_ShowChar(temp,1,' ');
		OLED_ShowChar(column,1,'>');
		temp=column;
	}
}
void Menu_Angle(void)
{
	
	void key_up_down();
	Menu_Init();
	column=2;
	OLED_ShowChar(column,1,'>');
	OLED_ShowString(1,1,"Angle");
	OLED_ShowString(2,2,"Angle");
	while(temp!=column)
	{
		OLED_ShowChar(temp,1,' ');
		OLED_ShowChar(column,1,'>');
		temp=column;
	}
}

uint8_t Menu_Select(uint8_t current_menu)
{
//	switch(current_menu)
//	{
//		case0:column_min=1;column_max=4; Main_Menu();break;
//		case1:column_min=2;column_max=3;Menu_Init();Menu_LEDControl();break;
//		case2:column_min=2;column_max=4;Menu_Init();Menu_PID();break;
//		case3:column_min=2;column_max=2;Menu_Init();Menu_Image();break;
//		case4:column_min=2;column_max=2;Menu_Init();Menu_Angle();break;
//	}
//	return current_menu;
	static uint8_t last_menu = 0xFF; // 记录上一次的菜单
    
    // 只有菜单切换时才清屏
    if(current_menu != last_menu)
    {
        Menu_Init(); // 清屏
        last_menu = current_menu;
		FLAG=0;
        for(uint8_t i = 0; i < KEY_COUNT; i++)
        {
            Key_Flag[i] = 0;
        }
        // 重置光标位置到合理值
        if(current_menu == 0) {
            column_min = 1; column_max = 4;column = 1; 
        } else if(current_menu == 1) {
            column_min = 2; column_max = 3;column = 2; 
            column = 2; // 重置到第一个选项
        } else if(current_menu == 2) {
            column_min = 2; column_max = 4;column = 2; 
            column = 2; // 重置到第一个选项
        } else if(current_menu == 3 || current_menu == 4) {
            column_min = 2; column_max = 2;column = 2; 
            column = 2;
        }
		 
    }
    
    if(current_menu == 0)
    {
        Main_Menu();
    }
    else if(current_menu == 1)
    {
        Menu_LEDControl();
    }
    else if(current_menu == 2)
    {
        Menu_PID();
    }
    else if(current_menu == 3)
    {
        Menu_Image();
    }
    else if(current_menu == 4)
    {
        Menu_Angle();
    }
    
    return current_menu;
	
}



