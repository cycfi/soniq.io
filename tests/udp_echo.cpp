/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <soniq/support.hpp>
#include <soniq/pin.hpp>
#include <soniq/app.hpp>
#include <soniq/ethernet.hpp>

namespace snq = cycfi::soniq;

///////////////////////////////////////////////////////////////////////////////
class echo_server : public snq::udp_server
{
public:

   virtual void on_receive(inf::net_buffer const& buff, snq::ip_address addr, std::uint16_t port)
   {
      snq::net_buffer tx_buff{buff};
      if (tx_buff)
      {
         connect(addr, port);    // Connect to remote client same port
         send(tx_buff);          // Tell the client that we have accepted it
         disconnect();           // Free the UDP connection, so we can accept new clients
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
int main()
{
   snq::system_init();

   snq::net_interface net{
      {192, 168, 254, 100} // ip
    , {255, 255, 255, 0}   // netmask
    , {192, 168, 254, 1}   // gateway
   };

   echo_server server;
   server.bind(inf::ip_add_any, 7); // bind to any address, port 7

   snq::delay_ms(1000);

   while (true)
   {
      net.process();
//      snq::delay_ms(1);
   }
}


