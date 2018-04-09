#include "apptask.h"
#include "errors.h"
#include "socket.hpp"
#include "systimer.h"
#include "tn_dhcp.h"
#include "tn_net_func.h"
#include "tn_net_mem_func.h"
#include "tn_net_utils.h"
#include "tn_tcp.h"


/* глобальное определение для tn net */
TN_NET* g_tnet_addr = &socket::net;


const net_proto_timer_func socket::g_net_proto_timer_functions[] =
{
  ip_slowtimo,                        // IP4 reassembly timeout timer

#if (TN_ARP != 0)
  arp_timer_func,                     // ARP timer
#endif

#if (TN_TCP != 0)
  tcp_timer_func,                     // TCP timer tick 100 ms - for TCP timers
#endif

#if (TN_DHCP != 0)
  dhcp_timer_func,                    // DHCP timer tick 100 ms - for DHCP timers
#endif

  0
};


TN_NETINFO socket::netinfo = {0, 0};
TN_NET     socket::net     = {0};
unsigned char socket::mempool[] = {0};


int socket::init_network(TN_NETIF* nif)
{
  socket::net.spl_holder_task = 0;

  socket::net.ip_sem.id_sem = 0;
  tn_sem_create(&socket::net.ip_sem, 1, 1);

  socket::net.spl_sem.id_sem = 0;
  tn_sem_create(&socket::net.spl_sem, 1, 1);

  socket::net.mem_hibuf_mpool     = (unsigned int*)&(mempool[0]);
  socket::net.mem_drv_lobuf_mpool = (unsigned int*)&(mempool[TNNET_HI_BUF_SIZE    * NUM_HI_BUF]);
  socket::net.mem_drv_m1buf_mpool = (unsigned int*)&(mempool[TNNET_SMALL_BUF_SIZE * NUM_DRV_SMALL_BUF]);

  //-------------------------------------------------------------
  int rc = tn_net_init_mem_pools(&socket::net);
  if(rc != TERR_NO_ERR)
    return rc;

  tn_srand(&socket::net.rnd_gen, (unsigned char*)nif->drv_addr());
  ip_init (&socket::net);
  udp_init(&socket::net);
  tcp_init(&socket::net);

  socket::netinfo.tnet  = &socket::net;
  socket::netinfo.ni[0] = nif;

  socket::net.task_net_timer.id_task = 0;
  socket::net.task_net_timer_stack   = new unsigned int[APPSTACK_TCPIP];
  tn_task_create(&socket::net.task_net_timer,                             // TCB
                 socket::net_timer,                                       // work function
                 APPPRIO_TCPIP,                                           // priority
                 &(socket::net.task_net_timer_stack[APPSTACK_TCPIP - 1]), // stack begin
                 APPSTACK_TCPIP,                                          // stack size
                 (void*)&socket::netinfo,                                 // work function parameter
                 TN_TASK_START_ON_CREATION);                              // start on creation

  socket::net.tcb.inp_next = socket::net.tcb.inp_prev = &socket::net.tcb; // active tcpcb's queue head

  socket::net.netif_default = nif;
  socket::net.netif_list    = nif;

  return nif->drv_init(&socket::net, nif);
}

void socket::net_timer(void* param)
{
  int i = 0;
  static TN_NETINFO* hwni = (TN_NETINFO*)param;
  static unsigned int cnt = 0;
  net_proto_timer_func timer_func = 0;

  for(;;)
  {
    cnt++;

    for(i = 0; ; i++)
    {
      timer_func = g_net_proto_timer_functions[i];

      if(timer_func)
        timer_func(hwni, cnt);
      else                                // 0 - function's array last element
        break;
    }

    tn_task_sleep(100 / SYSTIMER_PERIOD); // 100ms delay
  }
}


socket::socket(int domain, int type, int protocol)
{
  this->hsock = s_socket(domain, type, protocol);
}

socket::~socket()
{
  s_close(this->hsock);
}

int socket::bind(const struct _sockaddr* name, int namelen)
{
  return s_bind(this->hsock, name, namelen);
}

int socket::connect(struct _sockaddr* name, int namelen)
{
  return s_connect(this->hsock, name, namelen);
}

int socket::accept(struct _sockaddr* name, int* namelen)
{
  return s_accept(this->hsock, name, namelen);
}

int socket::listen(int backlog)
{
  return s_listen(this->hsock, backlog);
}

int socket::recv(unsigned char* buf, int bufsz, int flags)
{
  return s_recv(this->hsock, buf, bufsz, flags);
}

int socket::recvfrom(unsigned char* buf, int bufsz, int flags, struct _sockaddr* from, int* fromlen)
{
  return s_recvfrom(this->hsock, buf, bufsz, flags, from, fromlen);
}

int socket::send(unsigned char* buf, int bufsz, int flags)
{
  return s_send(this->hsock, buf, bufsz, flags);
}

int socket::sendto(unsigned char* buf, int bufsz, int flags, struct _sockaddr* to, int tolen)
{
  return s_sendto(this->hsock, buf, bufsz, flags, to, tolen);
}

int socket::shutdown(int how)
{
  return s_shutdown(this->hsock, how);
}

int socket::getpeername(struct _sockaddr* name, int* namelen)
{
  return s_getpeername(this->hsock, name, namelen);
}

int socket::setsockopt(int name, unsigned char* val, int valsz)
{
  return s_setsockopt(this->hsock, 0, name, val, valsz);
}

int socket::getsockopt(int name, unsigned char* val, int* valsz)
{
  return s_getsockopt(this->hsock, 0, name, val, valsz);
}
