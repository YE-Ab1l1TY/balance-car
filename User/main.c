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
#include "PID.h"
#include <math.h>
#include <stdint.h>


 int16_t AX, AY, AZ, GX, GY, GZ;
 uint16_t TimeErrorFlag;

 float AngleAcc;
 float AngleGyro;
 float Angle;

uint8_t KeyNum, RunFlag;

uint16_t LeftPWM, RightPWM;
uint16_t AvePWM, DifPWM;

PID_t AnglePID = {
    .Kp = 0,
    .Ki = 0,
    .Kd = 0,

    .OutMax = 100.0f,
    .OutMin = -100.0f,

};

 int main(void)
 {
    OLED_Init();
    MPU6050_Init();
    BlueSerial_Init();
    LED_Init();
    Key_Init();
    Motor_Init();
    Encoder_Init();
    Serial_Init();

    Timer_Init();

    while(1)
    {
        if (RunFlag)
        {
            LED_ON();
        }
        else
        {
            LED_OFF();
        }

        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            RunFlag = !RunFlag;
        
        }

        
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
    static uint16_t Count0;

    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

        Key_Tick();

        Count0++;
        if (Count0 >= 10)
        {
            Count0 = 0;

            MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
            
            GY -= 130;

            AngleAcc = -atan2(AX, AZ) * 180 / 3.1415926;

            AngleGyro =Angle + GY / 32768.0 * 2000 * 0.01;

            Angle = 0.99 * AngleGyro + 0.01 * AngleAcc;

            if (Angle  > 50 || Angle < -50)
            {
                RunFlag = 0;
            }

            if (RunFlag)
            {
                AnglePID.Actual = Angle;
                PID_Update(&AnglePID);
                AvePWM = AnglePID.Out;

                LeftPWM = AvePWM + DifPWM / 2;
                RightPWM = AvePWM - DifPWM / 2;

                if (LeftPWM > 100) {LeftPWM = 100;} else if (LeftPWM < -100) {LeftPWM = -100;}
                if (RightPWM > 100) {RightPWM = 100;} else if (RightPWM < -100) {RightPWM = -100;}
                
                Motor_SetPWM(1, LeftPWM);
                Motor_SetPWM(2, RightPWM);
                
            }
            else
            {
                Motor_SetPWM(1, 0);
                Motor_SetPWM(2, 0);
            }
            
        }


        if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
        {
            TimeErrorFlag = 1;
            TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        }
    }
}