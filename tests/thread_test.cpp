/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/pin.hpp>
#include <inf/app.hpp>

///////////////////////////////////////////////////////////////////////////////
// Simplest toggle the led test. This test uses simple delay to toggle
// the leds at a rate of 1 per second. No setup required.
///////////////////////////////////////////////////////////////////////////////

namespace snq = cycfi::soniq;
using namespace snq::port;

///////////////////////////////////////////////////////////////////////////////
int main()
{
   auto led = out<inf::main_led>();

   while (true)
   {
      snq::delay_ms(1000);
      led = !led;    // toggle LED
   }
}


