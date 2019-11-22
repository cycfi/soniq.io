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
}
