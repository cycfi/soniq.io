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

namespace inf = cycfi::infinity;
using namespace inf::port;
using inf::delay_ms;

///////////////////////////////////////////////////////////////////////////////
int main()
{
   inf::system_init();

   auto led = out<portb + 0>();

   led = off;
   while (true)
   {
      delay_ms(1000);
      led = !led;    // toggle LED
   }
}


