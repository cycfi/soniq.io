/*=============================================================================
   Copyright (c) 2015-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_ADC_IMPL_HPP_MAY_1_2017)
#define CYCFI_INFINITY_ADC_IMPL_HPP_MAY_1_2017

///////////////////////////////////////////////////////////////////////////////
// This implementation is for the STM32F4 series.
///////////////////////////////////////////////////////////////////////////////

#include <stm32f4xx_ll_cortex.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_adc.h>
#include <stm32f4xx_ll_dma.h>
#include <stm32f4xx_ll_gpio.h>

#include <inf/pin.hpp>
#include <inf/device.hpp>
#include <inf/timer.hpp>
#include <type_traits>

namespace cycfi { namespace infinity { namespace detail
{
   constexpr std::size_t adc_resolution = 4096;

   // Check if id is a valid adc.
   constexpr bool valid_adc(std::size_t id)
   {
      return id >= 1 && id <= 3;
   }

   // Check if id is a valid timer for the adc.
   constexpr bool valid_adc_timer(std::size_t id)
   {
      return id == 2 || id == 3 || id == 8;
   }

   // Check if channel is a valid adc channel.
   constexpr bool valid_adc_channel(std::size_t adc_id, std::size_t channel)
   {
      // $$$ Update me $$$
      return channel >= 0 && channel <= 15;
   }

   template <std::size_t channel>
   std::size_t get_adc_pin(std::size_t adc_id);

   template <>
   constexpr std::size_t get_adc_pin<0>(std::size_t adc_id)
   {
      return port::porta + 0;
   }

   template <>
   constexpr std::size_t get_adc_pin<1>(std::size_t adc_id)
   {
      return port::porta + 1;
   }

   template <>
   constexpr std::size_t get_adc_pin<2>(std::size_t adc_id)
   {
      return port::porta + 2;
   }

   template <>
   constexpr std::size_t get_adc_pin<3>(std::size_t adc_id)
   {
      return port::porta + 3;
   }

   template <>
   constexpr std::size_t get_adc_pin<4>(std::size_t adc_id)
   {
      return (adc_id == 3)? port::portf + 6 : port::porta + 4;
   }

   template <>
   constexpr std::size_t get_adc_pin<5>(std::size_t adc_id)
   {
      return (adc_id == 3)? port::portf + 7 : port::porta + 5;
   }

   template <>
   constexpr std::size_t get_adc_pin<6>(std::size_t adc_id)
   {
      return (adc_id == 3)? port::portf + 8 : port::porta + 6;
   }

   template <>
   constexpr std::size_t get_adc_pin<7>(std::size_t adc_id)
   {
      return (adc_id == 3)? port::portf + 9 : port::porta + 7;
   }

   template <>
   constexpr std::size_t get_adc_pin<8>(std::size_t adc_id)
   {
      return (adc_id == 3)? port::portf + 10 : port::portb + 0;
   }

   template <>
   constexpr std::size_t get_adc_pin<9>(std::size_t adc_id)
   {
      return (adc_id == 3)? port::portf + 3 : port::portb + 1;
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
   constexpr std::size_t get_adc_pin<12>(std::size_t adc_id)
   {
      return port::portc + 2;
   }

   template <>
   constexpr std::size_t get_adc_pin<13>(std::size_t adc_id)
   {
      return port::portc + 3;
   }

   template <>
   constexpr std::size_t get_adc_pin<14>(std::size_t adc_id)
   {
      return (adc_id == 3)? port::portf + 4 : port::portc + 4;
   }

   template <>
   constexpr std::size_t get_adc_pin<15>(std::size_t adc_id)
   {
      return (adc_id == 3)? port::portf + 5 : port::portc + 5;
   }

   ////////////////////////////////////////////////////////////////////////////
   // The ADCs: We provide template functions for getting the memory mapped
   // adcs given a constant id. That way, we can use generic programming.
   ////////////////////////////////////////////////////////////////////////////
   template <std::size_t id>
   struct adc_info;

#define INFINITY_ADC(id, stream, channel)                                      \
   template <>                                                                 \
   struct adc_info<id>                                                         \
   {                                                                           \
      static constexpr intptr_t adc = ADC##id##_BASE;                          \
      static constexpr uint32_t periph_id = LL_APB2_GRP1_PERIPH_ADC##id;       \
      static constexpr IRQn_Type dma_irq_id = DMA2_Stream##stream##_IRQn;      \
      static constexpr uint32_t dma_stream = LL_DMA_STREAM_##stream;           \
      static constexpr uint32_t dma_channel = LL_DMA_CHANNEL_##channel;        \
   };                                                                          \
   /***/

   template <std::size_t id>
   constexpr ADC_TypeDef* get_adc()
   {
      return reinterpret_cast<ADC_TypeDef*>(adc_info<id>::adc);
   }

   template <std::size_t id>
   uint32_t adc_timer_trigger_id();

   template <std::size_t id>
   uint32_t adc_rank();

   template <std::size_t id>
   uint32_t adc_channel();

