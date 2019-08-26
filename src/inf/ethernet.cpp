/*=============================================================================
   Copyright (c) 2015-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/ethernet.hpp>
#include <inf/detail/lwip.h>
#include <lwip/init.h>

// Exported functions
err_t ethernetif_init(netif* netif);
void ethernetif_input(netif* netif);

namespace cycfi { namespace infinity
{
   namespace detail
   {
      static void Ethernet_GPIO_Init()
      {
         // GPIO Ports Clock Enable
         __HAL_RCC_GPIOH_CLK_ENABLE();
         __HAL_RCC_GPIOC_CLK_ENABLE();
         __HAL_RCC_GPIOA_CLK_ENABLE();
         __HAL_RCC_GPIOB_CLK_ENABLE();
         __HAL_RCC_GPIOG_CLK_ENABLE();
      }
   }

   net_interface::net_interface(ip_address ip, ip_address netmask, ip_address gateway)
   {
      // Enable clocks
      detail::Ethernet_GPIO_Init();

      // Initilialize the LwIP stack without RTOS
      lwip_init();

      // add the network interface (IPv4/IPv6) without RTOS
      netif_add(&_netif, &ip, &netmask, &gateway, nullptr, &ethernetif_init, &ethernet_input);

      // Registers the default network interface
      netif_set_default(&_netif);

      if (netif_is_link_up(&_netif))
      {
         // When the netif is fully configured this function must be called
         netif_set_up(&_netif);
      }
      else
      {
         // When the netif link is down this function must be called
         netif_set_down(&_netif);
      }
   }

   void net_interface::process()
   {
      ethernetif_input(&_netif);
      sys_check_timeouts();
   }

   udp_server::udp_server()
    : _upcb(udp_new())
   {
   }

   udp_server::~udp_server()
   {
      if (!_upcb)
         udp_remove(_upcb);
   }

   void udp_server::receive(void* arg, udp_pcb* upcb, pbuf* p, ip_addr_t const* addr, u16_t port)
   {
      net_buffer buff{p};
   }

   void udp_server::bind(ip_address addr, std::uint16_t port)
   {
      if (_upcb)
      {
         err_t err = udp_bind(_upcb, &addr, port);

         if (err == ERR_OK)
         {
            // Set a receive callback for the upcb
            udp_recv(_upcb, receive, this);
         }
         else
         {
            udp_remove(_upcb);
            _upcb = nullptr;
         }
      }
   }
}}
