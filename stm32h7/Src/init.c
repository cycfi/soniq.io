#include "stm32h7xx_hal.h"
#include <inf/device.hpp>

void SystemClock_Config()
{
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

   // Supply configuration update enable
   HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

   // Configure the main internal regulator output voltage
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

   while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

   // Initializes the CPU, AHB and APB busses clocks
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 1;
   RCC_OscInitStruct.PLL.PLLN = 24;
   RCC_OscInitStruct.PLL.PLLP = 2;
   RCC_OscInitStruct.PLL.PLLQ = 4;
   RCC_OscInitStruct.PLL.PLLR = 2;
   RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
   RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
   RCC_OscInitStruct.PLL.PLLFRACN = 0;

   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
      Error_Handler();

   // Initializes the CPU, AHB and APB busses clocks
   RCC_ClkInitStruct.ClockType =
         RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
      |  RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
      |  RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1
      ;

   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
   RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
   RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
      Error_Handler();
}

void Error_Handler()
{
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif


