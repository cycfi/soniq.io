/*=============================================================================
   Copyright (c) 2015-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/pin.hpp>

namespace cycfi::infinity
{
   using exti_handler_array = std::array<std::function<void()>, 16>;
   std::array<std::function<void()>, 16> _exti_handlers;
}

namespace cycfi::infinity::detail
{
   template <std::size_t ID>
   void handle_exti()
   {
      static_assert(ID < 16, "Invalid EXTI ID");

      if (_exti_handlers[ID])
      {
         // Manage Flags
         if (LL_EXTI_IsActiveFlag_0_31(detail::exti_src<ID>()) != RESET)
         {
            // Clear EXTI flag
            LL_EXTI_ClearFlag_0_31(detail::exti_src<ID>());

            // Handle exti task
            _exti_handlers[ID]();
         }
      }
   }
}

extern "C"
{
   void EXTI0_IRQHandler(void)
   {
      cycfi::infinity::detail::handle_exti<0>();
   }

   void EXTI1_IRQHandler(void)
   {
      cycfi::infinity::detail::handle_exti<1>();
   }

   void EXTI2_IRQHandler(void)
   {
      cycfi::infinity::detail::handle_exti<2>();
   }

   void EXTI3_IRQHandler(void)
   {
      cycfi::infinity::detail::handle_exti<3>();
   }

   void EXTI4_IRQHandler(void)
   {
      cycfi::infinity::detail::handle_exti<4>();
   }

   void EXTI9_5_IRQHandler(void)
   {
      cycfi::infinity::detail::handle_exti<5>();
      cycfi::infinity::detail::handle_exti<6>();
      cycfi::infinity::detail::handle_exti<7>();
      cycfi::infinity::detail::handle_exti<8>();
      cycfi::infinity::detail::handle_exti<9>();
      cycfi::infinity::detail::handle_exti<10>();
   }

   void EXTI15_10_IRQHandler(void)
   {
      cycfi::infinity::detail::handle_exti<10>();
      cycfi::infinity::detail::handle_exti<11>();
      cycfi::infinity::detail::handle_exti<12>();
      cycfi::infinity::detail::handle_exti<13>();
      cycfi::infinity::detail::handle_exti<14>();
      cycfi::infinity::detail::handle_exti<15>();
   }
}