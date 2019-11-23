/*=============================================================================
   Copyright (c) 2015-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <soniq/thread.hpp>

namespace cycfi::soniq
{
   void thread::call(void* arg)
   {
      auto fp = reinterpret_cast<thread*>(arg);
      if (fp && fp->_f)
         fp->_f();
   }

   void thread::init(
      char const* name
    , std::size_t stack_size
    , thread_priority priority
   )
   {
      osThreadAttr_t attr;
      memset(&attr, 0, sizeof(attr));
      attr.name = name;
      attr.stack_size = stack_size;
      attr.priority = osPriority_t(priority);
      _id = osThreadNew(&call, this, &attr);
   }
}
