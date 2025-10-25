#include "flowing_led.h"
#include "stm32f10x.h"
#include "OLED.h"
#include "menu.h"

// 流水灯状态变量
static uint8_t led_state = 0x08;  // 初始状态：PB15亮 (1000)
static uint8_t led_index = 0;     // 当前LED索引
static uint16_t speed=500;


static uint8_t dir = 0;    // 添加方向变量

static const uint16_t LED_PINS[] = {
    GPIO_Pin_15,  
    GPIO_Pin_14,    
    GPIO_Pin_13,  
    GPIO_Pin_12   
};

void FlowLED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    

    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}


void FlowLED_TIM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
	TIM_TimeBaseStructure.TIM_Period = 100 - 1;          
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;      
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    

    TIM_Cmd(TIM2, ENABLE);
}


void FlowLED_Init(void)
{
    FlowLED_GPIO_Init();
    FlowLED_TIM_Init();
    


}

void LED_Tick(void)
{
	static uint32_t tick_count =0;
	tick_count ++;
	if(tick_count >=speed)
	{
		GPIO_ResetBits(GPIOB, LED_PINS[led_index]);
        
      
        if(dir == 0)  // 向右
        {
            led_index++;
            if(led_index >= 4)
                led_index = 0;
        }
        else  // 向左
        {
             if(led_index == 0)
            {
                led_index = 3;  // 从第一个跳到最后一个
            }
            else
            {
                led_index--;    // 正常向左移动
            }
        }
        
        // 点亮下一个LED
        GPIO_SetBits(GPIOB, LED_PINS[led_index]);
		tick_count =0;
		

	}

}


// 设置速度函数
void FlowLED_SetSpeed(uint16_t new_speed)
{
    speed = new_speed;
}

// 设置方向函数
void FlowLED_SetDirection(uint8_t new_dir)
{
    dir = new_dir;
    // 如果需要立即改变方向，可以在这里添加逻辑
}

// 获取速度函数
uint16_t FlowLED_GetSpeed(void)
{
    return speed;
}

// 获取方向函数  
uint8_t FlowLED_GetDirection(void)
{
    return dir;
}
/*
void FlowLED_SetSpeed(uint32_t speed)
{
    led_speed = speed;
}

*
  * @brief  设置流水灯方向
  * @param  dir: 方向 (0=向右, 1=向左)
  * @retval None
  
void FlowLED_SetDirection(uint8_t dir)
{
    direction = dir;
}

*
  * @brief  更新流水灯状态（非阻塞）
  * @param  None
  * @retval None
  
void TIM2_IRQHandler(void)
{

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        // 关闭当前LED
        switch(current_led)
        {
            case 0: GPIO_ResetBits(GPIOB, LED1_PIN); break;
            case 1: GPIO_ResetBits(GPIOB, LED2_PIN); break; 
            case 2: GPIO_ResetBits(GPIOB, LED3_PIN); break;
            case 3: GPIO_ResetBits(GPIOB, LED4_PIN); break;
        }
        
        // 根据方向移动LED
        if(direction == 0)
        {
            // 向右移动：PB15→PB14→PB13→PB12→PB15...
            current_led++;
            if(current_led > 3) current_led = 0;  // 防止下溢
        }
        else
        {
            // 向左移动：PB12→PB13→PB14→PB15→PB12...
            current_led--;
            if(current_led <0) current_led = 3;
        }
        
        // 点亮新的LED
        switch(current_led)
        {
            case 0: GPIO_SetBits(GPIOB, LED1_PIN); break;  // PB12
            case 1: GPIO_SetBits(GPIOB, LED2_PIN); break;  // PB13
            case 2: GPIO_SetBits(GPIOB, LED3_PIN); break;  // PB14  
            case 3: GPIO_SetBits(GPIOB, LED4_PIN); break;  // PB15
        }
    
}

*
  * @brief  获取当前LED状态（调试用）
  * @retval 当前亮的LED编号(0-3)
  
uint8_t FlowLED_GetCurrentLED(void)
{
    return current_led;
}

*
  * @brief  获取当前方向（调试用）
  * @retval 当前方向(0=右,1=左)
  
uint8_t FlowLED_GetDirection(void)
{
    return direction;
}

*
  * @brief  获取当前速度（调试用）
  * @retval 当前速度(ms)
  
uint32_t FlowLED_GetSpeed(void)
{
    return led_speed;
}
*/




