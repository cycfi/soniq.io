/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/pin.hpp>
#include <inf/app.hpp>
#include <inf/i2c.hpp>
#include <inf/canvas.hpp>
#include <inf/ssd1306.hpp>

///////////////////////////////////////////////////////////////////////////////
// OLED ssd1306 test.
//
// Setup: Connect the oled I2C SCL to PB10 and SDA to PB3.
///////////////////////////////////////////////////////////////////////////////

namespace inf = cycfi::infinity;
using namespace inf::port;

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
using namespace inf::monochrome;

///////////////////////////////////////////////////////////////////////////////
// Peripherals
i2c_type i2c;

///////////////////////////////////////////////////////////////////////////////
// Configuration.
auto config = inf::config(i2c.setup());

///////////////////////////////////////////////////////////////////////////////
int main()
{
   inf::system_init();
   oled_type cnv{i2c};

   while (true)
   {
      // Draw some lines
      for (std::size_t y = 0; y < cnv.height; y += 3)
      {
         cnv.draw_line(cnv.width / 2, y, cnv.width, 0);
         cnv.refresh();
         delay_ms(100);
      }
      for (std::size_t x = cnv.width / 2; x < cnv.width; x += 3)
      {
         cnv.draw_line(cnv.width, 0, x, cnv.height);
         cnv.refresh();
         delay_ms(100);
      }
      delay_ms(1000);

      // Draw some rects
      for (int i = 0; i < 32; i += 3)
      {
         auto w = cnv.width / 2 - i * 2;
         auto h = cnv.height - i * 2;
         cnv.draw_rect(i, i, w, h);
         cnv.refresh();
         delay_ms(100);
      }

      delay_ms(1000);
      cnv.clear();

      cnv.draw_string("Hello, World!", 25, 0, font::small);
      cnv.draw_string("Hello, World!", 15, 15, font::medium);
      cnv.refresh();
      delay_ms(1000);

      for (int y = -30; y <= 0; ++y)
      {
         cnv.clear();
         cnv.draw_string("cycfi", 30, y, font::large);
         cnv.refresh();
         delay_ms(50);
      }

      cnv.bright(false);
      for (int v = 255; v >= 0; --v)
      {
         cnv.brightness(v);
         delay_ms(1);
      }

      cnv.bright();
      delay_ms(1000);
      cnv.clear();
   }
}
