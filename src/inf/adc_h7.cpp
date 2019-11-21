/*=============================================================================
   Copyright (c) 2015-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

/*

#include <inf/support.hpp>
#include <inf/device.hpp>
#include <inf/detail/adc_impl_h7.hpp>

///////////////////////////////////////////////////////////////////////////////
// This implementation is for the STM32H7 series.
///////////////////////////////////////////////////////////////////////////////

namespace cycfi { namespace infinity { namespace detail
{
   ADC_HandleTypeDef adc_handles[3];
   DMA_HandleTypeDef dma_handles[3];

   ADC_TypeDef* get_adc(std::size_t id)
   {
      switch (id)
      {
         case 1: return ADC1;
         case 2: return ADC2;
         case 3: return ADC3;
         default: return nullptr;
      }
   }

   DMA_HandleTypeDef* get_hdma(ADC_HandleTypeDef* hadc, std::size_t& id)
   {
      if (hadc == &adc_handles[0])
      {
         id  = 1;
         return &dma_handles[0];
      }
      else if (hadc == &adc_handles[1])
      {
         id  = 2;
         return &dma_handles[1];
      }
      else if (hadc == &adc_handles[2])
      {
         id  = 3;
         return &dma_handles[2];
      }
      return nullptr;
   }

   void init_adc(std::size_t id)
   {
      if (id > 2)
         Error_Handler();

      auto *hadc = &adc_handles[id - 1];

      // Initialize ADC peripheral
      hadc->Instance = get_adc(id);
      if (HAL_ADC_DeInit(hadc) != HAL_OK)
         Error_Handler();  // ADC de-initialization Error

      hadc->Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;             // Asynchronous clock mode, input ADC clock divided
      hadc->Init.Resolution = ADC_RESOLUTION_16B;               // 16-bit resolution for converted data
      hadc->Init.ScanConvMode = ADC_SCAN_DISABLE;                 // Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1)
      hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;              // EOC flag picked-up to indicate conversion end
      hadc->Init.LowPowerAutoWait = DISABLE;                          // Auto-delayed conversion feature disabled
      hadc->Init.ContinuousConvMode = DISABLE;                          // Continuous mode enabled (automatic conversion restart after each conversion)
      hadc->Init.NbrOfConversion = 1;                                // Parameter discarded because sequencer is disabled
      hadc->Init.DiscontinuousConvMode = DISABLE;                          // Parameter discarded because sequencer is disabled

      // $$$ fixme $$$
      hadc->Init.ExternalTrigConv = ADC_EXTERNALTRIG_T2_TRGO;         // Software start to trig the 1st conversion manually, without external event
      hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;  // Parameter discarded because software trigger chosen
      hadc->Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;  // ADC DMA circular requested
      hadc->Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;         // DR register is overwritten with the last conversion result in case of overrun
      hadc->Init.OversamplingMode = DISABLE;                          // No oversampling

      // Initialize ADC peripheral according to the passed parameters
      if (HAL_ADC_Init(hadc) != HAL_OK)
         Error_Handler();

      // Start calibration
      if (HAL_ADCEx_Calibration_Start(hadc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
         Error_Handler();
   }

   void start_dma(std::size_t id, uint16_t values[], uint16_t size)
   {
      if (id > 2)
         Error_Handler();

      auto *hadc = &adc_handles[id - 1];
      if (HAL_ADC_Start_DMA(hadc, (uint32_t*)values, size) != HAL_OK)
         Error_Handler();
   }

   void enable_adc_channel(std::size_t id, uint32_t channel_id, uint32_t rank)
   {
      if (id > 2)
         Error_Handler();

      auto* hadc = &adc_handles[id-1];

      // Channel configuration
      // ADC channel configuration structure declaration
      ADC_ChannelConfTypeDef sConfig;

      sConfig.Channel      = channel_id;              // Sampled channel number
      sConfig.Rank         = rank;                    // Rank of sampled channel number
      sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5; // Sampling time (number of clock cycles unit)
      sConfig.SingleDiff   = ADC_SINGLE_ENDED;        // Single-ended input channel
      sConfig.OffsetNumber = ADC_OFFSET_NONE;         // No offset subtraction
      sConfig.Offset = 0;                             // Parameter discarded because offset correction is disabled
      if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
      {
         Error_Handler();
      }
   }

   DMA_Stream_TypeDef* get_dma_stream(std::size_t id)
   {
      switch (id)
      {
         case 1: return DMA2_Stream0;
         case 2: return DMA2_Stream1;
         case 3: return DMA2_Stream2;
      }
      return nullptr; // Default (won't happen)
   }

   IRQn_Type get_dma_irq_id(std::size_t id)
   {
      switch (id)
      {
         case 1: return DMA2_Stream0_IRQn;
         case 2: return DMA2_Stream1_IRQn;
         case 3: return DMA2_Stream2_IRQn;
      }
      return DMA2_Stream0_IRQn; // Default (won't happen)
   }
}}}

extern "C"
{
   using namespace cycfi::infinity::detail;

   void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
   {
      std::size_t id;
      auto* hdma = get_hdma(hadc, id);
      if (!hdma)
         return;

      // ADC Periph clock enable
      if (id == 3)
         __HAL_RCC_ADC3_CLK_ENABLE();
      else
         __HAL_RCC_ADC12_CLK_ENABLE();

      // ADC Periph interface clock configuration
      __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);

      // Enable DMA clock
      __HAL_RCC_DMA2_CLK_ENABLE();

      // Configure DMA
      hdma->Instance = get_dma_stream(id);
      hdma->Init.Request = DMA_REQUEST_ADC1;
      hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;
      hdma->Init.PeriphInc = DMA_PINC_DISABLE;
      hdma->Init.MemInc = DMA_MINC_ENABLE;
      hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      hdma->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
      hdma->Init.Mode = DMA_CIRCULAR;
      hdma->Init.Priority = DMA_PRIORITY_MEDIUM;

      // Deinitialize & Initialize the DMA for new transfer
      HAL_DMA_DeInit(hdma);
      HAL_DMA_Init(hdma);

      // Associate the DMA handle
      __HAL_LINKDMA(hadc, DMA_Handle, *hdma);

      // NVIC configuration for DMA Input data interrupt
      auto irq_id = get_dma_irq_id(id);
      HAL_NVIC_SetPriority(irq_id, 1, 0);
      HAL_NVIC_EnableIRQ(irq_id);
   }
}

*/