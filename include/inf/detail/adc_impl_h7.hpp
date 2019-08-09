/*=============================================================================
   Copyright (c) 2015-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_ADC_IMPL_HPP_MAY_1_2017)
#define CYCFI_INFINITY_ADC_IMPL_HPP_MAY_1_2017

///////////////////////////////////////////////////////////////////////////////
// This implementation is for the STM32H7 series.
///////////////////////////////////////////////////////////////////////////////

#include <inf/timer.hpp>
#include "stm32h7xx_hal.h"
#include <stm32h7xx_ll_adc.h>

namespace cycfi { namespace infinity { namespace detail
{
   constexpr std::size_t adc_resolution = 65536;

   // Check if id is a valid adc.
   constexpr bool valid_adc(std::size_t id)
   {
      return id >= 1 && id <= 3;
   }

   // Check if id is a valid timer for the adc.
   constexpr bool valid_adc_timer(std::size_t id) // $$$ fixme $$$
   {
      return
         id == 1 || id == 2 || id == 3 || id == 4 ||
         id == 6 || id == 8 || id == 15
         ;
   }

   // Check if channel is a valid adc channel.
   constexpr bool valid_adc_channel(std::size_t adc_id, std::size_t channel)
   {
      switch (adc_id)
      {
         case 1:
            return
               (channel >= 2 && channel <= 11) ||
               (channel >= 14 && channel <= 19)
               ;
         case 2:
            return
               (channel >= 2 && channel <= 11) ||
               channel >= 14 || channel >= 15 ||
               channel >= 18 || channel >= 19
               ;
         case 3:
            return channel >= 0 && channel <= 11;
      }
      return false;
   }

   template <std::size_t channel>
   std::size_t get_adc_pin(std::size_t adc_id);

   template <>
   constexpr std::size_t get_adc_pin<0>(std::size_t adc_id)
   {
      return port::portc + 2;
   }

   template <>
   constexpr std::size_t get_adc_pin<1>(std::size_t adc_id)
   {
      return port::portc + 3;
   }

   template <>
   constexpr std::size_t get_adc_pin<2>(std::size_t adc_id)
   {
      switch (adc_id)
      {
         case 1:     return port::portf + 11;
         case 2:     return port::portf + 13;
         case 3:     return port::portf + 9;
      }
      return -1;
   }

   template <>
   constexpr std::size_t get_adc_pin<3>(std::size_t adc_id)
   {
      switch (adc_id)
      {
         case 1:     return port::porta + 6;
         case 2:     return port::porta + 6;
         case 3:     return port::portf + 7;
      }
      return -1;
   }

   template <>
   constexpr std::size_t get_adc_pin<4>(std::size_t adc_id)
   {
      switch (adc_id)
      {
         case 1:     return port::portc + 4;
         case 2:     return port::portc + 4;
         case 3:     return port::portf + 5;
      }
      return -1;
   }

   template <>
   constexpr std::size_t get_adc_pin<5>(std::size_t adc_id)
   {
      switch (adc_id)
      {
         case 1:     return port::portb + 1;
         case 2:     return port::portb + 1;
         case 3:     return port::portf + 3;
      }
      return -1;
   }

   template <>
   constexpr std::size_t get_adc_pin<6>(std::size_t adc_id)
   {
      switch (adc_id)
      {
         case 1:     return port::portf + 12;
         case 2:     return port::portf + 14;
         case 3:     return port::portf + 10;
      }
      return -1;
   }

   template <>
   constexpr std::size_t get_adc_pin<7>(std::size_t adc_id)
   {
      switch (adc_id)
      {
         case 1:     return port::porta + 7;
         case 2:     return port::porta + 7;
         case 3:     return port::portf + 8;
      }
      return -1;
   }

   template <>
   constexpr std::size_t get_adc_pin<8>(std::size_t adc_id)
   {
      switch (adc_id)
      {
         case 1:     return port::portc + 5;
         case 2:     return port::portc + 5;
         case 3:     return port::portf + 6;
      }
      return -1;
   }

   template <>
   constexpr std::size_t get_adc_pin<9>(std::size_t adc_id)
   {
      switch (adc_id)
      {
         case 1:     return port::portb + 0;
         case 2:     return port::portb + 0;
         case 3:     return port::portf + 4;
      }
      return -1;
   }

   template <>
   constexpr std::size_t get_adc_pin<10>(std::size_t adc_id)
   {
      return port::portc + 0;
   }

   template <>
   constexpr std::size_t get_adc_pin<11>(std::size_t adc_id)
   {
      return port::portc + 1;
   }

   template <>
   constexpr std::size_t get_adc_pin<14>(std::size_t adc_id)
   {
      return port::porta + 2;
   }

   template <>
   constexpr std::size_t get_adc_pin<15>(std::size_t adc_id)
   {
      return port::porta + 3;
   }

   template <>
   constexpr std::size_t get_adc_pin<16>(std::size_t adc_id)
   {
      return port::porta + 0;
   }

   template <>
   constexpr std::size_t get_adc_pin<17>(std::size_t adc_id)
   {
      return port::porta + 1;
   }

   template <>
   constexpr std::size_t get_adc_pin<18>(std::size_t adc_id)
   {
      return port::porta + 4;
   }

   template <>
   constexpr std::size_t get_adc_pin<19>(std::size_t adc_id)
   {
      return port::porta + 5;
   }

///////////////////////////////////////////////////////////////////////////////
   template <std::size_t id>
   uint32_t adc_rank();

#define INFINITY_ADC_RANK(id)                                                  \
   template <>                                                                 \
   inline uint32_t adc_rank<id>()                                              \
   {                                                                           \
      return ADC_REGULAR_RANK_##id;                                            \
   }                                                                           \
   /***/

   INFINITY_ADC_RANK(1)
   INFINITY_ADC_RANK(2)
   INFINITY_ADC_RANK(3)
   INFINITY_ADC_RANK(4)
   INFINITY_ADC_RANK(5)
   INFINITY_ADC_RANK(6)
   INFINITY_ADC_RANK(7)
   INFINITY_ADC_RANK(8)
   INFINITY_ADC_RANK(9)
   INFINITY_ADC_RANK(10)
   INFINITY_ADC_RANK(11)
   INFINITY_ADC_RANK(12)
   INFINITY_ADC_RANK(13)
   INFINITY_ADC_RANK(14)
   INFINITY_ADC_RANK(15)
   INFINITY_ADC_RANK(16)

