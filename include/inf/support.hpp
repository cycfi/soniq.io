/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_SUPPORT_HPP_DECEMBER_20_2015)
#define CYCFI_INFINITY_SUPPORT_HPP_DECEMBER_20_2015

#include <type_traits>
#include <cstdint>
#include <cstring>
#include <cmsis_os2.h>

#if defined(STM32H743xx)
# include <system_stm32h7xx.h>
# include <stm32h7xx_ll_utils.h>
# include <stm32h7xx_hal.h>
#elif defined(STM32F446xx)
# include <system_stm32f4xx.h>
# include <stm32f4xx_ll_utils.h>
# include <stm32f4xx_hal.h>
#endif

namespace cycfi { namespace soniq
{
   ////////////////////////////////////////////////////////////////////////////
   // Basic metaprogramming utils
   ////////////////////////////////////////////////////////////////////////////
   template <bool b>
   using bool_ = std::integral_constant<bool, b>;

   template <int i>
   using int_ = std::integral_constant<int, i>;

   template <std::size_t i>
   using uint_ = std::integral_constant<std::size_t, i>;

   template <int8_t i>
   using int8_ = std::integral_constant<std::int8_t, i>;

   template <uint8_t i>
   using uint8_ = std::integral_constant<std::uint8_t, i>;

   template <int16_t i>
   using int16_ = std::integral_constant<std::int16_t, i>;

   template <uint16_t i>
   using uint16_ = std::integral_constant<std::uint16_t, i>;

   template <int32_t i>
   using int32_ = std::integral_constant<std::int32_t, i>;

   template <uint32_t i>
   using uint32_ = std::integral_constant<std::uint32_t, i>;

   template <int64_t i>
   using int64_ = std::integral_constant<std::int64_t, i>;

   template <uint64_t i>
   using uint64_ = std::integral_constant<std::uint64_t, i>;

   template <typename T>
   struct identity
   {
      using type = T;
   };

   ////////////////////////////////////////////////////////////////////////////
   // The MCU clock speed
   ////////////////////////////////////////////////////////////////////////////
   uint32_t const clock_speed = SystemCoreClock;

   ////////////////////////////////////////////////////////////////////////////
   // delay_ms function
	////////////////////////////////////////////////////////////////////////////
   inline void delay_ms(uint32_t ms)
   {
      osDelay(ms);
   }

   ////////////////////////////////////////////////////////////////////////////
   // Returns the current system tick, configured as milliseconds.
   ////////////////////////////////////////////////////////////////////////////
   inline auto millis()
   {
	   return HAL_GetTick();
   }

   ////////////////////////////////////////////////////////////////////////////
   // debounce
   ////////////////////////////////////////////////////////////////////////////
   inline bool debounce(uint32_t& time, uint32_t delay)
   {
      auto now = millis();
      auto elapsed = now - time;
      time = now;
      if (elapsed < delay)
         return false;
      return true;
   }

   template <uint32_t delay = 200 /* ms */>
   struct debouncer
   {
      debouncer()
       : _t(millis())
      {}

      bool operator()()
      {
         return debounce(_t, delay);
      }
      uint32_t _t;
   };

   ////////////////////////////////////////////////////////////////////////////
   // Quick to_string implementation.
   //
   //    n: integer to convert, scaled it by frac if you need
   //       fractional digits.
   //    s: the output string. should hold enough chars to represent
   //       the digits.
   //    frac: number of decimal places.
   //
   //    For example, if n: 1234 and frac: 2, the result is 12.34
   ////////////////////////////////////////////////////////////////////////////
   inline void reverse(char s[])
   {
      int i, j;
      for (i = 0, j = std::strlen(s)-1; i<j; i++, j--)
      {
         char c = s[i];
         s[i] = s[j];
         s[j] = c;
      }
   }

   inline void to_string(int n, char s[], int frac = 0)
   {
      int i, sign;

      if ((sign = n) < 0)                 // record sign
         n = -n;                          // make n positive
      i = 0;
      while (true)                        // generate digits in reverse order
      {
         if (frac && i == frac)
            s[i++] = '.';
         s[i++] = n % 10 + '0';           // get next digit

         if ((n /= 10) == 0)
         {
            if (i <= frac)
            {
               while (i != frac)
                  s[i++] = '0';
               s[i++] = '.';
               s[i++] = '0';
            }
            break;
         }
      }

      if (sign < 0)
         s[i++] = '-';
      s[i] = '\0';
      reverse(s);
   }
}}

#endif
