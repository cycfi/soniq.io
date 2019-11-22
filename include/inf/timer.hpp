/*=============================================================================
   Copyright (c) 2015-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_TIMER_HPP_DECEMBER_21_2015)
#define CYCFI_INFINITY_TIMER_HPP_DECEMBER_21_2015

#include <inf/pin.hpp>
#include <inf/support.hpp>
#include <inf/config.hpp>
#include <inf/detail/timer_impl.hpp>
#include <inf/device.hpp>

#if defined(STM32H7)
# include <stm32h7xx_ll_tim.h>
#elif defined(STM32F4)
# include <stm32f4xx_ll_tim.h>
#endif

namespace cycfi { namespace soniq
{
   ////////////////////////////////////////////////////////////////////////////
   // timer
   ////////////////////////////////////////////////////////////////////////////
   template <std::size_t id_>
   struct timer
   {
      using self_type = timer<id_>;
      static constexpr std::size_t id = id_;
      static_assert(detail::check_valid_timer(id), "Invalid Timer id");

      timer() = default;
      timer(timer const&) = delete;
      timer& operator=(timer const&) = delete;

      void init(uint32_t clock_frequency, uint32_t frequency)
      {
         // system_init(); // $$$ JDG $$$

         // Enable the timer peripheral clock
         periph_enable();

         // Set the pre-scaler value
         uint32_t timer_clock = SystemCoreClock / detail::timer_clock_div<id>();
         LL_TIM_SetPrescaler(get_timer(),
            __LL_TIM_CALC_PSC(timer_clock, clock_frequency));

         // Set the auto-reload value to have an initial update event frequency
         auto autoreload = __LL_TIM_CALC_ARR(
            timer_clock, LL_TIM_GetPrescaler(get_timer()), frequency);

         LL_TIM_SetAutoReload(get_timer(), autoreload);
      }

      void enable_interrupt(std::size_t priority = 0)
      {
         static_assert(detail::timer_irqn<id>() != -1,
            "Timer has no interrupt capability.");

         // Enable the update interrupt
         LL_TIM_EnableIT_UPDATE(get_timer());

         // Configure the NVIC to handle timer update interrupt
         NVIC_SetPriority(get_irqn(), priority);
         NVIC_EnableIRQ(get_irqn());
      }

      void start()
      {
         // Enable counter
         LL_TIM_EnableCounter(get_timer());

         // Force update generation
         LL_TIM_GenerateEvent_UPDATE(get_timer());
      }

      void stop()
      {
         LL_TIM_DisableCounter(get_timer());
      }

      auto setup(
         uint32_t clock_frequency, uint32_t frequency,
         std::size_t priority = 0)
      {
         init(clock_frequency, frequency);
         enable_interrupt();
         return [](auto base)
         {
            return make_basic_config<self_type>(base);
         };
      }

      template <typename F>
      auto setup(
         uint32_t clock_frequency, uint32_t frequency,
         F task, std::size_t priority = 0)
      {
         init(clock_frequency, frequency);
         enable_interrupt();
         return [task](auto base)
         {
            return make_task_config<self_type>(base, task);
         };
      }

   private:

      static constexpr TIM_TypeDef* get_timer()
      {
         return &detail::get_timer<id>();
      }

      static constexpr IRQn_Type get_irqn()
      {
         return IRQn_Type(detail::timer_irqn<id>());
      }

      static void periph_enable()
      {
         detail::timer_periph_enable<id>();
      }
   };
}}

#endif
