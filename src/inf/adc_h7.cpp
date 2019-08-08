/*=============================================================================
   Copyright (c) 2015-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/detail/adc_impl_h7.hpp>

///////////////////////////////////////////////////////////////////////////////
// This implementation is for the STM32H7 series.
///////////////////////////////////////////////////////////////////////////////

namespace cycfi { namespace infinity { namespace detail
{

}}}

extern "C"
{
   void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
   {
      // Enable GPIO clock
      __HAL_RCC_GPIOA_CLK_ENABLE();

      // ADC Periph clock enable
      __HAL_RCC_ADC12_CLK_ENABLE();

      // ADC Periph interface clock configuration
      __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);

      // Enable DMA clock
      __HAL_RCC_DMA1_CLK_ENABLE();

      // Configure DMA

      static DMA_HandleTypeDef dma;
      dma.Instance = DMA1_Stream1;
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
      HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);
      HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
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
}