/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_PIN_IMPL_HPP_DECEMBER_20_2015)
#define CYCFI_INFINITY_PIN_IMPL_HPP_DECEMBER_20_2015

#include <cstddef>
#include <cstdint>

#if defined(STM32H743xx)
# include <stm32h7xx.h>
# include <stm32h7xx_ll_gpio.h>
# include <stm32h7xx_ll_bus.h>
# include <stm32h7xx_ll_system.h>
# include <stm32h7xx_ll_exti.h>
#elif defined(STM32F446xx)
# include <stm32f4xx.h>
# include <stm32f4xx_ll_gpio.h>
# include <stm32f4xx_ll_bus.h>
# include <stm32f4xx_ll_system.h>
# include <stm32f4xx_ll_exti.h>
#endif

namespace cycfi { namespace infinity { namespace detail
{
   ////////////////////////////////////////////////////////////////////////////
   // The ports: We provide template functions for getting the memory mapped
   // ports given a constant N. That way, we can use generic programming.
   //
   // This implementation is for the STM32F4 series.
   ////////////////////////////////////////////////////////////////////////////

   template <std::size_t port>
   GPIO_TypeDef& get_port();

   template <std::size_t port>
   void enable_port_clock();

   template <std::size_t const bit>
   constexpr std::uint32_t exti_id()
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

   template <std::size_t const bit>
   constexpr std::uint32_t exti_src()
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
         case 16: return LL_EXTI_LINE_16;
         case 17: return LL_EXTI_LINE_17;
         case 18: return LL_EXTI_LINE_18;
         case 19: return LL_EXTI_LINE_19;
         case 20: return LL_EXTI_LINE_20;
         case 21: return LL_EXTI_LINE_21;
         case 22: return LL_EXTI_LINE_22;
         case 23: return LL_EXTI_LINE_23;
         case 24: return LL_EXTI_LINE_24;
         case 25: return LL_EXTI_LINE_25;
         case 26: return LL_EXTI_LINE_26;
         case 26: return LL_EXTI_LINE_27;
         case 28: return LL_EXTI_LINE_28;
         case 29: return LL_EXTI_LINE_29;
         case 30: return LL_EXTI_LINE_30;
         case 31: return LL_EXTI_LINE_31;

#if defined(STM32H743xx)
         case 32: return LL_EXTI_LINE_32;
         case 33: return LL_EXTI_LINE_33;
         case 34: return LL_EXTI_LINE_34;
         case 35: return LL_EXTI_LINE_35;
         case 36: return LL_EXTI_LINE_36;
         case 37: return LL_EXTI_LINE_37;
         case 38: return LL_EXTI_LINE_38;
         case 39: return LL_EXTI_LINE_39;
         case 40: return LL_EXTI_LINE_40;
         case 41: return LL_EXTI_LINE_41;
         case 42: return LL_EXTI_LINE_42;
         case 43: return LL_EXTI_LINE_43;
         case 44: return LL_EXTI_LINE_44;
      // case 45: return LL_EXTI_LINE_45;
         case 46: return LL_EXTI_LINE_46;
         case 47: return LL_EXTI_LINE_47;
         case 48: return LL_EXTI_LINE_48;
         case 49: return LL_EXTI_LINE_49;
         case 50: return LL_EXTI_LINE_50;
         case 51: return LL_EXTI_LINE_51;
         case 52: return LL_EXTI_LINE_52;
         case 53: return LL_EXTI_LINE_53;
         case 54: return LL_EXTI_LINE_54;
         case 55: return LL_EXTI_LINE_55;
         case 56: return LL_EXTI_LINE_56;
         case 57: return LL_EXTI_LINE_57;
         case 58: return LL_EXTI_LINE_58;
         case 59: return LL_EXTI_LINE_59;
         case 60: return LL_EXTI_LINE_60;
         case 50: return LL_EXTI_LINE_50;
         case 61: return LL_EXTI_LINE_61;
         case 62: return LL_EXTI_LINE_62;
         case 63: return LL_EXTI_LINE_63;
         case 64: return LL_EXTI_LINE_64;
         case 65: return LL_EXTI_LINE_65;
         case 66: return LL_EXTI_LINE_66;
         case 67: return LL_EXTI_LINE_67;
         case 68: return LL_EXTI_LINE_68;
         case 69: return LL_EXTI_LINE_69;
         case 71: return LL_EXTI_LINE_71;
         case 72: return LL_EXTI_LINE_72;
         case 73: return LL_EXTI_LINE_73;
         case 74: return LL_EXTI_LINE_74;
         case 75: return LL_EXTI_LINE_75;
         case 76: return LL_EXTI_LINE_76;
         case 77: return LL_EXTI_LINE_77;
         case 78: return LL_EXTI_LINE_78;
         case 79: return LL_EXTI_LINE_79;
      // case 81: return LL_EXTI_LINE_81;
         case 82: return LL_EXTI_LINE_82;
      // case 83: return LL_EXTI_LINE_83;
         case 84: return LL_EXTI_LINE_84;
         case 85: return LL_EXTI_LINE_85;
         case 86: return LL_EXTI_LINE_86;
         case 87: return LL_EXTI_LINE_87;
#endif
      }
   }

   template <std::size_t const bit>
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

   template <std::size_t port>
   std::size_t exti_port();

#if defined(STM32H743xx)
# define EnableClock(x) LL_AHB4_GRP1_EnableClock(x)
# define PERIPH_GPIO(x) LL_AHB4_GRP1_PERIPH_GPIO##x
# define PERIPH_SYSCFG LL_APB4_GRP1_PERIPH_SYSCFG
#elif defined(STM32F446xx)
# define EnableClock(x) LL_AHB1_GRP1_EnableClock(x)
# define PERIPH_GPIO(x) LL_AHB1_GRP1_PERIPH_GPIO##x
# define PERIPH_SYSCFG LL_APB2_GRP1_PERIPH_SYSCFG
#endif

#define INFINITY_IOPORT(N, PORT_NAME)                                          \
   template <>                                                                 \
   inline void enable_port_clock<N>()                                          \
   {                                                                           \
      EnableClock(PERIPH_GPIO(PORT_NAME));                                     \
   }                                                                           \
                                                                               \
   template <>                                                                 \
   inline GPIO_TypeDef& get_port<N>()                                          \
   {                                                                           \
      return *GPIO##PORT_NAME;                                                 \
   }                                                                           \
                                                                               \
   template <>                                                                 \
   constexpr std::size_t exti_port<N>()                                        \
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

#if defined(STM32H743xx)
#ifdef GPIOI
  INFINITY_IOPORT(9, J)
#endif
#ifdef GPIOI
  INFINITY_IOPORT(10, K)
#endif
#endif

}}}

#endif
