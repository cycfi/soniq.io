/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_TIMER_IMPL_HPP_DECEMBER_21_2015)
#define CYCFI_INFINITY_TIMER_IMPL_HPP_DECEMBER_21_2015

#include <cstdint>

namespace cycfi { namespace infinity { namespace detail
{
   ////////////////////////////////////////////////////////////////////////////
   // This implementation is for the STM32F4 series.
   ////////////////////////////////////////////////////////////////////////////

   constexpr bool check_valid_timer(std::size_t id)
   {
      return (id >= 1 && id <= 14);
   }

   template <std::size_t id>
   TIM_TypeDef& get_timer();

   template <std::size_t id>
   int timer_irqn();

   template <std::size_t id>
   void timer_periph_enable();

   template <std::size_t id>
   std::size_t timer_clock_div();

#define INFINITY_TIMER(ID, TIMER_NAME)                                         \
   template <>                                                                 \
   constexpr inline TIM_TypeDef& get_timer<ID>()                               \
   {                                                                           \
      return *TIMER_NAME;                                                      \
   }                                                                           \
   /***/

#define INFINITY_TIMER_IRQN(ID, IRQN)                                          \
   template <>                                                                 \
   constexpr inline int timer_irqn<ID>()                                       \
   {                                                                           \
      return IRQN;                                                             \
   }                                                                           \
   /***/

#define INFINITY_TIMER_PERIPH_ENABLE(ID, GROUP)                                \
   template <>                                                                 \
   inline void timer_periph_enable<ID>()                                       \
   {                                                                           \
      LL_APB##GROUP##_GRP1_EnableClock(LL_APB##GROUP##_GRP1_PERIPH_TIM##ID);   \
   }                                                                           \
                                                                               \
   template <>                                                                 \
   constexpr inline std::size_t timer_clock_div<ID>()                          \
   {                                                                           \
      return (GROUP == 1)? 2 : 1;                                              \
   }                                                                           \
   /***/

#if defined(TIM1)
 INFINITY_TIMER(1, TIM1)
 INFINITY_TIMER_IRQN(1, -1)
 INFINITY_TIMER_PERIPH_ENABLE(1, 2)
#endif

#if defined(TIM2)
 INFINITY_TIMER(2, TIM2)
 INFINITY_TIMER_IRQN(2, TIM2_IRQn)
 INFINITY_TIMER_PERIPH_ENABLE(2, 1)
#endif

#if defined(TIM3)
 INFINITY_TIMER(3, TIM3)
 INFINITY_TIMER_IRQN(3, TIM3_IRQn)
 INFINITY_TIMER_PERIPH_ENABLE(3, 1)
#endif

#if defined(TIM4)
 INFINITY_TIMER(4, TIM4)
 INFINITY_TIMER_IRQN(4, TIM4_IRQn)
 INFINITY_TIMER_PERIPH_ENABLE(4, 1)
#endif

#if defined(TIM5)
 INFINITY_TIMER(5, TIM5)
 INFINITY_TIMER_IRQN(5, TIM5_IRQn)
 INFINITY_TIMER_PERIPH_ENABLE(5, 1)
#endif

#if defined(TIM6)
 INFINITY_TIMER(6, TIM6)
 INFINITY_TIMER_IRQN(6, -1)
 INFINITY_TIMER_PERIPH_ENABLE(6, 1)
#endif

#if defined(TIM7)
 INFINITY_TIMER(7, TIM7)
 INFINITY_TIMER_IRQN(7, TIM7_IRQn)
 INFINITY_TIMER_PERIPH_ENABLE(7, 1)
#endif

#if defined(TIM8)
 INFINITY_TIMER(8, TIM8)
 INFINITY_TIMER_IRQN(8, -1)
 INFINITY_TIMER_PERIPH_ENABLE(8, 2)
#endif

#if defined(TIM9)
 INFINITY_TIMER(9, TIM9)
 INFINITY_TIMER_IRQN(9, -1)
 INFINITY_TIMER_PERIPH_ENABLE(9, 2)
#endif

#if defined(TIM10)
 INFINITY_TIMER(10, TIM10)
 INFINITY_TIMER_IRQN(10, -1)
 INFINITY_TIMER_PERIPH_ENABLE(10, 2)
#endif

#if defined(TIM11)
 INFINITY_TIMER(11, TIM11)
 INFINITY_TIMER_IRQN(11, -1)
 INFINITY_TIMER_PERIPH_ENABLE(11, 2)
#endif

#if defined(TIM12)
 INFINITY_TIMER(12, TIM12)
 INFINITY_TIMER_IRQN(12, -1)
 INFINITY_TIMER_PERIPH_ENABLE(12, 1)
#endif

#if defined(TIM13)
 INFINITY_TIMER(13, TIM13)
 INFINITY_TIMER_IRQN(13, -1)
 INFINITY_TIMER_PERIPH_ENABLE(13, 1)
#endif

#if defined(TIM14)
 INFINITY_TIMER(14, TIM14)
 INFINITY_TIMER_IRQN(14, -1)
 INFINITY_TIMER_PERIPH_ENABLE(14, 1)
#endif

#if defined(STM32H7)
#if defined(TIM15)
 INFINITY_TIMER(15, TIM15)
 INFINITY_TIMER_IRQN(15, -1)
 INFINITY_TIMER_PERIPH_ENABLE(15, 2)
#endif

#if defined(TIM16)
 INFINITY_TIMER(16, TIM16)
 INFINITY_TIMER_IRQN(16, -1)
 INFINITY_TIMER_PERIPH_ENABLE(16, 2)
#endif

#if defined(TIM17)
 INFINITY_TIMER(17, TIM17)
 INFINITY_TIMER_IRQN(17, -1)
 INFINITY_TIMER_PERIPH_ENABLE(17, 2)
#endif
#endif

}}}

#endif
