/*=============================================================================
   Copyright (c) 2015-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/ethernet.hpp>
#include <inf/detail/lwip.h>

namespace cycfi { namespace infinity { namespace detail
{
   static void Ethernet_GPIO_Init()
   {
      // GPIO Ports Clock Enable
      __HAL_RCC_GPIOH_CLK_ENABLE();
      __HAL_RCC_GPIOC_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      __HAL_RCC_GPIOG_CLK_ENABLE();
   }

}}}
