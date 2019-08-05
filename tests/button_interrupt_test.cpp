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
// Peripherals
inf::main_led_type led;
inf::main_button_type btn;

///////////////////////////////////////////////////////////////////////////////
// Our button task
void button_task()
{
   // We debounce the button by ignoring fast transitions
   // (edges spaced less than 200ms)

   static std::uint32_t t = 0;
   auto now = inf::millis();
   if (now - t > 200)
      led = !led; // Toggle the LED
   t = now;
}

///////////////////////////////////////////////////////////////////////////////
// Configuration
auto config = inf::config(
   led.setup(),
   btn.setup(button_task, 10)
);

///////////////////////////////////////////////////////////////////////////////
int main()
{
   inf::system_init();
   led = off;

   if (btn)
      btn.start(falling_edge);   // call button_task on the falling edge
   else
      btn.start(rising_edge);    // call button_task on the rising edge

   while (true)
   {
   }
}

// The actual "C" interrupt handlers are defined here:
#include <inf/irq.hpp>
