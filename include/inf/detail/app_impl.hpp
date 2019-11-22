/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_APP_IMPL_HPP_JANUARY_1_2016)
#define CYCFI_INFINITY_APP_IMPL_HPP_JANUARY_1_2016

namespace cycfi { namespace soniq
{
   ////////////////////////////////////////////////////////////////////////////
   // Generic LED indicator (used as a general indicator)
   // (Port and Pin is board/application specific)
   ////////////////////////////////////////////////////////////////////////////

#if defined(NUCLEO_H743ZI)

   // NUCLEO-H743ZI
   constexpr auto main_led = port::portb + 0;
   constexpr auto main_button = port::portc + 13;
   constexpr auto main_test_pin = port::porta + 15;

   constexpr auto led1 = port::portb + 0;
   constexpr auto led2 = port::portb + 7;
   constexpr auto led3 = port::portb + 14;

#endif

}}

#endif
