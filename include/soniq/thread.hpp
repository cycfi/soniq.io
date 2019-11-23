/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_THREAD_HPP_NOVEMBER_22_2019)
#define CYCFI_INFINITY_THREAD_HPP_NOVEMBER_22_2019

#include <cmsis_os.h>
#include <functional>
#include <cstring>

namespace cycfi::soniq
{
   enum class thread_priority
   {
      none               = osPriorityNone,
      idle               = osPriorityIdle,
      low                = osPriorityLow,
      low1               = osPriorityLow1,
      low2               = osPriorityLow2,
      low3               = osPriorityLow3,
      low4               = osPriorityLow4,
      low5               = osPriorityLow5,
      low6               = osPriorityLow6,
      low7               = osPriorityLow7,
      below_normal       = osPriorityBelowNormal,
      below_normal1      = osPriorityBelowNormal1,
      below_normal2      = osPriorityBelowNormal2,
      below_normal3      = osPriorityBelowNormal3,
      below_normal4      = osPriorityBelowNormal4,
      below_normal5      = osPriorityBelowNormal5,
      below_normal6      = osPriorityBelowNormal6,
      below_normal7      = osPriorityBelowNormal7,
      normal             = osPriorityNormal,
      normal1            = osPriorityNormal1,
      normal2            = osPriorityNormal2,
      normal3            = osPriorityNormal3,
      normal4            = osPriorityNormal4,
      normal5            = osPriorityNormal5,
      normal6            = osPriorityNormal6,
      normal7            = osPriorityNormal7,
      above_normal       = osPriorityAboveNormal,
      above_normal1      = osPriorityAboveNormal1,
      above_normal2      = osPriorityAboveNormal2,
      above_normal3      = osPriorityAboveNormal3,
      above_normal4      = osPriorityAboveNormal4,
      above_normal5      = osPriorityAboveNormal5,
      above_normal6      = osPriorityAboveNormal6,
      above_normal7      = osPriorityAboveNormal7,
      high               = osPriorityHigh,
      high1              = osPriorityHigh1,
      high2              = osPriorityHigh2,
      high3              = osPriorityHigh3,
      high4              = osPriorityHigh4,
      high5              = osPriorityHigh5,
      high6              = osPriorityHigh6,
      high7              = osPriorityHigh7,
      realtime           = osPriorityRealtime,
      realtime1          = osPriorityRealtime1,
      realtime2          = osPriorityRealtime2,
      realtime3          = osPriorityRealtime3,
      realtime4          = osPriorityRealtime4,
      realtime5          = osPriorityRealtime5,
      realtime6          = osPriorityRealtime6,
      realtime7          = osPriorityRealtime7,
      isr                = osPriorityISR,
      error              = osPriorityError,
      reserved           = osPriorityReserved
   };

   ////////////////////////////////////////////////////////////////////////////
   // Threads
   ////////////////////////////////////////////////////////////////////////////
   class thread
   {
   public:

      constexpr static auto default_stack_size = 512;

                        template <typename F>
                        thread(
                           F&& f
                         , char const* name = nullptr
                         , std::size_t stack_size = default_stack_size
                         , thread_priority priority = thread_priority::normal
                        );

                        thread(thread const&) = delete;

      char const*       name() const;
      void              priority(thread_priority priority);
      thread_priority   priority() const;

      void              suspend();
      void              resume();
      void              terminate();
      static void       yield();

   private:

      void              init(
                           char const* name
                         , std::size_t stack_size
                         , thread_priority priority
                        );
      static void       call(void* argument);

      using thread_function = std::function<void()>;

      thread_function   _f;
      osThreadId_t      _id;
   };

   ////////////////////////////////////////////////////////////////////////////
   // Inlines
   ////////////////////////////////////////////////////////////////////////////
   template <typename F>
   inline thread::thread(
      F&& f
    , char const* name
    , std::size_t stack_size
    , thread_priority priority
   )
    : _f(std::forward<F>(f))
   {
      init(name, stack_size, priority);
   }

   inline char const* thread::name() const
   {
      return osThreadGetName(_id);
   }

   inline void thread::priority(thread_priority priority)
   {
      osThreadSetPriority(_id, osPriority_t(priority));
   }

   inline thread_priority thread::priority() const
   {
      return thread_priority(osThreadGetPriority(_id));
   }

   inline void thread::yield()
   {
      osThreadYield();
   }

   inline void thread::suspend()
   {
      osThreadSuspend(_id);
   }

   inline void thread::resume()
   {
      osThreadResume(_id);
   }

   inline void thread::terminate()
   {
      osThreadTerminate(_id);
   }
}

#endif
