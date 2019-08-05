/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/pin.hpp>
#include <inf/app.hpp>

///////////////////////////////////////////////////////////////////////////////
// Simplest button test. We poll the main button (which is configured
// with a pull-up to vcc, hence normally 1). The main led is turned ON
// if the button is pushed, otherwise the main led is turned OFF.
// No setup required.
///////////////////////////////////////////////////////////////////////////////

namespace inf = cycfi::infinity;
using namespace inf::port;
using inf::delay_ms;

///////////////////////////////////////////////////////////////////////////////
// Peripherals
inf::main_led_type led;
inf::main_button_type btn;

///////////////////////////////////////////////////////////////////////////////
// Configuration
auto config = inf::config(
   led.setup(),
   btn.setup()
);

///////////////////////////////////////////////////////////////////////////////
int main()
{
   inf::system_init();
   led = off;

   // toggle LED
   while (true)
   {
      delay_ms(30);
      led = btn;
   }
}
