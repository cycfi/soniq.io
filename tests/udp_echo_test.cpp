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
// UDP Echo server test. Connect an ethernet cable from the dev board to a
// network switch, router, or directly to the computer's Gigabit Ethernet
// port (1000BASE-T), assuming it has Auto MDI-X (100BASE-T and lower
// connections typically do not have Auto MDI-X and require crossover cables
// while practically all 1000BASE-T interfaces support it). Run an echo
// client program in the host computer and connect to this echo server's port
// at 192, 168, 254, 100, port 7 to test. Here's a sample session using
// Boost.Asio's echo test example (see http://bit.ly/35vxcWs):
//
//    echo 192.168.254.100 7
//    Enter message: hello
//    Reply is: hello
//
// If there's a conflict, adjust the host IP in the net_interface below.
///////////////////////////////////////////////////////////////////////////////

class echo_server : public snq::udp_server
{
public:

   virtual void on_receive(snq::net_buffer const& buff, snq::ip_address addr, std::uint16_t port)
   {
      snq::net_buffer tx_buff{ buff };
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
   snq::net_interface net{
      { 192, 168, 254, 100 }  // ip
    , { 255, 255, 255, 0 }    // netmask
    , { 192, 168, 254, 1 }    // gateway
   };

   echo_server server;
   server.bind(snq::ip_add_any, 7); // bind to any address, port 7

   // Delay a bit to have the network interface stabilize
   snq::delay_ms(1000);

   while (true)
   {
      net.process();
   }
}


