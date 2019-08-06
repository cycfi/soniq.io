/*=============================================================================
   Copyright (c) 2015-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/detail/i2c_impl.hpp>
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"

namespace cycfi { namespace infinity { namespace detail
{
   I2C_HandleTypeDef i2c_handles[3];

   // I2C SPEEDCLOCK define to max value: 400 KHz
   static auto constexpr i2c_clock_speed = 400000;
   static auto constexpr i2c_duty_cycle = I2C_DUTYCYCLE_2;

   void setup_i2c_pin(GPIO_TypeDef& gpio, std::uint32_t pin_mask)
   {
      LL_GPIO_SetPinMode(&gpio, pin_mask, LL_GPIO_MODE_ALTERNATE);

      if (pin_mask > LL_GPIO_PIN_7)
         LL_GPIO_SetAFPin_8_15(&gpio, pin_mask, LL_GPIO_AF_4);
      else
         LL_GPIO_SetAFPin_0_7(&gpio, pin_mask, LL_GPIO_AF_4);

      LL_GPIO_SetPinSpeed(&gpio, pin_mask, LL_GPIO_SPEED_FREQ_HIGH);
      LL_GPIO_SetPinOutputType(&gpio, pin_mask, LL_GPIO_OUTPUT_OPENDRAIN);
      LL_GPIO_SetPinPull(&gpio, pin_mask, LL_GPIO_PULL_UP);
   }

   void i2c_config(
      std::size_t id,
      GPIO_TypeDef& scl_gpio, std::uint32_t scl_pin_mask,
      GPIO_TypeDef& sda_gpio, std::uint32_t sda_pin_mask
   )
   {
      auto& handle = i2c_handles[id-1];
      switch (id-1)
      {
           case 0: handle.Instance = I2C1; 	break;
           case 1: handle.Instance = I2C2; 	break;
           case 2: handle.Instance = I2C3; 	break;
      }
      handle.Init.ClockSpeed = i2c_clock_speed;
      handle.Init.DutyCycle = i2c_duty_cycle;
      handle.Init.OwnAddress1 = 0;
      handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
      handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
      handle.Init.OwnAddress2 = 0;
      handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
      handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

      // Configure SDA and SCL Pins
      setup_i2c_pin(scl_gpio, scl_pin_mask);
      setup_i2c_pin(sda_gpio, sda_pin_mask);

      // Peripheral clock enable
      switch (id-1)
      {
         case 0: __HAL_RCC_I2C1_CLK_ENABLE(); 	break;
         case 1: __HAL_RCC_I2C2_CLK_ENABLE(); 	break;
         case 2: __HAL_RCC_I2C3_CLK_ENABLE(); 	break;
      }

      HAL_I2C_Init(&handle);
   }
}}}
