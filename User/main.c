#include "stm32f10x.h"
#include "Motor.h"
#include "Encoder.h"
#include "Timer.h"
#include "OLED.h"
#include "LED.h"
#include "Delay.h"
#include "Key.h"
#include "MPU6050.h"


int16_t AX, AY, AZ, GX, GY, GZ;
uint16_t TimeErrorFlag;

int main(void)
{
    OLED_Init();
    MPU6050_Init();
    Timer_Init();

    while(1)
    {

        OLED_Printf(0, 0, OLED_8X16, "%+06d", AX);
        OLED_Printf(0, 16, OLED_8X16, "%+06d", AY);
        OLED_Printf(0, 32, OLED_8X16, "%+06d", AZ);
        OLED_Printf(64, 0, OLED_8X16, "%+06d", GX);
        OLED_Printf(64, 16, OLED_8X16, "%+06d", GY);
        OLED_Printf(64, 32, OLED_8X16, "%+06d", GZ);
        OLED_Printf(0, 48, OLED_8X16, "TimeErrorFlag: %d", TimeErrorFlag);
        OLED_Update();  
     }

}

 void TIM1_UP_IRQHandler(void)
 {
     if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
     {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);

        if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
        {
            TimeErrorFlag = 1;
            TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        }
     }
 }



// uint16_t count;
// uint8_t KeyNum;

// int main(void)
// {
//     Timer_Init();
//     OLED_Init();

//     Key_Init();

    
//     while (1)
//     {
//         KeyNum = Key_GetNum();
//         if (KeyNum)
//         {
//             OLED_Printf(0, 2, OLED_8X16, "Key: %d", KeyNum);
//             OLED_Update();
//         }
//     }
// }

// void TIM1_UP_IRQHandler(void)
// {
//     if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
//     {
//         count++;

//         Key_Tick();
//         TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//     }
// }
