/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/pin.hpp>
#include <inf/app.hpp>

///////////////////////////////////////////////////////////////////////////////
//
// Interrupt button test. The main button, configured normally 1 or normally
// 0, depending on the target dev-board. The button is also configured to
// fire up an interrupt on the falling edge, when the button is configured
// normally 1, or rising edge, when the button is configured normally 0. An
// exti_task is setup to handle this interrupt. The task simply toggles the
// main LED. No setup required.
///////////////////////////////////////////////////////////////////////////////

namespace inf = cycfi::infinity;
using namespace inf::port;

///////////////////////////////////////////////////////////////////////////////
int main()
{
   auto led = out<inf::main_led>();
   auto btn = in<inf::main_button>();
   auto debounce = inf::debouncer<>{};

   btn.on_rising_edge(
      [&]
      {
         if (debounce())
            led = !led; // Toggle the LED
      }
   );

   while (true)
   {
   }
}

// The actual "C" interrupt handlers are defined here:
#include <inf/irq.hpp>
