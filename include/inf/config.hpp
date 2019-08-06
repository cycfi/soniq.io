/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_CONFIG_HPP_SEPTEMBER_11_2019)
#define CYCFI_INFINITY_CONFIG_HPP_SEPTEMBER_11_2019

#include <type_traits>
#include <inf/support.hpp>

namespace cycfi { namespace infinity
{
   ///////////////////////////////////////////////////////////////////////////////
   // Application configuration makes sure there are no peripheral conflicts
   // and attaches IRQ tasks, if there are any. The config variable should
   // be a global variable in global scope.
   //
   // Example:
   //
   //    auto config = inf::config(
   //       tmr = timer_task
   //    );
   //
   ///////////////////////////////////////////////////////////////////////////////

   struct irq_not_handled {};
   struct peripheral_base_id {};

   struct peripheral_base
   {
      using id = peripheral_base_id;
      irq_not_handled operator()(...)
      {
         return irq_not_handled{};
      }
   };

   template <typename T, typename ID>
   struct has_unique_id
   {
      static bool const value =
         has_unique_id<typename T::base_type, ID>::value &&
         !std::is_same<typename T::id, ID>::value
         ;
   };

   template <typename ID>
   struct has_unique_id<peripheral_base, ID>
   {
      static bool const value = true;
   };

   template <typename ID, typename Base>
   struct basic_config : Base
   {
      using id = ID;
      using base_type = Base;
      using Base::operator();

      static_assert(has_unique_id<base_type, id>::value,
         "Error. Duplicate peripheral IDs.");

      basic_config(Base base)
       : Base(base)
      {}
   };

   template <typename ID, typename Base>
   inline basic_config<ID, Base> make_basic_config(Base base)
   {
      return {base};
   }

   template <typename ID, typename Base, typename F>
   struct task_config : Base
   {
      using id = ID;
      using base_type = Base;
      using Base::operator();

      static_assert(has_unique_id<base_type, id>::value,
         "Error. Duplicate peripheral IDs.");

      task_config(Base base, F task)
       : Base(base)
       , task(task)
      {}

      void operator()(identity<ID> const&)
      {
         task();
      }

      void operator()(ID const&)
      {
         task();
      }

      F task;
   };

   template <typename ID, typename Base, typename F>
   inline task_config<ID, Base, F>
   make_task_config(Base base, F task)
   {
      return {base, task};
   }

   template <typename T>
   inline auto config(T cfg)
   {
      return cfg(peripheral_base{});
   }

   template <typename T, typename... Rest>
   inline auto config(T cfg, Rest... rest)
   {
      return cfg(config(rest...));
   }
}}

#endif
