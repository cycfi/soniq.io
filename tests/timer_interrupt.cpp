/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <soniq/timer.hpp>
#include <soniq/pin.hpp>
#include <soniq/app.hpp>

///////////////////////////////////////////////////////////////////////////////
// Toggle led test using timers and interrupts. This test uses a timer to
// toggle the led at a rate of 1 per second. No setup required.
///////////////////////////////////////////////////////////////////////////////

namespace snq = cycfi::soniq;
using namespace snq::port;
constexpr uint32_t base_freq = 10000;

///////////////////////////////////////////////////////////////////////////////
int main()
{
   auto led = out<snq::main_led>();
   auto tmr = snq::timer<3>{ base_freq, 1 };

   tmr.on_trigger(
      [&]
      {
         led = !led;
      }
   );

   tmr.start();
   while (true)
      ;
}
