/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_INFINITY_ETHERNET_HPP_AUDUST_23_2019)
#define CYCFI_INFINITY_ETHERNET_HPP_AUDUST_23_2019

#include <infra/support.hpp>
#include <array>
#include <lwip/netif.h>
#include <lwip/udp.h>

namespace cycfi { namespace infinity
{
   struct ip_address : ip4_addr_t
   {
      constexpr ip_address(int a0, int a1, int a2, int a3)
      {
         IP4_ADDR(this, a0, a1, a2, a3);
      }

       constexpr ip_address(ip4_addr_t other)
       : ip4_addr_t(other)
      {}
   };

   ip_address const ip_add_any{ *IP_ADDR_ANY };

   ////////////////////////////////////////////////////////////////////////////
   class net_interface : non_copyable
   {
   public:

                  net_interface(ip_address ip, ip_address netmask, ip_address gateway);
      void        process();

   private:

      netif       _netif;
   };

   ////////////////////////////////////////////////////////////////////////////
   class net_buffer;

   class udp_server : non_copyable
   {
   public:
                     udp_server();
                     ~udp_server();

      void           bind(ip_address addr, std::uint16_t port);
      void           connect(ip_address addr, std::uint16_t port);
      void           disconnect();

      void           send(net_buffer const& buff);
      virtual void   on_receive(net_buffer const& buff, ip_address addr, std::uint16_t port) = 0;

   private:

      static void    receive(void* arg, udp_pcb* upcb, pbuf* p, ip_addr_t const* addr, u16_t port);

      udp_pcb*       _upcb = nullptr;
   };

   ////////////////////////////////////////////////////////////////////////////
   class net_buffer
   {
   public:
                  net_buffer(net_buffer const& rhs);
                  net_buffer(char* data, std::size_t len);
                  ~net_buffer();

      operator    bool() const { return _buff; }
      char*       data() const { return reinterpret_cast<char*>(_buff->payload); }
      std::size_t size() const { return _buff->len; }

   private:

      friend class udp_server;

               net_buffer(pbuf* buff_);

      pbuf*    _buff;
   };

   ////////////////////////////////////////////////////////////////////////////
   // Inlines
   inline void udp_server::connect(ip_address addr, std::uint16_t port)
   {
      udp_connect(_upcb, &addr, port);
   }

   inline void udp_server::disconnect()
   {
      udp_disconnect(_upcb);
   }

   inline void udp_server::send(net_buffer const& buff)
   {
      udp_send(_upcb, buff._buff);
   }

   inline net_buffer::net_buffer(net_buffer const& rhs)
    : net_buffer(rhs.data(), rhs.size())
   {
   }

   inline net_buffer::net_buffer(char* data, std::size_t len)
    : _buff(pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM))
   {
      if (_buff)
         pbuf_take(_buff, data, _buff->len);
   }

   inline net_buffer::~net_buffer()
   {
      pbuf_free(_buff);
   }

   inline net_buffer::net_buffer(pbuf* buff_)
    : _buff(buff_)
   {
   }
}}

#endif
