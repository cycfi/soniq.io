/*=============================================================================
   Copyright (c) 2014-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <inf/support.hpp>
#include <inf/pin.hpp>
#include <inf/app.hpp>


///////////////////////////////////////////////////////////////////////////////

#include "inf/detail/lwip.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#define UDP_SERVER_PORT    7
/* UDP remote connection port */
#define UDP_CLIENT_PORT    7

void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

void udp_echoserver_init()
{
   struct udp_pcb *upcb;
   err_t err;

   /* Create a new UDP control block  */
   upcb = udp_new();

   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);

      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, udp_echoserver_receive_callback, NULL);
      }
      else
      {
        udp_remove(upcb);
      }
   }
}

void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
  struct pbuf *p_tx;

  /* allocate pbuf from RAM*/
  p_tx = pbuf_alloc(PBUF_TRANSPORT,p->len, PBUF_RAM);

  if(p_tx != NULL)
  {
    pbuf_take(p_tx, (char*)p->payload, p->len);
    /* Connect to the remote client */
    udp_connect(upcb, addr, UDP_CLIENT_PORT);

    /* Tell the client that we have accepted it */
    udp_send(upcb, p_tx);

    /* free the UDP connection, so we can accept new clients */
    udp_disconnect(upcb);

    /* Free the p_tx buffer */
    pbuf_free(p_tx);

    /* Free the p buffer */
    pbuf_free(p);
  }
}

static void Ethernet_GPIO_Init()
{
   // GPIO Ports Clock Enable
   __HAL_RCC_GPIOH_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();
   __HAL_RCC_GPIOG_CLK_ENABLE();
}

///////////////////////////////////////////////////////////////////////////////
// Simplest toggle the led test. This test uses simple delay to toggle
// the leds at a rate of 1 per second. No setup required.
///////////////////////////////////////////////////////////////////////////////

namespace inf = cycfi::infinity;

///////////////////////////////////////////////////////////////////////////////
int main()
{
   inf::system_init();

   Ethernet_GPIO_Init();
   MX_LWIP_Init();
   HAL_Delay(1000);

   udp_echoserver_init();

   while (true)
   {
      MX_LWIP_Process();
      HAL_Delay(1);
   }
}


