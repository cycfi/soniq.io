/*=============================================================================
   Copyright (c) 2015-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_ADC_IMPL_HPP_MAY_1_2017)
#define CYCFI_INFINITY_ADC_IMPL_HPP_MAY_1_2017

///////////////////////////////////////////////////////////////////////////////
// This implementation is for the STM32H7 series.
///////////////////////////////////////////////////////////////////////////////

namespace cycfi { namespace infinity { namespace detail
{
   // Check if id is a valid adc.
   constexpr bool valid_adc(std::size_t id)
   {
      return id >=1 && id <= 3;
   }

   // Check if id is a valid timer for the adc.
   constexpr bool valid_adc_timer(std::size_t id)
   {
      return id == 1 || id == 2 || id == 3 ||
         id == 4 || id == 5 || id == 8 || id == 15
         ;
   }

}}}

#endif
