/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <soniq/support.hpp>
#include <soniq/pin.hpp>
#include <soniq/app.hpp>

///////////////////////////////////////////////////////////////////////////////
// Simplest button test. We poll the main button (which is configured
// normally high or normally low, depending on the target dev-board. The main
// led is turned ON or OFF depending on the state of the switch. No setup
// required.
///////////////////////////////////////////////////////////////////////////////

namespace snq = cycfi::soniq;
using namespace snq::port;

///////////////////////////////////////////////////////////////////////////////
int main()
{
   auto led = out<snq::main_led>();
   auto btn = in<snq::main_button>();

   while (true)
   {
      snq::delay_ms(30);
      led = btn;
   }
}
