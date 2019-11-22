/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_PIN_IMPL_H7_HPP_DECEMBER_20_2015)
#define CYCFI_INFINITY_PIN_IMPL_H7_HPP_DECEMBER_20_2015

#include <cstddef>
#include <cstdint>

# include <stm32h7xx.h>
# include <stm32h7xx_ll_gpio.h>
# include <stm32h7xx_ll_bus.h>
# include <stm32h7xx_ll_system.h>
# include <stm32h7xx_ll_exti.h>

namespace cycfi::soniq::detail
{
   ////////////////////////////////////////////////////////////////////////////
   // The ports: We provide template functions for getting the memory mapped
   // ports given a constant ID. That way, we can use generic programming.
   //
   // This implementation is for the STM32F4 series.
   ////////////////////////////////////////////////////////////////////////////

   template <uint32_t port>
   GPIO_TypeDef& get_port();

   template <uint32_t port>
   void enable_port_clock();

   template <uint32_t const bit>
   constexpr uint32_t exti_id()
   {
      switch (bit)
      {
         case 0:  return LL_SYSCFG_EXTI_LINE0;
         case 1:  return LL_SYSCFG_EXTI_LINE1;
         case 2:  return LL_SYSCFG_EXTI_LINE2;
         case 3:  return LL_SYSCFG_EXTI_LINE3;
         case 4:  return LL_SYSCFG_EXTI_LINE4;
         case 5:  return LL_SYSCFG_EXTI_LINE5;
         case 6:  return LL_SYSCFG_EXTI_LINE6;
         case 7:  return LL_SYSCFG_EXTI_LINE7;
         case 8:  return LL_SYSCFG_EXTI_LINE8;
         case 9:  return LL_SYSCFG_EXTI_LINE9;
         case 10: return LL_SYSCFG_EXTI_LINE10;
         case 11: return LL_SYSCFG_EXTI_LINE11;
         case 12: return LL_SYSCFG_EXTI_LINE12;
         case 13: return LL_SYSCFG_EXTI_LINE13;
         case 14: return LL_SYSCFG_EXTI_LINE14;
         case 15: return LL_SYSCFG_EXTI_LINE15;
      }
   }

   template <uint32_t const bit>
   constexpr uint32_t exti_src()
   {
      switch (bit)
      {
         case 0:  return LL_EXTI_LINE_0;
         case 1:  return LL_EXTI_LINE_1;
         case 2:  return LL_EXTI_LINE_2;
         case 3:  return LL_EXTI_LINE_3;
         case 4:  return LL_EXTI_LINE_4;
         case 5:  return LL_EXTI_LINE_5;
         case 6:  return LL_EXTI_LINE_6;
         case 7:  return LL_EXTI_LINE_7;
         case 8:  return LL_EXTI_LINE_8;
         case 9:  return LL_EXTI_LINE_9;
         case 10: return LL_EXTI_LINE_10;
         case 11: return LL_EXTI_LINE_11;
         case 12: return LL_EXTI_LINE_12;
         case 13: return LL_EXTI_LINE_13;
         case 14: return LL_EXTI_LINE_14;
         case 15: return LL_EXTI_LINE_15;
      }
   }

   template <uint32_t const bit>
   constexpr IRQn_Type exti_irq()
   {
      switch (bit)
      {
         case 0:  return EXTI0_IRQn;
         case 1:  return EXTI1_IRQn;
         case 2:  return EXTI2_IRQn;
         case 3:  return EXTI3_IRQn;
         case 4:  return EXTI4_IRQn;
         case 5:  return EXTI9_5_IRQn;
         case 6:  return EXTI9_5_IRQn;
         case 7:  return EXTI9_5_IRQn;
         case 8:  return EXTI9_5_IRQn;
         case 9:  return EXTI9_5_IRQn;
         case 10: return EXTI15_10_IRQn;
         case 11: return EXTI15_10_IRQn;
         case 12: return EXTI15_10_IRQn;
         case 13: return EXTI15_10_IRQn;
         case 14: return EXTI15_10_IRQn;
         case 15: return EXTI15_10_IRQn;
      }
   }