///////////////////////////////////////////////////////////////////////////////
   template <std::size_t id>
   uint32_t adc_channel();

#define INFINITY_ADC_CHANNEL(id)                                               \
   template <>                                                                 \
   inline uint32_t adc_channel<id>()                                           \
   {                                                                           \
      return ADC_CHANNEL_##id;                                                 \
   }                                                                           \
   /***/

   INFINITY_ADC_CHANNEL(0)
   INFINITY_ADC_CHANNEL(1)
   INFINITY_ADC_CHANNEL(2)
   INFINITY_ADC_CHANNEL(3)
   INFINITY_ADC_CHANNEL(4)
   INFINITY_ADC_CHANNEL(5)
   INFINITY_ADC_CHANNEL(6)
   INFINITY_ADC_CHANNEL(7)
   INFINITY_ADC_CHANNEL(8)
   INFINITY_ADC_CHANNEL(9)
   INFINITY_ADC_CHANNEL(10)
   INFINITY_ADC_CHANNEL(11)
   INFINITY_ADC_CHANNEL(12)
   INFINITY_ADC_CHANNEL(13)
   INFINITY_ADC_CHANNEL(14)
   INFINITY_ADC_CHANNEL(15)
   INFINITY_ADC_CHANNEL(16)
   INFINITY_ADC_CHANNEL(17)
   INFINITY_ADC_CHANNEL(18)
   INFINITY_ADC_CHANNEL(19)

   ///////////////////////////////////////////////////////////////////////////////
   template <std::size_t id>
   constexpr ADC_TypeDef* get_adc()
   {
      switch (id)
      {
         case 1: return ADC1;
         case 2: return ADC2;
         case 3: return ADC3;
      }
      return nullptr;
   }

   ///////////////////////////////////////////////////////////////////////////////

   // main channel enabler
   void enable_adc_channel(std::size_t id, uint32_t channel_id, uint32_t rank);

   template <std::size_t adc_id, std::size_t channel_, std::size_t rank_>
   inline void enable_adc_channel()
   {
      auto adc = get_adc<adc_id>();
      auto channel = adc_channel<channel_>();
      auto rank = adc_rank<rank_>();

      enable_adc_channel(adc_id, channel, rank);
   }

   void start_dma(std::size_t adc_id, uint16_t values[], uint16_t size);

   template <std::size_t adc_id>
   inline void start_adc(uint16_t values[], uint16_t size)
   {
      start_dma(adc_id, values, size);
      auto adc = get_adc<adc_id>();
      if (LL_ADC_IsEnabled(adc))
         LL_ADC_REG_StartConversion(adc);
   }

   template <std::size_t adc_id>
   inline void stop_adc()
   {
      auto adc = get_adc<adc_id>();
      if (LL_ADC_IsEnabled(adc))
         LL_ADC_REG_StopConversion(adc);
   }

   // Main initialization
   void init_adc(std::size_t id);

   template <std::size_t adc_id, std::size_t timer_id, std::size_t channels>
   inline void init_adc()
   {
      init_adc(adc_id);

      // Set timer the trigger output (TRGO)
      LL_TIM_SetTriggerOutput(&get_timer<timer_id>(), LL_TIM_TRGO_UPDATE);

      stop_adc<adc_id>();
   }
}}}

#endif
