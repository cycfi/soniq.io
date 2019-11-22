/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <soniq/support.hpp>
#include <soniq/pin.hpp>
#include <soniq/app.hpp>
#include <soniq/thread.hpp>

///////////////////////////////////////////////////////////////////////////////
// We toggle the 3 onboard LEDs at different rates using two threads plus the
// main thread. No setup required.
///////////////////////////////////////////////////////////////////////////////

namespace snq = cycfi::soniq;
using namespace snq::port;

///////////////////////////////////////////////////////////////////////////////
int main()
{
   auto led1 = out<snq::led1>();
   auto led2 = out<snq::led2>();
   auto led3 = out<snq::led3>();

   auto f1 = snq::thread(
      [&]
      {
         while (true)
         {
            snq::delay_ms(1000);
            led1 = !led1;
         }
      }
   );

   auto f2 = snq::thread(
      [&]
      {
         while (true)
         {
            snq::delay_ms(500);
            led2 = !led2;
         }
      }
   );

   while (true)
   {
      snq::delay_ms(250);
      led3 = !led3;
   }
}


