/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/pin.hpp>
#include <inf/app.hpp>
#include <inf/i2c.hpp>
#include <inf/canvas.hpp>
#include <inf/adc.hpp>
#include <inf/ssd1306.hpp>
#include <q/fx/lowpass.hpp>
#include <cstring>

///////////////////////////////////////////////////////////////////////////////
// Test the adc.
//
// Setup: Connect a 3.3v voltage divider using a potentiometer to and ADC pin
//        and the ssd1306 oled I2C SCL and SDA. The ADC value displayed in
//        the OLED follows the movement of the pot.
//
//    STM32F4:
//       ADC: PA4
//       SCL: PB10
//       SDA: PB3
//
//    STM32H7:
//       ADC: XXX
//       SCL: PB6
//       SDA: PB7
//
///////////////////////////////////////////////////////////////////////////////

namespace q = cycfi::q;
namespace inf = cycfi::infinity;

using namespace inf::monochrome;
using namespace inf::port;
using namespace q::literals;

#if defined(STM32H7)
constexpr auto scl_pin = portb+6;
constexpr auto sda_pin = portb+7;
#elif defined(STM32F4)
constexpr auto scl_pin = portb+10;
constexpr auto sda_pin = portb+3;
#endif

using inf::delay_ms;
using canvas_type = inf::mono_canvas<128, 32>;
using i2c_type = inf::i2c_master<scl_pin, sda_pin>;
using oled_type = inf::ssd1306<i2c_type, canvas_type>;

constexpr int sampling_rate = 16000;

///////////////////////////////////////////////////////////////////////////////
// Peripherals
i2c_type i2c;
inf::timer<2> master_clock;
alignas(32) inf::adc<1, 1> adc; // stm32h7 requires this to be 32-bits aligned

///////////////////////////////////////////////////////////////////////////////
// ADC conversion complete task
volatile int32_t adc_val = 0;
q::one_pole_lowpass lp{10_Hz, sampling_rate}; // 10Hz low pass filter

inline void conversion_complete()
{
   for (int i = 0; i < 8; ++i)
      adc_val = lp(adc[i][0]);
}

///////////////////////////////////////////////////////////////////////////////
// Configuration
auto config = inf::config(
   i2c.setup(),
   master_clock.setup(2000000, sampling_rate),
   adc.setup(master_clock, conversion_complete),
   adc.enable_channels<4>()
);

///////////////////////////////////////////////////////////////////////////////
int main()
{
   inf::system_init();

   oled_type cnv{i2c};
   adc.start();
   master_clock.start();

   int v = 0;

   while (true)
   {
      char out[sizeof(int)*8+1];
      inf::to_string(v, out);

      cnv.clear();
      cnv.draw_string(out, 15, 10, font::medium);
      cnv.refresh();

      delay_ms(100);
      v++;
      if (v == 9999)
         v = 0;
   }
}

// The actual "C" interrupt handlers are defined here:
#include <inf/irq.hpp>
