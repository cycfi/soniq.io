#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"

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
void UsageFault_Handler()
{
   while (1)
   {
   }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler()
{
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler()
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler()
{
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler()
{
   HAL_IncTick();
}

