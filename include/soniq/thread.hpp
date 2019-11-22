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

   class thread
   {
   public:

      template <typename F>
      thread(
         F&& f
       , char const* name = nullptr
       , std::size_t stack_size = 512
       , thread_priority priority = thread_priority::normal
      )
       : _f(std::forward<F>(f))
      {
         osThreadAttr_t attr;
         memset(&attr, 0, sizeof(attr));
         attr.name = name;
         attr.stack_size = stack_size;
         attr.priority = osPriority_t(priority);
         // attr.attr_bits = osThreadJoinable;
         _id = osThreadNew(&call, this, &attr);
      }

      void join()
      {
         osThreadJoin(_id);
      }

   private:

      static void call(void* argument);

      std::function<void()>   _f;
      osThreadId_t            _id;
   };
}

#endif
