/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_IRQ_IMPL_HPP_DECEMBER_22_2015)
#define CYCFI_INFINITY_IRQ_IMPL_HPP_DECEMBER_22_2015

///////////////////////////////////////////////////////////////////////////////
// This implementation is for the STM32F4 series.
///////////////////////////////////////////////////////////////////////////////

#include <soniq/pin.hpp>
#include <soniq/timer.hpp>
#include <soniq/adc.hpp>
#include <type_traits>

#if defined(STM32H7)
# include <stm32h7xx_ll_dma.h>
# include <stm32h7xx_ll_adc.h>
#elif defined(STM32F4)
# include <stm32f4xx_ll_dma.h>
# include <stm32f4xx_ll_adc.h>
#else
# error "Device not supported"
#endif

///////////////////////////////////////////////////////////////////////////////
// Timer Interrupts
///////////////////////////////////////////////////////////////////////////////

namespace cycfi::soniq::detail
{
   template <std::size_t N>
   void handle_exti()
   {
      static_assert(N < 16, "Invalid EXTI ID");

      // If the interrupt task is not handled anyway, this will
      // be compiled away by the compiler
      if (!std::is_same<decltype(
            ::config(identity<cycfi::soniq::exti_id<N>>{})
         ), cycfi::soniq::irq_not_handled>::value)
      {
         // Manage Flags
         if (LL_EXTI_IsActiveFlag_0_31(detail::exti_src<N>()) != RESET)
         {
            // Clear EXTI flag
            LL_EXTI_ClearFlag_0_31(detail::exti_src<N>());

            // Handle exti task
            ::config(identity<cycfi::soniq::exti_id<N>>{});
         }
      }
   }

   template <std::size_t N>
   void handle_timer_interrupt()
   {
      auto& timer = cycfi::soniq::detail::get_timer<N>();

      // Check whether update interrupt is pending
      if (LL_TIM_IsActiveFlag_UPDATE(&timer) == 1)
      {
         // Clear the update interrupt flag
         LL_TIM_ClearFlag_UPDATE(&timer);
      }
      // call timer_task
      ::config(identity<cycfi::soniq::timer<N>>{});
   }
}

extern "C"
{

#define TIMER_INTERRUPT_HANDLER(N, NAME)                                       \
   void NAME()                                                                 \
   {                                                                           \
      using cycfi::soniq::detail::handle_timer_interrupt;                   \
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

#define HANDLE_ADC_INTERRUPT(ID, STREAM)                                       \
   {                                                                           \
      if (LL_DMA_IsActiveFlag_TC##STREAM(DMA2) == 1)                           \
      {                                                                        \
         LL_DMA_ClearFlag_TC##STREAM(DMA2);                                    \
         ::config(cycfi::soniq::adc_conversion_complete<ID>{});             \
      }                                                                        \
                                                                               \
      if (LL_DMA_IsActiveFlag_HT##STREAM(DMA2) == 1)                           \
      {                                                                        \
         LL_DMA_ClearFlag_HT##STREAM(DMA2);                                    \
         ::config(cycfi::soniq::adc_conversion_half_complete<ID>{});        \
      }                                                                        \
                                                                               \
      if (LL_DMA_IsActiveFlag_TE##STREAM(DMA2) == 1)                           \
      {                                                                        \
         LL_DMA_ClearFlag_TE##STREAM(DMA2);                                    \
         cycfi::soniq::error_handler();                                     \
      }                                                                        \
   }                                                                           \
   /***/

   // $$$ Clean me up $$$
   // void DMA2_Stream0_IRQHandler()
   // {
   //    if (LL_DMA_IsActiveFlag_TC0(DMA2) == 1)
   //    {
   //       LL_DMA_ClearFlag_TC0(DMA2);

   //       // Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes
   //       // SCB_InvalidateDCache_by_Addr((uint32_t*) buff + (size/2), size);

   //       ::config(cycfi::soniq::adc_conversion_complete<1>{});
   //    }

   //    if (LL_DMA_IsActiveFlag_HT0(DMA2) == 1)
   //    {
   //       LL_DMA_ClearFlag_HT0(DMA2);

   //       ::config(cycfi::soniq::adc_conversion_half_complete<1>{});

   //       // Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes
   //       // SCB_InvalidateDCache_by_Addr((uint32_t*) buff, size);
   //    }

   //    if (LL_DMA_IsActiveFlag_TE0(DMA2) == 1)
   //    {
   //       LL_DMA_ClearFlag_TE0(DMA2);

   //       cycfi::soniq::error_handler();
   //    }
   // }


   void DMA2_Stream0_IRQHandler(void)
   {
      HANDLE_ADC_INTERRUPT(1, 0);
   }

   void DMA2_Stream1_IRQHandler(void)
   {
      HANDLE_ADC_INTERRUPT(3, 1);   // $$$ why not dma-stream == 2? $$$
   }

   void DMA2_Stream2_IRQHandler(void)
   {
      HANDLE_ADC_INTERRUPT(2, 2);   // $$$ why not dma-stream == 3? $$$
   }

   void ADC_IRQHandler(void)
   {
      // Check whether ADC group regular overrun caused the ADC interruption

      if (LL_ADC_IsActiveFlag_OVR(ADC1) != 0)
      {
         LL_ADC_ClearFlag_OVR(ADC1);
         cycfi::soniq::error_handler();
      }

      if (LL_ADC_IsActiveFlag_OVR(ADC2) != 0)
      {
         LL_ADC_ClearFlag_OVR(ADC2);
         cycfi::soniq::error_handler();
      }

      if (LL_ADC_IsActiveFlag_OVR(ADC3) != 0)
      {
         LL_ADC_ClearFlag_OVR(ADC3);
         cycfi::soniq::error_handler();
      }
   }

   void EXTI0_IRQHandler(void)
   {
      cycfi::soniq::detail::handle_exti<0>();
   }

   void EXTI1_IRQHandler(void)
   {
      cycfi::soniq::detail::handle_exti<1>();
   }

   void EXTI2_IRQHandler(void)
   {
      cycfi::soniq::detail::handle_exti<2>();
   }

   void EXTI3_IRQHandler(void)
   {
      cycfi::soniq::detail::handle_exti<3>();
   }

   void EXTI4_IRQHandler(void)
   {
      cycfi::soniq::detail::handle_exti<4>();
   }

   void EXTI9_5_IRQHandler(void)
   {
      cycfi::soniq::detail::handle_exti<5>();
      cycfi::soniq::detail::handle_exti<6>();
      cycfi::soniq::detail::handle_exti<7>();
      cycfi::soniq::detail::handle_exti<8>();
      cycfi::soniq::detail::handle_exti<9>();
      cycfi::soniq::detail::handle_exti<10>();
   }

   void EXTI15_10_IRQHandler(void)
   {
      cycfi::soniq::detail::handle_exti<10>();
      cycfi::soniq::detail::handle_exti<11>();
      cycfi::soniq::detail::handle_exti<12>();
      cycfi::soniq::detail::handle_exti<13>();
      cycfi::soniq::detail::handle_exti<14>();
      cycfi::soniq::detail::handle_exti<15>();
   }
}

#endif
