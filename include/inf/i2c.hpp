/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_I2C_HPP_AUGUST_16_2019)
#define CYCFI_INFINITY_I2C_HPP_AUGUST_16_2019

#include <inf/detail/i2c_impl.hpp>
#include <inf/pin.hpp>
#include <inf/config.hpp>
#include <cstdint>

namespace cycfi { namespace infinity
{
   ////////////////////////////////////////////////////////////////////////////
   // i2c
   ////////////////////////////////////////////////////////////////////////////
   template <std::size_t scl_pin_, std::size_t sda_pin_>
   struct i2c_master
   {
      static_assert(detail::valid_scl_pin<scl_pin_>(), "Invalid SCL pin");
      static_assert(detail::valid_sda_pin<sda_pin_>(), "Invalid SDA pin");

      static constexpr std::size_t scl_pin = scl_pin_;
      static constexpr std::size_t sda_pin = sda_pin_;
      static constexpr std::size_t id = detail::scl_pin<scl_pin>::i2c_id;
      using scl_peripheral_id = io_pin_id<scl_pin>;
      using sda_peripheral_id = io_pin_id<sda_pin>;

      void init();

      auto setup()
      {
         init();
         return [](auto base)
         {
            auto cfg1 = make_basic_config<scl_peripheral_id>(base);
            return make_basic_config<sda_peripheral_id>(cfg1);
         };
      }

      void write(
         std::uint32_t addr, std::uint8_t const* data,
         std::size_t len, uint32_t timeout = 0xffffffff
      );

      void read(
         std::uint32_t addr, std::uint8_t* data,
         std::size_t len, uint32_t timeout = 0xffffffff
      );

      void mem_write8(
         std::uint32_t addr, std::uint16_t mem_addr, std::uint8_t data,
         uint32_t timeout = 0xffffffff
      );

      void mem_write16(
         std::uint32_t addr, std::uint16_t mem_addr, std::uint16_t data,
         uint32_t timeout = 0xffffffff
      );

      std::uint8_t mem_read8(
         std::uint32_t addr, std::uint16_t mem_addr, uint32_t timeout = 0xffffffff
      );

      std::uint16_t mem_read16(
         std::uint32_t addr, std::uint16_t mem_addr, uint32_t timeout = 0xffffffff
      );
   };

   ////////////////////////////////////////////////////////////////////////////
   // i2c implementation
   ////////////////////////////////////////////////////////////////////////////
   template <std::size_t scl_pin, std::size_t sda_pin>
   inline void i2c_master<scl_pin, sda_pin>::init()
   {
      static constexpr auto scl_port = scl_pin / 16;
      static constexpr auto sda_port = sda_pin / 16;

      // Enable GPIO peripheral clocks
      detail::enable_port_clock<scl_port>();
      detail::enable_port_clock<sda_port>();

      detail::i2c_config(
         id,
         detail::get_port<scl_port>(),
         1 << (scl_pin % 16),
         detail::get_port<sda_port>(),
         1 << (sda_pin % 16)
      );
   }

   template <std::size_t scl_pin, std::size_t sda_pin>
   inline void i2c_master<scl_pin, sda_pin>::write(
      std::uint32_t addr, std::uint8_t const* data,
      std::size_t len, uint32_t timeout
   )
   {
      detail::i2c_write(id, addr, data, len, timeout);
   }

   template <std::size_t scl_pin, std::size_t sda_pin>
   inline void i2c_master<scl_pin, sda_pin>::read(
      std::uint32_t addr, std::uint8_t* data,
      std::size_t len, uint32_t timeout
   )
   {
      detail::i2c_read(id, addr, data, len, timeout);
   }

   template <std::size_t scl_pin, std::size_t sda_pin>
   inline void i2c_master<scl_pin, sda_pin>::mem_write8(
      std::uint32_t addr, std::uint16_t mem_addr, std::uint8_t data,
      uint32_t timeout
   )
   {
      detail::i2c_mem_write(id, addr, mem_addr, 1, &data, 1, timeout);
   }

   template <std::size_t scl_pin, std::size_t sda_pin>
   inline void i2c_master<scl_pin, sda_pin>::mem_write16(
      std::uint32_t addr, std::uint16_t mem_addr, std::uint16_t data,
      uint32_t timeout
   )
   {
      detail::i2c_mem_write(id, addr, mem_addr, 2,
         reinterpret_cast<std::uint8_t const*>(&data), 2, timeout);
   }

   template <std::size_t scl_pin, std::size_t sda_pin>
   inline std::uint8_t i2c_master<scl_pin, sda_pin>::mem_read8(
      std::uint32_t addr, std::uint16_t mem_addr, uint32_t timeout
   )
   {
      std::uint8_t result;
      detail::i2c_mem_read(id, addr, mem_addr, 1, &result, 1, timeout);
      return result;
   }

   template <std::size_t scl_pin, std::size_t sda_pin>
   inline std::uint16_t i2c_master<scl_pin, sda_pin>::mem_read16(
      std::uint32_t addr, std::uint16_t mem_addr, uint32_t timeout
   )
   {
      std::uint16_t result;
      detail::i2c_mem_read(id, addr, mem_addr, 2,
         reinterpret_cast<std::uint8_t*>(&result), 2, timeout);
      return result;
   }
}}

#endif
