/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/pin.hpp>
#include <inf/app.hpp>

///////////////////////////////////////////////////////////////////////////////
// Simplest button test. We poll the main button (which is configured
// normally high or normally low, depending on the target dev-board. The main
// led is turned ON or OFF depending on the state of the switch. No setup
// required.
///////////////////////////////////////////////////////////////////////////////

namespace inf = cycfi::infinity;
using namespace inf::port;

///////////////////////////////////////////////////////////////////////////////
int main()
{
   auto led = out<inf::main_led>();
   auto btn = in<inf::main_button>();

   while (true)
   {
      inf::delay_ms(30);
      led = btn;
   }
}
