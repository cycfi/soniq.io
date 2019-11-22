#include "stm32h7xx_it.h"
#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler()
{
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler()
{
   while (1)
   {
   }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler()
{
   while (1)
   {
   }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler()
{
   while (1)
   {
   }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
   while (1)
   {
   }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

extern TIM_HandleTypeDef htim17;

/**
  * @brief This function handles TIM17 global interrupt.
  */
void TIM17_IRQHandler(void)
{
   HAL_TIM_IRQHandler(&htim17);
}

