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
   void                 Error_Handler();
   void                 HAL_Delay(uint32_t Delay);
   uint32_t             HAL_GetTick();

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus)

namespace cycfi { namespace infinity
{
   inline void system_clock_config()
   {
      SystemClock_Config();
   }

   inline void system_init()
   {
      HAL_Init();
   }

   inline void error_handler()
   {
      Error_Handler();
   }
}}

#endif

#endif
