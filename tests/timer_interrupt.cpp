/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/timer.hpp>
#include <inf/pin.hpp>
#include <inf/config.hpp>
#include <inf/app.hpp>

///////////////////////////////////////////////////////////////////////////////
// Toggle led test using timers and interrupts. This test uses a timer to
// toggle the led at a rate of 1 per second. No setup required.
///////////////////////////////////////////////////////////////////////////////

namespace snq = cycfi::soniq;
using namespace snq::port;

///////////////////////////////////////////////////////////////////////////////
// Peripherals
inf::main_led_type led;
inf::timer<3> tmr;

///////////////////////////////////////////////////////////////////////////////
// Our timer task
void timer_task()
{
   led = !led;
}

///////////////////////////////////////////////////////////////////////////////
// Configuration
constexpr uint32_t base_freq = 10000;

auto config = snq::config(
   led.setup(),
   tmr.setup(base_freq, 1, timer_task) // calls timer_task every 1Hz
);

///////////////////////////////////////////////////////////////////////////////
int main()
{
   snq::system_init();

   tmr.start();
   led = on;
   while (true)
      ;
}

// The actual "C" interrupt handlers are defined here:
#include <inf/irq.hpp>
