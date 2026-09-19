# Third-Party Notices

This repository contains third-party source code in addition to original code written for this project.

The repository-level MIT License applies only to original project code authored for this repository. Third-party code remains subject to the copyright and usage terms of its original authors and licensors.

## STMicroelectronics

Files under `Library/` and parts of `Start/` originate from the STM32F10x Standard Peripheral Library and related STM32 device support files provided by STMicroelectronics.

Examples include:

- `Library/stm32f10x_*.c`
- `Library/stm32f10x_*.h`
- `Library/misc.c`
- `Library/misc.h`
- `Start/stm32f10x.h`
- `Start/system_stm32f10x.c`
- `Start/system_stm32f10x.h`

Their original copyright and usage notices are retained in the source files and continue to apply.

## Arm CMSIS

Some files under `Start/` originate from Arm CMSIS components.

Examples include:

- `Start/core_cm3.c`
- `Start/core_cm3.h`

Their original Arm copyright and redistribution notices are retained in the source files and continue to apply.

## 江协科技

The following drivers were ported from code published by 江协科技:

- `Hardware/OLED.c`
- `Hardware/OLED.h`
- `Hardware/OLED_Data.c`
- `Hardware/OLED_Data.h`
- `Hardware/NRF24L01.c`
- `Hardware/NRF24L01.h`
- `Hardware/NRF24L01_Define.h`

These files are not relicensed under the repository-level MIT License. Their original copyright and usage terms remain in effect. Original notices are retained where provided in the source files.

## Original Project Code

Unless otherwise indicated by an existing copyright or license notice, the remaining project-specific code was written for this project and is covered by the repository-level MIT License.
