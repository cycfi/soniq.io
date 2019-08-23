/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_DETAIL_DEVICE_HPP_AUGUST_5_2019)
#define CYCFI_INFINITY_DETAIL_DEVICE_HPP_AUGUST_5_2019

#if defined(__cplusplus)
 extern "C" {
#endif

   void                 SystemClock_Config();
   HAL_StatusTypeDef    HAL_Init();
   void                 CPU_Cache_Enable();
   void                 Error_Handler();
   void                 HAL_Delay(uint32_t Delay);
   uint32_t             HAL_GetTick();

#if defined(STM32H7)
   void                 MPU_Config();
#endif

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus)

namespace cycfi { namespace infinity
{
   inline void system_init()
   {
      static bool init = false;
      if (!init)
      {
         init = true;

         SystemClock_Config();

#if defined(STM32H7)
         MPU_Config();
         CPU_Cache_Enable();
#endif
         HAL_Init();
      }
   }

   inline void error_handler()
   {
      Error_Handler();
   }

   inline void invalidate_cache(void* start, std::size_t size)
   {
#if defined(STM32H7)
      SCB_InvalidateDCache_by_Addr((uint32_t*) start, size);
#endif
   }
}}

#endif

#endif
