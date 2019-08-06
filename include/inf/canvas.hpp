/*=============================================================================
   Copyright (c) 2015-2017 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_CANVAS_HPP_MAY_9_2017)
#define CYCFI_INFINITY_CANVAS_HPP_MAY_9_2017

#include <utility>
#include <cstdint>
#include <string>

namespace cycfi { namespace infinity
{
   ////////////////////////////////////////////////////////////////////////////
   // Simple basic types for monochrome graphics
   ////////////////////////////////////////////////////////////////////////////
   namespace monochrome
   {
      enum class color
      {
         white,
         black,
         inverse,
      };

      enum class font
      {
         small,
         medium,
         large,
      };

      struct bitmap
      {
         std::uint8_t const* data;
         int width;
         int height;
      };
   }

   ////////////////////////////////////////////////////////////////////////////
   // mono_canvas canvas. This is implements routines for drawing
   // into monochromatic OLED frame buffers.
   ////////////////////////////////////////////////////////////////////////////
   template <std::size_t width_, std::size_t height_>
   struct mono_canvas
   {
   public:

      enum { width  = width_, height = height_ };
      using color = monochrome::color;
      using font = monochrome::font;
      using bitmap = monochrome::bitmap;
      static std::size_t const size = (width * height) / 8;

      mono_canvas(std::uint8_t* buffer_)
       : _buffer(buffer_)
      {
         clear();
      }

      void  clear();

      void  draw_pixel(int x, int y, color color_ = color::white);
      void  draw_line(int x0, int y0, int x1, int y1, color color_ = color::white);
      void  fill_rect(int x, int y, int w, int h, color color_ = color::white);
      void  draw_rect(int x, int y, int w, int h, color color_ = color::white);
      void  draw_bitmap(bitmap const& img, int x, int y, color color_ = color::white);

      void  fast_hline(int x, int y, int w, color color_ = color::white);
      void  fast_vline(int x, int y, int h, color color_ = color::white);

      int   draw_char(char ch, int x, int y, font font_, color color_ = color::white);
      int   draw_string(char const* str, int x, int y, font font_, color color_ = color::white);
      int   draw_string(std::string const& str, int x, int y, font font_, color color_ = color::white);

      std::uint8_t*           begin()        { return _buffer; }
      std::uint8_t const*     begin() const  { return _buffer; }
      std::uint8_t*           end()          { return _buffer + size; }
      std::uint8_t const*     end() const    { return _buffer + size; }

      std::uint8_t&           operator[](std::size_t i)        { return _buffer[i]; }
      std::uint8_t const      operator[](std::size_t i) const  { return _buffer[i]; }

   private:

      std::uint8_t* _buffer;
   };

   ////////////////////////////////////////////////////////////////////////////
   // canvas implementation
   ////////////////////////////////////////////////////////////////////////////
   namespace detail
   {
      void fast_hline_impl(
         std::uint8_t* buffer, int width, int height,
         int x, int y, int w, monochrome::color color_
      );

      void fast_vline_impl(
         std::uint8_t* buffer, int width, int height,
         int16_t x, int16_t y_, int16_t h_, monochrome::color color_
      );

      void line_impl(
         std::uint8_t* buffer, int width, int height,
         int x0, int y0, int x1, int y1, monochrome::color color_
      );

      void draw_bitmap(
          std::uint8_t* buffer, std::uint8_t* end, int width, int height,
          int x, int y, monochrome::bitmap const& bm, monochrome::color color_
      );

      inline void draw_byte(std::uint8_t& out, std::uint8_t pix, monochrome::color color_)
      {
         switch (color_)
         {
            case monochrome::color::white:   out |=  pix;  break;
            case monochrome::color::black:   out &= ~pix;  break;
            case monochrome::color::inverse: out ^=  pix;  break;
         }
      };

      inline void draw_pixel(
         std::uint8_t* buffer, int width, int height,
         int16_t x, int16_t y, monochrome::color color_)
      {
         if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
            return;

         auto mask = y&7;
         auto* p = buffer + (x + (y / 8) * width);
         draw_byte(*p, 1 << mask, color_);
      }

      int draw_char(
          std::uint8_t* buffer, std::uint8_t* end, int width, int height,
          int x, int y, char ch, monochrome::font font_, monochrome::color color_
      );
   }

   template <std::size_t width, std::size_t height>
   inline void mono_canvas<width, height>::clear()
   {
      for (auto& pixels : *this)
         pixels = 0;
   }

   template <std::size_t width, std::size_t height>
   inline void mono_canvas<width, height>::fast_hline(int x, int y, int w, color color_)
   {
      detail::fast_hline_impl(_buffer, width, height, x, y, w, color_);
   }

   template <std::size_t width, std::size_t height>
   inline void mono_canvas<width, height>::fast_vline(int x, int y, int h, color color_)
   {
      detail::fast_vline_impl(_buffer, width, height, x, y, h, color_);
   }

   template <std::size_t width, std::size_t height>
   void mono_canvas<width, height>::draw_pixel(int x, int y, color color_)
   {
      detail::draw_pixel(_buffer, width, height, x, y, color_);
   }

   template <std::size_t width, std::size_t height>
   inline void mono_canvas<width, height>::draw_line(int x0, int y0, int x1, int y1, color color_)
   {
      if (x0 == x1)
      {
         if (y0 > y1)
            std::swap(y0, y1);
         fast_vline(x0, y0, y1 - y0 + 1, color_);
      }
      else if (y0 == y1)
      {
         if(x0 > x1)
            std::swap(x0, x1);
         fast_hline(x0, y0, x1 - x0 + 1, color_);
      }
      else
      {
         detail::line_impl(_buffer, width, height, x0, y0, x1, y1, color_);
      }
   }

   template <std::size_t width, std::size_t height>
   inline void mono_canvas<width, height>::fill_rect(int x, int y, int w, int h, color color_)
   {
      for (auto i=x; i<x+w; ++i)
         fast_vline(i, y, h, color_);
   }

   template <std::size_t width, std::size_t height>
   inline void mono_canvas<width, height>::draw_rect(int x, int y, int w, int h, color color_)
   {
      fast_hline(x, y, w, color_);
      fast_hline(x, y+h-1, w, color_);
      fast_vline(x, y, h, color_);
      fast_vline(x+w-1, y, h, color_);
   }

   template <std::size_t width, std::size_t height>
   inline int mono_canvas<width, height>::draw_char(char ch, int x, int y, font font_, color color_)
   {
      return detail::draw_char(_buffer, _buffer+size, width, height, x, y, ch, font_, color_);
   }

   template <std::size_t width, std::size_t height>
   inline int mono_canvas<width, height>::draw_string(char const* str, int x, int y, font font_, color color_)
   {
      if (str)
      {
         while (*str)
            x = draw_char(*str++, x, y, font_, color_);
         return x;
      }
      return 0;
   }

   template <std::size_t width, std::size_t height>
   inline int mono_canvas<width, height>::draw_string(std::string const& str, int x, int y, font font_, color color_)
   {
      return draw_string(str.c_str(), x, y, font_, color_);
   }
}}

#endif
