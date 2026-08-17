cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

project(phc LANGUAGES C ASM)

add_executable(phc
    User/main.c
    User/stm32f10x_it.c
    Hardware/PWM.c
    Hardware/Motor.c
    Hardware/Encoder.c
    Hardware/LED.c
    Hardware/OLED.c
    Hardware/OLED_Data.c
    System/Delay.c
    System/Timer.c
    Start/system_stm32f10x.c
    Start/startup_stm32f10x_md.s
    Library/misc.c
    Library/stm32f10x_gpio.c
    Library/stm32f10x_rcc.c
    Library/stm32f10x_tim.c
)

target_include_directories(phc PRIVATE
    User
    Hardware
    System
    Library
    Start
)

target_compile_definitions(phc PRIVATE
    USE_STDPERIPH_DRIVER
    STM32F10X_MD
)

target_compile_options(phc PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-Wall;-Wextra>
)

target_link_libraries(phc PRIVATE m)

add_custom_command(TARGET phc POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:phc>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:phc> phc.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:phc> phc.bin
)
