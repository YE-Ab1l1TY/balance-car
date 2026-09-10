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
#include <string.h>
#include <stdlib.h>

 int16_t AX, AY, AZ, GX, GY, GZ;
 uint16_t TimeErrorFlag;

 float AngleAcc;
 float AngleGyro;
 float Angle;

uint8_t KeyNum, RunFlag;

int16_t LeftPWM, RightPWM;
int16_t AvePWM, DifPWM;

PID_t AnglePID = {
    .Kp = 4.5,
    .Ki = 0.2,
    .Kd = 3.1,

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
            if (RunFlag == 0)
            {
                PID_Init(&AnglePID);
                RunFlag = 1;
            }
            else
            {
                RunFlag = 0;
            }
        
        }

        
        OLED_Printf(0, 0, OLED_6X8, "  Angle");
        OLED_Printf(0, 8, OLED_6X8, "p:%05.2f",AnglePID.Kp);
        OLED_Printf(0, 16, OLED_6X8, "i:%05.2f",AnglePID.Ki);
        OLED_Printf(0, 24, OLED_6X8, "d:%05.2f",AnglePID.Kd);
        OLED_Printf(0, 32, OLED_6X8, "T:%05.1f",AnglePID.Target);
        OLED_Printf(0, 40, OLED_6X8, "A:%05.1f",Angle);
        OLED_Printf(0, 48, OLED_6X8, "O:%05.1f",AnglePID.Out);
        
        OLED_Update();  

        if (BlueSerial_RxFlag == 1)
		{
			
			char *Tag = strtok(BlueSerial_RxPacket, ",");
			if (strcmp(Tag, "key") == 0)
			{
				char *Name = strtok(NULL, ",");
				char *Action = strtok(NULL, ",");
			
			}
			else if (strcmp(Tag, "slider") == 0)
			{
				char *Name = strtok(NULL, ",");
				char *Value = strtok(NULL, ",");
				
                if (strcmp(Name, "AngleKp") == 0)
                {
                    AnglePID.Kp = atoi(Value);
                }
                else if (strcmp(Name, "AngleKi") == 0)
                {
                    AnglePID.Ki = atof(Value);
                }
                else if (strcmp(Name, "AngleKd") == 0)
                {
                    AnglePID.Kd = atof(Value);
                }
			}
			else if (strcmp(Tag, "joystick") == 0)
			{
				int8_t LH = atoi(strtok(NULL, ","));
				int8_t LV = atoi(strtok(NULL, ","));
				int8_t RH = atoi(strtok(NULL, ","));
				int8_t RV = atoi(strtok(NULL, ","));
				
                AnglePID.Target = LV / 10;
                DifPWM = RH / 2;
                
			}
			
			BlueSerial_RxFlag = 0;
		}
         
       BlueSerial_Printf("[plot,%f,%f]", AnglePID.Target, Angle);
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

            AngleAcc += 0.5;

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
                AvePWM = -AnglePID.Out;

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