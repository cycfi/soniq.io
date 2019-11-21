/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_PIN_HPP_DECEMBER_20_2015)
#define CYCFI_INFINITY_PIN_HPP_DECEMBER_20_2015

#include <inf/support.hpp>

#if defined(STM32H7)
# include <inf/detail/pin_impl_h7.hpp>
#endif

namespace cycfi { namespace infinity
{
   ////////////////////////////////////////////////////////////////////////////
   // Constants
   ////////////////////////////////////////////////////////////////////////////

   namespace port
   {
      constexpr uint16_t porta = 0 * 16;
      constexpr uint16_t portb = 1 * 16;
      constexpr uint16_t portc = 2 * 16;
      constexpr uint16_t portd = 3 * 16;
      constexpr uint16_t porte = 4 * 16;
      constexpr uint16_t portf = 5 * 16;
      constexpr uint16_t portg = 6 * 16;
      constexpr uint16_t porth = 7 * 16;
      constexpr uint16_t porti = 8 * 16;
      constexpr uint16_t portj = 9 * 16;
      constexpr uint16_t portk = 10 * 16;
   }

   enum class port_output_speed
   {
      low_speed = LL_GPIO_SPEED_FREQ_LOW,             // 2MHz low speed
      mid_speed = LL_GPIO_SPEED_FREQ_MEDIUM,          // 12.5 MHz to 50 MHz medium speed
      high_speed = LL_GPIO_SPEED_FREQ_HIGH,           // 25 MHz to 100 MHz high speed
      very_high_speed = LL_GPIO_SPEED_FREQ_VERY_HIGH  // 50 MHz to 200 MHz very high speed
   };

   enum class port_output_type
   {
      push_pull = LL_GPIO_OUTPUT_PUSHPULL,
      open_drain = LL_GPIO_OUTPUT_OPENDRAIN
   };

   namespace port
   {
      auto constexpr low_speed = port_output_speed::low_speed;
      auto constexpr mid_speed = port_output_speed::mid_speed;
      auto constexpr high_speed = port_output_speed::high_speed;
      auto constexpr very_high_speed = port_output_speed::very_high_speed;

      auto constexpr push_pull = port_output_type::push_pull;
      auto constexpr open_drain = port_output_type::open_drain;
   }

   ////////////////////////////////////////////////////////////////////////////
   template <typename T>
   struct inverse_pin
   {
      operator bool() const
      {
         return pin.off();
      }

      T operator!() const
      {
         return pin;
      }

      T pin;
   };

   struct on_type
   {
      operator bool() const
      {
         return true;
      }

      bool operator!() const
      {
         return false;
      }
   };

   struct off_type
   {
      operator bool() const
      {
         return false;
      }

      bool operator!() const
      {
         return true;
      }
   };

   namespace port
   {
      constexpr on_type on = {};
      constexpr off_type off = {};
   }

   ////////////////////////////////////////////////////////////////////////////
   template <uint32_t ID>
   struct pin_descr
   {
      constexpr static uint16_t  id = ID;
      constexpr static uint16_t  bit = ID % 16;
      constexpr static uint16_t  port = ID / 16;
      constexpr static uint32_t  mask = 1 << bit;
   };

   ////////////////////////////////////////////////////////////////////////////
   template <uint32_t ID>
   struct output_pin
   {
      constexpr static uint16_t  id = ID;
      constexpr static uint16_t  bit = ID % 16;
      constexpr static uint16_t  port = ID / 16;
      constexpr static uint32_t  mask = 1 << bit;

      using self_type = output_pin;
      using inverse_type = inverse_pin<output_pin>;

      GPIO_TypeDef& gpio() const
      {
         return detail::get_port<port>();
      }

      volatile uint32_t& ref() const
      {
         return gpio().ODR;
      }

      bool state() const
      {
         return (ref() & mask) != 0;
      }

      inverse_type operator!() const
      {
         return { *this };
      }

      output_pin& operator=(bool val)
      {
         ref() ^= (-uint16_t(val) ^ ref()) & mask;
         return *this;
      }

      output_pin& operator=(self_type)
      {
         return *this;
      }

      output_pin& operator=(inverse_type)
      {
         ref() ^= mask;
         return *this;
      }

      output_pin& operator=(on_type)
      {
         ref() |= mask;
         return *this;
      }

      output_pin& operator=(off_type)
      {
         ref() &= ~mask;
         return *this;
      }
   };

   ////////////////////////////////////////////////////////////////////////////
   template <uint32_t ID>
   struct input_pin : pin_descr<ID>
   {
   };

   ////////////////////////////////////////////////////////////////////////////
   namespace port
   {
      template <
         uint32_t ID
       , port_output_speed speed
       , port_output_type type = port::push_pull
      >
      inline auto out()
      {
         using descr = pin_descr<ID>;
         detail::init_output_pin<
            uint32_t(descr::port)
          , uint32_t(descr::mask)
          , uint32_t(speed)
          , uint32_t(type)
         >();
         return output_pin<ID>{};
      }

      template <uint32_t ID, port_output_type type = port::push_pull>
      inline auto out()
      {
         return out<ID, port::high_speed, type>();
      }
   }
}}

#endif
