/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <soniq/support.hpp>
#include <soniq/pin.hpp>
#include <soniq/app.hpp>
#include <soniq/i2c.hpp>
#include <soniq/canvas.hpp>
#include <soniq/adc.hpp>
#include <soniq/ssd1306.hpp>
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
namespace snq = cycfi::soniq;

using namespace snq::monochrome;
using namespace snq::port;
using namespace q::literals;

#if defined(STM32H7)
constexpr auto scl_pin = portb+6;
constexpr auto sda_pin = portb+7;
#elif defined(STM32F4)
constexpr auto scl_pin = portb+10;
constexpr auto sda_pin = portb+3;
#endif

using snq::delay_ms;
using canvas_type = snq::mono_canvas<128, 32>;
using i2c_type = snq::i2c_master<scl_pin, sda_pin>;
using oled_type = snq::ssd1306<i2c_type, canvas_type>;

constexpr int sampling_rate = 16000;

///////////////////////////////////////////////////////////////////////////////
// Peripherals
i2c_type i2c;
inf::timer<2> master_clock;
alignas(32) snq::adc<1, 1, 256> adc; // stm32h7 requires this to be 32-bits aligned

///////////////////////////////////////////////////////////////////////////////
// ADC conversion complete task
float adc_val = 0;
q::one_pole_lowpass lp{10_Hz, sampling_rate}; // 10Hz low pass filter

inf::main_test_pin_type test;

void conversion_complete();

///////////////////////////////////////////////////////////////////////////////
// Configuration
auto config = snq::config(
   test.setup(),
   i2c.setup(),
   master_clock.setup(2000000, sampling_rate),
   adc.setup(master_clock, conversion_complete),
   adc.enable_channels<4>()
);

void conversion_complete()
{
   for (int i = 0; i < 256; ++i)
      adc_val = lp(adc[i][0]);

   test = !test;
}

///////////////////////////////////////////////////////////////////////////////
int main()
{
   snq::system_init();

   oled_type cnv{i2c};
   adc.start();
   master_clock.start();

   while (true)
   {
      char out[sizeof(int)*8+1];
      snq::to_string(adc_val, out);

      cnv.clear();
      cnv.draw_string(out, 15, 10, font::medium);
      cnv.refresh();

      delay_ms(250);
   }
}

// The actual "C" interrupt handlers are defined here:
#include <soniq/irq.hpp>
