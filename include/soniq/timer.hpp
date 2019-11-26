/*=============================================================================
   Copyright (c) 2015-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_TIMER_HPP_DECEMBER_21_2015)
#define CYCFI_INFINITY_TIMER_HPP_DECEMBER_21_2015

#include <soniq/pin.hpp>
#include <soniq/support.hpp>
#include <soniq/detail/timer_impl.hpp>
#include <soniq/device.hpp>

namespace cycfi::soniq
{
   ////////////////////////////////////////////////////////////////////////////
   // timer
   ////////////////////////////////////////////////////////////////////////////
   template <uint32_t id_>
   struct timer
   {
      using self_type = timer<id_>;
      static constexpr std::size_t id = id_;
      static_assert(detail::check_valid_timer(id), "Invalid Timer id");

      timer() = default;
      timer(timer const&) = delete;
      timer& operator=(timer const&) = delete;

      timer(uint32_t clock_frequency, uint32_t frequency)
      {
         detail::init_timer<id>(clock_frequency, frequency);
      }

      void enable_interrupt(std::size_t priority = 0)
      {
         detail::enable_timer_interrupt<id>(priority);
      }

      void start()
      {
         detail::start_timer<id>();
      }

      void stop()
      {
         detail::start_timer<id>();
      }

      template <typename F>
      auto on_trigger(F f, std::size_t priority = 0)
      {
         extern std::array<std::function<void()>, 7> _timer_interrupt_handlers;
         enable_interrupt(priority);
         _timer_interrupt_handlers[id] = f;
      }
   };
}

#endif
