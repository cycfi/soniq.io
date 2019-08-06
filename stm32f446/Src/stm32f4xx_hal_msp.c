#include "stm32f4xx_hal.h"

void HAL_MspInit()
{
   __HAL_RCC_SYSCFG_CLK_ENABLE();
   __HAL_RCC_PWR_CLK_ENABLE();
}

