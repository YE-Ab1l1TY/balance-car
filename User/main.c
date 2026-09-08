#include "stm32f10x.h"
#include "Motor.h"
#include "Encoder.h"
#include "Timer.h"
#include "OLED.h"
#include "LED.h"
#include "Delay.h"
#include "Key.h"
#include "MPU6050.h"
#include "Serial.h"
#include "BlueSerial.h"
#include <math.h>


 int16_t AX, AY, AZ, GX, GY, GZ;
 uint16_t TimeErrorFlag;

 float AngleAcc;
 float AngleGyro;
 float Angle;

 int main(void)
 {
    OLED_Init();
    MPU6050_Init();
    BlueSerial_Init();

    Timer_Init();

    while(1)
    {
        
        OLED_Printf(0, 0, OLED_8X16, "%+06d", AX);
        OLED_Printf(0, 16, OLED_8X16, "%+06d", AY);
        OLED_Printf(0, 32, OLED_8X16, "%+06d", AZ);
        OLED_Printf(64, 0, OLED_8X16, "%+06d", GX);
        OLED_Printf(64, 16, OLED_8X16, "%+06d", GY);
        OLED_Printf(64, 32, OLED_8X16, "%+06d", GZ);
        OLED_Printf(0, 48, OLED_8X16, "Flag: %d", TimeErrorFlag);
        OLED_Update();  
         
       BlueSerial_Printf("[plot,%f,%f]", AngleAcc, AngleGyro);
    }
}
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
        
        GY -= 130;

        AngleAcc = -atan2(AX, AZ) * 180 / 3.1415926;

        AngleGyro =Angle + GY / 32768.0 * 2000 * 0.001;

        Angle = 0.999 * AngleGyro + 0.001 * AngleAcc;
        
        if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
        {
            TimeErrorFlag = 1;
            TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        }
    }
}