/*=============================================================================
   Copyright (c) 2015-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_I2C_IMPL_HPP_AUGUST_16_2019)
#define CYCFI_INFINITY_I2C_IMPL_HPP_AUGUST_16_2019

#if defined(STM32H7)
# include <stm32h7xx_hal.h>
#elif defined(STM32F4)
# include <stm32f4xx_hal.h>
#endif

#include <cstdint>
#include <inf/pin.hpp>

namespace cycfi { namespace infinity { namespace detail
{
   template <std::size_t pin>
   struct scl_pin
   {
      static constexpr int i2c_id = -1;
   };

   template <std::size_t pin>
   struct sda_pin
   {
      static constexpr int i2c_id = -1;
   };

#define INFINITY_SCL_PIN(pin, i2c_id_)                                         \
   template <>                                                                 \
   struct scl_pin<pin>                                                         \
   {                                                                           \
      static constexpr int i2c_id = i2c_id_;                                   \
   };                                                                          \
   /***/

   #define INFINITY_SDA_PIN(pin, i2c_id_)                                      \
   template <>                                                                 \
   struct sda_pin<pin>                                                         \
   {                                                                           \
      static constexpr int i2c_id = i2c_id_;                                   \
   };                                                                          \
   /***/

   template <std::size_t pin>
   constexpr bool valid_scl_pin()
   {
      return scl_pin<pin>::i2c_id != -1;
   }

   template <std::size_t pin>
   constexpr bool valid_sda_pin()
   {
      return sda_pin<pin>::i2c_id != -1;
   }

#if defined(STM32H7)

   INFINITY_SCL_PIN(port::portb + 6, 1);
   INFINITY_SDA_PIN(port::portb + 7, 1);
   INFINITY_SCL_PIN(port::portb + 8, 1);
   INFINITY_SDA_PIN(port::portb + 9, 1);

   INFINITY_SCL_PIN(port::portf + 1, 2);
   INFINITY_SDA_PIN(port::portf + 0, 2);
   INFINITY_SCL_PIN(port::portb + 10, 2);
   INFINITY_SDA_PIN(port::portb + 11, 2);

   INFINITY_SCL_PIN(port::porta + 8, 3);
   INFINITY_SDA_PIN(port::porta + 9, 3);

   // $$$ JDG Should we upposrt these? $$$
   // INFINITY_SCL_PIN(port::portb + 6, 4); // LL_GPIO_AF_6
   // INFINITY_SDA_PIN(port::portb + 7, 4); // LL_GPIO_AF_6
   // INFINITY_SCL_PIN(port::portb + 8, 4); // LL_GPIO_AF_6
   // INFINITY_SDA_PIN(port::portb + 9, 4); // LL_GPIO_AF_6

   INFINITY_SCL_PIN(port::portd + 12, 4);
   INFINITY_SDA_PIN(port::portd + 13, 4);
   INFINITY_SDA_PIN(port::portf + 15, 4);


#elif defined(STM32F4)

   INFINITY_SCL_PIN(port::portb + 6, 1);
   INFINITY_SDA_PIN(port::portb + 7, 1);
   INFINITY_SCL_PIN(port::portb + 8, 1);
   INFINITY_SDA_PIN(port::portb + 9, 1);
   INFINITY_SCL_PIN(port::portc + 6, 1);
   INFINITY_SDA_PIN(port::portc + 7, 1);
   INFINITY_SCL_PIN(port::portd + 12, 1);
   INFINITY_SDA_PIN(port::portd + 13, 1);
   INFINITY_SCL_PIN(port::portd + 14, 1);
   INFINITY_SDA_PIN(port::portd + 15, 1);
   INFINITY_SCL_PIN(port::portf + 14, 1);
   INFINITY_SDA_PIN(port::portf + 15, 1);

   INFINITY_SCL_PIN(port::portb + 10, 2);
   INFINITY_SDA_PIN(port::portb + 3, 2);
   INFINITY_SDA_PIN(port::portb + 11, 2);
   INFINITY_SDA_PIN(port::portc + 12, 2);
   INFINITY_SCL_PIN(port::portf + 1, 2);
   INFINITY_SDA_PIN(port::portf + 0, 2);

   INFINITY_SCL_PIN(port::porta + 8, 3);
   INFINITY_SDA_PIN(port::portb + 4, 3);
   INFINITY_SDA_PIN(port::portc + 9, 3);

#endif

   void i2c_config(
	  std::size_t id,
      GPIO_TypeDef& scl_gpio, std::uint32_t scl_pin_mask, std::uint32_t scl_af,
      GPIO_TypeDef& sda_gpio, std::uint32_t sda_pin_mask, std::uint32_t sda_af
   );

   extern I2C_HandleTypeDef i2c_handles[3];

   inline void i2c_write(
      std::size_t id, std::uint32_t addr,
      uint8_t const* data, uint32_t len, uint32_t timeout)
   {
      HAL_I2C_Master_Transmit(
         &i2c_handles[id-1], addr, const_cast<uint8_t*>(data), len, timeout);
   }

   inline void i2c_read(
      std::size_t id, std::uint32_t addr,
      uint8_t* data, uint32_t len, uint32_t timeout)
   {
      HAL_I2C_Master_Receive(
         &i2c_handles[id-1], addr, data, len, timeout);
   }

   inline void i2c_mem_write(
      std::size_t id, std::uint32_t addr, uint16_t mem_addr, uint16_t mem_size,
      uint8_t const* data, uint32_t len, uint32_t timeout)
   {
      HAL_I2C_Mem_Write(
         &i2c_handles[id-1], addr, mem_addr, mem_size, const_cast<uint8_t*>(data), len, timeout);
   }

   inline void i2c_mem_read(
      std::size_t id, std::uint32_t addr, uint16_t mem_addr, uint16_t mem_size,
      uint8_t* data, uint32_t len, uint32_t timeout)
   {
      HAL_I2C_Mem_Read(
         &i2c_handles[id-1], addr, mem_addr, mem_size, data, len, timeout);
   }

}}}

#endif
