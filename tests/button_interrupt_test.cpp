/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/pin.hpp>
#include <inf/app.hpp>

///////////////////////////////////////////////////////////////////////////////
// Interrupt button test. The main button fires up an interrupt on the rising
// edge. An exti_task is setup to handle this interrupt. The task simply
// turns on main LED. When the main function sees the LED on, it turns off
// the LED after 2 seconds.
///////////////////////////////////////////////////////////////////////////////

namespace snq = cycfi::soniq;
using namespace snq::port;

///////////////////////////////////////////////////////////////////////////////
int main()
{
   auto led = out<snq::main_led>();
   auto btn = in<snq::main_button>();

   btn.on_rising_edge([&]{ led = on; });

   while (true)
   {
      if (led)
      {
         snq::delay_ms(2000);
         led = off;
      }
   }
}

// The actual "C" interrupt handlers are defined here:
#include <inf/irq.hpp>
