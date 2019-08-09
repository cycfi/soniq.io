/*=============================================================================
   Copyright (c) 2015-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/device.hpp>
#include <inf/detail/adc_impl_h7.hpp>

// $$$ TEMP $$$
# include <stm32h7xx_ll_dma.h>


///////////////////////////////////////////////////////////////////////////////
// This implementation is for the STM32H7 series.
///////////////////////////////////////////////////////////////////////////////

namespace cycfi { namespace infinity { namespace detail
{

}}}

extern "C"
{
   ADC_HandleTypeDef AdcHandle;

   // #define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  32)   /* Size of array aADCxConvertedData[] */
   // ALIGN_32BYTES (static uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]);

   static uint16_t* buff = 0;
   static uint16_t size = 0;

   void init_adc(uint16_t values_[], uint16_t size_)
   {
      buff = values_;
      size = size_;

      // Initialize ADC peripheral
      AdcHandle.Instance = ADC1;
      if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
      {
         // ADC de-initialization Error
         Error_Handler();
      }

      AdcHandle.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;            /* Asynchronous clock mode, input ADC clock divided by 2*/
      AdcHandle.Init.Resolution               = ADC_RESOLUTION_16B;              /* 16-bit resolution for converted data */
      AdcHandle.Init.ScanConvMode             = ADC_SCAN_DISABLE;                         /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
      AdcHandle.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;             /* EOC flag picked-up to indicate conversion end */
      AdcHandle.Init.LowPowerAutoWait         = DISABLE;                         /* Auto-delayed conversion feature disabled */
      AdcHandle.Init.ContinuousConvMode       = DISABLE;                          /* Continuous mode enabled (automatic conversion restart after each conversion) */
      AdcHandle.Init.NbrOfConversion          = 1;                               /* Parameter discarded because sequencer is disabled */
      AdcHandle.Init.DiscontinuousConvMode    = DISABLE;                         /* Parameter discarded because sequencer is disabled */
      // AdcHandle.Init.NbrOfDiscConversion      = 1;                               /* Parameter discarded because sequencer is disabled */
      AdcHandle.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_T2_TRGO;              /* Software start to trig the 1st conversion manually, without external event */
      AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_RISING;   /* Parameter discarded because software trigger chosen */
      AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; /* $$$ hrmmm */ /* ADC DMA circular requested */
      AdcHandle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;        /* DR register is overwritten with the last conversion result in case of overrun */
      AdcHandle.Init.OversamplingMode         = DISABLE;                         /* No oversampling */

      // Initialize ADC peripheral according to the passed parameters
      if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
      {
         Error_Handler();
      }

      // Start calibration
      if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
      {
         Error_Handler();
      }

      // Channel configuration
      // ADC channel configuration structure declaration
      ADC_ChannelConfTypeDef sConfig;

      sConfig.Channel      = ADC_CHANNEL_3;              /* Sampled channel number */
      sConfig.Rank         = ADC_REGULAR_RANK_1;         /* Rank of sampled channel number ADC_CHANNEL_3 */
      sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5; // ADC_SAMPLETIME_810CYCLES_5;   /* Sampling time (number of clock cycles unit) */
      sConfig.SingleDiff   = ADC_SINGLE_ENDED;           /* Single-ended input channel */
      sConfig.OffsetNumber = ADC_OFFSET_NONE;            /* No offset subtraction */
      sConfig.Offset = 0;                                /* Parameter discarded because offset correction is disabled */
      if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
      {
         Error_Handler();
      }

      // Configure peripheral GPIO
      // ADC Channel GPIO pin configuration
      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.Pin = GPIO_PIN_6;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      if (HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)values_, size_) != HAL_OK)
      {
         Error_Handler();
      }
   }

   void start(uint16_t values_[], uint16_t size_)
   {
      // Start conversion in DMA mode
      // if (HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)values_, size_) != HAL_OK)
      // {
      //    Error_Handler();
      // }

      // LL_TIM_EnableIT_UPDATE(&cycfi::infinity::detail::get_timer<2>());


      // (__HANDLE__)->Instance->DIER |= TIM_IT_UPDATE

      // HAL_TIM_Base_Start_IT(&htim4);

   }

   void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
   {
      // Enable GPIO clock
      __HAL_RCC_GPIOA_CLK_ENABLE();

      // ADC Periph clock enable
      __HAL_RCC_ADC12_CLK_ENABLE();

      // ADC Periph interface clock configuration
      __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);

      // Enable DMA clock
      __HAL_RCC_DMA2_CLK_ENABLE();

      // Configure DMA
      static DMA_HandleTypeDef dma;
      dma.Instance = DMA2_Stream0;
      dma.Init.Request = DMA_REQUEST_ADC1;
      dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
      dma.Init.PeriphInc = DMA_PINC_DISABLE;
      dma.Init.MemInc = DMA_MINC_ENABLE;
      dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
      dma.Init.Mode = DMA_CIRCULAR;
      dma.Init.Priority = DMA_PRIORITY_MEDIUM;
      /* Deinitialize  & Initialize the DMA for new transfer */
      HAL_DMA_DeInit(&dma);
      HAL_DMA_Init(&dma);

      // Associate the DMA handle
      __HAL_LINKDMA(hadc, DMA_Handle, dma);

      // NVIC configuration for DMA Input data interrupt
      HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 1, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
   }

   void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
   {
      // Reset peripherals
      __HAL_RCC_ADC12_FORCE_RESET();
      __HAL_RCC_ADC12_RELEASE_RESET();

      // ADC Periph clock disable
      // (automatically reset all ADC instances of the ADC common group)
      __HAL_RCC_ADC12_CLK_DISABLE();
   }

//    void foo() {}

//    void DMA2_Stream0_IRQHandler()
//    {
//       if (LL_DMA_IsActiveFlag_TC0(DMA2) == 1)
//       {
//          LL_DMA_ClearFlag_TC0(DMA2);

//          // Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes
//          SCB_InvalidateDCache_by_Addr((uint32_t*) buff + (size/2), size);

//          foo();
//       }

//       if (LL_DMA_IsActiveFlag_HT0(DMA2) == 1)
//       {
//          LL_DMA_ClearFlag_HT0(DMA2);

//          // Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes
//          SCB_InvalidateDCache_by_Addr((uint32_t*) buff, size);

//          foo();
//       }

//       if (LL_DMA_IsActiveFlag_TE0(DMA2) == 1)
//       {
//          LL_DMA_ClearFlag_TE0(DMA2);
//          foo();
//       }

// //      HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
//    }
}