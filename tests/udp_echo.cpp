/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/pin.hpp>
#include <inf/app.hpp>
#include <inf/ethernet.hpp>

namespace inf = cycfi::infinity;

///////////////////////////////////////////////////////////////////////////////
class echo_server : public inf::udp_server
{
public:

   virtual void on_receive(inf::net_buffer const& buff, inf::ip_address addr, std::uint16_t port)
   {
      inf::net_buffer tx_buff{buff};
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
   inf::system_init();

   inf::net_interface net{
      {192, 168, 254, 100} // ip
    , {255, 255, 255, 0}   // netmask
    , {192, 168, 254, 1}   // gateway
   };

   echo_server server;
   server.bind(inf::ip_add_any, 7); // bind to any address, port 7

   inf::delay_ms(1000);

   while (true)
   {
      net.process();
      inf::delay_ms(1);
   }
}


