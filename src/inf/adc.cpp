/*=============================================================================
   Copyright (c) 2015-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/detail/adc_impl.hpp>

namespace cycfi { namespace infinity { namespace detail
{
   auto const dma = DMA2;

   void adc_dma_config(
      ADC_TypeDef* adc_n,
      uint32_t dma_stream,
      uint32_t dma_channel,
      IRQn_Type dma_channel_irq,
      uint16_t values[],
      uint16_t size
   )
   {
      // Configuration of NVIC
      // Configure NVIC to enable DMA interruptions
      NVIC_SetPriority(dma_channel_irq, 1);  // DMA IRQ lower priority than ADC IRQ
      NVIC_EnableIRQ(dma_channel_irq);

      // Configuration of DMA
      // Enable the peripheral clock of DMA
      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

      LL_DMA_SetChannelSelection(dma, dma_stream, dma_channel);

      // Configure the DMA transfer
      //    - DMA transfer in circular mode to match with ADC configuration:
      //      DMA unlimited requests.
      //    - DMA transfer from ADC without address increment.
      //    - DMA transfer to memory with address increment.
      //    - DMA transfer from ADC by half-word to match with ADC configuration:
      //      ADC resolution 12 bits.
      //    - DMA transfer to memory by half-word to match with ADC conversion data
      //      buffer variable type: half-word.

      LL_DMA_ConfigTransfer(
         dma,
         dma_stream,
         LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
         LL_DMA_MODE_CIRCULAR              |
         LL_DMA_PERIPH_NOINCREMENT         |
         LL_DMA_MEMORY_INCREMENT           |
         LL_DMA_PDATAALIGN_HALFWORD        |
         LL_DMA_MDATAALIGN_HALFWORD        |
         LL_DMA_PRIORITY_HIGH
      );

      // Set DMA transfer addresses of source and destination
      LL_DMA_ConfigAddresses(
         dma,
         dma_stream,
         LL_ADC_DMA_GetRegAddr(adc_n, LL_ADC_DMA_REG_REGULAR_DATA),
         reinterpret_cast<uint32_t>(values),
         LL_DMA_DIRECTION_PERIPH_TO_MEMORY
      );

      // Set DMA transfer size
      LL_DMA_SetDataLength(dma, dma_stream, size);

      // Enable DMA transfer interruption: transfer complete
      LL_DMA_EnableIT_TC(dma, dma_stream);

      // Enable DMA transfer interruption: half transfer
      LL_DMA_EnableIT_HT(dma, dma_stream);

      // Enable DMA transfer interruption: transfer error
      LL_DMA_EnableIT_TE(dma, dma_stream);

      // Activation of DMA
      // Enable the DMA transfer
      LL_DMA_EnableStream(dma, dma_stream);
   }

   namespace
   {
      uint32_t seq_length(uint32_t num_channels)
      {
         switch (num_channels)
         {
            case 2: return LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS;
            case 3: return LL_ADC_REG_SEQ_SCAN_ENABLE_3RANKS;
            case 4: return LL_ADC_REG_SEQ_SCAN_ENABLE_4RANKS;
            case 5: return LL_ADC_REG_SEQ_SCAN_ENABLE_5RANKS;
            case 6: return LL_ADC_REG_SEQ_SCAN_ENABLE_6RANKS;
            case 7: return LL_ADC_REG_SEQ_SCAN_ENABLE_7RANKS;
            case 8: return LL_ADC_REG_SEQ_SCAN_ENABLE_8RANKS;
            case 9: return LL_ADC_REG_SEQ_SCAN_ENABLE_9RANKS;
            case 10: return LL_ADC_REG_SEQ_SCAN_ENABLE_10RANKS;
            case 11: return LL_ADC_REG_SEQ_SCAN_ENABLE_11RANKS;
            case 12: return LL_ADC_REG_SEQ_SCAN_ENABLE_12RANKS;
            case 13: return LL_ADC_REG_SEQ_SCAN_ENABLE_13RANKS;
            case 14: return LL_ADC_REG_SEQ_SCAN_ENABLE_14RANKS;
            case 15: return LL_ADC_REG_SEQ_SCAN_ENABLE_15RANKS;
            default: return LL_ADC_REG_SEQ_SCAN_DISABLE;
         }
      }
   }

   void adc_config(
      ADC_TypeDef* adc,
      uint32_t timer_trigger_id,
      uint32_t adc_periph_id,
      uint32_t num_channels
   )
   {
      // Configuration of NVIC
      // Configure NVIC to enable ADC interruptions
      NVIC_SetPriority(ADC_IRQn, 0); // ADC IRQ greater priority than DMA IRQ
      NVIC_EnableIRQ(ADC_IRQn);

      // Enable ADC clock (core clock)
      LL_APB2_GRP1_EnableClock(adc_periph_id);

      static bool adc_common_initialized = false;

      if (!adc_common_initialized)
      {
         // ADC common instance must not be enabled at this point
         if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() != 0)
            error_handler();

         // Set ADC clock (conversion clock) common to several ADC instances
         LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(adc), LL_ADC_CLOCK_SYNC_PCLK_DIV2);

         adc_common_initialized = true;
      }

      // ADC must not be enabled at this point
      if (LL_ADC_IsEnabled(adc) != 0)
         error_handler();

      // Set ADC data resolution
      LL_ADC_SetResolution(adc, LL_ADC_RESOLUTION_12B);

      // Set ADC conversion data alignment
      LL_ADC_SetResolution(adc, LL_ADC_DATA_ALIGN_RIGHT);

      // Set Set ADC sequencers scan mode, for all ADC groups
      // (group regular, group injected).
      LL_ADC_SetSequencersScanMode(adc, LL_ADC_SEQ_SCAN_ENABLE);

      // Set ADC group regular trigger source
      LL_ADC_REG_SetTriggerSource(adc, timer_trigger_id);

      // Set ADC group regular trigger polarity
      // LL_ADC_REG_SetTriggerEdge(adc, LL_ADC_REG_TRIG_EXT_RISING);

      // Set ADC group regular continuous mode
      LL_ADC_REG_SetContinuousMode(adc, LL_ADC_REG_CONV_SINGLE);

      // Set ADC group regular conversion data transfer
      LL_ADC_REG_SetDMATransfer(adc, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);

      // Set ADC group regular sequencer length and scan direction
      LL_ADC_REG_SetSequencerLength(adc, seq_length(num_channels));

      // Set ADC group regular sequencer discontinuous mode
      // LL_ADC_REG_SetSequencerDiscont(adc, LL_ADC_REG_SEQ_DISCONT_DISABLE);

      // Enable interruption ADC group regular overrun
      LL_ADC_EnableIT_OVR(adc);
   }
}}}