#define INFINITY_ADC_TIMER_TRIGGER(id)                                         \
   template <>                                                                 \
   inline uint32_t adc_timer_trigger_id<id>()                                  \
   {                                                                           \
      return LL_ADC_REG_TRIG_EXT_TIM##id##_TRGO;                               \
   }                                                                           \
   /***/

#define INFINITY_ADC_RANK(id)                                                  \
   template <>                                                                 \
   inline uint32_t adc_rank<id>()                                              \
   {                                                                           \
      return LL_ADC_REG_RANK_##id;                                             \
   }                                                                           \
   /***/

#define INFINITY_ADC_CHANNEL(id)                                               \
   template <>                                                                 \
   inline uint32_t adc_channel<id>()                                           \
   {                                                                           \
      return LL_ADC_CHANNEL_##id;                                              \
   }                                                                           \
   /***/

   INFINITY_ADC(1, 0, 0)
   INFINITY_ADC(2, 2, 1)
   INFINITY_ADC(3, 1, 2)

   INFINITY_ADC_TIMER_TRIGGER(2)
   INFINITY_ADC_TIMER_TRIGGER(3)
   INFINITY_ADC_TIMER_TRIGGER(8)

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

   void adc_dma_config(
      ADC_TypeDef* adc_n,
      uint32_t dma_stream,
      uint32_t dma_channel,
      IRQn_Type dma_channel_irq,
      uint16_t values[],
      uint16_t size
   );

   void adc_config(
      ADC_TypeDef* adc,
      uint32_t timer_trigger_id,
      uint32_t adc_periph_id,
      uint32_t num_channels
   );

   inline void activate_adc(ADC_TypeDef* adc)
   {
      // ADC must be disabled at this point
      if (LL_ADC_IsEnabled(adc) != 0)
         error_handler();

      // Enable ADC
      LL_ADC_Enable(adc);
   }

   template <std::size_t adc_id, std::size_t timer_id, std::size_t channels>
   inline void init_adc(uint16_t values[], uint16_t size)
   {
      adc_dma_config(
         get_adc<adc_id>(),
         adc_info<adc_id>::dma_stream,
         adc_info<adc_id>::dma_channel,
         adc_info<adc_id>::dma_irq_id,
         values, size
      );

      adc_config(
         get_adc<adc_id>(),
         adc_timer_trigger_id<timer_id>(),
         adc_info<adc_id>::periph_id,
         channels
      );

      activate_adc(get_adc<adc_id>());

      // Set timer the trigger output (TRGO)
      LL_TIM_SetTriggerOutput(&get_timer<timer_id>(), LL_TIM_TRGO_UPDATE);
   }

   template <std::size_t adc_id, std::size_t channel_, std::size_t rank_>
   inline void enable_adc_channel()
   {
      auto adc = get_adc<adc_id>();
      auto channel = adc_channel<channel_>();
      auto rank = adc_rank<rank_>();

      // Set ADC group regular sequence: channel on the selected sequence rank.
      LL_ADC_REG_SetSequencerRanks(adc, rank, channel);
      LL_ADC_SetChannelSamplingTime(adc, channel, LL_ADC_SAMPLINGTIME_3CYCLES);
   }

   template <std::size_t adc_id>
   inline void start_adc()
   {
      auto adc = get_adc<adc_id>();
      if (LL_ADC_IsEnabled(adc))
      {
         LL_ADC_REG_StartConversionExtTrig(adc, LL_ADC_REG_TRIG_EXT_RISING);
      }
   }

   template <std::size_t adc_id>
   inline void stop_adc()
   {
      auto adc = get_adc<adc_id>();
      if (LL_ADC_IsEnabled(adc))
      {
         LL_ADC_REG_StopConversionExtTrig(adc);
      }
   }

}}}

#endif
