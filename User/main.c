#include "stm32f10x.h"
#include "Motor.h"
#include "Encoder.h"
#include "Timer.h"
#include "OLED.h"
#include "LED.h"
#include "Delay.h"

uint16_t count;

int main(void)
{
    Timer_Init();

    OLED_Init();

    
    while (1)
    {
        OLED_Printf(0, 0, OLED_8X16, "Counter: %05d", count);
        OLED_Update();

    }

}

void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        count++;
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
