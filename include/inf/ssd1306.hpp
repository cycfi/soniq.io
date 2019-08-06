/*=============================================================================
   Copyright (c) 2015-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_SSD_1306_HPP_AUGUST_16_2017)
#define CYCFI_INFINITY_SSD_1306_HPP_AUGUST_16_2017

#include <inf/support.hpp>
#include <inf/canvas.hpp>
#include <cstdint>

namespace cycfi { namespace infinity
{
   ////////////////////////////////////////////////////////////////////////////
   // ssd1306 driver
   ////////////////////////////////////////////////////////////////////////////
   template <typename Port, typename Canvas, std::size_t timeout_ = 0xffffffff>
   struct ssd1306 : Canvas
   {
   public:

      using port = Port;
      using canvas = Canvas;

      static std::size_t const timeout = timeout_;
      static std::size_t const height = Canvas::height;
      static std::size_t const width = Canvas::width;
      static std::size_t const buffer_size = Canvas::size + 1;

      ssd1306(Port& out);

      void refresh();
      void on();
      void off();

      void bright(bool val = true);
      void brightness(std::uint8_t val);

   private:

      void command(std::uint8_t cmd);
      void out(std::uint8_t const* data, std::size_t len);

      std::uint8_t   _buffer[buffer_size];
      Port&          _out;
   };

   ////////////////////////////////////////////////////////////////////////////
   // ssd1306 implementation
   ////////////////////////////////////////////////////////////////////////////
   namespace ssd1306_constants
   {
      enum
      {
         set_mux_ratio              = 0xa8,
         charge_pump                = 0x8d,
         enable_charge_val          = 0x14,
         set_pre_charge             = 0xd9,
         pre_charge_value           = 0x10,
         vcom_detect_value          = 0x10,

         memory_mode                = 0x20,

         set_contrast               = 0x81,
         default_contrast           = 0x8F,

         set_display_clock_div      = 0xd5,
         set_display_offset         = 0xd3,
         set_start_line             = 0x40,

         seg_remap                  = 0xa0,
         com_scan_inc               = 0xc0,
         com_scan_dec               = 0xc8,

         set_com_pins               = 0xda,
         set_vcom_detect            = 0xdb,
         display_all_on_resume      = 0xa4,
         display_all_on             = 0xa5,
         normal_display             = 0xa6,
         invert_display             = 0xa7,

         display_off                = 0xae,
         display_on                 = 0xaf,

         activate_scroll            = 0x2f,
         deactivate_scroll          = 0x2e,

         set_low_column             = 0x00,
         set_high_column            = 0x10,
         column_addr                = 0x21,
         page_addr                  = 0x22,

   		ssd1306_i2c_addr           = 0x78
      };
   }

   template <typename Port, typename Canvas, std::size_t timeout>
   inline void ssd1306<Port, Canvas, timeout>::command(std::uint8_t cmd)
   {
      uint8_t buf[2] = { 0, cmd };
      out(buf, 2);
   }

   template <typename Port, typename Canvas, std::size_t timeout>
   inline void ssd1306<Port, Canvas, timeout>::out(std::uint8_t const* data, std::size_t len)
   {
      using namespace ssd1306_constants;
      _out.write(ssd1306_i2c_addr, data, len, timeout);
   }

   template <typename Port, typename Canvas, std::size_t timeout>
   inline ssd1306<Port, Canvas, timeout>::ssd1306(Port& out)
    : Canvas(&_buffer[1])
	 , _out(out)
   {
      using namespace ssd1306_constants;

      // place the 0x40 prefix
      _buffer[0] = 0x40;

      // Turn display off
      off();

      // Wait for DC-DC to stabilise.
      delay_ms(100);

      // Set MUX ratio
      command(set_mux_ratio);
      command(height - 1);

      // Enable charge pump
      command(charge_pump);
      command(enable_charge_val);

      // Set the overall backlight brightness
      bright();

      // Memory mode: act like ks0108
      command(memory_mode);
      command(0x00);

      // Set default contrast
      command(set_contrast);
      command(default_contrast);

      command(set_display_clock_div);
      command(0x80);

      // Set no display offset
      command(set_display_offset);
      command(0x0);
      command(set_start_line | 0x0);

      command(seg_remap | 0x1);
      command(com_scan_dec);

      command(set_com_pins);
      if (width == 128 && height == 32)
         command(0x02);
      else if (width == 64 && height == 32)
         command(0x12);
      else if (width == 96 && height == 16)
         command(0x2);

      command(display_all_on_resume);
      command(normal_display);
      command(deactivate_scroll);

      // Clear the canvas
      Canvas::clear();
      refresh();

      // Turn display on
      on();

      // Wait 150ms for display to turn on
      delay_ms(150);
   }

   template <typename Port, typename Canvas, std::size_t timeout>
   inline void ssd1306<Port, Canvas, timeout>::refresh()
   {
      using namespace ssd1306_constants;

      command(column_addr);
      command(0);                   // Column start address (0 = reset)
      command(width-1);             // Column end address (127 = reset)

      command(page_addr);
      command(0);                   // Page start address (0 = reset)

      if (height == 64)
          command(7);               // Page end address
      else if (height == 32)
          command(3);               // Page end address
      else if (height == 16)
          command(1);               // Page end address

      out(_buffer, buffer_size);
   }

   template <typename Port, typename Canvas, std::size_t timeout>
   inline void ssd1306<Port, Canvas, timeout>::on()
   {
      using namespace ssd1306_constants;
      command(display_on);
   }

   template <typename Port, typename Canvas, std::size_t timeout>
   inline void ssd1306<Port, Canvas, timeout>::off()
   {
      using namespace ssd1306_constants;
      command(display_off);
   }

   template <typename Port, typename Canvas, std::size_t timeout>
   void ssd1306<Port, Canvas, timeout>::brightness(std::uint8_t val)
   {
      using namespace ssd1306_constants;
      command(set_contrast);
      command(val);
      command(set_vcom_detect);
      command(val / 4);
   }

   template <typename Port, typename Canvas, std::size_t timeout>
   void ssd1306<Port, Canvas, timeout>::bright(bool val)
   {
      using namespace ssd1306_constants;
      command(set_pre_charge);
      command(val ? 0xf1 : pre_charge_value);
      command(set_vcom_detect);
      command(val ? 0x40 : vcom_detect_value);
   }
}}

#endif
