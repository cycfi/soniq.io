/*=============================================================================
   Copyright (c) 2015-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <soniq/timer.hpp>

namespace cycfi::soniq
{
   using timer_handler_array = std::array<std::function<void()>, 7>;
   timer_handler_array _timer_interrupt_handlers;
}

namespace cycfi::soniq::detail
{
   template <uint32_t ID>
   void handle_timer_interrupt()
   {
      auto& timer = get_timer<ID>();

      // Check whether update interrupt is pending
      if (LL_TIM_IsActiveFlag_UPDATE(&timer) == 1)
      {
         // Clear the update interrupt flag
         LL_TIM_ClearFlag_UPDATE(&timer);
      }
      // call timer_task
      if (_timer_interrupt_handlers[ID])
         _timer_interrupt_handlers[ID]();
   }
}

extern "C"
{
#define TIMER_INTERRUPT_HANDLER(N, NAME)                                       \
   void NAME()                                                                 \
   {                                                                           \
      using cycfi::soniq::detail::handle_timer_interrupt;                      \
      handle_timer_interrupt<N>();                                             \
   }                                                                           \
   /***/

#if defined(TIM1)
 TIMER_INTERRUPT_HANDLER(1, TIM1_UP_TIM16_IRQHandler)
#endif

#if defined(TIM2)
 TIMER_INTERRUPT_HANDLER(2, TIM2_IRQHandler)
#endif

#if defined(TIM3)
 TIMER_INTERRUPT_HANDLER(3, TIM3_IRQHandler)
#endif

#if defined(TIM4)
 TIMER_INTERRUPT_HANDLER(4, TIM4_IRQHandler)
#endif

#if defined(TIM5)
 TIMER_INTERRUPT_HANDLER(5, TIM5_IRQHandler)
#endif

#if defined(TIM7)
 TIMER_INTERRUPT_HANDLER(7, TIM7_IRQHandler)
#endif

#if defined(TIM8)
 TIMER_INTERRUPT_HANDLER(8, TIM8_UP_IRQHandler)
#endif
}

