#include "stm32f10x.h"
#include "Motor.h"
#include "Encoder.h"
#include "Timer.h"

int main(void)
{
    Motor_Init();
    Encoder_Init();
    Timer_Init();

    while (1)
    {
    }
}