   template <uint32_t port>
   uint32_t exti_port();

# define EnableClock(x) LL_AHB4_GRP1_EnableClock(x)
# define PERIPH_GPIO(x) LL_AHB4_GRP1_PERIPH_GPIO##x

   inline void enable_exti_clock()
   {
      LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SYSCFG);
   }

#define INFINITY_IOPORT(ID, PORT_NAME)                                         \
   template <>                                                                 \
   inline void enable_port_clock<ID>()                                         \
   {                                                                           \
      EnableClock(PERIPH_GPIO(PORT_NAME));                                     \
   }                                                                           \
                                                                               \
   template <>                                                                 \
   inline GPIO_TypeDef& get_port<ID>()                                         \
   {                                                                           \
      return *GPIO##PORT_NAME;                                                 \
   }                                                                           \
                                                                               \
   template <>                                                                 \
   constexpr uint32_t exti_port<ID>()                                          \
   {                                                                           \
      return LL_SYSCFG_EXTI_PORT##PORT_NAME;                                   \
   }                                                                           \
   /***/

#ifdef GPIOA
  INFINITY_IOPORT(0, A)
#endif
#ifdef GPIOB
  INFINITY_IOPORT(1, B)
#endif
#ifdef GPIOC
  INFINITY_IOPORT(2, C)
#endif
#ifdef GPIOD
  INFINITY_IOPORT(3, D)
#endif
#ifdef GPIOE
  INFINITY_IOPORT(4, E)
#endif
#ifdef GPIOF
  INFINITY_IOPORT(5, F)
#endif
#ifdef GPIOG
  INFINITY_IOPORT(6, G)
#endif
#ifdef GPIOH
  INFINITY_IOPORT(7, H)
#endif
#ifdef GPIOI
  INFINITY_IOPORT(8, I)
#endif
#ifdef GPIOJ
  INFINITY_IOPORT(9, J)
#endif
#ifdef GPIOK
  INFINITY_IOPORT(10, K)
#endif

   template <uint32_t port, uint32_t mask, uint32_t speed, uint32_t type>
   inline void init_output_pin()
   {
      // Enable GPIO peripheral clock
      enable_port_clock<port>();

      // Configure output mode
      LL_GPIO_SetPinMode(&get_port<port>(), mask, LL_GPIO_MODE_OUTPUT);

      // Configure output push pull or open drain
      LL_GPIO_SetPinOutputType(&get_port<port>(), mask, type);

      // Configure output speed
      LL_GPIO_SetPinSpeed(&get_port<port>(), mask, speed);

      // Configure pull-up/down resistor
      LL_GPIO_SetPinPull(&get_port<port>(), mask, LL_GPIO_PULL_NO);
   }

   template <uint32_t port, uint32_t mask, uint32_t type>
   inline void init_input_pin()
   {
      // Enable GPIO peripheral clock
      enable_port_clock<port>();

      // Configure input mode
      LL_GPIO_SetPinMode(&get_port<port>(), mask, LL_GPIO_MODE_INPUT);

      // Configure pull-up/down resistor
      LL_GPIO_SetPinPull(&get_port<port>(), mask, type);
   }

   template <uint32_t port, uint32_t bit>
   inline void enable_exti(uint32_t priority = 0)
   {
      // Connect External Line to the GPIO
      enable_exti_clock();
      LL_SYSCFG_SetEXTISource(exti_port<port>(), exti_id<bit>());

      // Configure NVIC to handle external interrupt
      NVIC_SetPriority(exti_irq<bit>(), priority);
      NVIC_EnableIRQ(exti_irq<bit>());
   }

   template <uint32_t port, uint32_t bit>
   inline void start_exti(bool rising)
   {
      LL_EXTI_EnableIT_0_31(exti_src<bit>());
      if (rising)
         LL_EXTI_EnableRisingTrig_0_31(exti_src<bit>());
      else
         LL_EXTI_EnableFallingTrig_0_31(exti_src<bit>());
   }

   template <uint32_t port, uint32_t bit>
   inline void stop_exti(bool rising)
   {
      LL_EXTI_DisableIT_0_31(exti_src<port>());
      if (rising)
         LL_EXTI_DisableRisingTrig_0_31(exti_src<port>());
      else
         LL_EXTI_DisableFallingTrig_0_31(exti_src<port>());
   }
}

#endif
